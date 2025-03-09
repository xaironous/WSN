#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

const int SENSOR_MIN = 0;
const int SENSOR_MAX = 4095;
const int HUMIDITY_MIN = 100; 
const int HUMIDITY_MAX = 0;
char buffer[16];

typedef struct struct_message {
  int id;
  float temperature;
  float humidity;
  int soilMoisture;
} struct_message;
struct_message myData;
struct_message board1;
struct_message board2;

struct_message boardsStruct[2] = {board1, board2};

void OnDataRecv(const esp_now_recv_info * info, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           info->src_addr[0], info->src_addr[1], info->src_addr[2], info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  boardsStruct[myData.id-1].temperature = myData.temperature;
  boardsStruct[myData.id-1].humidity = myData.humidity;
  boardsStruct[myData.id-1].soilMoisture = myData.soilMoisture;

  Serial.printf("Temperature value: %.1f \n", boardsStruct[myData.id-1].temperature);
  Serial.printf("Humidity value: %.1f \n", boardsStruct[myData.id-1].humidity);
  Serial.printf("soilMoisture value: %.1f \n", convertToPercentage(boardsStruct[myData.id-1].soilMoisture));

  lcd.clear();
  lcd.setCursor(0, 0);
  sprintf(buffer, "1: %.1fC/ %.1f%%", boardsStruct[0].temperature, convertToPercentage(boardsStruct[0].soilMoisture));
  lcd.print(buffer);
  lcd.setCursor(0, 1);
  sprintf(buffer, "2: %.1fC/ %.1f%%", boardsStruct[1].temperature, convertToPercentage(boardsStruct[1].soilMoisture));
  lcd.print(buffer);
  Serial.println();
}

float convertToPercentage(int sensorValue) {
  return map(sensorValue, SENSOR_MIN, SENSOR_MAX, HUMIDITY_MIN, HUMIDITY_MAX);
}
 
void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
}
   