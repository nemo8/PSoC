
#include "project.h"

#define LED_RED   (2u)
#define LED_GREEN (0u)
#define LED_BLUE  (1u)


void setLedValue(uint8 ledVal)
{
   LED_Control_Write( ledVal & 0b00000111);
}


int main(void)
{
    uint32 rx;
    uint8 ledState = 0xFF; /* All leds are disabled */
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_Start();
    UART_UartPutString("Select LED to on/off:\r\n");
    UART_UartPutString("press 'R' or 'r' for red\r\n");
    UART_UartPutString("press 'G' or 'g' for green\r\n");
    UART_UartPutString("press 'B' or 'b' for blue\r\n");
    
    for(;;)
    {
        /* get data from UART */
        rx = UART_UartGetChar();
        
        if (0u != rx)
        {
            switch(rx)
            {
                case 'B':
                case 'b':
                    ledState ^= 1 << LED_BLUE;
                    UART_UartPutString("Changed Blue LED state\r\n");
                    break;
                
                case 'R':
                case 'r':
                    ledState ^= 1 << LED_RED;
                    UART_UartPutString("Changed Red LED state\r\n");
                    break;
                
                case 'G':
                case 'g':
                    ledState ^= 1 << LED_GREEN;
                    UART_UartPutString("Changed Green LED state\r\n");
                    break;
                
                default:
                    UART_UartPutString("Not correct command\r\n");
                    break;
                
            }
            setLedValue(ledState);
        }
    }
}

/* [] END OF FILE */
