#include "sw_uart.h"

void Welcome_UART(void)
{
    char Msg1[] = "Welcome";
    char Msg2[] = "Team Archibald";
    putsUART(Msg1);
    NL_CR();
    putsUART(Msg2);
    NL_CR();
}


void NL_CR(void)
{
    // Proceeds to the beginning of the next line
    char NL = 0x0D;
    char CR = 0x0A;
    putcUART(NL);
    putcUART(CR);
}


void Manual_Help(void)
{
    char Msg1[] = "Manual Mode. Strike enter/joystick_X to enter Auto Mode.";
    char Msg2[] = "Use joystick_Y to set the maximum speed.";
    putsUART(Msg1);
    NL_CR();
    putsUART(Msg2);
    NL_CR();
}

void Auto_Help(void)
{
    char Msg1[] = "Auto Mode. Strike backspace/Joystick X to enter Manual Mode";
    char Msg2[] = "Strike P to enter Parrot Mode";
    putsUART(Msg1);
    NL_CR();
    putsUART(Msg2);
    NL_CR();
}

void Speed_Out(void)
{
    char Msg1[] = "Maximal Speed: ";
    putsUART(Msg1);
    NL_CR();
}


void Unknown_Out(void)
{
    char Msg[] = "Unknown Command.";
    putsUART(Msg);
    NL_CR();
}

void Parrot_Out(void)
{
    char msg[] = "Parrot Mode";
    putsUART(msg);
    NL_CR();
}

void Password_Out(void)
{
    char msg[] = "Enter Password";
    putsUART(msg);
    NL_CR();
}


void Factory_Out(void)
{
    char msg[] = "Factory Mode";
    putsUART(msg);
    NL_CR();
}