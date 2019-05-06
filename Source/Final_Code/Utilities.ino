void ShowDigitalClock(RtcDateTime nowTime)
{
  //Show on data
  u8g2.setFont(u8g_font_5x8);
  u8g2.setCursor(8, 55);
  u8g2.print(nowTime.Day());

  u8g2.drawStr( 19, 55, "/");
  u8g2.setCursor(24, 55);
  u8g2.print(nowTime.Month());
  u8g2.drawStr( 35, 55, "/");
  u8g2.setCursor(41, 55);
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
  u8g2.drawRFrame(0, 0, 128, 64, 3);
  u8g2.drawRFrame(68, 4, 55, 56, 2);
  ShowDigitalClock(currentTime);
  u8g2.setFont(u8g_font_5x8); 
  u8g2.drawStr( 78, 35, "MIN");
  u8g2.drawStr( 78, 53, "MAX");
  u8g2.setFont(u8g_font_6x13);
  //u8g2.setCursor(25, 41);
  //u8g2.print(Day_Of_Week[(currentTime.DayOfWeek() - 1)]);
  u8g2.drawXBMP(5, 25, Temperature_16Icon_width, Temperature_16Icon_height, Temperature_16Icon_bits);
  
  //Update actual temperature
  u8g2.setCursor(83, 19);
  u8g2.print(temperature); 
  u8g2.drawStr( 105, 19, "C");
  u8g2.drawCircle(100, 12, 2);
  
  //Show minimum temperature
  u8g2.setCursor(98, 36);
  u8g2.print(min_temp); 
  u8g2.drawCircle(113, 29, 2);
  
  //Show maxima temperature
  u8g2.setCursor(98, 54);
  u8g2.print(max_temp); 
  u8g2.drawCircle(113, 47, 2);
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
uint8_t pushDataToServer(String DataToSend, uint8_t type, String &response)
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
      String response = http.getString();             //Get the response to the request
      Serial.println(httpResponseCode);               //Print return code
      Serial.println(response);                       //Print request answer
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


uint8_t getTimeFromInternetAndUpdate(void)
{ 
  if(WiFi.status()== WL_CONNECTED)   //Check WiFi connection status
  {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
#if defined(ENABLE_DEBUG)
      Serial.println("Failed to obtain time");
#endif
      return 0;
    }
#if defined(ENABLE_DEBUG)
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
#endif
    RtcDateTime nowTime = RtcDateTime(timeinfo.tm_year%100, timeinfo.tm_mon + 1, timeinfo.tm_mday, 
                        timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec); //define date and time object
    rtcObject.SetDateTime(nowTime); //configure the RTC with object;
  }
  else
  {
#if defined(ENABLE_DEBUG)
    Serial.println("Error in WiFi connection");
#endif
    return 0;
  }
  return 1;
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
     xTaskResumeFromISR(tempTaskHandle);
  }
}
