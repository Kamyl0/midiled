import rwmidi.*;


// Example by Tom Igoe
MidiInput mymididevice; 

import processing.serial.*;

// The serial port
Serial myPort;       

// List all the available serial ports
//printArray(Serial.list());


void setup() {
  MidiInputDevice devices[] = RWMidi.getInputDevices();
  printArray(Serial.list());
  printArray(devices);
}