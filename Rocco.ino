#include "avr/wdt.h"
#include "Servo.h"
//#define SERVO_SPEED 25 //minimum number of milliseconds per degree

//red=positive, brown=negative, orange=signal
//servo bounds and specs (using 45 to 145 regardless)
//servo01 max >= 180, min <= 0 | servo02 max >= 180, min >= 25 | servo03 max >= 180, min >= 10



//-----Prototypes-----
void getAngle(int selected_servo);
void setAngle(char incoming_serial, int selected_servo);
void selectServo(char incoming_serial);
void motionPreset(int angle[], char motion_preset);
int motion(int angle[], int servo_speed);  //will always tell all joints to move, but movement may not be required based on origin and destination
void softwareReboot(void);
//void copyArrayByRef(int *angle_ptr);


//-----Variables-----
char incoming_serial;
unsigned long servo_time = 0;
int servo_speed = 25; //number of milliseconds per degree, 25 is a good very slow
boolean incoming_motion = false;
int selected_servo = 0;
int servo_quantity = 2; //zero indexed
int angle[18] = {90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90}; //*** WHY DOES THIS HAVE TO BE GLOBAL??? ***
int angle_increment = 5;
String str_angle = "";
boolean incoming_angle = false;
char motion_preset;
Servo servo[18];



//-----Main-----
void setup()
{
  //int angle[18] = {90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90};
  //Serial.begin(9600);  //create serial object
  delay(500);
  Serial.begin(19200);
  delay(500);
  //attach servos to pins
  servo[0].attach(2);
  servo[1].attach(3);
  servo[2].attach(4);
  delay(500);
  servo[0].write(90);
  servo[1].write(90);
  servo[2].write(90);
  //motionPreset('1');
  delay(500);
  Serial.println("Ready. ");
}



//-----Loop-----
void loop()
{
  while (Serial.available() > 0)
  {
    incoming_serial = Serial.read();  

    if (incoming_serial == 'g')
    {
      getAngle(selected_servo);
    }
    else if (incoming_serial == 'k')
    {
      selectServo(incoming_serial);
    }
    else if (incoming_serial == 'r')
    {
      softwareReboot();
    }
    else if (incoming_serial == ',' || incoming_serial == '.')
    {      
      selectServo(incoming_serial);
    }
    else if (incoming_serial == 'd')
    {
      servo[selected_servo].detach();
    }
    else if (incoming_serial == '+' || incoming_serial == '-')
    {
      setAngle(incoming_serial, selected_servo);
    }
    else if ((isDigit(incoming_serial)) && (incoming_angle == true))
    {
      //***debug***
      Serial.println("...str_angle was: ");
      Serial.println(str_angle);
      Serial.println("");
      //***debug***
      Serial.println(incoming_serial);
      Serial.println("...is digit");
      //if (str_angle.length() < 3)
      //{
        //***debug***
        //Serial.println("...is less than 3");
        str_angle += incoming_serial;
        //***debug***
        Serial.println("...str_angle IS NOW: ");
        Serial.println(str_angle);

      //}
      
      if (str_angle.length() == 3)
      {
        Serial.println("str_angle IS length 3");
        //horrible hack to avoid rewriting program with incoming_serial as String,
        //i'm just going to call setAngle three times until all digits of manual angle have
        //been copied, then let setAngle decode it... gross...
        //eventually, will use Serial.readString() and/or Serial.readStringUntil('\n') to fix this.
        //there is also Serial.parseInt();
        //
        char _cheat = 'Y';
        //
        setAngle(_cheat, selected_servo);
        incoming_angle = false;
        //***debug***
        Serial.println("incoming_angle is: ");
        Serial.println(incoming_angle);
        str_angle = "";
      }
      //else
      //{
        //Serial.println("Invalid angle, move cancelled.");
        //incoming_angle = false;
        //str_angle = "";
      //}
    }
    else if (incoming_serial == 'a')
    {
      incoming_angle = true;
      //***debug***
      Serial.println("incoming_angle is: ");
      Serial.println(incoming_angle);
    }
    else if (incoming_motion == true)
    {
      if (isDigit(incoming_serial))
      {
        motion_preset = incoming_serial;
        motionPreset(angle, motion_preset);
      }
      incoming_motion = false;
    }
    else if (incoming_serial == 'm')
    {
      incoming_motion = true;
    }
    else //if incoming_serial isn't something we know...
    {
      Serial.println("Invalid request.");
    }
  }
}



//-----Get Angle Function-----
void getAngle(int selected_servo)
{ 
  int _angle = servo[selected_servo].read();
  Serial.println(_angle);
  //return _angle;
};



//-----Select Servo Function-----
void selectServo(char incoming_serial)
{
  //int _q = servo_quantity - 1;  //"zero index" servo_quantity for convenience
  
  if (incoming_serial == ',')
  {
    if (selected_servo == 0)
    {
      selected_servo = servo_quantity;
    }
    else
    {
      selected_servo -= 1;
    }
  }
  else if (incoming_serial == '.')
  {      
    if (selected_servo == servo_quantity)
    {
      selected_servo = 0;
    }
    else
    {
      selected_servo += 1;
    } 
  }
  
  int _a = servo[selected_servo].read();
  if (_a >= 45 && _a <= 140)
  {
    Serial.println("Before move: ");
    Serial.println(_a);
    
    servo[selected_servo].write(_a + 5);
    _a += 5;
    Serial.println("After move: ");
    Serial.println(_a);
    
    delay (250);
    
    servo[selected_servo].write(_a - 5);
    _a -= 5;
    Serial.println("After move back: ");
    Serial.println(_a);
  }
  else if (_a = 145)
  {
    servo[selected_servo].write(_a - 5);
    _a -= 5;
    delay (250);
    servo[selected_servo].write(_a + 5);
    _a += 5;
  }

  //***debug***
  Serial.println("Selected servo: ");
  //Serial.println(servo_quantity);
  Serial.println(selected_servo);
  delay (250);
};



//-----Set Angle Function-----
void setAngle(char incoming_serial, int selected_servo)
{
  if (incoming_serial == '+' || incoming_serial == '-')
  {
    int _angleA = servo[selected_servo].read();
    int _angleB;
    
    if (incoming_serial == '+')
    {  
      _angleB = _angleA + angle_increment;
    }
    else if (incoming_serial == '-')
    {
      _angleB = _angleA - angle_increment;
    }
    
    if (_angleB >= 45 && _angleB <= 145)
    {
      servo[selected_servo].write(_angleB);  //move servo to the new angle
      //***debug***
      Serial.println(_angleB);   
    }
    else
    {
      Serial.println("Out of bounds. ");
    }
  }
  else if (incoming_serial = 'Y')
  {
    int _a = str_angle.toInt();
    
    if (_a <= 145 && _a >= 45)
    {
      Serial.println("Moving to: ");
      Serial.println(str_angle);
      Serial.println("");
      servo[selected_servo].write(_a);
      incoming_angle = false;
      str_angle = "";
      //***debug***
      Serial.println(str_angle);
    }
    else
    {
      Serial.println(str_angle);
      Serial.println("is out of bounds. ");
      incoming_angle = false;
      str_angle = "";
    }
  }
  else
  {
    Serial.println("Undefined angle. ");
    incoming_angle = false;
    str_angle = "";
  }
};



//-----Motion Preset Function-----
void motionPreset(int angle[], char motion_preset)
{  
  Serial.println(motion_preset);
  if (motion_preset == '1')  //preset 1 = all servos at 90 degrees 
  { 
    for (int a=0; a<=17; a++)
    {
      angle[a] = 90; //update angle[] array
    }
    motion(angle, servo_speed);  //pass the updated array to the motion function
  }
  else if (motion_preset == '2')  //preset for leg all the way extended (45/45)
  {  
    angle[0] = 45;
    //Serial.println("test");
    //Serial.println(angle[0]);
    angle[1] = 45;
    //Serial.println(angle[1]);
    angle[2] = 45;
    //Serial.println(angle[2]);
    motion(angle, servo_speed);
  }
  else if (motion_preset == '3')  //preset for leg all the way contracted (145/145)
  {
    angle[0] = 145;
    angle[1] = 145;
    angle[2] = 145;
    motion(angle, servo_speed);
  }
  //else if (motion_preset == '4')  //preset for leg all the way contracted (145/145)
  //{
    //angle[0] = 90;
    //angle[1] = 45;
    //angle[2] = 45;
    //motion(angle, servo_speed);
    //char knock_motion = '+';
    //int knock_servo = 1;
    //setAngle(knock_motion, knock_servo);
    //setAngle(knock_motion, knock_servo);
    //knock_motion = '-';
    //setAngle(knock_motion, knock_servo);
    //setAngle(knock_motion, knock_servo);
  //}
  else
  {
    Serial.println("Undefined motion.");
    //Serial.println(m);
    //Serial.println("is undefined. ");
  }
};



//-----Motion Function-----
int motion(int angle[], int servo_speed)  //receive array of angles
{
  servo_time = millis();  //get the "time"
  
  for (int a=0; a<=2; a++)
  {    
    if (angle[a] < 45 || angle[a] > 145)
    {
      return 1;
    }
    Serial.println(angle[a]);
  }
  
    //int b = 0;
    int motion_tally = 0;
    //Serial.println("Before while loop, motion_tally = ");
    //Serial.println(motion_tally);
    //int motion_status[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    while (motion_tally <= 2)
    {
      Serial.println("if millis - servo_time >= servo_speed");
      if ((millis()-servo_time) >= servo_speed)  // if it's been longer than servo_speed (ex: 25ms)...
      {
        Serial.println("it is, do stuff...");
        servo_time = millis(); // save time reference for next angles update  
        
        //for all servos
        for (int b=0; b<=2; b++)  
        {
          if (angle[b] > servo[b].read())
          {
            servo[b].write(servo[b].read() + 5);    
          }
          else if (angle[b] < servo[b].read())
          {
            servo[b].write(servo[b].read() - 5);
          }
          else
          {
            motion_tally += 1;
          }
        }
      }
    }
    Serial.println("Motion complete. ");
};



//----Software Reboot Function----
void softwareReboot(void)
{
  wdt_enable(WDTO_500MS);
  while(1)
  {
  }
}
