#include <Arduino.h>
#include <TFT_eSPI.h>  // Librería TFT_eSPI
#include <Adafruit_ST7735.h>
#include "Graph_TFT.h"

#define BAUDRATE 9600

TFT_eSPI tft = TFT_eSPI();

#define N 5
uint16_t x[N];
uint16_t y[N];
uint8_t percentage[5] = {10,15,25,35,15};

Graph_TFT canva = Graph_TFT(&tft, 4, 4, 124, 100, 15, 5, OCEAN);

void setup() 
{
  Serial.begin(BAUDRATE);
  Serial.println("Testing TFT 1.44 display");

  tft.init(); 
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setTextSize(1);

  canva.setTitle("Pie");
  canva.setAxisDiv(1,1);

  for(int i = 0; i<N; i++){
    x[i] = i + 3;
  }

  //uint8_t percentage[5] = {12,28,10,10,40};
  //const char* labels[5] = {"Aa", "Bb", "Cc", "Dd", "Ee"};
  //canva.setDataPIE(percentage, 5, labels);
}

void loop() 
{
  for(int i = 0; i<N ; i++){
    y[i] = 1 + rand() % 8;
  }

  canva.setDataBARS( y, N);
  //canva.setDataLINES(x, y, N);
  //canva.setDataPIE(percentage, 5);
  delay(1000);
}