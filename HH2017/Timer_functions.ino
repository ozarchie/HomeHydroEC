#include "Arduino.h"
/*
  // ACTION TIMER FUNCTIONS - LIGHTS AND PUMPS
 */
void CheckTimers(){
  CheckLightTimers();
  CheckPumpTimers();
}

//Check Light Timers
//
void CheckLightTimers() {
  //    Serial1.println();
  //    Serial1.print("Light Schedule Timer: ");
  //    Serial1.println();
  //    Serial1.print("hour: ");
  //    Serial1.print(hour());
  //    Serial1.println();
  //    Serial1.print("minute: ");
  //    Serial1.print(minute());
  //    Serial1.println();
  //    Serial1.print("Light_ON_hour: ");
  //    Serial1.print(Light_ON_hour);
  //    Serial1.println();
  //    Serial1.print("Light_ON_min: ");
  //    Serial1.print(Light_ON_min);
  //    Serial1.println();
  //    Serial1.print("Light_OFF_hour: ");
  //    Serial1.print(Light_OFF_hour);
  //    Serial1.println();
  //    Serial1.print("Light_OFF_min: ");
  //    Serial1.print(Light_OFF_min);
  //    Serial1.println();

  if (Relay_isAuto[SSRLIGHT] == 1){  
    int TurnLightOn = 2;                    //0: Turn Off  1: Turn On 2: Not sure, so OFF
    if (Light_ON_hour < Light_OFF_hour) {
      if (hour() >= Light_ON_hour && hour() <= Light_OFF_hour) {
        if (hour() == Light_OFF_hour && minute() < Light_OFF_min) {
          TurnLightOn = 1;
          goto exit_loop_light;          
        }
        if (hour() == Light_ON_hour && minute() >= Light_ON_min) {
          TurnLightOn = 1;
          goto exit_loop_light;          
        }
        if (hour() != Light_OFF_hour && hour() != Light_ON_hour && hour() > Light_ON_hour && hour() < Light_OFF_hour){
          TurnLightOn = 1;
          goto exit_loop_light;
        }
      }
    }

    if (Light_ON_hour > Light_OFF_hour) {
      if (hour() <= Light_OFF_hour || hour() >= Light_ON_hour) {
        if (hour() == Light_OFF_hour && minute() < Light_OFF_min) {
          TurnLightOn = 1;
          goto exit_loop_light;          
        }
        if (hour() == Light_ON_hour && minute() >= Light_ON_min) {
          TurnLightOn = 1;
          goto exit_loop_light;          
        }
        if (hour() != Light_OFF_hour && hour() != Light_ON_hour){
          if (hour() > Light_ON_hour && hour() < 24){
            TurnLightOn = 1;
            goto exit_loop_light;
          }
          if (hour() < Light_OFF_hour) {
            TurnLightOn = 1;
            goto exit_loop_light;
          }
        }
      }
    }

exit_loop_light:
  SwitchLight(TurnLightOn);
  }
}
  
void SwitchLight(int TurnLightOn){
    if (TurnLightOn == 1) {
        if (Relay_State[SSRLIGHT] == OFF) {
          Relay_State[SSRLIGHT] = ON;
          EEPROM.write(E2P_Relay_State+SSRLIGHT,ON);
          EEPROM.write(E2P_CRC8_RelayState, crc8((uint8_t*)Relay_State, MAXRELAYS*2));
          turnRelay(SSRLIGHT+1, ON);
        }
      }
      //    Serial1.println();
      //    Serial1.println("Turned Light ON");
      //    Serial1.println();
    else {
        if (Relay_State[SSRLIGHT] == ON) {
          Relay_State[SSRLIGHT] = OFF;
          EEPROM.write(E2P_Relay_State+SSRLIGHT,OFF);
          EEPROM.write(E2P_CRC8_RelayState, crc8((uint8_t*)Relay_State, MAXRELAYS*2));
          turnRelay((SSRLIGHT+1), OFF);
        }
      }
      //    Serial1.println();
      //    Serial1.println("Turned Light OFF");
      //    Serial1.println();
}

//Check Pump Timers
//
void CheckPumpTimers() {
  
  int Pump_hour_on;
  int Pump_min_on;
  int Pump_hour_off;
  int Pump_min_off;

  int TurnPumpOn = 2;  //0: Turn Off  1: Turn On  2: Unsure
  int i = 0;

  if (Relay_isAuto[WATERPUMP1] == 1) {

  //  Serial1.println();
  //  Serial1.print("Water Pump Timers: ");
  //  Serial1.println();
  
    for(i = 0;i < Pump_repeat;i++){
      Pump_hour_on = Pump_hour_array[i];
      Pump_min_on  = Pump_minute_array[i]; 
      Pump_min_off = Pump_min_on + Pump_for;

      if (Relay_State[WATERPUMP1] == OFF) {      // If the pump is OFF
        if (Pump_hour_on == hour() && minute() >= Pump_min_on ) {
          TurnPumpOn=1;
          SwitchPump(TurnPumpOn);
//          goto exit_loop_pump;
        }
      }
      if (Relay_State[WATERPUMP1] == ON) {      // If the pump is ON
         if (Pump_hour_on == hour() && minute() >= Pump_min_off ) {
          TurnPumpOn=0;
          SwitchPump(TurnPumpOn);
//          goto exit_loop_pump;
        }
      }
    }
      
  /*  
      if (Pump_isAM_array[i] == false && Pump_hour_on != 12){
        Pump_hour_on = Pump_hour_on + 12;
        //      Serial1.println("Added 12");
        //      Serial1.println(Pump_hour_on);
      }
  
      if (Pump_hour_on == 24){
        Pump_hour_on = 12;
        //      Serial1.println("Change 0 to 12");
      }
  
      Pump_hour_off = Pump_hour_on;
  
      if (Pump_min_on > 59) {
        Pump_min_on = 60 - Pump_min_on;
        Pump_hour_on++; 
      }
      if (Pump_hour_on > 23) {
        Pump_hour_on = Pump_hour_on - 24; 
      }
  
      if (Pump_min_off > 59) {
        Pump_min_off = Pump_min_off - 60;
        Pump_hour_off++; 
      }
      if (Pump_hour_off > 23) {
        Pump_hour_off = Pump_hour_off - 24; 
      }
  
      //    Serial1.print(i);
      //    Serial1.print(") ");
      //    Serial1.print("Hour: ");
      //    Serial1.print(hour());
      //    Serial1.print("   Minute: ");
      //    Serial1.print(minute());
      //    Serial1.print("   On Time: ");
      //    Serial1.print(Pump_hour_on);
      //    Serial1.print(":");
      //    Serial1.print(Pump_min_on);  
      //    Serial1.print("   Off Time: ");
      //    Serial1.print(Pump_hour_off);
      //    Serial1.print(":");
      //    Serial1.print(Pump_min_off);
      //    Serial1.print("   Is AM:");
      //    Serial1.println(Pump_isAM_array[i]);
      //    Serial1.println(); 
  
      
      // Turn On after 11.00 PM and turn off after 12.00 AM (midnight)
      if (Pump_hour_on == 23 && Pump_hour_off < Pump_hour_on) {
        
        if (Pump_hour_on == hour() && minute() >= Pump_min_on ) {
          TurnPumpOn=1;
          goto exit_loop_pump;
        }
        if (hour() < Pump_hour_off) {
          TurnPumpOn=1;
          goto exit_loop_pump;
        }
        if (hour() == Pump_hour_off) {
          if (minute() < Pump_min_off) {
            TurnPumpOn=1;
            goto exit_loop_pump;   
          }
        }
      }
  
      if (hour() == Pump_hour_on && hour() <= Pump_hour_off) {
        if (minute() >= Pump_min_on) {
          if (Pump_hour_off != Pump_hour_on && hour() == Pump_hour_off && minute() < Pump_min_off){
            TurnPumpOn=1;
            goto exit_loop_pump;          
          }
          if (Pump_hour_off != Pump_hour_on && hour() == Pump_hour_on && minute() >= Pump_min_on){
            TurnPumpOn=1;
            goto exit_loop_pump;          
          }
          if (Pump_hour_off == Pump_hour_on && minute() >= Pump_min_on && minute() < Pump_min_off){
            TurnPumpOn=1;
            goto exit_loop_pump;
          }
        }
  
      }
      
      if (hour() > Pump_hour_on && hour() < Pump_hour_off) {
        TurnPumpOn=1;
        goto exit_loop_pump;
      }
      
      if (hour() >= Pump_hour_on && hour() == Pump_hour_off) {
        if (Pump_hour_off != Pump_hour_on && hour() == Pump_hour_off && minute() < Pump_min_off){
          TurnPumpOn=1;
          goto exit_loop_pump;          
        }
        if (Pump_hour_off != Pump_hour_on && hour() == Pump_hour_on && minute() >= Pump_min_on){
          TurnPumpOn=1;
          goto exit_loop_pump;          
        }
        if (Pump_hour_off == Pump_hour_on && minute() >= Pump_min_on && minute() < Pump_min_off){
          TurnPumpOn=1;
          goto exit_loop_pump;
        }
      }
    }

  exit_loop_pump:
    SwitchPump(TurnPumpOn);
    */
}
}

void SwitchPump(int TurnPumpOn) {
  //  Serial1.print("TurnPumpOn: ");
  //  Serial1.println(TurnPumpOn);
  if (TurnPumpOn == ON) {
    if (Relay_State[WATERPUMP1] == OFF) {
      Relay_State[WATERPUMP1] = ON;
      EEPROM.write(E2P_Relay_State+WATERPUMP1,ON);
      EEPROM.write(E2P_CRC8_RelayState, crc8((uint8_t*)Relay_State, MAXRELAYS*2));
      turnRelay((WATERPUMP1+1), ON);
    }
    //        Serial1.println();
    //        Serial1.println("Turned Water Pump On");
    //        Serial1.println();
  }
  else if (TurnPumpOn == 0 || TurnPumpOn == 2) {
    if (Relay_State[WATERPUMP1] == ON){
      Relay_State[WATERPUMP1] = OFF;
      EEPROM.write(E2P_Relay_State+WATERPUMP1,OFF);
      EEPROM.write(E2P_CRC8_RelayState, crc8((uint8_t*)Relay_State, MAXRELAYS*2));
      turnRelay((WATERPUMP1+1), OFF);
    }
    //        Serial1.println();
    //        Serial1.println("Turned Water Pump Off");
    //        Serial1.println();
  }
}

// The Pump has an array of values derived from start, repeat type, and repeat number
// It affects only Relay1

void FillPumpTimesArrays(int Pump_start_hour, int Pump_start_min, int Pump_every_hours, int Pump_every_mins, int Pump_for, int Pump_repeat) {
  Pump_hour_array[0] = Pump_start_hour;
  Pump_minute_array[0] = Pump_start_minute;
  Pump_isAM_array[0] = Pump_start_isAM;

/*
  if (Pump_start_hour > 12) {
    Pump_isAM_array[0] = false;
  }
*/
  int i = 0;
  for(i = 0; i < Pump_repeat; i++){
    Pump_hour_array[i] = Pump_start_hour + ((i) * Pump_every_hours);
    Pump_minute_array[i] = Pump_start_minute + ((i) * Pump_every_mins);
    while (Pump_minute_array[i] > 59) {
      Pump_minute_array[i] = Pump_minute_array[i] - 60;
      Pump_hour_array[i]++;
    }

/*
    int AMPM_int = Pump_hour_array[i] / 12;
    while (Pump_hour_array[i] > 12) {
      Pump_hour_array[i] = Pump_hour_array[i] - 12;
    }
*/

/*
    Serial.println();
    Serial.print("Pump_start_isAM:");       
    Serial.print(Pump_start_isAM);       
    Serial.println();
    Serial.print(Pump_hour_array[i]);
    Serial.print(") ");
    Serial.print(AMPM_int);
    Serial.println();
*/

/*
    boolean case_zero = false;
    switch (AMPM_int) {
    case 0:
      case_zero = true;
      if (Pump_start_isAM == true) {
        Pump_isAM_array[i] = true;
      } 
      else {
        Pump_isAM_array[i] = false;
      }
      break;
    case 1:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      }
      break;
    case 2:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = true;
        } 
        else {
          Pump_isAM_array[i] = false;
        }
      }
      break;
    case 3:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = true;
        } 
        else {
          Pump_isAM_array[i] = false;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      }
      break;
    case 4:
      if (case_zero == true){
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = false;
        } 
        else {
          Pump_isAM_array[i] = true;
        }
      } 
      else {
        if (Pump_start_isAM == true) {
          Pump_isAM_array[i] = true;
        } 
        else {
          Pump_isAM_array[i] = false;
        }
      }
      break;
    }
    */
  }
}



