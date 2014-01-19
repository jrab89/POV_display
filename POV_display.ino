float rpm;        //the RPM, updated every 100 rotations
int rotations;    //the number of rotations since the start of the program
int go;           //start toggling lights when this is 1, otherwise 0
int seconds;      //seconds since the start of the program
int lastUpdate;   //millis since last RPM update
float radsPerSec; //the rpm converted to the angular velocity

int LEDs[11] = {
  0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11};

void setup()
{
  //setup an interrupt on digital pin 3 so we know when the sensor is at midnight
  attachInterrupt(1, hallIsOn, RISING);

  //Serial.begin(9600);

  //setup output pins for LEDS, when these pins are high they on
  for(int i = 0; i <  11; i++)
  {
    pinMode(LEDs[i], OUTPUT);
    digitalWrite(LEDs[i], LOW);
  }

  go = 0;
  rpm = 0;
  lastUpdate = 0;
  seconds = 0;
  radsPerSec = 0;
}

void loop()
{
  //print of every 20th rotation
  //if(rotations % 20 == 0)
  //{
  //  Serial.println(rotations, DEC);
  //}

  float t = 0;
  int pi2DivBy60 = 0;

  //Update RPM every 100 rotations
  if (rotations >= 100) 
  { 
    //calculate rpm from the milliseconds it look to do the last 100 rotations
    rpm = 60.0*1000.0/((float)(millis() - lastUpdate))*rotations;

    //convert rpm to angular velocity
    radsPerSec = rpm * .1047;

    //set the lastUpdate time to now
    lastUpdate = millis();

    //reset rotations to 0
    rotations = 0;

	//"t" is the milliseconds it takes to get from midnight (0) to 1 second (2pi/60), given the current radial velocity
    t = 1000*2*3.14159/60/radsPerSec;

    //the delay routine expects an integer
    pi2DivBy60 = int(1000*2*3.14159/60/radsPerSec);

    //print out rpm via serial
    //Serial.println(rpm, DEC);
  }

  if(go)
  {
    //"seconds" is the current position of the second hand
    seconds = (millis() / 1000) % 60;
	
    delay(pi2DivBy60*seconds);    //wait the time required to travel from midnight to the current second

    //digitalWrite(11, HIGH);     //toggle the LEDS on
    for(int i = 0; i <  11; i++)
    {
      digitalWrite(LEDs[i], HIGH);
    }
    
	delay(pi2DivBy60);            //wait the time required to travel 1 second

    //digitalWrite(11, LOW);      //togle the LEDS off
    for(int i = 0; i <  11; i++)
    {
      digitalWrite(LEDs[i], LOW);
    }

    go = 0;
  }
}

void hallIsOn()
{
  rotations++;
  go = 1;
}
