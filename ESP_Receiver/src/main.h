#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

void wifi_dis_cb(const WiFiEventStationModeDisconnected&);
void wifi_con_cb(const WiFiEventStationModeGotIP&);
void espnow_rev_cb(uint8_t*, uint8_t*, uint8_t);
