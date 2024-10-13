#include <Arduino.h>
#include <TFT_eSPI.h>  // Librería TFT_eSPI
#include <Adafruit_ST7735.h>
#include "Graph_TFT.h"

#define BAUDRATE 9600

TFT_eSPI tft = TFT_eSPI();

#define N 20
uint16_t x[N];
uint16_t y[N];

Graph_TFT canva = Graph_TFT(&tft, 4, 4, 124, 90, 15, 5, BARS, BLACK);

void setup() 
{
  Serial.begin(BAUDRATE);
  Serial.println("Testing TFT 1.44 display");

  tft.init(); 
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setTextSize(1);

  canva.setTitle(" *C vs [t]");
  canva.setAxisDiv(3,3);

  for(int i = 0; i<N; i++){
    x[i] = i;
  }
}

void loop() 
{
  delay(1000);
  for(int i = 0; i<N ; i++){
    y[i] = rand() % 10;
  }
  canva.setDataLINES(x, y, N, 10);
}