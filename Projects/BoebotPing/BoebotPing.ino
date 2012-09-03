/*
======================================================================

	Beo-Bot with PING))) Sensor



	The circuit:

	*

	Created 2012-07-29
	Peter Schmalfeldt me@peterschmalfeldt.com

	https://github.com/manifestinteractive/arduino

	License: http://creativecommons.org/licenses/by-sa/3.0/

	This Arduino Sketch is licensed under a
	Creative Commons Attribution-ShareAlike 3.0 License.

======================================================================
*/

// Include servo library
#include <Servo.h>

// include the SoftwareSerial library so we can use it to talk to the Emic 2 module
#include <SoftwareSerial.h>

/*
----------------------------------------------------------------------
	CONFIGURATION VARIABLES ( CHANGE THESE TO MEET YOUR NEEDS )
----------------------------------------------------------------------
*/

// Whether or not to enable debugging
bool debug = true;

/* Serial Baudrate */
long unsigned int serialBaudrate = 57600;
long unsigned int serialBaudrateTextToSpeech = 9600;

/* Setup some octaves for Tone ( Key of A - octave1[0] = A1, octave1[1] = A1# ) */
int octave1[] = { 440, 477, 513, 550, 587, 623, 660, 697, 733, 770, 807, 843 };
int octave2[] = { 880, 917, 953, 990, 1027, 1063, 1100, 1137, 1173, 1210, 1247, 1283 };
int octave3[] = { 1320, 1357, 1393, 1430, 1467, 1503, 1540, 1577, 1613, 1650, 1687, 1723 };
int octave4[] = { 1760, 1797, 1833, 1870, 1907, 1943, 1980, 2017, 2053, 2090, 2126, 2163 };
int octave5[] = { 2200, 2237, 2273, 2310, 2347, 2383, 2420, 2457, 2493, 2530, 2567, 2603 };

/* Define Servo Measures */
int leftSpeed = 1500;
int rightSpeed = 1500;

/* Head Offset ( cant always mount on 90 ) */
signed int headOffset = -4;

// Pin Numbers
//const int pinRx = 8;
//const int pinTx = 9;
const int pinTone = 4;
const int pinServoLeft = 10;
const int pinServoRight = 11;
const int pinServoHead = 12;
const int pinPingSensor = 8;

// Define Text to Speech Module Serial
//SoftwareSerial textToSpeechSerial(pinRx, pinTx);

/*
----------------------------------------------------------------------
	!!! YOU DO NOT NEED TO CHANGE ANYTHING BELOW HERE !!!
----------------------------------------------------------------------
*/

// Declare left, right & head servos
Servo servoLeft;
Servo servoRight;
Servo servoHead;

// establish variables for duration of the ping, and the distance result in inches and centimeters:
long duration;
double inches, cm;

/* Default Arduino Setup Function */
void setup()
{
	// Generic Serial to receive other Serial communications
	Serial.begin(serialBaudrate);

	if(debug)
	{
		Serial.println("Starting up Robot");
	}

	// Play Startup Sound
	startupTone();

	// Attach Header Servo & PING))) Sensor
	attachHead();

	// Attach Text to Speech Module
	//attachVoice();

	// Attach Left & Right Servos
	attachFeet();
}

/* Default Arduino Loop Function */
void loop()
{
	avoidHittingStuff();
}

/*
----------------------------------------------------------------------
	FUNCTIONS RELATED TO HEAD
----------------------------------------------------------------------
*/

/* Attach Text to Speech Module  */
void attachVoice()
{
	if(debug)
	{
		Serial.println("Attaching Voice");
	}
	
	// set the data rate for the SoftwareSerial port
	//textToSpeechSerial.begin(serialBaudrateTextToSpeech);

	//delay(100);

	// Get current settings
	//textToSpeechSerial.print('X');
	//textToSpeechSerial.print('\n');

	// When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
	//while (textToSpeechSerial.read() != ':');

	// Short delay
	//delay(10);

	// Flush the receive buffer
	//textToSpeechSerial.flush();
}

/* Attach Servos */
void attachHead()
{
	if(debug)
	{
		Serial.println("Attaching Head");
	}

	// Setup Servo Pins
	servoHead.attach(pinServoHead);

	// Move Head Servo to Center
	look(90);
}

/* Turn Head to this Angle */
void look(int angle)
{
	// Set angle and adjust for offset
	angle = (angle + headOffset);

	// Make sure angle is within limits
	angle = constrain(angle, 0, 180);

	// Left wheel counterclockwise
	servoHead.write(angle);
}

/* Read PING))) Sensor */
double readPingSensor(String unit)
{
	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(pinPingSensor, OUTPUT);
	digitalWrite(pinPingSensor, LOW);
	delayMicroseconds(2);
	digitalWrite(pinPingSensor, HIGH);
	delayMicroseconds(5);
	digitalWrite(pinPingSensor, LOW);


	// The same pin is used to read the signal from the PING))): a HIGH
	// pulse whose duration is the time (in microseconds) from the sending
	// of the ping to the reception of its echo off of an object.
	pinMode(pinPingSensor, INPUT);
	duration = pulseIn(pinPingSensor, HIGH);

	// convert the time into a distance
	inches = microsecondsToInches(duration);
	cm = microsecondsToCentimeters(duration);

	delay(500);
	
	if(unit == "ms")
	{
		return duration;
	}
	else if(unit == "in")
	{
		return inches;
	}
	else if(unit == "cm")
	{
		return cm;
	}
}

/* Say Text */
void sayText(String text)
{
	// Stop any existing text that is playing
	//textToSpeechSerial.print('X');
	//textToSpeechSerial.print('\n');

	// Send the desired string to convert to speech
	//textToSpeechSerial.print('S');
	//textToSpeechSerial.print(text);
	//textToSpeechSerial.print('\n');

	// Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
	//while (textToSpeechSerial.read() != ':');
	
	// Short delay
	//delay(10);

	// Flush the receive buffer
	//textToSpeechSerial.flush();
}

/*
----------------------------------------------------------------------
	FUNCTIONS RELATED TO FEET
----------------------------------------------------------------------
*/

/* Attach Servos */
void attachFeet()
{
	// Setup Servo Pins
	if(servoLeft.attached() == false)
	{
		servoLeft.attach(pinServoLeft);

		if(debug)
		{
			Serial.println("Attaching Left Foot");
		}
	}
	else
	{
		if(debug)
		{
			Serial.println("Left Foot Already Attached");
		}
	}

	// Setup Servo Pins
	if(servoRight.attached() == false)
	{
		servoRight.attach(pinServoRight);

		if(debug)
		{
			Serial.println("Attaching Right Foot");
		}
	}
	else
	{
		if(debug)
		{
			Serial.println("Right Foot Already Attached");
		}
	}

}

/* Detach Servos */
void detachFeet()
{
	// Setup Servo Pins
	if(servoLeft.attached())
	{
		servoLeft.detach();

		if(debug)
		{
			Serial.println("Detaching Left Foot");
		}
	}
	else
	{
		if(debug)
		{
			Serial.println("Left Foot Already Detached");
		}
	}

	// Setup Servo Pins
	if(servoRight.attached())
	{
		servoRight.detach();

		if(debug)
		{
			Serial.println("Detaching Right Foot");
		}
	}
	else
	{
		if(debug)
		{
			Serial.println("Right Foot Already Detached");
		}
	}
}

void avoidHittingStuff()
{
	int distance = readPingSensor("ms");
	
	if(distance < 2000)
	{
		look(45);
	}
}

void move(int directionAngle, int speedPercent, int duration)
{
	// On standard servos a parameter value of
	// 1000 is fully counter-clockwise,
	// 2000 is fully clockwise, and
	// 1500 is in the middle.

	if(servoLeft.attached() && servoRight.attached() && speedPercent == 0)
	{
		detachFeet();
	}
	else if(servoLeft.attached() == false && servoRight.attached() == false)
	{
		attachFeet();
	}

	float percent = (float)speedPercent / 100.0;

	if(directionAngle == 0 || directionAngle == 360)
	{
		if(debug)
		{
			Serial.println("Going Forward");
		}

		leftSpeed = 1500 + ( 500 * percent);
		rightSpeed = 1500 - ( 500 * percent);
	}
	else if(directionAngle == 270)
	{
		if(debug)
		{
			Serial.println("Going Left");
		}

		leftSpeed = 1500;
		rightSpeed = 1500 - ( 500 * percent);
	}
	else if(directionAngle == 90)
	{
		if(debug)
		{
			Serial.println("Going Right");
		}

		leftSpeed = 1500 + ( 500 * percent);
		rightSpeed = 1500;
	}
	else if(directionAngle == 180)
	{
		if(debug)
		{
			Serial.println("Going Straight Backwards");
		}

		leftSpeed = 1500 - ( 500 * percent);
		rightSpeed = 1500 + ( 500 * percent);
	}
	else if((directionAngle >= 0 && directionAngle <= 90) || (directionAngle >= 270 && directionAngle <= 360))
	{
		if(debug)
		{
			Serial.println("Going Straight Forward");
		}

		//leftSpeed = 1500;
		//rightSpeed = 1500;
	}
	else if(directionAngle >= 270 && directionAngle <= 360)
	{
		if(debug)
		{
			Serial.println("Going Backwards");
		}
	}

	if(debug)
	{
		Serial.print("directionAngle: ");
		Serial.print(directionAngle);
		Serial.print(", speedPercent: ");
		Serial.print(speedPercent);
		Serial.print(", percent: ");
		Serial.print(percent);
		Serial.print(", leftSpeed: ");
		Serial.print(leftSpeed);
		Serial.print(", rightSpeed: ");
		Serial.print(rightSpeed);
		Serial.print(", duration: ");
		Serial.println(duration);
		Serial.println();
	}

	servoLeft.writeMicroseconds(leftSpeed);
	servoRight.writeMicroseconds(rightSpeed);

	delay(duration);
}

/*
----------------------------------------------------------------------
	MISC FUNCTIONS
----------------------------------------------------------------------
*/

/* Play Startup Tone */
void startupTone()
{
	if(debug)
	{
		Serial.println("Playing Startup Tone");
	}

	tone(pinTone, octave1[6], 100);
	delay(100);
	tone(pinTone, (int)octave2[0], 300);
	delay(300);
}

/* Convert Microseconds to Inches */
double microsecondsToInches(long microseconds)
{
	return ((microseconds / 73.746) / 2.0);
}

/* Convert Microseconds to Centimeters */
double microsecondsToCentimeters(long microseconds)
{
	return ((microseconds / 29.034) / 2.0);
}

/*
----------------------------------------------------------------------
	JUST FOR FUN FUNCTIONS
----------------------------------------------------------------------
*/

void sing()
{
	// Stop any existing text that is playing
	//textToSpeechSerial.print('X');
	//textToSpeechSerial.print('\n');

	// Sing a song
    //textToSpeechSerial.print("D1\n");
    //while (textToSpeechSerial.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
}

void pimp()
{
	servoLeft.writeMicroseconds(2000);
	servoRight.writeMicroseconds(1000);

	delay(3000);

	detachFeet();

	look(145);

	sayText("Well, hello there.  I did not see you come in.");
	delay(1000);

	look(135);

	sayText("You sure are pretty.  How about a song?");

	delay(1000);

	sayText("Great! Here goes!");

	delay(1000);

	look(90);

	sing();

	delay(1000);

	look(135);

	sayText("Thanks. You are pretty amazing your self.");

	delay(1000);

	look(90);

	attachFeet();

	servoLeft.writeMicroseconds(2000);
	servoRight.writeMicroseconds(1000);

	delay(3000);

	detachFeet();
}
