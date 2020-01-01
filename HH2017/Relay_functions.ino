/*
  SET RELAYS
 */
void turnRelay(int Relay_num, int TurnOnOff){

  if (Relay_num < SSRLIGHT+1) {
    //Relay Board works opposite (Low/High) (Relays 1 - 4)
    if (TurnOnOff == 1) digitalWrite(Relay_Pin[Relay_num - 1], LOW);
    else if (TurnOnOff == 0) digitalWrite(Relay_Pin[Relay_num - 1], HIGH);      
  } 
  else if (Relay_num >= SSRLIGHT+1) {
    //SSR relays work as expected (High/Low) (Relay 5, 6)
    if (TurnOnOff == 1) digitalWrite(Relay_Pin[Relay_num - 1], HIGH);
    else if (TurnOnOff == 0) digitalWrite(Relay_Pin[Relay_num - 1], LOW);      
  }
//  sendRelayMessages();
}

int SetRelay(int RelayNumber, float Value, float ValueLow, float ValueHigh) {
  int i = RelayNumber+1;
  if ( Relay_isAuto[RelayNumber] == ON) {
//    Serial.println(RelayNumber);
//    Serial.println(Value);
//    Serial.println(ValueLow);
//    Serial.println(ValueHigh);
//    Serial.println(Relay_State[RelayNumber]);
    if (Relay_State[RelayNumber] == OFF) {   // If it is OFF , then
      if (Value < ValueLow) {      //  check if it should be ON
        turnRelay(RelayNumber+1, ON);
        Relay_State[RelayNumber] = ON;
        EEPROM.write((E2P_Relay_State+RelayNumber),ON);
        EEPROM.write(E2P_CRC8_RelayState, e2p_crc8(E2P_Relay_State, MAXRELAYS*2));
        // CRC8 the CRC8s
        EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
//    dumpcrc();
        i = 99;
//    Serial.println("Turn ON");
      }
    }
    if (Relay_State[RelayNumber] == ON) {  // If it is ON
      if (Value >= ValueHigh) {       //  then should it be turned OFF
        turnRelay(RelayNumber+1, OFF);
        Relay_State[RelayNumber] = OFF;
        EEPROM.write((E2P_Relay_State+RelayNumber),OFF);
        EEPROM.write(E2P_CRC8_RelayState, e2p_crc8(E2P_Relay_State, MAXRELAYS*2));
        // CRC8 the CRC8s
        EEPROM.write(E2P_CRC8, e2p_crc8(E2P_CRC8_RelayState, (E2P_CRC8_Pump-E2P_CRC8)));
//  dumpcrc();
        i = 99;
//    Serial.println("Turn OFF");       
      }
    }
                                  // Here, i = (RelayNum+1) or 99
    if (i == 99) return(1);       // Setpoint change (ON/OFF) for this relay?
    else return(0);               // No change for this relay
  }
  return(0);                      // This relay is NOT auto so no change
}



