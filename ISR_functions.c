#include <p18f4520.h>


void EnablePrio(void)
{
    RCONbits.IPEN = 1;
}


void EnableInt(void)
{
    // Enable interrupts
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}

void DisableInt(void)
{
    // Disable interrupts
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
}

void ReConPrio(void)
{
    INTCON3bits.INT1IP = 1;
}

void OriginPrio(void)
{
    INTCON3bits.INT1IP = 0;
}