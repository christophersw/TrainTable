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
#define BREAKOUT_RESET 9 // VS1053 reset pin (output)
#define BREAKOUT_CS 10   // VS1053 chip select pin (output)
#define BREAKOUT_DCS 8   // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET -1 // VS1053 reset pin (unused!)
#define SHIELD_CS 7     // VS1053 chip select pin (output)
#define SHIELD_DCS 6    // VS1053 Data/command select pin (output)

const int REED_PIN_0 = A8;  // Docks
const int REED_PIN_1 = A9;  // Railroad Crossing
const int REED_PIN_2 = A10; // Mountain Tunnel
const int REED_PIN_3 = A11; // Station
const int REED_PIN_4 = A12; // Viaduct Bridge
const int REED_PIN_5 = A13; // Seaside
const int LED_PIN_1 = A14;  // Mountain LED
const int LED_PIN_2 = A15;  // Mountain LED

//Audio file clip name prefix
const String GENERAL = "GENRL-";
const String DOCKS = "DOCKS-";
const String CROSSING = "CROSS-";
const String MOUNTAIN = "MOUNT-";
const String STATION = "STATN-";
const String VIADUCT = "BRIDG-";
const String SEASIDE = "CSIDE-";

// Number of Audio Files
int GENERAL_MAX = 0;
int DOCKS_MAX = 0;
int CROSSING_MAX = 0;
int MOUNTAIN_MAX = 0;
int STATION_MAX = 0;
int VIADUCT_MAX = 0;
int SEASIDE_MAX = 0;

// Percentage of the time we should play a "general clip"
const int GENERAL_PERCENTAGE = 20;

// These are common pins between breakout and shield
#define CARDCS 4 // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3 // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
    // create breakout-example object!
    // Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
    // create shield-example object!
    Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void setup()
{
  Serial.begin(9600);
  Serial.println("Setting things up.");

  if (!musicPlayer.begin())
  { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1)
      ;
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS))
  {
    Serial.println(F("SD failed, or not present"));
    while (1)
      ; // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10, 10);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Since the other end of the reed switch is connected to ground, we need
  // to pull-up the reed switch pin internally.
  Serial.println("Setting up sensors and lights.");
  pinMode(REED_PIN_0, INPUT_PULLUP); // Docks
  pinMode(REED_PIN_1, INPUT_PULLUP); // Railroad Crossing
  pinMode(REED_PIN_2, INPUT_PULLUP); // Mountain Tunnel
  pinMode(REED_PIN_3, INPUT_PULLUP); // City Station
  pinMode(REED_PIN_4, INPUT_PULLUP); // Viaduct Bridge
  pinMode(REED_PIN_5, INPUT_PULLUP); // Seaside
  pinMode(LED_PIN_1, OUTPUT);        // Mountain LED 1
  pinMode(LED_PIN_2, OUTPUT);        // Mountain LED 2

  // list files
  countClips(SD.open("/"));

  Serial.println("Setup complete. All Aboard!");
}

// Last triggered needs to be different from next triggered to catch "parking".
int lastPlayed = 0;
int flickerTimer = 0;

void loop()
{
  Serial.println(lastPlayed);

  if (flickerTimer = 9000)
  {
    analogWrite(LED_PIN_1, random(120, 255));
    analogWrite(LED_PIN_2, random(120, 255));
    flickerTimer = 0;
  }
  else
  {
    flickerTimer = flickerTimer + 1;
  }

  /*************
   * 0 - DOCKS *
   *************/
  if (digitalRead(REED_PIN_0) == LOW && lastPlayed != 0)
  {
    Serial.println("Train at the docks.");
    lastPlayed = 0;

    playFile(DOCKS, DOCKS_MAX);
  }

  /****************
   * 1 - CROSSING *
   ****************/
  if (digitalRead(REED_PIN_1) == LOW && lastPlayed != 1)
  {
    Serial.println("Train at the crossing.");
    lastPlayed = 1;
    playFile(CROSSING, CROSSING_MAX);
  }

  /****************
   * 2 - MOUNTAIN *
   ****************/
  if (digitalRead(REED_PIN_2) == LOW && lastPlayed != 2)
  {
    Serial.println("Train at the mountain tunnel.");
    lastPlayed = 2;
    playFile(MOUNTAIN, MOUNTAIN_MAX);
  }

  /****************
   * 3 - STATION *
   ****************/
  if (digitalRead(REED_PIN_3) == LOW && lastPlayed != 3)
  {
    Serial.println("Train at the station.");
    lastPlayed = 3;
    playFile(STATION, STATION_MAX);
  }

  /*********************
   * 4 - VIADUCT BRIDGE*
   *********************/
  if (digitalRead(REED_PIN_4) == LOW && lastPlayed != 4)
  {
    Serial.println("Train at the viaduct bridge.");
    lastPlayed = 4;
    playFile(VIADUCT, VIADUCT_MAX);
  }

  /****************
   * 5 - SEASIDE *
   ****************/
  if (digitalRead(REED_PIN_5) == LOW && lastPlayed != 5)
  {
    Serial.println("Train at the seaside.");
    lastPlayed = 5;
    playFile(SEASIDE, SEASIDE_MAX);
  }
}

/// File listing helper
void countClips(File dir)
{
  Serial.println("Counting files.");
  while (true)
  {

    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    
    if (!entry.isDirectory())
    {
      Serial.println(entry.name());
      String nameStr = String(entry.name());
      if (nameStr.startsWith(GENERAL))
      {
        GENERAL_MAX++;
      }

      if (nameStr.startsWith(DOCKS))
      {
        DOCKS_MAX++;
      }

      if (nameStr.startsWith(CROSSING))
      {
        CROSSING_MAX++;
      }

      if (nameStr.startsWith(MOUNTAIN))
      {
        MOUNTAIN_MAX++;
      }

      if (nameStr.startsWith(STATION))
      {
        STATION_MAX++;
      }

      if (nameStr.startsWith(VIADUCT))
      {
        VIADUCT_MAX++;
      }

      if (nameStr.startsWith(SEASIDE))
      {
        SEASIDE_MAX++;
      }
    }
    entry.close();
  }

  Serial.println("*** FILE COUNTS *** ");
  Serial.print("General clips: ");
  Serial.println(GENERAL_MAX);
  Serial.print("Dock clips: ");
  Serial.println(DOCKS_MAX);
  Serial.print("Crossing clips: ");
  Serial.println(CROSSING_MAX);
  Serial.print("Mountain clips: ");
  Serial.println(MOUNTAIN_MAX);
  Serial.print("Station clips: ");
  Serial.println(STATION_MAX);
  Serial.print("Viaduct clips: ");
  Serial.println(VIADUCT_MAX);
  Serial.print("Seaside clips: ");
  Serial.println(SEASIDE_MAX);
  Serial.println("****************** ");

}

void playFile(String stop, int max)
{
  int x = random(1,100);
  String file = "";
  char fileCharArray[13];

  if(random(1,100) <= GENERAL_PERCENTAGE) {
    file = GENERAL;
    file += random(1, GENERAL_MAX);
    file += ".mp3";
  } else {
    file = stop;
    file += random(1, max);
    file += ".mp3";
  }

  file.toCharArray(fileCharArray, 13);
  Serial.print("Playing file: ");
  Serial.println(fileCharArray);
  musicPlayer.playFullFile(fileCharArray);
}
