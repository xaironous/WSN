#include <WiFi.h>
#include <esp_now.h>
#include <DHT.h>
#include <esp_sleep.h>
#include "time.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN 34
#define uS_TO_S_FACTOR 1000000  

const char* ssid     = "";
const char* password = "";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

DHT dht(DHTPIN, DHTTYPE);
uint8_t broadcastAddress[] = {0xA0, 0xA3, 0xB3, 0x2A, 0x5A, 0xF4};

typedef struct struct_message {
  int id;
  float temperature;
  float humidity;
  int soilMoisture;
} struct_message;
struct_message myData;

esp_now_peer_info_t peerInfo;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  myData.id = 1;
  myData.temperature = dht.readTemperature();
  myData.humidity = dht.readHumidity();
  myData.soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  if (isnan(myData.temperature) || isnan(myData.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    } else {
      Serial.println("Error sending the data");
    }
    Serial.print("Temperature: ");
    Serial.print(myData.temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(myData.humidity);
    Serial.print(" %, Soil Moisture: ");
    Serial.println(myData.soilMoisture);
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  int secondsUntilNextInterval = (10 - (timeinfo.tm_min % 10)) * 60 - timeinfo.tm_sec;
  uint64_t sleepTime = secondsUntilNextInterval * uS_TO_S_FACTOR;

  delay(100);
  Serial.println("Going to sleep now...");
  esp_sleep_enable_timer_wakeup(sleepTime);
  esp_deep_sleep_start();
}
