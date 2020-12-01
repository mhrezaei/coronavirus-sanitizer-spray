/*********
  Author: Mohmammad Hadi Rezaei
  Email: hadi@yasna.team
  Website: https://rabbitfox.com
  Project: Corona virus snitizer spary
  Description: Corona virus sanitizer spary made with ESP8266 
*********/

#include <Ultrasonic.h>

// The Trigger pin connected to GPIO 5
// The Echo pin connected to GPIO 16
Ultrasonic ultrasonic(5, 16);
int distance;


bool distanceFlag = false, timeFlag = false;
unsigned long currentTime = 0, oldTime = 0;

// Spray LED connected to GPIO 12
// Health check LED connected to GPIO 15
// Relay IN pin connected to GPIO 12
int sprayLed = 12, healthLed = 15, relayPin = 14, distanceLimit = 10, sprayTime = 1000;


void setup() {
  Serial.begin(115200);

  pinMode (sprayLed, OUTPUT);
  pinMode (healthLed, OUTPUT);
  pinMode (relayPin, OUTPUT);
  
}

// Run sanitizer spary for limited time
void sanitizerSpray()
{
  Serial.println("sanitizerSpray");
  digitalWrite(sprayLed, HIGH);
  digitalWrite(relayPin, HIGH);
  delay(sprayTime);
  sanitizerSprayShutdown();
}

// Shutdown sanitizer spray
void sanitizerSprayShutdown()
{
  Serial.println("sanitizerSprayShutdown");
  digitalWrite(sprayLed, LOW);
  digitalWrite(relayPin, LOW);
  delay(200);

}

// Check last time sanitizer spray run
void checkTime()
{
  currentTime = millis();
  if(currentTime - oldTime >= 4000)
  {
    oldTime = millis();
    timeFlag = true;
  }
  else
  {
    timeFlag = false;
  }
  Serial.print("Time Flag ");
  Serial.print(timeFlag);
  Serial.print(" - currentTime: ");
  Serial.print(currentTime);
  Serial.print(" - oldTime: ");
  Serial.println(oldTime);
}

// check the hand distance with sanitizer spray
void checkDistance()
{
  distance = ultrasonic.read();
  if(distance <= distanceLimit)
  {
    distanceFlag = true;
    checkTime();
  }
  else
  {
    distanceFlag = false;
  }
  Serial.print(distance);
  Serial.print(" CM - distanceFlag: ");
  Serial.println(distanceFlag);
}

// display the led process
void processAlarm(boolean state = false)
{
  if(state == true)
  {
    digitalWrite(healthLed, HIGH);
  }
  else
  {
    digitalWrite(healthLed, LOW);
    delay(500);
  }
}

// check spray process
void sprayProcess()
{
  if (distanceFlag == true && timeFlag == true)
  {
    sanitizerSpray();
  }
  else
  {
    sanitizerSprayShutdown();
  }
}

// run sanitizer spary process
void sanitizerProcess()
{
  processAlarm(true);
  checkDistance();
  sprayProcess();
  processAlarm();
}

void loop() {
  sanitizerProcess();
}
