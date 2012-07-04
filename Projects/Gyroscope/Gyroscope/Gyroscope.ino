/*
======================================================================

	GYROSCOPE L3G4200D

	For the 3-Axis L3G4200D ( Rev A 27911 ) Gyroscope Module
	
	Helpful Info: http://en.wikipedia.org/wiki/Flight_dynamics
	
	* Pitch ( 3D X Axis )
	* Roll ( 3D Y Axis )
	* Yaw ( 3D Z Axis )

	The circuit:

	* L3G4200D GND -> Arduino GND
	* L3G4200D VIN -> Arduino 5V
	* L3G4200D SCL -> Arduino Analog Pin 5
	* L3G4200D SDA -> Arduino Analog Pin 4

	Created 2012-07-03
	Peter Schmalfeldt me@peterschmalfeldt.com

	https://github.com/manifestinteractive/arduino

======================================================================

----------------------------------------------------------------------
	CONFIGURATION VARIABLES ( CHANGE THESE TO MEET YOUR NEEDS )
----------------------------------------------------------------------
*/

/* Whether or not to debug output */
bool debug = true;

/* Time between Serial reads in milliseconds */
int read_delay = 250;

/* Serial Baudrate */
long unsigned int serial_baudrate = 9600;

/*
----------------------------------------------------------------------
	!!! YOU DO NOT NEED TO CHANGE ANYTHING BELOW HERE !!!
----------------------------------------------------------------------
*/

/* Include Required Libraries */
#include <Wire.h>
#include <L3G4200D.h> // https://github.com/pololu/L3G4200D

/* Define Gyrsocope */
L3G4200D gyro;

/* Default Arduino Setup Function */
void setup()
{
    Serial.begin(serial_baudrate);
    Wire.begin();
    gyro.enableDefault();
}

/* Default Arduino Loop Function */
void loop()
{
	// Get Gyro Data
	gyro.read();
	
	// Use Gyro Data
	useGyro((int)gyro.g.x, (int)gyro.g.y, (int)gyro.g.z);
	
	// Wait before doing another read
	delay(read_delay);
}

/* Use Gyroscopes Data to do something cool */
void useGyro(int pitch, int roll, int yaw)
{
	if(debug)
	{
		String debug_ouptut;
		debug_ouptut += "pitch: ";
		debug_ouptut += pitch;
		debug_ouptut += ", roll: ";
		debug_ouptut += roll;
		debug_ouptut += ", yaw: ";
		debug_ouptut += yaw;
		
		Serial.println(debug_ouptut);
	}
	
	// YOUR CODE HERE
}