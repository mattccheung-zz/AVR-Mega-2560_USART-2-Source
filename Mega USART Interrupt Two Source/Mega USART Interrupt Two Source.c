/*
 * Mega_USART_Interrupt_Two_Source.c
 *
 * Created: 4/14/2015 2:41:03 PM
 *  Author: Chiuyin
 */ 

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_CPU 16000000
#define USART_BAUDRATE 38400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

//This function is used to initialize the USART
//at a given UBRR value
void USARTInit()
{

   //Set Baud rate

   UBRR0L = BAUD_PRESCALE;
   UBRR0H = (BAUD_PRESCALE>>8);

	UBRR1L = BAUD_PRESCALE;
	UBRR1H = (BAUD_PRESCALE>>8);
	
	UBRR2L = BAUD_PRESCALE;
	UBRR2H = (BAUD_PRESCALE>>8);
   /*Set Frame Format


   >> Asynchronous mode
   >> No Parity
   >> 1 StopBit

   >> char size 8

   */

   //UCSR0C=(1<<USBS0)|(3<<UCSZ00);
   //UCSR1C=(1<<USBS1)|(3<<UCSZ01);



   //Enable The receiver and transmitter

    UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	UCSR1B=(1<<RXEN1)|(1<<TXEN1)|(1 << RXCIE1);
	UCSR2B=(1<<RXEN2)|(1<<TXEN2)|(1 << RXCIE2);

	
	

}

void USARTWriteChar(char data)
{
	//Wait untill the transmitter is ready

	while(!(UCSR0A & (1<<UDRE0)))
	{
		//Do nothing
	}

	//Now write the data to USART buffer

	UDR0=data;
}

volatile char ReceivedByte1;
volatile bool completeflag1 = false;
volatile char ReceivedByte2;
volatile bool completeflag2 = false;

void main()
{

USARTInit();
USARTWriteChar('A');
USARTWriteChar('\n');
sei(); //Enable interrupts

   for(;;)
   {
	  if (completeflag1 == true) {
		  USARTWriteChar(ReceivedByte1);
		  completeflag1 = false;
	  }
	  else if (completeflag2 == true) {
		  USARTWriteChar(ReceivedByte2);
		  completeflag2 = false;
	  }
	  
   }
}

ISR(USART1_RX_vect) {
	
	ReceivedByte1 = UDR1; // Fetch the received byte value into the variable "ByteReceived"
	completeflag1 = true;
}

ISR(USART2_RX_vect) {
	
	ReceivedByte1 = UDR2; // Fetch the received byte value into the variable "ByteReceived"
	completeflag1 = true;
}