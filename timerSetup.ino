void timerStart()
{
  //PRR |= (0 << PRTIM1); // Timer 1 power up ! used for seconds
  TIMSK1 = 0x02;
  OCR1A = 499;
}

void timerStop()
{
  //PRR |= (1 << PRTIM1); // Timer 1 power down ! used for seconds
  OCR1A = 0x00;
  TIMSK1 = 0x00;
}

void timerReset()
{
  countTMR = 0;
  secondTMR = 0;
}

void stepCountON()
{
  EIMSK = 0x03;; // INT0 disabled; INT 1 enabled
}

void stepCountOFF()
{
  EIMSK = 0x02; // INT0 disabled; INT 1 enabled
}

