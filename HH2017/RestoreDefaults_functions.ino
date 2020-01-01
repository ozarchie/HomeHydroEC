/*
 RESTORE DEFAULTS
 */
void RestoreDefaults() {

  RestoreRTC();
  DefaultRelays();
  DefaultpH1();
  DefaultpH2();
  DefaultRH();
  DefaultTemp();
  DefaultTDS1();
  DefaultTDS2();
  DefaultLight();
  DefaultLightV();  
  DefaultPumps();
  
  saveE2PValues();

//  setTime(tmp_hour,tmp_minute,tmp_second,tmp_day,tmp_month,tmp_year);
  setSyncProvider(RTC.get);

}
void DefaultRelays(void) {
  int i;
  //Turn all relays off and set to manual mode by default
  //Relay_States
  for (i = 1; i < MAXRELAYS+1; i++) {
    turnRelay(i, 0);
  }
  for (i = 0; i < MAXRELAYS; i++) {
    Relay_State[i] = 0;
    Relay_isAuto[i] = 0;
  }
}

void DefaultpH1(void) {
  pH1RawValue = 0;
  pH1Value = 0;
  strcpy(pH1_Status,"OK");
  pH1Value_Low = 5.60;
  pH1Value_High = 6.10;
}

void DefaultpH2(void) {
  pH2RawValue = 0;
  pH2Value = 0;
  strcpy(pH2_Status,"OK");
  pH2Value_Low = 5.60;
  pH2Value_High = 6.10;
}

void DefaultRH(void) {
  RHRawValue = 0;
  RHValue = 0;
  strcpy(RH_Status,"OK");
  RHValue_Low = 30.00;
  RHValue_High = 80.00;
  Humidifier_ON = 40.00;
  Humidifier_OFF = 60.00;
  Dehumidifier_ON = 80.00;
  Dehumidifier_OFF = 60.00;
}

void DefaultTemp(void) {
  TempRawValue = 0;
  TempValue = 0;
  strcpy(Temp_Status,"OK");
  TempValue_Low = 21.00;
  TempValue_High = 29.00;
  Heater_ON = 21.00;
  Heater_OFF = 25.00;
  AC_ON = 29.00;
  AC_OFF = 25.00;
}
  
void DefaultTDS1(void) {
  TDS1RawValue = 0;
  TDS1Value = 0;
  strcpy(TDS1_Status,"OK");
  TDS1Value_Low = 500.0;
  TDS1Value_High = 800.0;
  NutePump1_ON = 500.0;
  NutePump1_OFF = 800.0;
  MixPump1_Enabled = false;
}
  
void DefaultTDS2(void) {
  TDS2RawValue = 0;
  TDS2Value = 0;
  strcpy(TDS2_Status,"OK");
  TDS2Value_Low = 500.0;
  TDS2Value_High = 800.0;
  NutePump2_ON = 500.0;
  NutePump2_OFF = 800.0;
  MixPump2_Enabled = false;
}

void DefaultCO2(void) {
  CO2RawValue = 0;
  CO2Value = 0;
  strcpy(CO2_Status,"OK");
  CO2Value_Low = 60.00;
  CO2Value_High= 75.00;
  CO2_ON = 40.00;
  CO2_OFF = 60.00 ;
  CO2_Enabled = false;
}

void DefaultLight(void) {
  //Committed Light Settings
  Light_ON_hour = 18;
  Light_ON_min = 0;
  Light_OFF_hour = 6;
  Light_OFF_min = 0;
}

void DefaultLightV(void) {
  LightRawValue = 0;
  LightValue = 0;
  strcpy(Light_Status,"OK");
  LightValue_Low = 20.00;
  LightValue_High = 95.00;
}

void DefaultPumps(void) {
  //Committed Pump Schedule Settings
  Pump_start_hour = 7;
  Pump_start_minute = 15;
  Pump_start_isAM = true;
  Pump_every_hours = 2;
  Pump_every_mins = 0;
  Pump_for = 15; //In Minutes
  Pump_repeat = 6; //Number of times per day

  FillPumpTimesArrays(Pump_start_hour, Pump_start_minute, Pump_every_hours, Pump_every_mins, Pump_for, Pump_repeat);
}

void RestoreRTC(void) {
  rtc.read(tm);
  //Set-Time  setTime(hr,min,sec,day,month,yr);
  setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tmYearToCalendar(tm.Year));
  setSyncProvider(rtc.get);
}


