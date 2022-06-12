#include <Arduino.h>
#include <PNGdec.h>
#include "scooter.h" // Image is stored here in an 8 bit array
#include "stateIcon.h"
PNG png; // PNG decoder inatance
#define MAX_IMAGE_WDITH 80 // Adjust for your images
byte xpos = 0;
byte ypos = 0;

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
    tft.startWrite();
    rc = png.decode(NULL, 0);
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
  ypos=0;
  uint8_t *iname;
  int16_t isize;
  switch (icon)
  {
  case 1:
    iname = (uint8_t *)mainbg;
    isize=sizeof(mainbg);
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
  }
}
void icoDisplay(const uint8_t *icname,byte ix,byte iy){
  // iname = (uint8_t *)iname;
  size_t i=strlen(icname);
  xpos=ix;
  ypos=iy;
  int16_t rc = png.openFLASH((uint8_t *)icname,sizeof(icname), pngDraw);
  Serial.println(i);
  if (rc == PNG_SUCCESS) {
    tft.startWrite();
    rc = png.decode(NULL, 0);
    tft.endWrite();
    png.close(); // not needed for memory->memory decode
  }
}

void speedDisplay(char speed){
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.drawString("88",15,65,7);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawNumber(speed,15,65,7);
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
  logoDisplay();
  statusDisplay(0);
  Serial.println("\r\nInitialisation done.");
}
//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  // modeDisplay(random(3));
  // statusDisplay(0);
  speedDisplay(random(30));
  icoDisplay(bat0,53,3);
  delay(1000);
  // for (int i=0;i<10;i++){

  //  delay(300);
  // }
}



