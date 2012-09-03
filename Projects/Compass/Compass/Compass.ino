/*
======================================================================

	HMC5883L COMPASS

	For the 3-Axis HMC5883L ( Rev A 29133 ) Compass Module
	
	Detailed Info on Chipset:
	
	* http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/HMC5883L_3-Axis_Digital_Compass_IC.pdf

	Setup for Declination Angle ( required for accurate readings ):

	* Go to http://www.ngdc.noaa.gov/geomag-web/#declination
	* Enter your Zip Code in "Lookup location" in right column and click "Get Location"
	* Once your location data is populated, choose CSV file in "Calculate Declination" Result -> Result Format in the left column
	* Click "Calculate" and open the downloaded CSV File
	* Set your declinationAngle below ( line #56 ) to the "Declination in Decimal Degrees" in Column D ( Note that it might be negative, so make sure to copy that too )

	The circuit:

	* HMC5883L GND -> Arduino GND
	* HMC5883L VIN -> Arduino 5V
	* HMC5883L SCL -> Arduino Analog Pin 5
	* HMC5883L SDA -> Arduino Analog Pin 4

	Created 2012-07-04
	Peter Schmalfeldt me@peterschmalfeldt.com

	https://github.com/manifestinteractive/arduino

======================================================================

*/

/* Include Required Libraries */
#include <Wire.h>
#include <HMC5883L.h> // http://bildr.org/2012/02/hmc5883l_arduino/

/* Define Compass */
HMC5883L compass;

/*
----------------------------------------------------------------------
	CONFIGURATION VARIABLES ( CHANGE THESE TO MEET YOUR NEEDS )
----------------------------------------------------------------------
*/

/* Whether or not to debug output */
bool debug = true;

/* Serial Baudrate */
long unsigned int serialBaudrate = 9600;

/* Declination Angle in Decimal Degrees ( See Setup instruction for how to determine this ) */
float declinationAngle = -1.01244;

/* Sensor Field Range ( default 1.3 ): Valid values are: 0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1 */
float sensorFieldRange = 1.3;


/*
----------------------------------------------------------------------
	!!! YOU DO NOT NEED TO CHANGE ANYTHING BELOW HERE !!!
----------------------------------------------------------------------
*/

/* Record any errors that may occur in the compass. */
int error = 0;

/* Default Arduino Setup Function */
void setup()
{
	// Initialize the serial port.
	Serial.begin(serialBaudrate);

	// Start the I2C interface.
	if(debug)
	{
		Serial.println("Starting the I2C interface.");
	}

	Wire.begin();

	// Construct a new HMC5883 compass.
	if(debug)
	{
		Serial.println("Constructing new HMC5883L");
	}

	compass = HMC5883L();

	// Set the scale of the compass.
	if(debug)
	{
		Serial.print("Setting scale to +/- ");
		Serial.print(sensorFieldRange);
		Serial.println(" Ga");
	}

	// A bug in HMC5883L lib returns 512 for error, which is not an actual error, so ignoring
	error = compass.SetScale(sensorFieldRange);
	if(error != 0 && error != 512)
	{
		if(debug)
		{
			Serial.print("ERROR: ");
			Serial.println(error);
			Serial.println();
			Serial.println(compass.GetErrorText(error));
		}
		error = 0;
	}

	// Set the measurement mode to Continuous
	if(debug)
	{
		Serial.println("Setting measurement mode to continuos.");
	}

	error = compass.SetMeasurementMode(Measurement_Continuous);
	if(error != 0 && error != 512)
	{
		if(debug)
		{
			Serial.print("ERROR: ");
			Serial.println(error);
			Serial.println();
			Serial.println(compass.GetErrorText(error));
		}
		error = 0;
	}
}

/* Default Arduino Loop Function */
void loop()
{
	// Retrive the raw values from the compass (not scaled).
	MagnetometerRaw raw = compass.ReadRawAxis();

	// Retrived the scaled values from the compass (scaled to the configured scale).
	MagnetometerScaled scaled = compass.ReadScaledAxis();

	// Values are accessed like so:
	int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

	// Calculate heading when the magnetometer is level, then correct for signs of axis.
	float heading = atan2(scaled.YAxis, scaled.XAxis);
	heading += declinationAngle;

	// Correct for when signs are reversed.
	if(heading < 0)
	{
		heading += 2*PI;
	}

	// Check for wrap due to addition of declination.
	if(heading > 2*PI)
	{
		heading -= 2*PI;
	}

	// Convert radians to degrees for readability.
	float headingDegrees = heading * 180/M_PI;

	// Output the data via the serial port.
	useCompass(raw, scaled, heading, headingDegrees);
}

/* Use Compass Data to do something cool */
void useCompass(MagnetometerRaw raw, MagnetometerScaled scaled, float heading, float headingDegrees)
{
	if(debug)
	{
		Serial.print("Raw:\t");
		Serial.print(raw.XAxis);
		Serial.print("   ");
		Serial.print(raw.YAxis);
		Serial.print("   ");
		Serial.print(raw.ZAxis);
		Serial.print("   \tScaled:\t");

		Serial.print(scaled.XAxis);
		Serial.print("   ");
		Serial.print(scaled.YAxis);
		Serial.print("   ");
		Serial.print(scaled.ZAxis);

		Serial.print("   \tHeading:\t");
		Serial.print(heading);
		Serial.print(" Radians   \t");
		Serial.print(headingDegrees);
		Serial.print(" Degrees   \t");

		Serial.println(getDirection(headingDegrees));
	}
	
	// DO SOMETHING COOL HERE
}

/* What good is all this if we can't set the expected directional info */
String getDirection(int headingDegrees)
{
	if (headingDegrees < 0 || headingDegrees > 360)
	{
		return false;
	}
	else if (headingDegrees >= 0 && headingDegrees <= 11.25)
	{
		return "N";
	}
	else if (headingDegrees > 348.75 && headingDegrees <= 360)
	{
		return "N";
	}
	else if (headingDegrees > 11.25 && headingDegrees <= 33.75)
	{
		return "NNE";
	}
	else if (headingDegrees > 33.75 && headingDegrees <= 56.25)
	{
		return "NE";
	}
	else if (headingDegrees > 56.25 && headingDegrees <= 78.75)
	{
		return "ENE";
	}
	else if (headingDegrees > 78.75 && headingDegrees <= 101.25)
	{
		return "E";
	}
	else if (headingDegrees > 101.25 && headingDegrees <= 123.75)
	{
		return "ESE";
	}
	else if (headingDegrees > 123.75 && headingDegrees <= 146.25)
	{
		return "SE";
	}
	else if (headingDegrees > 146.25 && headingDegrees <= 168.75)
	{
		return "SSE";
	}
	else if (headingDegrees > 168.75 && headingDegrees <= 191.25)
	{
		return "S";
	}
	else if (headingDegrees > 191.25 && headingDegrees <= 213.75)
	{
		return "SSW";
	}
	else if (headingDegrees > 213.75 && headingDegrees <= 236.25)
	{
		return "SW";
	}
	else if (headingDegrees > 236.25 && headingDegrees <= 258.75)
	{
		return "WSW";
	}
	else if (headingDegrees > 258.75 && headingDegrees <= 281.25)
	{
		return "W";
	}
	else if (headingDegrees > 281.25 && headingDegrees <= 303.75)
	{
		return "WNW";
	}
	else if (headingDegrees > 303.75 && headingDegrees <= 326.25)
	{
		return "NW";
	}
	else if (headingDegrees > 326.25 && headingDegrees <= 348.75)
	{
		return "NNW";
	}
}