/**
 * 
 */
#include <OneWire.h>
#include <DallasTemperature.h>

const int AnodeUnits = 11;   // common anode pin
const int AnodeTens = 12;   // common anode pin

/**
 Schematic view of the 7 segment display together with the connected pins on the Arduino board.
      -  A=8
F=10 | | B=9
      -  G=4
E=3  | | C=6
      -  D=2
*/
const int LedA = 8;  // top
const int LedB = 9;   // top right  
const int LedC = 5;   // bottom right
const int LedD = 2;   // bottom
const int LedE = 3;   // bottom left
const int LedF = 10;   // top left
const int LedG = 4;   // middle

const int LedDP = 7;  // decimal point

const int PinArray[] = {LedA, LedB, LedC, LedD, LedE, LedF, LedG}; // all pin connections saved in an array for easier processing

/**
 * The bytes are created with the following pattern:
 * B0|LedG|LedF|LedE|LedD|LedC|LedB|LedA
 * B0  0    1    1    1    1    1    1     pattern for 0
 */
byte Segemtpattern[10] = {  
  B00111111, // pattern for 0
  B00000110, // pattern for 1
  B01011011, // pattern for 2
  B01001111, // pattern for 3
  B01100110, // pattern for 4
  B01101101, // pattern for 5
  B01111101, // pattern for 6
  B00000111, // pattern for 7
  B01111111, // pattern for 8
  B01101111};// pattern for 9

const int DELAY = 6;  // the time how long a Led lights

/**
 * Setup for the analog temperature sensor TMP36
 */
 const int TempSensorPin = A0; // named constant for the pin the temperature sensor (TMP36) is connected to


/**
 * Setup for the digital temperature sensor DS18B20
 */
bool useDigitalTempSensor = true;
#define ONE_WIRE_BUS 6 //data port Dallas 18B20 connected to digital pin 13
int res = 11; //temperature resolution

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); // pass the OneWire instance to DallasTemperature
DeviceAddress temperatureDeviceAddress;
unsigned long lastTempRequest = 22; // Initialize with a default room temperature
int  delayInMillis = 0;
float temperaturDigital = 0.0;

/**
 * Setup heating pad / mosfet
 */
 const int heatpad = 7; // digital output for the mosfet

 
void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps
  
  pinMode(AnodeUnits, OUTPUT);  // common anode is obviously an output
  pinMode(AnodeTens, OUTPUT);  // common anode is obviously an output
  
  pinMode(LedA, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(LedC, OUTPUT);
  pinMode(LedD, OUTPUT);
  pinMode(LedE, OUTPUT);
  pinMode(LedF, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedDP, OUTPUT);
  
  pinMode(TempSensorPin, INPUT);
  
  sensors.begin();
  sensors.setResolution(temperatureDeviceAddress, res);
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();

  if (!sensors.getAddress(temperatureDeviceAddress, 0)) {
    Serial.println(F("Error connecting to digital temperature sensor."));
  } else {
    sensors.requestTemperatures();
    temperaturDigital = sensors.getTempCByIndex(0);
  }
}

void loop(){
  float temp = 0;
  if (useDigitalTempSensor) {
    temp = readTemperatureDigital();
  } else {
    temp = readTemperature();  
  }
  
  Serial.println(temp);
  
  manageHeatingPad(temp);
  
  showTemperature((int) temp);
}

/**
 * Method for showing the current temperature.
 */
void showTemperature(int temperature){
  byte units, tens;
  tens = int(temperature / 10); // calculate the digit the tens
  units = temperature - tens * 10; // calculate digit of the units

  //activate the units pin
  digitalWrite(AnodeUnits, HIGH);
  digitalWrite(AnodeTens, LOW);
  activate7segment(units);

  //activate the tens pin
  digitalWrite(AnodeUnits, LOW);
  digitalWrite(AnodeTens, HIGH);
  activate7segment(tens);
}

/**
 * Method for activating the LEDs of one of the 7 segment displays.
 */
void activate7segment(int num){
  for(int j = 0; j <=6; j++) {
    digitalWrite(PinArray[j], bitRead(Segemtpattern[num], j) == 1 ? LOW : HIGH);
  }
  delay(DELAY);
}

/**
 * Method for reading the trempreture with the analog sensor TMP36.
 */
int readTemperature() {
   // read the value on AnalogIn pin 0 and store it in a variable
  int sensorVal = analogRead(TempSensorPin); //the 10-bit sensor value out the serial port

  // convert the ADC reading to voltage
  float voltage = (sensorVal / 1024.0) * 5.0;

  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((voltage - 500 mV) times 100)
  int temperature = (voltage - .5) * 100;
  return temperature;
}


/**
 * Method for reading the trempreture with the digital sensor DS18B20.
 */
float readTemperatureDigital() {

//  sensors.setResolution(temperatureDeviceAddress, res);
//  sensors.requestTemperatures();
//  float temperaturDigital = sensors.getTempCByIndex(0);

  
  if (millis() - lastTempRequest >= delayInMillis) {
    sensors.setResolution(temperatureDeviceAddress, res);
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    Serial.println(temp);
    delayInMillis = 2500 / (1 << (12 - res));
    lastTempRequest = millis();
 
    //Filter
    temperaturDigital = temperaturDigital - 0.05 * (temperaturDigital - temp) ;
    //Output
    Serial.print(temperaturDigital); Serial.println (F(" degree Celsius"));
  }
  return temperaturDigital;
}

/**
 * Setting the Pwm value for the mosfet/heating pad.
 * The PWM values could be between 0 and 255.
 */
void manageHeatingPad(float temperature ) {
  
  if ((temperature > 14) and (temperature < 18)){  // Start heating, when the water's temperature is at least 15°Celsius
    analogWrite(heatpad, 190);
    Serial.println("heating level 6");
  }
  else if ((temperature >= 18) and (temperature < 21)){
    analogWrite(heatpad, 185);
    Serial.println("heating level 5");
  }
  else if ((temperature >= 21) and (temperature < 22)){
    analogWrite(heatpad, 180);
    Serial.println("heating level 4");
  }
  else if ((temperature >= 22) and (temperature < 23)){
    analogWrite(heatpad, 175);
    Serial.println("heating level 3");
  }
//  else if ((temperature >= 23) and (temperature < 24)){
//    analogWrite(heatpad, 140);
//    Serial.println("heating level 2");
//  }
//    else if ((temperature >= 24) and (temperature < 26)){
//    analogWrite(heatpad, 100);
//    Serial.println("heating level 1");
//  }
  else{
    analogWrite(heatpad, 0); // shut down heating pad
  }
}
