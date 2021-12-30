////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////  Author: Jiri Kucera                           ////
////  Description: BME280 sensor data sending,      ////
////               using ESP-NOW - receiver         ////
////  Using board: ESP32 DEV KIT V1                 ////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
////  include libraries for esp-now                 ////
////////////////////////////////////////////////////////

#include <esp_now.h>
#include <WiFi.h>

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
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
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
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //register callback function will be called when data received
  esp_now_register_recv_cb(OnDataRecv);
}

////////////////////////////////////////////////////////
////  Loop                                          ////
////////////////////////////////////////////////////////

void loop() {

}
