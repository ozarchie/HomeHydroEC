#include <TimeLib.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DallasTemperature.h>

//*********** PPM Configuration Parameters ******************************//

// Hana      [USA]        PPMconversion:  0.5
// Eutech    [EU]         PPMconversion:  0.64
// Tranchen  [Australia]  PPMconversion:  0.7
#define PPMUS  0.5
#define PPMEU  0.64
#define PPMAu  0.7
#define PPM PPMAu               // <<<--- Change for your region

float PPMConst = PPM * 1000;    // Convert EC to ppm - Au

//************ EC Configuration Parameters *******************************//

float CalibrationKCl = 1.41;      // EC value of KCL Calibration solution (in mS/cm)
                                  // 0.01 D KCl solution @ 25°C
float CalibrationNaCl = 1014.9;   // EC value of NaCl Calibration solution (in uS/cm)
                                  // 0.05% NaCl solution @ 25°C
                                
float TemperatureCoef = 0.019;  // Depends on chemical composition of solution
                                // See below 0.019 = 1.9%/°C

float Rs = 470;                     // Current Sensing Resistor
float Ra = 25;                      // Estimated Source/Sink Resistance of powering Pins

float Vin = 5;                    // Power supply value

// Compensation for temperature
// ----------------------------
// conductivity = (Kcell / R) / (1 + (TemperatureCoef/100) * (T−25))


// Determination of the temperature coefficient (θ)
// ------------------------------------------------
//  By measuring the conductivity of a sample at temperature T1
//   close to Tref (25°C) and another temperature T2
//   coefficient by using the following equation:
//
//  θ = (κT2 - κT1) * 100 / (T2 - T1) * κT1
//   κT2 = Conductivity at T2
//   κT1 = Conductivity at T1
//  T2 should be approximately 10°C different from T1
//
// The temperature coefficients of the following electrolytes generally fall
// into the ranges shown below:
//  Acids: 1.0 - 1.6%/°C
//  Bases: 1.8 - 2.2%/°C
//  Salts: 2.2 - 3.0%/°C
//  Drinking water: 2.0%/°C
//  Ultrapure water: 5.2%/°C

//************ EC Probe Configuration Parameters **************************//

// Typical Conductivity values at 25°C
// -----------------------------------
//  Pure water            0.055 µS/cm
//  Deionised water       1 µS/cm
//  Rainwater             50 µS/cm
//  Drinking water        500 µS/cm
//  Industrial wastewater 5 mS/cm
//  Seawater              50 mS/cm
//  1 mol/l NaCl          85 mS/cm
//  1 mol/l HCl           332 mS/cm


// Calculate K from measurements of US style AC connector
// ------------------------------------------------------
// Area of prongs A = 2 * ((l * w) + (l * t)) // Side faces (two)
//                      + w * t               //  + End
//                      - 2 * (pi * r^2)      //  - Holes
//                      + (2 * pi * r) * t    //  + Internal annulus
//  where
//  l = length of prongs
//  w = width of prongs
//  t = thickness of prongs
//  r = radius of holes in prongs
//
// cell constant = Kcell = s/A
// ---------------------------
//  where
//  s = separation in cm between the electrodes (prongs)
//  A = area in cm2 of the electrodes (prongs)

// Input AC prong measurements in cm
// ---------------------------------
#define l 1.7
#define w .62
#define t .14
#define s 1.16
#define r .25
#define pi 3.17

// Calculated cell constant
// ------------------------
// One Prong Area = 2 * (Faces + sides) + end  - 2 * Circular hole + Internal annulus of hole
// float Ap = (2 * ((l * w) + (l * t))) + w * t - (2 * (pi * r * r)) + ((2 * pi * r) * t) ;
// float Kc = (s / (2 * Ap));          // Two prongs
// float K = Kc;                       // Calculated cell constant

// However, practice suggests that the other face and sides on each prong only contribute to 'fringe' effects.
// so ...
float  Ap = (l * w) + (l * t) - pi * r * r;     // Surface area of one prong
float  Kc = (s / (2 * Ap));                     // Two prongs
float  K = Kc;

//************ EC Probe Related ******************************************//

int ECAin=A0;                   // Probe A Sense Pin
int ECBin=A1;                   // Probe B Sense Pin 
int ECA=6;                      // Probe A Drive Pin
int ECB=7;                      // Probe B Drive Pin
int Polarity = 0;               // Polarity for this reading

float EC = 0;                     // EC - Temerature Compensated
float ECu = 0 ;                   // Uncompensated
int ppm = 0;                      // parts per million
char Units = 'm';                 // mS or uS

#define typeEC 0                // EC - Temerature Compensated
#define typeECu 1               // EC - UnCompensated
#define typePPM 2               // ppm - Temerature Compensated
#define typeRc 3                // Raw solution resistance - uncompensated

float ECVolts;                  // Sensing variables
float RsVolts;
float Vdrop = 0;
float Rc = 0;
float Ic = 0;

// The conductivity of a solution
// ------------------------------
// EC = K * G
// EC = K / R
// EC = conductivity (S/cm)
// G  = conductance (S), where G = 1/R
// K  = cell constant (cm-1)

// Frequency of measurements
// -------------------------
//  Measuring frequencies of a typical conductivity meter
//  94 Hz     in 4.000 µS and 40.00 µS ranges
//  46.9 Hz  in 400.0 mS and 2.000 S ranges
#define ECDelay 50              // 25 Hz

// Analog reference options
// ------------------------
// DEFAULT: the default analog reference of 5 volts (on 5V Arduino boards) or 3.3 volts (on 3.3V Arduino boards)
// INTERNAL: an built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328 and 2.56 volts on the ATmega8 (not available on the Arduino Mega)
// INTERNAL1V1: a built-in 1.1V reference (Arduino Mega only)
// INTERNAL2V56: a built-in 2.56V reference (Arduino Mega only)
// EXTERNAL: the voltage applied to the AREF pin (0 to 5V only) is used as the reference.

#define AVolts Vin/1024           // = 5.00/1024

//************ Temp Probe Related *********************************************//
#define ONE_WIRE_BUS 10          // Data wire For Temp Probe is plugged into pin 10 on the Arduino
const int TempProbePositive=8;  // Temp Probe power connected to pin 9
const int TempProbeNegative=9;   // Temp Probe Negative connected to pin 8

OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.

float TemperatureStart;
float TemperatureFinish;
 
int i, j = 1;
float temp;

//************ LCD Display Related *********************************************//
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

time_t tod;
void setup()
{
  Serial.begin ( 57600 );
  analogReference(DEFAULT);     // Default 5V reference

  //lcd.setBacklightPin ( BACKLIGHT_PIN, POSITIVE );
  //lcd.setBacklight ( HIGH );
  lcd.backlight();
  lcd.begin(20,4);               // initialize the lcd 
  lcd.home ();                   // go home
  lcd.print("EC Sensor V1"); 
  lcd.setCursor ( 0, 3 );
  lcd.print("Calculated Kc: ");
  lcd.println(Kc);
    
  Serial.println("EC Sensor V1.1");
  Serial.print("Calculated Kc: ");
  Serial.println(K); 
      
  pinMode(TempProbeNegative , OUTPUT ); //seting ground pin as output for tmp probe
  digitalWrite(TempProbeNegative , LOW );//Seting it to ground so it can sink current
  pinMode(TempProbePositive , OUTPUT );//ditto but for positive
  digitalWrite(TempProbePositive , HIGH );
  delay(100);// gives sensor time to settle
  sensors.begin();
  delay(100);
  pinMode(ECAin,INPUT);         // Always inputs
  pinMode(ECBin,INPUT);
}

void loop()
{
  readTemp(0);                  // Read temperature
//  calibrateK();               // Calibrate K
  tod = now();
  EC=readECProbe(typeEC);

  if (EC < 1) {                 // Set display units
    EC *= 1000;
    ECu *= 1000;
    Units = 'u';
  }
  else if (EC < 1000) {
    EC *= 1000;
    ECu *= 1000;
    Units = 'm';    
  }
  else if (EC > 1000) {
    EC /= 1000;
    ECu /= 1000;
    Units = ' ';    
  }
  
  lcd.clear();
  lcd.setCursor ( 0, 0 );        // go to the top line
  lcd.print ("EC");
  lcd.setCursor ( 10, 0 );
  lcd.print(EC);
  lcd.print(Units);
  lcd.print("S");
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("EC (cal)");
  lcd.setCursor ( 10, 1 );
  lcd.print(CalibrationNaCl);
  lcd.print("uS");
  lcd.setCursor ( 0, 2 );        // go to the next line 
  lcd.print("Kcell");
  lcd.setCursor ( 10, 2 );
  lcd.print(K);
  lcd.setCursor ( 0, 3 );        // go to the next line    
  lcd.print("Ts ");
  lcd.print(TemperatureStart);
  lcd.setCursor ( 10, 3 ); 
  lcd.print("Tf ");
  lcd.print(TemperatureFinish);
    
  if ((TemperatureStart-TemperatureFinish) <= .2){
//    Serial.print("Temperature stable to .2C - ");
//    Serial.print("Ts ");
    Serial.print(j);
    Serial.print(", ");
    Serial.print(tod);
    Serial.print(", ");
    Serial.print(K);
    Serial.print(", ");
    Serial.print(TemperatureFinish);
    Serial.print(", ");
    Serial.print(ECu);
    Serial.print(", ");
    Serial.print(EC);
    Serial.print(", ");
    Serial.print(Rc);
    Serial.print(", ");             // Debug print
    Serial.println(Ic * 1000);

           
//    Serial.print("Calibration Solution EC: ");
//    Serial.print(CalibrationNaCl);
//    Serial.print("mS");

  }
  else{
    Serial.println("  Error - Wait For Temperature To settle");
    Serial.print("Ts ");
    Serial.print(TemperatureStart);
    Serial.print(", Tf ");
    Serial.print(TemperatureFinish);
  }
  delay(10000);
  j += 1;
}


float readTemp(int tsensor) {
  float x;
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp=sensors.getTempCByIndex(tsensor);
  return(temp);
}

// Read data from EC probe
// -----------------------
// type typeEC  0   T Compensated Electrical conductivity
// type typeECu 1   T Compensated Electrical conductivity
// type typePPM 2   T Compensated ppm
// type typeRc  3   Uncompensated Electrical Resistance

float readECProbe(int type) {

  int i = 0;
  float x;
  ECVolts = 0;
  RsVolts = 0;
  Polarity = 0;
  
  TemperatureStart = readTemp(0);    // Read temperature   
  pinMode(ECA,OUTPUT);               // Set drive pins to outputs
  pinMode(ECB,OUTPUT);
  digitalWrite(ECA,LOW);            // Set PinA low
  digitalWrite(ECB,LOW);            // Set PinB low
  delay(ECDelay);                   // Wait til settled
  
// Measure voltages of probe - average 10 
  while(i < 10){    
// Alternately set polarity of sensing circuit to avoid polarizing the solution
//  and fouling the probe

    if (Polarity == 0) {
      digitalWrite(ECA,HIGH);   // Set PinA high
      digitalWrite(ECB,LOW);    // Set PinB low
    }
    else if (Polarity == 1) {
      digitalWrite(ECA,LOW);    // Set PinA low
      digitalWrite(ECB,HIGH);   // Set PinB high
    }
    delay(ECDelay);             // Wait for settling
    
    if (Polarity == 0) {               // Take alternate A->B, B->A readings
      x = analogRead(ECAin);
      ECVolts += analogRead(ECAin);    // This is not a mistake, First reading will be low
      x = analogRead(ECBin);
      RsVolts += analogRead(ECBin);      
      Polarity = 1;                     // Switch every measurement
    }
    else if (Polarity == 1) {
      x = analogRead(ECBin);
      ECVolts += analogRead(ECBin);    // This is not a mistake, First reading will be low
      x = analogRead(ECAin);
      RsVolts += analogRead(ECAin);
      Polarity = 0;                      // Switch every measurement 
    }
    i++;                            // Next measurement
    
//    Serial.print("ECVolts: ");      // Debug print
//    Serial.print((ECVolts/i)*AVolts);
//    Serial.print(", RsVolts: ");
//    Serial.print((RsVolts/i)*AVolts);
//    Serial.print(", Polarity: ");
//    Serial.println(Polarity);

  };

  digitalWrite(ECA,LOW);            // Set PinA low
  digitalWrite(ECB,LOW);            // Set PinB low
  delay(ECDelay);                   // Wait til settled
  pinMode(ECA,INPUT);               // Reset drive pins to inputs
  pinMode(ECB,INPUT);

  ECVolts *= (AVolts/10);           // Average of 10 measurements, convert to Volts
  RsVolts *= (AVolts/10);           // Average of 10 measurements, convert to Volts
  ECVolts -= RsVolts;               // Difference of volts across EC probe
  Ic = (RsVolts / (Rs + Ra));       // Calculate current through sense resistor
  Rc = ECVolts / Ic;                // Calculate EC Resistance from V=IR->R=V/I
  ECu = 1000 * (K / Rc);            // Calculate EC in mS
  TemperatureFinish = readTemp(0);  // Temperature Compensate the EC  
  EC = ECu / (1 + (TemperatureCoef * (TemperatureFinish - 25.0)));

//  if (Polarity == 0) Polarity = 1;  // Switch every new measurement set
//  else if (Polarity > 0) Polarity = 0;

  if (type == typeEC) return(EC);             // Return EC
  if (type == typeECu) return(ECu);           // Return EC - uncompensated
  if (type == typePPM) return(EC * PPMConst); // Return ppm
  if (type == typeRc) return(Rc);             // Return Rc
  else return(0);
  
}

float calibrateK(void) {  
  float Rec;                            // Place to store measured Rc
  Polarity = 0;                         // Reset probe polarity
  TemperatureStart = readTemp(0);       // Read temperature 
  Rec  = readECProbe(typeRc);           // Read Rc A->B
  Rec += readECProbe(typeRc);           // Read Rc B->A
  Rec /= 2;                             // Average two readings
  
// Temperature Compensate the EC of solution
  TemperatureFinish = readTemp(0);     
  EC=CalibrationNaCl*(1+(TemperatureCoef*(TemperatureFinish-25.0)));
// EC = K/R
// K = EC/R

  K = 1000*( EC / Rec) ;                // Calculate K
  return(K);
}

void displayEC(float ec) {
  EC=readECProbe(typeEC);
}

void displayTemp(float ts, float tf) {
  
}




