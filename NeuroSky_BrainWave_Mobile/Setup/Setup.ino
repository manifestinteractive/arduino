/*
======================================================================

	NEUROSKY BRAINWAVE MOBILE - SETUP

	Arduino & BlueSMiRF Gold Shield Programming & Factory Reset.

	The circuit:

	* See Wiring Diagram in Fritzing File

	Created 2012-06-19
	Peter Schmalfeldt me@peterschmalfeldt.com

	http://arduino.peterschmalfeldt.com

======================================================================

----------------------------------------------------------------------
	CONFIGURATION VARIABLES ( CHANGE THESE TO MEET YOUR NEEDS )
----------------------------------------------------------------------
*/

// Enable Debug Output
const boolean ENABLE_DUBUG_OUTPUT = true;

// Enable Factory Reset
const boolean ENABLE_FACTORY_RESET = true;

// Bluetooth MAC Address to your NeuroSky BrainWave Mobile
const char BLUETOOTH_MAC_ADDRESS[13] = "9CB70D75A7DD";

// Bluetooth Pass Key for you NeuroSky BrainWave Mobile ( should be "0000" )
const char BLUETOOTH_PASS_KEY[5] = "0000";

/*
----------------------------------------------------------------------
	!!! YOU DO NOT NEED TO CHANGE ANYTHING BELOW HERE !!!
----------------------------------------------------------------------
*/

// Has the process completed
boolean is_complete = false;

// Was the process successful
boolean is_successful = false;

// Check if BlueSMiRF device is in Command Mode
boolean in_command_mode = false;

// Has the Bluetooth device been Shutdown
boolean shutdown = false;

// Response from Serial connection
String response;

// Individual Letter from response
char letter[1];

// BlueSMiRF Baudrate
unsigned long BAUDRATE = 115200;

// Pin that powers BlueSMiRF
const unsigned int PIN_DIGITAL_ON = 7;

// Red LED,   connected to digital pin 11
const unsigned int PIN_LED_RED = 11;

// Green LED, connected to digital pin 10
const unsigned int PIN_LED_GREEN = 10;

// Blue LED,  connected to digital pin 9
const unsigned int PIN_LED_BLUE = 9;

// Delay Between Bluetooth Messages
const unsigned int DELAY = 500;

/* Default Arduino Setup Function */
void setup()
{
	// Initialize pins
	pinMode(PIN_DIGITAL_ON, OUTPUT);
	pinMode(PIN_LED_RED, OUTPUT);
	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_LED_BLUE, OUTPUT);

	// Turn BlueSMiRF on
	digitalWrite(PIN_DIGITAL_ON, HIGH);

	// Wait for BlueSMiRF to finish turning on
	delay(DELAY);

	// Initialize serial ports
	if(ENABLE_FACTORY_RESET)
	{
		BAUDRATE = 57600;
	}
        
	Serial.begin(115200);

	// Enter Programming Mode
	enterProgrammingMode();
}

/* Default Arduino Loop Function */
void loop()
{
	// Everything loaded and is complete
	if(is_successful && is_complete && !in_command_mode && !shutdown)
	{

		// Turn BlueSMiRF off, we're done
		digitalWrite(PIN_DIGITAL_ON, LOW);

		Serial.flush();

		logOutput("Completed Successfully :)");

		Serial.end();

		// Fade RGB LED to Green
		fadeLed(0, 255, 0);

		// Update that we have shutdown bluetooth board
		shutdown = true;
	}

	// We completed, but not everything was a success
	if(is_complete && !is_successful && !shutdown)
	{
		// Turn BlueSMiRF off, we're done
		digitalWrite(PIN_DIGITAL_ON, LOW);

		Serial.flush();

		logOutput("Completed Unsuccessfully :(");

		Serial.end();

		delay(5000);

		// Fade RGB LED to Green
		fadeLed(255, 0, 0);

		// Update that we have shutdown bluetooth board
		shutdown = true;
	}
}

/* Enter Programming Mode */
void enterProgrammingMode()
{
	// Show Activity
	blinkLed(255, 255, 255, 5, 50);

	// Send command to put BlueSMiRF into programming mode
	if(sendCommand("$$$", "CMD"))
	{
		in_command_mode = true;
	}
	else
	{
		is_complete = true;
		logOutput("Failed setting BlueSMiRF into Command Mode");
		return;
	}

	// Make sure we are in programming mode before doing anything else
	if(in_command_mode)
	{
		// Reset the BlueSMiRF
		if(ENABLE_FACTORY_RESET)
		{
			factoryReset();
		}
		// Setup the BlueSMiRF data
		else
		{
			configureBluetooth();
		}
	}
}

/* Perform Factory Reset */
void factoryReset()
{
	// Show Activity
	blinkLed(0, 0, 255, 5, 50);

	// Perform Factory Reset
	if( !sendCommand("SF,1", "AOK"))
	{
		is_complete = true;
		logOutput("Failed factory resetting BlueSMiRF");
		return;
	}

	delay(DELAY);

	// Set Baudrate to 57600
	if( !sendCommand("SU,115", "AOK"))
	{
		is_complete = true;
		logOutput("Failed setting BlueSMiRF Baudrate");
		return;
	}
	
	// Continue on to Configure Bluetooth now that it was reset
	configureBluetooth();
}

/* Configure BlueSMiRF Bluetooth with your settings */
void configureBluetooth()
{
	// Show Activity
	blinkLed(255, 255, 255, 5, 50);

	// Perform Factory Reset
	String build_command = "SR,";
	build_command += BLUETOOTH_MAC_ADDRESS;

	if( !sendCommand(build_command, "AOK"))
	{
		is_complete = true;
		logOutput("Failed to set MAC Address");
		return;
	}

	//Set the passkey
	build_command = "SP,";
	build_command += BLUETOOTH_PASS_KEY;

	if( !sendCommand(build_command, "AOK"))
	{
		is_complete = true;
		logOutput("Failed to set Pass Key");
		return;
	}

	// Set the BlueSMiRF mode
	if( !sendCommand("SM,3", "AOK"))
	{
		is_complete = true;
		logOutput("Failed to Change Mode");
		return;
	}

	// Exit command mode
	if( !sendCommand("---", "END"))
	{
		is_complete = true;
		logOutput("Failed to exit BlueSMiRF CMD Mode");
		return;
	}

	// Update Setting and Exist Factory Reset
	in_command_mode = false;
	is_complete = true;
	is_successful = true;
}

/* Handy Function to Send Commands to BlueSMiRF */
bool sendCommand(String command, String expected_response)
{
	// Flush Any Existing Data from Other Calls
	Serial.flush();

	// Send command to BlueSMiRF, but check if its the initial $$$ command since it cannot have a return
	if(command == "$$$")
	{
		Serial.print(command);
	}
	else
	{
		Serial.println(command);
	}

	// Wait for BlueSMiRF to receive command
	delay(DELAY);

	// Fetch Response
	response = "";
	while(Serial.available() > 0)
	{
		letter[0] = (char)Serial.read();
		if(letter[0] != '\r' && letter[0] != '\n')
		{
			response += letter[0];
		}
	}

	// Flush Response
	Serial.flush();

	// Make sure we got the response we wanted
	if(response == expected_response)
	{
		return true;
	}
	else
	{
		logOutput("Executing Command: " + command);
		logOutput("Expected Response: " + expected_response);
		logOutput("Actual Response: " + response);

		return false;
	}
}

/* Fade In/Out RGB Led */
void fadeLed(int red, int green, int blue)
{
	int new_red = 0;
	int new_green = 0;
	int new_blue = 0;

	// Fade RGB LED to Red
	for(int fadeValue = 255; fadeValue >= 0; fadeValue -=5)
	{
		new_red = (red == 0) ? fadeValue : red;
		new_green = (green == 0) ? fadeValue : green;
		new_blue = (blue == 0) ? fadeValue : blue;

		analogWrite(PIN_LED_RED, new_red);
		analogWrite(PIN_LED_GREEN, new_green);
		analogWrite(PIN_LED_BLUE, new_blue);

		delay(10);
	}

	delay(2500);
	
	blinkLed(new_red, new_green, new_blue, 5, 100);
	
	delay(2500);

	// Fade RGB LED Off
	for(int fadeValue = 255; fadeValue >= 0; fadeValue -=5)
	{
		new_red = (red == 0) ? red : fadeValue;
		new_green = (green == 0) ? green : fadeValue;
		new_blue = (blue == 0) ? blue : fadeValue;

		analogWrite(PIN_LED_RED, new_red);
		analogWrite(PIN_LED_GREEN, new_green);
		analogWrite(PIN_LED_BLUE, new_blue);

		delay(10);
	}
}

/* Blink RGB Led */
void blinkLed(int red, int green, int blue, int count, int speed)
{
	for(int i = 0; i < count; i++)
	{
		if ( (i % 2) == 0)
		{
			analogWrite(PIN_LED_RED, red);
			analogWrite(PIN_LED_GREEN, green);
			analogWrite(PIN_LED_BLUE, blue);
		}
		else
		{
			analogWrite(PIN_LED_RED, 0);
			analogWrite(PIN_LED_GREEN, 0);
			analogWrite(PIN_LED_BLUE, 0);
		}

		delay(speed);
	}
}

/* Log Output if Debugging is Enabled */
void logOutput(String message)
{
	if(ENABLE_DUBUG_OUTPUT)
	{
		Serial.println(message);
	}
}
