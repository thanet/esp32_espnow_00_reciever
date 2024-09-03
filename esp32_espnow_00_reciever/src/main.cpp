/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>

#include <HTTPClient.h> 

// Replace with your network credentials (STATION)  enjoy office
  // const char* ssid = "True Enjoy";
  // const char* password = "enjoy7777777777";



// Replace with your network credentials (STATION)  บ้านริมโขง
  const char* ssid = "ENJMesh";
  const char* password = "enjoy042611749";

// For uplod data to server , xampp
  String URL = "http://192.168.0.113/EspData/upload.php";


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float temp;
  float hum;
  unsigned int readingId;
} struct_message;


// Create a struct_message called myData
  struct_message incomingReadings;
// Create a Json data package
  JSONVar board;

AsyncWebServer server(80);
AsyncEventSource events("/events");


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  board["id"] = incomingReadings.id;
  board["temperature"] = incomingReadings.temp;
  board["humidity"] = incomingReadings.hum;
  board["readingId"] = String(incomingReadings.readingId);
  String jsonString = JSON.stringify(board);
  events.send(jsonString.c_str(), "new_readings", millis());
  
  Serial.printf("Board ID %u: %u bytes\n", incomingReadings.id, len);
  Serial.printf("t value: %4.2f \n", incomingReadings.temp);
  Serial.printf("h value: %4.2f \n", incomingReadings.hum);
  Serial.printf("readingID value: %d \n", incomingReadings.readingId);
  Serial.println();

}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}