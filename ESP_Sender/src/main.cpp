#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <user_interface.h>
#include "main.h"

char apssid[] = "ESPNOW_NETWORK"; // Name of the ESP-NOW network

rst_info *res;
mem_t mem;
wdata_t wdata;

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
  Serial.begin(115200);
  delay(400);
  Serial.println("\r\nStarting\r\n");
  WiFi.mode(WIFI_STA);
  system_rtc_mem_read(MEMPOS, &mem, sizeof(mem));
  res = ESP.getResetInfoPtr();
  if (mem.ch < 1 || mem.ch > 13 || mem.failcount >= 2 || res->reason != REASON_DEEP_SLEEP_AWAKE){
    Serial.println("Searching for station.");
    uint8_t x = FindStation(apssid);
    if (x == 0){
      Serial.println("No Station");
      SleepNow();
    }
    printf("Station: 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x Channel: %d\r\n", mem.mac[0], mem.mac[1], mem.mac[2], mem.mac[3], mem.mac[4], mem.mac[5], mem.ch);
  }
  if (esp_now_init() != 0){
    Serial.println("ESP-NOW failed to init");
    SleepNow();
  }
  wifi_promiscuous_enable(1);
  wifi_set_channel(mem.ch);
  wifi_promiscuous_enable(0);
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  wdata.id = 0x22;
  wdata.temp = 22.33;
  wdata.humidity = 73.54;
  wdata.pressure = 1020.33;
  wdata.bat = 3.62;

  esp_now_send(mem.mac, (uint8_t*)&wdata, sizeof(wdata));
}

void OnDataSent(uint8_t *mac, uint8_t status){
  if (status != 0){
    Serial.println("No ACK received");
    mem.failcount++;
  } else {
    Serial.println("Data Sent");
    mem.failcount = 0;
  }
  system_rtc_mem_write(MEMPOS, &mem, sizeof(mem));
  SleepNow();
}

void loop(){
  
}

void SleepNow(){
  Serial.flush();
  digitalWrite(LED_BUILTIN, 1);
  delay(10);
  ESP.deepSleep(5000000);
}

int8_t FindStation(char *s){
  WiFi.disconnect();
  int8_t x = WiFi.scanNetworks(0, 1, 0, (uint8_t*)s);
  if (x > 0){
    mem.ch = WiFi.channel(0);
    memcpy(&mem.mac, WiFi.BSSID(0), 6);
    mem.failcount = 0;
  } else {
    mem.ch = 0;
  }
  system_rtc_mem_write(MEMPOS, &mem, sizeof(mem));
  WiFi.scanDelete();
  return x;
}
