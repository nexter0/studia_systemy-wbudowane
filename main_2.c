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
#include "adc.h"
#include <stdio.h>

int main(void) {
    
    // Inicjalizacja p?ytki
    unsigned char portValue = 1;
    unsigned char mode = 0;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    
    LCD_Initialize();
    // Domyślna konfiguracja ADC
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    // ADC ma czytać potencjometr
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    // output LED ów PORT A

    unsigned int value;
    unsigned int alarm = 0;

    // G?ówna p?tla programu
    while(1)
    {
        // Czytanie 10-bit wartości z potencjometru
        value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
        // Błąd?
        if (value == 0xFFFF) 
            continue;
            
            // normalizacja zakresu do 0 255
            unsigned char normalizedValue = value >> 2;
            // Output na port A (LEDy)
            
            if ((normalizedValue > 127) && (alarm == 0))
            {
                alarm = 1;
                for(int blink = 0; blink <= 5; blink++)
                {
                    value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
                    normalizedValue = value >> 2;
                    LATA = 1;
                    __delay32(2000000);
                    LATA = 0;
                    __delay32(2000000);
                    if (normalizedValue <= 127)
                    {
                        LATA = 0;
                        alarm = 0;
                        break;
                    }
                    // Obsluga przycisku STOP
                    if ((BUTTON_IsPressed(BUTTON_S3)) && (alarm == 1))
                    {
                        LATA = 0;
                        break;
                    } 
                }
                LATA = 255;
                
            }
            if (normalizedValue <= 127)
            {
                LATA = 0;
                alarm = 0;
            }
            
            // Obsluga przycisku STOP
            if ((BUTTON_IsPressed(BUTTON_S3)) && (alarm == 1))
            {
                LATA = 0;
            }
            
            // DEBUG LCD
            PRINT_ClearScreen();
            char string[5];
            sprintf(string, "%d", normalizedValue); 
            PRINT_String(string, 5);
            __delay32(100000);

    }

    return 0;
}
