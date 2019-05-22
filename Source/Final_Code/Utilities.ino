void ShowDigitalClock(RtcDateTime nowTime)
{
  //Show on data
  u8g2.setFont(u8g_font_5x8);
  u8g2.setCursor(8, 59);
  u8g2.print(nowTime.Day());

  u8g2.drawStr( 19, 59, "/");
  u8g2.setCursor(24, 59);
  u8g2.print(nowTime.Month());
  u8g2.drawStr( 35, 59, "/");
  u8g2.setCursor(41, 59);
  u8g2.print(nowTime.Year());
  
  // Show hour and minutes
  u8g2.drawRBox(3, 4, 62, 21,2);
  u8g2.setColorIndex(0);
  u8g2.setFont(u8g2_font_fub14_tn);
  u8g2.drawStr(29, 21, ":");
  
  // Set the position of digit in case the time
  // Check if hour is less than 10
  if (nowTime.Hour() < 10)
  {
    u8g2.drawStr(3, 23, "0");
    positionHour = 16;
  }
  else positionHour = 3;
  u8g2.setCursor(positionHour, 23);
  u8g2.print(nowTime.Hour());
  
  //Hit the digit position in case the minute
  //is less than 10
  if (nowTime.Minute() < 10)
  {
    u8g2.drawStr(38, 23, "0");
    positionMinute = 51;
  }
  else positionMinute = 38;
  u8g2.setCursor(positionMinute, 23);
  u8g2.print(nowTime.Minute());
  u8g2.setColorIndex(1);
}

void Screen_2(void)
{
  u8g2.clearBuffer();
  u8g2.drawRFrame(0, 0, 128, 64, 3);
  u8g2.drawRFrame(68, 4, 55, 56, 2);
  
  ShowDigitalClock(currentTime);

  u8g2.setFont(u8g2_font_open_iconic_human_4x_t);
  u8g2.drawGlyph(80, 40, 68);
  
  u8g2.setFont(u8g_font_6x13);
  u8g2.setCursor(10, 41);
  u8g2.print(dhtData.temperature);
  //u8g2.drawXBMP(5, 25, Temperature_16Icon_width, Temperature_16Icon_height, Temperature_16Icon_bits);
  
  u8g2.setFont(u8g2_font_t0_12b_me);
  if(success)
  {
    xQueueReceive(queue_uid, &readQueue, 1000);
    Serial.print("Read queue:");
    Serial.println(readQueue);
    if (readQueue == 1) {
      u8g2.setCursor(85, 55);
      u8g2.print("OKE");
    }
    else {
      u8g2.setCursor(80, 55);
      u8g2.print("ERROR");
    }
  }
  else
  {
    u8g2.setCursor(72, 55);
    u8g2.print("SCAN TAG");
  }
  u8g2.sendBuffer();
}

void readtimeTask(RtcDateTime &nowTime) 
{
  SEMAPHORE_TAKE(xMutex_i2c, I2CDEV_TIMEOUT);
  nowTime = rtcObject.GetDateTime();    //get the time from the RTC
  SEMAPHORE_GIVE(xMutex_i2c);
  
#if defined(ENABLE_DEBUG)
  char str[20];   //declare a string as an array of chars
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d allows to print an integer to the string
      nowTime.Year(),     //get year method
      nowTime.Month(),    //get month method
      nowTime.Day(),      //get day method
      nowTime.Hour(),     //get hour method
      nowTime.Minute(),   //get minute method
      nowTime.Second()    //get second method
     );
  Serial.println(str); //print the string to the serial port
#endif
}

/**
 * pushDataToServer
 * push data to server
 * @param 
 * @return 
 */
uint8_t pushDataToServer(String DataToSend, uint8_t type, char response[])
{
  if(WiFi.status()== WL_CONNECTED)   //Check WiFi connection status
  {
    SEMAPHORE_TAKE(xMutex_post, HTTP_POST_TIMEOUT);
    HTTPClient http;   
    http.begin(String(host) + "/smartclass/systems.php");   //Specify destination for HTTP request
    if(type == TYPE_JSON) 
    {
      http.addHeader("Content-Type", "application/json");     //Specify content-type header
    }
    else
    {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header
    }
    
    int httpResponseCode = http.POST(DataToSend);                 //Send the actual POST request
    if (httpResponseCode > 0)
    {
      String response_temp = http.getString();             //Get the response to the request
      response_temp.toCharArray(response, 20);
      Serial.println(httpResponseCode);                    //Print return code
      Serial.println(response_temp);                       //Print request answer
    } 
    else
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      SEMAPHORE_GIVE(xMutex_post);
      return 0;
    }
    //Free resources
    http.end();
   SEMAPHORE_GIVE(xMutex_post);
  }
  else
  {
    Serial.println("Error in WiFi connection");
    return 0;
  }
  
  return 1;
}

uint8_t getMenuButton(void)
{
  uint8_t result;
  buttonRight.read();
  buttonLeft.read();
  buttonSelect.read();
  
  if (buttonRight.wasReleased()) 
  {
    result = 1;
  }
  else if(buttonLeft.wasReleased())
  {
    result = 2;
  }
  else if(buttonSelect.wasReleased())
  {
    result = 3;
  }
  else
  {
    result = 0;
  }

#if defined(ENABLE_DEBUG)
    char str[30];   //declare a string as an array of chars
    sprintf(str, "Status of Buttons: %d", result);
    Serial.println(str); //print the string to the serial port
#endif
  return result;
}

/**
 * triggerGetStatus
 * Set flag dhtUpdated to true for handling in loop()
 * call by Ticker getTempTimer
 */
void triggerGetStatus(void) {
  Serial.println("Trigge to get data");
  vTaskResume(getStatusTaskHandle);
}


uint8_t handleUpdateTime(void)
{ 
  u8g2.clearBuffer();
  // Show title.
  u8g2.drawBox(0, 0, 127, 13);
  u8g2.setColorIndex(0);
  u8g2.setFont(u8g2_font_helvB10_tr);
  u8g2.setCursor(25, 12);
  u8g2.print("Clock Setup");
  u8g2.setColorIndex(1);
  u8g2.sendBuffer();
  
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.setCursor(0, 20);
  u8g2.print("1.Check status wifi: ");
  u8g2.sendBuffer();
  delay(100);
  if(WiFi.status()== WL_CONNECTED)   //Check WiFi connection status
  {
    u8g2.print("OK");

    u8g2.setCursor(0, 28);
    u8g2.print("2. Config parameter of ntp time");
    u8g2.sendBuffer();
    delay(200);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;

    u8g2.setCursor(0, 36);
    u8g2.print("3. Get time from internet:");
    u8g2.sendBuffer();
    delay(500);
    if (!getLocalTime(&timeinfo)) {
      u8g2.print("ERROR");
#if defined(ENABLE_DEBUG)
      Serial.println("Failed to obtain time");
#endif
      return 0;
    }

    u8g2.print("OK");
#if defined(ENABLE_DEBUG)
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
#endif
    u8g2.setCursor(0, 44);
    u8g2.print("4. Update time to DS3231.");
    u8g2.sendBuffer();
    delay(200);
    RtcDateTime nowTime = RtcDateTime(timeinfo.tm_year%100, timeinfo.tm_mon + 1, timeinfo.tm_mday, 
                        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); //define date and time object
    rtcObject.SetDateTime(nowTime); //configure the RTC with object;
  }
  else
  {
    u8g2.print("ERROR");
    u8g2.sendBuffer();
#if defined(ENABLE_DEBUG)
    Serial.println("Error in WiFi connection");
#endif
    delay(1000);
    return 0;
  }
  delay(1000);
  return 1;
}

void handleControlDevices(void)
{
  uint8_t event = 0;
  uint8_t temp = 0;
  String sendValue;
  while (1) {
    drawMenuButton(&currentBt_state);
    event = getMenuButton();
    if (event == 1) {
      toDownButton(&currentBt_state);
    }
    else if (event == 2) {
      toUpButton(&currentBt_state);
    }
    else if (event == 3) {
      if (menu_button_list[currentBt_state.position].name != NULL &&
              menu_button_list[currentBt_state.position].type == MENU_BUTTON)
      {
        temp = menu_button_list[currentBt_state.position].state;
        menu_button_list[currentBt_state.position].state = ~temp&0x01;
        Serial.println(currentBt_state.position);
        Serial.println(menu_button_list[currentBt_state.position].state);
      }
      else {
        // Make our document be an object
        JsonObject root = doc.to<JsonObject>();
        root["Dv1"] = (uint8_t)menu_button_list[0].state;
        root["Dv2"] = (uint8_t)menu_button_list[1].state;
        root["Dv3"] = (uint8_t)menu_button_list[2].state;
        root["Dv4"] = (uint8_t)menu_button_list[3].state;
        serializeJsonPretty(root, sendValue);
        int result = pushDataToServer(sendValue, TYPE_JSON, response_data);
        break;
      }
    }
  } /* End while loop */
}


/**
 * triggerGetTemp
 * Sets flag dhtUpdated to true for handling in loop()
 * called by Ticker getTempTimer
 */
void triggerGetTemp() 
{
  if (tempTaskHandle != NULL) 
  {
    Serial.println("Trigge to push temp and humi.");
     xTaskResumeFromISR(tempTaskHandle);
  }
}
