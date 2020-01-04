Rocco
=====

A hexapod robot built on an Arduino Mega 2560 R3 using C.


Please see the complete documentation of the build process on my blog at the following link:
http://blog.lennyevans.net


The model is available in exported formats in this repo or online at Tinkercad (it is not ready for 3D printing):
https://tinkercad.com/things/i91TkKbNDc9


The motion function uses the servo_interval variable to determine how many degrees each servo should move at a time on the way to completing a motion.  It also uses the servo_speed variable to determine how frequently each servo should move.

Key List
--------

"[" or "]"   |   Increase/decrease servo speed - Rocco continues executing a motion every "servo_speed" milliseconds until motion is complete

"s###"   |   Set a three digit servo speed (default is 25ms, maximum 999ms)

"i##"   |   Set a two digit servo interval (default is 32 which is the maximum value)

"p"   |   Toggle usage of potentiometer for reading the servo interval (potentiometer is used by default)

"j##"   |   Select a servo using a two digit servo number

"," (the "<" key)   |   Select previous servo

"." (the ">" key)   |   Select next servo

"k"   |   "Knock" selected servo (move +5 degrees then -5 degrees just to see which servo is selected)

"a###"   |   Set the angle of current servo, requires three digit angle (add leading zeros if necessary)

"g"   |   Get angle of selected servo

"m##" (00-16 currently)   |   Execute preset motion

"+"   |   Selected servo plus 5 degrees

"-"   |   Selected servo minus 5 degrees

"d"   |   Disconnect selected servo (reset required to reconnect)

"r"   |   Reset
