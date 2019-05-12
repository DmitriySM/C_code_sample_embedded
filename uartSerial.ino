void USARTWriteStr(char *data)
{
  int i = 0;
  while ( data[i] != '\n' ) {
    USARTWriteChar( data[i] );
    i++;
  }
}

void USARTWriteChar(char data)
{
  UDR0 = data; // Echo back the received byte back to the computer
  while ((UCSR0A & (1 << UDRE0)) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
}

void USART_transmit(char* str)
{
  txStr = str;
  UCSR0B |= ( 0 << RXCIE0) | ( 0 << TXCIE0)| ( 1 << UDRIE0);
}

ISR(USART_RX_vect)
{
  in_char = 1;
  ReceivedChar = UDR0;
}

void UART_Flush(void)
{
  unsigned char dummy;
  while ( UCSR0A & ( 1 << RXC0 )) dummy = UDR0;
}

// no need
void UART_Read(char* c_str)
{int i = 0;
  while (1)
  {
    if (in_char)
    {
      in_char = 0;
      if ( ReceivedChar != '\n')
      {
        c_str[i] = ReceivedChar;
        i++;
      } else

      {
        UCSR0B |= ( 0 << RXCIE0) | ( 0 << TXCIE0);
        break;
      }
    }
  }
}


ISR(USART_UDRE_vect)
{
  static uint8_t txIndex = 0;

  UDR0 = txStr[txIndex];
  txIndex++;

  if (txStr[txIndex] == 0)
  {
    txIndex = 0;
    UCSR0B &= ~(1 << UDRIE0);
  }
}
