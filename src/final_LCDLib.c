
/*
 * File:   jian0655_lab3_main_v001.c
 * Author: yuan
 *
 * Created on February 17, 2022, 2:33 AM
 */


#include "xc.h"
#include "final_LCDHeader.h"
//#define CONTRAST 0b0000   // Make sure the contrast is adjustable


void delay_ms(unsigned int ms){    // set X ms delay
    while(ms-- > 0){
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_cmd(char Package){  // Sends an instruction byte to the LCD to configure 
                             //it according to the peripheral?s data sheet.
    
    //Send START
    IFS3bits.MI2C2IF = 0; //Clear interrupt flag
    I2C2CONbits.SEN = 1;  //Begin (S)tart sequency
    
    while(I2C2CONbits.SEN == 1);// SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000; // 8-bits consisting of control byte
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package; // 8-bits consisting of the data byte
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;  //Stop bit
    
    while(I2C2CONbits.PEN == 1);// PEN will clear when Stop bit is complete
}


void lcd_init(void){ //Initializes the LCD using the lcd_cmd function by data sheet. 
    
    I2C2CONbits.I2CEN = 0; //Disable I2C for safe
    I2C2BRG = 0x9D;        // IC2 speed/rate: 100KHz from the Data sheet
    IFS3bits.MI2C2IF = 0;  //Clear interrupt flag
    I2C2CONbits.I2CEN = 1; //Enable I2C
    
    delay_ms(40);
    
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, advanced instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01010110); // contrast low
    lcd_cmd(0b01011110); 
    lcd_cmd(0b01101100); // follower control
    
    delay_ms(200);
    
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display  
    
    delay_ms(1);
}


void lcd_setCursor(char x, char y){ //Uses the lcd_cmd function to set the cursor from which 
                                    //characters can be set on the LCD. Uses values x and y, column and row
    
    char cursor = ( (0x40 * y) + x) | 0b10000000; //Sets cursor using x, y
    lcd_cmd(cursor);

}; 


void lcd_printChar(char Package){ //A test function that sets a single character
                                    //on the LCD based on the position of the cursor
    
    I2C2CONbits.SEN = 1;//Initiate Start condition
    
    while(I2C2CONbits.SEN == 1);// SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte w/ RS=1
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = Package; // 8-bits consisting of the data byte
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);// PEN will clear when Stop bit is complete
    
}; 

void lcd_printStr(const char *s){
    
    I2C2CONbits.SEN = 1;//START BIT
    
    while(I2C2CONbits.SEN == 1);// SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    
    while(*s){
       
        if(*(s+1)!='\0'){ // Not the last char
            while(IFS3bits.MI2C2IF == 0);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = 0b11000000; // 8-bits consisting of control byte w/ RS=1, C0=1
            
            while(IFS3bits.MI2C2IF == 0);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = *(s); // 8-bits consisting of the data byte
            
        }else{ //the last char
            while(IFS3bits.MI2C2IF == 0);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = 0b01000000; // 8-bits consisting of control byte w/ RS=1, C0=0
            
            while(IFS3bits.MI2C2IF == 0);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = *(s); // 8-bits consisting of the data byte
        }
        s++;
        
    }
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1; // STOP BIT
    while(I2C2CONbits.PEN == 1);// PEN will clear when Stop bit is complete
    
};



