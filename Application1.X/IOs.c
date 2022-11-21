/*
 * File:   IOs.c
 * Author: Rushi V
 *
 * Created on September 30, 2020, 11:33 AM
 */


#include "xc.h"
#include "IOs.h"
#include "UART.h"
#include "time.h"






void delay_ms(uint16_t, uint8_t);
unsigned int TMR2flag;
//// IOinit() with IO interrupts
void IOinit(void)
{
    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;  // enable CN on CN0
    
    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN1IE = 1;  // enable CN on CN1
    
    TRISAbits.TRISA2 = 1; // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN2bits.CN30IE = 1;  // enable CN on CN30
    
    //CN Interrupt settings
    IPC4bits.CNIP = 5; //7 is highest priority, 1 is lowest, 0 is disabled interrupt
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable CN interrupts
    
    return;
}


// IO Check to demo HW debugger
//void Disp2String(char *);
void increment_min(void);
void increment_sec(void);
void Disp2String(char*);
void displayTime(void);
void start_stop_timer(void);
uint8_t start_stop_flag;
void resetTime(void);
void IOcheck(void)
{
//    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
//    delay_ms(400,1);   // 400 msec delay to filter out debounces 
//    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
    
    int pb3_pressTime=0;
    int pb3Pressed=0;
    while((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
    {
        pb3_pressTime++;
        pb3Pressed=1;
        delay_ms(1000,1);
    }
    if((pb3_pressTime<3)&&(pb3Pressed==1)){
        if(start_stop_flag==0)
            start_stop_flag=1;
        else
            start_stop_flag=0;
        LATBbits.LATB8=1;
       
    }else if(pb3_pressTime>=3){
        resetTime();
        displayTime();
    }
    
    while((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) &&  (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
    {
        //this is PB2
        delay_ms(500,1);
        increment_sec();
        displayTime();
    }
    
    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While only RA2 pb is pressed
    {
//        this is PB1
        delay_ms(500,1);
        increment_min();
        displayTime();
        
    }
    
   
    
    return;

}


///// Change of pin Interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
	CNflag = 1;  // global user defined flag - use only if needed
    IFS1bits.CNIF = 0;		// clear IF flag
//    IOcheck();
	
	Nop();	
    
    return;
}

