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

void shiftLeft(char *str, int len) {
    char first_char = str[0];
    for (int i = 1; i < len; i++) {
        str[i - 1] = str[i];
    }
    str[len - 1] = first_char;
}

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
    int ad_counter = 0;

    // G?ówna p?tla programu
    while(1)
    {
        char display_string[] = "SALE SALE SALE ";
        char ad_1[] = "50% OFF T-SHIRTS";
        char ad_2[] = "30% OFF SHOES";
        char ad_3[] = "15% OFF JACKETS";
        int str_length = strlen(display_string);
        int lines_to_display = 10;  // Number of lines to display

        if (ad_counter <= 12)
        {
            for (int line = 0; line < lines_to_display; line++) {
            PRINT_ClearScreen();
            
            char displayed[32];
            strcpy(displayed, display_string);
            strcat(displayed, "\n");
            if ((ad_counter % 6 == 0) || (ad_counter % 6 == 1))
                strcat(displayed, ad_1);
            if ((ad_counter % 6 == 2) || (ad_counter % 6 == 3))
                strcat(displayed, ad_2);
            if ((ad_counter % 6 == 4) || (ad_counter % 6 == 5))
                strcat(displayed, ad_3);
            PRINT_String(displayed, 32);
            __delay32(2000000);
            shiftLeft(display_string, str_length);
            ad_counter++;
            }
        }
        if (ad_counter > 12)
        {
            PRINT_ClearScreen();
            PRINT_String("MO-FR 10AM-4PM\nSAT   10AM-2PM", 32);
            __delay32(20000000);
            ad_counter = 0;
        }
    

    }

    return 0;
}
