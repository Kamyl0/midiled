import ddf.minim.*;
import processing.serial.*;
import rwmidi.*;  
import controlP5.*;

String valeo;
String valmega;
String valuno;
boolean leofirstContact = false;
boolean megafirstContact = false;
boolean unofirstContact = false;

Serial leoPort;  // Create object from Serial class
Serial megaPort;  // Create object from Serial class
Serial unoPort;  // Create object from Serial class

Minim minim;
AudioPlayer player;
MidiInput mymididevice; 

ControlP5 cp5;
Println console;
Textarea myTextarea;

int decayValue = 10;

int r1=255;        //default rgb value
int g1=255;
int b1=255;

int r2=255;        //default rgb value
int g2=255;
int b2=255;

int r3=255;        //default rgb value
int g3=255;
int b3=255;

void setup() 
{
  smooth(4);

  size(800, 800); //make our canvas 200 x 200 pixels big
  noStroke();
  String portuno = Serial.list()[14]; //change the 0 to a 1 or 2 etc. to match your port
  String portleo = Serial.list()[15]; //change the 0 to a 1 or 2 etc. to match your port
  String portmega = Serial.list()[16]; //change the 0 to a 1 or 2 etc. to match your port
  //leoPort.bufferUntil(lf); 
  leoPort = new Serial(this, portleo, 19200);
  leoPort.bufferUntil('\n'); 
  megaPort = new Serial(this, portmega, 38400);
  megaPort.bufferUntil('\n'); 
  unoPort = new Serial(this, portuno, 9600);
  unoPort.bufferUntil('\n'); 

  println("processing started");

  MidiInputDevice devices[] = RWMidi.getInputDevices();
  mymididevice = RWMidi.getInputDevices()[0].createInput(this); 



  cp5 = new ControlP5(this);
  cp5.enableShortcuts();
  frameRate(50);
  myTextarea = cp5.addTextarea("txt")          //consol interface
    .setPosition(100, 100)
    .setSize(200, 500)
    .setFont(createFont("", 10))
    .setLineHeight(14)
    .setColor(color(200))
    .setColorBackground(color(0, 100))
    .setColorForeground(color(255, 100));
  ;

  // add a horizontal sliders, the value of this slider will be linked
  // to variable 'sliderValue' 

  /*********** DECAY SLIDER************/

  cp5.addSlider("decayValueStrip")
    .setPosition(400, 100)
    .setRange(1, 10)
    .setSize(200, 20)
    .setValue(5)
    ;
  //cp5.addSlider("decayValueRGBStrip")
  //  .setPosition(400, 125)
  //  .setRange(1, 10)
  //  .setSize(200, 20)
  //  .setValue(5)
  //  ;
  cp5.addSlider("decayValueRoundedLed")
    .setPosition(400, 125)
    .setRange(1, 10)
    .setSize(200, 20)
    .setValue(5)
    ;
  cp5.addSlider("decayValueRING")
    .setPosition(400, 150)
    .setRange(1, 10)
    .setSize(200, 20)
    .setValue(5)
    ;

  /*********** RGB SLIDER************/

  cp5.addSlider("R1")
    .setPosition(400, 300)
    .setRange(0, 255)
    .setSize(200, 20)
    .setValue(255)
    .setColorForeground(color(255, 0, 0))
    .setColorActive(color(200, 0, 0)) 
    ;

  cp5.addSlider("G1")
    .setPosition(400, 330)
    .setRange(0, 255)
    .setSize(200, 20)
    .setValue(255)
    .setColorForeground(color(0, 255, 0)) 
    .setColorActive(color(0, 200, 0)) 
    ;

  cp5.addSlider("B1")
    .setPosition(400, 360)
    .setRange(0, 255)
    .setSize(200, 20)
    .setValue(255)
    .setColorForeground(color(0, 0, 255)) 
    .setColorActive(color(0, 0, 200)) 
    ;

  //SLIDER 2

  cp5.addSlider("R2")
    .setPosition(400, 430)
    .setRange(0, 255)
    .setSize(200, 20)
    .setValue(255)
    .setColorForeground(color(255, 0, 0))
    .setColorActive(color(200, 0, 0)) 
    ;

  cp5.addSlider("G2")
    .setPosition(400, 460)
    .setRange(0, 255)
    .setSize(200, 20)
    .setValue(255)
    .setColorForeground(color(0, 255, 0)) 
    .setColorActive(color(0, 200, 0)) 
    ;

  cp5.addSlider("B2")
    .setPosition(400, 490)
    .setRange(0, 255)
    .setSize(200, 20)
    .setValue(255)
    .setColorForeground(color(0, 0, 255)) 
    .setColorActive(color(0, 0, 200)) 
    ;


  //SLIDER 3

  //  cp5.addSlider("R3")
  //    .setPosition(400, 560)
  //    .setRange(0, 255)
  //    .setSize(200, 20)
  //    .setValue(255)
  //    .setColorForeground(color(255, 0, 0))
  //    .setColorActive(color(200, 0, 0)) 
  //    ;

  //  cp5.addSlider("G3")
  //    .setPosition(400, 590)
  //    .setRange(0, 255)
  //    .setSize(200, 20)
  //    .setValue(255)
  //    .setColorForeground(color(0, 255, 0)) 
  //    .setColorActive(color(0, 200, 0)) 
  //    ;

  //  cp5.addSlider("B3")
  //    .setPosition(400, 620)
  //    .setRange(0, 255)
  //    .setSize(200, 20)
  //    .setValue(255)
  //    .setColorForeground(color(0, 0, 255)) 
  //    .setColorActive(color(0, 0, 200)) 
  //    ;


  console = cp5.addConsole(myTextarea);
}

////************INTERFACE*************///////////////

class MyCanvas extends Canvas {
  public void draw(PGraphics pg) {
    pg.fill(100);
    pg.rect(20, 20, 240, 30);
    pg.fill(255);
    pg.text("This text is drawn by MyCanvas", 100, 100);
  }
}




void draw() {

  textSize(14);
  text ("praktish Balade Version Beta 1.0 © Camilo De Martino Berlin 2018", 20, 780);
  text ("CHANEL 1: PRGM RGB STRIP  // CHANEL 11: DECAY = VELOCITY", 20, 640);
  text ("CHANEL 2: FULL RGB STRIP // CHANEL 12: DECAY = VELOCITY", 20, 660);
  text ("CHANEL 3: ROUNDED LED // CHANEL 13: DECAY = VELOCITY", 20, 680);
  text ("CHANEL 4: PRGM RING // CHANEL 14: DECAY = VELOCITY", 20, 700);

  text ("COLOR FOR STRIP", 400, 290);
  //  text ("COLOR FOR ROUNDED LED",400, 550);
  text ("COLOR FOR RING", 400, 420);
}

void keyPressed() {
  switch(key) {
    case('1'):
    console.pause();
    break;
    case('2'):
    console.play();
    break;
    case('3'):
    console.setMax(8);
    break;
    case('4'):
    console.setMax(-1);
    break;
    case('5'):
    console.clear();
    break;
  }
}
////************END INTERFACE*************///////////////

////************RECEIVE DATA FROM ARDUINO*************///////////////

void serialEvent( Serial leoPort) {
  valeo = leoPort.readStringUntil('\n');
  valmega = leoPort.readStringUntil('\n');
  valuno = unoPort.readStringUntil('\n');

  if (valeo != null) {
    //trim whitespace and formatting characters (like carriage return)
    valeo = trim(valeo);
    println(valeo);
    if (leofirstContact == false) {
      if (valeo.equals("A")) {
        leoPort.clear();
        leofirstContact = true;
        leoPort.write("A");
        println(" LEO contact");
      }
    }
  }

  if (valmega != null) {
    //trim whitespace and formatting characters (like carriage return)
    valmega = trim(valmega);
    println(valmega);
    if (megafirstContact == false) {
      if (valmega.equals("A")) {
        megaPort.clear();
        megafirstContact = true;
        megaPort.write("A");
        println("MEGA contact");
      }
    }
  }

  if (valuno != null) {
    //trim whitespace and formatting characters (like carriage return)
    valuno = trim(valuno);
    println(valuno);
    if (unofirstContact == false) {
      if (valmega.equals("A")) {
        unoPort.clear();
        unofirstContact = true;
        unoPort.write("A");
        println("UNO contact");
      }
    }
  }
}

////************END RECEIVE DATA FROM ARDUINO*************///////////////

void decayValueStrip(int sliderdecay) {
  decayValue = sliderdecay;
  sendsettings(decayValue, r1, g1, b1, 1);
}
//void decayValueRGBStrip(int sliderdecay) {
//  decayValue = sliderdecay;
//  sendsettings(decayValue, r1, g1, b1, 2);
//}
void decayValueRoundedLed(int sliderdecay) {
  decayValue = sliderdecay;
  sendsettings(decayValue, r2, g2, b2, 3);
}
void decayValueRING(int sliderdecay) {
  decayValue = sliderdecay;
  sendsettings(decayValue, r3, g3, b3, 4);
}

/*********** RGB FUNCTION************/

void R1(int rvalue) {
  colorref(1, rvalue, 1);
}

void G1(int gvalue) {
  colorref(2, gvalue, 1);
}

void B1(int bvalue) {
  colorref(3, bvalue, 1);
}

// NEXT

void R2(int rvalue) {
  colorref(1, rvalue, 2);
}

void G2(int gvalue) {
  colorref(2, gvalue, 2);
}

void B2(int bvalue) {
  colorref(3, bvalue, 2);
}


//void R3(int rvalue) {
//  colorref(1, rvalue, 3);
//}

//void G3(int gvalue) {
//  colorref(2, gvalue, 3);
//}

//void B3(int bvalue) {
//  colorref(3, bvalue, 3);
//}

void colorref(int data, int slidercolor, int ledselect) {

  if (ledselect==1) {
    if (data==1) {
      r1=slidercolor;
    } else if (data==2) {
      g1=slidercolor;
    } else if (data==3) {
      b1=slidercolor;
    }

    fill(r1, g1, b1);
    rect(350, 300, 40, 80);

    sendsettings(decayValue, r1, g1, b1, 1);
  }

  if (ledselect==2) {
    if (data==1) {
      r2=slidercolor;
    } else if (data==2) {
      g2=slidercolor;
    } else if (data==3) {
      b2=slidercolor;
    }

    sendsettings(decayValue, r2, g2, b2, 4);

    fill(r2, g2, b2);
    rect(350, 430, 40, 80);
  }

  //if (ledselect==3) {
  //  if (data==1) {
  //    r3=slidercolor;
  //  } else if (data==2) {
  //    g3=slidercolor;
  //  } else if (data==3) {
  //    b3=slidercolor;
  //  }

  //  fill(r3, g3, b3);
  //  rect(350, 560, 40, 80);

  //  sendsettings(decayValue, r2, g2, b2, 3);
  //}
}


/***************/

////************SEND SERIAL PACKET TO ARDUINO*************///////////////

void sendMidiOnMessage(int p, int v, int c) {        //Note ON
  //println("PROCESSING Message to send: channel " + c + " pitch "+ p + " velocity " + v);

  if (c==3) {
    megaPort.write("<");
    megaPort.write(p);
    megaPort.write(v);
    megaPort.write(c);
    megaPort.write(">");
  } else if (c==4) {
    unoPort.write("<");
    unoPort.write(p);
    unoPort.write(v);
    unoPort.write(c);
    unoPort.write(">");
  } else {  
    leoPort.write("<");
    leoPort.write(p);
    leoPort.write(v);
    leoPort.write(c);
    leoPort.write(">");
  }
}

void sendMidiOffMessage(int p, int v, int c) {      //Note OFF
  //println("PROCESSING Message to send: channel " + c + " pitch "+ p + " velocity " + v);

  if (c==3) {
    megaPort.write("#");
    megaPort.write(p);
    megaPort.write(v);
    megaPort.write(c);
    megaPort.write("%");
  } else if (c==4) {
    unoPort.write("#");
    unoPort.write(p);
    unoPort.write(v);
    unoPort.write(c);
    unoPort.write("%");
  } else {  
    leoPort.write("#");
    leoPort.write(p);
    leoPort.write(v);
    leoPort.write(c);
    leoPort.write("%");
  }
}

void sendsettings(int intdecay, int rvts, int gvts, int bvts, int data) {



  if (data==1 || data ==2) {
    leoPort.write("$");
    leoPort.write(intdecay);
    leoPort.write(rvts);
    leoPort.write(gvts);
    leoPort.write(bvts);
    leoPort.write("@");
  }

  if (data==3) {
    megaPort.write("$");
    megaPort.write(intdecay);
    megaPort.write(rvts);
    megaPort.write(gvts);
    megaPort.write(bvts);
    megaPort.write("@");
  }

  if (data==4) {
    unoPort.write("$");
    unoPort.write(intdecay);
    unoPort.write(rvts);
    unoPort.write(gvts);
    unoPort.write(bvts);
    unoPort.write("@");
  }


  //println("Send from processing");
  //println(intdecay, rvts, gvts,bvts);
}

////************END SEND SERIAL PACKET TO ARDUINO*************///////////////



////************MIDI NOTE FUNCTION*************///////////////

void noteOnReceived(Note myreceivednote) {
  println("note on  " + myreceivednote.getPitch()+ "  " + myreceivednote.getVelocity()+ "  " + myreceivednote.getChannel());
  int rp = myreceivednote.getPitch();
  int rv = myreceivednote.getVelocity();
  int nc = myreceivednote.getChannel()+1;
  sendMidiOnMessage(rp, rv, nc);

  if (nc==11 || nc==12) {
    float rvm = map(rv, 0, 127, 0, 10);
    int i = int(rvm);
    cp5.getController("decayValueStrip").setValue(i);
    sendsettings(i, r1, g1, b1, 1);
  }
  if (nc==13) {
    float rvm = map(rv, 0, 127, 0, 10);
    int i = int(rvm);
    cp5.getController("decayValueRING").setValue(i);
    sendsettings(i, r1, g1, b1, 4);
  }

  if (nc==14) {
    float rvm = map(rv, 0, 127, 0, 10);
    int i = int(rvm);
    cp5.getController("decayValueRoundedLed").setValue(i);
    sendsettings(i, r2, g2, b2, 3);
  }

  //println(148);
  //leoPort.write(datasend);
  //println("note ON");
}

void noteOffReceived(Note myreceivednote) {
  //println("note OFF");
  //leoPort.write("&");
  println("note off "  + "  " + myreceivednote.getPitch()+ "  " + myreceivednote.getVelocity()+ "  " + myreceivednote.getChannel());
  int rp = myreceivednote.getPitch();
  int rv = myreceivednote.getVelocity();
  int nc = myreceivednote.getChannel()+1;
  sendMidiOffMessage(rp, rv, nc);
}

////************END SEND SERIAL PACKET TO ARDUINO*************///////////////