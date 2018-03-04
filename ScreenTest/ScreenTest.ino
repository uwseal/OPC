#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>   // Touchscreen library

#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 

#define LCD_RESET A4 

#define YP A2  
#define XM A3  
#define YM 8   
#define XP 9  

#define TS_MINX 113
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
enum Screen {Home, AQI, Data};
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
     Serial.print("RAW: ");
     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\n");
    
     Serial.print("Calc ");
     Serial.print("X = "); Serial.print(xCord);
     Serial.print("\tY = "); Serial.print(yCord);
     Serial.print("\n\n"); 
  }

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
   
  if (State == Home){
    
    // Update rect is needed so it dosn't constantly draw rectangles in loop
    // They remain the same so they only need to be updated once
    if (updateRect){
      tft.fillScreen(BLACK); // Clear screen
      
      tft.fillRect(0,0,240,40,WHITE);
      tft.drawRect(0,0,240,40,WHITE);

      tft.fillRect(40,100,150,60,CYAN);

      tft.fillRect(40,190,150,60,YELLOW);

      updateRect = false;
    }

    tft.setCursor(47, 10);
    tft.setTextColor(RED);  
    tft.setTextSize(3);
    tft.println("AreoSpec");

    tft.setCursor(90, 115);
    tft.setTextColor(BLACK);  
    tft.setTextSize(3);
    tft.println("AQI");

    tft.setCursor(45, 205);
    tft.setTextColor(BLACK);  
    tft.setTextSize(3);
    tft.println("Particle");

    if(xCord>40 && xCord<190 && yCord>100 && yCord<160 && p.z>0) {
      State = AQI;
      updateRect = true;
    }

    if(xCord>40 && xCord<190 && yCord>190 && yCord<250 && p.z>0) {
      State = Data;
      updateRect = true;
    }
  
  } else if (State == AQI){

    if (updateRect) {
      tft.fillScreen(BLACK); // Clear Screen
      
      tft.setCursor(0, 0);
      tft.fillRect(0,0,240,40,WHITE);
  
      tft.fillRect(0,260,240,60,GREEN);                             //TODO Change rectange status to change with AQI
            
      updateRect = false;
    }
    
    tft.setCursor(47, 10);
    tft.setTextColor(RED);  
    tft.setTextSize(3);
    tft.println("AreoSpec");
  
    tft.setCursor(25, 265);
    tft.setTextColor(BLACK);  
    tft.setTextSize(3);
    tft.println("Air Quality");
  
    tft.setCursor(0, 290);
    tft.setTextColor(BLACK);  
    tft.setTextSize(3);
    tft.println("    Clean");
  
    tft.setCursor(55, 80);
    tft.setTextColor(WHITE); 
    tft.setTextSize(8);
    tft.println("AQI");
  
    tft.setCursor(100, 170);
    tft.setTextColor(GREEN); 
    tft.setTextSize(10);
    tft.println("0");                                                         // TODO have real AQI value here

    if(xCord>0 && xCord<240 && yCord>0 &&yCord<40 && p.z>0) {
      State = Home;
      updateRect = true;
    }
  }else if (State == Data) {
    
    if (updateRect){
      tft.fillScreen(BLACK); // Clear Screen
      
      tft.fillRect(0,0,240,40,WHITE);

      updateRect = false;
    }

    tft.setCursor(47, 10);
    tft.setTextColor(RED);  
    tft.setTextSize(3);
    tft.println("AreoSpec");    

    tft.setCursor(0, 50);
    tft.setTextColor(WHITE); 
    tft.setTextSize(2);
    tft.println("      Particle ");                            // TODO Add values at end of setences
    tft.println("   Concentraiton"); 
    tft.println("   Units in ug/m3");
    tft.setTextSize(2);
    tft.println(" 1.0  um : ");
    tft.println(" 2.5  um : ");
    tft.println(" 10.0 um : ");

    tft.println("");
    tft.setTextSize(2);
    tft.println("Number of Particles");                        // TODO Add values at end of setences
    tft.println(" within size range"); 
    tft.println("   Units in 0.1 L");
    tft.setTextSize(2);
    tft.println(" 0.3-0.5  um : ");
    tft.println(" 0.5-1.0  um : ");
    tft.println(" 1.0-2.5  um : ");
    tft.println(" 2.5-5.0  um : ");
    tft.println(" 5.0-10.0 um : ");
    tft.println(" 10.0 +   um : ");
    
    if(xCord>0 && xCord<240 && yCord>0 &&yCord<40 && p.z>0) {
      State = Home;
      updateRect = true;
    }
    
  }
  
}