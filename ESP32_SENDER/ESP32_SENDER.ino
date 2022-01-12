////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////  Author: Jiri Kucera                           ////
////  Description: BME280 sensor data sending,      ////
////               using ESP-NOW - sender           ////
////  Using board: ESP32 DEV KIT V1                 ////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
  
////////////////////////////////////////////////////////
////  include libraries for sensors                 ////
////////////////////////////////////////////////////////
  
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

////////////////////////////////////////////////////////
////  include libraries for esp-now                 ////
////////////////////////////////////////////////////////

#include <esp_now.h>
#include <WiFi.h>

////////////////////////////////////////////////////////
////  set MAC for receiver and struct message       ////
////////////////////////////////////////////////////////
//change to your receiver MAC address 
uint8_t broadcastAddress[] = {0xAC, 0x67, 0xB2, 0x36, 0x9C, 0x9C};

//structure to send data
//must match receiver structure
typedef struct struct_message {
  float humidity;
  float temperature;
  int pressure;
  int bme_altitude;
} struct_message;

//create struct_message called SensorData
struct_message SensorData;

////////////////////////////////////////////////////////
////  define and set sensors                        ////
////////////////////////////////////////////////////////
  
#define SEALEVELPRESSURE_HPA (1013.25) //this is default
//value, change to the value with the current sea level 
//pressure at your location
Adafruit_BME280 bme;

////////////////////////////////////////////////////////
////  callback when data is sent                    ////
////////////////////////////////////////////////////////
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

////////////////////////////////////////////////////////
////  Setup                                         ////
////////////////////////////////////////////////////////
  
void setup() {
  //initialize serial for debugging and print result
  Serial.begin(115200);
  
  //initialize BME
  Serial.println(F("BME280 test"));
 
  bool status;
 
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  //set as Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //ESP-NOW init
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //ESP-NOW  register callback function
  esp_now_register_send_cb(OnDataSent);

  //register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  //add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

////////////////////////////////////////////////////////
////  reading sensor values                         ////
////////////////////////////////////////////////////////

void sensor_reading() {
  //BME sensor read data
  SensorData.humidity = bme.readHumidity();
  SensorData.temperature = bme.readTemperature();
  SensorData.pressure =  bme.readPressure() / 100.0F;
  SensorData.bme_altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
}

////////////////////////////////////////////////////////
////  Loop                                          ////
////////////////////////////////////////////////////////

void loop() {
  //get sensor value to send
  sensor_reading();

  //send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &SensorData, sizeof(SensorData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  
  delay(20000);
}
