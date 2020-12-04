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
  
    DDRD |= (1<<PD5); //R
    DDRD |= (1<<PD6); //E
    DDRD |= (1<<PD7); //A
    DDRB |= (1<<PB0); //G
    DDRB |= (1<<PB6);  
    DDRB |= (1<<PB7);  //D
}

void setE(bool on)
{
    if(on)
    {
        PORTD &= ~_BV(PD6);
    }else
    {
        PORTD |= _BV(PD6);
    }
}

void setD(bool on)
{
    if(on)
    {
        PORTB &= ~_BV(PB7);
    }else
    {
        PORTB |= _BV(PB7);
    }
}

void setG(bool on)
{
    if(on)
    {
        PORTB &= ~_BV(PB0);
    }else
    {
        PORTB |= _BV(PB0);
    }
}

void setA(bool on)
{
    if(on)
    {
        PORTD &= ~_BV(PD7);
    }else
    {
        PORTD |= _BV(PD7);
    }
}

void setR(bool on)
{
    if(on)
    {
        PORTD &= ~_BV(PD5);
    }else
    {
        PORTD |= _BV(PD5);
    }
}


void setRed(bool on)
{
    if(on)
    {
        //PORTD |= _BV(PD5);
        //PORTD |= _BV(PD6);
        //PORTD |= _BV(PD7);

        //PORTB |= _BV(PB0);
        //PORTB |= _BV(PB6);
        PORTB |= _BV(PB7);

    }else
    {
        //PORTD &= ~_BV(PD5);
        //PORTD &= ~_BV(PD6);
        //PORTD &= ~_BV(PD7);

        //PORTB &= ~_BV(PB0);
        //PORTB &= ~_BV(PB6);
        PORTB &= ~_BV(PB7);

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
    setE(false);
    setD(false);
    setG(false);
    setA(false);
    setR(false);
    while(true)
    {
      setR(true);
      _delay_ms(60);
      setR(false);
      _delay_ms(400);
    }
}