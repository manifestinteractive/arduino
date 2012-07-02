/*
======================================================================

	RFID READER

	RFID Reader ID-12 for Arduino

	The circuit:

	* Disconnect the rx serial wire to the ID-12 when uploading the sketch

	Created 2012-07-01
	Peter Schmalfeldt me@peterschmalfeldt.com

	https://github.com/manifestinteractive/arduino

======================================================================

----------------------------------------------------------------------
	CONFIGURATION VARIABLES ( CHANGE THESE TO MEET YOUR NEEDS )
----------------------------------------------------------------------
*/

// Register your RFID tags here
char* myTags[] = {
	"4C0020FE8C1E",
	"4C0020FA5BCD"
};

/*
----------------------------------------------------------------------
	!!! YOU DO NOT NEED TO CHANGE ANYTHING BELOW HERE !!!
----------------------------------------------------------------------
*/

int RFIDResetPin = 13;
int PinGoodTag = 7;
int PinBadTag = 9;

/* Default Arduino Setup Function */
void setup()
{
	// connect to the serial port
	Serial.begin(9600);

	pinMode(RFIDResetPin, OUTPUT);
	digitalWrite(RFIDResetPin, HIGH);

	pinMode(PinGoodTag, OUTPUT);
	pinMode(PinBadTag, OUTPUT);
}

/* Default Arduino Loop Function */
void loop()
{
	char tagString[13];
	int index = 0;
	boolean reading = false;

	while(Serial.available())
	{
		// read next available byte
		int readByte = Serial.read();

		// begining of tag
		if(readByte == 2)
		{
			reading = true;
		}
		// end of tag
		if(readByte == 3)
		{
			reading = false;
		}

		if(reading && readByte != 2 && readByte != 10 && readByte != 13)
		{
			//store the tag
			tagString[index] = readByte;
			index ++;
		}
	}

	// Check if it is a match
	checkTag(tagString);

	// Clear the char of all value
	clearTag(tagString);

	// Reset the RFID reader
	resetReader();
}

/* Check the read tag against known tags */
void checkTag(char tag[])
{
	// empty, no need to contunue
	if(strlen(tag) == 0)
	{
		return;
	}

	// get size of tags
	int size = sizeof(myTags);

	// loops through tags for validation
	for (int i = 0; i < size; i++)
	{
		// set flag to valid if it was in the list
		if(compareTag(tag, myTags[i]))
		{
			// Trigger Good Tag
			goodTag(tag);

			return;
		}
	}

	// Trigger Bad Tag
	badTag(tag);
}

/* Turn on LED on pin "pin" for 1000ms */
void lightLED(int pin)
{
	digitalWrite(pin, HIGH);
	delay(1000);
	digitalWrite(pin, LOW);
}

/* Reset the RFID reader to read again. */
void resetReader()
{
  digitalWrite(RFIDResetPin, LOW);
  digitalWrite(RFIDResetPin, HIGH);
  delay(150);
}

/* clear the char array by filling with null - ASCII 0 Will think same tag has been read otherwise */
void clearTag(char one[])
{
	for(int i = 0; i < strlen(one); i++)
	{
    	one[i] = 0;
	}
}

/* compare two value to see if same, strcmp not working 100% so we do this */
boolean compareTag(char one[], char two[])
{
	// empty
	if(strlen(one) == 0)
	{
		return false;
	}

  	for(int i = 0; i < 12; i++)
	{
    	if(one[i] != two[i])
		{
			return false;
		}
  	}

	// no mismatches
	return true;
}

/* This was a Good Tag */
void goodTag(String tag)
{
	Serial.println("GOOD TAG: " + tag);

	lightLED(PinGoodTag);
}

/* This was a Bad Tag */
void badTag(String tag)
{
	Serial.println("BAD TAG: " + tag);

	lightLED(PinBadTag);
}
