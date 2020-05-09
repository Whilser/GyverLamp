void webserver() {
  
  http = new(ESP8266WebServer);
  httpUpdater = new(ESP8266HTTPUpdateServer);
  httpUpdater->setup(http);
  
  if (http !=NULL) {

    http->onNotFound(routeNotFound);

    /** главная */
    http->on("/", routeHome);
    
    /** прием конфигурации */
    http->on("/setconfig", routeSetConfig); 
    
    /** получить текущие настройки/конфигурацию */
    http->on("/getconfig", routeGetConfig); 
  
    /** страница настройка будильника */
    http->on("/alarm", routeAlarm); 
    
    http->on("/weather", routeWeather); 

    /** прием конфигурации будильника */
    http->on("/setalarmconfig", routeSetAlarmConfig); 
    
    /** получить текущие настройки/конфигурацию будильника */
    http->on("/getalarmconfig", routeGetAlarmConfig);

    /** stub for favicon  **/
    http->on("/favicon.ico", []() {
      http->send(404, F("text/plain"), F("none"));
    });

    http->begin();
    
    Serial.printf("Launched web server at: http://%s.local/\r\n", clientId.c_str());
    
  } else {
    
    Serial.println("Error starting web server. \r\n");
  }
  
}

/**
 * шаблон/отправщик страницы
 */
void responseHtml(String out, String title = "AlexGyver Lamp", int code = 200) {

  #ifdef WEBAUTH
  if (!http->authenticate(clientId.c_str(), clientId.c_str())) {
      return http->requestAuthentication();
  }
  #endif
  
  String html;
  
  html = "<html>";
    html += "<head>";
      html += "<title>" + title + "</title>";
      html += "<meta http-equiv=\"refresh\" content=\"50\" >";
      html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1\" />";
      html += "<link rel=\"stylesheet\" href=\"https://demos.jquerymobile.com/1.4.5/css/themes/default/jquery.mobile-1.4.5.min.css\">";
      html += "<link rel=\"stylesheet\" href=\"http://fonts.googleapis.com/css?family=Open+Sans:300,400,700\">";
      html += "<script src=\"https://demos.jquerymobile.com/1.4.5/js/jquery.js\"></script>";
      html += "<script src=\"https://demos.jquerymobile.com/1.4.5/js/jquery.mobile-1.4.5.min.js\"></script>";
    html += "</head>";
    html += "<body>";
      html += "<div data-role='page'>";
        html += "<div data-role='header' class='jqm-header'><h2><a class='ui-link' data-ajax='false' href='/'><img style='width: 100%' src='//i0.wp.com/alexgyver.ru/wp-content/uploads/2018/07/site_mob-1.png'></a></h2></div><!-- /header -->";
        html += "<div role='main' class='ui-content jqm-content' style='padding-bottom: 100px;'>";
        html += "";
        html += out;
        html += "";
        html += "</div>";
        html += "<div data-role='footer' data-theme='b' style='position: fixed;width: 100%;bottom: 0;z-index: 1;'>";
            html += "<div data-role='navbar' data-iconpos='bottom'>";
                 #ifdef ENG
                html += "<ul>";
                    html += "<li><a href='/' data-ajax='false' data-icon='gear'>Basic settings</a></li>"; // сдлеать активной class='ui-btn-active'
                    html += "<li><a href='/alarm' data-ajax='false' data-icon='clock'>Alarm clock</a></li>";
                    html += "<li><a href='/weather' data-ajax='false' data-icon='cloud'>Weather settings</a></li>";
                    html += "<!--<li><a href='/timer' data-ajax='false' data-icon='power'>Schedule</a></li>-->";
                html += "</ul>";
                #else  
                html += "<ul>";
                    html += "<li><a href='/' data-ajax='false' data-icon='gear'>Основные настройки</a></li>"; // сдлеать активной class='ui-btn-active'
                    html += "<li><a href='/alarm' data-ajax='false' data-icon='clock'>Будильник</a></li>";
                    html += "<!--<li><a href='/timer' data-ajax='false' data-icon='power'>Расписание</a></li>-->";
                html += "</ul>";
                #endif
                
            html += "</div>"; // .navbar
        html += "</div>"; // .footer
      html += "</div>"; // .page

      // js функция отправки/получения данных формы
      html += "    <script type=\"text/javascript\">\n";
      html += "    function syncConfig(getconfig = '/getconfig', setconfig = '/setconfig'){\n";
      html += "        $.ajax({url: getconfig, dataType:'json', success: init});\n";
      html += "        function init(config){\n";
      html += "            /**\n";
      html += "             * костыль/фича\n";
      html += "             * лень было искать как нормально установить параметы в виджеты\n";
      html += "             * пока не установятся параметры из ESP, отправка не будет осуществляться\n";
      html += "             * @type {boolean}\n";
      html += "             */\n";
      html += "            window.changeReaction = false;\n";
      html += "                let timeouts = {};\n";
      html += "                $('select, input').on('change',(v) => {\n";
      html += "                    /** если данные не пришли, ничего не отправляем/сохраненяем */\n";
      html += "                    if(window.changeReaction === false) return;\n";
      html += "                    let that = $(v.currentTarget), name = that.attr('name'), outData = {};\n";
      html += "                    /** если в очереди на отправку/сохранение есть такой параметр, то снимим предыдущее значение */\n";
      html += "                    if(timeouts[name] != undefined)\n";
      html += "                        clearTimeout(timeouts[name]);\n";
      html += "                    /**\n";
      html += "                     * установим измененный параметр в очередь на отправку/сохранение в ESP\n";
      html += "                     * @type {number}\n";
      html += "                     */\n";
      html += "                    timeouts[name] = setTimeout(() => {\n";
      html += "                        window.changeReaction = false;\n";
      html += "                        outData[name] = that.val();\n";
      html += "                        $.ajax({\n";
      html += "                            url:setconfig,\n";
      html += "                            data: outData,\n";
      html += "                            dataType: 'json',\n";
      html += "                            success: (json) => {\n";
      html += "                                if(json[name] = outData[name])\n";
      html += "                                    window.changeReaction = true;\n";
      html += "                                else\n";
      html += "                                    alert('Не удалось сохранить настройки.');\n";
      html += "                                setConfig(json);\n";
      html += "                            },\n";
      html += "                            error: (event) => alert(`Не удалось сохранить настройки.\nПроизошла ошибка \"${event.status} ${event.statusText}\".`)\n";
      html += "                        });\n";
      html += "                    }, 500);\n";
      html += "                });\n";
      html += "                setConfig(config);\n";
      html += "                /** установим актуальные параметры из ESP */\n";
      html += "                function setConfig(config){\n";
      html += "                  window.changeReaction = false;\n";
      html += "                  for (let key in config){\n";
      html += "                      let that = $(`[name=${key}]`);\n";
      html += "                      that.val(config[key]);\n";
      html += "                      that.trigger('change');\n";
      html += "                  }\n";
      html += "                  /**\n";
      html += "                   * разрешаем вносить изменеия в конфигу\n";
      html += "                   * @type {boolean}\n";
      html += "                   */\n";
      html += "                  window.changeReaction = true;\n";
      html += "                }\n";
      html += "        }\n";
      html += "    }\n";
      html += "    </script>\n";
      
    html += "</body>";
  html += "</html>";
  
  http->sendHeader("Cache-Control","max-age=0, private, must-revalidate");
  http->send(code, "text/html; charset=utf-8", html); 
}

/**
 * исключение/вывод ошибки о не найденном пути
 */
void routeNotFound() {
  String out;
  
  out = "Path not found";
  out += "<br />URI: ";
  out += http->uri();
  out += "<br />Method: ";
  out += (http->method() == HTTP_GET) ? "GET" : "POST";
  out += "<br />Arguments: ";
  out += http->args();
  out += "<br /><pre>";
  for (uint8_t i = 0; i < http->args(); i++) {
    out += " " + http->argName(i) + ": " + http->arg(i) + "<br />";
  }
  out += "</pre><hr /><a class='ui-link' data-ajax='false' href=\"/\">Перейти на главную</a>";
  responseHtml(out, "Error 404", 404);
}

/**
 * отправка текущей конфигурации 
 * + отправка JSON(обязательно должен завершаться запятой)
 */
void routeGetConfig() {

  #ifdef WEBAUTH
  if (!http->authenticate(clientId.c_str(), clientId.c_str())) {
      return http->requestAuthentication();
  }
  #endif

  String out;

  out += "{";
  out += "\"status\": \"ok\",";
  out += "\"value\": " + String(modes[currentMode].brightness) + ",";
  out += "\"currentMode\": " + String(currentMode) + ",";
  out += "\"brightness\": " + String(modes[currentMode].brightness) + ",";
  out += "\"speed\": " + String(modes[currentMode].speed) + ",";
  out += "\"scale\": " + String(modes[currentMode].scale) + ",";
  out += "\"on\": " + String(ONflag);
  out += "}";
  
  http->send(200, "text/json", out);
}

/**
 * изменение/применение новой конфигурации
 */
void routeSetConfig() {

  #ifdef WEBAUTH
  if (!http->authenticate(clientId.c_str(), clientId.c_str())) {
      return http->requestAuthentication();
  }
  #endif
    
  if (http->hasArg("currentMode")) {
    
    String value;

    value = http->arg("currentMode");
    currentMode =  value.toInt();

   if (currentMode == 29) {

      demo = true;
      currentMode = random(0, MODE_AMOUNT-1);      
    } else {

      demo = false;
      currentMode =  value.toInt();
      if (currentMode >= MODE_AMOUNT || currentMode < 0) currentMode = 0;
    }

    manualOff = true;
    dawnFlag = false;
    settChanged = true;
    saveEEPROM();
    loadingFlag = true;
    FastLED.clear();
    delay(1);
    sendCurrent();
    FastLED.setBrightness(modes[currentMode].brightness);
    
  }
  
  if(http->hasArg("scale")){

    byte scale = http->arg("scale").toInt();
    if (currentMode == 17 && scale > 100) scale = 100;
    
    modes[currentMode].scale = scale;
    loadingFlag = true;
    settChanged = true;
    eepromTimer = millis();

  }
  
  if(http->hasArg("brightness")){
    modes[currentMode].brightness = http->arg("brightness").toInt();
    FastLED.setBrightness(modes[currentMode].brightness);

    sendCurrent();
    settChanged = true;
    eepromTimer = millis();

  }
  
  if(http->hasArg("speed")){
    modes[currentMode].speed = http->arg("speed").toInt();
    loadingFlag = true;
    settChanged = true;
    eepromTimer = millis();

  }
  
  if(http->hasArg("r")){
    r = http->arg("r").toInt();

  }

  if(http->hasArg("g")){
    g = http->arg("g").toInt();

  }

  if(http->hasArg("b")){
    b = http->arg("b").toInt();

  }

  if (http->hasArg("on")) {
    
    ONflag = (http->arg("on").toInt() > 0) ? true : false;
    settChanged = true;
    changePower();
    sendCurrent();
    
  }

/** в знак завершения операции отправим текущую конфигурацию */
  routeGetConfig();
  MQTTUpdateState();
  
}

void routeWeather(){
  if (http->hasArg("Forecast")) {
    forecast = http->arg("Forecast");
    addr = WEATHER_SETTINGS_ADDR;
    eeprom_write_string(forecast);  
    msg = "Save Successfull!";
    request_weather();
  }
  String out = "<form>";
  out += "Forecast:  <select name=\"Forecast\">";
  out += "<option " + String((forecast == "0")?"selected ":"") + "value=\"0\">Now</option>";
  out += "<option " + String((forecast == "6")?"selected ":"") + "value=\"6\">+6  hours</option>";
  out += "<option " + String((forecast == "12")?"selected ":"") + "value=\"12\">+12 hours</option>";
//  out += "<option " + String((forecast == "18")?"selected ":"") + "value=\"18\">+18 hours</option>";
//  out += "<option " + String((forecast == "24")?"selected ":"") + "value=\"24\">+24 hours</option>";
//  out += "<option " + String((forecast == "48")?"selected ":"") + "value=\"48\">+2  days</option>";
//  out += "<option " + String((forecast == "72")?"selected ":"") + "value=\"72\">+3  days</option>";
//  out += "<option " + String((forecast == "96")?"selected ":"") + "value=\"96\">+4  days</option>";
  out += "</select>";
  
  out += "<br><br><input type='submit' name='SUBMIT' value='Save'></form>";
  out += "<br>";
  out += "The forecast in " + forecast + " hours : " + weatherString;
  out += "<br>";
  out += msg + "<br><br>";
  responseHtml(out);

}


void routeAlarm(){
  #ifdef ENG
  String out, days[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
  #else
  String out, days[] = {"пн","вт","ср","чт","пт","сб","вс"};
  #endif

  out = "<form>";
    for (byte i = 0; i < 7; i++) {
      out += "<div class='ui-field-contain'>";  
          out += "<label for='day_" + String(i) + "'>"+days[i]+"</label>";
          out += "<select name='day_" + String(i) + "' id='day_" + String(i) + "' data-role='slider' data-mini='true'>";
              out += "<option value='0'></option>";
              out += "<option value='1'></option>";
          out += "</select>";
      out += "</div>";
      #ifdef ENG
      out += "<div class='ui-field-contain'><label for='time_" + String(i) + "'>Time</label><input name='time_" + String(i) + "' id='time_" + String(i) + "' type='time' value='00:00' /></div>";
      #else
      out += "<div class='ui-field-contain'><label for='time_" + String(i) + "'>время</label><input name='time_" + String(i) + "' id='time_" + String(i) + "' type='time' value='00:00' /></div>";
      #endif
    }
    
    out += "<div class='ui-field-contain'>";
      #ifdef ENG
      out += "<label for='dawnMode'>Dawn for:</label>";
      #else
      out += "<label for='dawnMode'>Рассвета за:</label>";
      #endif
      out += "<select name='dawnMode' id='dawnMode'>";
      for(byte i = 0; i <= sizeof(dawnOffsets) - 1; i++){
       out += "<option value='" + String(i) + "'>" + String(dawnOffsets[i]) + "</option>"; 
      }
      out += "</select>";
    out += "</div>";
  out += "</form>";
  
  out += "<script type='text/javascript'>$(document).ready(()=>{syncConfig('/getalarmconfig','/setalarmconfig');});</script>";
  
  responseHtml(out);
}

void routeSetAlarmConfig(){
  
  for (byte i = 0; i < 7; i++) {
    
    if(http->hasArg("day_"+String(i))){
      alarm[i].state = (http->arg("day_" + String(i)).toInt() > 0);
      saveAlarm(i);
      settChanged = true;
    }
    if(http->hasArg("time_"+String(i))){
      
      alarm[i].time = http->arg("time_" + String(i)).substring(0,2).toInt() * 60 + http->arg("time_" + String(i)).substring(3,5).toInt();
      saveAlarm(i);
      settChanged = true;
    }
  }
  
  if(http->hasArg("dawnMode")){
    dawnMode = http->arg("dawnMode").toInt();
    saveDawnMmode();
    settChanged = true;
  }
  
  routeGetAlarmConfig();
  
}


void routeGetAlarmConfig() {

  #ifdef WEBAUTH
  if (!http->authenticate(clientId.c_str(), clientId.c_str())) {
      return http->requestAuthentication();
  }
  #endif
  
  String out = "{";
  int _time;
    
  for (byte i = 0; i < 7; i++) {
    out += (alarm[i].state == true) ? "\"day_"+String(i)+"\":\"1\"," : "\"day_"+String(i)+"\":\"0\",";
    if(alarm[i].time){
      String h,m;
      h = (alarm[i].time/60<10) ? "0" + String(alarm[i].time/60) : String(alarm[i].time/60);
      m = (alarm[i].time%60<10) ? "0" + String(alarm[i].time%60) : String(alarm[i].time%60);
      out += "\"time_"+String(i)+"\":\""+h+":"+m+"\",";
    }else{
      out += "\"time_"+String(i)+"\":\""+timeClient.getHours()+":"+timeClient.getMinutes()+"\",";
    }
  }

  out += "\"dawnMode\":\"" + String(dawnMode) + "\"}";
  
  http->send(200, "text/json", out);
}

/**
 * главная страница
 */
void routeHome(){
  
  String out;

  #ifdef ENG
  
  out = "<form>";

      out += "<div class='ui-field-contain'>";
        out += "<label for='on'>Power:</label>";
        out += "<select name='on' id='on' data-role='slider' data-mini='true'>";
          out += "<option value='0'>Off</option>";
          out += "<option value='1'>On</option>";
        out += "</select>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='currentMode'>Mode:</label>";
        out += "<select name='currentMode' id='currentMode' data-mini='true'>";
          
          out += "<option value='0'>Confetti</option>";
          out += "<option value='1'>Fire</option>";
          out += "<option value='2'>Rainbow vertical</option>";
          out += "<option value='3'>Rainbow horizontal</option>";
          out += "<option value='4'>Color change</option>";
          out += "<option value='5'>3D Madness</option>";
          out += "<option value='6'>3D clouds</option>";
          out += "<option value='7'>3D lava</option>";
          out += "<option value='8'>3D plasma</option>";
          out += "<option value='9'>3D rainbow</option>";
          out += "<option value='10'>3D peacock</option>";
          out += "<option value='11'>3D zebra</option>";
          out += "<option value='12'>3D forest</option>";
          out += "<option value='13'>3D ocean</option>";
          out += "<option value='14'>Color</option>";
          out += "<option value='15'>Snowfall</option>";
          out += "<option value='16'>Matrix</option>";
          out += "<option value='17'>Fireflies</option>";
          out += "<option value='18'>Aquarium</option>";
          out += "<option value='19'>Starfall</option>";
          out += "<option value='20'>Paintball</option>";
          out += "<option value='21'>Spiral</option>";
          out += "<option value='22'>Warm Light</option>";
          out += "<option value='23'>Pendulum</option>";
          out += "<option value='24'>Twinkles</option>";
          out += "<option value='25'>Police Strobo</option>";
          out += "<option value='26'>Incremental Drift Rose</option>";
          out += "<option value='27'>Pride</option>";
          out += "<option value='28'>Weather mode</option>";
          out += "<option value='29'>Demo</option>";
          
        out += "</select>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='brightness'>Brightness:</label>";
        out += "<input type='range' name='brightness' id='brightness' value='50' min='1' max='255' data-highlight='true'>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='speed'>Speed:</label>";
        out += "<input type='range' name='speed' id='speed' value='50' min='0' max='255' data-highlight='true'>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='scale'>Scale:</label>";
        out += "<input type='range' name='scale' id='scale' value='50' min='0' max='100' data-highlight='true'>";
      out += "</div>";

  out += "</form>";
  out += "<script type='text/javascript'>$(()=>{syncConfig('/getconfig','/setconfig');});</script>";
  out += "<br>";
  out += getTimeStampString();
  

  #else
  
  out = "<form>";

      out += "<div class='ui-field-contain'>";
        out += "<label for='on'>Питание лампы:</label>";
        out += "<select name='on' id='on' data-role='slider' data-mini='true'>";
          out += "<option value='0'>Выкл</option>";
          out += "<option value='1'>Вкл</option>";
        out += "</select>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='currentMode'>Режим:</label>";
        out += "<select name='currentMode' id='currentMode' data-mini='true'>";
          
          out += "<option value='0'>Конфетти</option>";
          out += "<option value='1'>Огонь</option>";
          out += "<option value='2'>Радуга вертикальная</option>";
          out += "<option value='3'>Радуга горизонтальная</option>";
          out += "<option value='4'>Смена цвета</option>";
          out += "<option value='5'>Безумие 3D</option>";
          out += "<option value='6'>Облака 3D</option>";
          out += "<option value='7'>Лава 3D</option>";
          out += "<option value='8'>Плазма 3D</option>";
          out += "<option value='9'>Радуга 3D</option>";
          out += "<option value='10'>Павлин 3D</option>";
          out += "<option value='11'>Зебра 3D</option>";
          out += "<option value='12'>Лес 3D</option>";
          out += "<option value='13'>Океан 3D</option>";
          out += "<option value='14'>Цвет</option>";
          out += "<option value='15'>Снегопад</option>";
          out += "<option value='16'>Матрица</option>";
          out += "<option value='17'>Светлячки</option>";
          out += "<option value='18'>Аквариум</option>";
          out += "<option value='19'>Звездопад</option>";
          out += "<option value='20'>Пейнтбол</option>";
          out += "<option value='21'>Спираль</option>";
          out += "<option value='22'>Теплый свет</option>";
          out += "<option value='23'>Маятник</option>";
          out += "<option value='24'>Мерцание</option>";
          out += "<option value='25'>Полицейская сирена</option>";
          out += "<option value='26'>Инкрементная дрифт-роз</option>";
          out += "<option value='27'>гордость</option>";
          out += "<option value='28'>Weather mode</option>";
          out += "<option value='29'>Демо</option>";
          
        out += "</select>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='brightness'>Яркость:</label>";
        out += "<input type='range' name='brightness' id='brightness' value='50' min='1' max='255' data-highlight='true'>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='speed'>Скорость:</label>";
        out += "<input type='range' name='speed' id='speed' value='50' min='0' max='255' data-highlight='true'>";
      out += "</div>";
      
      out += "<div class='ui-field-contain'>";
        out += "<label for='scale'>Масштаб:</label>";
        out += "<input type='range' name='scale' id='scale' value='50' min='0' max='100' data-highlight='true'>";
      out += "</div>";
      
    
  out += "</form>";
  out += "<script type='text/javascript'>$(()=>{syncConfig('/getconfig','/setconfig');});</script>";
  out += "<br>";
  out += getTimeStampString();

  #endif

  responseHtml(out);
}
