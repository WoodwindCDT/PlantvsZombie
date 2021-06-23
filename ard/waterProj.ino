#include <TimeLib.h>

int x=0;
int servotime=0;

const int runsw=0;
const int waterlevel=1;
const int pump=2;
const int lowalertlamp=3;
const int runninglamp=4;
const int servo=5;

int lastmin=0;
int lastsec=0;
int sysstate=0;
int servoangle=0;
int servodelay=0;

bool runswitch=false;
bool waterok=false;
bool pumpon=false;
bool lowalerton=false;
bool runninglampon=false;

void setup() {
    // put your setup code here, to run once:
  Serial.begin(9600);
  // Serial.println(8);
  pinMode(runsw,INPUT_PULLUP); //this is pin 0
  pinMode(waterlevel,INPUT_PULLUP); //this is pin 1
  
  pinMode(pump,OUTPUT); //this is pin 2
  pinMode(lowalertlamp,OUTPUT); //this is pin 3
  pinMode(runninglamp,OUTPUT); //this is pin 4
  pinMode(servo,OUTPUT); //this is pin 5
  pinMode(13,OUTPUT); //this is the onboard led
}

void loop() {
  // put your main code here, to run repeatedly

  time_t t = Teensy3Clock.get();

  //this just prints the time to the serial monitor if the minutes change
  //it also flashes the onboard led for a second if the minutes change
  //can be deleted
  if (minute(t)!=lastmin){
    lastmin=minute(t);
    Serial.print(hour(t));
    Serial.print(":");
    if (minute(t)<10){
    Serial.print("0");
    Serial.println(minute(t));
  }

  if (minute(t)>9){
    Serial.println(minute(t));
  }
  if((hour(t)==15) & (minute(t)==10)){
    Serial.println("It's 3:10PM");
  }
  Serial.print("sysstate = ");
  Serial.println(sysstate);
  Serial.print("lowalert = ");
  Serial.println(lowalerton);
  digitalWrite(13,HIGH);
  delay(100);
  digitalWrite(13,LOW);
}

//left this in to see the format for referencing the time
//Serial.print(hour(t));
//Serial.print(minute(t));
//Serial.print(second(t));
//Serial.print(month(t));
//Serial.print(day(t));
//Serial.println(year(t));

//runswitch=!digitalRead(runsw); //if the runsw is on, runswitch if true
//waterok=!digitalRead(waterlevel); //if the water level is ok waterok is true
//scaninputs();

if (sysstate==0){
  scaninputs();
  servoangle=0;
  pumpon=false;
  runninglampon=false;
  lowalerton=false;
  //the following lines set the new state of the system if conditions are met
  if (waterok==false) sysstate=10; //state 10 if the water is low
  if ((runswitch==true) && (waterok==true)) sysstate=100; //if the switch is on and the water is ok, state 100
  updateoutputs();
}

if (sysstate==10){
  scaninputs();
  servoangle=0;
  pumpon=false;
  runninglampon=false;
  lowalerton=true;
  delay(1000);
  delay(1000);
  //the following lines set the new state of the system if conditions are met
  if (waterok==false) sysstate=20; //state 20 if the water is low
  if (waterok==true) sysstate=0; //state 0 if the water is ok
  updateoutputs();
}


//update the position of the servo each loop through the program
//it generates 20 pulses each separated by a 20msec delay
//this positions the servo
x=0;
do
{
  if (servoangle==0) servodelay=1500;
  digitalWrite(servo,LOW);
  digitalWrite(servo,HIGH);
  delayMicroseconds(servodelay);
  digitalWrite(servo,LOW);
  delay(20);          // wait for sensors to stabilize
  x++; // check the sensors
} while (x < 20);
x=0;
}

void scaninputs(){
runswitch=!digitalRead(runsw); //if the runsw is on, runswitch if true
waterok=!digitalRead(waterlevel); //if the water level is ok waterok is true
return;
}

void updateoutputs(){
//the following six lines turn on the pump, lowalert and runninglamp
//based on the value set during the various states
digitalWrite(pump,pumpon);
// if (!pumpon) digitalWrite(pump,LOW);

digitalWrite(lowalertlamp,lowalerton);
// if (!lowalerton) digitalWrite(lowalertlamp,LOW);

digitalWrite(runninglamp,runninglampon);
// if (!runninglampon) digitalWrite(runninglamp,LOW); 
return; 
}
