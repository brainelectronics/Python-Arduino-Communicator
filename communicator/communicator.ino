/*
  Arduino Sketch for the Python Communication
  Upload this Sketch to the communicating controller
  
  you can use following commands:
    "s1D12" to set digital Pin D12 to 1 (HIGH)
    "s0D9" to set digital Pin D9 to 0 (LOW)
    "s217D3" to set digital Pin D3 to 217 (PWM)
    "gD1" to get the value of digital Pin D1
    "gA0" to get the value of analog Pin A0
    "#d1" to switch serial debug output on
    "#f0" to switch serial feedback output off
  
  by Jonas Scharpf
  brainelectronics.de
*/

static char buffer[10];
byte b = 0;
byte thePin;
int theValue;
String str;
boolean DEBUG = 0;
boolean FEEDBACK = 1;

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega32U4__)
  #define numberOfAnalogPins 5
  #define numberOfDigitalPins 13
  #define numberOfPWMPins 5
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define numberOfAnalogPins 15
  #define numberOfDigitalPins 54
  #define numberOfPWMPins 13
#else
  #define numberOfAnalogPins 0
  #define numberOfDigitalPins 0
  #define numberOfPWMPins 0
#endif

void setup()
{
  // setup all pin definitions
  
  Serial.begin(19200);
  delay(100);  // just wait a bit...
  Serial.println("Communicator by brainelectronics");
  if (FEEDBACK)
  {
    Serial.println("Feedback is on");
  }
  else
  {
    Serial.println("Feedback is off");
  }
  if (DEBUG)
  {
    Serial.println("Debug is on");
  }
  else
  {
    Serial.println("Debug is off");
  }
}

void loop() // run over and over
{
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    buffer[b] = c;
    
    if (c == '\n' || c == '\r')  // got a end of line
    {
      String str(buffer);
      if (DEBUG) Serial.print("Received via serial: ");
      if (DEBUG) Serial.print(buffer);
      
      if (buffer[0] == 's' || buffer[0] == 'S')  // you want to set a pin...
      {        
        byte startIndex = str.indexOf('d');
        
        if (startIndex > 250)  // 'd' has not be found, search for 'D'
        {
          startIndex = str.indexOf('D');
        }
        
        if (startIndex == 255)
        {
          if (DEBUG) Serial.println("Start index unknown!");
          thePin = 250;
          theValue = 999;
        }
        else  // a valid start index
        {
          thePin = str.substring(startIndex+1).toInt();  // the specified pin
          theValue = str.substring(1, startIndex).toInt();  // the specified value
        }
        
        //if (DEBUG) Serial.print("Start index ");
        //if (DEBUG) Serial.println(startIndex);
        
        if (FEEDBACK) Serial.print("You want to SET pin ");
        
        if (FEEDBACK || DEBUG) Serial.print(thePin);
        if (FEEDBACK || DEBUG) Serial.print(" to: ");
        if (FEEDBACK || DEBUG) Serial.println(theValue);
        
        if (numberOfDigitalPins >= thePin)  // pin is generally available
        {
          if (theValue > 1)  // you want to set a pwm
          {
            if (theValue > 255)  // the pwm is very large, too large!
            {
              if (FEEDBACK) Serial.println("allowed PWM range is 0-255, set to max (255)");
              analogWrite(thePin, 254);
            }
            if (numberOfPWMPins >= thePin)  // pin is available
            {
              analogWrite(thePin, theValue);  // set the pwm at the pin
              if (FEEDBACK) Serial.println("PWM set");
            }
            else
            {
              if (FEEDBACK) Serial.println("No such pwm pin to set to");
            }
          }
          else if (theValue < 0)  // received a value lower than 0
          {
            if (FEEDBACK) Serial.println("You can not set the pin lower than 0");
          }
          else  // you want to set a digital pin
          {
            digitalWrite(thePin, theValue);
            if (FEEDBACK) Serial.println("Digital Pin set");
          }
        }
        else
        {
          Serial.println("No such digital pin to set to");
        }
      }
      else if (buffer[0] == 'g' || buffer[0] == 'G')  // you want to get a pin value...
      {
        if (FEEDBACK) Serial.print("You want to GET pin ");
        
        thePin = str.substring(2).toInt();

        if (buffer[1] == 'a' || buffer[1] == 'A')  // you want to get an analog value
        {
          if (numberOfAnalogPins >= thePin)  // pin is available
          {
            if (FEEDBACK || DEBUG) Serial.println(thePin);
            if (FEEDBACK) Serial.println(analogRead(thePin));
          }
          else
          {
            if (FEEDBACK) Serial.println("No such analog pin to read from");
          }
        }
        else if (buffer[1] == 'd' || buffer[1] == 'D')  // you want to get an digital value
        {
          if (numberOfDigitalPins >= thePin)
          {
            if (FEEDBACK || DEBUG) Serial.println(thePin);
            if (FEEDBACK) Serial.println(digitalRead(thePin));
          }
          else
          {
            if (FEEDBACK) Serial.println("No such digital pin to read from");
          }
        }
      }
      else if (buffer[0] == '#')  // you want to set the serial com...
      {
        if (buffer[1] == 'd' || buffer[1] == 'D')  // you want to toggle Debug com
        {
          if (buffer[2] == '0')  // toggle off
          {
            DEBUG = 0;
            Serial.println("Debug com inactive");
          }
          else if (buffer[2] == '1')  // toggle on
          {
            DEBUG = 1;
            Serial.println("Debug com active");
          }
          else
          {
            Serial.println("Unknown toggle state of Debug com");
          }
        }
        else if (buffer[1] == 'f' || buffer[1] == 'F')  // you want to toggle Feedback com
        {
          if (buffer[2] == '0')  // toggle off
          {
            FEEDBACK = 0;
            Serial.println("Feedback com inactive");
          }
          else if (buffer[2] == '1')  // toggle on
          {
            FEEDBACK = 1;
            Serial.println("Feedback com active");
          }
          else
          {
            Serial.println("Unknown toggle state of Feedback com");
          }
        }
        else
        {
          Serial.print("Unknown serial command");
          Serial.println(buffer);
        }
      }
      b = 0;  // reset the index of the buffer
      memset(buffer, 0, sizeof(buffer));  // clear the buffer
      if (DEBUG) Serial.println();  // print a new line, to be more clear
    }
    else
    {
      b++;  // increase the index because of no line end
    }
  }
}
