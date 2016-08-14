#define F_CPU 16000000UL   //CPU-freq
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
  // Set up Interrupts //
  //DDRD &= ~(1 << DDD2);     // Clear the PD2 pin
  // PD2 (INT0 pin) is now an input
  
  //PORTD |= (1 << PORTD2);    // turn On the Pull-up
  // PD0 is now an input with pull-up enabled
  
  
  /*MCUCR |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
  GICR |= (1 << INT0);      // Turns on INT0
  
  sei();                    // turn on interrupts
  */
  
  // Solo ESC
  // A:
  //   PD4 +
  //   PD5 -
  // B:
  //   PC5 +
  //   PC4 -

  // A:
  //   PC3 +
  //   PB0 -
  // B:
  //   PC5 +
  //   PC4 -

  // A:
  //   PD5 +
  //   PD7 -
  // B:
  //   PB1 +
  //   PB2 -

  DDRD &= ~(1<<PD2);
  DDRD |= (1<<PD4);
  DDRD |= (1<<PD5);

  DDRC |= (1<<PC5);
  DDRC |= (1<<PC4);

  int timeUPStart = 0;
  int timeUPEnd = 0;
  int Pulse = -1;

  TCCR0 |= (0 << CS02) | (1 << CS01) | (1 << CS00); // prescaler: clock/64
  //TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); // prescaler: clock
  
  int trigged = 0;
  int num_trigs = 1;
  int dir = 0; // 1: Forward 0: Stopped -1: Reverse
  unsigned long counter2 = 0;

  int PWMMargin = 15;
  int PWMMax = 100;
  int PWMMiddle = 100;
  while(1) {
    //PORTD |= (1<<PD4);
    // MIN: 1030 MAX: 1830
    //while (PWMin == 0) {}   /* wait for input to go high*/
    if (!trigged && TCNT0 < 60000 && (PIND & (1<<PD2))) {
      TCNT0 = 0;
      timeUPStart = TCNT0;        /* capture the time when the signal when high*/
      trigged = 1;
      num_trigs++;
    }
    //while (PWMin == 1) {}  /* wait for the signal to go low*/
    if (trigged && TCNT0 < 60000 && !(PIND & (1<<PD2))) {
      timeUPEnd = TCNT0;  /* capture when the pulse goes low*/
      Pulse = (timeUPEnd - timeUPStart);  /*be careful deviding this so you don't end up with 0: 250 / 500 = 0 in integer math*/
      //Pulse = map(Pulse, 100, 500, 100, 200);
      trigged = 0;
    }

    counter2 = TCNT0*num_trigs;

    /*if (counter2 > 40000) {
      num_trigs = 1;
      }*/


    // 1000: > 140
    // 1400:
    //if (Pulse < 100 && TCNT0 > 120) {
    //(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

    //Pulse Input: 100 - 700?
    //Pulse Output: 100 - 200


    if (TCNT0 > 60000)
      continue;
    /*
    if (Pulse < 50-15)
      Pulse = map(Pulse, 0, 50-15, 50-15, 0);
    else if (Pulse > 50-15 && Pulse < 50+15)
      Pulse = 0;
    else
      Pulse = map(Pulse, 50+15, 100, 0, 50+15);
    */

    //Pulse = map(Pulse, 0, 80, 0, 70);

    //Pulse = Pulse*

    if (Pulse-PWMMiddle < 0)
      dir=-1;
    else
      dir=1;

    if (Pulse > PWMMiddle-PWMMargin && Pulse < PWMMiddle+PWMMargin) {
      dir = 0;
    }
    
    if (Pulse > PWMMiddle+PWMMax)
      Pulse = PWMMiddle+PWMMax;
    if (Pulse < PWMMiddle-PWMMax)
      Pulse = PWMMiddle-PWMMax;



	
    if (TCNT0 < Pulse-PWMMiddle && dir == 1) {

      // Forward //
      PORTD &= ~(1<<PD4);
      PORTD &= ~(1<<PD5);
      
      PORTC |= (1<<PC5);
      PORTC |= (1<<PC4);

    } else if (TCNT0 < (Pulse-PWMMiddle)*-1 && dir == -1) {

      // Forward //
      PORTD |= (1<<PD4);
      PORTD |= (1<<PD5);
      
      PORTC &= ~(1<<PC5);
      PORTC &= ~(1<<PC4);
      
            
    } else {                            
      // Reverse // ++
      //PORTD &= ~(1<<PD4);
      //PORTD &= ~(1<<PD5);

      PORTD |= (1<<PD4);
      PORTD |= (1<<PD5);
      
      PORTC |= (1<<PC5);
      PORTC |= (1<<PC4);

    }

    //_delay_ms(300);
    
    //PORTD &= ~(1<<PD4);
    //PORTD &= ~(1<<PD5);
  }
  
  return 0;
}

/*ISR (INT0_vect)
{
    
}*/
