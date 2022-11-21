#include "time.h"
#include <xc.h> 
#include "UART.h"
#include "TimerDelay.h"

void Disp2Dec(unsigned int);
void XmitUART2(char, unsigned int);
void Disp2String(char*);


void delay_ms(uint16_t, uint8_t);


void resetTime(void){
    minutes=0;
    seconds=0;
    start_stop_flag=0;
}

void displayTime(void){
    Disp2String("\n\r");
    Disp2Dec(minutes);
    Disp2String(" m");
    
    Disp2String(" : ");
    Disp2Dec(seconds);
    Disp2String(" s");
    
    
    
}
void count_down(void){
    
    if((seconds>0||minutes>0)&&(start_stop_flag==1)){
        delay_ms(930,1);
        decrement_sec();
        displayTime();
        
        LATBbits.LATB8=~LATBbits.LATB8;
        if (minutes == 0 && seconds == 0) 
            alarm();
    
    }
}
int decrement_min(void){
    if(minutes>0){
        minutes--;
        return 1;
    }
    return 0;
}

void decrement_sec(void){
    if(seconds>0){
        seconds--;
    }else{
        if(decrement_min()==1) {
            seconds=59;
        }
         
    }
}
void increment_min(void){
    minutes++;
    if(minutes>59) resetTime();
}

void increment_sec(void){
    seconds++;
    if(seconds>59){
        seconds=0;
        increment_min();
    }
}

void alarm(){
    Disp2String(" --ALARM");
    Disp2String("\n\r");
    start_stop_flag=0;
    LATBbits.LATB8=1;
}
