![Arduino &amp; Fritzing Sketches](https://raw.github.com/manifestinteractive/arduino/master/assets/logo.png "Arduino &amp; Fritzing Sketches")
PIR SENSOR
-------
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