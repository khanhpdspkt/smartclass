#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "UTE shooting girl";
const char *password = "Apple_123456";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connecting to the WiFi network");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Check the current connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin("http://jsonplaceholder.typicode.com/comments?id=10");
    int httpCode = http.GET();

    // Check for the returning code
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }

    // Free the resources
    http.end();
  }

  delay(10000);
}
