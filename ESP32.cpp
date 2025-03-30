
#include <HardwareSerial.h>
#include <WiFi.h>
#include <Arduino.h>
#include <WiFiMulti.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define WIFI_SSID "Thanhdeptrai"
#define WIFI_PASSWORD "aaaa1111"
#define INFLUXDB_URL "http://192.168.104.59:8086"
#define INFLUXDB_TOKEN "a7GQg5j8AOGkjKaa99fR0JiiWJVsyXaONk2ao5ULslDzAj9p35NJEpICiDP9ipY3svVCm8FMu-2qyboRFLFMvQ=="
#define INFLUXDB_ORG "64b9af716bf21a97"
#define INFLUXDB_BUCKET "ap_trung"

WiFiMulti wifiMulti;
#define DEVICE "ESP32"

HardwareSerial STM32(1);
#define RXD1 16
#define TXD1 17
#define stm32_reset_pin 4

#define TZ_INFO "WET0WEST,M3.5.0/1,M10.5.0"
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

Point sensor("wifi_status");
Point ap_trung("ap_trung");

typedef struct {
  char ssid[32];
  char pass[32];
} Wifi_info;
Wifi_info Wifi;

// Định nghĩa struct SensorData
typedef struct {
    float MQ135_ppm;     // Giá trị nồng độ khí MQ135
    float Heating_Temp;   // Nhiệt độ sưởi
    float Air_Temp;       // Nhiệt độ không khí
    uint8_t water_level;  // Mức nước
    uint8_t Do_am;        // Độ ẩm
} SensorData;
SensorData sensorData;


uint8_t data; // Biến để lưu giá trị đọc từ UART
// Hàm con nhận bất kỳ struct nào qua UART 
bool getDatatoStruct(const char* str ,void* destStruct, size_t structSize);


void setup() {
  pinMode(stm32_reset_pin,OUTPUT);
  digitalWrite(stm32_reset_pin,0); 
  STM32.begin(115200, SERIAL_8N1, RXD1, TXD1);

  Serial.begin(115200);
  delay(1000);
  digitalWrite(stm32_reset_pin,1);

  Serial.println("Start");

  Serial.println("Đợi phản hồi từ stm32.");
  unsigned long startTime = millis();
  while (!STM32.available()) {
    delay(500);
    Serial.print(".");
      if (millis() - startTime > 50000) {
          Serial.println("Không có phản hồi từ STM32.");
          Serial.println("Vui lòng kiểm tra lại kết nối");

      } 
  }
  Serial.println("Đã nhận được phản hồi từ stm32.");
  for (uint8_t i = 0; i < 100; i++)
  {
    STM32.read();
  }
  STM32.flush();

  WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();

    sensor.addTag("device", DEVICE);
    sensor.addTag("SSID", WiFi.SSID());
    ap_trung.addTag("ap_trung", DEVICE);

    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    if (client.validateConnection()) {
        Serial.print("Connected to InfluxDB: ");
        Serial.println(client.getServerUrl());
    } else {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(client.getLastErrorMessage());
    }

}

void loop() {
  getDatatoStruct("GSD",&sensorData, sizeof(SensorData));
  sensor.clearFields();
    sensor.addField("rssi", WiFi.RSSI());
    Serial.print("Writing: ");
    Serial.println(client.pointToLineProtocol(sensor));

    ap_trung.clearFields();
    ap_trung.addField("MQ135_ppm", sensorData.MQ135_ppm);
    ap_trung.addField("Heating_Temp", sensorData.Heating_Temp);
    ap_trung.addField("Air_Temp", sensorData.Air_Temp);
    ap_trung.addField("water_level", sensorData.water_level);
    ap_trung.addField("Do_am", sensorData.Do_am);
    Serial.print("Writing: ");
    Serial.println(client.pointToLineProtocol(ap_trung));

    if (wifiMulti.run() != WL_CONNECTED) {
        Serial.println("Wifi connection lost");
    }
    if (!client.writePoint(sensor)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(client.getLastErrorMessage());
    }
    if (!client.writePoint(ap_trung)) {
        Serial.print("Lỗi ghi InfluxDB (Ấp Trứng): ");
        Serial.println(client.getLastErrorMessage());
    } else {
        Serial.println("Ghi dữ liệu Ấp Trứng thành công!");
    }

    Serial.println("Wait 10s");
    delay(10000);
}



bool getDatatoStruct(const char* str ,void* destStruct, size_t structSize) {
  uint8_t rxBuffer[structSize];  // Mảng để lưu trữ dữ liệu nhận được
  uint8_t receivedByte;
  uint8_t j = 0;
  unsigned long startTime;

  STM32.write(strlen(str)/10);
  STM32.write(strlen(str)%10);

  // Đợi đến khi có dữ liệu hoặc hết thời gian chờ (timeout)
  startTime = millis();
  while (!STM32.available()) {
    if (millis() - startTime > 2000) {
        Serial.println("getDatatoStruct Timeout.1");
        for (uint8_t i = 0; i < 128; i++)
        {
          STM32.read();
        }
        STM32.flush();
        return false; // Thoát nếu hết thời gian chờ
    }
  }
    for (uint8_t i = 0; i < 128; i++)
  {
    STM32.read();
  }
  STM32.flush();
  startTime = millis();
  STM32.write((const uint8_t*)str, strlen(str));
  delay(100);
  while (!STM32.available()) {
    if (millis() - startTime > 4000) {
        Serial.println("getDatatoStruct Timeout.2");
        for (uint8_t i = 0; i < 128; i++)
        {
          STM32.read();
        }
        STM32.flush();
        return false; // Thoát nếu hết thời gian chờ
    }
  }

  // Kiểm tra xem có dữ liệu trong UART không
  while (STM32.available() > 0) {
    receivedByte = STM32.read();  // Đọc một byte từ UART
    Serial.print(receivedByte);
    // Lưu dữ liệu vào buffer
    if (j < structSize) {
      rxBuffer[j] = receivedByte;
      j++;
    }
    // Khi nhận đủ dữ liệu cho struct, sao chép
    if (j >= structSize) {
      memcpy(destStruct, rxBuffer, structSize);// Sao chép vào cấu trúc
      for (uint8_t i = 0; i < 128; i++)
      {
        STM32.read();
      }
      STM32.flush();
      Serial.println();
      return true; 
    }
  }

  for (uint8_t i = 0; i < 128; i++)
  {
    STM32.read();
  }
  STM32.flush();
  return false; 
}

