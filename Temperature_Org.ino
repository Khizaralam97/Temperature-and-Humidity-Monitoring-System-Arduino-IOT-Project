#define BLYNK_PRINT Serial // Enables Serial Monitor
#define BLYNK_TEMPLATE_ID "TMPL4276vLRyT"
#define BLYNK_TEMPLATE_NAME "ArduinoTest"
#define BLYNK_AUTH_TOKEN "hzzlW8uQKl0DkBh6LE8nuMDq7wKeKdYn"

#include <Bridge.h>
#include <BlynkSimpleYun.h>

char auth[] = "hzzlW8uQKl0DkBh6LE8nuMDq7wKeKdYn";

 
int Lmax = 0;
int Lmin = 1023; 
int L_value;
void setup()
{
  pinMode(A0, INPUT); // LM35 sensor
  pinMode(A1, INPUT); // Light sensor
  pinMode(A2, INPUT); // Humidity sensor
  pinMode(13,OUTPUT); //  Pin13 LED
  Bridge.begin();
  Blynk.begin(auth);
  Serial.begin(9600);
  
   
  int L; // Light Sensor calibratiion; only one time at the beginning
  Serial.println("Beginning Light Sensor calibration...");
  while (millis()<5000) {
    L = analogRead(A1);
    if (L>Lmax) Lmax = L;
    if (L<Lmin) Lmin = L;
    delay(10);
  }
  Serial.println("Calibration DONE!!");
  Serial.println(Lmin);
  Serial.println(Lmax);
  delay(2000);
 
}

void loop()
{
Blynk.run();
  
  delay(1000);
  float temperature = getTemperature();
  Blynk.virtualWrite(V1, temperature);  // Send temperature to Blynk app
  Serial.println(temperature);

  if(temperature > 30){
    Blynk.logEvent("temp_alert","Temp above 30 degree");
  }

  
  delay(1000);
  float lightvalue= analogRead(A1);
  Blynk.virtualWrite(V2, lightvalue); // Send LightValue to Blynk app
  Serial.println(lightvalue);

  delay(1000);
  float H_value = analogRead(A2);
  Blynk.virtualWrite(V4, H_value); // Send Humidity to Blynk app
  Serial.println(H_value);

  delay(1000);
  char* daynight = getdaynight();
  Blynk.virtualWrite(V3, daynight); // Send Day/Night status to Blynk app
  Serial.println(daynight);
  delay(500);

}


float getTemperature()
{
  int sensorValue = analogRead(A0);
  float voltage = (sensorValue / 1024.0) * 5.0;
  float temperatureC = ((voltage - 0.5) * 100.0)*-1;
  return temperatureC;
}


const char* getdaynight() {
  int a = analogRead(A1);

  if (a < 50) {
    return "Night";
  } else {
    return "Day";
  }
}


BLYNK_WRITE(V0){
int value = param.asInt();
if (value==1)
  digitalWrite(13,HIGH);
else
  digitalWrite(13,LOW);
}

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V0);
}