
#include <p18f4520.h>



void Init_CCP(void)
{
    // Trigger special event
    // Upon compare match
    // Special events including automatic resetting of the timer registers
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 0;
    CCP2CONbits.CCP2M1 = 1;
    CCP2CONbits.CCP2M0 = 1;
    
  /* Since the special event automatically resets the timers we just need to 
     set CCP2L(H) to a constant value from the onset 
     In order to fire every millisecond the values must be:
    
     1 MS
    CCPR2L = 0xE8;
    CCPR2H = 0x03;
    
     2.5 MS
    CCPR2L = 0xC4;
    CCPR2H = 0x09;  */
   
   
    
    // 5 MS
    CCPR2L = 0x88;
    CCPR2H = 0x13;
}


void Init_Timer(void)
{
    // Enables read/write of the 16 bit value in one operation
    T3CONbits.RD16 = 1;
    // Selecting timer 3 as the timer base for both CCP modules
    T3CONbits.T3CCP2 = 1;
    // Pre scaler of 1
    T3CONbits.T3CKPS1 = 0;
    T3CONbits.T3CKPS0 = 0;
    // Use internal clock
    T3CONbits.TMR3CS = 0;
    T3CONbits.TMR3ON = 1;
}

void IE_CCP(void)
{
    IPR2bits.CCP2IP = 0;
    PIE2bits.CCP2IE = 1;
   
}