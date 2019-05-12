void displayTime24h()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
  // retrieve data from DS3232
  readDS3232time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  oled.clear(PAGE);
  oled.setCursor(0,0);
  // send it to the serial monitor
  oled.print(hour, DEC);// convert the byte variable to a decimal number when being displayed
  oled.print(":");
  if (minute < 10)
  {
    oled.print("0");
  }
  oled.print(minute, DEC);
  oled.print(":");
  if (second < 10)
  {
    oled.print("0");
  }
  oled.print(second, DEC);
  oled.print("  ");
  oled.print(dayOfMonth, DEC);
  oled.print("/");
  oled.print(month, DEC);
  oled.print("/");
  oled.print(year, DEC);
  oled.print("\n");
  switch (dayOfWeek) {
    case 1:
      oled.println(F("Sunday"));
      break;
    case 2:
      oled.println(F("Monday"));
      break;
    case 3:
      oled.println(F("Tuesday"));
      break;
    case 4:
      oled.println(F("Wednesday"));
      break;
    case 5:
      oled.println(F("Thursday"));
      break;
    case 6:
      oled.println(F("Friday"));
      break;
    case 7:
      oled.println(F("Saturday"));
      break;
  }
  oled.print(F("Workouts \n done: "));
  oled.print(wkCounter);
  oled.display();
}


void displayTime12h()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  char AM = 'A';
  char PM = 'P';
  char AMPM;
  // retrieve data from DS3232
  readDS3232time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  if(hour > 12)
  {
    hour = hour - 12;
    AMPM = PM;
  }else
    if(hour<12)
      {
        hour = hour;
        AMPM = AM;
      }
  
  oled.clear(PAGE);
  oled.setCursor(0,0);
  // send it to the serial monitor
  oled.print(hour, DEC);// convert the byte variable to a decimal number when being displayed
  oled.print(":");
  if (minute < 10)
  {
    oled.print("0");
  }
  oled.print(minute, DEC);
  oled.print(":");
  if (second < 10)
  {
    oled.print("0");
  }
  oled.print(second, DEC);
  oled.print(AMPM);
  oled.print("  ");
  oled.print(dayOfMonth, DEC);
  oled.print("/");
  oled.print(month, DEC);
  oled.print("/");
  oled.print(year, DEC);
  oled.print("\n");
  switch (dayOfWeek) {
       case 1:
      oled.println(F("Sunday"));
      break;
    case 2:
      oled.println(F("Monday"));
      break;
    case 3:
      oled.println(F("Tuesday"));
      break;
    case 4:
      oled.println(F("Wednesday"));
      break;
    case 5:
      oled.println(F("Thursday"));
      break;
    case 6:
      oled.println(F("Friday"));
      break;
    case 7:
      oled.println(F("Saturday"));
      break;
  }
  oled.print(F("Workouts \n done: "));
  oled.print(wkCounter);
  oled.display();
}

void set12h()
{
  //Serial.print(F("12 h format enabled"));
  displayTime = &displayTime12h;
}

void set24h()
{
  //Serial.print(F("24 h format enabled"));
  displayTime = &displayTime24h;
}

