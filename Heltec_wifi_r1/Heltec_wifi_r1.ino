/*********************************************************************

DBM Dec 2 2017
Adapted from all sorts of sources

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ESP8266WiFi.h"

#define OLED_RESET 16
Adafruit_SSD1306 display(OLED_RESET);

#define NUMLINES    4
#define LINELENGTH  20
#define FIRSTLINE   0
#define LASTLINE    NUMLINES-1
#define LINEHEIGHT  8

char oneline[LINELENGTH];

char lines[NUMLINES][LINELENGTH]=
{
    "",  // 0
    "",
    "",
    ""   // 3
};

int curline = FIRSTLINE; // line FIRSTLINE to LASTLINES
boolean firsttime = true;

//===========
void setup()   {                
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Clear the buffer.
  display.clearDisplay();

  // text display
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false),

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

}

/*
insert this line into the lines buffer
*/
void displayline(char *linep ) {
  Serial.println(linep);
  
  if (curline == LASTLINE) {
    if (firsttime) {
      // Serial.print("don't do anything the first time because we are still filling the buffer");
      firsttime = false;
    } 
    else {
      //not first time so move things up
      for (int i=FIRSTLINE; i<=NUMLINES-1; i++) {
        strcpy(lines[i], lines[i+1]); //copy [2] into [1]
      }
    }
  }
  strcpy(lines[curline], linep);
  
  writelines();

  // make sure current line does not exceed the last line
  if (++curline > LASTLINE) {
    curline = LASTLINE;
  }
}

//write all x lines every time
void writelines() {
  display.clearDisplay();
  for (int i=FIRSTLINE; i<=LASTLINE; i++) {
    display.setCursor(0,i*LINEHEIGHT);
    display.print(lines[i]);
  }
  display.display();
}

void scanWifi() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  //Serial.println("scan done");
  //sprintf(oneline, "line = %d", i);
  //displayline("scan done");
  if (n == 0)
    displayline("No networks found");
  else
  {
    sprintf(oneline, "%d networks found", n);
    displayline(oneline);
    delay(300);
//*    Serial.print(n);
//*    Serial.println(" ");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      // 1: MOTOROLA-BCD9A (-47)*
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      //sprintf(oneline, "%d: %s (%d)%s", i, WiFi.SSID(i), WiFi.RSSI(i), (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      //sprintf(oneline, "%d: %d (%d)", i, WiFi.SSID(i), WiFi.RSSI(i));
      sprintf(oneline, "%d: %s", i, "asda");
      displayline(oneline);
      delay(200);
    }
  }
  Serial.println("");

}
  

//===========
void loop() {
  for (int i=0; i<=99; ++i) {
    
    //oneline = "line =";
    sprintf(oneline, "line = %d", i);
    displayline(oneline);
    
    delay(1000); 
  }
  //scanWifi();
  delay(1000); 
}
