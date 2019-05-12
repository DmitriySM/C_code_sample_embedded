#include <Wire.h>  // Include Wire if you're using I2C
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 


#define bit_get(p,m) ((p) & (m)) 
#define bit_set(p,m) ((p) |= (m)) 
#define bit_clr(p,m) ((p) &= ~(m)) 
#define bit_flip(p,m) ((p) ^= (m)) 
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 
#define BIT(x) (0x01 << (x))

#define EEPROM __attribute__((section(".eeprom")))
//////////////////////////
// MicroOLED Definition //
//////////////////////////
#define PIN_RESET 4  // Connect RST to pin 9
#define PIN_DC    5  // Connect DC to pin 8
#define PIN_CS    10 // Connect CS to pin 10
///////////////////// RTC ADDRESS
#define DS3232_I2C_ADDRESS 0x68
//

MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // SPI declaration

#define MENU_INIT   0
#define MENU_UP     1
#define MENU_SELECT 2
#define MENU_BACK   3
////////////// Define variables ///////
#define SERIAL_TIMEOUT 23
#define WEIGHT_SIZE 4
#define HERATE_SIZE 4
#define HEIGHT_SIZE 4
#define DOCDATE_SIZE 11
#define UNAME_SIZE 20
////////////// STRUCTURE FOR EEPROM ///////////////////////////
#define EEPROM_END 1024

typedef struct
{
  uint32_t eepromWkData[55][2];
  char user_name[6];
  char user_height[4], user_weight[4], user_heartRate[4];
  char doctor_Date[11];
  uint16_t wkCount_backup;
}
struct_eepvar;

struct_eepvar EEPROM Ep; // declare eeprom variable

///////////////////////////////////////////////////////////////

enum menus {sTime = 0, sActivity, sSettings, sRunning, sWalking, sCycling, sRopeJump, sSwimming, sTennis, sTeamSport, sOther,
            sSetTimeData,
            sSetPersonal, sSync, sSyncing,
            sDispPersonal,
            sChooseFormat,
            sAM,
            sPM,
            sRecording
           };
unsigned char state = sTime; // initial state is time
unsigned char old_state;
unsigned int stepCount = 0;
unsigned int secondTMR, countTMR;
short unsigned int correctionCounter;
volatile bool backFlag;
uint16_t wkCounter;
unsigned char button;
//////// SERIAL EVENT VARIABLES
volatile uint8_t ReceivedChar;
volatile bool in_char = 0;

String inputString = "";
boolean stringComplete = false;
// conversion variable
char* dataStr;
volatile char* txStr;
////////

void (*displayTime)(void);

void setup() {

  DDRD = 0x00;
  SREG |= (1 << 7); // global interrupt enabled
  DDRB = 0xFF;
  Wire.begin();
  //setDS3232time(00, 53, 16, 5, 4, 2, 16);   // set DS3232 seconds, minutes, hours, day of week, date, month, year
  EICRA = 0b00001111; //INT1 (button) falling edge interrupt, INT0 rising edge interrupt
  EIMSK = 0b00000010; // INT0 disabled; INT 1 enabled
  // Timer1 settings (workout duration)
  TCCR1A = 0x00; // CTC Normal port operation
  TCCR1B = 0b00001010; // prescaler 8 , 8ms per count

  oled.begin();
  // clear(ALL) will clear out the OLED's graphic memory.
  // clear(PAGE) will clear the Arduino's display buffer.
  oled.clear(ALL);  // Clear the display's memory (gets rid of artifacts)
  oled.clear(PAGE);
  // To actually draw anything on the display, you must call the
  // display() function.
  oled.display();
  oled.setFontType(0);
  oled.setCursor(0, 0);
  displayTime = &displayTime12h;
  updateMenu(MENU_INIT);
  ///wkCounter = 0;
  inputString.reserve(11);
  old_state = state;
  dataStr = (char*)malloc(6);
  //
  wkRestore();
  pwrDwn();
  WDT_off();
  
  state = sTime;
  displayMenu();
  timerStart();
  // serial communication enable
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | ( 1 << RXCIE0) | ( 0 << TXCIE0);
  UCSR0C |= (1 << USBS0) | ( 1 << UCSZ00);

  UBRR0H = ( BAUD_PRESCALE >> 8 );
  UBRR0L = BAUD_PRESCALE;

  sei();
}

void wkRestore()
{
  uint16_t wkCounter_d = eeprom_read_word(&Ep.wkCount_backup);
  eeprom_busy_wait();
  uint8_t b[2];
  b[0] = (uint8_t) (wkCounter_d >>  0u);
  b[1] = (uint8_t) (wkCounter_d >>  8u);
  wkCounter = b[0] | b[1];
  if(wkCounter == 255)
  {
    wkCounter = 0;
    eeprom_write_word(&Ep.wkCount_backup, wkCounter);
    eeprom_busy_wait();
  }
}

void loop() {
  button = (~PIND & 0xC8); // excludes unused bits
  delay(200);
  switch (button)
  {
    case ( 1 << PD3 ) : updateMenu(MENU_BACK);    break;
    case ( 1 << PD6 ) : updateMenu(MENU_UP);      break;
    case ( 1 << PD7 ) : updateMenu(MENU_SELECT);  break;
  }

  if ( state <= 3 )
  {
    timerStart();
    // Serial.println("Timer sleep:");
    // Serial.println(secondTMR);

    if (old_state != state)
    {
      old_state = state;
      //  timerStop();
      timerReset();
      //  timerStart();
    } else {

      if (secondTMR >= 15)
      {
        // Serial.println("Sleeping mode enter");
        timerReset();
        timerStop();
        deepSleep();
      }
      sleep_disable();               //wake up here
      oled.command(0xAF); // Display ON
      displayMenu();
    }
  }

}

void pwrDwn()
{
  PRR |= ( (1 << PRTIM2)); // timer 2 shut down - not in use
  ACSR | (1 << ACD); // Analog comparator disabled - not in use
  WDT_off(); // WDT off not in use
  PRR |= (1 << PRADC); // Power reduction ADC - not in use
}

void WDT_off(void)
{
  cli();
  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1 << WDRF);
  /* Write logical one to WDCE and WDE */
  /* Keep old prescaler setting to prevent unintentional
  time-out */
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  /* Turn off WDT */
  WDTCSR = 0x00;
  // enable interrupts
  sei();
}

void deepSleep()
{
  oled.command(0xAE); // display OFF
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_cpu();                   //go to sleep
}

void EraseEEPROM()
{
  uint8_t sreg, i;
  uint16_t addr;
  //uint8_t clr[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; // write FF
  uint8_t clr[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 
  uint8_t data[8];

  sreg = SREG;
  cli();
  //
  for(addr = 0; addr < EEPROM_END; addr += 8)
  {
    eeprom_read_block((void*)&data[0],(const void*)addr,8);
    for(i = 0; i < 8; i++)
      //if(data[i] != 0xFF) // write 0xFF
      if(data[i] != 0x00)
      {
        eeprom_update_block((void*)&clr[0],(void*)addr,8);
        break;
      }
  }

  SREG = sreg;
}
