//WiFi
#define WIFI_SSID "" //Receiver Only
#define WIFI_PASSWORD "" //Receiver Only

//MQTT
#define MQTT_HOST "" //Receiver Only
#define MQTT_PORT 1883 //Receiver Only
#define MQTT_USERNAME "" //Receiver Only
#define MQTT_PASSWORD "" //Receiver Only

//ESP_NOW
#define ESP_NOW_SSID "ESPX_" //Receiver and Sensor
#define ESP_NOW_PASSWORD "55666df7cf164a07a679f9776bc8f72e" //Sensor Only

//Receiver and Sensor
struct __attribute__((packed)) SENSOR_DATA {
  char topic[50] = "";
  char payload[150] = "";
  bool retain = false;
} sensorData;