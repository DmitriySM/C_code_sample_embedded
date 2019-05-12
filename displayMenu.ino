void displayMenu(void)
{
  oled.setCursor(0,0);
        switch (state)
      {
        case sTime:         updateMenu(MENU_INIT); break;
        case sActivity:     oled.clear(PAGE); oled.print(F("Activity")); oled.display(); break;
        case sSettings:     oled.clear(PAGE); oled.print(F("Settings")); oled.display(); break;
        case sRunning:      oled.clear(PAGE); oled.print(F("Running")); oled.display(); break;
        case sWalking:      oled.clear(PAGE); oled.print(F("Walking")); oled.display(); break;
        case sCycling:      oled.clear(PAGE); oled.print(F("Cycling")); oled.display(); break;
        case sRopeJump:     oled.clear(PAGE); oled.print(F("Rope Jump")); oled.display(); break;
        case sSwimming:     oled.clear(PAGE); oled.print(F("Swimming")); oled.display(); break;
        case sTennis:       oled.clear(PAGE); oled.print(F("Tennis")); oled.display(); break;
        case sTeamSport:    oled.clear(PAGE); oled.print(F("Team Sports")); oled.display(); break;
        case sOther:        oled.clear(PAGE); oled.print(F("Other")); oled.display(); break;
        case sSetTimeData:  oled.clear(PAGE); oled.print(F("12h/24h")); oled.display(); break;
        case sAM:           set12h(); oled.clear(PAGE); oled.print(F(" -= 12h =- ")); oled.display(); break;
        case sPM:           set24h(); oled.clear(PAGE); oled.print(F(" -= 24h =- ")); oled.display(); break;
        case sSetPersonal:  oled.clear(PAGE); oled.print(F("Personal")); oled.display(); break;
        case sSync:         oled.clear(PAGE); oled.print(F("-= Sync =-")); oled.display(); break;
        case sSyncing:      oled.clear(PAGE); oled.print(F("In process")); oled.display(); break;
      }
}

void dispPersonal()
{
  char uname[20];
  char docDate[11];
  char weight[5];
  char heartRate[5];
  memset(weight,0,sizeof(weight));
  memset(heartRate,0,sizeof(heartRate));
  eeprom_read_block(docDate, &Ep.doctor_Date,11);
  eeprom_busy_wait();
  eeprom_read_block(weight,&Ep.user_weight,4);
  eeprom_busy_wait();
  eeprom_read_block(heartRate,&Ep.user_heartRate,4);
  eeprom_busy_wait();
  eeprom_read_block(uname,&Ep.user_name,10);
  eeprom_busy_wait();

  oled.clear(PAGE);
  oled.setCursor(0,0);
  oled.print(uname);
  oled.print("\n");
  oled.print(weight);
  oled.print("\n");
  oled.print(heartRate);
  oled.print("\n");
  oled.print(docDate);
  oled.display();
}

