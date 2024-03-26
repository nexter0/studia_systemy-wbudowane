// PIC24FJ128GA010 Configuration Bit Settings
// For more on Configuration Bits, see Section 1.1
// consult your device data sheet
// CONFIG2
#pragma config POSCMOD = XT // XT Oscillator mode selected
#pragma config OSCIOFNC = ON // OSC2/CLKO/RC15 as port I/O (RC15)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor disabled
#pragma config FNOSC = PRI // Primary Oscillator (XT, HS, EC)
#pragma config IESO = ON // Int Ext Switch Over Mode enabled
// CONFIG1
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = ON // Watchdog Timer Window Mode disabled
#pragma config FWDTEN = OFF // Watchdog Timer disabled
#pragma config ICS = PGx2 // Emulator/debugger uses EMUC2/EMUD2
#pragma config GWRP = OFF // Writes to program memory allowed
#pragma config GCP = OFF // Code protection is disabled
#pragma config JTAGEN = OFF // JTAG port is disabled
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include "buttons.h"
#include "print_lcd.h"
#include "lcd.h"

int main(void) {
    
    unsigned char portValue = 1;
    unsigned char mode = 0; // Port A access
    AD1PCFG = 0xFFFF; // set to digital I/O (not analog)
    TRISA = 0x0000; // set all port bits to be output
    LATA = portValue;
    LCD_Initialize();
    //while(1)
    //{
    //    if (BUTTON_IsPressed(BUTTON_S3))
    //        portValue++;
    //        
    //    LATA = portValue; // write to port latch
    //    __delay32(100000);
    //}


    while(1)
    {
        if (mode == 0)
        {
            portValue = 0;
            while(1)
            {
                portValue++;
                __delay32(1000000);
                LATA = portValue; // write to port latch

                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                }
                    
            }
        }
        if (mode == 1)
        {
            portValue = 255;
            while(1)
            {
                portValue--;
                __delay32(1000000);
                LATA = portValue; // write to port latch

                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode--;
                    break;
                }
                    
            }
        }
    }


    return 0;
}
