#include <Arduino.h>
#include <PNGdec.h>
#include "scooter.h" // Image is stored here in an 8 bit array
#include "stateIcon.h"
PNG png; // PNG decoder inatance
#define MAX_IMAGE_WDITH 80 // Adjust for your images
int16_t xpos = 0;
int16_t ypos = 0;

// Include the TFT library https://github.com/Bodmer/TFT_eSPI
#include "SPI.h"
#include <TFT_eSPI.h>              // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();         // Invoke custom library
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}


void logoDisplay(){
    int16_t rc = png.openFLASH((uint8_t *)scooter, sizeof(scooter), pngDraw);
  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    delay(3000);
    tft.fillScreen(TFT_BLACK);
     png.close(); // not needed for memory->memory decode
  }
}
//TESSSSSSSSSSSSSSSSSSSSTTT
void statusDisplay(byte icon){
  //Serial.println(ESP.getFreeHeap());
  xpos=0;
  ypos=80;
  uint8_t *iname;
  int16_t isize;
  switch (icon)
  {
  case 1:
    iname = (uint8_t *)charge;
    isize=sizeof(charge);
    break;
  case 2:
    iname = (uint8_t *)heavy;
    isize=sizeof(heavy);
    break;
  case 3:
    iname = (uint8_t *)fast;
    isize=sizeof(fast);
    break;
  case 4:
    iname = (uint8_t *)off;
    isize=sizeof(off);
    break;
  case 5:
    iname = (uint8_t *)location;
    isize=sizeof(location);
    break;
  case 6:
    iname = (uint8_t *)locked;
    isize=sizeof(locked);
    break;
  case 7:
    iname = (uint8_t *)on;
    isize=sizeof(on);
    break;
  default:
    break;
  }
    int16_t rc = png.openFLASH(iname, isize, pngDraw);
  if (rc == PNG_SUCCESS) {
    tft.startWrite();
    rc = png.decode(NULL, 0);
    tft.endWrite();
    png.close(); // not needed for memory->memory decode
    Serial.println(sizeof(iname));
  }
}

void speedDisplay(char speed){
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.drawString("88",10,0,7);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if(speed < 10){
    tft.drawNumber(0,10,0,7);
    tft.drawNumber(speed,44,0,7);
  }
  else tft.drawNumber(speed,10,0,7);
}
void modeDisplay(byte mode){
  uint16_t color;
  String scolor;
  String modeText;
  switch (mode)
  {
  case 0:
    color=0x0FF0;
    modeText ="   ECO   ";
    break;
  case 1:
    color=0x00FF;
    modeText ="  NORM  ";
    break;
  case 2:
    color = 0xF00F;
    modeText ="  SPORT  ";
    break;
  default:
    break;
  }
  tft.fillRect(0,60,40,80,TFT_BLACK);
  tft.setTextColor(color, TFT_BLACK);
  Serial.println(modeText);
  tft.drawCentreString(modeText,40,60,4);
}
//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(9600);
  Serial.println("\n\n Using the PNGdec library");

  // Initialise the TFT
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  //logoDisplay();
  Serial.println("\r\nInitialisation done.");
}
//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  // modeDisplay(random(3));
  statusDisplay(random(8));
  delay(1000);
  // for (int i=0;i<10;i++){
  //  speedDisplay(i);
  //  delay(300);
  // }
}



