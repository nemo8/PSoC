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
#include "display.h"


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
