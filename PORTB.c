
#include <p18f4520.h>




void IE_PTB(void)
{
    // Enable interrupts associated with pin 1 port B
    INTCON3bits.INT1IE = 1;
    
    // High priority
    INTCON3bits.INT1IP = 0;
    
    // Interrupt on a falling edge
    // Falling edge is associated with reception start bit
    INTCON2bits.INTEDG1 = 0;
    
    // Not sure if required
    // TRISB = 0xFE;
    
}

