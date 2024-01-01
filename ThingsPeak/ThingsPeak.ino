#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int alarm = D6;
int led   = D7;
int pin   = A0;

/* Set these to your desired credentials. */
const char *ssid = "Jones";
const char *password = "kj12345678910";

const char *host = "api.thingspeak.com";
String apiKey = "KKP4A86QXBKYZK1B";




void setup() {
  // put your setup code here, to run once:
pinMode(pin, INPUT);
pinMode(led,OUTPUT);
pinMode(alarm,OUTPUT);
lcd.init();

lcd.backlight();
lcd.setCursor(0,0);
lcd.print("CO2 Level in PPM");
Serial.begin(115200);

WiFi.mode(WIFI_STA);

Serial.println();
  Serial.print("Connecting to Wifi");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting ");

  while(WiFi.status()!= WL_CONNECTED){
  delay(500);
  Serial.println(".");
  }


  Serial.println("Node MCU IP Address:");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // put your main code here, to run repeatedly:
WiFiClient client;
const int httpPort = 80;

//Connect to host, host(website) is define at top
if(!client.connect(host, httpPort)){
  Serial.println("Connection Failed");
  delay(300);
  return; // Keep retrying until we get connected
}

//Make GET request as per the HTTP GET protocol format
String ADCData;
int adcvalue = analogRead(pin);
Serial.println(adcvalue);



if (adcvalue > 350){
  digitalWrite(led,HIGH);
  digitalWrite(alarm,HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Danger  !!!!");
  lcd.setCursor(1,1);
  lcd.print("CO2 ABOVE NORMAL");
}

else if(adcvalue < 350){
  digitalWrite(led,LOW);
  digitalWrite(alarm,LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CO2 LEVEL GOOD");
  lcd.setCursor(1,1);
  lcd.print("PPM : ");
  lcd.setCursor(8,1);
  lcd.print(adcvalue);
  
}

ADCData = String (adcvalue); //Integer to string conversion
String Link = "GET /update?api_key="+apiKey+"&field1="; // Request Webpage
Link = Link + ADCData;
Link = Link + "HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
client.print(Link);
delay(100);

//Wait for server to respond to timeout of 5 seconds
int timeout = 0;
while((!client.available()) && (timeout < 1000)){
  delay(10); // use this with timeout
  timeout++;
}

/*
// Reading through data
if (timeout < 500){
while(client.available()){
   Serial.println(client.readString()); // Reading From Cloud
   }
  }

  else {
Serial.println("Request Timeout..");
delay(5000);  // Read Page every 5 seconds
  }*/
}
