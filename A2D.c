#include <p18f4520.h>




void Init_A2D(void)
{
    // Selecting channel 0 for analog input
    // We toggle between channel 0 and channel 1 according to the mode we are 
    // currently in
    ADCON0bits.CHS0 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS3 = 0;
    
    
    // Configure TRISA properly
    // Channel 0 and 1 respectively correspond to the X and Y voltages
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    
    // Turn on the module
    ADCON0bits.ADON = 1;
    
    // VSS and VDD are reference voltages
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    
    // PORTA channels 0 through to 7 configured as Analog Inputs
    // The rest are configured as general Digital I/O.
    // The reason this was done is because PORTB Pins 0 and 1 are tied to 
    // analog channels 10 and 12. We would like to set these as I/O as they are
    // used in the software serial module
    
    
    ADCON1bits.PCFG3 = 0;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG0 = 1;
    
    // Left justification
    ADCON2bits.ADFM = 0;
    
    
    // When result is ready we do not sample for extra acquisition time
    // prior to conversion. Start the conversion immediately
    ADCON2bits.ACQT2 = 0;
    ADCON2bits.ACQT1 = 0;
    ADCON2bits.ACQT0 = 0; 
    ADCON2bits.ADCS2 = 1;
    ADCON2bits.ADCS1 = 0;
    ADCON2bits.ADCS0 = 0;
}


void IE_A2D(void)
{
    // Clear flag as a pre-cautionary measure
    PIR1bits.ADIF = 0;
    
    // Enable A2D interrupts to occur
    PIE1bits.ADIE = 1;
    
    // High PRIO
    IPR1bits.ADIP = 0;
}
