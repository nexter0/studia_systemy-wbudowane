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


void formatTime(char *display, int time_p1, int time_p2) {
    // Calculate minutes and seconds for Player 1
    int minutes_p1 = time_p1 / 60;
    int seconds_p1 = time_p1 % 60;

    // Calculate minutes and seconds for Player 2
    int minutes_p2 = time_p2 / 60;
    int seconds_p2 = time_p2 % 60;

    // Format the time strings for both players
    char time_str_p1[6];  // "MM:SS\0"
    char time_str_p2[6];  // "MM:SS\0"

    sprintf(time_str_p1, "%02d:%02d", minutes_p1, seconds_p1);
    sprintf(time_str_p2, "%02d:%02d", minutes_p2, seconds_p2);

    // Format the display string
    sprintf(display, "%s      %s\n P1          P2", time_str_p1, time_str_p2);
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

    unsigned int value;
    unsigned int time;
    unsigned int power = 500;
    int player = 0;
    int time_p1 = 0;
    int time_p2 = 0;
    int bonus = 0;
    int real_min;
    int real_sec;
    unsigned int state = 0;
    int mode_selected = 0;
    
    char modes_str_arr[9][20] = {"1 min\nBULLET", "1 | 1\nBULLET", "2 | 1\nBULLET",   
                                 "3 min\nBLITZ", "3 | 2\nBLITZ", "5 min\nBLITZ", 
                                 "10 min\nRAPID", "15 | 10\nRAPID", "30 min\nRAPID"};


    // G?ówna p?tla programu
    while(1)
    {
        if (state == 0)
        {
            PRINT_ClearScreen();
            PRINT_String(modes_str_arr[mode_selected], 16);

            if (BUTTON_IsPressed(BUTTON_S4))
            {
                mode_selected++;
                if (mode_selected>=9)
                    mode_selected = 0;
                __delay32(500000);
                continue;
            }
            if (BUTTON_IsPressed(BUTTON_S3))
            {
                state = 1;
                __delay32(500000);
                continue;
            }

            __delay32(500000);
        }
        else if (state == 1)
        {
            switch (mode_selected) {
            case 0:
                time_p1 = 60;
                time_p2 = 60;
                bonus = 0;
                break;
            case 1:
                time_p1 = 60;
                time_p2 = 60;
                bonus = 1;
                break;
            case 2:
                time_p1 = 120;
                time_p2 = 120;
                bonus = 1;
                break;
            case 3:
                time_p1 = 180;
                time_p2 = 180;
                bonus = 0;
                break;
            case 4:
                time_p1 = 180;
                time_p2 = 180;
                bonus = 2;
                break;
            case 5:
                time_p1 = 300;
                time_p2 = 300;
                bonus = 0;
                break;
            case 6:
                time_p1 = 600;
                time_p2 = 600;
                bonus = 0;
                break;
            case 7:
                time_p1 = 900;
                time_p2 = 900;
                bonus = 10;
                break;
            case 8:
                time_p1 = 1800;
                time_p2 = 1800;
                bonus = 0;
                break;
            }
            
            char display[34];
            formatTime(display, time_p1, time_p2);
            PRINT_ClearScreen();
            PRINT_String(display, 32);
            
            if (BUTTON_IsPressed(BUTTON_S4))
            {
                state = 2;
                player = 2;
                __delay32(4000000);
                continue;
            }
            if (BUTTON_IsPressed(BUTTON_S3))
            {
                state = 2;
                player = 1;
                __delay32(4000000);
                continue;
            }
            
        }
        else if (state == 2)
        {
            if (player == 1)
                time_p1--;
            else
                time_p2--;
            if (!(time_p1 && time_p2))
                state = 5;
            char display[34];
            formatTime(display, time_p1, time_p2);
            PRINT_ClearScreen();
            PRINT_String(display, 32);
            for (int i = 0; i<2; i++)
            {
                if (BUTTON_IsPressed(BUTTON_S4))
                {
                    if (player == 2) {
                        time_p2 += bonus;
                        player = 1;
                    }
                    
                    
                }
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    if (player == 1) {
                        time_p1 += bonus;
                        player = 2;
                    }
                }
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    state = 3;
                }
                if (BUTTON_IsPressed(BUTTON_S5))
                {
                    state = 4;
                }
                __delay32(400000);
            }   
        }
        else if (state == 3) {
            PRINT_ClearScreen();
            PRINT_String("------DRAW------", 16);
            __delay32(4000000);
        }
        else if (state == 4) {
            PRINT_ClearScreen();
            if (player == 1)
                PRINT_String("--P1 RESIGNED--", 16);
            else
                PRINT_String("--P2 RESIGNED--", 16);
            __delay32(4000000);
        }
        else if (state == 5) {
            PRINT_ClearScreen();
            if (player == 1)
                PRINT_String("P1 OUT OF TIME\nP2 WINS", 24);
            else
                PRINT_String("P2 OUT OF TIME\nP1 WINS", 24);
            __delay32(4000000);
        }
            

    }

    return 0;
}
