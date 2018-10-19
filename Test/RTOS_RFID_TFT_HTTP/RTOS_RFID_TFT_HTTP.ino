#include <Wire.h>
#include <SPI.h>
#include <Adafruit_NFCShield_I2C.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define IRQ   (14)
#define RESET (5)                     // Not connected by default on the NFC Shield

//const char* ssid = "Linh Nam";
//const char* password =  "0919607667";
const char* ssid = "UTE shooting girl";
const char* password = "Apple_123456";

Adafruit_NFCShield_I2C nfc(IRQ, RESET);
TFT_eSPI myGLCD = TFT_eSPI();

void ScanTag(void *pvParameters);
SemaphoreHandle_t xMutex;

/* Status of devices */
uint8_t DV1, DV2, DV3, DV4;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("Demo PN532!");
  nfc.

	// Setup the PN532
	nfc.begin();

	// Setup the LCD
	myGLCD.init();
	myGLCD.setRotation(1);

	// Set up network to send and receive data
//	WiFi.begin(ssid, password);
//	/* Check for the connection */
//	while (WiFi.status() != WL_CONNECTED) {
//		delay(1000);
//		Serial.println("Connecting to WiFi..");
//	}
//	Serial.println("Connected to the WiFi network");

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

	/* create Mutex */
	xMutex = xSemaphoreCreateMutex();

	// Start task to scan tag
	xTaskCreatePinnedToCore(
		ScanTag,                       /* Function toimplement the task */
		"scantag ",                    /* Name of the task */
		4000,                          /* Stack size in words */
		NULL,                          /* Task input parameter */
		5,                             /* Priority of the task */
		NULL,                          /* Task handle. */
		1);                            /* Core where the task should run */
}

void loop() {
  // put your main code here, to run repeatedly:

}

void ScanTag(void *pvParameters) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  char buffer[5];
  String sendData;                          // Used to send data to server
  
  while(1) {
    myGLCD.fillScreen(TFT_BLUE);
    myGLCD.setTextColor(TFT_WHITE, TFT_BLUE);
    myGLCD.drawCentreString("SCAN TAG", 160, 100, 4);
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    
    if (success) {
      Serial.println("Found a card!");
      Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      Serial.print("UID Value: ");
      sprintf(buffer, "%X:%X:%X:%X", uid[0], uid[1], uid[2], uid[3]);
      sendData = "UID=" + String(buffer);                             // Prepare data to send
      
      myGLCD.setTextColor(TFT_WHITE, TFT_BLUE);
      myGLCD.drawCentreString(buffer, 160, 100, 4);
      
      for (uint8_t i=0; i < uidLength; i++) {
        Serial.print(" 0x");Serial.print(uid[i], HEX); 
      }
      Serial.println("");

      /* Send data to server using http post protocol */
      if( xMutex != NULL )
      {
          /* See if we can obtain the semaphore.  If the semaphore is not
          available wait 10 ticks to see if it becomes free. */
          if( xSemaphoreTake( xMutex, ( TickType_t ) 10 ) == pdTRUE )
          {
              /* We were able to obtain the semaphore and can now access the
              shared resource. */

              /*
              if(WiFi.status()== WL_CONNECTED) //Check WiFi connection status
              {
                HTTPClient http;   
                http.begin("http://192.168.1.19:8080/Test/Test.php");  //Specify destination for HTTP request
                http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header
                int httpResponseCode = http.POST(sendData);   //Send the actual POST request
                
                if(httpResponseCode>0)
                {
                  String response = http.getString();             //Get the response to the request
                  Serial.println(httpResponseCode);               //Print return code
                  //Serial.println(response);                     //Print request answer
                  
                }
                else 
                {
                  Serial.print("Error on sending POST: ");
                  Serial.println(httpResponseCode);
                }
                http.end();  //Free resources
              }
              else
              {
                Serial.println("Error in WiFi connection");   
              }
              */
              
              /* We have finished accessing the shared resource.  Release the
              semaphore. */
              xSemaphoreGive( xMutex );
          }
          else
          {
              /* We could not obtain the semaphore and can therefore not access
              the shared resource safely. */
          }
      } /* End transmition data to server */

      delay(1000);
      // Wait 1 second before continuing
    }
    else
    {
      // PN532 probably timed out waiting for a card
      Serial.println("Timed out waiting for a card");
    }
    nfc.mifareclassic_Halt();
  }
}

void GetStatusDevices(void) {
  while (1) {
    if( xMutex != NULL )
    {
        /* See if we can obtain the semaphore.  If the semaphore is not
        available wait 10 ticks to see if it becomes free. */
        if( xSemaphoreTake( xMutex, ( TickType_t ) 10 ) == pdTRUE )
        {
            /* We were able to obtain the semaphore and can now access the
            shared resource. */

            if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
            
              HTTPClient http;   
              http.begin("http://192.168.1.19:8080/Test/controller.php");  //Specify destination for HTTP request
              int httpResponseCode = http.GET();   //Send the actual POST request
            
              if (httpResponseCode > 0) {
                String response = http.getString();             //Get the response to the request
                deserializeJson(doc, response);
                JsonObject obj = doc.as<JsonObject>();
                DV1 = obj[String("Dv1")];
                
                Serial.println(DV1);                            //Print return code
                Serial.println(response);                       //Print request answer
              } 
              else {
                Serial.print("Error on sending POST: ");
                Serial.println(httpResponseCode);
              }
              http.end();  //Free resources
            
            }
            else {
              Serial.println("Error in WiFi connection");   
            }

            /* We have finished accessing the shared resource.  Release the
            semaphore. */
            xSemaphoreGive( xMutex );
        }
        else
        {
            /* We could not obtain the semaphore and can therefore not access
            the shared resource safely. */
        }
    }/* End mutex checking */
    
  } /*End while loop */
}

void readTemperature(void) {
}
