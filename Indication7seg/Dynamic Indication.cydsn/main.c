/* ========================================
 *
 * Copyright YOUR COMPANY, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
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


const uint8 table7Seg[TABLE_7SEG_SIZE] = {

/********   HGFEDCBA   ******************************/
/********   76543210   ******************************/   
/* 0 */   0b11000000u,      /*      *** A ***       */
/* 1 */   0b11111001u,      /*      *       *       */
/* 2 */   0b10100100u,      /*      F       B       */
/* 3 */   0b10110000u,      /*      *       *       */
/* 4 */   0b10011001u,      /*      *** G ***       */
/* 5 */   0b10010010u,      /*      *       *       */
/* 6 */   0b10000010u,      /*      E       C       */
/* 7 */   0b11111000u,      /*      *       *       */
/* 8 */   0b10000000u,      /*      *** D *** H *   */
/* 9 */   0b10010000u,      /*                      */ 
/* OFF */ 0b11111111u       /*                      */    
/***************************************************/    
        
};

static DisplayTypedef display;

CY_ISR(CLK_ISR_Handler);
CY_ISR(SPI_ISR_Handler);

void Display_Init(void);
void Display_Update(uint16 bcdValue);

void Display_Init(void)
{
    display.digIdx = 0u;
       
    /* init anode */
    display.anode[0] = 0xFEu; // 0b1111 1110
    display.anode[1] = 0xFDu; // 0b1111 1101
    display.anode[2] = 0xFBu; // 0b1111 1011
    display.anode[3] = 0xF7u; // 0b1111 0111
    
    SPIM_Start();
    
    /* Sets up the CLK interrupt and enables it */
    ClkISR_StartEx(CLK_ISR_Handler); 
    SpiISR_StartEx(SPI_ISR_Handler);  
    
    Pin_Enable_Write(0u); /* EO=0 - enalbed */
}

static inline void bcdTo7segDecoder(uint8* outBuff, uint16 inputData)
{
    /* reset buffers after function start */
    outBuff[0] = 0;
    outBuff[1] = 0;
    outBuff[2] = 0;
    outBuff[3] = 0;
    
    while(inputData > 999u)
    {
        ++outBuff[0];
        inputData -= 1000u;
    }
    
    while(inputData > 99u)
    {
        ++outBuff[1];
        inputData -= 100u;
    }
    
    while(inputData > 9u)
    {
        ++outBuff[2];
        inputData -= 10u;
    }
    
    outBuff[3] = inputData;
    
#ifndef _ZEROS_ENABLE_ 
    if (outBuff[0] < 1u)
    {
        outBuff[0] = SEGS_OFF; 
        if (outBuff[1] < 1u)
        {
            outBuff[1] = SEGS_OFF; 
            if (outBuff[2] < 1u)
            {
                outBuff[2] = SEGS_OFF; 
            }
        }
    }
#endif
}

void Display_Update(uint16 bcdValue)
{
    uint8 digBuff[4];
    
    bcdTo7segDecoder(digBuff, bcdValue);
    
    display.data[0] = table7Seg[digBuff[0]];
    display.data[1] = table7Seg[digBuff[1]];
    display.data[2] = table7Seg[digBuff[2]];
    display.data[3] = table7Seg[digBuff[3]];
}


/* Intrerupt for disylay updating */
CY_ISR(CLK_ISR_Handler)
{ 
    /* select appropriate anode */
    Pin_Anodes_Write(display.anode[display.digIdx]); 
    
    /* send data to 74HC595 IC via SPI */
    SPIM_WriteTxData(display.data[display.digIdx]);
          
    ++display.digIdx;
    /* MASK 0x03 will reset value to 0 if value reached 4 */
    display.digIdx = display.digIdx & (NUMBER_OF_DIGITS - 1u); /* 0x03 */
    
    ClkISR_ClearPending();
}

CY_ISR(SPI_ISR_Handler)
{
    /* send strobe pulse ( __-__ ) to IC */
    /* rising edje writes data to shift register of 74HC595 */
    Pin_Enable_Write(1u);
    CyDelayUs(5);
    /* falling edje enables IC output of 74HC595 */
    Pin_Enable_Write(0u);
    
    SpiISR_ClearPending();
}

static uint16 cnt = 9999u;

int main(void)
{  
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    Display_Init();
        
    for(;;)
    {
        Display_Update(cnt);
       
        if (--cnt == 0u) cnt = 9999u;
        CyDelay(100);
    }
}

/* [] END OF FILE */
