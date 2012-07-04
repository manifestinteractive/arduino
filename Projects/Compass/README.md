![Arduino &amp; Fritzing Sketches](https://raw.github.com/manifestinteractive/arduino/master/assets/logo.png "Arduino &amp; Fritzing Sketches")
HMC5883L COMPASS
-------
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