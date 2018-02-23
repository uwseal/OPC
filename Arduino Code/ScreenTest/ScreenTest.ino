#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>   // Touchscreen library

#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 

#define LCD_RESET A4 

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define TS_MINX 156
#define TS_MINY 133
#define TS_MAXX 945
#define TS_MAXY 898

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
enum Screen {Home, AQI, Detail};
Screen State;
boolean updateRect;

void setup(void) {
  Serial.begin(9600);

  tft.reset();
  tft.begin(0x9341);
  
  tft.fillScreen(BLACK);
  State = Home;
  updateRect = true;

  pinMode(13, OUTPUT);
}

void loop(void) {
  
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  int xCord = map(p.y, TS_MAXY, TS_MINY, 0, 240);
  int yCord = 320 - map(p.x, TS_MAXX, TS_MINX, 0, 320);

  if(p.z>0){
     Serial.print("Calc ");
     Serial.print("X = "); Serial.print(xCord);
     Serial.print("\tY = "); Serial.print(yCord);
     Serial.print("\n"); 
  }



  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
   
  if (State == Home){

    if (updateRect){
      tft.fillScreen(BLACK);
      
      tft.fillRect(0,0,240,40,WHITE);
      tft.drawRect(0,0,240,40,WHITE);

      tft.fillRect(40,100,150,60,CYAN);
      tft.drawRect(40,100,150,60,BLACK);

      updateRect = false;
      delay(10);  
    }
    
    tft.setCursor(40, 10);
    tft.setTextColor(RED);  
    tft.setTextSize(3);
    tft.println("AreoSpec");

    tft.setCursor(90, 115);
    tft.setTextColor(BLACK);  
    tft.setTextSize(3);
    tft.println("AQI");

    if(xCord>40 && xCord<190 && yCord>100 && yCord<160 && p.z>0) {
      State = AQI;
      updateRect = true;
    }

    
  } else if (State == AQI){

    if (updateRect) {
      tft.fillScreen(BLACK);
      
      tft.setCursor(0, 0);
      tft.fillRect(0,0,240,40,WHITE);
      tft.drawRect(0,0,240,40,WHITE);
  
      tft.fillRect(0,40,240,60,GREEN);
      tft.drawRect(0,40,240,60,GREEN);

      tft.drawRect(0,220,240,100,WHITE);
           
      updateRect = false;
      delay(10);  
    }
    
    tft.setCursor(40, 10);
    tft.setTextColor(RED);  
    tft.setTextSize(3);
    tft.println("AreoSpec");
  
    tft.setCursor(25, 45);
    tft.setTextColor(BLACK);  
    tft.setTextSize(3);
    tft.println("Air Quality");
  
    tft.setCursor(0, 70);
    tft.setTextColor(BLACK);  
    tft.setTextSize(3);
    tft.println("    Clean");
  
    tft.setCursor(60, 110);
    tft.setTextColor(WHITE); 
    tft.setTextSize(7);
    tft.println("AQI");
  
    tft.setCursor(110, 170);
    tft.setTextColor(GREEN); 
    tft.setTextSize(5);
    tft.println("0");
  
    tft.setCursor(0, 223);
    tft.setTextColor(WHITE); 
    tft.setTextSize(2);
    tft.println(" NumP0.3-0.5  : ");
    tft.println(" NumP0.5-1.0  : ");
    tft.println(" NumP1.0-2.5  : ");
    tft.println(" NumP2.5-5.0  : ");
    tft.println(" NumP5.0-10.0 : ");
    tft.println(" NumP10.0     : ");

    if(xCord>0 && xCord<240 && yCord>0 &&yCord<40 && p.z>0) {
      State = Home;
      updateRect = true;
    }
  }
  
}
