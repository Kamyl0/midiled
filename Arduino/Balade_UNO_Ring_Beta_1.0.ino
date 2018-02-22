#include <FastLED.h>

#define NUM_LEDSRING 16
#define DATA_PIN_RING 10

// Define the array of leds
CRGB ledsring[NUM_LEDSRING];

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
  Serial.begin(9600);
  LEDS.addLeds<WS2812, DATA_PIN_RING, RGB>(ledsring, NUM_LEDSRING);


  for (int i = 0; i < NUM_LEDSRING; i++) {
    // Set the i'th led to red
    ledsring[i].r = 255;
    ledsring[i].g = 0;
    ledsring[i].b = 0;
    // Show the leds
    FastLED.show();
    delay(10);
  }

  for (int i = 0; i < NUM_LEDSRING; i++) {
    // Set the i'th led to red
    ledsring[i].r = 0;
    ledsring[i].g = 255;
    ledsring[i].b = 0;
    // Show the leds
    FastLED.show();
    delay(10);
  }

  for (int i = 0; i < NUM_LEDSRING; i++) {
    // Set the i'th led to red
    ledsring[i].r = 0;
    ledsring[i].g = 0;
    ledsring[i].b = 255;
    // Show the leds
    FastLED.show();
    delay(10);
  }

  for (int i = 0; i < NUM_LEDSRING; i++) {
    // Set the i'th led to red
    ledsring[i].r = 150;
    ledsring[i].g = 150;
    ledsring[i].b = 150;
    // Show the leds
    FastLED.show();
    delay(10);
  }

  //light before contact

  establishContact();


  for (int i = 0; i < NUM_LEDSRING; i++) {
    // Set the i'th led to red
    ledsring[i] = CHSV(0, 0, 0);
    // Show the leds
    FastLED.show();
    delay(1);
  }

  Serial.println("UNO Initialized");
}

/*************FUNCTION AREA************/
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("UNO No data incoming from Processing");
    delay(300);
  }
}


void analysemididata(int midipitch, int midivelocity, int midichannel, int notestatus) {   //ANAYLSE DATA AND SEND TO LED FNC
reset:
  int settingdata = setdec;
  int rdata = setrc;
  int gdata = setgc;
  int bdata = setbc;

  int nmbrled = midipitch;

  if (notestatus == 1) {
    ledbrightness = map(midivelocity, 0, 127 , 0 , 255);
  }


  /***********LED STRIP PRGMBLE RING CHANNEL 4****************/

  if (midichannel == 4) {
    if (notestatus == 1) {                              //FCN TO TURN ON THE LIGHT
      ledsring[nmbrled].r = map(ledbrightness, 0, 255, 0, gdata);       //bug color R/G inverted on the ring
      ledsring[nmbrled].g = map(ledbrightness, 0, 255, 0, rdata);
      ledsring[nmbrled].b = map(ledbrightness, 0, 255, 0, bdata);

      FastLED.show();
    }
  }
}


void updatledstripfnc() {


  if (noteoff[1] == 1 ) {
    int n = 0;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[1] == 1 ) {
    int n = 1;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[2] == 1 ) {
    int n = 2;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[3] == 1 ) {
    int n = 3;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[4] == 1 ) {
    int n = 4;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[5] == 1 ) {
    int n = 5;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[6] == 1 ) {
    int n = 6;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[7] == 1 ) {
    int n = 7;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[8] == 1 ) {
    int n = 8;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[9] == 1 ) {
    int n = 9;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[10] == 1 ) {
    int n = 10;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[11] == 1 ) {
    int n = 11;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[12] == 1 ) {
    int n = 12;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[13] == 1 ) {
    int n = 13;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[14] == 1 ) {
    int n = 14;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[15] == 1 ) {
    int n = 15;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
    }
    FastLED.show();
  }

  if (noteoff[16] == 1 ) {
    int n = 16;
    if (ledsring[n].r > 10) {
      ledsring[n].r -= decaytime;
    } else {
      ledsring[n].r = 0;
    }
    if (ledsring[n].g > 10) {
      ledsring[n].g -= decaytime;
    } else {
      ledsring[n].g = 0;
    }
    if (ledsring[n].b > 10) {
      ledsring[n].b -= decaytime;
    } else {
      ledsring[n].b = 0;
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
      Serial.println("UNO: Settings Updated");
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
