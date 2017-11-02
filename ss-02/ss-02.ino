
#include <SD.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

// These are 'flexible' lines that can be changed
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset
#define SD_CS 15

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// SoftSPI - note that on some processors this might be *faster* than hardware SPI!
//Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, MOSI, SCK, TFT_RST, MISO);



void setup() {
  Serial.begin(9600);
  Serial.println("Diagnostics: Project Shade Sort!");
  tft.begin(HX8357D);

  //bmpDraw("jumpers.bmp", 0, 0);

  tft.fillScreen(HX8357_BLACK);
  tft.setRotation(1);
  tft.setTextColor(HX8357_YELLOW); tft.setTextSize(2);
  tft.println("Broadway Dental Lab"); // Good Size Text
  tft.println("");
  tft.println("This software is in beta");
  tft.println("and will run at reduced");
  tft.println("quality until product has");
  tft.println("been completed.");
  tft.setCursor(0, 120);
  tft.println("This hardware and software were         developed by");
  tft.setTextColor(HX8357_BLUE);
  tft.setTextSize(4);
  tft.print("Donovan Goodwin");
  delay(5000);
  tft.fillScreen(HX8357_BLACK);
}




void loop(void) {
  testText();
}


unsigned long testText() {
  unsigned long start = micros();
  tft.setCursor(2, 0);
  tft.setTextColor(HX8357_YELLOW);
  tft.setTextSize(3);
  tft.println("Broadway Dental           Labratories"); // Good Size Text
  tft.setCursor(2, 100);
  tft.setTextColor(HX8357_GREEN);
  tft.setTextSize(2);
  tft.print("Start a search by pressing the");
  tft.setTextColor(HX8357_BLUE);
  tft.print(" search ");
  tft.setTextColor(HX8357_GREEN);
  tft.setTextSize(2);
  tft.println(" button");

  return micros() - start;
}
