void updateMenu(unsigned char instruction)
{
  switch (instruction)
  {
    case MENU_INIT:
      state = sTime;
      timerStart();
      while ( backFlag != true)
      {
        displayTime();
        if (secondTMR >= 5)
        {
          //Serial.println("Sleeping mode enter");
          delay(20);
          timerReset();
          timerStop();
          deepSleep();
        }
      }
      state = sActivity;
      displayMenu();
      backFlag = false;
      break;

    case MENU_UP:
      switch (state)
      {
        case sActivity:       state = sSettings; displayMenu(); break;
        case sSettings:       state = sActivity; displayMenu();  break;
        case sRunning:        state = sWalking;  displayMenu(); break;
        case sWalking:        state = sCycling;  displayMenu(); break;
        case sCycling:        state = sRopeJump; displayMenu();  break;
        case sRopeJump:       state = sSwimming; displayMenu(); break;
        case sSwimming:       state = sTennis;   displayMenu(); break;
        case sTennis:         state = sTeamSport; displayMenu(); break;
        case sTeamSport:      state = sOther; displayMenu(); break;
        case sOther:          state = sRunning; displayMenu(); break;
        case sSetTimeData:    state = sSetPersonal; displayMenu(); break;
        case sAM:             state = sPM;   displayMenu();  break;
        case sPM:             state = sAM;   displayMenu(); break;
        case sSetPersonal:    state = sSync; displayMenu(); break;
        case sSync:           state = sSetTimeData; displayMenu();  break;
      }
      break;

    case MENU_SELECT:
      switch (state)
      {
        case sTime:           state = sActivity;  displayMenu(); break;
        case sActivity:       state = sRunning; displayMenu(); break;
        case sSettings:       state = sSetPersonal; displayMenu(); break;
        case sRunning:        workoutRecording(sRunning);   displayMenu(); break;
        case sWalking:        workoutRecording(sWalking);   displayMenu(); break;
        case sCycling:        workoutRecording(sCycling);   displayMenu(); break;
        case sRopeJump:       workoutRecording(sRopeJump);  displayMenu(); break;
        case sSwimming:       workoutRecording(sSwimming);  displayMenu(); break;
        case sTennis:         workoutRecording(sTennis);    displayMenu(); break;
        case sTeamSport:      workoutRecording(sTeamSport); displayMenu(); break;
        case sOther:          workoutRecording(sOther);     displayMenu(); break;
        case sSetTimeData:    state = sAM; displayMenu();  break;
        case sAM:             state = sSettings; displayMenu(); break;
        case sPM:             state = sSettings; displayMenu(); break;
        case sSetPersonal:    dispPersonal(); break;
        case sSync:           state = sSyncing; displayMenu(); syncWithPC();  break;
      }
      break;

    case MENU_BACK:
      backFlag = false;
      switch (state)
      {
        case sActivity:       state = sTime; displayMenu(); break;
        case sSettings:       state = sTime; displayMenu(); break;
        case sRunning:        state = sActivity; displayMenu(); break;
        case sWalking:        state = sActivity; displayMenu(); break;
        case sCycling:        state = sActivity; displayMenu(); break;
        case sRopeJump:       state = sActivity; displayMenu(); break;
        case sSwimming:       state = sActivity; displayMenu(); break;
        case sTennis:         state = sActivity; displayMenu(); break;
        case sTeamSport:      state = sActivity; displayMenu(); break;
        case sOther:          state = sActivity; displayMenu(); break;
        case sSetTimeData:    state = sSettings; displayMenu(); break;
        case sChooseFormat:   state = sSettings; displayMenu(); break;
        case sAM:             state = sSetTimeData; displayMenu(); break;
        case sPM:             state = sSetTimeData; displayMenu(); break;
        case sSetPersonal:    state = sSettings; displayMenu();  break;
        case sDispPersonal:   state = sSettings; displayMenu();  break;
        case sSync:           state = sSettings; displayMenu();  break;
        case sSyncing:        state = sSettings; displayMenu(); break;
          //case sTime:           deepSleep(); break;
      }
      break;
  }
}

