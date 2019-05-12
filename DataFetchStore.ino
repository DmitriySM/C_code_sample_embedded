void storeWorkout(unsigned int type, uint16_t duration, uint8_t day, uint8_t month, uint16_t steps)
{
  uint16_t storage1(0);
  uint32_t storage2(0);
  // storage 1 - type, duration
  steps = steps / 10; // dividing by ten to save space.
  storage1 = (type);
  storage1 = (storage1 << 9) | duration;
  // sorage 2 - day, month, steps
  storage2 = (storage2)       | day;
  storage2 = (storage2 << 4)  | month;
  storage2 = (storage2 << 16) | steps;
  // write values to EEPROM
  uint32_t val1, val2;
  val1 = storage1;
  val2 = storage2;
  eeprom_write_dword(&Ep.eepromWkData[wkCounter][1], val1);
  eeprom_busy_wait();
  eeprom_write_dword(&Ep.eepromWkData[wkCounter][2], val2);
  eeprom_busy_wait();
}

void printWorkout(uint8_t wkC, uint32_t val1, uint32_t val2)
{
  char line_n[35];
  uint8_t v1 = ( val1 ) & 0xff ;
  uint8_t v2 = ( val1 >> 8 ) & 0xff ;
  uint8_t v4 = ( val2 ) & 0xff ;
  uint8_t v5 = ( val2 >> 8 ) & 0xff ;
  uint8_t v6 = ( val2 >> 16 ) & 0xff ;
  uint8_t v7 = ( val2 >> 24 ) & 0xff ;
  //sprintf(line_n,"0x0A,%d-%d-%d-%d-%d-%d-%d\0\r\n", wkC, v1, v2, v4, v5, v6, v7);
  //Serial.println(line_n);
  sprintf(line_n,"0x0A,%d-%d-%d-%d-%d-%d-%d\r\n\0", wkC, v1, v2, v4, v5, v6, v7);
  USART_transmit(line_n);
  delay(500);
  memset(line_n, 0 , sizeof(line_n));
}

void fetchingData()
{
  char dataRetrieved[30];
  if (wkCounter == 0 ) {
    //Serial.print(F("no data to fetch"));
    //Serial.print("\n");
  }
  else
  {
    char msg[20];
    char pers_identifier[] = "0xAA,";
    char endl[] = "\r\n";
    char uname[7], uweight[5],uheight[5], uhr[5], udocdate[12];
    
    memset(msg,0,sizeof(msg));
    eeprom_read_block(uname, &Ep.user_name, sizeof(uname) + 1);
    eeprom_busy_wait();
    delay(30);
    memcpy(msg, pers_identifier, sizeof(pers_identifier));
    memcpy(msg + strlen(msg), uname, sizeof(uname));
    memcpy(msg + strlen(msg), endl, sizeof(endl));
    USART_transmit(msg);
    /*
    memset(msg,0,sizeof(msg));
    eeprom_read_block(uweight, &Ep.user_weight, sizeof(uweight) + 1);
    eeprom_busy_wait();
    delay(30);
    memcpy(msg, pers_identifier, sizeof(pers_identifier));
    memcpy(msg + strlen(msg), uweight, sizeof(uweight));
    memcpy(msg + strlen(msg), endl, sizeof(endl));
    USART_transmit(msg);

    memset(msg,0,sizeof(msg));
    eeprom_read_block(uheight, &Ep.user_height, sizeof(uheight) + 1);
    eeprom_busy_wait();
    delay(30);
    memcpy(msg, pers_identifier, sizeof(pers_identifier));
    memcpy(msg + strlen(msg), uheight, sizeof(uheight));
    memcpy(msg + strlen(msg), endl, sizeof(endl));
    USART_transmit(msg);

    memset(msg,0,sizeof(msg));
    eeprom_read_block(uhr, &Ep.user_heartRate, sizeof(uhr) + 1);
    eeprom_busy_wait();
    delay(30);
    memcpy(msg, pers_identifier, sizeof(pers_identifier));
    memcpy(msg + strlen(msg), uhr, sizeof(uhr));
    memcpy(msg + strlen(msg), endl, sizeof(endl));
    USART_transmit(msg);
    */
    uint32_t val1, val2;
    uint8_t wkC = 0;
   for ( int i = 0; i < (wkCounter); i++ )
    {
      wkC = i + 1;
      val1 = eeprom_read_dword(&Ep.eepromWkData[i][1]);
      eeprom_busy_wait();
      val2 = eeprom_read_dword(&Ep.eepromWkData[i][2]);
      eeprom_busy_wait();
      delay(30);
      printWorkout(wkC,val1, val2);
    }
  char eom[] = "EOM\r\n";
  USART_transmit(eom);
  delay(30);
  return;
  }
  
}



