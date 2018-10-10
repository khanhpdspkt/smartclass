#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//const char* ssid = "Linh Nam";
//const char* password =  "0919607667";
const char* ssid = "UTE shooting girl";
const char* password = "Apple_123456";
uint8_t DV1, DV2, DV3, DV4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin
  
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  DynamicJsonDocument doc;
  
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
    HTTPClient http;   
    http.begin("http://192.168.1.19:8080/Test/controller.php");  //Specify destination for HTTP request
    int httpResponseCode = http.GET();   //Send the actual POST request
    
    if(httpResponseCode>0){
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
  else{
    Serial.println("Error in WiFi connection");   
  }
  
  delay(10000);  //Send a request every 10 seconds
}
