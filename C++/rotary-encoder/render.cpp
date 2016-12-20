/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
	Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
	Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/


#include <Bela.h>
#include <stdlib.h>

int encoder0PinA = P8_08;
int encoder0PinB = P8_07;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int status = LOW;

float gInterval = 0.25;
float gSecondsElapsed = 0;
int gCount = 0;


bool setup(BelaContext *context, void *userData)
{
	// Set the mode of digital pins
	pinMode(context, 0, P8_08, INPUT);
	pinMode(context, 0, P8_07, INPUT);
	return true;
}


void render(BelaContext *context, void *userData)
{
	for(unsigned int n=0; n<context->digitalFrames; n++){
		status = digitalRead(context, n, encoder0PinA);
	    if ((encoder0PinALast == LOW) && (status == HIGH)) {
	      	if (digitalRead(encoder0PinB) == LOW) {
	        	encoder0Pos--;
	      	} else {
	        	encoder0Pos++;
	      	}
	    } 
	    encoder0PinALast = status;

	    // Increment a counter on every frame
		gCount++;
		
		// Print a message every second indicating the number of seconds elapsed
		if(gCount % (int)(context->audioSampleRate*gInterval) == 0) {
		    gSecondsElapsed += gInterval;
		    rt_printf("Time elapsed: %f\n",encoder0PinALast);
		}
	}
}


void cleanup(BelaContext *context, void *userData)
{
	// Nothing to do here
}


/**
\example digital-input/render.cpp

Switching an LED on and off
---------------------------

This example brings together digital input and digital output. The program will read 
a button and turn the LED on and off according to the state of the button.

- connect an LED in series with a 470ohm resistor between P8_07 and ground.
- connect a 1k resistor to P9_03 (+3.3V),
- connect the other end of the resistor to both a button and P8_08
- connect the other end of the button to ground.

Before using the digital pins we need to set whether they are input or output.
This is done via `pinMode(context, 0, P8_08, INPUT);`.

You will notice that the LED will normally stay off and will turn on as long as 
the button is pressed. This is due to the fact that the LED is set to the same 
value read at input P8_08. When the button is not pressed, P8_08 is `LOW` and so 
P8_07 is set to `LOW` as well, so that the LED conducts and emits light. When 
the button is pressed, P8_08 goes `HIGH` and P8_07 is set to `HIGH`, turning off the LED.

Note that there are two ways of specifying the digital pin: using the GPIO label (e.g. `P8_07`), 
or using the digital IO index (e.g. 0)

As an exercise try and change the code so that the LED only turns off when 
the button is pressed.
*/

