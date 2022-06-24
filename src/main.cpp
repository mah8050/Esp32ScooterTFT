#include <Arduino.h>
#include <PNGdec.h>
#include "scooter.h" // Image is stored here in an 8 bit array
#include "stateIcon.h"
//Freq Counter
#include "FreqCountESP.h"
#define DAC1 25
int inputPin = 5;
int timerMs = 1000;
//Set Speed PWM from ADC
int speedADCPIN = 34; //set Gpio 34 as speed ADC
int speedOutPIN = 13; // set GPIO 35 as pwm out
int PWMFreq = 10000;
int PWMChannel = 0;
int PWMResolution = 10;
int ADC_RESOLUTION = 4095;
uint32_t frequency;
uint16_t actualSpeed;
byte speedCount;
byte cruiseControl;
uint16_t dutyCycle;
uint16_t adcRead;
byte dacVal;
int maxSpeed=30;
//display PNG
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
void mainBgDisplay(byte icon){
  //Serial.println(ESP.getFreeHeap());
  xpos=0;
  ypos=0;
  int16_t rc = png.openFLASH((uint8_t *)mainbg, sizeof(mainbg), pngDraw);
  if (rc == PNG_SUCCESS) {
    tft.startWrite();
    rc = png.decode(NULL, 0);
    tft.endWrite();
    png.close(); // not needed for memory->memory decode
  }
}
void icoDisplay(uint8_t *icname,int size,byte ix,byte iy){
  // uint8_t *iname ;
  xpos=ix;
  ypos=iy;
  int16_t rc = png.openFLASH(icname,size, pngDraw);
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

//setspeed
void setSpeed(){

  /* Cruise Control First Implementation
  if(speedCount <= 5 && dutyCycle!=actualSpeed){
    adcRead=analogRead(speedADCPIN);
  }else if(speedCount <=5 && dutyCycle==actualSpeed){
    adcRead=analogRead(speedADCPIN);
    speedCount++;
  }else if(speedCount>=5 && cruiseControl==0 && (dutyCycle==actualSpeed || analogRead(speedADCPIN) < 20)){
    cruiseControl=1;
  }else if (speedCount>=5 && cruiseControl==1 && analogRead(speedADCPIN) > 20){
    cruiseControl=0;
    speedCount=0;
  }*/
  adcRead=analogRead(speedADCPIN);
  //dutyCycle = map(adcRead, 900, 2960, 0, maxSpeed*100);
  dacVal = map(adcRead, 900, 2960, 0, 255);
  if(dutyCycle>=2950 || dutyCycle <= 20){
  tft.fillScreen(TFT_YELLOW);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawCentreString("Error",40,20,4);
  tft.setTextColor(TFT_BLACK, TFT_YELLOW);
  tft.drawCentreString("Sensor",40,50,2);
  tft.drawCentreString("Error",40,70,2);

  }else{
     dacWrite(DAC1,dacVal);
  }
}

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(9600);
  Serial.println("\n\n Using the PNGdec library");
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  ledcAttachPin(speedOutPIN, PWMChannel);
  // Initialise the TFT
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  logoDisplay();
  mainBgDisplay(0);
  FreqCountESP.begin(inputPin, timerMs);
  Serial.println("\r\nInitialisation done.");
}
//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  icoDisplay((uint8_t *)bat0,sizeof(bat0),53,3);
  delay(1000);
  // for (int i=0;i<10;i++){
  
  //  delay(300);
  // }
// setSpeed();
dacWrite(DAC1,dacVal);
// ledcWrite(PWMChannel, 550);
Serial.println(dacVal);
  //show speed
  // if (FreqCountESP.available())
  // {
  //   frequency = FreqCountESP.read();
  //   //Serial.println(frequency/1000);
  //   speedDisplay(frequency/10);
  //   actualSpeed=frequency/10;

  // }
  setSpeed();
}



