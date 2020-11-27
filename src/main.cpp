#define F_CPU   1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 


#define LED _BV(PD6)     // = (1 << PB0)


#define LED_DDR DDRD
#define LED_PORT PORTD
void setupGPIO()
{
    DDRB = 0xFF; //1 output port
    PORTB = 0xFF; // 1 high
    
    DDRD &= ~(1<<PD2);
    PORTD |= (1<<PD2);
    
    
    DDRB  = 0b00001000;   // PB3 output
    DDRD  = 0b01100000;   // PD5 and PD6 outputs
  
    DDRD |= (1<<PD5);
    DDRD |= (1<<PD6);
    DDRD |= (1<<PD7);
    DDRB |= (1<<PB0);  
    DDRB |= (1<<PB6);  
    DDRB |= (1<<PB7);  
}

void setRed(bool on)
{
    if(on)
    {
        PORTD |= _BV(PD5);
        PORTD |= _BV(PD6);
        PORTD |= _BV(PD7);
    }else
    {
        PORTD &= ~_BV(PD5);
        PORTD &= ~_BV(PD6);
        PORTD &= ~_BV(PD7);
    }
}

void setYellow(bool on)
{
    if(on)
    {
        PORTD |= _BV(PD7);
    }else
    {
        PORTD &= ~_BV(PD7);
    }
}

void setGreen(bool on)
{
    if(on)
    {
        PORTB |= _BV(PB0);
    }else
    {
        PORTB &= ~_BV(PB0);
    }
}



ISR (INT0_vect)
{
      EIMSK = 0;
      setRed(false);
      setYellow(false);
      setGreen(false);
}



void disableInt0()
{
     cli();
     PCMSK0 &= ~(1 << PCINT0); 
     EIMSK &= ~(1 << INT0);
}

void enableSleep()
{
    setRed(true);
    setYellow(true);
    setGreen(true);
    _delay_ms(2000);
    setRed(false);
    setYellow(false);
    setGreen(false);
    
    
    cli();
    PCMSK0 |= (1 << PCINT0); 
    EIMSK |= (1 << INT0);
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    
    
    sleep_enable();
    cli();
    sei();
    // actually sleep
    sleep_cpu();
    sleep_disable();
        
}


void wakeUpSequence()
{
    for(int i = 0;i < 2; ++i)
    {
        setYellow(false);
        setGreen(false);
        setRed(true);
        _delay_ms(100);
    
        setRed(false);
        setGreen(true);
        _delay_ms(100);
    
        setYellow(true);
        setGreen(false);
        _delay_ms(100);
        setYellow(false);
        setRed(true);
        _delay_ms(100);
    }
    
    setRed(false);
    setYellow(false);
    setGreen(false);
}




int main (void) 
{   
    setupGPIO();
    while(true)
    {
      setRed(true);
      _delay_ms(100);
      setRed(false);
      _delay_ms(100);
    }
}