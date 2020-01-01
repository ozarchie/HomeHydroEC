/*
 UPDATE LONG DATE
 */

  //COMBINED LONG FORMAT DATE
  // char longdate[25] = "Nov  1, 2016 12:00:00 AM";
  // updates char longdate[22];
  
  void updatelongdate() {
  
  char month_array[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
  uint8_t tempB;
  int tempI;
  char buffer[5];

  //MONTH
  tempB = month();
  strcpy(buffer,month_array[tempB-1]);
  strcpy (longdate,buffer);
  strcat (longdate," ");

  //DAY
  tempB = day();
  uint8ToStr(buffer, tempB, 2, 0);
  strcat (longdate,buffer);
  strcat (longdate," ");
  
  //YEAR
  tempI = year();
  UIToStr(buffer, tempI, 4, 0);
  strcat (longdate,buffer);
  strcat (longdate," ");
  
  //HOUR
  tempB = hour();
  if (tempB > 12) {
    tempB -= 12; 
  }
  uint8ToStr(buffer, tempB, 2, 0);
  strcat (longdate,buffer);
  strcat (longdate,":");
  
  //MINUTE
  tempB = minute();
  uint8ToStr(buffer, tempB, 2, 0);
  strcat (longdate,buffer);
  strcat (longdate,":");
  
  //SECOND
  tempB = second();
  uint8ToStr(buffer, tempB, 2, 0);
  strcat (longdate,buffer);
  strcat (longdate," ");
  
  // AM/PM
  if (isAM() ==true){
    strcat(longdate, "AM");
  } 
  else if (isAM()==false) {
    strcat(longdate, "PM");
  }
}


