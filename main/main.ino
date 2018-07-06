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
const int REED_PIN_0 = A0; // Pin connected to reed switch 0
const int REED_PIN_1 = A1; // Pin connected to reed switch 1
const int REED_PIN_2 = A2; // Pin connected to reed switch 2
const int REED_PIN_3 = A3; // Pin connected to reed switch 3
const int REED_PIN_4 = A4; // Pin connected to reed switch 4
const int REED_PIN_5 = A5; // Pin connected to reed switch 5

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
  musicPlayer.setVolume(20,20);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Since the other end of the reed switch is connected to ground, we need
  // to pull-up the reed switch pin internally.
  pinMode(REED_PIN_0, INPUT_PULLUP);
//  pinMode(REED_PIN_1, INPUT_PULLUP);
//  pinMode(REED_PIN_2, INPUT_PULLUP);
//  pinMode(REED_PIN_3, INPUT_PULLUP);
//  pinMode(REED_PIN_4, INPUT_PULLUP);
//  pinMode(REED_PIN_5, INPUT_PULLUP);
}

void loop() {

  if (digitalRead(REED_PIN_0) == LOW) // If the pin reads low, the switch is closed.
  {
    Serial.println("Switch 0 closed");
    // Play one file, don't return until complete
    musicPlayer.playFullFile("track001.mp3");
  }

  if (digitalRead(REED_PIN_1) == LOW) // If the pin reads low, the switch is closed.
  {
    Serial.println("Switch 1 closed");
    // Play one file, don't return until complete
    musicPlayer.playFullFile("track002.mp3");
  }
//
//   if (digitalRead(REED_PIN_2) == LOW) // If the pin reads low, the switch is closed.
//  {
//    Serial.println("Switch 2 closed");
//    // Play one file, don't return until complete
//    musicPlayer.playFullFile("track001.mp3");
//  }
//
//   if (digitalRead(REED_PIN_3) == LOW) // If the pin reads low, the switch is closed.
//  {
//    Serial.println("Switch 3 closed");
//    // Play one file, don't return until complete
//    musicPlayer.playFullFile("track001.mp3");
//  }
//
//   if (digitalRead(REED_PIN_4) == LOW) // If the pin reads low, the switch is closed.
//  {
//    Serial.println("Switch 4 closed");
//    // Play one file, don't return until complete
//    musicPlayer.playFullFile("track001.mp3");
//  }
//
//   if (digitalRead(REED_PIN_5) == LOW) // If the pin reads low, the switch is closed.
//  {
//    Serial.println("Switch 5 closed");
//    // Play one file, don't return until complete
//    musicPlayer.playFullFile("track001.mp3");
//  }
}

