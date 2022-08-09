#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "main.h"

char wifissid[] = "SSID";     // Your WiFi routers SSID
char wifipass[] = "PASSWORD"; // Your WiFi routers password

char apssid[] = "ESPNOW_NETWORK"; // Name of the ESP-NOW network
char appass[] = "0f2918ca6f8b019469fe0c5563527208"; // ESP-NOW password ( Not used in this example )

WiFiEventHandler wifi_disconnected, wifi_connected;

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
  Serial.begin(115200);
  delay(400);
  Serial.println("\r\nStarting\r\n");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apssid, appass, 1, 1, 4);
  WiFi.begin(wifissid, wifipass);
  WiFi.setAutoReconnect(1);
  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());
  wifi_disconnected = WiFi.onStationModeDisconnected(wifi_dis_cb);
  wifi_connected = WiFi.onStationModeGotIP(wifi_con_cb);
  if (esp_now_init() != 0){
    Serial.println("ESP-NOW INIT FAILED!");
    while(1){
      digitalWrite(LED_BUILTIN, 0);
      delay(500);
      digitalWrite(LED_BUILTIN, 1);
      delay(500);
    }
  }
  esp_now_register_recv_cb(espnow_rev_cb);
}

void loop(){
  
}

void wifi_dis_cb(const WiFiEventStationModeDisconnected& event){
    Serial.println("Station disconnected");
}

void wifi_con_cb(const WiFiEventStationModeGotIP& event){
    Serial.print("Station connected IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(" Channel: ");
    Serial.println(WiFi.channel());
}

void espnow_rev_cb(uint8_t* mac, uint8_t *data, uint8_t len){
  uint8_t id = *data;
  printf("Received packet from: ID: 0x%02x, MAC: 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x Length: %d\r\n", id, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], len);
}
