![PROJECT_PHOTO](https://github.com/AlexGyver/GyverLamp/blob/master/proj_img.jpg)
# Крутая WiFi лампа на esp8266 своими руками
* [Описание проекта](#chapter-0)
* [Папки проекта](#chapter-1)
* [Схемы подключения](#chapter-2)
* [Материалы и компоненты](#chapter-3)
* [Как скачать и прошить](#chapter-4)
* [Настройка интеграции с HomeAssistant](#chapter-5)
* [Настройка интеграции с Home Bridge и Apple Home Kit](#chapter-6)
* [Настройка интеграции с Яндекс Алисой](#chapter-7)
* [Настройка интеграции с Node-Red](#chapter-8)
* [Управляющие топики MQTT (для интеграции с системами домашней автоматизации)](#chapter-9)
* [FAQ](#chapter-10)
* [Полезная информация](#chapter-11)
[![AlexGyver YouTube](http://alexgyver.ru/git_banner.jpg)](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)

<a id="chapter-0"></a>
## Описание проекта
Представляю вашему вниманию светильник на адресных светодиодах с кучей эффектов, управлением по Wi-Fi и функцией будильник-рассвет!
### Железо
- Проект собран на базе микроконтроллера ESP8266 в лице платы NodeMCU или Wemos D1 mini (неважно, какую из этих плат использовать!).
- Вместо адресной ленты используется гибкая адресная матрица 16×16, что выходит дешевле ленты (матрица 16×16 стоит 1500р, она состоит из 256 диодов с плотностью 100 штук на метр. Лента такой же плотности стоит 1000р за метр (за 100 светодиодов). Для склейки матрицы размером 16×16 понадобится 2.5 метра ленты, то есть 2500р. А готовая матрица стоит на 1000р дешевле!).
- Система управляется со смартфона по Wi-Fi (приложение GyverLamp для Android), а также “оффлайн” с кнопки на корпусе (сенсорная кнопка на TTP223).

### Фишки
- 14 крутых эффектов
- Настройка скорости, яркости и “масштаба” для каждого эффекта
- Настройка эффектов со смартфона
- Работа системы как в локальной сети, так и в режиме “точки доступа”
- Встроенный Wi-Fi менеджер для удобной настройки сети
- Система получает точное время из Интернета
- Управление кнопкой: смена режима, настройка яркости, вкл/выкл
- Режим будильник-рассвет: менеджер будильников на неделю в приложении

### Корпус
- Корпус выглядит очень презентабельно, несмотря на простоту и доступность материалов
- Рассеиватель – матовый плафон из Леруа Мерлен
- Остальные элементы корпуса – канализационные трубы, в лучших традициях жанра!
- Страница проекта на сайте: https://alexgyver.ru/GyverLamp/

<a id="chapter-1"></a>
## Папки
**ВНИМАНИЕ! Если это твой первый опыт работы с Arduino, читай [инструкцию](#chapter-4)**
- **libraries** - библиотеки проекта. Заменить имеющиеся версии
- **firmware** - прошивки для Arduino
- **schemes** - схемы подключения компонентов

<a id="chapter-2"></a>
## Схемы
![SCHEME](https://github.com/AlexGyver/GyverLamp/blob/master/schemes/scheme1.jpg)
![SCHEME](https://github.com/AlexGyver/GyverLamp/blob/master/schemes/scheme2.jpg)

<a id="chapter-3"></a>
## Материалы и компоненты
### Ссылки оставлены на магазины, с которых я закупаюсь уже не один год
* NodeMCU http://ali.ski/RgD5P  http://ali.ski/_1FJZ
* Wemos D1 mini http://ali.ski/FuTgbO  http://ali.ski/Z9feWU
* Матрица 16x16 http://ali.ski/BCKQT  http://ali.ski/bRW14  http://ali.ski/X-tBrQ
* Адресная лента (для DIY матрицы) http://ali.ski/2dmOe_  http://ali.ski/rqgqdq  http://ali.ski/4Ma9iH
* Сенсорная кнопка http://ali.ski/aWQBAa  http://ali.ski/rsOrSB
* БП 5V (брать 3A минимум) http://ali.ski/K-CThT  http://ali.ski/3UWXJ
* Проводочки http://ali.ski/JQRler  http://ali.ski/_SuCF
* Плафон https://leroymerlin.ru/product/plafon-cilindr-18212968/

## Вам скорее всего пригодится
* [Всё для пайки (паяльники и примочки)](http://alexgyver.ru/all-for-soldering/)
* [Недорогие инструменты](http://alexgyver.ru/my_instruments/)
* [Все существующие модули и сенсоры Arduino](http://alexgyver.ru/arduino_shop/)
* [Электронные компоненты](http://alexgyver.ru/electronics/)
* [Аккумуляторы и зарядные модули](http://alexgyver.ru/18650/)

<a id="chapter-4"></a>
## Как скачать и прошить
* [Первые шаги с Arduino](http://alexgyver.ru/arduino-first/) - ультра подробная статья по началу работы с Ардуино, ознакомиться первым делом!
* Скачать архив с проектом
> На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**
* Установить библиотеки в  
`C:\Program Files (x86)\Arduino\libraries\` (Windows x64)  
`C:\Program Files\Arduino\libraries\` (Windows x86)
* **Подключить внешнее питание 5 Вольт**
* Подключить плату к компьютеру
* Файл/Настройки, вставить ссылку http://arduino.esp8266.com/stable/package_esp8266com_index.json в в текст-бокс для дополнительных ссылок для менеджера плат
* Открыть Инструменты/Плата/Менеджер плат…, найти esp8266 by ESP8266 Community, установить версию 2.5.0
* Открыть Инструменты, настроить Плату, Порт и другие параметры как на скриншоте
* Загрузить прошивку


## Настройки в коде
    // -------- ВРЕМЯ -------
    GMT 3              // смещение (москва 3)
    NTP_ADDRESS  "europe.pool.ntp.org"    // сервер времени

    // -------- РАССВЕТ -------
    DAWN_BRIGHT 200       // макс. яркость рассвета
    DAWN_TIMEOUT 1        // сколько рассвет светит после времени будильника, минут

    // ---------- МАТРИЦА ---------
    BRIGHTNESS 40         // стандартная маскимальная яркость (0-255)
    CURRENT_LIMIT 2000    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

    WIDTH 16              // ширина матрицы
    HEIGHT 16             // высота матрицы

    COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

    MATRIX_TYPE 0         // тип матрицы: 0 - зигзаг, 1 - параллельная
    CONNECTION_ANGLE 0    // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
    STRIP_DIRECTION 0     // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
    // при неправильной настрйоке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
    // шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/

    // --------- ESP --------
    ESP_MODE 1
    // 0 - точка доступа (192.168.4.1 или другой)
    // 1 - локальный (192.168.1.232 или другой)

    // -------- Менеджер WiFi ---------
    AC_SSID "AutoConnectAP"
    AC_PASS "12345678"

    // -------------- AP ---------------
    AP_SSID "GyverLamp"
    AP_PASS "12345678"
    AP_PORT 8888
	
<a id="chapter-5"></a>
## Настройка интеграции с HomeAssistant
### Добавление ползунков Scale, Speed в Home Assistant

# в configuration.yaml
```
input_number:
  speed:
    name: Speed
    initial: 30
    min: 0
    max: 255
    step: 1

  scale:
    name: Scale
    initial: 30
    min: 1
    max: 100
    step: 1
```
# в automations.yaml
```
- alias: Gyver Speed
  trigger:
    platform: state
    entity_id: input_number.speed
    
  action:
    - service: mqtt.publish
      data_template:
        topic: homeassistant/light/ESP-3dc22b/effect/speed/set
        payload_template: "{{ trigger.to_state.state | int }}"
    
- alias: Gyver Scale
  trigger:
    platform: state
    entity_id: input_number.scale
    
  action:
    - service: mqtt.publish
      data_template:
        topic: homeassistant/light/ESP-3dc22b/effect/scale/set
        payload_template: "{{ trigger.to_state.state | int }}"

```

**3dc22b - заменить на ID чипа ESP, он будет виден в названии лампы и в entity_id в конце строки после знака подчеркивания.
Настройка MQTT сервера появится в wi-fi менеджере при первом запуске. Нужно указать адрес MQTT сервера, имя пользователя и пароль для доступа к вашему MQTT серверу.**

<a id="chapter-6"></a>
## Настройка интеграции с Home Bridge и Apple Home Kit

Для управления из Home Bridge необходимо установить плагин homebridge-mqttthing:

    npm install -g homebridge-mqttthing
    
И внести в файл конфигурации некоторые изменения. Переключение эффектов в Home Bridge не предусмотрено, поэтому заведем эффекты в компонент телевизора. Ну чем панель не телевизор? :)

```
    "accessories": [
        {
            "accessory": "mqttthing",
            "type": "lightbulb",
            "name": "Gyver Lamp",
            "url": "http://127.0.01:1883",
            "mqttPubOptions": {
                "retain": false
            },
            "topics": {
                "getOn": "homeassistant/light/ESP-3bd20b/status",
                "setOn": "homeassistant/light/ESP-3bd20b/switch",
                "getBrightness": "homeassistant/light/ESP-3bd20b/brightness/status",
                "setBrightness": "homeassistant/light/ESP-3bd20b/brightness/set",
                "getRGB": "homeassistant/light/ESP-3bd20b/rgb/status",
                "setRGB": "homeassistant/light/ESP-3bd20b/rgb/set"
            },
            "onValue": "ON",
            "offValue": "OFF"
        },
        {
            "accessory": "mqttthing",
            "type": "television",
            "name": "Gyver Lamp Effects",
            "url": "http://127.0.0.1:1883",
            "topics": {
                "setActive": "homeassistant/light/ESP-3bd20b/switch",
                "getActive": "homeassistant/light/ESP-3bd20b/status",
                "setActiveInput": "homeassistant/light/ESP-3bd20b/effect/set",
                "getActiveInput": "homeassistant/light/ESP-3bd20b/effect/status"
            },
            "inputs": [
                {
                    "name": "Конфетти",
                    "value": "Конфетти"
                },
                {
                    "name": "Огонь",
                    "value": "Огонь"
                },
                {
                    "name": "Радуга верт.",
                    "value": "Радуга верт."
                },
                {
                    "name": "Радуга гориз.",
                    "value": "Радуга гориз."
                },
                {
                    "name": "Смена цвета",
                    "value": "Смена цвета"
                },
                {
                    "name": "Безумие 3D",
                    "value": "Безумие 3D"
                },
                {
                    "name": "Облака 3D",
                    "value": "Облака 3D"
                },
                {
                    "name": "Лава 3D",
                    "value": "Лава 3D"
                },
                {
                    "name": "Плазма 3D",
                    "value": "Плазма 3D"
                },
                {
                    "name": "Радуга 3D",
                    "value": "Радуга 3D"
                },
                {
                    "name": "Павлин 3D",
                    "value": "Павлин 3D"
                },
                {
                    "name": "Зебра 3D",
                    "value": "Зебра 3D"
                },
                {
                    "name": "Лес 3D",
                    "value": "Лес 3D"
                },
                {
                    "name": "Океан 3D",
                    "value": "Океан 3D"
                },
                {
                    "name": "Снегопад",
                    "value": "Снегопад"
                },
                {
                    "name": "Матрица",
                    "value": "Матрица"
                },
                {
                    "name": "Светлячки",
                    "value": "Светлячки"
                }
            ],
            "onValue": "ON",
            "offValue": "OFF"
        }
    ]

```

Здесь вместо 3bd20b нужно прописать ID чипа ESP. Как его получить - я писал выше. В итоге после перезагрузки Home Bridge просто и легко у нас в Home Kit  появится два новых устройства: RGB лампа и "телевизор", выходами которого можно переключать эффекты. 

Включение и выключение синхронизируется и в "телевизоре", в лампе, в Home assistant, в веб интерфейсе и в приложении для смартфона. Описанным выше способом можно добавить все остальные эффекты. Ползунки скорости и масштабирования если они необходимы можно добавить как отдельную лампу и управлять через уровень яркости.

<a id="chapter-7"></a>

## Настройка интеграции с Яндекс Алисой

Как же без "импортозамещения"? Сервис умного дома Яндекс хоть очень молод, можно сказать находится в стадии бета тестирования, но уже сейчас он может управлять умными устройствами вашего дома на русском языке. Для интеграции лампы Alex Gyver'а с умным домом Яндекс необходимо подключить интеграцию Домовенок Кузя и загрузить актуальную версию прошивки. На сайте предусмотрено два способов интеграции:

* [x] С помощью брокера MQTT
* [x] С помощью http запросов

Текущая версию прошивки поддерживает оба способа. Но так как не у всех есть возможность установить свой mqtt брокер, рассмотрим интеграцию через http запросы. Для удаленного управления необходимо прокинуть в роутере порт до вашей лампы. Далее необходимо настроить правила управления лампой на сайте Домовенка Кузи и связать их с виртуальной лампой. 

```
Управление включением лампы: http://login:password@ip-address:8000/setconfig?on=1
Управление выключением лампы: http://login:password@ip-address:8000/setconfig?on=0
Установка яркости лампы: http://login:password@ip-address:8000/setconfig?brightness={value}
Состояние лампы: http://login:password@ip-address:8000/getconfig
```

Если вы собираетесь использовать навык Домовенок Кузя совместно с Умным домом Яндекс, можно настроить ответ домовенка в поле "Ответ Алисы" (да, да.. логика разработчика немного страдает..) необходимо вписать:

    яркость {brightness} процентов, эффект {currentMode}
    
т.к. теперь у нас лампа будет доступна из интернет, в прошивку добавлена базовая аутентификация. Для ее включения необходимо раскоментировать строчку **#define WEBAUTH** в прошивке.  В качестве логина и пароля используется уникальный идентификатор ESP, о котором говорилось выше. Он отображается при первичной настройке в менеджере подключения. Если вы не собираетесь использовать mqtt брокер, в поле mqtt server при первичной настройке необходимо указать **none**. 
Для управления доступны также переключение эффекта, скорости и масштабирования. Управляются следующими запросами:

```
Управление переключением эффекта лампы: http://login:password@ip-address:8000/setconfig?currentMode=1
Установка скорости эффекта : http://login:password@ip-address:8000/setconfig?speed=10
Установка масштабирования эффекта : http://login:password@ip-address:8000/setconfig?scale=10
Установка цвета из RGB палитры: http://login:password@ip-address:8000/setconfig?on=1&currentMode=14&r=254&g=100&b=10
Вместо 8000 нужно указать порт, который прокинут от лампы с порта 80 в интернет. 
```

**В первый раз надо прошить, полностью стерев ESP**

После первого запуска лампа автоматически появится в списке устройств Home Assistant. С помощью MQTT интеграции можно подключить лампу не только к Home Assitant, но и к HomeBridge, ioBroker, а так же к любым системам домашней автоматизации. 

Прошивка с поддержкой MQTT лежит https://github.com/Whilser/GyverLamp/tree/master/firmware/GyverLamp_v1.4_MQTT

### Изменения относительно оригинальной прошивки:

* [x] Поддержка работы с MQTT сервером
* [x] Нативная интеграция с Home Assistant методом Discover
* [x] Возможность ОТА обновления через сетевой порт с интересным эффектом "матрица" во время обновления
* [x] Возможность выбирать цвет из палитры Home Assistant либо через MQTT сервер
* [x] Автоматическое обнаружение подключенной сенсорной кнопки и корректная работа без нее
* [x] Корректная работа в случае отключения WiFi сигнала либо MQTT брокера (адаптивное подключение)
* [x] Возможность настроить работу как с MQTT брокером, так и без него без повторной компиляции прошивки. Для этого достаточно при настройке вместо адреса MQTT сервера ввести **none**. 
* [x] Добавлен веб интерфейс управления (не свой, доработал, взял наработки CoOre, который так же создал Fork проекта на GitHub) - для управления нужно перейти по адресу http://ESP-ID.local/. Вместо ID нужно вписать ID чипа ESP8266. Он отображается в названии лампы и в entity_id после знака подчеркивания
* [x] Возможность выбирать цвет свечения лампы из RGB палитры
* [x] OTA обновление - второй раз не нужно подключать через USB для прошивки, можно обновить прошивку через сетевой порт


<a id="chapter-8"></a>

## Настройка интеграции с Node-Red

Спасибо [@cacherocks](https://github.com/cacherocks) за проделанную работу. Картинка с флоу для лампы:
![LampFlow](https://github.com/Whilser/GyverLamp/blob/master/Node-Red/glamp_nr.png)
![LampFlow](https://github.com/Whilser/GyverLamp/blob/master/Node-Red/gleff_nr.png)

Флоу для импорта в нодред находятся в папке Node-Red: https://github.com/Whilser/GyverLamp/tree/master/Node-Red.
Дополнительно необходимо установить: node-red-contrib-homekit-bridged для добавления в Homekit и node-red-contrib-color-convert для конвертации RGB в HSV и обратно. 

* [x] в mqtt нодах надо поменять ESP-267059 на свой ID
* [x] всех homekit нодах надо поменять bridge на свой
* [x] в нодах также необходимо заменить ESP-267059 на свой ID

<a id="chapter-9"></a>
### Управляющие топики MQTT (для интеграции с прочими системами домашней автоматизации)

Получение статуса лампы:

```
-- статус включено/выключено. Статус включено "ON" статус выключено "OFF"
"homeassistant/light/ESP-ID/status"

-- статус яркости от 0 до 255
"homeassistant/light/ESP-ID/brightness/status"

-- название эффекта, например "Огонь"
"homeassistant/light/ESP-ID/effect/status"

-- текущая скорость эффекта
"homeassistant/light/ESP-ID/effect/speed/status"

-- текущее значение масштабирования эффекта
"homeassistant/light/ESP-ID/effect/scale/status"

-- значение цвета в RGB палитре, например, 255,255,255 - белый цвет
"homeassistant/light/ESP-ID/rgb/status"
```

Управление лампой:

```
-- включение/выключение. Статус включено "ON" статус выключено "OFF"
"homeassistant/light/ESP-ID/switch"

-- установить яркость от 0 до 255
"homeassistant/light/ESP-ID/brightness/set"

-- включение эффекта, например "Огонь"
"homeassistant/light/ESP-ID/effect/set"

-- установить скорость эффекта
"homeassistant/light/ESP-ID/effect/speed/set"

-- установить масштабироване эффекта
"homeassistant/light/ESP-ID/effect/scale/set"

-- установить значение цвета в RGB палитре, например, 255,255,255 - белый цвет
"homeassistant/light/ESP-ID/rgb/set"

```

<a id="chapter-10"></a>

## FAQ
### Основные вопросы
В: Как скачать с этого грёбаного сайта?  
О: На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**

В: Скачался какой то файл .zip, куда его теперь?  
О: Это архив. Можно открыть стандартными средствами Windows, но думаю у всех на компьютере установлен WinRAR, архив нужно правой кнопкой и извлечь.

В: Я совсем новичок! Что мне делать с Ардуиной, где взять все программы?  
О: Читай и смотри видос http://alexgyver.ru/arduino-first/

В: Вылетает ошибка загрузки / компиляции!
О: Читай тут: https://alexgyver.ru/arduino-first/#step-5

В: Сколько стоит?  
О: Ничего не продаю.

### Вопросы по этому проекту

<a id="chapter-11"></a>
## Полезная информация
* [Мой сайт](http://alexgyver.ru/)
* [Основной YouTube канал](https://www.youtube.com/channel/UCgtAOyEQdAyjvm9ATCi_Aig?sub_confirmation=1)
* [YouTube канал про Arduino](https://www.youtube.com/channel/UC4axiS76D784-ofoTdo5zOA?sub_confirmation=1)
* [Мои видеоуроки по пайке](https://www.youtube.com/playlist?list=PLOT_HeyBraBuMIwfSYu7kCKXxQGsUKcqR)
* [Мои видеоуроки по Arduino](http://alexgyver.ru/arduino_lessons/)
