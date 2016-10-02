#include "delays.h"

void DelayTXBitUART(void)
{
    Delay100TCYx(2);
    Delay10TCYx(4);
    _asm 
            NOP 
            NOP
            NOP
            NOP
            NOP
            NOP
            NOP
            NOP
    _endasm  
}
void DelayRXHalfBitUART(void)
{
    Delay100TCYx(1);
    Delay10TCYx(2);
    _asm NOP _endasm
}

void DelayRXBitUART(void)
{
     Delay100TCYx(2);
    Delay10TCYx(4);
    _asm 
            NOP 
            NOP
            NOP
            NOP
            NOP
            NOP
    _endasm   
}



void Delay(void)
{
    Delay10KTCYx(0);
    Delay10KTCYx(0);
}

void smallDelay(void)
{
    Delay10KTCYx(0);
}