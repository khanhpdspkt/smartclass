#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <RtcDS3231.h>
#include <U8g2lib.h>
#include <SPI.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHTesp.h"
#include "Ticker.h"
#include <EasyButton.h>
#include <ArduinoTrace.h>
#include "define.h"
#include "Menu.h"

/**************************************************************/
/* Example how to read DHT sensors from an ESP32 using multi- */
/* tasking.                                                   */
/* This example depends on the ESP32Ticker library to wake up */
/* the task every 20 seconds                                  */
/* Please install Ticker-esp32 library first                  */
/* bertmelis/Ticker-esp32                                     */
/* https://github.com/bertmelis/Ticker-esp32                  */
/**************************************************************/

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
const char* ssid = "Linh Nam";
const char* password =  "0919607667";
//const char* ssid = "Mi Phone";
//const char* password =  "012345678";
//const char* ssid = "ziroom201";
//const char* password =  "ziroomer002";

//const char* host = "http://192.168.0.105:8080";
const char* host = "http://192.168.0.100:8080";
//const char* host = "http://192.168.43.90:8080";

const char* UID_OK = "Updated";

void scanTagTask(void *pvParameters);

//U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_MIRROR, /* clock=*/ 18, /* data=*/ 23, /* cs=*/ 5, /* dc=*/ 17, /* reset=*/ NULL);
U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_MIRROR, /* cs=*/ 5, /* dc=*/ 17, NULL);

Adafruit_NFCShield_I2C nfc(PN532_IRQ, RESET);
RtcDS3231<TwoWire> rtcObject(Wire);   //Uncomment for version 2.0.0 of the rtc library

StaticJsonDocument<150> doc;
RtcDateTime currentTime;
DHTesp dht;
TempAndHumidity dhtData;
TempAndHumidity *pdhtData;

// Instance of the button.
EasyButton buttonMenu(BUTTON_MENU_PIN);
EasyButton buttonRight(BUTTON_RIGH_PIN);
EasyButton buttonLeft(BUTTON_LEFT_PIN);
EasyButton buttonSelect(BUTTON_SELT_PIN);

DeviceStatus dvStatus;
QueueHandle_t queue_dht;
QueueHandle_t queue_uid;

char response_uid[20];
char response_data[20];
struct tm timeinfo;

/* Declare variables */
boolean success;                          // status when reading tag
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
char uid_sendData[15];
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
uint8_t StatusUID;

char* Day_Of_Week[]={
                      "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
                    };

/* declare variables for rtos */
SemaphoreHandle_t xMutex_i2c;               // Mutex use to comunicate i2c
SemaphoreHandle_t xMutex_post;              // Mutex use to send data to host between tasks
TaskHandle_t getStatusTaskHandle = NULL;    // Task handle for the read task of device status 
TaskHandle_t tempTaskHandle = NULL;         // Task handle for the light value read task
Ticker tempTicker_dv;                       // Ticker for get status of devices
Ticker tempTicker_dht;                      // Ticker for read temperature and humidity

// For setting up critical sections (enableinterrupts and disableinterrupts not available)
// used to disable and interrupt interrupts
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

//Handle interrupt
void IRAM_ATTR handleInterrupt() 
{
  portENTER_CRITICAL_ISR(&mux);

  buttonMenu.read();
  if (buttonMenu.wasReleased()) {
    if (MenuStatus == 0) {
      MenuStatus = 1;
    }
    else {
      MenuStatus = 0;
    }
  }
  portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  // put your setup code here, to run once:

  /* Setup Serial */
  Serial.begin(115200);
  u8g2.begin();

  /* Setup BUZZER */
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(BUZZER_PIN, channel);

  //Configure device pins
  pinMode(RELAY_DV3, OUTPUT);
  pinMode(RELAY_DV4, OUTPUT);  
  pinMode(RELAY_DV1, OUTPUT);
  pinMode(RELAY_DV2, OUTPUT);
  
//#if defined(ENABLE_CONNECT_CLOUD)
  // Set up network to send and receive data
  WiFi.begin(ssid, password);
  /* Check for the connection */
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
//#endif

  /* Initialize DS3231 */
  rtcObject.Begin();     //Starts I2C
  RtcDateTime currentTime = RtcDateTime(18, 11, 23, 7, 34, 0); //define date and time object
  rtcObject.SetDateTime(currentTime); //configure the RTC with object

  //Initialize RFID reader
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

  // Initialize temperature sensor
  dht.setup(DHT22_PIN, DHTesp::DHT22);

  //Configure buttons
  buttonRight.begin();
  buttonLeft.begin();
  buttonSelect.begin();
  buttonMenu.begin();
  attachInterrupt(digitalPinToInterrupt(BUTTON_MENU_PIN), handleInterrupt, RISING);

  /* create Mutex */
  xMutex_i2c = xSemaphoreCreateMutex();
  
#if defined(ENABLE_CONNECT_CLOUD)
  xMutex_post = xSemaphoreCreateMutex();
#endif

  //ueue_dht = xQueueCreate( 10, sizeof( TempAndHumidity * ) );
  queue_uid = xQueueCreate(5, sizeof(uint8_t));
  
#if defined(ENABLE_CONNECT_CLOUD)
  tempTicker_dv.attach(20, triggerGetStatus);
#endif

  tempTicker_dht.attach(15, triggerGetTemp);

  delay(1000);
  
  // Start task to scan tag
  xTaskCreatePinnedToCore(
    scanTagTask,                   /* Function toimplement the task */
    "scanTagTask",                 /* Name of the task */
    2000,                          /* Stack size in words */
    NULL,                          /* Task input parameter */
    4,                             /* Priority of the task */
    NULL,                          /* Task handle. */
    1);                            /* Core where the task should run */

  // Start task to get temperature
  xTaskCreatePinnedToCore(
      tempTask,                       /* Function to implement the task */
      "tempTask ",                    /* Name of the task */
      2000,                           /* Stack size in words */
      NULL,                           /* Task input parameter */
      5,                              /* Priority of the task */
      &tempTaskHandle,                /* Task handle. */
      1);                             /* Core where the task should run */

#if defined(ENABLE_CONNECT_CLOUD)
  // Start task to get status of devices
  xTaskCreatePinnedToCore(
    getStatusDevices,              /* Function toimplement the task */
    "GetStatusDevices",            /* Name of the task */
    2000,                          /* Stack size in words */
    NULL,                          /* Task input parameter */
    5,                             /* Priority of the task */
    &getStatusTaskHandle,          /* Task handle. */
    1);                            /* Core where the task should run */
#endif

  // Start task to scan tag
  xTaskCreatePinnedToCore(
    mainTask,                       /* Function toimplement the task */
    "mainTask",                    /* Name of the task */
    6000,                          /* Stack size in words */
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
  uint8_t result_uid;
  //String sendData;                          // Used to send data to server
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
      
#if defined(ENABLE_CONNECT_CLOUD)
      sprintf(uid_sendData, "uid=%02X%02X%02X%02X",uid[0], uid[1], uid[2], uid[3]);
      int result = pushDataToServer(uid_sendData, RX_READ, response_uid);
      if(result == 1) {
        Serial.print("String:");
        Serial.println(response_uid);
        if (strncmp(response_uid, UID_OK, strlen(UID_OK)) == 0) {
          result_uid = 1;
          Serial.println("Oke ne");
        }
        else {
          result_uid = 0;
        }
        Serial.print("result_uid:");
        Serial.println(result_uid);
        xQueueSend(queue_uid, &result_uid, 100);
        Serial.println(uid_sendData);
        Serial.println(response_uid);
      }
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
     success = 0;
  }
}

void getStatusDevices(void *pvParameters) 
{
  while (1) 
  {
    if(WiFi.status()== WL_CONNECTED)   //Check WiFi connection status
    {
      SEMAPHORE_TAKE(xMutex_post, HTTP_POST_TIMEOUT);
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
#if defined(ENABLE_DEBUG)
        Serial.println(response);                       //Print request answer
#endif
      } 
      else
      {
        Serial.print("Error on sending GET: ");
        Serial.println(httpResponseCode);
      }
      //Free resources
      http.end();
      SEMAPHORE_GIVE(xMutex_post);
    }
    else
    {
      Serial.println("Error in WiFi connection");   
    }
    menu_button_list[0].state = dvStatus.DV1;
    menu_button_list[1].state = dvStatus.DV2;
    menu_button_list[2].state = dvStatus.DV3;
    menu_button_list[3].state = dvStatus.DV4;
    Serial.print(dvStatus.DV1);
    Serial.print(dvStatus.DV2);
    Serial.print(dvStatus.DV3);
    Serial.print(dvStatus.DV4);
    Serial.println(~(dvStatus.DV1&0x01));


    //Control devices after get status from internet
    digitalWrite(RELAY_DV1, dvStatus.DV1);
    digitalWrite(RELAY_DV2, dvStatus.DV2);
    digitalWrite(RELAY_DV3, ~(dvStatus.DV3&0x01));
    digitalWrite(RELAY_DV4, ~(dvStatus.DV4&0x01));
    
    // Got sleep again
    vTaskSuspend(NULL);
  } /*End while loop */
}


/**
 * Task to reads temperature from DHT11 sensor
 * @param pvParameters
 *    pointer to task parameters
 */
void tempTask(void *pvParameters) {
  String sendData;
  Serial.println("tempTask loop started");
  while (1) // tempTask loop
  {
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    dhtData = dht.getTempAndHumidity();
    // Check if any reads failed and exit early (to try again).
    if (dht.getStatus() != 0) {
#if defined (ENABLE_DEBUG)
      Serial.println("DHT22 error status: " + String(dht.getStatusString()));
#endif
    }
#if defined (ENABLE_DEBUG)
    char str[20];   //declare a string as an array of chars
    sprintf(str, "Temp: %0.2f \t Humi: %0.1f", dhtData.temperature, dhtData.humidity);
    Serial.println(str); //print the string to the serial port
#endif

#if defined(ENABLE_CONNECT_CLOUD)
      // Make our document be an object
      JsonObject root = doc.to<JsonObject>();
      root["Temp"] = (uint8_t)dhtData.temperature;
      root["Humi"] = (uint8_t)dhtData.humidity;
      serializeJsonPretty(root, sendData);
      
      int result = pushDataToServer(sendData, TYPE_JSON, response_data);
      //Serial.println(sendData);
      Serial.println(response_data);
#endif

    // Got sleep again
    vTaskSuspend(NULL);
  }
}

void mainTask(void *pvParameters)
{
  while(1)
  {
    if (MenuStatus == 1)
    {
      showMenu();
      MenuStatus = 0;
    }
    // Read time from DS3231
    readtimeTask(currentTime);
    
    //picture loop
    u8g2.firstPage(); 
    do {
      Screen_2();
      delay(10);
    } 
    while( u8g2.nextPage() );
    delay(100);
  }
}
