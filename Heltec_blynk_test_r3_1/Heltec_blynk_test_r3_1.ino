/*************************************************************
  DBM Dec 17 2017 -move to Heltec "wifi kit 8" wtih broken screen
  DBM Dec 10 2017 -remove DHT temp sensor stuff
  DBM Nov 18 2020 -just trying to get heltec and blynk working
  Note: 
    ESP8266 LED on board is Pin 2
    Wifi Kit 8 does not have an onboard LED
    
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into D6 on Wifi Kit 8
#define ONE_WIRE_BUS D6
// DS18B20 temp probe requires 4.7k resistor between output/yellow and VCC/red

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

const int green_led = D0; //offboard LED connected to GPIO16 which is D0
int timerId;

// You should get Auth Token in the Blynk App
// Go to the Project Settings (nut icon)
char auth[] = "fVQ67FcP5qthZaDWPU5jeyO-BAak53eC"; // DBM Heltec
//char auth[] = "a5f9e0e1996044f2b4c9d626eb120c94"; // DBM "Hot Tub Temperature"


// Your WiFi credentials
// Set password to "" for open networks.
char ssid[] = "lab-i5";
char pass[] = "a1b1c1d1e1";

BlynkTimer timer;
WidgetTerminal terminal(V2);
WidgetLED alive_led(V3);


//=======
// 
void fastblinkLed()
{
  if (digitalRead(green_led) == HIGH) {
      digitalWrite(green_led, LOW);
  } else {
      digitalWrite(green_led, HIGH);
  }
}

//=======
// V1 LED Widget is blinking
void blinkLedWidget()
{
  if (digitalRead(green_led) == HIGH) {
      alive_led.off();
      digitalWrite(green_led, LOW);
  } else {
      alive_led.on();
      digitalWrite(green_led, HIGH);
  }
}

//======
// Read the temp from the DS1820 and write it to the Blynk app
void readtemp()
{
  char Buffer[16];
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  float t_DS1820 = sensors.getTempCByIndex(0);
  dtostrf(t_DS1820, 1, 1, Buffer); //convert from float to string with on 1 decimal
  Serial.print("DS18B20 temp = "); 
  Serial.print(Buffer);  // getTempCByIndex for celcius
  Serial.println("°C");
  Blynk.virtualWrite(V4, Buffer); //First Blynk widget is a Super Graph
  Blynk.virtualWrite(V6, 20); //Second Blynk is a Gauge
}

// ========
// This function sends Arduino's up time to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void mainTimer()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis()/1000);
  Serial.print("Up time = ");
  Serial.print(millis()/1000);
  Serial.println("s");

  terminal.print("Up time = ");
  terminal.print(millis()/1000);
  terminal.println("s");
  // Ensure everything is sent
  terminal.flush();

  //go read the temp
  readtemp();
}

// ==============
void setup()
{
  // Blink the LED really fast until the connection to Blynk is made
  //timerId = timer.setInterval(100L, fastblinkLed);
  // timer.enable(timerId);  // Enables the specified timer
  //timer.run(); // Initiates BlynkTimer

  // Debug console
  Serial.begin(115200);

  // get the DS1820 ready
  sensors.begin();

  Blynk.begin(auth, ssid, pass);

  // Initialize LED as output    
  pinMode(green_led, OUTPUT);     
  digitalWrite(green_led, HIGH);

  // Setup a function to be called every X.Xs
  timer.setInterval(10000L, mainTimer);
  timer.setInterval( 1000L, blinkLedWidget);
}

// =======
void loop()
{
  Blynk.run(); // Sets up Blynk connection
  timer.run(); // Initiates BlynkTimer
  //  timer.disable(timerId);  // Disables the specified timer
}
