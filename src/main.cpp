#include <Arduino.h>
#include <TFT_eSPI.h>  // Librería TFT_eSPI
#include <Adafruit_ST7735.h>
#include "Graph_TFT.h"

#define BAUDRATE 9600

TFT_eSPI tft = TFT_eSPI();

#define N 6
uint16_t y[N];
Graph_TFT canva = Graph_TFT(&tft, 1, 1, 124, 125, 20, 10, BARS, BLACK);

void setup() 
{
  Serial.begin(BAUDRATE);
  Serial.println("Testing TFT 1.44 display");

  tft.init(); 
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setTextSize(1);

  canva.setTitle("    HOLA");
  canva.setAxisDiv(2);
}

void loop() 
{
  delay(500);
  for(int i = 0; i<N ; i++){
    y[i] = rand() % 10;
  }
  canva.setDataBARS(y, N, 15);
}
