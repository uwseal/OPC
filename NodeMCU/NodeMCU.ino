// #include <DHT.h> // Including library for dht

#include <ESP8266WiFi.h>

String apiKey = "OJI8QSGGMYSROQ4P";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Mihir";     // replace with your wifi ssid and wpa2 key
const char *pass =  "12345678";
const char* server = "api.thingspeak.com";

//#define DHTPIN 0          //pin where the dht11 is connected

//DHT dht(DHTPIN, DHT11);

WiFiClient client;

void setup()
{
  Serial.begin(9600);
  delay(10);
  //     dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop()
{

  //    float a = 1.000;
  //    float b = 2.000;
  //    float c = 3.0;
  //    float d = 4.0;
  String data = "";
  while (!Serial.available() || Serial.read() != '<');

  char data0;
  data = "";
  while((data0 = Serial.read()) != '\n') {
    data += data0;
  }
//  
//  {
//    if (!mySerial.available() || mySerial.read() != '<') {
//      continue;
//    }
//    
//    //    if (!mySerial.available() || mySerial.peek() != '<')
//    //    {
//    //      Serial.write(mySerial.read());
//    //      continue;
//    //
//    //    }
//
//    char data0 = mySerial.read();
//    data += data0;
//    if (data0 == '\n')
//      break;
//
//
//  }

  Serial.println(data);


  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {
    yield();
    String postStr = apiKey;
    postStr += data;


    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    yield();
    //                           Serial.print("Temperature: ");
    //                           Serial.print(a);
    Serial.println("%. Send to Thingspeak.");
  } else {
    Serial.println("Could not connect.");
  }
  client.stop();

  Serial.println("Waiting...");

  for (uint16_t i = 0; i < 300; i++) {
    delay(100);
    yield();
  }
}
