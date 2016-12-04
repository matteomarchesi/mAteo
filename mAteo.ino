#include <SFE_BMP180.h>
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
 *  Clock definitions 
 *  Connect D2 with D3 on Arduino
 */

int clockInterrupt = 0; //interrupt 0 is pin 2 on UNO
int pwmOut = 3; //pin D3

int cyclesPerSecond = 490;

int seconds = 0;
int minutes = 44;
int hours = 14;
int masterClock = 0;

String clocktime = ""; 

String printTime();

/*
 *  DHT22 (temperature and humidity) definitions
 */
#define DHTTYPE DHT22 
#define DHTPIN 13

DHT dht(DHTPIN, DHTTYPE);

/*
 *  BMP180 (temperature and pressure) definitions
 */

SFE_BMP180 pressure;

/*
 *  SSD1306 display definitions
 */

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

  char status;
  double T,P;
  float h,t;

unsigned long previous = 0;
const long interval = 2000;

void schermo();

void readSensors();


void setup() {
  Serial.begin(9600);
  Serial.println("... on ...");

/*  
 *   Clock setup
 */

  // clockInterrupt is our interrupt, clockCounter() function is called when invoked on a RISING clock edge
  attachInterrupt(clockInterrupt, clockCounter, RISING);
  //I always give enough time to restart if its locked
  delay(2000);
  analogReference(DEFAULT);//not sure if this is needed
  Serial.print("writing to pin: ");
  Serial.println(pwmOut);
  analogWrite(pwmOut, 127); // this starts our PWM 'clock' with a 50% duty cycle
  
/*  
 *   Display setup
 */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Stazione mAteo 0.0");
  display.display();
  delay(500);

/*  
 *   DHT setup
 */
  pinMode(1,INPUT);
  
/*  
 *   BMP setup
 */

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
   
}

void loop() {
  unsigned long current = millis();

  if (current - previous >= interval) {
    previous = current;
    readSensors();   
  }


  schermo();

  
}

void readSensors(){
  h = dht.readHumidity();
  t = dht.readTemperature();

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressure.getTemperature(T);
  }

  status = pressure.startPressure(3);
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressure.getPressure(P,T);
  }
  
}

void schermo(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Temp     H/P");
    display.print(t,1);
    display.print(" *C ");
    display.print(h,1);
    display.println(" %");
   
    display.print(T,1);
    display.print(" *C ");
    display.print(P,1);
    display.println(" mb");
    clocktime = printTime();
    Serial.println(clocktime); //<<<<<
    display.print(clocktime);
    display.display();
}

void clockCounter() // called by interrupt 0 (pin 2 on the UNO) receiving a rising clock edge PWM
{
  masterClock ++; // with each clock rise add 1 to masterclock count
  if(masterClock == cyclesPerSecond) // 974hz on pin 6, may be 490Hz if you use pin 9 or 10
    {
    seconds ++; // after one cycle add 1 second
    masterClock = 0; //reset clock counter
    
    if(seconds==60){
      minutes++;
      seconds = 0;//reset seconds counter
      
      if(minutes==60){
        hours++;
        minutes = 0;
        }
      }
    }
  return;
}

String printTime(){
  String ct = "";
  if(hours<10){
    ct = "0";
  }
  ct = String(ct + hours + ":");
  
  if(minutes<10){
    ct = String(ct + "0");
  }
  ct = String(ct + minutes + ":");

  if(seconds<10){
    ct = String(ct + "0");
  }
  ct = String(ct + seconds);
  return ct;
}
