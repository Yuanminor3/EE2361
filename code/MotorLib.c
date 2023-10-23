#include <xc.h>
#include "final_LCDHeader.h"
int speedMode = 0;

 void setSpeed(int speed){  // change the speed of fan when fan turns on 
                              
        if (speed==0){      // mode: 0, stop
            speedMode = 0;
            OC1RS = 0;
        }
        else if (speed==1){ // mode: 1, low speed (also is the initialization speed)
            speedMode = 1;
            OC1RS = 12000;
        }
        else if (speed==2){ // mode: 2, medium speed
            speedMode = 2;
            OC1RS = 24000;
        }
        else if (speed==3){ // mode: 3, high speed
            speedMode = 3;
            OC1RS = 36000;
        }
};


void initMotor(void){           // in1(RB8):H in2(RB7):L PWM(RB6):H STBY(RB9):H  -----> clockwise
                                // *****speed control

    TRISB = 0b1111110000111111; // RB6 through RB9 are set as outputs to motor driver
     LATB = 0b0000001101000000; // set RB6,8,9 high
     
    //PPS
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 18;  // use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    //TMR3
    T3CON = 0;                  // stop Timer
    T3CONbits.TCKPS = 1;        // PRE 1:8
    TMR3 = 0;                   // Initialize to zero
    PR3 = 39999;                // period for 20 ms with 1:8 prescaler
    _T3IF = 0;                  // clear Timer 3 interrupt flag
    
    //OC1 
    OC1CON = 0;                 // turn off OC1 for now
    
    OC1R = 200;                 
    OC1RS = 0;                  // stop at initial (speed mode: 0)

    OC1CONbits.OCTSEL = 1;      // use Timer 3 for compare source
    OC1CONbits.OCM = 0b110;     // output compare PWM w/o faults  
    
    T3CONbits.TON = 1;          // turn on the timer3 now!
}

void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void){
                                // every time press the push button, 
                                // the speed mode will increase by 1
        delay_ms(10);           // delay for button debounce
        _IC1IF = 0;             // clear input capture Interrupt
            
        if (speedMode==0)      
            setSpeed (1);
        
        else if (speedMode==1) 
            setSpeed (2);
        
        else if (speedMode==2) 
            setSpeed (3);
        
        else if (speedMode==3) // Reset the mode to 0 to stop the fan
            setSpeed (0);
}
 

void initPushButton(void){       // connect push button to RB5
    
    //AD1PCFG & TRIS bits
    TRISBbits.TRISB5 = 1;        // RB5 to input
    CNPU2bits.CN27PUE = 1;       // pull-up resistor
    
    //TMR2
    T2CON = 0;              // stop Timer
    T2CONbits.TCKPS = 0b11; // PRE 1:256
    TMR2 = 0;               // initialize to zero
    PR2 = 62499;            // period for 1 sec with 1:256 prescaler
    _T2IF = 0;              // clear timer 2 interrupt flag
    _T2IE = 0;              // disable Timer 2 interrupt
    
    //PPS
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 5;  // Use Pin RP8 = "5", for Input Capture 1 (Table 10-2)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    // IC1
    IC1CON = 0;             // reset IC1 
    IC1CONbits.ICTMR = 1;   // timer 2
    IC1CONbits.ICM = 0b010; // capture on every falling edge
    
    _IC1IF = 0;
    _IC1IE = 1;             // enable IC1 interrupt
    
    
    T2CONbits.TON = 1;      // turn on the timer2 now!
}