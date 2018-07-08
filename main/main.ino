/*************************************************** 
  Train Table
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

const int REED_PIN_0 = A8; // Docks
const int REED_PIN_1 = A9; // Railroad Crossing
const int REED_PIN_2 = A10; // Mountain Tunnel
const int REED_PIN_3 = A11; // Station
const int REED_PIN_4 = A12; // Viaduct Bridge
const int REED_PIN_5 = A13; // Seaside
const int LED_PIN_1 = A14; // Mountain LED
const int LED_PIN_2 = A15; // Mountain LED

//MP3s
const String DOCKS = "docks-";
const String CROSSING = "cross-";
const String MOUNTAIN = "mount-";
const String STATION = "statn-";
const String VIADUCT = "bridg-";
const String SEASIDE = "cside-";

const int DOCKS_MAX = 10;
const int CROSSING_MAX = 10;
const int MOUNTAIN_MAX = 10;
const int STATION_MAX = 10;
const int VIADUCT_MAX = 10;
const int SEASIDE_MAX = 17;


// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
// create breakout-example object!
// Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
// create shield-example object!
Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  
void setup() {
  Serial.begin(9600);
  Serial.println("Setting things up.");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10,10);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Since the other end of the reed switch is connected to ground, we need
  // to pull-up the reed switch pin internally.
  pinMode(REED_PIN_0, INPUT_PULLUP); // Docks
  pinMode(REED_PIN_1, INPUT_PULLUP); // Railroad Crossing
  pinMode(REED_PIN_2, INPUT_PULLUP); // Mountain Tunnel
  pinMode(REED_PIN_3, INPUT_PULLUP); // City Station
  pinMode(REED_PIN_4, INPUT_PULLUP); // Viaduct Bridge
  pinMode(REED_PIN_5, INPUT_PULLUP); // Seaside
  pinMode(LED_PIN_1, OUTPUT); // Mountain LED 1 
  pinMode(LED_PIN_2, OUTPUT); // Mountain LED 2
  Serial.println("Testing Playback 5");
  musicPlayer.playFullFile("docks-12.mp3");
  
}

// Last triggered needs to be different from next triggered to catch "parking".
int lastPlayed=0;
int flickerTimer=0;

void loop() {
   Serial.println(lastPlayed);
   
   if(flickerTimer = 9000) {
      analogWrite(LED_PIN_1, random(120,255));
      analogWrite(LED_PIN_2, random(120,255));
      flickerTimer = 0;
   } else {
    flickerTimer = flickerTimer + 1;
   }

  /*************
   * 0 - DOCKS *
   *************/
  if (digitalRead(REED_PIN_0) == LOW && lastPlayed != 0)
  {
    Serial.println("Train at the docks.");
    lastPlayed=0;

    
    String file = DOCKS;
    file += random(10, DOCKS_MAX);
    file += ".mp3";
    
    char fileCharArray[13];
    
    file.toCharArray(fileCharArray, 13);
    
    Serial.println(fileCharArray);
    musicPlayer.playFullFile(fileCharArray);
  }

  /****************
   * 1 - CROSSING *
   ****************/
  if (digitalRead(REED_PIN_1) == LOW && lastPlayed != 1)
  {
    Serial.println("Train at the crossing.");
    lastPlayed=1;

    String file = CROSSING;
    file += random(10, CROSSING_MAX);
    file += ".mp3";
    
    char fileCharArray[13];
    
    file.toCharArray(fileCharArray, 13);
    
    Serial.println(fileCharArray);
    
    musicPlayer.playFullFile(fileCharArray);
  }

  /****************
   * 2 - MOUNTAIN *
   ****************/
  if (digitalRead(REED_PIN_2) == LOW && lastPlayed != 2)
  {
    Serial.println("Train at the mountain tunnel.");
    lastPlayed=2;

    
    String file = MOUNTAIN;
    file += random(10, MOUNTAIN_MAX);
    file += ".mp3";
    
    char fileCharArray[13];
    
    file.toCharArray(fileCharArray, 13);
    
    Serial.println(fileCharArray);
    
    musicPlayer.playFullFile(fileCharArray);

    // TODO: Light up LED!
  }

  /****************
   * 3 - STATION *
   ****************/
  if (digitalRead(REED_PIN_3) == LOW && lastPlayed != 3)
  {
    Serial.println("Train at the station.");
    lastPlayed=3;

    
    String file = STATION;
    file += random(10, STATION_MAX);
    file += ".mp3";
    
    char fileCharArray[13];
    
    file.toCharArray(fileCharArray, 13);
    
    Serial.println(fileCharArray);
    
    musicPlayer.playFullFile(fileCharArray);
  }

  /*********************
   * 4 - VIADUCT BRIDGE*
   *********************/
  if (digitalRead(REED_PIN_4) == LOW && lastPlayed != 4)
  {
    Serial.println("Train at the viaduct bridge.");
    lastPlayed=4;

    
    String file = VIADUCT;
    file += random(10, VIADUCT_MAX);
    file += ".mp3";
    
    char fileCharArray[13];
    
    file.toCharArray(fileCharArray, 13);
    
    Serial.println(fileCharArray);
    
    musicPlayer.playFullFile(fileCharArray);
  }

  /****************
   * 5 - SEASIDE *
   ****************/
  if (digitalRead(REED_PIN_5) == LOW && lastPlayed != 5)
  {
    Serial.println("Train at the seaside.");
    lastPlayed=5;

    
    String file = SEASIDE;
    file += random(10, SEASIDE_MAX);
    file += ".mp3";
    
    char fileCharArray[13];
    
    file.toCharArray(fileCharArray, 13);
    
    Serial.println(fileCharArray);
    
    musicPlayer.playFullFile(fileCharArray);
  }
}

