/* 
 * File:   UI_MAIN.c
 * Author: Marwan
 *
 * Created on October 1, 2016, 4:17 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "delays.h"
#include "sw_uart.h"
#include "configReg.h"
#include "xlcd.h"
#include "functions.h"
#include "CCP_Head.h"
#include "AD.h"

#define BUFFSIZE 32

volatile char RECIEVE;
volatile int RCV_CHAR;
volatile char CONVERT;
volatile char TOGGLE;
volatile char F_PRESSED;
volatile char FACTORY_MODE;
volatile char USER_MANUAL;
volatile char USER_AUTO;
volatile signed char STATE;


/*
 * 
 */

void Init_gVars(void);
void Read_Pin1(void);
void Read_Pin0(void);



void main(void)
{
    
    char PARROT_MODE = 0;
    
    // This is arbitrary. To be determined later
    char  MAX_SPEED = 5;
    
    
    // Exact parameters to be determined later
    char P_GAIN = 1;
    char I_GAIN = 0;
    char D_GAIN = 0;
    char YAW_MAX = 5;
    int IR_SPE = 0;
    int IR_SR = 0;
    int RF_SPE = 0;
    
    
    // Global variables INIT
    Init_gVars();
    
    // UART Module INIT.
    OpenUART();
    
    // Welcome message as seen on terminal
    Welcome_UART();
    
    // LCD Module INIT. Also outputs a welcome message
    init_XLCD();
    
    // Delay for approximately 5 seconds to allow welcome message to be seen
    Delay();
    
    // CCP Module INIT
    Init_CCP();
    
    // Timer Module INIT
    Init_Timer();
    
    // A2D Module INIT
    Init_A2D();
    
    // Manual Mode to appear across LCD screen
    UM_XLCD();
    
    // Helpful message to appear across terminal program
    Manual_Help();
    
    // Enable priorities
    EnablePrio();
    
    // PORTB Pin 1 (Pin associated with reception) INIT
    IE_PTB();
    
    // Configuring interrupt registers associated with both CCP and A2D Modules
    IE_CCP();
    IE_A2D();
    
    // Enable Interrupts
    EnableInt();
   
    while(1)
    {
        char INCREMENT = 1;
        
        // Acting upon the voltages read
        if (CONVERT)
        {
            
            unsigned char UPPER_THRESHOLD = 200;
            unsigned char LOWER_THRESHOLD = 50;
            
            // Sampling off the X Voltages generated and seeing whether its
            // high enough to transition to AUTO_MODE
            if (USER_MANUAL && (ADRESH > 200) && (TOGGLE == 0))
            {
                USER_MANUAL = 0;
                USER_AUTO = 1;
                clear_XLCD();
                home_XLCD();
                auto_XLCD();
                Auto_Help();
            }
            
            
            if (USER_MANUAL && (ADRESH < 50) && (TOGGLE == 1))
            {
                MAX_SPEED -= INCREMENT;
                Speed_Out();
                speed_XLCD();
            }
            
            if (USER_MANUAL && (ADRESH > 200) && (TOGGLE == 1))
            {
                MAX_SPEED += INCREMENT;
                Speed_Out();
                speed_XLCD();   
            }
            
           if (USER_AUTO && (ADRESH < 50) && (TOGGLE == 0))
           {
               USER_AUTO = 0;
               USER_MANUAL = 1;
               UM_XLCD();
               Manual_Help();
           }
            
            // If TOGGLE = 0 set to TOGGLE to 1 and sample from Pin 1 now
            // Otherwise Toggle is set to 0 (from 1) and we sample from Pin 0
            // now
            // However this is only applicable to USER_MANUAL only as we are
            // required to cycle between reading the voltages generated in the 
            // X and Y direction.
            // This is not required in AUTO_MODE. We are just reading off the 
            // X voltages. By construction toggle is cleared in AUTO_MODE
            
            if (USER_MANUAL)
            {
                
                if(TOGGLE = ((TOGGLE == 0) ? (1) : (0)))
                {
                    Read_Pin1();
                }
                else
                {
                    Read_Pin0();
                }
            }
            // Indicates action (or lack thereof) completed
            CONVERT = 0;
            
            // Re-enable Interrupts
            EnableInt();
        }
        
        // Indicates we have received a character
        
        if (RECIEVE)
        {
            // Indicates we have transitioned from USER_MANUAL to USER_AUTO
            // once this flag is set
            
            char transition = 0;
            
            if (USER_MANUAL)
            {
                switch(RCV_CHAR)
                {
                    // A was depressed
                    case(65):
                        MAX_SPEED += INCREMENT;
                        Speed_Out();
                        speed_XLCD();
                        break;
                        
                    // B was depressed   
                    case(66):
                        MAX_SPEED -= INCREMENT;
                        Speed_Out();
                        speed_XLCD();
                        break;
                        
                    // Enter was depressed 
                    case(13):
                        USER_MANUAL = 0;
                        USER_AUTO = 1;
                        transition++;
                        clear_XLCD();
                        home_XLCD();
                        auto_XLCD();
                        Auto_Help();
                        break;
                        
                    default:
                        Unknown_Out();
                        clear_XLCD();
                        home_XLCD();
                        Unknown_XLCD();
                        break;
                }
            }
            
            //
            if (USER_AUTO && !(transition))
            {
                switch(RCV_CHAR)
                {
                    // Indicates M was depressed. For some reason the backspace
                    // character (8) is not working
                    case(77):
                        USER_MANUAL = 1;
                        USER_AUTO = 0;
                        Manual_Help();
                        UM_XLCD();
                        break;
                        
                    // Indicates P was depressed.   
                    case(80):
                        USER_AUTO = 0;
                        PARROT_MODE = 1;
                        Parrot_Out();
                        clear_XLCD();
                        home_XLCD();
                        parrot_XLCD();
                        break;
                    default:
                        Unknown_Out();
                        clear_XLCD();
                        home_XLCD();
                        Unknown_XLCD();
                        break;    
                }
            }
            
            RECIEVE = 0;
            EnableInt();
        }
     
        while (PARROT_MODE)
        {
            // Do stuff. To be completed later
            // Possibly have a manually controlled parrot mode where the 
            // voltages generated are sampled and the user manually controls
            // the motion of the robot in order to track down the whereabouts of
            // the parrot.
            // Also have the standard autonomous mode as per the specifications.
            ;
        }
        
        while (F_PRESSED)
        {
            // The password which enables entry into FACTORY_MODE
            char password[] = "deeznuts";
            
            // ASCII Carriage return
            char CR = 13;
            
            // Allows us to know where along the string we are
            int len = BUFFSIZE;
            
            // Buffer to store user input along with an associated pointer
            char buff[BUFFSIZE];
            char *pbuff = buff;
            
            // Enter Password to appear along terminal program and LCD
            Password_Out();
            clear_XLCD();
            home_XLCD();
            pass_XLCD();
            
            // Recall interrupts are disabled at this stage. We are essentially
            // polling for user input as getcUART is essentially a polling
            // routine
            
            // Keep filling the buffer and stop until enter was depressed
            while (((*pbuff++) = getcUART()) != CR)
            {
                len--;
                
                // If maximum capacity exceeded repoint to the beginning of the
                // buffer
                if (len == 0)
                {
                    len = BUFFSIZE;
                    pbuff = buff;
                }
            }
            
            // Compare the first 8 bytes of password and buff
            // As the password is 8 characters (bytes) long
            // A return value of 0 indicates both strings are identical
            
         if ((len = memcmp(password,buff,8)) == 0)
         {  
             // Prepare to transition to FACTORY_MODE by outputting the relevant
             // messages to terminal and LCD 
             FACTORY_MODE = 1;
             USER_MANUAL = 0;
             USER_AUTO = 0;
             Factory_Out();
             clear_XLCD();
             home_XLCD();
             fact_XLCD();
             
             // Reconfigure priorities now. We are in FACTORY_MODE and so we 
             // choose to set the reception of a character a high priority
             // because as soon as the ESCAPE key is depressed we exit
             // FACTORY_MODE and we return to the original priority settings
             ReConPrio();
             
             // Allow a small delay in order for the FACTORY MODE message to 
             // be seen across the LCD prior to outputting P GAIN 
             // (As this is by default the first parameter to be configured)
             smallDelay();
             clear_XLCD();
             home_XLCD();
             pGain_XLCD();
         }
            // Otherwise inputted string is incorrect. By default we choose to 
            // go back to USER_MANUAL mode
         else
         {
             USER_MANUAL = 1;
             USER_AUTO = 0;
             UM_XLCD();
             Manual_Help();
         }
            // Whether or not the string was correct or not we re-enable 
            // interrupts and stop polling for the password
            // We are either in USER_MANUAL MODE or FACTORY_MODE at this stage.
            // We need interrupts to act accordingly
            F_PRESSED = 0;
            EnableInt();
        }
        
        while (FACTORY_MODE)
        {
            // The interrupt service routine serves to change the current state
            // within FACTORY_MODE provided TOGGLE is cleared. We are sampling
            // the voltages in the X direction
            // We change TOGGLE states towards the end of this FACTORY_MODE loop
            // We do not change the magnitude of the parameters unless TOGGLE 
            // was set which means we are currently sampling the Y Voltages
            if (TOGGLE == 1)
            {
                // Perform the corrective action based on the state we are 
                // currently at
                // The thresholds are the same as previous
                // TODO: Output the magnitudes and perform corrective action if
                // ranges are exceeded.
                // BEAR IN MIND a corrective action must be performed prior to 
                // being interrupted (from the Timer and thus the ADCON)
                // I have not yet outputted to terminal for this particular 
                // reason. If enough 
                
                switch(STATE)
                {
                    case 0:
                        if (ADRESH > 200)
                        {
                            P_GAIN += 1;
                            clear_XLCD();
                            home_XLCD();
                            pGain_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            P_GAIN -= 1;
                            clear_XLCD();
                            home_XLCD();
                            pGain_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            pGain_XLCD();
                            break;
                        }
                    case 1:
                        if (ADRESH > 200)
                        {
                            I_GAIN += 1;
                            clear_XLCD();
                            home_XLCD();
                            iGain_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            I_GAIN -= 1;
                            clear_XLCD();
                            home_XLCD();
                            iGain_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            iGain_XLCD();
                            break;
                        }
                    case 2:
                        if (ADRESH > 200)
                        {
                            D_GAIN += 1;
                            clear_XLCD();
                            home_XLCD();
                            dGain_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            D_GAIN -= 1;
                            clear_XLCD();
                            home_XLCD();
                            dGain_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            dGain_XLCD();
                            break;
                        }
                    case 3:
                        if (ADRESH > 200)
                        {
                            MAX_SPEED += 1;
                            clear_XLCD();
                            home_XLCD();
                            speed_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            MAX_SPEED -= 1;
                            clear_XLCD();
                            home_XLCD();
                            speed_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            speed_XLCD();
                            break;
                        }
                    case 4:
                        if (ADRESH > 200)
                        {
                            YAW_MAX += 1;
                            clear_XLCD();
                            home_XLCD();
                            yaw_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            YAW_MAX -= 1;
                            clear_XLCD();
                            home_XLCD();
                            yaw_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            yaw_XLCD();
                            break;
                        }
                    case 5:
                        if (ADRESH > 200)
                        {
                            IR_SPE += 1;
                            clear_XLCD();
                            home_XLCD();
                            IR1_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            IR_SPE -= 1;
                            clear_XLCD();
                            home_XLCD();
                            IR1_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            IR1_XLCD();
                            break;
                        }
                    case 6:
                        if (ADRESH > 200)
                        {
                            IR_SR += 1;
                            clear_XLCD();
                            home_XLCD();
                            IR2_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            IR_SR -= 1;
                            clear_XLCD();
                            home_XLCD();
                            IR2_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            IR2_XLCD();
                            break;
                        }
                    case 7:
                        if (ADRESH > 200)
                        {
                            RF_SPE += 1;
                            clear_XLCD();
                            home_XLCD();
                            RF_XLCD();
                            break;
                        }
                        else if (ADRESH < 50)
                        {
                            RF_SPE -= 1;
                            clear_XLCD();
                            home_XLCD();
                            RF_XLCD();
                            break;
                        }
                        else
                        {
                            clear_XLCD();
                            home_XLCD();
                            RF_XLCD();
                            break;
                        }
                    default:
                        break;
                }
            }
            
            // Change which pin we are sampling the voltages from
            if (TOGGLE = ((TOGGLE == 0) ? (1) : (0)))
            {
                Read_Pin1;
            }
            else
            {
                Read_Pin0;
            }
        }
    }
}





#pragma code lowVec = 0x18

void interrupt_at_lowVec(void)
{
    _asm GOTO LP_ISR _endasm
}

#pragma code 

#pragma interrupt LP_ISR
void LP_ISR(void)
{   
    if (PIR1bits.ADIF)
    {
        // Flag must be cleared in software
        PIR1bits.ADIF = 0;
        
        if (!(FACTORY_MODE))
        {
            // Disable Interrupts momentarily
            DisableInt();

            // Indicates we have to act upon the analog to digital conversion 
            //  in the main loop
            CONVERT++;
        }
        // If in FACTORY_MODE and we are currently sampling off the voltages in
        // the X direction then change states accordingly
        // Return to factory mode and provided we are currently sampling in
        // the Y direction perform the necessary action
        if (FACTORY_MODE && TOGGLE == 0)
        {
            if (ADRESH > 200)
            {
                STATE++;
                
                if (STATE > 7)
                {
                    STATE = 0;
                }
            }
            if (ADRESH < 50)
            {
                STATE--;
                
                if (STATE < 0)
                {
                    STATE = 7;
                }
            }
        }
        return;
    }
    
    
    if (PIR2bits.CCP2IF)
    {
        // Flag must be cleared in software.
        // This will trigger an A2D Conversion to take place.
        PIR2bits.CCP2IF = 0;
       
        return;
    }
    
    if (INTCON3bits.INT1IF)
    {
        // Character F
        char F = 70;
        
        // May have to modify to exclude the waiting part for pulling the line
        // low
        RCV_CHAR = ReadUART();
        
        // Temporarily disable interrupts
        DisableInt();
        
        // Set the receive flag and act accordingly
        
        if (RCV_CHAR != F)
        {
            RECIEVE = 1;
        }
        else
        {
            F_PRESSED = 1;
        }
        
        
        // Flag must be cleared in software
        INTCON3bits.INT1IF = 0;
        
        return;
    }  
    return;
}

#pragma code highVec = 0x08

void interrupt_at_highVec(void)
{
    _asm GOTO HP_ISR _endasm
}

#pragma code 

#pragma interrupt HP_ISR
void HP_ISR(void)
{   
    // By default we are in FACTORY_MODE if high priorities are set
    // I have included the && FACTORY_MODE just as a pre-caution
    if (INTCON3bits.INT1IF && FACTORY_MODE)
    {
        // If the escape key was depressed (27)
        if ((RCV_CHAR = getcUART()) == 27)
        {
            FACTORY_MODE = 0;
            USER_MANUAL = 1;
            USER_AUTO = 0;
            OriginPrio();
        }
    }
}


void Read_Pin1(void)
{
    ADCON0bits.CHS0 = 1;
}

void Read_Pin0(void)
{
    ADCON0bits.CHS1 = 1;
}


void Init_gVars(void)
{
    
    // We start off in this mode
    char USER_MANUAL = 1;
    
    // We do not start off in USER_AUTO mode or PARROT_MODE
    char USER_AUTO = 0;
    
    // The following are in use during the interrupt service routine and hence 
    // the need to globalize these variables
    FACTORY_MODE = 0;
    
    // Toggle cleared indicates we are reading voltages in the X direction
    // Hence reading from PORTA Pin 0
    // Toggle  = 1 means we are reading from PORTA Pin 1
    TOGGLE = 0;
    
    // This indicates whether an A2D conversion is complete and we can read the
    // joystick output (along with the value of toggle also)
    CONVERT = 0;
    
    // Arbitrary INIT
    RCV_CHAR = 0;
    
    // Flag set indicates we received a character and we act upon it
    RECIEVE = 0;
    
    // Flag set means F was depressed
    F_PRESSED = 0;
    
    // Factory Mode has multiple states within to configure some parameters 
    STATE = 0;
}