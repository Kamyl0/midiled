#include <FastLED.h>

#define NUM_LEDS 120
#define DATA_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

const int GledIn = 8;
const int BledIn = 12;
const int RledIn = 13;   //connection LED Ctrl

const int roundled1 = 10;
const int roundled2 = 11;

const int GSledIn = 3;
const int BSledIn = 6;
const int RSledIn = 9;    //connection LED Strip 12v

char inChar;    // string to hold input
String notepitch;
String notevelocity;

int inData[4];
int index;
boolean NoteOnStarted = false;
boolean NoteOnEnded = false;

boolean NoteOffStarted = false;
boolean NoteOffEnded = false;

int noteoff[128];


int soundout = 7;

boolean settingin = false;
boolean settingend = false;

//int decaysettingvalue[4]; //default decay
int setdec;
int setrc;
int setgc;
int setbc;

int decaytime = 10;  //default to 10
int ledbrightness = 0;
int fadeamount = 1;

boolean loopbreak = false;
int looptour = 0;

bool noteonoff = 0;

char val; // Data received from the serial port

void setup() {
  Serial.begin(19200);
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  analogWrite(GSledIn, 255);
  analogWrite(RSledIn, 255);
  analogWrite(BSledIn, 255);

  analogWrite(GledIn, 0);
  analogWrite(RledIn, 255);
  analogWrite(BledIn, 0);

  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i].r = 255;
    leds[i].g = 0;
    leds[i].b = 0;
    // Show the leds
    FastLED.show();
    delay(1);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i].r = 0;
    leds[i].g = 255;
    leds[i].b = 0;
    // Show the leds
    FastLED.show();
    delay(1);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i].r = 0;
    leds[i].g = 0;
    leds[i].b = 255;
    // Show the leds
    FastLED.show();
    delay(1);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i].r = 150;
    leds[i].g = 150;
    leds[i].b = 150;
    // Show the leds
    FastLED.show();
    delay(1);
  }

  //light before contact

  establishContact();

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);   //define output

  analogWrite(GledIn, 255);
  analogWrite(RledIn, 0);
  analogWrite(BledIn, 0);

  analogWrite(GSledIn, 0);
  analogWrite(RSledIn, 0);
  analogWrite(BSledIn, 0);

  analogWrite(roundled1, 0);
  analogWrite(roundled2, 0);
  //off after contact

  for (int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red
    leds[i] = CHSV(0, 0, 0);
    // Show the leds
    FastLED.show();
    delay(1);
  }

  tone(soundout, 400, 200); //make sound when inititialized

  Serial.println("LEO Initialized");
}

/*************FUNCTION AREA************/
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("LEO No data incoming from Processing");
    delay(300);
  }
}


void analysemididata(int midipitch, int midivelocity, int midichannel, int notestatus) {   //ANAYLSE DATA AND SEND TO LED FNC
reset:
  int settingdata = setdec;
  int rdata = setrc;
  int gdata = setgc;
  int bdata = setbc;

  int rgbcolor = map(midipitch, 0, 127 , 0 , 255);                         // data received from processing, mapped to match arduino
  int nmbrled = midipitch;



  if (notestatus == 1) {
    ledbrightness = map(midivelocity, 0, 127 , 0 , 255);
  }


  /***********LED STRIP PRGMBLE FNC****************/

  if (midichannel == 1) {
    if (notestatus == 1) {                              //FCN TO TURN ON THE LIGHT
      leds[nmbrled].r = rdata;
      leds[nmbrled].g = gdata;
      leds[nmbrled].b = bdata;

      FastLED.show();
    }
  }

  /***********LED STRIP ALL FNC****************/

  if (midichannel == 2) {
    if (notestatus == 1) {                              //FCN TO TURN ON THE LIGHT
      analogWrite(RSledIn, rdata);
      analogWrite(GSledIn, gdata);
      analogWrite(BSledIn, bdata);
    }

    if (notestatus == 2) {                              //FCN TO TURN ON THE LIGHT
      while (ledbrightness > 0) {
        ledbrightness += -decaytime;
        delay(2);
        if (ledbrightness < 0) {
          ledbrightness = 0;
        }
        analogWrite(RSledIn, map(ledbrightness, 0, 255, 0, rdata));
        analogWrite(GSledIn, map(ledbrightness, 0, 255, 0, gdata));
        analogWrite(BSledIn, map(ledbrightness, 0, 255, 0, bdata));

        int inChar = Serial.read();
        if (inChar != -1) {               //If new data coming Exit Function and reset LOOP //Avoid lagging and bug with this
          loopbreak = true;
          looptour ++;
          Serial.println("Loop Tour:");
          Serial.println(looptour);
          goto reset;
          break;
        }
      }
    }
  }
}


void updatledstripfnc() {

  if (noteoff[0] == 1 ) {
    int n = 0;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[1] == 1 ) {
    int n = 1;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[2] == 1 ) {
    int n = 2;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[3] == 1 ) {
    int n = 3;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[4] == 1 ) {
    int n = 4;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[5] == 1 ) {
    int n = 5;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[6] == 1 ) {
    int n = 6;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[7] == 1 ) {
    int n = 7;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[8] == 1 ) {
    int n = 8;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[9] == 1 ) {
    int n = 9;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[10] == 1 ) {
    int n = 10;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[11] == 1 ) {
    int n = 11;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[12] == 1 ) {
    int n = 12;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[13] == 1 ) {
    int n = 13;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[14] == 1 ) {
    int n = 14;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[15] == 1 ) {
    int n = 15;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[16] == 1 ) {
    int n = 16;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[17] == 1 ) {
    int n = 17;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[18] == 1 ) {
    int n = 18;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[19] == 1 ) {
    int n = 19;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[20] == 1 ) {
    int n = 20;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[21] == 1 ) {
    int n = 21;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[22] == 1 ) {
    int n = 22;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[23] == 1 ) {
    int n = 23;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }
  if (noteoff[24] == 1 ) {
    int n = 24;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[25] == 1 ) {
    int n = 25;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[26] == 1 ) {
    int n = 26;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[27] == 1 ) {
    int n = 27;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }
  if (noteoff[28] == 1 ) {
    int n = 28;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[29] == 1 ) {
    int n = 29;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[30] == 1 ) {
    int n = 30;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }
  if (noteoff[31] == 1 ) {
    int n = 31;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[32] == 1 ) {
    int n = 32;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[33] == 1 ) {
    int n = 33;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[34] == 1 ) {
    int n = 34;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[35] == 1 ) {
    int n = 35;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[36] == 1 ) {
    int n = 36;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[37] == 1 ) {
    int n = 37;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[38] == 1 ) {
    int n = 38;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }
  if (noteoff[39] == 1 ) {
    int n = 39;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[40] == 1 ) {
    int n = 40;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[41] == 1 ) {
    int n = 41;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[42] == 1 ) {
    int n = 42;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[43] == 1 ) {
    int n = 43;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[44] == 1 ) {
    int n = 44;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[45] == 1 ) {
    int n = 45;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[46] == 1 ) {
    int n = 46;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[47] == 1 ) {
    int n = 47;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[48] == 1 ) {
    int n = 48;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[49] == 1 ) {
    int n = 49;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[50] == 1 ) {
    int n = 50;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[51] == 1 ) {
    int n = 51;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[52] == 1 ) {
    int n = 52;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[53] == 1 ) {
    int n = 53;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[54] == 1 ) {
    int n = 54;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[55] == 1 ) {
    int n = 55;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[56] == 1 ) {
    int n = 56;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[57] == 1 ) {
    int n = 57;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[58] == 1 ) {
    int n = 58;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[59] == 1 ) {
    int n = 59;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[60] == 1 ) {
    int n = 60;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[61] == 1 ) {
    int n = 61;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[62] == 1 ) {
    int n = 62;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }

    FastLED.show();

  }

  if (noteoff[63] == 1 ) {
    int n = 63;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[64] == 1 ) {
    int n = 64;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[65] == 1 ) {
    int n = 65;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[66] == 1 ) {
    int n = 66;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[67] == 1 ) {
    int n = 67;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[68] == 1 ) {
    int n = 68;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[69] == 1 ) {
    int n = 69;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[70] == 1 ) {
    int n = 70;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[71] == 1 ) {
    int n = 71;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[72] == 1 ) {
    int n = 72;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[73] == 1 ) {
    int n = 73;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[74] == 1 ) {
    int n = 74;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[75] == 1 ) {
    int n = 75;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[76] == 1 ) {
    int n = 76;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[77] == 1 ) {
    int n = 77;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[78] == 1 ) {
    int n = 78;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[79] == 1 ) {
    int n = 79;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[80] == 1 ) {
    int n = 80;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[81] == 1 ) {
    int n = 81;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[82] == 1 ) {
    int n = 82;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[83] == 1 ) {
    int n = 83;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[84] == 1 ) {
    int n = 84;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[85] == 1 ) {
    int n = 85;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[86] == 1 ) {
    int n = 86;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[87] == 1 ) {
    int n = 87;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[88] == 1 ) {
    int n = 88;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[89] == 1 ) {
    int n = 89;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[90] == 1 ) {
    int n = 90;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[91] == 1 ) {
    int n = 91;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[92] == 1 ) {
    int n = 92;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[93] == 1 ) {
    int n = 93;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[94] == 1 ) {
    int n = 94;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[95] == 1 ) {
    int n = 95;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[96] == 1 ) {
    int n = 96;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[97] == 1 ) {
    int n = 97;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[98] == 1 ) {
    int n = 98;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[99] == 1 ) {
    int n = 99;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[100] == 1 ) {
    int n = 100;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[101] == 1 ) {
    int n = 101;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[102] == 1 ) {
    int n = 102;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[103] == 1 ) {
    int n = 103;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[104] == 1 ) {
    int n = 104;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[105] == 1 ) {
    int n = 105;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[106] == 1 ) {
    int n = 106;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[107] == 1 ) {
    int n = 107;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[108] == 1 ) {
    int n = 108;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[109] == 1 ) {
    int n = 109;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[110] == 1 ) {
    int n = 110;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[111] == 1 ) {
    int n = 111;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[112] == 1 ) {
    int n = 112;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[113] == 1 ) {
    int n = 113;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[114] == 1 ) {
    int n = 114;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[115] == 1 ) {
    int n = 115;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[116] == 1 ) {
    int n = 116;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[117] == 1 ) {
    int n = 117;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[118] == 1 ) {
    int n = 118;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[119] == 1 ) {
    int n = 119;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[120] == 1 ) {
    int n = 120;
    if (leds[n].r > 10) {
      leds[n].r -= decaytime;
    } else {
      leds[n].r = 0;
    }
    if (leds[n].g > 10) {
      leds[n].g -= decaytime;
    } else {
      leds[n].g = 0;
    }
    if (leds[n].b > 10) {
      leds[n].b -= decaytime;
    } else {
      leds[n].b = 0;
    }
    FastLED.show();
  }
}

void loop() {
  //Serial.println("hello from arduino");
  /********************UPDATE SINGLE LED**************************/
  updatledstripfnc();

  /***********READ DATA FROM PROCESSING*************/

  while (Serial.available() > 0)
  {

    /*** RECEIVE AND SORT PROCESSING SERIAL DATA INCOMING ****/
    int inChar = Serial.read();
    /**** MIDI NOTE ON INCOMING FROM PROCESSING SORTING ***/
    //  Serial.println(inChar);
    if (inChar == '<' && NoteOnStarted != true && settingin != true && NoteOffStarted != true)
    {
      NoteOnStarted = true;
      index = 0;
      inData[index] = '\0';
    }
    else if (inChar == '>' && index >= 2)
    {
      NoteOnEnded = true;
    }
    else if (NoteOnStarted)
    {
      inData[index] = inChar;
      index++;
      inData[index] = '\0';
    }

    /**** MIDI NOTE OFF INCOMING FROM PROCESSING SORTING ***/

    if (inChar == '#' && NoteOffStarted != true && settingin != true && NoteOnStarted != true)
    {
      NoteOffStarted = true;
      index = 0;
      inData[index] = '\0';
    }
    else if (inChar == '%' && index >= 2)
    {
      NoteOffEnded = true;
    }
    else if (NoteOffStarted)
    {
      inData[index] = inChar;
      index++;
      inData[index] = '\0';
    }

    /**** SETTING INCOMING FROM PROCESSING SORTING ***/

    if (inChar == '$' && NoteOnStarted != true && NoteOffStarted != true)
    {
      settingin = true;
    }
    else if (settingin)
    {
      inData[index] = inChar;
      index++;
      //inData[index] = '\0';
    }
    if (inChar == '@' && index >= 3)
    {
      settingend = true;
    }


    /**** SENDING DATA TO FNC ***/

    if (settingin && settingend)
    {
      setdec = inData[0];
      setrc = inData[1];
      setgc = inData[2];
      setbc = inData[3];

      decaytime = map(setdec, 1, 10, 10, 1);                        //Set Global DECAY TIME
      
      settingin = false;
      settingend = false;
      index = 0;
      inData[index] = '\0';
      Serial.println("LEO: Settings Updated");
    }

    if (NoteOffStarted && NoteOffEnded)
    {
      analysemididata(inData[0], inData[1], inData[2], 2); //data 1 is pitch data 2 is velocity
      noteoff[inData[0]] = 1;                             //global INT note ON
      NoteOffStarted = false;
      NoteOffEnded = false;
      index = 0;
      inData[index] = '\0';
    }

    if (NoteOnStarted && NoteOnEnded)
    {
      analysemididata(inData[0], inData[1], inData[2], 1); //data 1 is pitch data 2 is velocity
      noteoff[inData[0]] = 2;                               //global INT note OFF
      NoteOnStarted = false;
      NoteOnEnded = false;
      index = 0;
      inData[index] = '\0';
    }
    //delay(10); // Wait 10 milliseconds for next reading
  }
}
