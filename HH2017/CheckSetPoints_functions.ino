/*
  CHECK SETPOINTS 
 */
 
 void CheckSetPoints() {
   int j = 0;
  
//   j += CheckpH(1);
   j += CheckTDS(1);
   j += CheckHumidity(1);
   j += CheckTemperature(1);
   if (j > 0) {
     Serial.println("Changed relay state");
     sendRelayData();
   } 
 }

int CheckpH(int probe) {
  if ((SetRelay(pHADJUST, pH1Value, pH1Value_Low, pH1Value_High)) > 0) {
    Serial.print("CP pH ");
    return (1);
  }
  else return (0);
}
  
int CheckTDS(int probe) {
  if ((SetRelay(NUTRIENT, TDS1Value, TDS1Value_Low, TDS1Value_High)) > 0) {
    Serial.print("CP TDS ");
    return (1);
  }
  else return (0);
}

int CheckHumidity(int probe) {
  if ((SetRelay(HUMIDITY, RHValue, RHValue_Low, RHValue_High)) > 0) {
    Serial.print("CP Hum ");
    return (1);
  }
  else return (0);
}

int CheckTemperature(int probe) {
  if ((SetRelay(SSRHEAT, TempValue, TempValue_Low, TempValue_High)) > 0) {
    Serial.print("CP Temp ");
    return (1);
  }
  else return (0);
}


