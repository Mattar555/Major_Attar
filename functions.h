/* 
 * File:   functions.h
 * Author: matt8911
 *
 * Created on 30 September 2016, 12:49 PM
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

void DisableInt(void);
void EnableInt(void);
void EnablePrio(void);
void ReConPrio(void);
void OriginPrio(void);
void IE_PTB(void);
void LP_ISR(void);
void HP_ISR(void);
void Manual_Help(void);
void Auto_Help(void);
void Speed_Out(void);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTIONS_H */
