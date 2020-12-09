#define F_CPU   1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 


#define LED _BV(PD6)     // = (1 << PB0)


#define LED_DDR DDRD
#define LED_PORT PORTD

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



ISR (INT0_vect)
{
      EIMSK = 0;
}



void disableInt0()
{
     cli();
     PCMSK0 &= ~(1 << PCINT0); 
     EIMSK &= ~(1 << INT0);
}

void enableSleep()
{
    
    
    
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
    
    setE(false);
    setD(false);
    setG(false);
    setA(false);
    setR(false);
}

void delay(int ms)
{
    while(ms > 0)
    {
        _delay_ms(1);
        --ms;
    }
}

void wakeUpSequence()
{
    for(int i = 0 ; i < 10; ++i)
    {

   setE(true);
   _delay_ms(50);
   setE(false);
   setD(true);
   _delay_ms(50);
   setD(false);
   setG(true);
   _delay_ms(50);
   setG(false);
   setA(true);
   _delay_ms(50);
   setA(false);
   setR(true);
   _delay_ms(50);
   setR(false);
   setA(true);
   _delay_ms(50);
   setA(false);
   setG(true);
   _delay_ms(50);
   setG(false);
   setD(true);
   _delay_ms(50);
   setD(false);
   setE(true);
   _delay_ms(50);
   setE(false);
    }

    for(int i = 0; i < 75; i++)
    {
        setE(true);
        setD(true);
        setG(true);
        setA(true);
        setR(true);
        delay(i);
        setE(false);
    setD(false);
    setG(false);
    setA(false);
    setR(false);
    delay(i);
    }
    for(int i = 75; i > 0; i--)
    {
        setE(true);
        setD(true);
        setG(true);
        setA(true);
        setR(true);
        delay(i);
        setE(false);
    setD(false);
    setG(false);
    setA(false);
    setR(false);
    delay(i);
    }

}




int main (void) 
{   
    setupGPIO();
    wakeUpSequence();
    
    setE(true);
    setD(true);
    setG(true);
    setA(true);
    setR(true);
    enableSleep();
    /*OCR0A = 128;
    // set PWM for 50% duty cycle


    TCCR0A |= (1 << COM0A1);
    // set none-inverting mode

    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // set fast PWM Mode

    //TCCR0B |= (1 << CS01);
    // set prescaler to 8 and starts PWM

    TCCR0B |= (1 << CS02);
    // set prescaler to 0 and starts PWM*/
    int i = 5;
    int j = 100;
    bool increase = true;
    while(true)
    {
        setE(true);
        setD(true);
        setG(true);
        setA(true);
        setR(true);
        delay(i);
        setE(false);
        setD(false);
        setG(false);
        setA(false);
        setR(false);
        delay(i);
        j--;
        if(j==0)
        {
            if(false == increase)
            {
                i--;
            }else
            {
                i++;
            }
            
            j=100;
        }
        if((i<0) || (i>5))
        {
            increase = ~increase;
        }

       /* enableSleep();
      setR(true);
      _delay_ms(60);
      setR(false);
      _delay_ms(400);*/
    }
}
