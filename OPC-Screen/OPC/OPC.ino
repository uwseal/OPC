#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>   // Touchscreen library

unsigned char data_hi;
unsigned char data_lo;
int AQI_max, AQI_min, PM_max, PM_min, AQI_1_0, AQI_2_5, AQI_10_0;
unsigned short data;

unsigned short startChar_1, startChar_2, framelength, PM_1_0, PM_2_5, PM_10_0,
         NumP_0_3, NumP_0_5, NumP_1_0, NumP_2_5, NumP_5_0, NumP_10_0;

unsigned char read();
unsigned short readField(const char* field);
void recordData();
void calculateAQI();
void sendBluetooth();
void updateScreen();

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

void setup()
{
  // USB Serial 9600 baud.
  Serial.begin(9600);

  // HC-05 9600 baud.
  Serial2.begin(9600);

  // Plantower Senser 9600.
  Serial1.begin(9600);
  
  tft.reset();
  tft.begin(0x9341);
  
  tft.fillScreen(BLACK);
  State = Home;
  updateRect = true;

  pinMode(13, OUTPUT);
}

void loop()
{

  updateScreen();

    
  // send data only when you receive Start Character:
  startChar_1 = read();
  Serial.println(data_hi, HEX);
  if (startChar_1 == 0x42)
  {
    startChar_2 = read(); // Second Fixed Byte
    Serial.println(startChar_2, HEX);
    if (startChar_2 == 0x4D) {
      recordData();
      calculateAQI();
      sendBluetooth();
    }
  }


}




// Read 8 bits from Serial 1 port (Particle Sensor)
unsigned char read()
{
  while (!Serial1.available());
  return Serial1.read();
}

// Print out what is read from Serial 1 port to Serial 0 terminal
unsigned short readField(const char* field)
{
  unsigned char data_hi, data_lo;

  // Read the frame length.
  data_hi = read();
  data_lo = read();
  delay(5);
  Serial.println(data_hi, HEX);
  delay(5);
  Serial.println(data_lo, HEX);
  return (data_hi << 8) + data_lo;
}

// Record one set of data from Plantower sensor
void recordData()
{
  // Read the frame length.
  framelength = readField("\nFrame Length: ");

  // Read concentraition data.
  PM_1_0 = readField("PM1.0 Std. P:   ");
  PM_2_5 = readField("PM2.5 Std. P:   ");
  PM_10_0 = readField("PM_10_0, Std. P:   ");

  // Read unused data fields.
  // Concentraition unit under atmospheric environment.
  readField("PM1.0 Atm. Env. ");
  readField("PM2.5 Atm. Env. ");
  readField("PM_10_0  Atm. Env. ");

  // Read number of particle sizes.
  NumP_0_3 = readField("Num. P. 0.3 um. ");
  NumP_0_5 = readField("Num. P. 0.5 um. ");
  NumP_1_0 = readField("Num. P. 1   um. ");
  NumP_2_5 = readField("Num. P. 2.5 um. ");
  NumP_5_0 = readField("Num. P. 5.0 um. ");
  NumP_10_0 = readField("Num. P. 10  um. ");

  // Read unused data fields.
  readField("Reserved        ");
  readField("Data Check      ");
}

void calculateAQI()
{
  // AQI Calculation for 1.0 um particle sizes.
  if (PM_1_0 <= 12 * 0.73)
  {
    PM_max = 12 * 0.73;
    PM_min = 0;
    AQI_max = 50;
    AQI_min = 0;
  }
  else if (PM_1_0 >= 12.1 * 0.73 && PM_1_0 <= 35.4 * 0.73)
  {
    PM_max = 35.4 * 0.73;
    PM_min = 12.1 * 0.73;
    AQI_max = 100;
    AQI_min = 51;
  }
  else if (PM_1_0 >= 35.5 * 0.73 && PM_1_0 <= 55.4 * 0.73)
  {
    PM_max = 55.4 * 0.73;
    PM_min = 35.5 * 0.73;
    AQI_max = 150;
    AQI_min = 101;
  }
  else if (PM_1_0 >= 55.5 * 0.73 && PM_1_0 <= 150.4 * 0.73)
  {
    PM_max = 150.4 * 0.73;
    PM_min = 55.5 * 0.73;
    AQI_max = 200;
    AQI_min = 151;
  }
  else if (PM_1_0 >= 150.5 * 0.73 && PM_1_0 <= 250.4 * 0.73)
  {
    PM_max = 250.4 * 0.73;
    PM_min = 150.5 * 0.73;
    AQI_max = 300;
    AQI_min = 201;
  }
  else if (PM_1_0 >= 250.5 * 0.73 && PM_1_0 <= 350.4 * 0.73)
  {
    PM_max = 350.4 * 0.73;
    PM_min = 250.5 * 0.73;
    AQI_max = 400;
    AQI_min = 301;
  }
  else if (PM_1_0 >= 350.5 * 0.73 && PM_1_0 <= 500 * 0.73)
  {
    PM_max = 500 * 0.73;
    PM_min = 350.5 * 0.73;
    AQI_max = 500;
    AQI_min = 401;
  }
  AQI_1_0 = (((PM_1_0 - PM_min) * (AQI_max - AQI_min)) / (PM_max - PM_min)) + AQI_min;

  // AQI Calculation for 2.5 um particle sizes.
  if (PM_2_5 <= 12)
  {
    PM_max = 12;
    PM_min = 0;
    AQI_max = 50;
    AQI_min = 0;
  }
  else if (PM_2_5 >= 12.1 && PM_2_5 <= 35.4)
  {
    PM_max = 35.4;
    PM_min = 12.1;
    AQI_max = 100;
    AQI_min = 51;
  }
  else if (PM_2_5 >= 35.5 && PM_2_5 <= 55.4)
  {
    PM_max = 55.4;
    PM_min = 35.5;
    AQI_max = 150;
    AQI_min = 101;
  }
  else if (PM_2_5 >= 55.5 && PM_2_5 <= 150.4)
  {
    PM_max = 150.4;
    PM_min = 55.5;
    AQI_max = 200;
    AQI_min = 151;
  }
  else if (PM_2_5 >= 150.5 && PM_2_5 <= 250.4)
  {
    PM_max = 250.4;
    PM_min = 150.5;
    AQI_max = 300;
    AQI_min = 201;
  }
  else if (PM_2_5 >= 250.5 && PM_2_5 <= 350.4)
  {
    PM_max = 350.4;
    PM_min = 250.5;
    AQI_max = 400;
    AQI_min = 301;
  }
  else if (PM_2_5 >= 350.5 && PM_2_5 <= 500)
  {
    PM_max = 500;
    PM_min = 350.5;
    AQI_max = 500;
    AQI_min = 401;
  }
  AQI_2_5 = (((PM_2_5 - PM_min) * (AQI_max - AQI_min)) / (PM_max - PM_min)) + AQI_min;

  Serial.print(AQI_2_5);
  Serial.print("\n");

  // AQI Calculation for 10.0 um particle sizes.
  if (PM_10_0 <= 54)
  {
    PM_max = 54;
    PM_min = 0;
    AQI_max = 50;
    AQI_min = 0;
  }
  else if (PM_10_0 >= 55 && PM_10_0 <= 154)
  {
    PM_max = 154;
    PM_min = 55;
    AQI_max = 100;
    AQI_min = 51;
  }
  else if (PM_10_0 >= 155 && PM_10_0 <= 254)
  {
    PM_max = 254;
    PM_min = 155;
    AQI_max = 150;
    AQI_min = 101;
  }
  else if (PM_10_0 >= 255 && PM_10_0 <= 354)
  {
    PM_max = 354;
    PM_min = 255;
    AQI_max = 200;
    AQI_min = 151;
  }
  else if (PM_10_0 >= 355 && PM_10_0 <= 424)
  {
    PM_max = 424;
    PM_min = 355;
    AQI_max = 300;
    AQI_min = 201;
  }
  else if (PM_10_0 >= 425 && PM_10_0 <= 504)
  {
    PM_max = 504;
    PM_min = 425;
    AQI_max = 400;
    AQI_min = 301;
  }
  else if (PM_10_0 >= 505 && PM_10_0 <= 604)
  {
    PM_max = 604;
    PM_min = 505;
    AQI_max = 500;
    AQI_min = 401;
  }

  AQI_10_0 = (((PM_10_0 - PM_min) * (AQI_max - AQI_min)) / (PM_max - PM_min)) + AQI_min;
}

// This section is used to transmit information over Bluetooth to third party.
// Communication protocol is described in the Demo User Manual document.
void sendBluetooth()
{
  // Print 1 um particle size information.
  Serial2.print("PM1.0Con,");    // Tag name.
  Serial2.print(PM_1_0);         // Concentraition.
  Serial2.print(",");
  Serial2.println(AQI_1_0);      // AQI

  // Print 2.5 um particle size information.
  Serial2.print("PM2.5Con,");    // Tag name.
  Serial2.print(PM_2_5);         // Concentraition
  Serial2.print(",");
  Serial2.println(AQI_2_5);      // AQI

  // Print 10 um particle size information.
  Serial2.print("PM10Con,");      // Tag name.
  Serial2.print(PM_10_0);         // Concentraition.
  Serial2.print(",");
  Serial2.println(AQI_10_0);      // AQI.

  // Print particle count information.
  Serial2.print("NumP0.3-0.5,");
  Serial2.println(NumP_0_3 - NumP_0_5);

  Serial2.print("NumP0.5-1.0,");
  Serial2.println(NumP_0_5 - NumP_1_0);

  Serial2.print("NumP1.0-2.5,");
  Serial2.println(NumP_1_0 - NumP_2_5);

  Serial2.print("NumP2.5-5.0,");
  Serial2.println(NumP_2_5 - NumP_5_0);

  Serial2.print("NumP5.0-10.0,");
  Serial2.println(NumP_5_0 - NumP_10_0);

  Serial2.print("NumP10.0,");
  Serial2.println(NumP_10_0);
}

void updateScreen(){
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
    tft.println("AeroSpec");

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
    tft.println("AeroSpec");
  
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
  
    tft.setCursor(60, 170);
    tft.setTextColor(GREEN); 
    tft.setTextSize(10);
    tft.fillRect(0,170,240,80,BLACK); // TODO Only update if AQI value changed
    tft.println(AQI_2_5);

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
    tft.println("AeroSpec");    

    tft.setCursor(0, 50);
    tft.setTextColor(WHITE); 
    tft.setTextSize(2);
    tft.println("      Particle ");                            // TODO Add values at end of setences
    tft.println("   Concentraiton"); 
    tft.println("   Units in ug/m3");
    tft.setTextSize(2);
    tft.fillRect(130,100,120,60,BLACK);
    tft.println(" 1.0  um : " + (String)PM_1_0);
    tft.println(" 2.5  um : " + (String)PM_2_5);
    tft.println(" 10.0 um : " + (String)PM_10_0);


    tft.println("");
    tft.setTextSize(2);
    tft.println("Number of Particles");
    tft.println(" within size range"); 
    tft.println("   Units in 0.1 L");
    tft.setTextSize(2);
    tft.fillRect(155,200,80,180,BLACK);
    tft.println(" 0.3-0.5 um: " + (String)NumP_0_3);
    tft.println(" 0.5-1.0 um: " + (String)NumP_0_5);
    tft.println(" 1.0-2.5 um: " + (String)NumP_1_0);
    tft.println(" 2.5-5.0 um: " + (String)NumP_2_5);
    tft.println(" 5.0-10.0um: " + (String)NumP_5_0);
    tft.println(" 10.0 +  um: " + (String)NumP_10_0);

    
    if(xCord>0 && xCord<240 && yCord>0 &&yCord<40 && p.z>0) {
      State = Home;
      updateRect = true;
    }
    
  }
}

