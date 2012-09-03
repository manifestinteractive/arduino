/*
======================================================================

	PING))) SENSOR

    This sketch reads a PING))) ultrasonic rangefinder and returns the
    distance to the closest object in range. To do this, it sends a pulse
    to the sensor to initiate a reading, then listens for a pulse
    to return.  The length of the returning pulse is proportional to
    the distance of the object from the sensor.

	See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf

	According to Parallax's datasheet for the PING))), there are
	73.746 microseconds per inch (i.e. sound travels at 1130 feet per
	second).  This gives the distance travelled by the ping, outbound
	and return, so we divide by 2 to get the distance of the obstacle.

    The speed of sound is 340 m/s or 29.034 microseconds per centimeter.
    The ping travels out and back, so to find the distance of the
    object we take half of the distance travelled.

	The circuit:

    * PING 5V  -> Arduino 5V
    * PING GND -> Arduino GND
    * PING SIG -> Arduino digital Pin 7

	Created 2012-07-08
	Peter Schmalfeldt me@peterschmalfeldt.com

	https://github.com/manifestinteractive/arduino

======================================================================

----------------------------------------------------------------------
	CONFIGURATION VARIABLES ( CHANGE THESE TO MEET YOUR NEEDS )
----------------------------------------------------------------------
*/

// Whether or not to enable debugging
bool debug = true;

/* Serial Baudrate */
long unsigned int serialBaudrate = 9600;

/*
----------------------------------------------------------------------
	!!! YOU DO NOT NEED TO CHANGE ANYTHING BELOW HERE !!!
----------------------------------------------------------------------
*/

// this constant won't change.  It's the pin number of the sensor's output:
const int pingPin = 7;

/* Default Arduino Setup Function */
void setup()
{
	// Initialize the serial port.
	Serial.begin(serialBaudrate);
}

/* Default Arduino Loop Function */
void loop()
{
	// establish variables for duration of the ping, and the distance result in inches and centimeters:
	long duration;
	double inches, cm;

	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(pingPin, OUTPUT);
	digitalWrite(pingPin, LOW);
	delayMicroseconds(2);
	digitalWrite(pingPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pingPin, LOW);

	// The same pin is used to read the signal from the PING))): a HIGH
	// pulse whose duration is the time (in microseconds) from the sending
	// of the ping to the reception of its echo off of an object.
	pinMode(pingPin, INPUT);
	duration = pulseIn(pingPin, HIGH);

	// convert the time into a distance
	inches = microsecondsToInches(duration);
	cm = microsecondsToCentimeters(duration);
	
	if(debug)
	{
		Serial.print(duration);
		Serial.print(" ms, ");
		Serial.print(inches);
		Serial.print(" in, ");
		Serial.print(cm);
		Serial.println(" cm");
	}

	delay(100);
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
