////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////  Author: Jiri Kucera                           ////
////  Description: BME280 sensor data sending,      ////
////               using ESP-NOW - receiver         ////
////  Using board: Wemos d1 mini (ESP8266)          ////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
////  include libraries for esp-now                 ////
////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <espnow.h>

////////////////////////////////////////////////////////
////  struct message                                ////
////////////////////////////////////////////////////////

// Structure to send data
// Must match the sender structure
typedef struct struct_message {
  float humidity;
  float temperature;
  int pressure;
  int bme_altitude;
} struct_message;

// Create a struct_message called SensorData
struct_message SensorData;

////////////////////////////////////////////////////////
////  callback will be executed when data received  ////
////////////////////////////////////////////////////////

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&SensorData, incomingData, sizeof(SensorData));

  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("humidity: ");
  Serial.println(SensorData.humidity);
  Serial.print("temperature: ");
  Serial.println(SensorData.temperature);
  Serial.print("pressure: ");
  Serial.println(SensorData.pressure);
  Serial.print("bme_altitude: ");
  Serial.println(SensorData.bme_altitude);
  Serial.println();
}

////////////////////////////////////////////////////////
////  Setup                                         ////
////////////////////////////////////////////////////////

void setup() {
  //initialize serial for debugging and print result
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //ESP-NOW init
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //ESP-NOW set role and register callback function
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}
