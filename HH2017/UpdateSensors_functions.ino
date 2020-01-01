/*
// UPDATE SENSOR VALUES
*/

//READ ALL SENSOR VALUES AND CONVERT FOR LCD DISPLAY
void updateSensorValues() {
//  Serial.println("Updating sensor data ... "); 
//  analogReference(DEFAULT);  //Seems more accurate.
// pH SENSORS
  // pH1
  int pH1Sum;    // pH averaging
  int j;

  digitalWrite(pH1Pwr, HIGH);          // Turn the power On
  delay(2000);          // Wait for startup
  
  analogRead(pH1Pin);  //Get ADC to switch to correct pin
  delay(50); //Wait for Pin to Change

  pH1Sum = 0;
  j = 0;
  while(j < NSAMPLES) {
    pH1Sum = pH1Sum + analogRead(pH1Pin);
    delay(10);        // Short delay
    j++;
  }

//  Serial.print("Digital reading   - ");
//  Serial.print(pH1Sum);
//  Serial.print(", ");
  pH1RawValue = pH1Sum;
//  Serial.print(pH1RawValue);
//  Serial.print(": phRef - ");
  
  analogRead(pH1Ref); //Get ADC to switch to correct pin
  delay(50); //Wait for Pin to Change

  pH1Sum = 0;
  j = 0;
  while(j < NSAMPLES) {
    pH1Sum = pH1Sum + analogRead(pH1Ref);
    delay(10);        // Short delay
    j++;
  }

//  Serial.print(pH1Sum);
//  Serial.print(", ");
//  Serial.print(pH1Sum);
//  Serial.println("");
  
//  Serial.print("Analog conversion - ");
  pH1RawValue = pH1RawValue - pH1Sum;         // Subtract reference
  pH1Value = pH1RawValue;                     // Convert to float
//  Serial.print(pH1Value, 2);
//  Serial.print(", ");
  pH1Value = (pH1Value / NSAMPLES) * ArefValue;            // Convert to mV
//  Serial.print(pH1Value, 2);
//  Serial.print(", ");  
  pH1Value = (7.00 - (pH1Value / 89.0));     //  Convert to pH (89mV/1pH)
//  Serial.println(pH1Value);
//  Serial.print("");

  if(isnan(pH1Value)){
    pH1Value = 0;        
  }
  if (pH1Value < 0) pH1Value = 0;
  if (pH1Value > 14) pH1Value = 14;
//  UIToStr(pH1_char, pH1Value, 5, 1);
//  strcat(pH1_char,"pH");
//  Serial.println(pH1_char);

  digitalWrite(pH1Pwr, LOW);          // Turn the power Off

  // pH2
/*
  float pH2Sum = 0;
  j = 0;
  analogRead(pH2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<30) {
    pH2Sum = pH2Sum + analogRead(pH2Pin);
    j++;
  }
  pH2RawValue = pH2Sum/30;

  pH2Value = (0.0178 * pH2RawValue - 1.889);
  if(isnan(pH2Value)){
    pH2Value = 0;        
  }

  UIToStr(pH2_char, pH2Value, 5, 1); 
  strcat(pH1_char,"pH");

// Temperature and Humidity

  dht.read22(DHTPIN);  //Use updated DHT library for hum, temp

  // DHT22 Temperature
  TempValue=dht.temperature;
  if(isnan(TempValue)){
    TempValue = 0;        
  }
  UIToStr(Temp_char, TempValue, 5, 2);
  strcat(Temp_char," C"); 
  
  // DHT22 Humidity
  RHValue = dht.humidity;
  if (isnan(RHValue)) {
    RHValue = 0;
  } 
  UIToStr(RH_char, RHValue, 5, 2);
  if (RHValue < 10){
    strcat(RH_char,"% ");
  } 
  else {
    strcat(RH_char,"%");
  }
*/
  
// TDS/EC

  EC = EC_read(2);
/*  
  float TDS1Sum = 0;
  j = 0;
  analogRead(TDS1Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    TDS1Sum = TDS1Sum + analogRead(TDS1Pin);
    j++;
  }
  TDS1RawValue = TDS1Sum/((j-1) * 2);
  TDS1Value = ((TDS1RawValue * 1000.0)/1024.0);
  if(isnan(TDS1Value)){
    TDS1Value = 0;        
  }
  */
  TDS1Value = EC;
  UIToStr(TDS1_char, TDS1Value, 5, 2);
  strcat(TDS1_char," ppm");    
  
  float TDS2Sum = 0;
  j = 0;
  analogRead(TDS2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    TDS2Sum = TDS2Sum + analogRead(TDS2Pin);
    j++;
  }
  TDS2RawValue = TDS2Sum/((j-1) * 2);
  TDS2Value = ((TDS2RawValue * 1000.0)/1024.0);
  if(isnan(TDS2Value)){
    TDS2Value = 0;        
  }
  UIToStr(TDS2_char, TDS2Value, 5, 2);
  strcat(TDS2_char," ppm");   
  
    
// CO2

  float CO2Sum = 0;
  j = 0;
  analogRead(CO2Pin);  //Get ADC to switch to correct pin
  delay(15); //Wait for Pin to Change

  while(j<10) {
    CO2Sum = CO2Sum + analogRead(CO2Pin);
    j++;
  }
  CO2RawValue = CO2Sum/((j-1) * 2);
  CO2Value = ((CO2RawValue * 100.0)/1024.0);
  if(isnan(CO2Value)){
    CO2Value = 0;        
  }
  UIToStr(CO2_char, CO2Value, 5, 2);
  strcat(CO2_char," ppm");   
 
  
// LIGHT

  float LightSum = 0;
  j = 0;
  analogRead(LightPin);  //Get ADC to switch to correct pin
  delay(10); //Wait for Pin to Change
  
  while(j<10) {
    LightSum = LightSum + analogRead(LightPin);
    j++;
  }
  LightRawValue = LightSum/10;
  LightValue = ((LightRawValue * 100.0) / 1024.0);
  if (isnan(LightValue)) {
    LightValue = 0;
  } 
  UIToStr(Light_char, LightValue, 5, 2);
  if (LightValue < 10){
    strcat(Light_char,"% ");
  } 
  else {
    strcat(Light_char,"%");
  }

// CHECK LIMITS  
  //pH
  if (pH1Value < pH1Value_Low) {
    strcpy(pH1_Status,"LOW");
  } 
  else if (pH1Value > pH1Value_Low && pH1Value < pH1Value_High) {
    strcpy(pH1_Status,"OK");
  } 
  else if (pH1Value > pH1Value_High) {
    strcpy(pH1_Status,"HIGH");
  }
  if (pH2Value < pH2Value_Low) {
    strcpy(pH2_Status,"LOW");
  } 
  else if (pH2Value > pH2Value_Low && pH2Value < pH2Value_High) {
    strcpy(pH2_Status,"OK");
  } 
  else if (pH2Value > pH2Value_High) {
    strcpy(pH2_Status,"HIGH");
  }
  //Temperature
  if (TempValue < TempValue_Low) {
    strcpy(Temp_Status,"LOW");
  } 
  else if (TempValue > TempValue_Low && TempValue < TempValue_High) {
    strcpy(Temp_Status,"OK");
  } 
  else if (TempValue > TempValue_High) {
    strcpy(Temp_Status,"HIGH");
  }
  //RH
  if (RHValue < RHValue_Low) {
    strcpy(RH_Status,"LOW");
  } 
  else if (RHValue > RHValue_Low && RHValue < RHValue_High) {
    strcpy(RH_Status,"OK");
  }
  else if (RHValue < RHValue_High) {
    strcpy(RH_Status,"HIGH");
  }
  //TDS1
  if (TDS1Value < TDS1Value_Low) {
    strcpy(TDS1_Status,"LOW");
  } 
  else if (TDS1Value > TDS1Value_Low && TDS1Value < TDS1Value_High) {
    strcpy(TDS1_Status,"OK");
  } 
  else if (TDS1Value < TDS1Value_High) {
    strcpy(TDS1_Status,"HIGH");
  }
  //TDS2
  if (TDS2Value < TDS2Value_Low) {
    strcpy(TDS2_Status,"LOW");
  } 
  else if (TDS2Value > TDS2Value_Low && TDS2Value < TDS2Value_High) {
    strcpy(TDS2_Status,"OK");
  } 
  else if (TDS2Value < TDS2Value_High) {
    strcpy(TDS2_Status,"HIGH");
  }
  //CO2
  if (CO2Value < CO2Value_Low) {
    strcpy(CO2_Status,"LOW");
  } 
  else if (CO2Value > CO2Value_Low && CO2Value < CO2Value_High) {
    strcpy(CO2_Status,"OK");
  } 
  else if (CO2Value > CO2Value_High){
    strcpy(CO2_Status,"HIGH");
  }
  //Light
  if (LightValue < LightValue_Low) {
    strcpy(Light_Status,"LOW");
  } 
  else if (LightValue > LightValue_Low && LightValue < LightValue_High) {
    strcpy(Light_Status,"OK");
  } 
  else if (LightValue < LightValue_High) {
    strcpy(Light_Status,"HIGH");
  }
}


