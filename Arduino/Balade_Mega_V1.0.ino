#include <FastLED.h>
#include <LEDFader.h>

#define NUM_LEDS 128
#define DATA_PIN 5

const int roundled1 = 4;
const int roundled2 = 5;
const int roundled3 = 6;
const int roundled4 = 7;
const int roundled5 = 8;
const int roundled6 = 9;
const int roundled7 = 10;
const int roundled8 = 11;
const int roundled9 = 12;
const int roundled10 = 13;

LEDFader roundedled1 = LEDFader(roundled1);
LEDFader roundedled2 = LEDFader(roundled2);
LEDFader roundedled3 = LEDFader(roundled3);
LEDFader roundedled4 = LEDFader(roundled4);
LEDFader roundedled5 = LEDFader(roundled5);
LEDFader roundedled6 = LEDFader(roundled6);
LEDFader roundedled7 = LEDFader(roundled7);
LEDFader roundedled8 = LEDFader(roundled8);
LEDFader roundedled9 = LEDFader(roundled9);
LEDFader roundedled10 = LEDFader(roundled10);

// Define the array of leds
CRGB leds[NUM_LEDS];

const int GledIn = 8;
const int BledIn = 12;
const int RledIn = 13;   //connection LED Ctrl




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
  Serial.begin(38400);

  analogWrite(roundled1, 255);
  analogWrite(roundled2, 255);
  analogWrite(roundled3, 255);
  analogWrite(roundled4, 255);
  analogWrite(roundled5, 255);
  analogWrite(roundled6, 255);
  analogWrite(roundled7, 255);
  analogWrite(roundled8, 255);
  analogWrite(roundled9, 255);
  analogWrite(roundled10, 255);

  //light before contact

  establishContact();

  analogWrite(roundled1, 0);
  analogWrite(roundled2, 0);
  analogWrite(roundled3, 0);
  analogWrite(roundled4, 0);
  analogWrite(roundled5, 0);
  analogWrite(roundled6, 0);
  analogWrite(roundled7, 0);
  analogWrite(roundled8, 0);
  analogWrite(roundled9, 0);
  analogWrite(roundled10, 0);

  Serial.println("MEGA Initialized");
}

/*************FUNCTION AREA************/
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("MEGA: No data incoming from Processing");
    delay(300);
  }
}

//void fadeout(int nmbrled, int ledbrightness ) {
//  for (int i = ledbrightness; i > -1; i--) {                                        //TURN OFF SINGLE LED
//    leds[nmbrled] = CHSV ( 255, 255, i);
//    FastLED.show();
//  }
//  Serial.println("Start FADEOUT");
//}


void analysemididata(int midipitch, int midivelocity, int midichannel, int notestatus) {   //ANAYLSE DATA AND SEND TO LED FNC
reset:
  int settingdata = setdec;
  int rdata = setrc;
  int gdata = setgc;
  int bdata = setbc;

  int bvalue = map(midivelocity, 0, 127 , 0 , 255);                         // data received from processing, mapped to match arduino
  int nmbrled = midipitch;

  int decaytime = map(settingdata, 1, 10, 10, 1);

  if (notestatus == 1) {
    ledbrightness = map(midivelocity, 0, 127 , 0 , 255);
  }


  /***********ROUND LED FNC****************/

  if (midichannel == 3) {
    if (notestatus == 1) {                              //FCN TO TURN ON THE LIGHT
      if (midipitch == 0) {
        roundedled1.fade(bvalue, 100);
      }
      if (midipitch == 1) {
        roundedled2.fade(bvalue, 100);
      }
      if (midipitch == 2) {
        roundedled3.fade(bvalue, 100);
      }
      if (midipitch == 3) {
        roundedled4.fade(bvalue, 100);
      }
      if (midipitch == 4) {
        roundedled5.fade(bvalue, 100);
      }
      if (midipitch == 5) {
        roundedled6.fade(bvalue, 100);
      }
      if (midipitch == 6) {
        roundedled7.fade(bvalue, 100);
      }
      if (midipitch == 7) {
        roundedled8.fade(bvalue, 100);
      }
      if (midipitch == 8) {
        roundedled9.fade(bvalue, 100);
      }
      if (midipitch == 9) {
        roundedled10.fade(bvalue, 100);
      }
    }

    if (notestatus == 2) {                              //FCN TO TURN ON THE LIGHT

      int roundeday = map(settingdata, 0, 10, 100, 5000);

      if (midipitch == 0) {
        roundedled1.fade(0, roundeday);
      }
      if (midipitch == 1) {
        roundedled2.fade(0, roundeday);
      }
      if (midipitch == 2) {
        roundedled3.fade(0, roundeday);
      }
      if (midipitch == 3) {
        roundedled4.fade(0, roundeday);
      }
      if (midipitch == 4) {
        roundedled5.fade(0, roundeday);
      }
      if (midipitch == 5) {
        roundedled6.fade(0, roundeday);
      }
      if (midipitch == 6) {
        roundedled7.fade(0, roundeday);
      }
      if (midipitch == 7) {
        roundedled8.fade(0, roundeday);
      }
      if (midipitch == 8) {
        roundedled9.fade(0, roundeday);
      }
      if (midipitch == 9) {
        roundedled10.fade(0, roundeday);
      }
    }
  }

}


void loop() {
  //Serial.println("hello from arduino");
  roundedled1.update();
  roundedled2.update();
  roundedled3.update();
  roundedled4.update();
  roundedled5.update();
  roundedled6.update();
  roundedled7.update();
  roundedled8.update();
  roundedled9.update();
  roundedled10.update();
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

      settingin = false;
      settingend = false;
      index = 0;
      inData[index] = '\0';
      Serial.println("MEGA: Settings Updated");
    }

    if (NoteOffStarted && NoteOffEnded)
    {
      analysemididata(inData[0], inData[1], inData[2], 2); //data 1 is pitch data 2 is velocity
      NoteOffStarted = false;
      NoteOffEnded = false;
      index = 0;
      inData[index] = '\0';
    }

    if (NoteOnStarted && NoteOnEnded)
    {
      analysemididata(inData[0], inData[1], inData[2], 1); //data 1 is pitch data 2 is velocity
      NoteOnStarted = false;
      NoteOnEnded = false;
      index = 0;
      inData[index] = '\0';
    }
    //delay(10); // Wait 10 milliseconds for next reading
  }
}
