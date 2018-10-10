#include <WiFi.h>
#include <HTTPClient.h>

//const char* ssid = "Linh Nam";
//const char* password =  "0919607667";
const char* ssid = "UTE shooting girl";
const char* password = "Apple_123456";

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
  // put your main code here, to run repeatedly:
  String info = "temp=" + String(36);
  info += "&humi=" + String(67);
    
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
    HTTPClient http;   
    http.begin("http://192.168.1.19:8080/Test/Test.php");  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");             //Specify content-type header
    int httpResponseCode = http.POST(info);   //Send the actual POST request
    
    if(httpResponseCode>0){
      String response = http.getString();             //Get the response to the request
      Serial.println(httpResponseCode);               //Print return code
      //Serial.println(response);                       //Print request answer
      
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
