#include "DHTesp.h"
#include "Ticker.h"

#ifndef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

DHTesp dht;

void tempTask(void *pvParameters);
bool getTemperature();
void triggerGetTemp();

/* Pin number for DHT22 data pin */
int dhtPin = 13;

/** Ticker for temperature reading */
Ticker tempTicker;

/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;

/** Flag if task should run */
bool tasksEnabled = false;


/**
 * initTemp
 * Setup DHT library
 * Setup task and timer for repeated measurement
 * @return bool
 *    true if task and timer are started
 *    false if task or timer couldn't be started
 */
bool initTemp(void) {
  /* Initialize temperature sensor */
  dht.setup(dhtPin, DHTesp::DHT22);
  Serial.println("DHT initiated");

  // Start task to get temperature
  xTaskCreatePinnedToCore(
      tempTask,                       /* Function toimplement the task */
      "tempTask ",                    /* Name of the task */
      4000,                           /* Stack size in words */
      NULL,                           /* Task input parameter */
      5,                              /* Priority of the task */
      &tempTaskHandle,                /* Task handle. */
      1);                             /* Core where the task should run */
  if (tempTaskHandle == NULL) {
    Serial.println("Failed to start task for temperature update");
    return false;
  } else {
    // Start update of environment data every 20 seconds
    tempTicker.attach(20, triggerGetTemp);
  }
  return true; 
}

/**
 * triggerGetTemp
 * Set flag dhtUpdated to true for handling in loop()
 * call by Ticker getTempTimer
 */
void triggerGetTemp(void) {
  if (tempTaskHandle != NULL) {
    //xTaskResumeFromISR(tempTaskHandle);
  }
}


/**
  * getTemperature
  * Read temperature from DHT22 sensor
  * @return bool
  *    true if temperature could be aquired
  *    false if aquisition failed
 */
bool getTemperature(void) {
  // Reading temperature and humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    return false;
  }
  Serial.println(" T:" + String(newValues.temperature) + " H:" + String(newValues.humidity));
  return true;
}

/**
 * Task to read temperature from DHT22 sensor
 * &param pvParameters
 *    pointer to task parameter
 */
void tempTask(void *pvParameters) {
  Serial.println("tempTask loop started");
  while (1) // tempTask loop
  {
    if (tasksEnabled) {
      // Get temperature values
      getTemperature();
    }
    // Got sleep again
    vTaskSuspend(tempTaskHandle);
  }
}

void setup() {
  // put your setup code here, to run once:
  /* Initial serial */
  Serial.begin(115200);
  Serial.println("Demo RTOS and DHT");
  
  initTemp();
  // Signal end of setup() to tasks
  tasksEnabled = true;
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!tasksEnabled) {
    // Wait 2 seconds to let system settle down
    delay(2000);
    // Enable task that will read values from the DHT sensor
    tasksEnabled = true;
    if (tempTaskHandle != NULL) {
//      vTaskResume(tempTaskHandle);
      vTaskSuspend(tempTaskHandle);
    }
  }
  yield();

}
