/*
 * File:   finalproject_main.c
 * Author: yuan
 *
 * Created on April 15, 2022, 2:11 PM
 */


#include "xc.h"
#include <stdio.h>    // for sprintf
#include "final_LCDHeader.h"
#include "final_TempHeader.h"
#include "motorHeader.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

void pic24_init(){
    _RCDIV = 0; //Sets 16MHz frequency for PIC24
    AD1PCFG = 0xffff; //Sets pins to digital
}


int main(void) {   
    
    pic24_init();                           // PIC24 initialization first
    unsigned char tempbits[6];              // save the temp data to [0] and [1] got from temp sensor
    unsigned long int temp;                 // get the long int "fake" temp by tempbits[0] and tempbits[1]
    
                                            
    initMotor();                            //initialize the Motor, PWM makes it stop (speedMode: 0)
    lcd_init();                             // initialize the LCD
   
    while(1){
        
        tempInit();                         // write temp by sensor
        delay_ms(10);                       // delay 10 ms until finish read
        tempRead(tempbits,6);               // read temp data in tempbits array with six bytes length 


        temp = (unsigned long int)tempbits[0]*256+(unsigned long int)tempbits[1]; // get the long int "fake" temp

        double decTemp;                     // "real" temp with decimals      
        decTemp = -45.0+175.0*(temp)/65535; // get the "real" temp with decimals from "fake" temp by particular math calculation
        if(decTemp>100){                               
            decTemp = 100.0;                // set the upper limit (the highest temp is 100 degree in Celsius)
        }
        if(decTemp<0){                      // set the lower limit (the lowest temp is 0 degree in Celsius)
            decTemp = 0.0;
        }
    
                                                        
        lcd_setCursor(0,0);
        lcd_printStr("Temp:");                          // LCD display (first line)
        lcd_setCursor(0,1);                             // display decimal temp (second line)
        char realTemp[8];                               

        sprintf(realTemp, "%5.2f D", decTemp);  // ?x.xx D?    
                                                // 5.2 in the format string ?%5.2f? means 5 place holders for the whole
                                                // floating-point number, 2 of which are for the fractional part.
        lcd_printStr(realTemp);                 // prints to the LCD with the decimal temp
    
        if(decTemp >= 30.0){                    // when temp >= 30 degree, makes the fan turn automatically in low speed
                                                // and we can control speed by push button
            
            setSpeed(1);                        // initializes the speed to mode 1((low))
            initPushButton();                   // initializes the push button
        }
        while(decTemp >= 30.0){                 // always running till the temp < 30 degree in order to automatic fan with speed controls

            delay_ms(10);                       // Again! delay 10 ms until finish read 
            tempRead(tempbits,6);               // Again! update temp data in tempbits array with six bytes length


            temp = (unsigned long int)tempbits[0]*256+(unsigned long int)tempbits[1];

            decTemp = -45.0+175.0*(temp)/65535;     // get the "real" temp with decimals
            if(decTemp>100){                                
                decTemp = 100.0;                    // set the upper limit (the highest temp is 100 degree in Celsius)
            }
            if(decTemp<0){                          // set the lower limit (the lowest temp is 0 degree in Celsius)
                decTemp = 0.0;
            }


            lcd_setCursor(0,0);
            lcd_printStr("Temp:");                  // LCD display (first line)
            lcd_setCursor(0,1);
            char realTemp[8];                       // display decimal temp (second line)

            sprintf(realTemp, "%5.2f D", decTemp);  // ?x.xx D?    
                                                    // 5.2 in the format string ?%5.2f? means 5 place holders for the whole
                                                    // floating-point number, 2 of which are for the fractional part.
            lcd_printStr(realTemp);                 // Again! prints to the LCD with the decimal temp
            if(decTemp < 30.0){
                break;                              // Once temp < 30 degree, dumps out of while loop
            }
        }
    
                                                    // when temp < 30 degree, makes motor speed stop(mode: 0)
        setSpeed(0);
        CNPU2bits.CN27PUE = 0;                      // when temp < 30 degree, disables the push button
          
    };
   
    return 0;
}
