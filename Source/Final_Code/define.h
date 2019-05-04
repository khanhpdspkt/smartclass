/* define pin for connection */

#define I2CDEV_TIMEOUT          100
#define HTTP_POST_TIMEOUT       1000

#define RESET (19)// Not use
#define PN532_IRQ               14
#define BUZZER_PIN              12
#define BUTTON_MENU_PIN         27
#define BUTTON_RIGH_PIN         25
#define BUTTON_LEFT_PIN         26
#define BUTTON_SELT_PIN         02

#define RELAY_DV1               34
#define RELAY_DV2               35
#define RELAY_DV3               32
#define RELAY_DV4               33

#define DHT22_PIN               15

//#define ENABLE_DEBUG
//#define ENABLE_CONNECT_CLOUD

#define NUM_BUTTONS 3
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {BUTTON_RIGH_PIN, BUTTON_LEFT_PIN, BUTTON_SELT_PIN};

#define SEMAPHORE_TAKE(port, timeOut) do { \
          if (!xSemaphoreTake(port, timeOut / portTICK_RATE_MS)) \
          { \
          } \
        } while (0)

#define SEMAPHORE_GIVE(port) do { \
          if (!xSemaphoreGive(port)) \
          { \
          } \
        } while (0)
