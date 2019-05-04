/* define pin for connection */

#define I2CDEV_TIMEOUT          100
#define HTTP_POST_TIMEOUT       1000

#define RESET (19)// Not use
#define PN532_IRQ               14
#define BUZZER_PIN              12
#define BUTTON_MENU             27
#define BUTTON_RIGH             25
#define BUTTON_LEFT             26
#define BUTTON_SELT             02

#define RELAY_DV1               34
#define RELAY_DV2               35
#define RELAY_DV3               32
#define RELAY_DV4               33

#define DHT22_PIN               15

//#define ENABLE_DEBUG
//#define ENABLE_CONNECT_CLOUD

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
