
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

void setup()
{
  // USB Serial 9600 baud.
  Serial.begin(9600);

  // HC-05 9600 baud.
  Serial2.begin(9600);

  // Plantower Senser 9600.
  Serial1.begin(9600);
}

void loop()
{
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
