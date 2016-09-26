/* 
 * File:   Init_File.c
 * Author: Marwan
 *
 * Created on September 23, 2016, 2:38 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18f4520.h>
#include <delays.h>
#include <string.h>
#include "configReg.h"
#include "Serial_Head.h"
#include "LCD_Head.h"
#include "ISR.h"

volatile int USER_MANUAL;
volatile int USER_AUTO;
volatile int PARROT_MODE;
volatile float SPEED_MAX;


/* Function declaration */

void Init_gVars(void);


/*
 * 
 */
void main(void)
{
    LCD_Init();
    Init_gVars();
    // Set registers associated with RS232 accordingly 
    Serial_Init();
    
    // Welcome message to appear across LCD screen
    LCD_MSG_START();
    
    // Welcome message to appear on screen
    Start_MSG();
    
    // Newline followed by a carriage return
    NL_CR();
    
    // USER_MANUAL message.
    Start_Help();
    NL_CR();
    
    // "USER_MANUAL" to appear across LCD screen
    // First clear and home the LCD
    LCD_Clear();
    LCD_Home();
    LCD_UM();
    
    // Enable serial receive interrupts. Transmit interrupt disabled for the 
    // time being
    IE_Serial();
    
    // Wait for user prompts and act accordingly
    while(1)
    {
        ;  
    }
}



#pragma code highVec = 0x08
void interrupt_at_highVec(void)
{
    _asm GOTO hpISR _endasm
}

#pragma code

#pragma interrupt hpISR
void hpISR(void)
{   
    if (PIR1bits.RCIF)
    {
        int transition = 0;
        if (USER_MANUAL)
        {
            float increment = 0.1;
            int tmp = RCREG; 
            switch(tmp)
            {
                // 13 Denotes enter was pressed and hence we want to transition  
                // to USER_AUTO Mode
                case 13:
                    USER_MANUAL = 0;
                    USER_AUTO = 1;
                    ID_Serial;
                    AUTO_MSG();
                    NL_CR();
                    LCD_Clear();
                    LCD_Home();
                    LCD_UA();
                    IRE_Serial;
                    transition++;
                    break;
                    // 65 denotes A was depressed
                case 65:
                    // If we exceeded the maximum range do not further increase
                    // the speed
                    // TODO LATER: Write a function that prints the actual speed
                    // to serial
                    if (SPEED_MAX >= 1)
                    {
                        ID_Serial();
                        warning();
                        NL_CR();
                        LCD_Clear();
                        LCD_Home();
                        LCD_MAX();
                        IRE_Serial();
                        break;
                    }
                  //  char *pStr = createString(SPEED_MAX);
                    SPEED_MAX += increment;
                    ID_Serial();
                    Speed_Out(SPEED_MAX);
                    NL_CR();
                    LCD_Clear();
                    LCD_Home();
                    LCD_SPEED_OUT(SPEED_MAX);
                    IRE_Serial();
                    break;
                    // 66 denotes B was depressed
                case 66:
                    if (SPEED_MAX <= 0.01)
                    {
                        ID_Serial();
                        warning();
                        NL_CR(); 
                        LCD_Clear();
                        LCD_Home();
                        LCD_MIN();
                        IRE_Serial();
                        break;
                    }
                    SPEED_MAX -= increment;
                    ID_Serial();
                    Speed_Out(SPEED_MAX);
                    NL_CR();
                    LCD_Clear();
                    LCD_Home();
                    LCD_SPEED_OUT(SPEED_MAX);
                    IRE_Serial();
                    break;
                default:
                    ID_Serial();
                    Start_Help();
                    NL_CR();
                    IRE_Serial();
            }
        }
        if (USER_AUTO && (!(transition)))
        {
            int tmp = RCREG;
            switch(tmp)
            {
                case 80:
                    USER_AUTO = 0;
                    PARROT_MODE = 1;
                    ID_Serial();
                    Parrot_MSG();
                    NL_CR();
                    LCD_Clear();
                    LCD_Home();
                    LCD_PM();
                    IRE_Serial();
                    break;
                case 8:
                    USER_MANUAL = 1;
                    USER_AUTO = 0;
                    ID_Serial();
                    Start_Help();
                    NL_CR();
                    LCD_Clear();
                    LCD_Home();
                    LCD_UM();
                    IRE_Serial();
                    break;
                default:
                    ID_Serial();
                    AUTO_MSG();
                    NL_CR();
                    IRE_Serial();
            }   
        }
    }
}


void Init_gVars(void)
{
    USER_MANUAL = 1;
    USER_AUTO = 0;
    PARROT_MODE = 0;
    SPEED_MAX = 0.5;
}

