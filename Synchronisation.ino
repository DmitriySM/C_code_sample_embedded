char buffer[100];
int servo1pos;

void syncWithPC()
{
  boolean communicationComplete = false;
  char str[100];
  int i = 0;
  while (!backFlag)
  {
    while (ReceivedChar != '\n') {
      if ( backFlag == true) break;

      if (in_char)
      {
        in_char = 0;
        str[i] = ReceivedChar;
        i++;
      }
    }

    UCSR0B |= ( 0 << RXCIE0) | ( 0 << TXCIE0); // Disable TX RX interrupts

    if (backFlag == true)
    {
      backFlag == false;
      state = sTime;
      displayMenu();
      return;
    }
    //Serial.println(readString); //see what was received

    String readString = String(str);
    int commaIndex = readString.indexOf(' ');

    String firstValue = readString.substring(0, commaIndex);
    servo1pos = firstValue.toInt();

    switch (servo1pos) {
      case 150: /*Serial.println(F("Set personal data..."));*/ delay(30); set_personal(readString); break;
      case 254: /*Serial.println(F("Set time")); */set_time(readString); break;
      case 222: readString = ""; /*Serial.println(F("erase memory"));*/ eraseMemory(); break;
      case 133:
        {
          //Serial.println("Fetching data");
          delay(25);
          fetchingData();
          readString = "";
          state = sTime;
          return;
        }
      case 111: readString = ""; /*Serial.println(F("Return!"));*/ readString = ""; state = sTime; displayMenu(); return;
    }
    readString = "";
    UCSR0B |= ( 1 << RXCIE0) | ( 1 << TXCIE0); // Disable TX RX interrupts
  }


//очистим строку

communicationComplete = false;
state = sTime;
return;
}
// Serial.end();
//}

void set_time(String str)
{
  int hours, minutes, seconds;
  char hours_c[5];
  char minutes_c[5];
  char seconds_c[5];
  char dayOfWeek_c[5];
  char day_c[5];
  char month_c[5];
  char year_c[5];

  int count_time = 0;
  str.toCharArray(buffer, 30);
  char* command = strtok(buffer, ",");
  char* separator = strchr(command, ':');
  char* token;
  token = strtok(separator, ":");
  while (token != 0)
  {
    switch (count_time)
    {
      case 0: strncpy(hours_c, token, 5); break;
      case 1: strncpy(minutes_c, token, 5); break;
      case 2: strncpy(seconds_c, token, 5); break;
      case 3: strncpy(dayOfWeek_c, token, 5); break;
      case 4: strncpy(day_c, token, 5); break;
      case 5: strncpy(month_c, token, 5); break;
      case 6: strncpy(year_c, token, 5); break;
    }
    ++count_time;
    token = strtok(NULL, ":");
  }

  setDS3232time(atoi(seconds_c), atoi(minutes_c), atoi(hours_c), atoi(dayOfWeek_c), atoi(day_c), atoi(month_c), atoi(year_c));

  delay(5);

  //queue_display();

}

void set_personal(String str)
{
  struct
  {
    char uName[6];
    char uWeight[5];
    char uHeight[5];
    char uHR[5];
    char uDocDate[11];
  } personal_data_struct;

  str.toCharArray(buffer, 100);
  //Serial.println(buffer); // printing buffer
  int count_personal = 0;
  char* command = strtok(buffer, ",");
  //char uName[20], uWeight[5], uHR[5], uDocDate[11];
  char* separator = strchr(command, ':');
  char* token;
  token = strtok(separator, ":");
  while (token != 0)
  {
    switch (count_personal) {
      case 0: strcpy(personal_data_struct.uName, token); break;
      case 1: strcpy(personal_data_struct.uHeight, token); break;
      case 2: strcpy(personal_data_struct.uWeight, token); break;
      case 3: strcpy(personal_data_struct.uHR, token); break;
      case 4: strcpy(personal_data_struct.uDocDate, token); break;
    }
    ++count_personal;
    token = strtok(NULL, ":");
  }

  eeprom_write_block(personal_data_struct.uName, &Ep.user_name, sizeof(personal_data_struct.uName));
  eeprom_busy_wait();
  eeprom_write_block(personal_data_struct.uHR,     &Ep.user_heartRate, 4);
  eeprom_busy_wait();
  eeprom_write_block(personal_data_struct.uDocDate,  &Ep.doctor_Date , sizeof(personal_data_struct.uDocDate));
  eeprom_busy_wait();
  eeprom_write_block(personal_data_struct.uWeight, &Ep.user_weight, 4);
  eeprom_busy_wait();
  eeprom_write_block(personal_data_struct.uHeight, &Ep.user_height, 4);
  eeprom_busy_wait();
  //Serial.println(F("Personal data is set"));
  delay(30);
}

void  eraseMemory()
{
  EraseEEPROM();
  wkCounter = 0;
  eeprom_write_word(&Ep.wkCount_backup, wkCounter);
  eeprom_busy_wait();
}
