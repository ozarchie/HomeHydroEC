#include "Arduino.h"
/*
  SEND SERIAL MESSAGE
 */
void sendserialmessages(){
  int i;
  //The purpose of 'serialcounter' is so that serial messages are not sent as one big chunk of data every 'loop()'
  //Time, Sensors, and Relay values are sent every odd messages.

  if ((serialcounter & 7) == 1){
    // TIMESTAMP
    sendDate();
  }
  if ((serialcounter & 7) == 3){
    // DATA
    updateSensorValues();
    sendSensorData();
  }
  if ((serialcounter & 7) == 5){
    // RELAYS  
    sendRelayData(); // Relay state
  }
  if ((serialcounter & 7) == 7){
    // RELAYS  
    sendRelaySetup(); // Relay setup
  }
  if (serialcounter == 2) { 
    //Light, Water Schedules
    sendLightSchedule();
  }
 if (serialcounter == 4) { 
    //Light, Water Schedules
    sendWaterSchedule();
  }
 if (serialcounter == 6) {
    //pH Setpoints
    sendpH1Setpoints();
  }
 if (serialcounter == 8) {
    //TDS Setpoints
    sendTDS1Setpoints();
  }
 if (serialcounter == 10) {
    //pH Setpoints
    sendpH2Setpoints();
  }
 if (serialcounter == 12) {
    //TDS Setpoints
    sendTDS2Setpoints();
  }
 if (serialcounter == 14) { 
    //Temperature Setpoints
    sendTemperatureSetpoints();
  }
 if (serialcounter == 16) { 
    //Humidifier Setpoints
    sendHumiditySetpoints();
  }
 if (serialcounter == 18) { 
    //CO2 Setpoints
    sendCO2Setpoints();
  }
 if (serialcounter == 20) { 
    //Light Setpoints
    sendLightSetpoints();
    serialcounter = 0;
  }
  serialcounter++;
}

void sendDate(){
    updatelongdate();
    Serial.print("Time,");
    Serial.print(longdate);
    Serial.print(",");
    Serial.print(month());
    Serial.print(",");
    Serial.print(day());
    Serial.print(",");
    Serial.print(year());
    Serial.print(",");
    Serial.print(hour());
    Serial.print(",");
    Serial.print(minute());
    Serial.print(",");
    Serial.println(second());
}

void sendAllMessages() {
  sendDate();
  updateSensorValues();
  sendSensorData();
  sendRelayData(); // Relay state and setup

  sendLightSchedule();
  sendWaterSchedule();

  //pH
  sendpHSetpoints();
  //Temp
  sendTemperatureSetpoints();
  //RH
  sendHumiditySetpoints();
  //TDS
  sendTDSSetpoints();
  //CO2
  sendCO2Setpoints();
  //Light
  sendLightSetpoints();  
}

void sendSensorData() {
    // SENSOR VALUES
    Serial.print("Sensors,");
    Serial.print(pH1Value);
    Serial.print(",");
    Serial.print(pH2Value);
    Serial.print(",");
    Serial.print(TempValue);
    Serial.print(",");
    Serial.print(RHValue);
    Serial.print(",");
    Serial.print(TDS1Value);
    Serial.print(",");
    Serial.print(TDS2Value);
    Serial.print(",");
    Serial.print(CO2Value);
    Serial.print(",");
    Serial.println(LightValue);
}
    
void sendLightSchedule() {
    Serial.print("Light_Schedule,");
    Serial.print(Light_ON_hour);
    Serial.print(",");
    Serial.print(Light_ON_min);
    Serial.print(",");
    Serial.print(Light_OFF_hour);
    Serial.print(",");
    Serial.println(Light_OFF_min);  
}

void sendWaterSchedule() {
    Serial.print("Watering_Schedule,");
    Serial.print(Pump_start_hour);
    Serial.print(",");
    Serial.print(Pump_start_minute);
    Serial.print(",");
    if (Pump_start_isAM == true) {
      Serial.print("1,");
    } 
    else {
      Serial.print("0,");
    }
    Serial.print(Pump_every_hours);
    Serial.print(",");
    Serial.print(Pump_every_mins);
    Serial.print(",");
    Serial.print(Pump_for);
    Serial.print(",");
    Serial.println(Pump_repeat);  
}

void sendpHSetpoints() {
  sendpH1Setpoints();
  sendpH2Setpoints();
}
void sendpH1Setpoints() {
    Serial.print("SetPoint_pH1,");
    Serial.print(pH1Value_Low);
    Serial.print(",");
    Serial.print(pH1Value_High);
    Serial.print(",");
    Serial.println(pH1_Status);
}
void sendpH2Setpoints() {
    Serial.print("SetPoint_pH2,");
    Serial.print(pH2Value_Low);
    Serial.print(",");
    Serial.print(pH2Value_High);
    Serial.print(",");
    Serial.println(pH2_Status);  
}

void sendTemperatureSetpoints() {
    Serial.print("SetPoint_Temp,");
    Serial.print(TempValue_Low);
    Serial.print(",");
    Serial.print(TempValue_High);
    Serial.print(",");
    Serial.print(Heater_ON);
    Serial.print(",");
    Serial.print(Heater_OFF);
    Serial.print(",");
    Serial.print(AC_ON);
    Serial.print(",");
    Serial.print(AC_OFF);
    Serial.print(",");
    Serial.println(Temp_Status);  
}

void sendHumiditySetpoints() {
    Serial.print("SetPoint_RH,");
    Serial.print(RHValue_Low);
    Serial.print(",");
    Serial.print(RHValue_High);
    Serial.print(",");
    Serial.print(Humidifier_ON);
    Serial.print(",");
    Serial.print(Humidifier_OFF);
    Serial.print(",");
    Serial.print(Dehumidifier_ON);
    Serial.print(",");
    Serial.print(Dehumidifier_OFF);
    Serial.print(",");        
    Serial.println(RH_Status);  
}

void sendLightSetpoints(){
    Serial.print("SetPoint_Light,");
    Serial.print(LightValue_Low);
    Serial.print(",");
    Serial.print(LightValue_High);
    Serial.print(",");
    Serial.println(Light_Status); 
 }

void sendTDSSetpoints() {
  sendTDS1Setpoints();
  sendTDS2Setpoints();
}

void sendTDS1Setpoints(){
    Serial.print("SetPoint_TDS1,");
    Serial.print(TDS1Value_Low);
    Serial.print(",");
    Serial.print(TDS1Value_High);
    Serial.print(",");
    Serial.print(NutePump1_ON);
    Serial.print(",");
    Serial.print(NutePump1_OFF);
    Serial.print(",");
    Serial.print(MixPump1_Enabled);
    Serial.print(",");
    Serial.println(TDS1_Status);
}
void sendTDS2Setpoints(){
    Serial.print("SetPoint_TDS2,");
    Serial.print(TDS2Value_Low);
    Serial.print(",");
    Serial.print(TDS2Value_High);
    Serial.print(",");
    Serial.print(NutePump2_ON);
    Serial.print(",");
    Serial.print(NutePump2_OFF);
    Serial.print(",");
    Serial.print(MixPump2_Enabled);
    Serial.print(",");
    Serial.println(TDS2_Status);
}

void sendCO2Setpoints() {
    Serial.print("SetPoint_CO2,");
    Serial.print(CO2Value_Low);
    Serial.print(",");
    Serial.print(CO2Value_High);
    Serial.print(",");
    Serial.print(CO2_ON);
    Serial.print(",");
    Serial.print(CO2_OFF);
    Serial.print(",");
    Serial.print(CO2_Enabled);
    Serial.print(",");
    Serial.println(CO2_Status);
}

/*
  HANDLE SERIAL INPUT DATA
 */
void serialEvent() {

  int j, r, n;
  int i = 0;
  while ((Serial.available() > 0) && (stringComplete == false)) {
    // get the new byte:
    char inChar = (char)Serial.read();

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      SerialBuffer[pSerialBuffer++] = inChar;
      SerialBuffer[pSerialBuffer] = 0x00;
    }
  }
  // Found a string, so parse it and action
  // --------------------------------------
  if (stringComplete == true) {
   stringComplete = false;
//   Serial.print("Command: ");
//   Serial.println(SerialBuffer);
   
   // Parse
   // Scan string buffer and separate into sub-strings (if any)
   pSerialBuffer = 0;
   while ((SerialBuffer[pSerialBuffer] != 0) && (i < 16)) {
     message[i]=&SerialBuffer[pSerialBuffer];
     i += 1;
     while ((SerialBuffer[pSerialBuffer] != ',') && (SerialBuffer[pSerialBuffer] != 0)){
       pSerialBuffer += 1;
     }
     if (SerialBuffer[pSerialBuffer] != 0 ) {
       SerialBuffer[pSerialBuffer] = 0; // Null terminate sub-string
       pSerialBuffer += 1;              // Go past
     }
   }
   i -= 1;
   pSerialBuffer = 0;

// Completed message so determine action
// =====================================

    j = 99;      // Set default error
    
    if (strcmp(message[0], "Relay1 on") == 0) {
      j = 0;
      r = 0;
    }
    else if (strcmp(message[0], "Relay1 off") == 0) {
      j = 1;
      r = 0;
    }
    else if (strcmp(message[0], "Relay2 on") == 0) {
      j = 2;
      r = 1;
    }
    else if (strcmp(message[0], "Relay2 off") == 0) {
      j = 3;
      r = 1;
    }
    else if (strcmp(message[0], "Relay3 on") == 0) {
      j = 4;
      r = 2;
    }
    else if (strcmp(message[0], "Relay3 off") == 0) {
      j = 5;
      r = 2;
    }
    else if (strcmp(message[0], "Relay4 on") == 0) {
      j = 6;
      r = 3;
    }
    else if (strcmp(message[0], "Relay4 off") == 0) {
      j = 7;
      r = 3;
    }
    else if (strcmp(message[0], "Relay5 on") == 0) {
      j = 8;
      r = 4;
    }
    else if (strcmp(message[0], "Relay5 off") == 0) {
      j = 9;
      r = 4;
    }
    else if (strcmp(message[0], "Relay6 on") == 0) {
      j = 10;
      r = 5;
    }
    else if (strcmp(message[0], "Relay6 off") == 0) {
      j = 11;
      r = 5;
    }
    else if (strcmp(message[0], "Relay1 isAuto 1") == 0) {
      j = 12;
      r = 0;
    }
    else if (strcmp(message[0], "Relay1 isAuto 0") == 0) {
      j = 13;
      r = 0;
    }
    else if (strcmp(message[0], "Relay2 isAuto 1") == 0) {
      j = 14;
      r = 1;
    }
    else if (strcmp(message[0], "Relay2 isAuto 0") == 0) {
      j = 15;
      r = 1;
    }
    else if (strcmp(message[0], "Relay3 isAuto 1") == 0) {
      j = 16;
      r = 2;
    }
    else if (strcmp(message[0], "Relay3 isAuto 0") == 0) {
      j = 17;
      r = 2;
    }
    else if (strcmp(message[0], "Relay4 isAuto 1") == 0) {
      j = 18;
      r = 3;
    }
    else if (strcmp(message[0], "Relay4 isAuto 0") == 0) {
      j = 19;
      r = 3;
    }
    else if (strcmp(message[0], "Relay5 isAuto 1") == 0) {
      j = 20;
      r = 4;
    }
    else if (strcmp(message[0], "Relay5 isAuto 0") == 0) {
      j = 21;
      r = 4;
    }
    else if (strcmp(message[0], "Relay6 isAuto 1") == 0) {
      j = 22;
      r = 5;
    }
    else if (strcmp(message[0], "Relay6 isAuto 0") == 0) {
      j = 23;
      r = 5;
    }
    else if (strcmp(message[0], "restoredefaults") == 0) {
      j = 24;
    }
    else if (strcmp(message[0], "setdate") == 0) {
      j = 25;
      n = 6;
    }
    else if (strcmp(message[0], "setlightschedule") == 0) {
      j = 26;
      n = 4;
    }
    else if (strcmp(message[0], "setwateringschedule") == 0) {
      j = 27;
      n = 6;
    }
    else if (strcmp(message[0], "setpH1") == 0) {
      j = 28;
      r = 0;
      n = 2;
    }
    else if (strcmp(message[0], "setpH2") == 0) {
      j = 29;
      r = 1;
      n = 2;
    }
    else if (strcmp(message[0], "setTemp") == 0) {
      j = 30;
      n = 6;
    }
    else if (strcmp(message[0], "setRH") == 0) {
      j = 31;
      n = 6;
    }
    else if (strcmp(message[0], "setTDS1Value") == 0) {
      j = 32;
      r = 0;
      n = 5;
    }
    else if (strcmp(message[0], "setTDS2Value") == 0) {
      j = 33;
      r = 1;
      n = 5;
    }
    else if (strcmp(message[0], "setCO2") == 0) {
      j = 34;
      n = 5;
    }
    else if (strcmp(message[0], "setLight") == 0) {
      j = 35;
      n = 5;
    }

/*
   Serial.print("CmdIndex: ");
   Serial.println(j);
   Serial.print("Options : ");
   Serial.println(i);
   Serial.print("Command : ");
   Serial.println(message[0]);
*/

// Execute the commands
// ====================

    switch(j) {
    // Relays On/Off
    // r = (relay number - 1)
     case 0:      // Relay1 On
     case 1:      // Relay1 Off
     case 2:      // Relay2 On
     case 3:      // Relay2 Off
     case 4:      // Relay3 On
     case 5:      // Relay3 Off
     case 6:      // Relay4 On
     case 7:      // Relay4 Off
     case 8:      // Relay5 On
     case 9:      // Relay5 Off
     case 10:     // Relay6 On
     case 11:     // Relay6 Off
       if (i == 0) {
          if ((j & 1) == 0) {
            Relay_State[r] = ON;
            EEPROM.write((E2P_Relay_State+r),ON);
            turnRelay((r+1), ON);
          }
          else {
            Relay_State[r] = OFF;
            EEPROM.write((E2P_Relay_State+r),OFF);
            turnRelay((r+1), OFF);
          }
          EEPROM.write(E2P_CRC8_RelayState, e2p_crc8(E2P_Relay_State, MAXRELAYS*2));
        // CRC8 the CRC8s
          EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
//          dumpcrc();
//          sendRelayData();
       }
       else j = 98;
       break;
    // Relays Auto On/Off
     case 12:      // Relay1 isAuto On
     case 13:      // Relay1 isAuto Off
     case 14:      // Relay2 isAuto On 
     case 15:      // Relay2 isAuto Off
     case 16:      // Relay3 isAuto On 
     case 17:      // Relay3 isAuto Off
     case 18:      // Relay4 isAuto On 
     case 19:      // Relay4 isAuto Off
     case 20:      // Relay5 isAuto On 
     case 21:      // Relay5 isAuto Off
     case 22:      // Relay6 isAuto On
     case 23:      // Relay6 isAuto Off
       if (i == 0) {
          if ((j & 1) == 0) {
            Relay_isAuto[r] = 1;
            EEPROM.write((E2P_Relay_isAuto+r),1);
          }
          else {
            Relay_isAuto[r] = 0;
            EEPROM.write((E2P_Relay_isAuto+r),0);
          }
          EEPROM.write(E2P_CRC8_RelayState, e2p_crc8(E2P_Relay_State, MAXRELAYS*2));
        // CRC8 the CRC8s
          EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
//          dumpcrc();
//          sendRelaySetup();
       }
       else j = 98;
       break;
    // Restore defaults
     case 24:
       if (i == 0) {
         RestoreDefaults();
       }
       else j = 98;
       break;
    // Set Date
     case 25:
       if (i == n) {
         int parsed_month = atoi(message[1]);
         int parsed_day = atoi(message[2]);
         int parsed_year = atoi(message[3]);
         int parsed_hour = atoi(message[4]);
         int parsed_minute = atoi(message[5]);
         int parsed_second = atoi(message[6]);
         
          // Set time    
         setTime(parsed_hour,parsed_minute,parsed_second,parsed_day,parsed_month,parsed_year);
         now();
          // Set RTC
         tm.Hour=hour();
         tm.Minute=minute();
         tm.Second=second();
         tm.Day=day();
         tm.Month=month();
         tm.Year=CalendarYrToTm(year());
         rtc.write(tm);
       }
       else j = 98;
       break;
    // Set Light Schedule
     case 26:
       if (i == n) {
        int parsed_Light_ON_hour = atoi(message[1]);
        int parsed_Light_ON_min = atoi(message[2]);
        int parsed_Light_OFF_hour = atoi(message[3]);
        int parsed_Light_OFF_min = atoi(message[4]);

        Light_ON_hour = parsed_Light_ON_hour;
        Light_ON_min = parsed_Light_ON_min;
        Light_OFF_hour = parsed_Light_OFF_hour;
        Light_OFF_min = parsed_Light_OFF_min;

        EEPROM.write(E2P_Light_ON_hour, Light_ON_hour);
        EEPROM.write(E2P_Light_OFF_hour, Light_OFF_hour);
        EEPROM.write(E2P_Light_ON_min, Light_ON_min);
        EEPROM.write(E2P_Light_OFF_min, Light_OFF_min);
        EEPROM.write(E2P_CRC8_Light, e2p_crc8(E2P_Light_ON_hour, 4));
        // CRC8 the CRC8s
        EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
//        EEPROM.write(E2P_CRC8_Light, crc8((uint8_t*)&Light_ON_hour,(E2P_Light_OFF_min+1-E2P_Light_ON_hour)));
//        sendLightSchedule();
       }
       else j = 98;
       break;
    // Set Watering Schedule
     case 27:
       if ( i == n) {
          int parsed_Pump_start_hour = atoi(message[1]);
          int parsed_Pump_start_min = atoi(message[2]);
          int parsed_Pump_every_hours = atoi(message[3]);
          int parsed_Pump_every_mins = atoi(message[4]);
          int parsed_Pump_for = atoi(message[5]);
          int parsed_Pump_repeat = atoi(message[6]);
  
          Pump_start_hour = parsed_Pump_start_hour;
          Pump_start_minute = parsed_Pump_start_min;
          Pump_every_hours = parsed_Pump_every_hours;
          Pump_every_mins = parsed_Pump_every_mins;
          Pump_for = parsed_Pump_for;
          Pump_repeat = parsed_Pump_repeat;
          if (Pump_repeat > MAXREPEAT) Pump_repeat = MAXREPEAT;

          EEPROM.write(E2P_Pump_start_hour,Pump_start_hour);
          EEPROM.write(E2P_Pump_start_minute,Pump_start_minute);
          EEPROM.write(E2P_Pump_every_hours,Pump_every_hours);
          EEPROM.write(E2P_Pump_every_mins,Pump_every_mins);
          EEPROM.write(E2P_Pump_for,Pump_for);
          EEPROM.write(E2P_Pump_repeat,Pump_repeat);
//          EEPROM.write(E2P_CRC8_Pump, crc8((uint8_t*)&Pump_start_hour,(E2P_Pump_repeat+1-E2P_Pump_start_hour)));
          EEPROM.write(E2P_CRC8_Pump, e2p_crc8(E2P_Pump_start_hour, 6));
        // CRC8 the CRC8s
          EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
          FillPumpTimesArrays(Pump_start_hour, Pump_start_minute, Pump_every_hours, Pump_every_mins, Pump_for, Pump_repeat);
//          sendWaterSchedule();
       }
       else j = 98;
       break;
    // Set pH Limits
     case 28:
     case 29:
       if ( i == n) {
         if ((j & 1) == 0) {
          float parsed_pH1Value_Low = atof(message[1]);
          float parsed_pH1Value_High = atof(message[2]);
          pH1Value_Low = parsed_pH1Value_Low;
          pH1Value_High = parsed_pH1Value_High;
          eepromWriteFloat(E2P_pH1Value_Low, parsed_pH1Value_Low);
          eepromWriteFloat(E2P_pH1Value_High, parsed_pH1Value_High);  
//          EEPROM.write(E2P_CRC8_pH1, crc8((uint8_t*)&pH1Value_Low,(E2P_pH1Value_High+4-E2P_pH1Value_Low)));
          EEPROM.write(E2P_CRC8_pH1, e2p_crc8(E2P_pH1Value_Low, 8));
        // CRC8 the CRC8s
          EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //          sendpHSetpoints();
         }
         else {
          float parsed_pH2Value_Low = atof(message[1]);
          float parsed_pH2Value_High = atof(message[2]);
          pH1Value_Low = parsed_pH2Value_Low;
          pH1Value_High = parsed_pH2Value_High;
          eepromWriteFloat(E2P_pH2Value_Low, parsed_pH2Value_Low);
          eepromWriteFloat(E2P_pH2Value_High, parsed_pH2Value_High);  
//          EEPROM.write(E2P_CRC8_pH2, crc8((uint8_t*)&pH2Value_Low,(E2P_pH2Value_High+4-E2P_pH2Value_Low)));
          EEPROM.write(E2P_CRC8_pH2, e2p_crc8(E2P_pH2Value_Low, 8));
        // CRC8 the CRC8s
          EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //          sendpHSetpoints();
         }
       }
       else j = 98;
       break;
    // Set Temperature Limits
     case 30:
       if (n == i) {

        float parsed_TempValue_Low = atof(message[1]);
        float parsed_TempValue_High = atof(message[2]);
        float parsed_Heater_ON = atof(message[3]);
        float parsed_Heater_OFF = atof(message[4]);
        float parsed_AC_ON = atof(message[5]);
        float parsed_AC_OFF = atof(message[6]);

        TempValue_Low = parsed_TempValue_Low;
        TempValue_High = parsed_TempValue_High;
        Heater_ON = parsed_Heater_ON;
        Heater_OFF = parsed_Heater_OFF;
        AC_ON = parsed_AC_ON;
        AC_OFF = parsed_AC_OFF;
        eepromWriteFloat(E2P_TempValue_Low, parsed_TempValue_Low);
        eepromWriteFloat(E2P_TempValue_High, parsed_TempValue_High);  
        eepromWriteFloat(E2P_Heater_ON, parsed_Heater_ON);
        eepromWriteFloat(E2P_Heater_OFF, parsed_Heater_OFF);  
        eepromWriteFloat(E2P_AC_ON, parsed_AC_ON);
        eepromWriteFloat(E2P_AC_OFF, parsed_AC_OFF);  
//        EEPROM.write(E2P_CRC8_Temp, crc8((uint8_t*)&TempValue_Low,(E2P_AC_OFF+4-E2P_TempValue_Low)));
        EEPROM.write(E2P_CRC8_Temp, e2p_crc8(E2P_TempValue_Low, 24));
        // CRC8 the CRC8s
        EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //        sendTemperatureSetpoints();
       }
       else j = 98;
       break;
    // Set Relative Humidity Limits
     case 31:
       if (n == i) {
        float parsed_RHValue_Low = atof(message[1]);
        float parsed_RHValue_High = atof(message[2]);
        float parsed_Humidifier_ON = atof(message[3]);
        float parsed_Humidifier_OFF = atof(message[4]);
        float parsed_Dehumidifier_ON = atof(message[5]);
        float parsed_Dehumidifier_OFF = atof(message[6]);

        RHValue_Low = parsed_RHValue_Low;
        RHValue_High = parsed_RHValue_High;
        Humidifier_ON = parsed_Humidifier_ON;
        Humidifier_OFF = parsed_Humidifier_OFF;
        Dehumidifier_ON = parsed_Dehumidifier_ON;
        Dehumidifier_OFF = parsed_Dehumidifier_OFF;

        eepromWriteFloat(E2P_RHValue_Low, parsed_RHValue_Low);
        eepromWriteFloat(E2P_RHValue_High, parsed_RHValue_High);  
        eepromWriteFloat(E2P_Humidifier_ON, parsed_Humidifier_ON);
        eepromWriteFloat(E2P_Humidifier_OFF, parsed_Humidifier_OFF);  
        eepromWriteFloat(E2P_Dehumidifier_ON, parsed_Dehumidifier_ON);
        eepromWriteFloat(E2P_Dehumidifier_OFF, parsed_Dehumidifier_OFF);  
//        EEPROM.write(E2P_CRC8_RH, crc8((uint8_t*)&RHValue_Low,(E2P_Dehumidifier_OFF+4-E2P_RHValue_Low)));
        EEPROM.write(E2P_CRC8_RH, e2p_crc8(E2P_RHValue_Low, 24));
        // CRC8 the CRC8s
        EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //        sendHumiditySetpoints();
       }
       else j = 98;
       break;
    // Set TDS Limits
     case 32:
     case 33:
       if (n == i) {

         float parsed_TDSValue_Low = atof(message[1]);
         float parsed_TDSValue_High = atof(message[2]);
         float parsed_NutePump_ON = atof(message[3]);
         float parsed_NutePump_OFF = atof(message[4]);
         int parsed_MixPump_Enabled = atoi(message[5]);
          
         if ((j & 1) == 0) {  
          TDS1Value_Low = parsed_TDSValue_Low;
          TDS1Value_High = parsed_TDSValue_High;
          NutePump1_ON = parsed_NutePump_ON;
          NutePump1_OFF = parsed_NutePump_OFF;
          eepromWriteFloat(E2P_TDS1Value_Low, parsed_TDSValue_Low);
          eepromWriteFloat(E2P_TDS1Value_High, parsed_TDSValue_High);  
          eepromWriteFloat(E2P_NutePump1_ON, parsed_NutePump_ON);
          eepromWriteFloat(E2P_NutePump1_OFF, parsed_NutePump_OFF);  
          if (parsed_MixPump_Enabled == 1) {
            MixPump1_Enabled = true;
            EEPROM.write(E2P_MixPump1, 1);
          } 
          else if(parsed_MixPump_Enabled == 0) {
            MixPump1_Enabled = false;
            EEPROM.write(E2P_MixPump1, 0);            
          }
//          EEPROM.write(E2P_CRC8_TDS1, crc8((uint8_t*)&TDS1Value_Low,(E2P_MixPump1+1-E2P_TDS1Value_Low)));
          EEPROM.write(E2P_CRC8_TDS1, e2p_crc8(E2P_TDS1Value_Low, 17));
        // CRC8 the CRC8s
          EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //          sendTDSSetpoints();
         }
        else {
          TDS2Value_Low = parsed_TDSValue_Low;
          TDS2Value_High = parsed_TDSValue_High;
          NutePump2_ON = parsed_NutePump_ON;
          NutePump2_OFF = parsed_NutePump_OFF;
          eepromWriteFloat(E2P_TDS2Value_Low, parsed_TDSValue_Low);
          eepromWriteFloat(E2P_TDS2Value_High, parsed_TDSValue_High);  
          eepromWriteFloat(E2P_NutePump2_ON, parsed_NutePump_ON);
          eepromWriteFloat(E2P_NutePump2_OFF, parsed_NutePump_OFF);  
          if (parsed_MixPump_Enabled == 1) {
            MixPump2_Enabled = true;
            EEPROM.write(E2P_MixPump2, 1);
          } 
          else if(parsed_MixPump_Enabled == 0) {
            MixPump2_Enabled = false;
            EEPROM.write(E2P_MixPump2, 0);            
          }
//          EEPROM.write(E2P_CRC8_TDS2, crc8((uint8_t*)&TDS2Value_Low,(E2P_MixPump2+1-E2P_TDS2Value_Low)));
          EEPROM.write(E2P_CRC8_TDS2, e2p_crc8(E2P_TDS2Value_Low, 17));
        // CRC8 the CRC8s
          EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //          sendTDSSetpoints();
        }         
       }
       else j = 98;
       break;
    // Set/Enable CO2 Limits
     case 34:
       if (n == i) {
        float parsed_CO2Value_Low = atof(message[1]);
        float parsed_CO2Value_High = atof(message[2]);
        float parsed_CO2_ON = atof(message[3]);
        float parsed_CO2_OFF = atof(message[4]);
        int parsed_CO2_Enabled = atoi(message[5]);

        CO2Value_Low = parsed_CO2Value_Low;
        CO2Value_High = parsed_CO2Value_High;
        CO2_ON = parsed_CO2_ON;
        CO2_OFF = parsed_CO2_OFF;
        eepromWriteFloat(E2P_CO2Value_Low, parsed_CO2Value_Low);
        eepromWriteFloat(E2P_CO2Value_High, parsed_CO2Value_High);  
        eepromWriteFloat(E2P_CO2_ON, parsed_CO2_ON);
        eepromWriteFloat(E2P_CO2_OFF, parsed_CO2_OFF);  
        if (parsed_CO2_Enabled == 1) {
          CO2_Enabled = true;
          EEPROM.write(E2P_CO2_Enabled, 1);
        } 
        else if(parsed_CO2_Enabled == 0) {
          CO2_Enabled = false;
          EEPROM.write(E2P_CO2_Enabled, 0);            
        }
//        EEPROM.write(E2P_CRC8_CO2, crc8((uint8_t*)&CO2Value_Low,(E2P_CO2_Enabled+1-E2P_CO2Value_Low)));
        EEPROM.write(E2P_CRC8_CO2, e2p_crc8(E2P_CO2Value_Low, 17));
        // CRC8 the CRC8s
        EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //        sendCO2Setpoints();
       }
       else j = 98;
       break;
    // Set Light Times
     case 35:
       if (i == n) {
        float parsed_LightValue_Low = atof(message[1]);
        float parsed_LightValue_High = atof(message[2]);
        LightValue_Low = parsed_LightValue_Low;
        LightValue_High = parsed_LightValue_High;
        eepromWriteFloat(E2P_LightValue_Low, parsed_LightValue_Low);
        eepromWriteFloat(E2P_LightValue_High, parsed_LightValue_High);  
//        EEPROM.write(E2P_CRC8_LightV, crc8((uint8_t*)&LightValue_Low,(E2P_LightValue_High+4-E2P_LightValue_Low)));
        EEPROM.write(E2P_CRC8_LightV, e2p_crc8(E2P_LightValue_Low, 8));
        // CRC8 the CRC8s
        EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
        //        sendLightSetpoints();
       }
       else j = 98;
       break;
    // Bad command
     default:
       j = 0;
       break;
    }
  }
}
      

void sendRelayData() {
  int i;
  //RELAYS
    Serial.print("Relays");
    for (i = 0; i < SSRLIGHT; i++) {
      Serial.print(",");
      if (digitalRead(Relay_Pin[i]) == LOW){
        Serial.print(1);
      }
      else if (digitalRead(Relay_Pin[i]) == HIGH){
        Serial.print(0);
      }
    }
    //The SSRs are opposite
    for (i = SSRLIGHT; i < MAXRELAYS-2; i++) {
        Serial.print(",");
      if (digitalRead(Relay_Pin[i]) == LOW){
        Serial.print(0);
      }
      else if (digitalRead(Relay_Pin[i]) == HIGH){
        Serial.print(1);
      }
    }
    Serial.print("\n");
}

void sendRelaySetup() {
  int i;
  //SETUP

    //Relay_isAuto Values (Modes)
    Serial.print("Relay_isAuto");
    for (i = 0; i < MAXRELAYS-2; i++) {
      Serial.print(",");
      Serial.print(Relay_isAuto[i]);
    } 
    Serial.print("\n");
}


