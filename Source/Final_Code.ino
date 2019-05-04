
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <RtcDS3231.h>  //RTC library
#include <U8g2lib.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Ticker.h"


#include "define.h"



enum Content_Type {
  TYPE_JSON,
  RX_READ
};


struct DeviceStatus 
{
  uint8_t DV1:1;
  uint8_t DV2:1;
  uint8_t DV3:1;
  uint8_t DV4:1;
};

/* define wifi and host */
//const char* ssid = "Linh Nam";
//const char* password =  "0919607667";
const char* ssid = "ziroom201";
const char* password =  "ziroomer002";

const char* host = "http://192.168.0.105:8080";

void scanTagTask(void *pvParameters);
void readtimeTask(RtcDateTime &nowTime);

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_MIRROR, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 5, /* dc=*/ 17, /* reset=*/ 8);
Adafruit_NFCShield_I2C nfc(PN532_IRQ, RESET);
RtcDS3231<TwoWire> rtcObject(Wire);   //Uncomment for version 2.0.0 of the rtc library
StaticJsonDocument<150> doc;
RtcDateTime currentTime;
DeviceStatus dvStatus;
QueueHandle_t queue;

/* Declare variables */
boolean success;                          // status when reading tag
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

/* Configure BUZZER */
int freq = 2000;
int channel = 0;
int resolution = 8;

// Variables for LCD_GPHONE
int X2 = 0;
int Y2 = 0;
int X3 = 0;
int Y3 = 0;
int positionMinute = 0;
int positionHour = 0;
float angle = 0;
int ScreenWith = 128;
int ScreenWithC = 96;
int ScreenHeight = 64;
int ScreenHeightC = 32;

int temperature =16;
int min_temp = 500;
int max_temp = -500;

uint8_t MenuStatus = 0;

char* Day_Of_Week[]={
                      "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
                    };

/* declare variables for rtos */
SemaphoreHandle_t xMutex_i2c;               // Mutex use to comunicate i2c
SemaphoreHandle_t xMutex_post;              // Mutex use to send data to host between tasks
TaskHandle_t getStatusTaskHandle = NULL;    // Task handle for the light value read task
Ticker tempTicker;                          // Ticker for get status of devices

void IRAM_ATTR handleInterrupt() 
{
  if(MenuStatus == 0)
  {
    MenuStatus = 1;
  }
  else
  {
    MenuStatus = 0;
  }
}

void setup() {
  // put your setup code here, to run once:

  /* Setup Serial */
  Serial.begin(115200);

  u8g2.begin();

  /* Setup BUZZER */
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(BUZZER_PIN, channel);

  // Set up network to send and receive data
  WiFi.begin(ssid, password);
  /* Check for the connection */
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  /* Initialize DS3231 */
  rtcObject.Begin();     //Starts I2C
  RtcDateTime currentTime = RtcDateTime(18, 11, 23, 7, 34, 0); //define date and time object
  rtcObject.SetDateTime(currentTime); //configure the RTC with object

  // Setup the PN532
  nfc.begin();
  /* Get version of pn532 */
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A card");

  //Configure for pin MENU and interrupt
  pinMode(BUTTON_MENU, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_MENU), handleInterrupt, HIGH);
 
  /* create Mutex */
  xMutex_i2c = xSemaphoreCreateMutex();
  xMutex_post = xSemaphoreCreateMutex();
  
  delay(1000);
  // Start task to scan tag
  xTaskCreatePinnedToCore(
    scanTagTask,                       /* Function toimplement the task */
    "scanTagTask",                    /* Name of the task */
    2000,                          /* Stack size in words */
    NULL,                          /* Task input parameter */
    4,                             /* Priority of the task */
    NULL,                          /* Task handle. */
    1);                            /* Core where the task should run */

  // Start task to scan tag
  xTaskCreatePinnedToCore(
    mainTask,                       /* Function toimplement the task */
    "mainTask",                    /* Name of the task */
    4000,                          /* Stack size in words */
    NULL,                          /* Task input parameter */
    5,                             /* Priority of the task */
    NULL,                          /* Task handle. */
    1);                            /* Core where the task should run */
}

void loop() {
  // put your main code here, to run repeatedly:

}



void scanTagTask(void *pvParameters) 
{
  String sendData;                          // Used to send data to server
  while (1) 
  {
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    SEMAPHORE_TAKE(xMutex_i2c, I2CDEV_TIMEOUT);
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    SEMAPHORE_GIVE(xMutex_i2c);
    
    if (success) 
    {
#if defined(ENABLE_DEBUG)
      Serial.println("Found a card!");
      Serial.print("UID Length: ");
      Serial.print(uidLength, DEC);
      Serial.println(" bytes");
      Serial.print("UID Value: ");
      for (uint8_t i=0; i < uidLength; i++) 
      {
        Serial.print(" 0x");Serial.print(uid[i], HEX); 
      }
      Serial.println("");
#endif  
      sendData = "uid=" + String(uid[0], HEX) + String(uid[1], HEX) + String(uid[2], HEX) + String(uid[3], HEX);// Prepare data to send
      Serial.println(sendData);

#if defined(ENABLE_CONNECT_CLOUD)
      pushDataToServer(sendData, RX_READ, responsed);
#endif
    
      /* Tone */
      for (int dutyCycle = 0; dutyCycle < 3; dutyCycle++)
      {
        ledcWrite(channel, 200);
        delay(50);
        
        ledcWrite(channel, 25);
        delay(50);
      }
      /* Turn off buzzer */
      ledcWrite(channel, 0);
    }
    else
    {
      // PN532 probably timed out waiting for a card
      Serial.println("Timed out waiting for a card");
    }
    
    SEMAPHORE_TAKE(xMutex_i2c,I2CDEV_TIMEOUT);
    nfc.mifareclassic_Halt();
    SEMAPHORE_GIVE(xMutex_i2c);
    
    delay(1000);
    // Wait 1 second before continuing
  }
}



void getStatusDevices(void *pvParameters) 
{
  while (1) 
  {
    if(WiFi.status()== WL_CONNECTED)   //Check WiFi connection status
    {
      HTTPClient http;   
      http.begin(String(host) + "/smartclass/controller.php");  //Specify destination for HTTP request
      int httpResponseCode = http.GET();   //Send the actual POST request
      
      if (httpResponseCode > 0)
      {
        String response = http.getString();             //Get the response to the request
        deserializeJson(doc, response);
        JsonObject obj = doc.as<JsonObject>();
        dvStatus.DV1 = obj[String("Dv1")];
        dvStatus.DV2 = obj[String("Dv2")];
        dvStatus.DV3 = obj[String("Dv3")];
        dvStatus.DV4 = obj[String("Dv4")];
        Serial.println(response);                       //Print request answer
      } 
      else
      {
        Serial.print("Error on sending GET: ");
        Serial.println(httpResponseCode);
      }
      //Free resources
      http.end();
    }
    else
    {
      Serial.println("Error in WiFi connection");   
    }
    
    // Got sleep again
    vTaskSuspend(NULL);
  } /*End while loop */
}

void mainTask(void *pvParameters)
{
  while(1)
  {
    if (MenuStatus == 1)
    {
      showMenu();
    }
    // Read time from DS3231
    readtimeTask(currentTime);
    
    //picture loop
    u8g2.firstPage(); 
    do {
      Screen_2();
    } 
    while( u8g2.nextPage() );
    delay(100);
  }
}