// include the library code: 
#include <LiquidCrystal.h> //library for LCD 

// initialize the library with the numbers of the interface pins 
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); 

// Measuring Voltage
#include "EmonLib.h"   // Include Emon Library
EnergyMonitor emon1;   // Create an instance

// Measuring Current Using ACS712
const int Sensor_Pin = A1; //Connect current sensor with A1 of Arduino
int sensitivity = 185; // use 100 for 20A Module and 66 for 30A Module
int offsetvoltage = 2542; 

void setup()
{
  emon1.voltage(A0,187, 1.7); // Voltage: input pin, calibration, phase_shift

  lcd.begin(20, 4); // set up the LCD's number of columns and rows:
  lcd.setCursor(0,0);
  lcd.print("  AC ENERGY METER             ");
  lcd.setCursor(0,1);
  lcd.print("  Made by ABHI    ");
  
}

void loop()
{
  //************************* Measure Voltage ******************************************
  emon1.calcVI(20,2000); // Calculate all. No.of half wavelengths (crossings), time-out
  int Voltage   = emon1.Vrms;  //extract Vrms into Variable
  
  lcd.setCursor(0,2);
  lcd.print("V = ");
  lcd.print(Voltage);
  lcd.print("V  ");

  //************************* Measure Current ******************************************
  unsigned int temp=0;
  float maxpoint = 0;
  for(int i=0;i<500;i++)
  {
    if(temp = analogRead(Sensor_Pin), temp>maxpoint)
    {
      maxpoint = temp;
    }
  }
  float ADCvalue = maxpoint; 
  double eVoltage = (ADCvalue / 1024.0) * 5000; // Gets you mV
  double Current = ((eVoltage - offsetvoltage) / sensitivity);
  double AC_Current = ( Current ) / ( sqrt(2) );

  lcd.print("I = ");
  lcd.print(AC_Current,2);
  lcd.print("A          "); //unit for the current to be measured

  //************************* Measure Power ******************************************
  int Power = (Voltage * AC_Current);
  
  lcd.setCursor(0,3);
  lcd.print("P = ");
  lcd.print(Power);
  lcd.print("W  "); //unit for the current to be measured
  
  //************************* Measure Energy ******************************************
  long milisec = millis(); // calculate time in milliseconds 
  long time=milisec/1000; // convert milliseconds to seconds 
  
  float Energy = (Power*time)/3600; //Watt-sec is again convert to Watt-Hr by dividing 1hr(3600sec) 

  lcd.print("E = ");
  lcd.print(Energy,1);
  lcd.print("Wh    "); //unit for the current to be measured
  
  delay(200);
}
