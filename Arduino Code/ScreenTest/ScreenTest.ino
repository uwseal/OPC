#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 
#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_RD A0 

#define LCD_RESET A4 

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(9600);

  tft.reset();
  tft.begin(0x9341);

  tft.fillScreen(BLACK);

  tft.setCursor(0, 0);
  tft.fillRect(0,0,240,40,WHITE);
  tft.drawRect(0,0,240,40,WHITE);
}

void loop(void) {
  unsigned long start = micros();
  
  tft.setCursor(40, 10);
  tft.setTextColor(RED);  
  tft.setTextSize(3);
  tft.println("AreoSpec");

  tft.setCursor(60, 80);
  tft.setTextColor(WHITE); 
  tft.setTextSize(7);
  tft.println("AQI");

  tft.setCursor(110, 140);
  tft.setTextColor(GREEN); 
  tft.setTextSize(5);
  tft.println("0");

  
  /*
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.println();
  tft.setTextColor(GREEN); tft.setTextSize(2);
   tft.println("Hello World!");
  tft.println(01234.56789);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.println();
  
  tft.setTextColor(BLUE);    tft.setTextSize(3);
   tft.println("Hello World!");
  tft.println(01234.56789);
  tft.println(0xDEADBEEF, HEX);
  
  tft.setTextColor(WHITE);    tft.setTextSize(4);
   tft.println("Hello!");
   tft.setTextColor(YELLOW);    tft.setTextSize(5);
   tft.println("Hello!");
   tft.setTextColor(RED);    tft.setTextSize(6);
   tft.println("Hello!");
  tft.println();
  tft.println();
  */
  delay(1000);delay(1000);delay(1000);delay(1000);delay(1000);
}
