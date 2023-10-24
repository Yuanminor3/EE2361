/* 
 * File:   final_TempHeader.h
 * Author: yuan
 *
 * Created on April 15, 2022, 2:51 PM
 */

#ifndef FINAL_TEMPHEADER_H
#define	FINAL_TEMPHEADER_H

#ifdef	__cplusplus
extern "C" {
#endif
    void tempInit(void);
    void tempRead(unsigned char data[], int dataSize);
    
#ifdef	__cplusplus
}
#endif

#endif	/* FINAL_TEMPHEADER_H */

