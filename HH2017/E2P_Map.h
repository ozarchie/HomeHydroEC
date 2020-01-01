// Relays
#define MAXRELAYS 8
// Water Pump Schedule
#define MAXREPEAT 24

// Relay State
#define E2P_Relay_State    0x20
#define E2P_Relay_isAuto   E2P_Relay_State+MAXRELAYS

//EEPROM Water Pump Schedule
#define E2P_Pump_start_hour     E2P_Relay_isAuto+MAXRELAYS
#define E2P_Pump_start_minute   E2P_Pump_start_hour+1
#define E2P_Pump_every_hours    E2P_Pump_start_minute+1
#define E2P_Pump_every_mins     E2P_Pump_every_hours+1
#define E2P_Pump_for            E2P_Pump_every_mins+1
#define E2P_Pump_repeat         E2P_Pump_for+1

//EEPROM Light Schedule
#define E2P_Light_ON_hour      E2P_Pump_repeat+1
#define E2P_Light_ON_min       E2P_Light_ON_hour+1
#define E2P_Light_OFF_hour     E2P_Light_ON_min+1
#define E2P_Light_OFF_min      E2P_Light_OFF_hour+1

// Settings are usually float (4 bytes)
//EEPROM Light Settings
#define E2P_LightValue_Low     0x100
#define E2P_LightValue_High    E2P_LightValue_Low+4

//EEPROM pH Settings
#define E2P_pH1Value_Low       E2P_LightValue_High+4
#define E2P_pH1Value_High      E2P_pH1Value_Low+4
#define E2P_pH2Value_Low       E2P_pH1Value_High+4
#define E2P_pH2Value_High      E2P_pH2Value_Low+4

//EEPROM Temp Settings
#define E2P_TempValue_Low      E2P_pH2Value_High+4
#define E2P_TempValue_High     E2P_TempValue_Low+4
#define E2P_Heater_ON          E2P_TempValue_High+4
#define E2P_Heater_OFF         E2P_Heater_ON+4
#define E2P_AC_ON              E2P_Heater_OFF+4
#define E2P_AC_OFF             E2P_AC_ON+4

//EEPROM RH Settings
#define E2P_RHValue_Low        E2P_AC_OFF+4
#define E2P_RHValue_High       E2P_RHValue_Low+4
#define E2P_Humidifier_ON      E2P_RHValue_High+4
#define E2P_Humidifier_OFF     E2P_Humidifier_ON+4
#define E2P_Dehumidifier_ON    E2P_Humidifier_OFF+4
#define E2P_Dehumidifier_OFF   E2P_Dehumidifier_ON+4

//EEPROM TDS1 Settings
#define E2P_TDS1Value_Low      E2P_Dehumidifier_OFF+4
#define E2P_TDS1Value_High     E2P_TDS1Value_Low+4
#define E2P_NutePump1_ON       E2P_TDS1Value_High+4
#define E2P_NutePump1_OFF      E2P_NutePump1_ON+4
#define E2P_MixPump1           E2P_NutePump1_OFF+4

//EEPROM TDS2 Settings
#define E2P_TDS2Value_Low      E2P_MixPump1+4
#define E2P_TDS2Value_High     E2P_TDS2Value_Low+4
#define E2P_NutePump2_ON       E2P_TDS2Value_High+4
#define E2P_NutePump2_OFF      E2P_NutePump2_ON+4
#define E2P_MixPump2           E2P_NutePump2_OFF+4

//EEPROM CO2 Settings
#define E2P_CO2Value_Low       E2P_MixPump2+4
#define E2P_CO2Value_High      E2P_CO2Value_Low+4
#define E2P_CO2_ON             E2P_CO2Value_High+4
#define E2P_CO2_OFF            E2P_CO2_ON+4
#define E2P_CO2_Enabled        E2P_CO2_OFF+4

// Actual EEPROM CRCs

#define  E2P_CRC8              0x200
//#define  E2P_CRC8_Time         E2P_CRC8+1
#define  E2P_CRC8_RelayState   E2P_CRC8+1
#define  E2P_CRC8_pH1          E2P_CRC8_RelayState+1
#define  E2P_CRC8_pH2          E2P_CRC8_pH1+1
#define  E2P_CRC8_Temp         E2P_CRC8_pH2+1
#define  E2P_CRC8_RH           E2P_CRC8_Temp+1
#define  E2P_CRC8_TDS1         E2P_CRC8_RH+1
#define  E2P_CRC8_TDS2         E2P_CRC8_TDS1+1
#define  E2P_CRC8_CO2          E2P_CRC8_TDS2+1
#define  E2P_CRC8_Light        E2P_CRC8_CO2+1
#define  E2P_CRC8_LightV       E2P_CRC8_Light+1
#define  E2P_CRC8_Pump         E2P_CRC8_LightV+1    // Pump schedule
//#define  E2P_CRC8_PumpRepeat   E2P_CRC8_Pump+1

