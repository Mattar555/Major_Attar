#include "xlcd.h"
#include <p18f4520.h>
#include <string.h>

void init_XLCD(void)
{
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while ((BusyXLCD()));
    WriteCmdXLCD(0x01);
    while ((BusyXLCD()));
    WriteCmdXLCD(BLINK_ON);
    while ((BusyXLCD()))
    WriteCmdXLCD(SHIFT_DISP_RIGHT);
    while ((BusyXLCD()))
    WriteCmdXLCD(0x00 | 0x80);
    while ((BusyXLCD()));
    putrsXLCD("Welcome");
    while ((BusyXLCD()));
    WriteCmdXLCD(0x40 | 0x80);
    while ((BusyXLCD()));
    putrsXLCD("Team Archibald"); 
}

void UM_XLCD(void)
{
    while ((BusyXLCD()));
    WriteCmdXLCD(0x01);
    while ((BusyXLCD()));
    WriteCmdXLCD(0x00 | 0x80);
    while ((BusyXLCD()));
    putrsXLCD("Manual Mode");
}


void clear_XLCD(void)
{
     while ((BusyXLCD()));
     WriteCmdXLCD(0x01);
}


void home_XLCD(void)
{
    while ((BusyXLCD()));
    WriteCmdXLCD(0x00 | 0x80);
}

void auto_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("User Auto");
}

void max_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Max Speed: 1");
}

void min_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Max Speed: 0");
}

void speed_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Speed: ");
    while ((BusyXLCD()));
}

void parrot_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Parrot Mode");
}

void Unknown_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Unknown Command");
}

void pass_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Enter Password");
}

void fact_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Factory Mode");
}

void pGain_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("P Gain: ");
}

void iGain_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("I Gain: ");
}

void dGain_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("D Gain: ");
}

void yaw_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("Yaw Max: ");
}

void IR1_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("IR SPE: ");
}

void IR2_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("IR SF: ");
}

void RF_XLCD(void)
{
    while ((BusyXLCD()));
    putrsXLCD("RF SPE: ");
}