//*********** PPM Configuration Parameters ******************************//
// Hana      [USA]        PPMconversion:  0.5
// Eutech    [EU]         PPMconversion:  0.64
// Tranchen  [Australia]  PPMconversion:  0.7

#define PPMUS  0.5
#define PPMEU  0.64
#define PPMAu  0.7
#define PPM PPMAu               // <<<--- Change for your region

#define MAX_EC 10
#define MIN_EC 0
#define DEFAULT_EC 2

// Temperature defaults in centigrade
#define MAX_TEMP 60
#define MIN_TEMP -30
#define DEFAULT_TEMP 25

float PPMConst = PPM * 1000;    // Convert EC to ppm - Au

//************ EC Configuration Parameters *******************************//

float CalibrationKCl = 1.41;      // EC value of KCL Calibration solution (in mS/cm)
                                  // 0.01 D KCl solution @ 25°C
float CalibrationNaCl = 1014.9;   // EC value of NaCl Calibration solution (in uS/cm)
                                  // 0.05% NaCl solution @ 25°C
float TemperatureCoef = 0.019;    // Depends on chemical composition of solution
                                  // See below 0.019 = 1.9%/°C

float Rs = 470;                     // Current Sensing Resistor
float Ra = 25;                      // Estimated Source/Sink Resistance of powering Pins

float Vin = 5;                      // Power supply value

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
#define plength 1.7
#define pwidth 0.62
#define pthick 0.14
#define psepn 1.16
#define pradius 0.25
#define pi 3.17

// Calculated cell constant
// ------------------------
// One Prong Area = 2 * (Faces + sides) + end  - 2 * Circular hole + Internal annulus of hole
// float Ap = (2 * ((l * w) + (l * t))) + w * t - (2 * (pi * r * r)) + ((2 * pi * r) * t) ;
// float Kc = (s / (2 * Ap));          // Two prongs
// float K = Kc;                       // Calculated cell constant

// However, practice suggests that the other face and sides on each prong only contribute to 'fringe' effects.
// so ...
float  Ap = (plength * pwidth) + (plength * pthick) - pi * pradius * pradius;     // Surface area of one prong
float  Kc = (psepn / (2 * Ap));                                                   // Two prongs
float  K = Kc;

//************ EC Probe Related ******************************************//
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

float ECTemperatureStart = 23.0;
float ECTemperatureFinish = 23.0;
 
float temp;




