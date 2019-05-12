ISR (INT0_vect)
{
  stepCount++;
  //Serial.println(F(" step : "));
  //Serial.println(stepCount);
  //Serial.println("\n");
}

ISR (INT1_vect)
{
  backFlag = true;
  delay(50);
 // Serial.println(backFlag);
  //Serial.println(F("Back"));
}

ISR (TIMER1_COMPA_vect)
{
  OCR1A = OCR1A + 499;
  countTMR++;
  if (countTMR > 62)
  {
    countTMR = 0;
    correctionCounter++;
    if (correctionCounter > 39)
    {
    secondTMR += 2;
    correctionCounter = 0;
    } else
    secondTMR++;
    //Serial.print(secondTMR);
    //Serial.print("\n");
  }
}

