//#include <WiFi.h>
//#include <HTTPClient.h>
#include <ArduinoJson.h>

//const char* ssid = "Linh Nam";
//const char* password =  "0919607667";
const char* ssid = "UTE shooting girl";
const char* password = "Apple_123456";
int temp;
int humi;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin
  
//  WiFi.begin(ssid, password); 
//  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
//    delay(1000);
//    Serial.println("Connecting to WiFi..");
//  }
//  Serial.println("Connected to the WiFi network");

}

void loop() {
  // put your main code here, to run repeatedly:
  // put your main code here, to run repeatedly:
  //String info = "{\"Temp\":25,\"Humi\":50}";
  String info;
  
  temp = 15;
  humi = 70;
  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<150> doc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Make our document be an object
  JsonObject root = doc.to<JsonObject>();

  // Add values in the object
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root.set<long>("time", 1351824120);
  root["Temp"] = temp;
  root["Humi"] = humi;

  serializeJsonPretty(root, info);
  Serial.println(info);
  
    
//  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
//  
//    HTTPClient http;   
//    http.begin("http://192.168.1.19:8080/Test/json.php");  //Specify destination for HTTP request
//    http.addHeader("Content-Type", "application/json");             //Specify content-type header
//    int httpResponseCode = http.POST(info);   //Send the actual POST request
//    
//    if(httpResponseCode>0){
//      String response = http.getString();             //Get the response to the request
//      Serial.println(httpResponseCode);               //Print return code
//      Serial.println(response);                       //Print request answer
//      
//    } 
//    else {
//      Serial.print("Error on sending POST: ");
//      Serial.println(httpResponseCode);
//    }
//    http.end();  //Free resources
//    
//  }
//  else{
//    Serial.println("Error in WiFi connection");   
//  }
//  
  delay(10000);  //Send a request every 10 seconds
}
