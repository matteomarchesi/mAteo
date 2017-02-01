#include <SFE_BMP180.h>
#include <Wire.h>
#include <mAteo_DHT.h>
#include <mAteo_GFX.h>
#include <mAteo_SSD1306.h>
#include <EEPROM.h>

/*
 *  Keyboard
 */

#define KEYPIN A6 
#define KEY5 1023
#define KEY4 990
#define KEY3 930
#define KEY2 700
#define KEY1 512

/*
 *  Clock definitions 
 *  Connect D2 with D3 on Arduino
 */

uint8_t clockInterrupt = 0; //interrupt 0 is pin 2 on UNO
uint8_t pwmOut = 3; //pin D3

int cyclesPerSecond = 490;

volatile int masterClock = 0;

volatile int8_t seconds, minutes, hours, dd, mm;
volatile int yyyy;

uint8_t mmdds[] = {31,28,31,30,31,30,31,31,30,31,30,31};

String clocktime = ""; 

String printTime2();

/*
 *  DHT22 (temperature and humidity) definitions
 */
#define DHTTYPE DHT22 
#define DHTPIN 9

mAteo_DHT dht(DHTPIN, DHTTYPE);

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
mAteo_SSD1306 display(OLED_RESET);

  char status;
  double P, h, t;
//  double T;

/*
 *   grafici
 */

#define DATASTORED 48
#define DATASTOREDSIZE 6
uint8_t mempointer = 0;

typedef struct {
  int tem;
  int hum;
  int pre;
} graph_data;

graph_data gdata;

uint8_t stored = 0;

/*
 *  altro
 */

unsigned long previous = 0;
const int interval = 4000;

void schermo();

void spento();

void tupo(); //temperatura umidità pressione ora

void graphE();

void set_clock();

void readSensors();

uint8_t tastiera();

int8_t funzione = 0;
uint8_t premuto = 0;
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

  String ora = __TIME__;
  String data = __DATE__;
  seconds = ora.substring(6,8).toInt();
  minutes = ora.substring(3,5).toInt();
  hours = ora.substring(0,2).toInt();

  dd = data.substring(4,6).toInt();
  yyyy = data.substring(7,11).toInt();
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

  // clockInterrupt is our interrupt, clockCounter() function is called when invoked on a RISING clock edge
  attachInterrupt(clockInterrupt, clockCounter, RISING);

  delay(2000);
  analogReference(DEFAULT);//not sure if this is needed
  analogWrite(pwmOut, 127); // this starts our PWM 'clock' with a 50% duty cycle

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
    Serial.println("BMP180 init fail");
    while(1); // Pause forever.
  }
/*
 * reset the memory
  gdata.tem = 0;
  gdata.hum = 0;
  gdata.pre = 0;
*/  
  EEPROM.get(1023,mempointer);

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

//  if (((seconds == 0) || (seconds == 5) || (seconds == 10) || (seconds == 15) || (seconds == 20) || (seconds == 25) || (seconds == 30) || (seconds == 35) || (seconds == 40) || (seconds == 45) || (seconds == 50) || (seconds == 55)) && (stored == 0)) {
  if (((minutes == 0) || (minutes == 30)) && (stored == 0)) {
      gdata.tem = (int) (t*10);
      gdata.hum = (int) (h*10);
      gdata.pre = (int) P; // no decimal
/* 
 *   mempointer points to next data to be written
 *   writes data in position 0, 1, 2 ...
 *  
 *  
 *   store position  0   1   2   3   4   5   6   7
 *   mempointer      x-1 x   P  older data
 *   for graph                   |--------------->
 *                   ------->P
 *  
 */      
      EEPROM.put(mempointer*DATASTOREDSIZE,gdata);
      mempointer++;
	    if(mempointer==DATASTORED){mempointer=0;}
      EEPROM.put(1023,mempointer);
      stored = 1;
  } else
      stored = 0;
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
        graphE(1, "Temperatura (dC)", 1, mempointer);
        break;
      case 3:
        graphE(2, "Umidita' (%RH)", 1, mempointer);
        break;
      case 4:
        graphE(3, "Pressione (mb)", 0, mempointer);
        break;
      case 5:
        set_clock();
        break;
        
      default:
        if (funzione < 0)
          {
            funzione = 5;
          }
        if (funzione > 5)
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
    display.println("C");
    
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

    clocktime = printTime2();
    display.setCursor(6,24);
    display.print(clocktime);
    display.display();
  }

String dataOut(int val, int r)
{
  // r=0 no decimal
  // r=1 1 decimal
  String printData;
  int in = (val/10);
  int de = val - in * 10;
  if (r==1)
    {printData = String(in) + '.' + String(de);}
  else
    {printData = val;};
  
  return printData;
}

int datafromeeprom(int wgraph, graph_data gdata){
  int rd = 0;
  switch (wgraph){
      case 1: // temperatura
        rd = gdata.tem;
        break;
      case 2: // temperatura
        rd = gdata.hum;
        break;
      case 3: // temperatura
        rd = gdata.pre;
        break;
    }
    return rd;
}

void graphE(int wgraph, String message, int r, int mempointer)
  {
	int vmin, vmax, pix, rd;
    float tick;
	graph_data gdata;
    uint8_t pnt, pntp, i, gi;
    // trova min e max nell'array
    // get last data
    if (mempointer==0){
      pnt = 0;
      pntp = DATASTORED-1;
    }else{
      pnt = mempointer;
      pntp = mempointer-1;
    }
    
    EEPROM.get((pntp*DATASTOREDSIZE),gdata);
    rd = datafromeeprom(wgraph,gdata);
    vmin = vmax = rd;
    i = pnt;
    while (i!=pntp){
      EEPROM.get((i*DATASTOREDSIZE),gdata);
      rd = datafromeeprom(wgraph,gdata);
      vmin=min(rd,vmin);
      vmax=max(rd,vmax);
      i++;
      if (i == DATASTORED){i = 0;}
    }

    tick = (vmax - vmin)/19.0;  // 20 lines graph
    if (vmax==vmin) {tick=0.1;};
    display.clearDisplay();
    i = pnt;
	  gi = 0;
    while (gi<DATASTORED){
      EEPROM.get((i*DATASTOREDSIZE),gdata);
      rd = datafromeeprom(wgraph,gdata);
      pix= -1*(((rd-vmin)/tick)-29);
      display.drawPixel(gi, pix, WHITE);
      Serial.println(i);
      i++;
	    gi++;
      if (i == DATASTORED){i = 0;}
    }

    display.setCursor(0,0);
    display.print(message);
    display.setCursor(90,8);
    display.print(dataOut(vmax, r));
    display.setCursor(90,23);
    display.print(dataOut(vmin, r));

    display.display();
  }


void set_clock()
{
    int stfun = 0;
    
    display.clearDisplay();
    clocktime = printTime2();
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
          clocktime = printTime2();
          display.print(clocktime);
          display.display();

          
          switch(premuto){
            case 1:
              switch(stfun){
                case 1: //hrs
                  hours++;
				  if (hours>23){hours=0;}
                  break;
                case 2: //min
                  minutes++;
				  if (minutes>59){minutes=0;}
                  break;
                case 3: //sec
                  seconds=0;
                  break;
                case 4: //dd
                  dd++;
                  if (!((dd == 29) && (mm == 2) && ((yyyy == 2020) || (yyyy == 2024) || (yyyy == 2028) || (yyyy == 2032)))){
					          if (dd>mmdds[mm-1]){dd=1;}
				          }
	                break;
                case 5: //mm
                  mm++;
				  if (mm>12){mm=1;}
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
				  if (hours<0){hours=23;}
                  break;
                case 2: //min
                  minutes--;
				  if (minutes<0){minutes=59;}
                  break;
                case 3: //sec
                  seconds=0;
                  break;
                case 4: //dd
                  dd--;
/*
 * missing management of leap year in dd decreemention (should not be a big issue)
 *                  if (!((dd == 29) && (mm == 2) && ((yyyy == 2020) || (yyyy == 2024) || (yyyy == 2028) || (yyyy == 2032)))){
 *					if (dd>mmdds[mm-1]){dd=1}
 *				  }
 */
				  if (dd<0){dd=mmdds[mm-1];}
                  break;
                case 5: //mm
                  mm--;
				  if (mm<0){mm=12;}
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
                  if (dd>mmdds[mm-1])
                  {
                    if (!((dd == 29) && (mm == 2) && ((yyyy == 2020) || (yyyy == 2024) || (yyyy == 2028) || (yyyy == 2032)))) 
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

String twodigits(int val){
  String td = "";
  if(val<10){
    td = "0";
  }
  td = String(td + val);
  return td;
}

String printTime2(){
  String ct;
  ct = String(twodigits(hours) + ":" + twodigits(minutes) + ":" + twodigits(seconds) + " " + twodigits(dd) + "/" + twodigits(mm) + "/" + twodigits(yyyy));
  return ct;  
}

uint8_t tastiera(){
  int keyp;
  uint8_t pressed = 0;
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

