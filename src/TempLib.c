#include <xc.h>
# include <stdio.h>
#include "final_LCDHeader.h"

void tempInit(void){        // temp write
    
    I2C2CONbits.I2CEN = 0;  // disable i2c
    I2C2BRG = 0x9D;         // IC2 speed/rate: 100KHz from the Data sheet
    I2C2CONbits.I2CEN = 1;  // enable i2c
    IFS3bits.MI2C2IF = 0;   // clear interrupt flag
    
    I2C2CONbits.SEN = 1;    // start sending
    
    while(I2C2CONbits.SEN == 1); 
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b10001000 ;  // slave address 0x44 and w
    
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b11111101 ;  // write command 0xfd (measure T & RH with high precision)
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;    // send STOP  
    while(I2C2CONbits.PEN == 1); // wait for STOP to finish
}


void tempRead(unsigned char data[], int dataSize){  // // temp read
    
    IFS3bits.MI2C2IF = 0; 			// Clear interrupt flag
    
    // send START
    I2C2CONbits.SEN = 1;			// send START
    while(I2C2CONbits.SEN == 1);    // wait for START to finish
    IFS3bits.MI2C2IF = 0; 			// clear interrupt flag
    
    // send slave address with W bit
    I2C2TRN = 0b10001000 ; 			// slave address 0x44. write command
    while(!IFS3bits.MI2C2IF);   	// wait for ACK
    IFS3bits.MI2C2IF = 0;       	// clear interrupt

	// send read register
    I2C2TRN = 0xfd;  				// write command 0xfd (measure T & RH with high precision)
    while(!IFS3bits.MI2C2IF);   	// wait for ACK
    IFS3bits.MI2C2IF = 0;	      	// clear interrupt
    
    delay_ms(10); 
    // send resend bit
    I2C2CONbits.RSEN = 1; 			// repeated start bit
    while(I2C2CONbits.RSEN == 1);	// wait for resend bit to be 0
    IFS3bits.MI2C2IF = 0;			// clear interrupt
    
    // send slave address with R bit
    I2C2TRN = 0b10001001; 			// slave address 0x44. read command
    while(!IFS3bits.MI2C2IF);   	// wait for ACK
    IFS3bits.MI2C2IF = 0;	      	// clear interrupt
 
    // get data
    I2C2CONbits.RCEN = 1;			// start receiving
    int i;
    delay_ms(10); 
    for (i = 0; i < 5; i++) {
	    data[i] = I2C2RCV; 	        // save temp bytes in data[]
        while(!IFS3bits.MI2C2IF);
	}

    // send NACK
    I2C2CONbits.ACKDT = 1;			// use NACK
    I2C2CONbits.ACKEN = 1;			// start NACK sequence
    while(I2C2CONbits.ACKEN == 1); 	// wait for NACK to finish sending
      
    // send STOP
    I2C2CONbits.PEN = 1;			// send STOP
    while(I2C2CONbits.PEN == 1); 	// wait for STOP to finish
}