#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <FreeRAM.h>
#include "Command.h"
#include "KeepCube.h"

/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/*
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */
 

RF24 radio(10, 3);                // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio

Command message('#', ';');
KeepCube cube(radio, network);


const uint16_t this_node = 00;     // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 01;   // Address of the other node in Octal format


// struct SLP {
//     bool L;             // loop?
//     unsigned long D;    // on_time
//     unsigned long T;    // prechod
//     char P[32];           // data
// };

int missed = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("RF24Network/examples/helloworld_rx/"));
 
    SPI.begin();
    
    cube.begin();

}

unsigned long long a = 0;

void loop() {
    delay(1);
    network.update();
    cube.updateLED();
    
    
    if (Serial.available()) {
        message.readSerial();
    }
    
    
    if (network.available()) {
        Serial.println("\n\n\nReceived!, " + (String)millis() + " RAM: " + (String)freeram() + "B");
        
        
        int size = 64;    // #LNCL32;
        char a[size];
        
        RF24NetworkHeader header;
        network.read(header, &a, sizeof(a));
        
        Command msg(a);
        
        if (msg.getIdentifier() == "SLP") {
            int numberOfPoints     = msg.getParameterValue('L').toInt();
            int transitionDuration = msg.getParameterValue('T').toInt();
            int litDuration        = msg.getParameterValue('D').toInt();
            String colors          = msg.getParameterValue('P');
            
            cube.setTransitionParameters(numberOfPoints, transitionDuration, litDuration, colors);
        }
        
        //Serial.println(msg.toString());
        //cube.led(random(1, 254), random(1, 254), random(1, 254), 300);
        
    }
    
    
    
    
    
    
}

```
int length = 0;
radio.read(&length, sizeof(int));

char pol



