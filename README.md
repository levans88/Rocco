Rocco
=====

A hexapod robot built on an Arduino Mega 2560 R3.

The code is somewhat (but not completely) written with 18 leg servos in mind. Only one leg is built currently, so I'm testing with three servos.

Please see the complete documentation on my blog at the link below.
http://blog.lennyevans.net

This is an active project so please see issues and updates on GitHub as well.
https://github.com/levans88/Rocco/issues

The main focus of the code right now is to read input over a serial connection and execute appropriate functions based on the characters received.  The core function handling movement is "motion".

There are no inverse or forward kinematics involved, only motion presets which are defined in the motionPreset function.  Destination angles are set in that function as well and passed to the motion function to be executed.

The motion function uses the servo_interval variable to determine how many degrees each servo should move at a time on the way to completing a motion.  It also uses the servo_speed variable to determine how frequently each servo should move on the way to completing a motion.

Key List
--------

"[" or "]"   |   Increase/decrease servo speed - Rocco continues executing a motion every "servo_speed" milliseconds until motion is complete

"s###"   |   Set a three digit servo speed (default is 25ms, maximum 999ms)

Hardware potentiometer   |   Increase/decrease servo_interval - Rocco moves each servo "servo_interval" number of degrees until executed motion is complete

"i##"   |   Set a two digit servo interval (default is 32 which is the maximum value)

"p"   |   Toggle usage of potentiometer for reading the servo interval (potentiometer is used by default)

"j##"   |   Select a servo using a two digit servo number

"," (the "<" key)   |   Select previous servo

"." (the ">" key)   |   Select next servo

"k"   |   "Knock" selected servo (move +5 degrees then -5 degrees just to see which servo is selected)

"a###"   |   Set the angle of current servo, requires three digit angle (add leading zeros if necessary)

"g"   |   Get angle of selected servo

"m#" (1-3 currently)   |   Execute pre-coded motion

"+"   |   Selected servo plus 5 degrees

"-"   |   Selected servo minus 5 degrees

"d"   |   Disconnect selected servo (reset required to reconnect)

"r"   |   Reset
