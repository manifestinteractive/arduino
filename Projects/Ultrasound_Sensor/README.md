![Arduino &amp; Fritzing Sketches](https://raw.github.com/manifestinteractive/arduino/master/assets/logo.png "Arduino &amp; Fritzing Sketches")
PING))) SENSOR
-------
This sketch reads a PING))) ultrasonic rangefinder and returns the distance to the closest object in range. To do this, it sends a pulse to the sensor to initiate a reading, then listens for a pulse to return.  The length of the returning pulse is proportional to the distance of the object from the sensor.
	
See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
	
According to Parallax's datasheet for the PING))), there are 73.746 microseconds per inch (i.e. sound travels at 1130 feet per second).  This gives the distance travelled by the ping, outbound and return, so we divide by 2 to get the distance of the obstacle.
	
The speed of sound is 340 m/s or 29.034 microseconds per centimeter. The ping travels out and back, so to find the distance of the object we take half of the distance travelled.

The circuit:

* +V connection of the PING))) attached to +5V
* GND connection of the PING))) attached to ground
* SIG connection of the PING))) attached to digital pin 7

Created 2012-07-08
