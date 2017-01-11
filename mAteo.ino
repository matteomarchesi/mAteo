#include <SFE_BMP180.h>
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
 *  Keyboard
 */

#define KEYPIN A6 
#define KEY1 1024
#define KEY2 980
#define KEY3 930
#define KEY4 710
#define KEY5 520

/*
 *  Clock definitions 
 *  Connect D2 with D3 on Arduino
 */

int clockInterrupt = 0; //interrupt 0 is pin 2 on UNO
int pwmOut = 3; //pin D3

int cyclesPerSecond = 490;
String ora = __TIME__;
String data = __DATE__;

int seconds = ora.substring(6,8).toInt();
int minutes = ora.substring(3,5).toInt();
int hours = ora.substring(0,2).toInt();

int dd = data.substring(4,6).toInt();
int mm;
int yyyy = data.substring(7,11).toInt();

int mmdds[] = {31,28,31,30,31,30,31,31,30,31,30,31};

int masterClock = 0;

String clocktime = ""; 

String printTime();

/*
 *  DHT22 (temperature and humidity) definitions
 */
#define DHTTYPE DHT22 
#define DHTPIN 12

DHT dht(DHTPIN, DHTTYPE);

/*
 *  BMP180 (temperature and pressure) definitions
 */

SFE_BMP180 pressure;

/*
 *  Battery voltage meter
 */

#define VPIN A7 

/*
 *  SSD1306 display definitions
 *  
 *  char size 6x8
 *  rows start at 0, 8, 16, 24 
 *  
 *  +---------------------+
 *  |Temp:  21.5 C B: 9.0V|
 *  |RH:    42.3%         |
 *  |Pre: 1000.1mb        |
 *  | 16:05:38 02/12/2017 |
 *  +---------------------+
 *
     *012345678901234567890*
     ***********************
 *  
 */

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

  char status;
  double T,P, h, t;
  float vbatt;
//  float h,t,v;

unsigned long previous = 0;
const long interval = 2000;

void schermo();

void spento();
void tupo();
void newfun();
void set_clock();
void batteryICO();
void readSensors();

int tastiera();

int funzione = 0;
int premuto = 0;
/*
 *  0 = spento
 *  1 = t/h/p + ora
 */

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
  data = data.substring(0,3);
  if (data=="Jan") {mm=1;}
  if (data=="Feb") {mm=2;}
  if (data=="Mar") {mm=3;}
  if (data=="Apr") {mm=4;}
  if (data=="May") {mm=5;}
  if (data=="Jun") {mm=6;}
  if (data=="Jul") {mm=7;}
  if (data=="Aug") {mm=8;}
  if (data=="Sep") {mm=9;}
  if (data=="Oct") {mm=10;}
  if (data=="Nov") {mm=11;}
  if (data=="Dec") {mm=12;}

/*  
 *   Display setup
 */
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(21,16);
  display.print("Stazione mAteo");
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

  premuto = tastiera();
  switch(premuto) {
    case 1:
      funzione ++;
      break;
    case 2:
      funzione --;
      break;
    default:
      break;
  }
  schermo();

  
}

void readSensors(){
  int vin;
  h = dht.readHumidity();
  t = dht.readTemperature();

/*  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressure.getTemperature(T);
  }
*/
  status = pressure.startPressure(3);
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressure.getPressure(P,t);
  }
  vin = analogRead(VPIN);
  vbatt = vin/1023.0*5.0*2; //voltage partition divide by 2
}

void schermo()
  {
    switch (funzione) {
      case 0:
        spento();
        break;
      case 1:
        tupo();
        break;
      case 2:
        set_clock();
        break;
      case 3:
        newfun();
        break;
        
      default:
        if ((funzione < 0)|(funzione > 3))
          {
            funzione = 0;
          }
        break;
    }
}

void spento()
  {
    display.clearDisplay();
    display.display();   
  }

void tupo()
  {
    /***********************
     *Temp:  21.5 C B: 9.0V*
     *RH:    42.3%         *
     *Pre: 1000.1mb        *
     * 16:05:38 02/12/2017 *
     ***********************
     *012345678901234567890*
     ***********************
     */
    display.clearDisplay();
    
    display.setCursor(0,0);
    display.print("Temp: ");
    if ((t>=100)||(t<=-10)){
      display.setCursor(6*6,0); // 100.0 -10.0
    } else if ((t>=10)||(t<0)) {
      display.setCursor(7*6,0); // 10.0 -1.0
    } else {
      display.setCursor(8*6,0); // 1.0
    }
    
    display.print(t,1);
// degree circle
    display.drawPixel(68, 0, WHITE);
    display.drawPixel(69, 0, WHITE);
    display.drawPixel(67, 1, WHITE);
    display.drawPixel(70, 1, WHITE);
    display.drawPixel(68, 2, WHITE);
    display.drawPixel(69, 2, WHITE);
// degree circle
    
	batteryICO(vbatt, 12*8, 0);
//    display.setCursor(12*6,0);
//    display.print("C B: ");
//    display.print(vbatt,1);
//    display.println("V");

    display.print("RH:    ");
    display.print(h,1);
    display.println("%");
    
    display.print("Pre: ");
    if (P>=1000){
      display.setCursor(5*6,16); //terza riga (0, 8, 16, 24)
      } else {
      display.setCursor(6*6,16);
      }
    display.print(P,1);
    display.println("mb");

    clocktime = printTime();
    display.setCursor(6,24);
    display.print(clocktime);
    display.display();
  }

void set_clock()
{
    int stfun = 0;
    
    display.clearDisplay();
    clocktime = printTime();
    display.setCursor(0,0);
    display.print("Set Time");

    display.setCursor(6,24);
    display.print(clocktime);
    display.display();

    premuto = tastiera();
    switch(premuto) {
      case 3:
        stfun++;
        do {
          premuto = tastiera();

          display.setCursor(0,8);
          display.fillRect(0,8,127,22,0);
          switch(stfun){
            case 1: //hrs
              display.print("hours");
			  display.drawFastHLine(6,21,12, WHITE);
			  display.drawFastHLine(6,22,12, WHITE);
              break;
            case 2: //min
              display.print("minutes");
			  display.drawFastHLine(24,21,12, WHITE);
			  display.drawFastHLine(24,22,12, WHITE);
              break;
            case 3: //sec
              display.print("seconds");
			  display.drawFastHLine(42,21,12, WHITE);
			  display.drawFastHLine(42,22,12, WHITE);
              break;
            case 4: //dd
              display.print("dd");
			  display.drawFastHLine(60,21,12, WHITE);
			  display.drawFastHLine(60,22,12, WHITE);
              break;
            case 5: //mm
              display.print("mm");
			  display.drawFastHLine(78,21,12, WHITE);
			  display.drawFastHLine(78,22,12, WHITE);
              break;
            case 6: //yyyy
              display.print("yyyy");
			  display.drawFastHLine(96,21,24, WHITE);
			  display.drawFastHLine(96,22,24, WHITE);
              break;
            default:
              break;
          };
		  
          display.setCursor(6,24);
          display.fillRect(0,24,127,30,0);
          clocktime = printTime();
          display.print(clocktime);
          display.display();

          
          switch(premuto){
            case 1:
              switch(stfun){
                case 1: //hrs
                  hours++;
                  break;
                case 2: //min
                  minutes++;
                  break;
                case 3: //sec
                  seconds=0;
                  break;
                case 4: //dd
                  dd++;
                  break;
                case 5: //mm
                  mm++;
                  break;
                case 6: //yyyy
                  yyyy++;
                  break;
                default:
                  break;
              }
              break;          
            case 2:
              switch(stfun){
                case 1: //hrs
                  hours--;
                  break;
                case 2: //min
                  minutes--;
                  break;
                case 3: //sec
                  seconds=0;
                  break;
                case 4: //dd
                  dd--;
                  break;
                case 5: //mm
                  mm--;
                  break;
                case 6: //yyyy
                  yyyy--;
                  break;
                default:
                  break;
              }
              break;          
            case 3:
              stfun++;
              break;          
            case 4:
              stfun--;
              break;          
            case 5:
              stfun=0;
              break;          
            default:
              break;          
          }
        } while (stfun>0);
        break;
      default:
      break;
    }
        
}


void newfun()
  {
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("funzione ");
    display.print(funzione);
    display.display();

  }
  

void clockCounter() // called by interrupt 0 (pin 2 on the UNO) receiving a rising clock edge PWM
{
  masterClock ++; // with each clock rise add 1 to masterclock count
  if(masterClock == cyclesPerSecond) // 974hz on pin 6, may be 490Hz if you use pin 9 or 10
    {
      seconds ++; // after one cycle add 1 second
      masterClock = 0; //reset clock counter
      
      if(seconds==60)
        {
          minutes++;
          seconds = 0;//reset seconds counter
          
          if(minutes==60)
            {
              hours++;
              minutes = 0;
              if (hours == 24)
                {
                  dd ++;
                  hours = 0;
                  if (dd>mmdds[mm])
                  {
                    if (!((dd = 29) && (mm = 2) && ((yyyy = 2020) || (yyyy = 2024) || (yyyy = 2028) || (yyyy = 2032)))) 
                    {
                      mm ++;
                      dd = 1;
                      if (mm>12)
                      {
                        yyyy ++;
                        mm = 1;
                }
              }
            }
          }
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
  ct = String(ct + seconds + " ");
  if (dd<10){
    ct = String(ct + "0");
  }
  ct = String(ct + dd + "/");

  if (mm<10){
    ct = String(ct + "0");
  }
  ct = String(ct + mm + "/" + yyyy);

  return ct;
}

int tastiera(){
  int keyp;
  int pressed = 0;
  delay(100);
  keyp = analogRead(KEYPIN);
  if (keyp > KEY1-20 && keyp < KEY1+20){
    pressed = 1;
  }
  if (keyp > KEY2-20 && keyp < KEY2+20){
    pressed = 2;
  }
  if (keyp > KEY3-20 && keyp < KEY3+20){
    pressed = 3;
  }
  if (keyp > KEY4-20 && keyp < KEY4+20){
    pressed = 4;
  }
  if (keyp > KEY5-20 && keyp < KEY5+20){
    pressed = 5;
  }
  return pressed;
}

int float2int(float n) {
	int  rounded = (int) n;
	n = n - rounded;
	if (n>=0.5) {rounded++;}
	return rounded;
}

void batteryICO(float vbatt, int x0, int y0){
	int blevel;
	// draw empty battery
	display.drawFastHLine(x0+1,y0,11, WHITE);   
	display.drawFastHLine(x0+1,y0+6,11, WHITE);  
	display.drawFastVLine(x0,y0+1,3, WHITE);
	display.drawFastVLine(x0+1,y0,7, WHITE);
	display.drawFastVLine(x0+11,y0,7, WHITE);
	
	// calculate how full is the battery, i.e how many thicks
	blevel = float2int((vbatt - 7.0)/0.35); //empty batt is < 7.0V, full batt is >= 9.45, each thick is 0.35V, 7 thicks
	
	// fill battery with 1 thick every 0.35V
	for (int i = 0; i = blevel; i++) {
		display.drawFastVLine(x0+10-i,y0+2,3, WHITE);
	}
}

