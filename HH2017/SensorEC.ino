/*
  Electrical Conductivity Probes
  */


void EC_setup()
{

  pinMode(ECAin,INPUT);         // Always inputs
  pinMode(ECBin,INPUT);
}

/*
void EC_calibrate()
{
//  readTemp(0);                  // Read temperature
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
    Serial.print("Temperature stable to .2C - ");
    Serial.print("Ts ");
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
*/

float readTemp(int tsensor) {
  float x;
  ec_temp.requestTemperatures(); // Send the command to get temperatures
  temp=ec_temp.getTempCByIndex(tsensor);
  if ((temp > MAX_TEMP) || (temp < MIN_TEMP)) temp = DEFAULT_TEMP;
  return(temp);
}


// Read data from EC probe
// -----------------------
// type typeEC  0   T Compensated Electrical conductivity
// type typeECu 1   Uncompensated Electrical conductivity
// type typePPM 2   T Compensated ppm
// type typeRc  3   Uncompensated Electrical Resistance

float EC_read(int type) {

  int i = 0;
  float x;
  ECVolts = 0;
  RsVolts = 0;
  Polarity = 0;
  
  ECTemperatureStart = readTemp(0);  // Read temperature   
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
  ECTemperatureFinish = readTemp(0);  // Temperature Compensate the EC  
  EC = ECu / (1 + (TemperatureCoef * (ECTemperatureFinish - 25.0)));

//  if (Polarity == 0) Polarity = 1;  // Switch every new measurement set
//  else if (Polarity > 0) Polarity = 0;

  // Check for unreasonable values
  if ((EC > MAX_EC) || (EC < MIN_EC)) EC = DEFAULT_EC;    // Set a default for open connections

  if (type == typeEC) return(EC);             // Return EC
  if (type == typeECu) return(ECu);           // Return EC - uncompensated
  if (type == typePPM) return(EC * PPMConst); // Return ppm
  if (type == typeRc) return(Rc);             // Return Rc
  else return(DEFAULT_EC);
  
}

float EC_calibrateK(void) {  
  float Rec;                            // Place to store measured Rc
  Polarity = 0;                         // Reset probe polarity
  ECTemperatureStart = readTemp(0);       // Read temperature 
  Rec  = EC_read(typeRc);           // Read Rc A->B
  Rec += EC_read(typeRc);           // Read Rc B->A
  Rec /= 2;                             // Average two readings
  
// Temperature Compensate the EC of solution
  ECTemperatureFinish = readTemp(0);     
  EC=CalibrationNaCl*(1+(TemperatureCoef*(ECTemperatureFinish-25.0)));
// EC = K/R
// K = EC/R

  K = 1000*( EC / Rec) ;                // Calculate K
  return(K);
}







