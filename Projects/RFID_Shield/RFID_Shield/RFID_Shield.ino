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

int PinGoodTag = 7;
int PinBadTag = 9;
bool Debug = true;

/* Default Arduino Setup Function */
void setup()
{
	// connect to the serial port
	Serial.begin(9600);

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
			tagString[index] = (char)readByte;
			index ++;
		}
	}

	// Check if it is a match
	checkTag(tagString);
	
	// Clear out memory of tag string
	memset(tagString,0,sizeof(tagString));

	// Give Reader time to Reset
	delay(150);
}

/* Check the read tag against known tags */
void checkTag(char tag[])
{
	// Setup Custom Variables to Hold RFID Data
	String scannedRFID;
	String acceptedRFID;

	// Cleanup Scanned RFID
	scannedRFID = tag;
	scannedRFID = scannedRFID.substring(0, 11);

	// empty, no need to contunue
	if(scannedRFID.length() != 11)
	{
		return;
	}

	// get size of tags
	int size = sizeof(myTags) / sizeof(myTags[0]);

	// loops through tags for validation
	for (int i = 0; i < size; i++)
	{
		// Cleanup Accepted RFID
		acceptedRFID = myTags[i];
		acceptedRFID = acceptedRFID.substring(0, 11);

		// Debug Output
		if(Debug)
		{
			Serial.print(scannedRFID);
			if(scannedRFID.equals(acceptedRFID))
			{
				Serial.print(" == ");
			}
			else
			{
				Serial.print(" != ");
			}
			Serial.print(acceptedRFID);
			Serial.println();
		}

		// Check if we have a match
		if(scannedRFID.equals(acceptedRFID))
		{
			// Trigger Good Tag
			goodTag(scannedRFID);

			return;
		}
	}

	// Trigger Bad Tag
	badTag(scannedRFID);
}


/* This was a Good Tag */
void goodTag(String tag)
{
	if(Debug)
	{
		Serial.println("GOOD TAG: " + tag);
		Serial.println();
	}

	digitalWrite(PinGoodTag, HIGH);
	delay(1000);
	digitalWrite(PinGoodTag, LOW);
}

/* This was a Bad Tag */
void badTag(String tag)
{
	if(Debug)
	{
		Serial.println("BAD TAG: " + tag);
		Serial.println();
	}

	digitalWrite(PinBadTag, HIGH);
	delay(1000);
	digitalWrite(PinBadTag, LOW);
}
