void dLogSt(int wkType)  // Save data with steps
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  wkType = wkType - 2;
  readDS3232time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  wkCounter++;
  storeWorkout((uint8_t)wkType, (uint16_t)secondTMR, (uint8_t)dayOfMonth, (uint8_t)month, (uint16_t)stepCount);
  eeprom_write_word(&Ep.wkCount_backup, wkCounter);
  eeprom_busy_wait();
  stepCount = 0; // reset steps
  secondTMR = 0; // reset timer
}

void workoutWithSteps(unsigned int wkType)
{
  boolean buttonPressed = false;
  stepCountON();
  timerStart();

  while(1)
  {
    if(backFlag == true)
    {
      break;
    }
  }
  //Serial.println("Cycle exit");
  backFlag = false;
  timerStop();
  stepCountOFF();
  //buttonPressed = false;
  
  dLogSt(wkType);
  
  //Serial.println("Saved");
  //Serial.println("Exit");
}

void workoutWOSteps(unsigned int wkType)
{
 boolean buttonPressed = false;
  timerStart();

  while(1)
  {
    if(backFlag == true)
    {
      break;
    }
  }
  //Serial.println("Cycle exit");
  backFlag = false;
  timerStop();
  //buttonPressed = false;
  
  dLogSt(wkType);
}

void workoutRecording(unsigned int wkType)
{
  oled.command(0xAE); // display OFF
  timerStop();
  timerReset();
  state = sRecording;
  
  switch(wkType)
  {
    case 3  : 
    case 4  :
    case 5  :
    case 6  :
      workoutWithSteps(wkType);
      state = wkType;
      break;
    case 7  :
    case 8  :
    case 9  :
    case 10 :
    case 11 :
      workoutWOSteps(wkType);
      state = wkType;
      break;
  }
  oled.command(0xAF); // display ON
}

