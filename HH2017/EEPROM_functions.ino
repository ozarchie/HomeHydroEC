/*
 EEPROM READ/WRITE FUNCTIONS
 */
void eepromClear() {  
  //Clear the EEPROM
  for (int i = 0; i < 4096; i++) {
    EEPROM.write(i, 0);
  }
  delay(100);
}

int eepromReadInt(int address){
  int value = 0x0000;
  value = value | (EEPROM.read(address) << 8);
  value = value | EEPROM.read(address+1);
  return value;
}

void eepromWriteInt(int address, int value){
  EEPROM.write(address, (value >> 8) & 0xFF );
  EEPROM.write(address+1, value & 0xFF);
}

float eepromReadFloat(int address){
  union u_tag {
    byte b[4];
    float fval;
  } 
  u;   
  u.b[0] = EEPROM.read(address);
  u.b[1] = EEPROM.read(address+1);
  u.b[2] = EEPROM.read(address+2);
  u.b[3] = EEPROM.read(address+3);
  return u.fval;
}

void eepromWriteFloat(int address, float value){
  union u_tag {
    byte b[4];
    float fval;
  } 
  u;
  u.fval=value;

  EEPROM.write(address  , u.b[0]);
  EEPROM.write(address+1, u.b[1]);
  EEPROM.write(address+2, u.b[2]);
  EEPROM.write(address+3, u.b[3]);
}

//          EEPROM.write(E2P_CRC8_RelayState, crc8((uint8_t*)Relay_State, MAXRELAYS*2));
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
uint8_t crc8(const uint8_t *addr, uint8_t len)
{
	uint8_t crc;
        crc = 0;
	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

uint8_t e2p_crc8(int addr, int len)
{
	uint8_t crc;
        crc = 0;
	while (len--) {
		uint8_t inbyte = EEPROM.read(addr++);
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}


int loadE2PValues(void) {
int i;
int ec = 0;

  //EEPROM Relay Saved Values
  if(EEPROM.read(E2P_CRC8_RelayState) == e2p_crc8(E2P_Relay_State, MAXRELAYS*2)) {
    for(i = 0; i < MAXRELAYS; i++){
      Relay_State[i] =  EEPROM.read(i+E2P_Relay_State);
//      Serial.print(Relay_State[i]);
//      Serial.print(",");
      Relay_isAuto[i] = EEPROM.read(i+E2P_Relay_isAuto);
//      Serial.print(Relay_isAuto[i]);
//      Serial.println(".");
    }
  }
  else {
    DefaultRelays();
    ec+=1;
  }
  
  // If Relay is not set for automatic mode, then resume last saved state 
  // (ie. Set relay as it was saved before a power failure?)
  for(i = 0; i < MAXRELAYS; i++){
      if (Relay_isAuto[i] == 0) {
     turnRelay(i+1, Relay_State[i]); 
    }
  }

//  if(EEPROM.read(E2P_CRC8_pH1) == e2p_crc8(E2P_pH1Value_Low, (E2P_pH2Value_Low - E2P_pH1Value_Low))) {
  if(EEPROM.read(E2P_CRC8_pH1) == e2p_crc8(E2P_pH1Value_Low, 8)) {
    //EEPROM pH1 Settings
    pH1Value_Low = eepromReadFloat(E2P_pH1Value_Low);
    pH1Value_High = eepromReadFloat(E2P_pH1Value_High);
  }
  else {
    DefaultpH1();
    ec+=1;
  }

//  if(EEPROM.read(E2P_CRC8_pH2) == e2p_crc8(pH2Value_Low,(E2P_TempValue_Low - E2P_pH2Value_Low))) {
  if(EEPROM.read(E2P_CRC8_pH2) == e2p_crc8(E2P_pH2Value_Low, 8)) {
    //EEPROM pH2 Settings
    pH2Value_Low = eepromReadFloat(E2P_pH2Value_Low);
    pH2Value_High = eepromReadFloat(E2P_pH2Value_High);
  }
  else {
    DefaultpH2();
    ec+=1;
  }

  //EEPROM Temp Settings
//  if (EEPROM.read(E2P_CRC8_Temp) == e2p_crc8(E2P_TempValue_Low,(E2P_AC_OFF+4-E2P_TempValue_Low))) {
  if (EEPROM.read(E2P_CRC8_Temp) == e2p_crc8(E2P_TempValue_Low, 24)) {
    TempValue_Low  = eepromReadFloat(E2P_TempValue_Low);
    TempValue_High = eepromReadFloat(E2P_TempValue_High);
    Heater_ON = eepromReadFloat(E2P_Heater_ON);
    Heater_OFF =eepromReadFloat(E2P_Heater_OFF);
    AC_ON = eepromReadFloat(E2P_AC_ON);
    AC_OFF = eepromReadFloat(E2P_AC_OFF);
  }
  else {
    DefaultTemp();
    ec+=1;
  }

  //EEPROM RH Settings
//  if (EEPROM.read(E2P_CRC8_RH) == e2p_crc8(E2P_RHValue_Low,(E2P_Dehumidifier_OFF+4-E2P_RHValue_Low))) {
  if (EEPROM.read(E2P_CRC8_RH) == e2p_crc8(E2P_RHValue_Low, 24)) {
    RHValue_Low = eepromReadFloat(E2P_RHValue_Low);
    RHValue_High = eepromReadFloat(E2P_RHValue_High);
    Humidifier_ON = eepromReadFloat(E2P_Humidifier_ON);
    Humidifier_OFF = eepromReadFloat(E2P_Humidifier_OFF);
    Dehumidifier_ON = eepromReadFloat(E2P_Dehumidifier_ON);
    Dehumidifier_OFF = eepromReadFloat(E2P_Dehumidifier_OFF);
  }
  else {
    DefaultRH();
    ec+=1;
  }
  
  //EEPROM TDS1 Settings
//  if (EEPROM.read(E2P_CRC8_TDS1) == e2p_crc8(E2P_TDS1Value_Low,(E2P_MixPump1+1-E2P_TDS1Value_Low))) {
  if (EEPROM.read(E2P_CRC8_TDS1) == e2p_crc8(E2P_TDS1Value_Low,17)) {
    TDS1Value_Low = eepromReadFloat(E2P_TDS1Value_Low);
    TDS1Value_High = eepromReadFloat(E2P_TDS1Value_High);
    NutePump1_ON = eepromReadFloat(E2P_NutePump1_ON);
    NutePump1_OFF = eepromReadFloat(E2P_NutePump1_OFF);
    if (EEPROM.read(E2P_MixPump1) == 1){
      MixPump1_Enabled = true;
    } 
    else {
      MixPump1_Enabled = false;
    }
  }
  else {
    DefaultTDS1();
    ec+=1;
  }
  
  //EEPROM TDS2 Settings
//  if (EEPROM.read(E2P_CRC8_TDS2) == e2p_crc8(E2P_TDS2Value_Low,(E2P_MixPump2+1-E2P_TDS2Value_Low))) {
  if (EEPROM.read(E2P_CRC8_TDS2) == e2p_crc8(E2P_TDS2Value_Low, 17)) {
    TDS2Value_Low = eepromReadFloat(E2P_TDS2Value_Low);
    TDS2Value_High = eepromReadFloat(E2P_TDS2Value_High);
    NutePump2_ON = eepromReadFloat(E2P_NutePump2_ON);
    NutePump2_OFF = eepromReadFloat(E2P_NutePump2_OFF);
    if (EEPROM.read(E2P_MixPump2) == 1){
      MixPump2_Enabled = true;
    } 
    else {
      MixPump2_Enabled = false;
    }
  }
  else {
    DefaultTDS2();
    ec+=1;
  }
  
  //EEPROM CO2 Settings
//  if (EEPROM.read(E2P_CRC8_CO2) == e2p_crc8(E2P_CO2Value_Low,(E2P_CO2_Enabled+1-E2P_CO2Value_Low))) {
  if (EEPROM.read(E2P_CRC8_CO2) == e2p_crc8(E2P_CO2Value_Low, 17)) {
    CO2Value_Low = eepromReadFloat(E2P_CO2Value_Low);
    CO2Value_High = eepromReadFloat(E2P_CO2Value_High);
    CO2_ON = eepromReadFloat(E2P_CO2_ON);
    CO2_OFF = eepromReadFloat(E2P_CO2_OFF);
    if (EEPROM.read(E2P_CO2_Enabled) == 1){
      CO2_Enabled = true;
    } 
    else {
      CO2_Enabled = false;
    }
  }
  else {
    DefaultCO2();
    ec+=1;
  }

  //EEPROM Light Schedule
//  if (EEPROM.read(E2P_CRC8_Light) == e2p_crc8(E2P_Light_ON_hour,(E2P_Light_OFF_min+1-E2P_Light_ON_hour))) {
  if (EEPROM.read(E2P_CRC8_Light) == e2p_crc8(E2P_Light_ON_hour, 4)) {
    Light_ON_hour = EEPROM.read(E2P_Light_ON_hour);
    Light_ON_min = EEPROM.read(E2P_Light_ON_min);
    Light_OFF_hour = EEPROM.read(E2P_Light_OFF_hour);
    Light_OFF_min = EEPROM.read(E2P_Light_OFF_min);
  }
  else {
    DefaultLight();
    ec+=1;
  }
  
  //EEPROM Light Settings
//  if (EEPROM.read(E2P_CRC8_LightV) == e2p_crc8(E2P_LightValue_Low,(E2P_LightValue_High+4-E2P_LightValue_Low))) {
  if (EEPROM.read(E2P_CRC8_LightV) == e2p_crc8(E2P_LightValue_Low, 8)) {
    LightValue_Low = eepromReadFloat(E2P_LightValue_Low);
    LightValue_High = eepromReadFloat(E2P_LightValue_High);
  }
  else {
    DefaultLightV();
    ec+=1;
  }

  //EEPROM Water Pump Schedule
//  if(EEPROM.read(E2P_CRC8_Pump) == e2p_crc8(E2P_Pump_start_hour,(E2P_Pump_repeat+1-E2P_Pump_start_hour))) {
  if(EEPROM.read(E2P_CRC8_Pump) == e2p_crc8(E2P_Pump_start_hour, 6)) {
    Pump_start_hour = EEPROM.read(E2P_Pump_start_hour);
    Pump_start_minute = EEPROM.read(E2P_Pump_start_minute);
    Pump_every_hours = EEPROM.read(E2P_Pump_every_hours);
    Pump_every_mins = EEPROM.read(E2P_Pump_every_mins);
    Pump_for = EEPROM.read(E2P_Pump_for);
    Pump_repeat = EEPROM.read(E2P_Pump_repeat);
  }
  else {
    DefaultPumps();
    ec+=1;
  }
  FillPumpTimesArrays(Pump_start_hour, Pump_start_minute, Pump_every_hours, Pump_every_mins, Pump_for, Pump_repeat);

  return ec;
}

void saveE2PValues(void) {
  int i;
  
// Relay States
// Relay isAuto
  for(i = 0; i < MAXRELAYS; i++){
    EEPROM.write(i+E2P_Relay_State,Relay_State[i]);
    EEPROM.write(i+E2P_Relay_isAuto,Relay_isAuto[i]);
  }
  EEPROM.write(E2P_CRC8_RelayState, e2p_crc8(E2P_Relay_State, MAXRELAYS*2));
  
  //pH
  eepromWriteFloat(E2P_pH1Value_Low,pH1Value_Low);
  eepromWriteFloat(E2P_pH1Value_High,pH1Value_High);
//  EEPROM.write(E2P_CRC8_pH1, crc8((uint8_t*)&pH1Value_Low, (E2P_pH1Value_High+4-E2P_pH1Value_Low)));
  EEPROM.write(E2P_CRC8_pH1, e2p_crc8(E2P_pH1Value_Low, 8));

  eepromWriteFloat(E2P_pH2Value_Low,pH2Value_Low);
  eepromWriteFloat(E2P_pH2Value_High,pH2Value_High);
//  EEPROM.write(E2P_CRC8_pH2, crc8((uint8_t*)&pH2Value_Low, (E2P_pH2Value_High+4-E2P_pH2Value_Low)));
  EEPROM.write(E2P_CRC8_pH2, e2p_crc8(E2P_pH2Value_Low, 8));

  //Temp
  eepromWriteFloat(E2P_TempValue_Low,TempValue_Low);
  eepromWriteFloat(E2P_TempValue_High,TempValue_High);
  eepromWriteFloat(E2P_Heater_ON,Heater_ON);
  eepromWriteFloat(E2P_Heater_OFF,Heater_OFF);
  eepromWriteFloat(E2P_AC_ON,AC_ON);
  eepromWriteFloat(E2P_AC_OFF,AC_OFF);
  EEPROM.write(E2P_CRC8_Temp, e2p_crc8(E2P_TempValue_Low, 24));

  //RH
  eepromWriteFloat(E2P_RHValue_Low,RHValue_Low);
  eepromWriteFloat(E2P_RHValue_High,RHValue_High);
  eepromWriteFloat(E2P_Humidifier_ON,Humidifier_ON);
  eepromWriteFloat(E2P_Humidifier_OFF,Humidifier_OFF);
  eepromWriteFloat(E2P_Dehumidifier_ON,Dehumidifier_ON);
  eepromWriteFloat(E2P_Dehumidifier_OFF,Dehumidifier_OFF);
  EEPROM.write(E2P_CRC8_RH, e2p_crc8(E2P_RHValue_Low, 24));

  //TDS1
  eepromWriteFloat(E2P_TDS1Value_Low,TDS1Value_Low);
  eepromWriteFloat(E2P_TDS1Value_High,TDS1Value_High);
  eepromWriteFloat(E2P_NutePump1_ON,NutePump1_ON);
  eepromWriteFloat(E2P_NutePump1_OFF,NutePump1_OFF);
  if (MixPump1_Enabled == true) {
    EEPROM.write(E2P_MixPump1,1);
  } 
  else {
    EEPROM.write(E2P_MixPump1,0);
  }
//  EEPROM.write(E2P_CRC8_TDS1, crc8((uint8_t*)&TDS1Value_Low, (E2P_MixPump1+1-E2P_TDS1Value_Low)));
  EEPROM.write(E2P_CRC8_TDS1, e2p_crc8(E2P_TDS1Value_Low, 17));

  //TDS2
  eepromWriteFloat(E2P_TDS2Value_Low,TDS2Value_Low);
  eepromWriteFloat(E2P_TDS2Value_High,TDS2Value_High);
  eepromWriteFloat(E2P_NutePump2_ON,NutePump2_ON);
  eepromWriteFloat(E2P_NutePump2_OFF,NutePump2_OFF);
  if (MixPump2_Enabled == true) {
    EEPROM.write(E2P_MixPump2,1);
  } 
  else {
    EEPROM.write(E2P_MixPump2,0);
  }
//  EEPROM.write(E2P_CRC8_TDS2, crc8((uint8_t*)&TDS2Value_Low, (E2P_MixPump2+1-E2P_TDS2Value_Low)));
  EEPROM.write(E2P_CRC8_TDS2, e2p_crc8(E2P_TDS2Value_Low, 17));

  //CO2
  eepromWriteFloat(E2P_CO2Value_Low,CO2Value_Low);
  eepromWriteFloat(E2P_CO2Value_High,CO2Value_High);
  eepromWriteFloat(E2P_CO2_ON,CO2_ON);
  eepromWriteFloat(E2P_CO2_OFF,CO2_OFF);
  if (CO2_Enabled == true) {
    EEPROM.write(E2P_CO2_Enabled,1);
  } 
  else {
    EEPROM.write(E2P_CO2_Enabled,0);
  }
//  EEPROM.write(E2P_CRC8_CO2, crc8((uint8_t*)&CO2Value_Low, (E2P_CO2_Enabled+1-E2P_CO2Value_Low)));
  EEPROM.write(E2P_CRC8_CO2, e2p_crc8(E2P_CO2Value_Low, 17));

  //Light
  EEPROM.write(E2P_Light_ON_hour, Light_ON_hour);
  EEPROM.write(E2P_Light_ON_min, Light_ON_min);
  EEPROM.write(E2P_Light_OFF_hour, Light_OFF_hour);
  EEPROM.write(E2P_Light_OFF_min, Light_OFF_min);
//  EEPROM.write(E2P_CRC8_Light, crc8((uint8_t*)&Light_ON_hour, (E2P_Light_OFF_min+1-E2P_Light_ON_hour)));
  EEPROM.write(E2P_CRC8_Light, e2p_crc8(E2P_Light_ON_hour, 4));

  eepromWriteFloat(E2P_LightValue_Low,LightValue_Low);
  eepromWriteFloat(E2P_LightValue_High,LightValue_High);
//  EEPROM.write(E2P_CRC8_LightV, crc8((uint8_t*)&LightValue_Low, (E2P_LightValue_High+4-E2P_LightValue_Low)));
  EEPROM.write(E2P_CRC8_LightV, e2p_crc8(E2P_LightValue_Low, 8));

  //Pump
  EEPROM.write(E2P_Pump_start_hour,Pump_start_hour);
  EEPROM.write(E2P_Pump_start_minute,Pump_start_minute);
  EEPROM.write(E2P_Pump_every_hours,Pump_every_hours);
  EEPROM.write(E2P_Pump_every_mins,Pump_every_mins);
  EEPROM.write(E2P_Pump_for,Pump_for);
  EEPROM.write(E2P_Pump_repeat,Pump_repeat);
//  EEPROM.write(E2P_CRC8_Pump, crc8((uint8_t*)&Pump_start_hour, (E2P_Pump_repeat+1-E2P_Pump_start_hour)));
  EEPROM.write(E2P_CRC8_Pump, e2p_crc8(E2P_Pump_start_hour, 6));

// CRC8 the CRC8s
  EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));

}

void printE2PValues(void) {

  int i;
  // CRC8 Stored values
  
  Serial.print("\n>>CRC8 Values:\nCRC8>> ");
  Serial.print(e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)), HEX);
  Serial.print("\n>>CRC8 Values:\ne2p >> ");
  for(i = 0; i < ((E2P_CRC8_Pump-E2P_CRC8)+1); i++){
    printHEXsp(i+E2P_CRC8, EEPROM.read(i+E2P_CRC8));
  }
  // Relay States
  // Relay isAuto
  Serial.print("\n>>Relay_State:\nmem >> ");
  for(i = 0; i < MAXRELAYS; i++){
    printsp(i+E2P_Relay_State, Relay_State[i]);
  }
//  Serial.print("\n");
  Serial.print("\ne2p >> ");
  for(i = 0; i < MAXRELAYS; i++){
    printsp(i+E2P_Relay_State, EEPROM.read(i+E2P_Relay_State));
  }
//Serial.print("\n");
  Serial.print("\n>>Relay_isAuto:\nmem >> ");
  for(i = 0; i < MAXRELAYS; i++){
    printsp(i+E2P_Relay_isAuto, Relay_isAuto[i]);
  }
//Serial.print("\n");
  Serial.print("\ne2p >> ");
  for(i = 0; i < MAXRELAYS; i++){
    printsp(i+E2P_Relay_isAuto, EEPROM.read(i+E2P_Relay_isAuto));
  }
//Serial.print("\n");

  //pH
  Serial.print("\n>>pH:\nmem >> ");
  printflsp(E2P_pH1Value_Low,pH1Value_Low);
  printflsp(E2P_pH1Value_High,pH1Value_High);
  printflsp(E2P_pH2Value_Low,pH2Value_Low);
  printflsp(E2P_pH2Value_High,pH2Value_High);
//Serial.print("\n");
  Serial.print("\ne2p >> ");
  printflsp(E2P_pH1Value_Low,eepromReadFloat(E2P_pH1Value_Low));
  printflsp(E2P_pH1Value_High,eepromReadFloat(E2P_pH1Value_High));
  printflsp(E2P_pH2Value_Low,eepromReadFloat(E2P_pH2Value_Low));
  printflsp(E2P_pH2Value_High,eepromReadFloat(E2P_pH2Value_High));
//Serial.print("\n");
  
  //Temp
  Serial.print("\n>>Temp, Heater, AC:\nmem >>");
  printflsp(E2P_TempValue_Low,TempValue_Low);
  printflsp(E2P_TempValue_High,TempValue_High);
  printflsp(E2P_Heater_ON,Heater_ON);
  printflsp(E2P_Heater_OFF,Heater_OFF);
  printflsp(E2P_AC_ON,AC_ON);
  printflsp(E2P_AC_OFF,AC_OFF);
//Serial.print("\n");
  Serial.print("\ne2p >>");
  for (i=E2P_TempValue_Low;i<=E2P_AC_OFF;i+=4){
    printflsp(i, eepromReadFloat(i));
  }
//Serial.print("\n");
  //RH
  Serial.print("\n>>RH:\nmem >>");
  printflsp(E2P_RHValue_Low,RHValue_Low);
  printflsp(E2P_RHValue_High,RHValue_High);
  printflsp(E2P_Humidifier_ON,Humidifier_ON);
  printflsp(E2P_Humidifier_OFF,Humidifier_OFF);
  printflsp(E2P_Dehumidifier_ON,Dehumidifier_ON);
  printflsp(E2P_Dehumidifier_OFF,Dehumidifier_OFF);
//Serial.print("\n");
  Serial.print("\ne2p >>");
  for (i=E2P_RHValue_Low;i<=E2P_Dehumidifier_OFF;i+=4){
    printflsp(i, eepromReadFloat(i));
  }
//Serial.print("\n");
  //TDS1
  Serial.print("\n>>TDS1, Pump:\nmem >>");
  printflsp(E2P_TDS1Value_Low,TDS1Value_Low);
  printflsp(E2P_TDS1Value_High,TDS1Value_High);
  printflsp(E2P_NutePump1_ON,NutePump1_ON);
  printflsp(E2P_NutePump1_OFF,NutePump1_OFF);
  if (MixPump1_Enabled == true) {
    printsp(E2P_MixPump1,1);
  } 
  else {
    printsp(E2P_MixPump1,0);
  }
//Serial.print("\n");
  Serial.print("\ne2p >>");
  for (i=E2P_TDS1Value_Low;i<=E2P_NutePump1_OFF;i+=4){
    printflsp(i, eepromReadFloat(i));
  }
  printsp(E2P_MixPump1, EEPROM.read(E2P_MixPump1));
//Serial.print("\n");
  
  //TDS2
  Serial.print("\n>>TDS2, Pump:\nmem >>");
  printflsp(E2P_TDS2Value_Low,TDS2Value_Low);
  printflsp(E2P_TDS2Value_High,TDS2Value_High);
  printflsp(E2P_NutePump2_ON,NutePump2_ON);
  printflsp(E2P_NutePump2_OFF,NutePump2_OFF);
  if (MixPump2_Enabled == true) {
    printsp(E2P_MixPump2,1);
  } 
  else {
    printsp(E2P_MixPump2,0);
  }
//Serial.print("\n");
  Serial.print("\ne2p >>");
  for (i=E2P_TDS2Value_Low;i<=E2P_NutePump2_OFF;i+=4){
    printflsp(i, eepromReadFloat(i));
  }
  printsp(E2P_MixPump2, EEPROM.read(E2P_MixPump2));
//Serial.print("\n");

  //CO2
  Serial.print("\n>>CO2:\nmem >>");
  printflsp(E2P_CO2Value_Low,CO2Value_Low);
  printflsp(E2P_CO2Value_High,CO2Value_High);
  printflsp(E2P_CO2_ON,CO2_ON);
  printflsp(E2P_CO2_OFF,CO2_OFF);
  if (CO2_Enabled == true) {
    printsp(E2P_CO2_Enabled,1);
  } 
  else {
    printsp(E2P_CO2_Enabled,0);
  }
//Serial.print("\n");
  Serial.print("\ne2p >>");
  for (i=E2P_CO2Value_Low;i<=E2P_CO2_OFF;i+=4){
    printflsp(i, eepromReadFloat(i));
  }
  printsp(E2P_CO2_Enabled,EEPROM.read(E2P_CO2_Enabled));
//Serial.print("\n");

  //Light
  Serial.print("\n>>Light:\nmem >>");
  printflsp(E2P_LightValue_Low,LightValue_Low);
  printflsp(E2P_LightValue_High,LightValue_High);
  printsp(E2P_Light_ON_hour, Light_ON_hour);
  printsp(E2P_Light_ON_min, Light_ON_min);
  printsp(E2P_Light_OFF_hour, Light_OFF_hour);
  printsp(E2P_Light_OFF_min, Light_OFF_min);
//Serial.print("\n");
  Serial.print("\ne2p >>");
  printflsp(E2P_LightValue_Low,eepromReadFloat(E2P_LightValue_Low));
  printflsp(E2P_LightValue_High,eepromReadFloat(E2P_LightValue_High));
  printsp(E2P_Light_ON_hour,EEPROM.read(E2P_Light_ON_hour));
  printsp(E2P_Light_ON_min,EEPROM.read(E2P_Light_ON_min));
  printsp(E2P_Light_OFF_hour,EEPROM.read(E2P_Light_OFF_hour));
  printsp(E2P_Light_OFF_min,EEPROM.read(E2P_Light_OFF_min));
//Serial.print("\n");
  
  //Pump
  Serial.print("\n>>Pumps:\nmem >>");
  printsp(E2P_Pump_start_hour,Pump_start_hour);
  printsp(E2P_Pump_start_minute,Pump_start_minute);
  printsp(E2P_Pump_every_hours,Pump_every_hours);
  printsp(E2P_Pump_every_mins,Pump_every_mins);
  printsp(E2P_Pump_for,Pump_for);
  printsp(E2P_Pump_repeat,Pump_repeat);
//Serial.print("\n");
  Serial.print("\ne2p >>");
  for (i=E2P_Pump_start_hour;i<=E2P_Pump_repeat;i+=1){
    printsp(i, EEPROM.read(i));
  }
//Serial.print("\n");
  Serial.println("End crc");

}


void printsp(int addr, uint8_t val) {
  Serial.print(addr);
  Serial.print('-');
  Serial.print(val);
  Serial.print(' ');
//  delay(100);
}

void printflsp(int addr, float val) {
  Serial.print(addr);
  Serial.print('-');
  Serial.print(val);
  Serial.print(' ');
//  delay(100);
}

void printHEXsp(int addr, uint8_t val) {
  Serial.print(addr, HEX);
  Serial.print('-');
  Serial.print(val, HEX);
  Serial.print(' ');
//  delay(100);
}

void dumpcrc(void) {
//    printcrcError("crc         ", EEPROM.read(E2P_CRC8), e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)), (E2P_CRC8_Pump-E2P_CRC8));
//    printcrcError("Relay State ", EEPROM.read(E2P_CRC8_RelayState), e2p_crc8(E2P_Relay_State, MAXRELAYS*2), MAXRELAYS*2);
//    printcrcError("pH1         ", EEPROM.read(E2P_CRC8_pH1), e2p_crc8(E2P_pH1Value_Low, 8), 8);
//    printcrcError("pH2         ", EEPROM.read(E2P_CRC8_pH2), e2p_crc8(E2P_pH2Value_Low, 8), 8);
//    printcrcError("Temp        ", EEPROM.read(E2P_CRC8_Temp), e2p_crc8(E2P_TempValue_Low, 24), 24);
//    printcrcError("RH          ", EEPROM.read(E2P_CRC8_RH), e2p_crc8(E2P_RHValue_Low, 24), 24);
//    printcrcError("TDS1        ", EEPROM.read(E2P_CRC8_TDS1), e2p_crc8(E2P_TDS1Value_Low, 17), 17);
//    printcrcError("TDS2        ", EEPROM.read(E2P_CRC8_TDS2), e2p_crc8(E2P_TDS2Value_Low, 17), 17);
//    printcrcError("CO2         ", EEPROM.read(E2P_CRC8_CO2), e2p_crc8(E2P_CO2Value_Low, 17), 17);
//    printcrcError("Light Sched ", EEPROM.read(E2P_CRC8_Light), e2p_crc8(E2P_Light_ON_hour, 4), 4);
//    printcrcError("Light       ", EEPROM.read(E2P_CRC8_LightV), e2p_crc8(E2P_LightValue_Low, 8), 8);
//    printcrcError("Pump Sched  ", EEPROM.read(E2P_CRC8_Pump), e2p_crc8(E2P_Pump_start_hour, 6), 6);
}

void printcrcError(char* region, uint8_t E2Pcrcaddr, uint8_t calccrc, uint8_t lencrc) {
  Serial.print(region);
  Serial.print("CRC error: E2P -  ");
  Serial.print(E2Pcrcaddr, HEX);
  Serial.print("> Len  - ");
  Serial.print(lencrc, DEC);
  Serial.print(" ,Calc - ");
  Serial.println(calccrc, HEX);
//  delay(200);
}


