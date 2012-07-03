/*
======================================================================

	PIR SENSOR

	For the Parallax PIR Sensor

	Switches a LED according to the state of the sensors output pin.
	Determines the beginning and end of continuous motion sequences.

	The sensor's output pin goes to HIGH if motion is present.
	However, even if motion is present it goes to LOW from time to time,
	which might give the impression no motion is present.
	This program deals with this issue by ignoring LOW-phases shorter than a given time,
	assuming continuous motion is present during these phases.


	The Parallax PIR Sensor is an easy to use digital infrared motion sensor module.
	http://www.parallax.com/StoreSearchResults/tabid/768/txtSearch/555-28027/List/0/SortField/4/ProductID/83/Default.aspx

	PIR Sensor Jumper Pin Settings:

	* H (sometimes labeled as "s") = Retrigger ( Output remains HIGH when sensor is retriggered repeatedly.  Output is LOW when idle [not triggered]. )
	* L = Normal ( Output goes HIGH then LOW when triggered.  Continuous motion results in repeated HIGH/LOW pulses.  Output is LOW when idle )

	The circuit:

	* PIR Ground => Arduino Ground
	* PIR VCC => Arduino 5V
	* PIR Out => Arduino Digital Pin ~3

	Created 2012-07-02
	Peter Schmalfeldt me@peterschmalfeldt.com

	https://github.com/manifestinteractive/arduino

======================================================================

----------------------------------------------------------------------
	CONFIGURATION VARIABLES ( CHANGE THESE TO MEET YOUR NEEDS )
----------------------------------------------------------------------
*/

// The time we give the sensor to calibrate ( 10-60 secs according to the datasheet - DO NOT MOVE DURING THIS TIME )
int calibrationTime = 15;

// The digital pin connected to the PIR sensor's output
int pirPin = 3;

// The digital pin connected to the LED
int ledPin = 13;

// The amount of milliseconds the sensor has to be low before we assume all motion has stopped
long unsigned int pause = 5000;

// Whether or not to enable debugging
bool debug = true;

/*
----------------------------------------------------------------------
	!!! YOU DO NOT NEED TO CHANGE ANYTHING BELOW HERE !!!
----------------------------------------------------------------------
*/

int delayTime = 50;

// Time motion started
long unsigned int startTime;

// Time Motion Ended
long unsigned int endTime;

// Difference between endTime & startTime
long unsigned int totalTime;

// The time when the sensor outputs a low impulse
long unsigned int lowIn;

// Whether or not there is currently motion going on
boolean inMotion = false;

// Whether or not pirPin has been on LOW for pause duration or longer
boolean lockLow = true;

// Whether to take the LOW time from pirPin ( first instance only )
boolean takeLowTime;

/* Default Arduino Setup Function */
void setup()
{
	Serial.begin(9600);
	pinMode(pirPin, INPUT);
	pinMode(ledPin, OUTPUT);
	digitalWrite(pirPin, LOW);

	if(debug)
	{
		Serial.println();
		Serial.print("Calibrating PIR Sensor ");
	}

	// Give the sensor some time to calibrate
	for(int i = 0; i < calibrationTime; i++)
	{
		if(debug)
		{
			Serial.print(".");
		}
		
		delay(1000);
	}

	if(debug)
	{
		Serial.println(" Done");
		Serial.println();
		Serial.println("!!! SENSOR ACTIVE !!!");
	}

	delay(delayTime);
}

/* Default Arduino Loop Function */
void loop()
{
	if(digitalRead(pirPin) == HIGH)
	{
		// The led visualizes the sensors output pin state
		digitalWrite(ledPin, HIGH);

		// Make sure we wait for a transition to LOW before any further output is made:
		if(lockLow)
		{
			motionStarted();
		}

		takeLowTime = true;
	}

	if(digitalRead(pirPin) == LOW)
	{
		// The led visualizes the sensors output pin state
		digitalWrite(ledPin, LOW);

		if(takeLowTime)
		{
			// Save the time of the transition from high to LOW
			lowIn = millis();

			// Make sure this is only done at the start of a LOW phase
			takeLowTime = false;
		}

		// If the sensor is low for more than the given pause, we assume that no more motion is going to happen
		if( !lockLow && millis() - lowIn > pause)
		{
			motionEnded();
		}
	}
}

/* Motion Started */
void motionStarted()
{
	lockLow = false;
	
	inMotion = true;
	
	startTime = millis()/1000;
	endTime = 0;
	totalTime = 0;
	
	if(debug)
	{
		Serial.println();
		Serial.print("START: Motion Detected at ");
		Serial.print(startTime);
		Serial.println(" Seconds.");	
	}

	delay(delayTime);
}

/* Motion Ended */
void motionEnded()
{
	lockLow = true;
	
	inMotion = false;

	endTime = (millis() - pause)/1000;
	totalTime = (endTime - startTime);
	
	if(debug)
	{
		Serial.print("END:   Motion Ended at ");
		Serial.print(endTime);
		Serial.print(" Seconds, Lasting a Total of ");
		Serial.print(totalTime);
		Serial.println(" Seconds.");
	}

	delay(delayTime);
}

/* Example Function that can be used to check if there currently motion going on */
bool getInMotion()
{
	return inMotion;
}