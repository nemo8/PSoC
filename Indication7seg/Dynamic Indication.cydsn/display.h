/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "project.h"

/* when this macro is active display format will 
   if _ZEROS_ENABLE_ defined when value is "0" - display shows "0000" 
   else   "0" - display shows "0" (cut off zeros befor digits)               
*/
#define _ZEROS_ENABLE_

#define NUMBER_OF_DIGITS (4u)
#define SEGS_OFF         (10u)
#define TABLE_7SEG_SIZE  (11u)

typedef struct {
    uint8 data[4];
    uint8 anode[4];
    uint8 digIdx;
}DisplayTypedef;
    
void Display_Init(void);
void Display_Update(uint16 bcdValue);    
    
#endif
/* []_DISPLAY_H */
