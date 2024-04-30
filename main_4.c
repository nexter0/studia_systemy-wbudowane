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
#include <string.h>

int main(void) {
    
    // Inicjalizacja p?ytki
    AD1PCFG = 0xFFFF;
    //TRISA = 0x0000;
    
    LCD_Initialize();
    // Domy?lna konfiguracja ADC
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    // ADC ma czyta? potencjometr
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    // output LED ów PORT A

    unsigned int value;
    unsigned int time;
    unsigned int power = 500;
    int real_time_sec = 0;
    int work_setup = 0;
    int real_min;
    int real_sec;
    unsigned int state = 0;



    // G?ówna p?tla programu
    while(1)
    {
        if (state == 0)
        {
            // Czytanie 10-bit warto?ci z potencjometru
            value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
            // B??d?
            if (value == 0xFFFF) 
                continue;

                // normalizacja zakresu do 0 255
                unsigned char normalizedValue = value >> 2;
                time = normalizedValue * 5;
                int min = time / 60;
                int sec = time % 60;
                // DEBUG LCD
                PRINT_ClearScreen();
                char string[18];
                char sec_str[4];
                char power_str[8];


                sprintf(string, "%d", min); 
                strcat(string, " m ");
                sprintf(sec_str, "%d", sec);
                strcat(string, sec_str);
                strcat(string, " s");
                strcat(string, "\n");
                sprintf(power_str, "%d", power);         
                strcat(string, power_str);
                strcat(string, " W");
                PRINT_String(string, 18);

                if (BUTTON_IsPressed(BUTTON_S4))
                {
                    power+=100;
                    if (power>=1300)
                        power = 500;
                    __delay32(500000);
                    continue;
                }
                
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    state = 1;
                    __delay32(500000);
                } 

                __delay32(500000);
        }
        else if (state == 1)
        {
            if (work_setup == 0)
            {
                int min = time / 60;
                int sec = time % 60;
                real_time_sec = sec + min * 60;
                work_setup = 1;
            }

            real_min = real_time_sec / 60;
            real_sec = real_time_sec % 60;
            // DEBUG LCD
            PRINT_ClearScreen();
            char string[24];
            char sec_str[4];


            sprintf(string, "%d", real_min); 
            strcat(string, " m ");
            sprintf(sec_str, "%d", real_sec);
            strcat(string, sec_str);
            strcat(string, " s");
            strcat(string, "\n");
            strcat(string, "WORKING...");

            PRINT_String(string, 24);
            real_time_sec--;
            
            for (int i = 0; i<10; i++)
            {
                __delay32(400000);
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    state = 2;
                    __delay32(100000);
                }
                if (BUTTON_IsPressed(BUTTON_S5))
                {
                    state = 0;
                    __delay32(100000);
                } 
            }
            
            
        }
        else if (state == 2)
        {
            PRINT_ClearScreen();
            char string[24];
            char sec_str[4];

            sprintf(string, "%d", real_min); 
            strcat(string, " m ");
            sprintf(sec_str, "%d", real_sec);
            strcat(string, sec_str);
            strcat(string, " s");
            strcat(string, "\n");
            strcat(string, "PAUSED");
            
            PRINT_String(string, 24);
                
            for (int i = 0; i<10; i++)
            {
                __delay32(400000);
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    state = 1;
                    __delay32(100000);
                } 
                if (BUTTON_IsPressed(BUTTON_S5))
                {
                    state = 0;
                    __delay32(100000);
                } 
            }
            
        }

    }

    return 0;
}
