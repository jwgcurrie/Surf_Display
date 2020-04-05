#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#include <RTClib.h>
#include <Wire.h>

DateTime now;
RTC_DS3231 rtc;
void showDate(void);
void showTime(void);
void showDay(void); 
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22

int ledPin1 = 8;
int ledPin2 = 9;

const int buttonPin1 = A0;  
const int buttonPin2 = A1;  
const int buttonPin3 = A2;  
const int buttonPin4 = A3;  
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;         // variable for reading the pushbutton status
int buttonState3 = 0;         // variable for reading the pushbutton status
int buttonState4 = 0;         // variable for reading the pushbutton status


static const int DHT_SENSOR_PIN = 7;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

int mode = 1;

unsigned long int start_hour;
unsigned long int start_minute;
unsigned long int start_second;


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() 
{
    pinMode(buttonPin1, INPUT);
    pinMode(buttonPin2, INPUT);
    pinMode(buttonPin3, INPUT);
    pinMode(buttonPin4, INPUT);


    pinMode(ledPin1, OUTPUT);    // sets the digital pin 13 as output LED
    pinMode(ledPin2, OUTPUT);    // sets the digital pin 13 as output LED

    if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC Module");
    while (1);
  }
    if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Setup Modes:
  bool intro_screen = false;
  bool Serial_Enabled = true;
  bool LCD_Enabled = true;
  // Begin Output Communications
  if(LCD_Enabled)
  {
    lcd.begin(16, 4);
  }
  if(Serial_Enabled)
  {
    Serial.begin(9600);
  }
  Wire.begin();

 
  startup_display(intro_screen);
  getTime();

}



void loop() 
{
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);

if(buttonState4 == HIGH && mode == 1)
{
  mode = 2;
  Serial.print("Going to Shaping Mode");
  delay(300);
      lcd.clear();


}
else if(buttonState4==HIGH && mode ==2)
{
  mode = 1;
  Serial.print("Going to Forcasting Mode");

  delay(300);
      lcd.clear();

}
  now = rtc.now();
  //showDate();
  //showDay();
  float temperature;
  float humidity;
if(mode==1)
{
    lcd.setCursor(0, 0);
    lcd.print("Mode: Forcast");
    lcd.setCursor(0,1);
    
}
else if(mode==2)
{
    showTime();
  if( measure_environment( &temperature, &humidity ) == true )
  {
  lcd.setCursor(0, 0);
  lcd.print("Mode: Shaping");
  lcd.setCursor(0,1);
  lcd.print("Temp:");
  lcd.setCursor(5,1);
  lcd.print(temperature, 1 );
  lcd.setCursor(10,1);
  lcd.print("C");
  lcd.setCursor(0,2);
  lcd.print("Humd:");
  lcd.setCursor(5,2);
  lcd.print(humidity, 1 );
  lcd.setCursor(10,2);
  lcd.print("%");
  LED(temperature);
  }
}
 
}
}

void startup_display(bool intro_screen)
{
  if(intro_screen)
    {
    lcd.print("Surf Thermometer");
    lcd.setCursor(0, 1);
    lcd.print("Version: 1.1");
    lcd.setCursor(2, 2);
    lcd.print("Created by:");
    lcd.setCursor(2,3);
    lcd.print("Joel Currie");
    delay(2000);
    int i;
    int x;
    for(x = 0; x < 16; x++)
    {
    lcd.setCursor(x,0);
    lcd.print("   #");
    lcd.setCursor(x,1);
    lcd.print("  ##");
    lcd.setCursor(x,2);
    lcd.print(" ## ");
    lcd.setCursor(x,3);
    lcd.print("####");
    delay(200);
    }
    x = 0;
    lcd.setCursor(x,0);
    lcd.print("000 0   000 0  0");
    lcd.setCursor(x,1);
    lcd.print("0 0 000 0 0 00 0");
    lcd.setCursor(x,2);
    lcd.print("000 0 0 0 0 0 00");
    lcd.setCursor(x,3);
    lcd.print("0 0 000 000 0  0");
    delay(2000);
    
    lcd.setCursor(x,0);
    lcd.print("000 0  0 000 000");
    lcd.setCursor(x,1);
    lcd.print("00  0  0 0 0 0  ");
    lcd.setCursor(x,2);
    lcd.print(" 00 0  0 000 000");
    lcd.setCursor(x,3);
    lcd.print("000 0000 0 0 0   ");
    delay(2000);
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("Debugging Mode");
      delay(500);
    }
    lcd.clear();

}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }
  return( false );
}

 void showDate()
 {
  lcd.setCursor(0,0);
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
 }
 
 void showDay()
 {
  lcd.setCursor(11,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
 }
 
 void showTime()
 {
  lcd.setCursor(0,3);
  lcd.print("Time:");
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
  lcd.print("    ");
 } 

  void getTime()
 {
  start_hour = now.hour();
  start_minute = now.minute();
  start_second = now.second();
 } 

 void calculateTime()
 {
  lcd.setCursor(0,3);
  lcd.print(now.hour() - start_hour);
  lcd.setCursor(4,3);
  lcd.print(now.minute() - start_minute);
  lcd.setCursor(8,3);
  lcd.print(now.second() - start_second);
 }

 void LED(float temperature)
{
  if((temperature >= 22.0) && (temperature <= 30.0))
  {
  digitalWrite(ledPin1, HIGH);   // sets the LED on
  digitalWrite(ledPin2, LOW);   // sets the LED on

  }
  else if((temperature < 22.0) || (temperature > 30.0))
  {
  digitalWrite(ledPin1, LOW);    // sets the LED off
  digitalWrite(ledPin2, HIGH);    // sets the LED off

  }
  else
  {
    Serial.println("Error in LED");
    Serial.println(temperature);
  }
}
 
