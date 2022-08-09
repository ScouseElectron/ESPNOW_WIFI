#pragma once

#include <Arduino.h>

#define MEMPOS 0x41

typedef struct{
  uint8_t ch;
  uint8_t mac[6];
  uint8_t failcount;
} mem_t;

typedef struct{
    uint8_t id;
    float temp;
    float humidity;
    float pressure;
    float bat;
} wdata_t;

void OnDataSent(uint8_t*, uint8_t);
void SleepNow(void);
int8_t FindStation(char*);
