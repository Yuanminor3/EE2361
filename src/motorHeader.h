/* 
 * File:   motorHeader.h
 * Author: yuan
 *
 * Created on April 19, 2022, 1:53 AM
 */

#ifndef MOTORHEADER_H
#define	MOTORHEADER_H

#ifdef	__cplusplus
extern "C" {
#endif
void setSpeed(int speed);
void initMotor(void);
void initPushButton(void);
void __attribute__((interrupt, auto_psv)) _IC1Interrupt(void);



#ifdef	__cplusplus
}
#endif

#endif	/* MOTORHEADER_H */

