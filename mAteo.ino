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

int day = data.substring(4,6).toInt();
int month;
int year = data.substring(7,11).toInt();

int monthdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};

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
  double T,P, h, t, v;
//  float h,t,v;

unsigned long previous = 0;
const long interval = 2000;

void schermo();

void spento();
void tupo();
void newfun();
void s_c();

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
  if (data="Jan") {month=1;}
  if (data="Feb") {month=2;}
  if (data="Mar") {month=3;}
  if (data="Apr") {month=4;}
  if (data="May") {month=5;}
  if (data="Jun") {month=6;}
  if (data="Jul") {month=7;}
  if (data="Aug") {month=8;}
  if (data="Sep") {month=9;}
  if (data="Oct") {month=10;}
  if (data="Nov") {month=11;}
  if (data="Dec") {month=12;}

  
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
  v=0.0;
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
        s_c();
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
    
    display.setCursor(12*6,0);
    display.print("C B: ");
    display.print(v,1);
    display.println("V");

    display.print("RH:    ");
    display.print(h,1);
    display.println("%");
    
    display.print("Pre: ");
    if (P>1000){
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

void s_c()
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
          display.fillRect(0,8,127,14,0);
          switch(stfun){
            case 1: //hrs
              display.print("hours");
              break;
            case 2: //min
              display.print("minutes");
              break;
            case 3: //sec
              display.print("seconds");
              break;
            case 4: //day
              display.print("day");
              break;
            case 5: //month
              display.print("month");
              break;
            case 6: //year
              display.print("year");
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
                case 4: //day
                  day++;
                  break;
                case 5: //month
                  month++;
                  break;
                case 6: //year
                  year++;
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
                case 4: //day
                  day--;
                  break;
                case 5: //month
                  month--;
                  break;
                case 6: //year
                  year--;
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

void set_clock()
{
    int stfun = 0;
    
    display.clearDisplay();
    clocktime = printTime();
    display.setCursor(0,0);
    display.print(clocktime);
    display.display();

    premuto = tastiera();
    switch(premuto) {
      case 3:
        do {
          stfun++;
          display.clearDisplay();
          switch (stfun) {
              case 1: //set hrs
                display.setCursor(0,0);
                display.setTextColor(BLACK, WHITE);
                display.print(hours);
                display.setTextColor(WHITE);
                display.setCursor(2,0);
                display.print(":");
                display.setCursor(3,0);
                display.print(minutes);
                display.setCursor(5,0);
                display.print(":");
                display.setCursor(6,0);
                display.print(seconds);
                display.display();
              break;
              case 2: //set min
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.print(hours);
                display.setCursor(2,0);
                display.print(":");
                display.setCursor(3,0);
                display.setTextColor(BLACK, WHITE);
                display.print(minutes);
                display.setTextColor(WHITE);
                display.setCursor(5,0);
                display.print(":");
                display.setCursor(6,0);
                display.print(seconds);
                display.display();
              break;
              case 3: //set sec
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                display.print(hours);
                display.setCursor(2,0);
                display.print(":");
                display.setCursor(3,0);
                display.print(minutes);
                display.setCursor(5,0);
                display.print(":");
                display.setCursor(6,0);
                display.setTextColor(BLACK, WHITE);
                display.print(seconds);
                display.setTextColor(WHITE);
                display.display();
              break;
              case 4: //set day
              break;
              case 5: //set month
              break;
              case 6: //set year
              break;
              default:
              break;
          }
          premuto = tastiera();
          
        } while (stfun > 0);
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
                  day ++;
                  hours = 0;
                  if (day>monthdays[month])
                  {
                    if (!((day = 29) && (month = 2) && ((year = 2020) || (year = 2024) || (year = 2028) || (year = 2032)))) 
                    {
                      month ++;
                      day = 1;
                      if (month>12)
                      {
                        year ++;
                        month = 1;
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
  if (day<10){
    ct = String(ct + "0");
  }
  ct = String(ct + day + "/");

  if (month<10){
    ct = String(ct + "0");
  }
  ct = String(ct + month + "/" + year);

  return ct;
}

int tastiera(){
  int keyp;
  int pressed = 0;
  delay(100);
  keyp = analogRead(A6);
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

