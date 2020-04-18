/*
  Скетч к проекту "Многофункциональный RGB светильник"
  Страница проекта (схемы, описания): https://alexgyver.ru/GyverLamp/
  Исходники на GitHub: https://github.com/AlexGyver/GyverLamp/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019
  https://AlexGyver.ru/

  клон версии от whisler
  https://github.com/Whisler/GyverLamp

  адаптация под сборку на platformio
  https://github.com/vortigont/GyverLamp
*/

#include "Globals.h"
#include "gyverlamp.h"

#include <Ticker.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <GyverButton.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <Timer.h>
#include "fonts.h"

// ------------------- ТИПЫ --------------------

CRGB leds[NUM_LEDS];
WiFiUDP Udp;
Ticker tickerAlarm;       // alarm checker
Ticker tickerScroller;    // scheduler for text scroller
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);
ESP8266WebServer *http; // запуск слушателя 80 порта (эйкей вебсервер)
ESP8266HTTPUpdateServer *httpUpdater;

// ----------------- ПЕРЕМЕННЫЕ ------------------

const char AP_NameChar[] = AP_SSID;
const char WiFiPassword[] = AP_PASS;
unsigned int localPort = AP_PORT;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet
String inputBuffer;
static const byte maxDim = max(WIDTH, HEIGHT);

struct {
  byte brightness = DEFAULT_BRIGHNESS;
  byte speed = DEFAULT_SPEED;
  byte scale = DEFAULT_SCALE;
} modes[MODE_AMOUNT];

byte r = 255;
byte g = 255;
byte b = 255;

byte boot_count = 0;
bool demo = false;
bool epilepsy = false; // отключает эффект "смена цвета" в демо режиме если задано false.

struct {
  boolean state = false;
  int time = 0;
} alarm[7];

byte dawnOffsets[] = {5, 10, 15, 20, 25, 30, 40, 50, 60};
byte dawnMode;
boolean dawnFlag = false;
boolean manualOff = false;        // alarm override
boolean sendSettings_flag = false;

int8_t currentMode = 0;
boolean loadingFlag = true;
boolean ONflag = true;
uint32_t eepromTimer;
boolean settChanged = false;
// Конфетти, Огонь, Радуга верт., Радуга гориз., Смена цвета,
// Безумие 3D, Облака 3D, Лава 3D, Плазма 3D, Радуга 3D,
// Павлин 3D, Зебра 3D, Лес 3D, Океан 3D,

unsigned char matrixValue[WIDTH][HEIGHT];
String lampIP = "";
static time_t now;

WiFiClient espClient;
PubSubClient mqttclient(espClient);

// ID клиента, менять для интеграции с системами умного дома в случае необходимости
String clientId = "ESP-"+String(ESP.getChipId(), HEX);
//String clientId = "ESP-8266";

bool USE_MQTT = true; // используем  MQTT?
bool _BTN_CONNECTED = true;

struct MQTTconfig {
  char HOST[32];
  char USER[32];
  char PASSWD[32];
  char PORT[10];
};

bool shouldSaveConfig = false;

void saveConfigCallback () {
  _SPLN("should save config");
  shouldSaveConfig = true;
}

char mqtt_password[32] = "DEVS_PASSWD";
char mqtt_server[32] = "";
char mqtt_user[32] = "DEVS_USER";
char mqtt_port[10] = "1883";
byte mac[6];

ADC_MODE (ADC_VCC);

Timer *infoTimer = new Timer(TIMER_MQTT); //  время обновления статусов MQTT
Timer *demoTimer = new Timer(TIMER_DEMO); //  время переключения эффектов в "Демо" режиме

void setup() {

  // ЛЕНТА
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection( TypicalLEDStrip )*/;
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.show();

  touch.setStepTimeout(100);
  touch.setClickTimeout(500);

  Serial.begin(BAUD);
  _SPLN();

  EEPROM.begin(512);

  // читаем количество запусков
  boot_count = EEPROM.read(410);
  boot_count +=1;

  // записываем колиество перезапусков
  EEPROM.write(410, boot_count); EEPROM.commit();

  // читаем статус лампы
  ONflag = EEPROM.read(420);

  // Wi-Fi
  static WiFiEventHandler e1, e2;
  e1 = WiFi.onStationModeGotIP(onSTAGotIP);   // WiFi client gets IP event
  e2 = WiFi.onStationModeDisconnected(onSTADisconnec); // WiFi client disconnected event

  if (ESP_MODE == 0) {    // режим точки доступа
    WiFi.softAPConfig(IPAddress(IP_AP[0], IP_AP[1], IP_AP[2], IP_AP[3]),
                      IPAddress(192, 168, 4, 1),
                      IPAddress(255, 255, 255, 0));

    WiFi.softAP(AP_NameChar, WiFiPassword);
    IPAddress myIP = WiFi.softAPIP();
    _SP("Access point Mode");
    _SPLN("AP IP address: ");
    _SPLN(myIP);
    USE_MQTT = false;

  } else {  // подключаемся к роутеру

    char esp_id[32] = "";

    _SP("WiFi manager...");
    sprintf(esp_id, "<br><p> Chip ID: %s </p>", clientId.c_str());

    WiFiManager wifiManager;

    WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 32);
    WiFiManagerParameter custom_mqtt_username("user", "mqtt user", mqtt_user, 32);
    WiFiManagerParameter custom_mqtt_password("password", "mqtt_password", mqtt_password, 32);
    WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 10);
    WiFiManagerParameter custom_text_1("<br>MQTT configuration:");
    WiFiManagerParameter custom_text_2(esp_id);

    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.setDebugOutput(false);
    wifiManager.setConfigPortalTimeout(180);

    wifiManager.addParameter(&custom_text_1);
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_username);
    wifiManager.addParameter(&custom_mqtt_password);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_text_2);

    if (boot_count >= 5) {
      while (!fillString("Сброс параметров подключения!", CRGB::Red, true)) {
        delay(1); yield();
      }

      // обнуляем счетчик перезапусков
      boot_count = 0; EEPROM.write(410, boot_count); EEPROM.commit();

      if (!wifiManager.startConfigPortal()) {
         _SPLN("failed to start config Portal");
      }
    }

    if (!wifiManager.autoConnect()) {
      if (!wifiManager.startConfigPortal()) {
         _SPLN("failed to connect and hit timeout");
      }
    }

    if (shouldSaveConfig) {

      strcpy(mqtt_server, custom_mqtt_server.getValue());
      strcpy(mqtt_user, custom_mqtt_username.getValue());
      strcpy(mqtt_password, custom_mqtt_password.getValue());
      strcpy(mqtt_port, custom_mqtt_port.getValue());

      writeMQTTConfig(mqtt_server, mqtt_user, mqtt_password, mqtt_port);
      _SPLN("MQTT configuration written");
      delay(100);
    };

    _SP("connected! IP address: ");
    _SP(WiFi.localIP());
    _SP(". Signal strength: ");
    _SP(2*(WiFi.RSSI()+100));
    _SPLN("%");

    _SPLN();
    _SP("MAC: ");
    _SPLN(WiFi.macAddress());

    #ifdef _DEBUG_
    _SP("Free Heap size: ");
    _SP(ESP.getFreeHeap()/1024);
    _SPLN("Kb");
    #endif

    WiFi.setOutputPower(20);

    if (!MDNS.begin(clientId)) {
        _SPLN("Error setting up MDNS responder!");
    }

    ArduinoOTA.onStart([]() {
      _SPLN("OTA Start");
      ONflag = true;
      currentMode = 16;
      loadingFlag = true;
      FastLED.clear();
      FastLED.setBrightness(modes[currentMode].brightness);

    });

    ArduinoOTA.onEnd([]() {
      _SPLN("OTA End");  //  "Завершение OTA-апдейта"
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      effectsTick();
      _SPF("Progress: %u%%\n\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
      _SPF("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) _SPLN("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) _SPLN("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) _SPLN("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) _SPLN("Receive Failed");
      else if (error == OTA_END_ERROR) _SPLN("End Failed");
   });

    ArduinoOTA.begin();
  }

  Udp.begin(localPort);
  _SPF("UDP server on port %d\n", localPort);

  // EEPROM

  delay(50);

  if (EEPROM.read(198) != 20) {   // первый запуск
    EEPROM.write(198, 20);
    //EEPROM.commit();

    for (byte i = 0; i < MODE_AMOUNT; i++) {
      EEPROM.put(3 * i + 40, modes[i]);
      //EEPROM.commit();
    }
    for (byte i = 0; i < 7; i++) {
      EEPROM.write(5 * i, alarm[i].state);   // рассвет
      eeWriteInt(5 * i + 1, alarm[i].time);
      //EEPROM.commit();
    }
    EEPROM.write(199, 0);   // рассвет
    EEPROM.write(200, 0);   // режим

    EEPROM.commit();
  }

  for (byte i = 0; i < MODE_AMOUNT; i++) {
    EEPROM.get(3 * i + 40, modes[i]);
  }

  for (byte i = 0; i < 7; i++) {
    alarm[i].state = EEPROM.read(5 * i);
    alarm[i].time = eeGetInt(5 * i + 1);
  }

  dawnMode = EEPROM.read(199);
  currentMode = (int8_t)EEPROM.read(200);
  FastLED.setBrightness(modes[currentMode].brightness);

  // отправляем настройки
  sendSettings();

  memset(matrixValue, 0, sizeof(matrixValue));

  randomSeed(micros());

  // инициализируем NTP-клиента
  settimeofday_cb(timeset);
  configTime(TZONE, NTP_SERVER);

  webserver();
  MDNS.addService("http", "tcp", 80);

  MQTTconfig MQTTConfig = readMQTTConfig();

  if ((String(MQTTConfig.HOST) == "none") || (ESP_MODE == 0) || String(MQTTConfig.HOST).length() == 0) {

    USE_MQTT = false;
    _SPLN("Использование MQTT сервера отключено.");
  }

   _BTN_CONNECTED = !digitalRead(BTN_PIN);

  #ifdef DEBUG
  _BTN_CONNECTED ? _SPLN("Обнаружена сенсорная кнопка") : _SPLN("Cенсорная кнопка не обнаружена, управление сенсорной кнопкой отключено");
  #endif

  infoTimer->setOnTimer(&infoCallback);
  infoTimer->Start();

  demoTimer->setOnTimer(&demoCallback);
  demoTimer->Start();

}

void loop() {
  infoTimer->Update();
  demoTimer->Update();

  parseUDP();
  effectsTick();
  eepromTick();
  buttonTick();

  MDNS.update();
  http->handleClient();

  if (USE_MQTT && !mqttclient.connected()) MQTTreconnect();
  if (USE_MQTT) mqttclient.loop();

  ArduinoOTA.handle();
}

void eeWriteInt(int pos, int val) {
  byte* p = (byte*) &val;
  EEPROM.write(pos, *p);
  EEPROM.write(pos + 1, *(p + 1));
  EEPROM.write(pos + 2, *(p + 2));
  EEPROM.write(pos + 3, *(p + 3));
  EEPROM.commit();
}

int eeGetInt(int pos) {
  int val;
  byte* p = (byte*) &val;
  *p        = EEPROM.read(pos);
  *(p + 1)  = EEPROM.read(pos + 1);
  *(p + 2)  = EEPROM.read(pos + 2);
  *(p + 3)  = EEPROM.read(pos + 3);
  return val;
}

// WiFi connection callbacks
// station aquired IP
void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) {
  sntp_init();    // start ntp updates
}

// station disconnect
void onSTADisconnec(WiFiEventStationModeDisconnected event_info) {
    sntp_stop();              // disable NTP updates on station disconnect
}
