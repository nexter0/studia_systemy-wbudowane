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

unsigned int lfsr_generate(unsigned int *state, unsigned int taps) {
    unsigned int lsb = *state & 1; // get the least significant bit
    *state >>= 1; // shift the register one position to the right

    if (lsb) // if the least significant bit was set (1)
        *state ^= taps; // perform XOR operation with the taps mask

    return lsb;
}

int main(void) {
    
    // Inicjalizacja p?ytki
    unsigned char portValue = 1;
    unsigned char mode = 0;
    AD1PCFG = 0xFFFF;
    TRISA = 0x0000;
    LATA = portValue;
    LCD_Initialize();

    // G?ówna p?tla programu
    while(1)
    {
        // PROG 1 Licznik binarny 0 - 255
        if (mode == 0)
        {
            // Wyczyszczenie i wy?wietlenie informacji na LCD
            PRINT_ClearScreen();
            PRINT_String("Program 1", 10);
            
            portValue = -1;
            // P?tla podprogramu
            while(1)
            {
                // Odliczanie w gór?
                portValue++;
                
                // Opó?nienie
                __delay32(1000000);
                
                // Przekazanie na port
                LATA = portValue;
                
                // Warunek zap?tlenia
                if (portValue >= 255)
                    portValue = -1;

                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                    if (mode > 8)
                        mode = 0;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode = 8;
                    break;
                }
                    
            }
        }
        
        // PROG 2 Licznik binarny 255 - 0
        if (mode == 1)
        {
            // Wyczyszczenie i wy?wietlenie informacji na LCD
            PRINT_ClearScreen();
            PRINT_String("Program 2", 10);
            
            portValue = 256;
            // P?tla podprogramu
            while(1)
            {
                // Odliczanie w dó?
                portValue--;
                
                // Opó?nienie
                __delay32(1000000);
                
                // Przekazanie na port
                LATA = portValue;
                
                // Warunek zap?tlenia
                if (portValue <= 0)
                    portValue = 256;
                
                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                    if (mode > 8)
                        mode = 0;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode--;
                    break;
                    if (mode < 0)
                        mode = 8;
                }
                    
            }
        }
        
        // PROG 3 Licznik w kodzie Gray'a 0 - 255
        if (mode == 2)
        {
            // Wyczyszczenie i wy?wietlenie informacji na LCD
            PRINT_ClearScreen();
            PRINT_String("Program 3", 10);
            
            portValue = -1;
            // P?tla podprogramu
            while(1)
            {
                // Odliczanie w gór?
                portValue++;
                
                // Opó?nienie
                __delay32(1000000);
                
                // Konwersja na kod Gray'a i przekazanie na port
                int gray = portValue ^ (portValue >> 1);
                LATA = gray;
                
                // Warunek zap?tlenia
                if (portValue >= 255)
                    portValue = -1;


                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                    if (mode > 8)
                        mode = 0;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode--;
                    break;
                    if (mode < 0)
                        mode = 8;
                }
                    
            }
        }
        
        // PROG 4 Licznik w kodzie Gray'a 255 - 0
        if (mode == 3)
        {
            PRINT_ClearScreen();
            PRINT_String("Program 4", 10);
            
            portValue = 256;
            // P?tla podprogramu
            while(1)
            {
                // Odliczanie w dó?
                portValue--;
                
                // Opó?nienie
                __delay32(1000000);
                
                // Konwersja na kod Gray'a i przekazanie na port
                int gray = portValue ^ (portValue >> 1);
                LATA = gray;
                
                // Warunek zap?tlenia
                if (portValue <= 0)
                    portValue = 256;

                
                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                    if (mode > 8)
                        mode = 0;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode--;
                    break;
                    if (mode < 0)
                        mode = 8;
                }
            }
        }
        
        // PROG 5 Licznik BCD 2x4 0 - 99
        if (mode == 4)
        {
            PRINT_ClearScreen();
            PRINT_String("Program 5", 10);
            
            portValue = -1;
            // P?tla podprogramu
            while(1)
            {
                // Odliczanie w gór?
                portValue++;
                
                // Wyodr?bnienie dziesi?tek
                unsigned char tens = (portValue / 10);
                // Wyodr?bnienie jednostek
                unsigned char units = (portValue % 10);
                // Po??czenie dziesi?tki i jednostki w jedn? liczb? binarn?
                unsigned char binary = (tens << 4) | units;
                
                // Opó?nienie
                __delay32(1000000);
                
                // Przekazanie na port
                LATA = binary;
                
                // Warunek zap?tlenia
                if (portValue >= 99)
                    portValue = -1;
                
                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                    if (mode > 8)
                        mode = 0;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode--;
                    break;
                    if (mode < 0)
                        mode = 8;
                }
            }
        }
        
        // PROG 6 Licznik BCD 2x4 99 - 0
        if (mode == 5)
        {
            PRINT_ClearScreen();
            PRINT_String("Program 6", 10);
            
            portValue = 100;
            // P?tla podprogramu
            while(1)
            {
                // Odliczanie w gór?
                portValue--;
                
                // Wyodr?bnienie dziesi?tek
                unsigned char tens = (portValue / 10);
                // Wyodr?bnienie jednostek
                unsigned char units = (portValue % 10);
                // Po??czenie dziesi?tki i jednostki w jedn? liczb? binarn?
                unsigned char binary = (tens << 4) | units;
                
                // Opó?nienie
                __delay32(1000000);
                // Przekazanie na port
                LATA = binary;
                
                // Warunek zap?tlenia
                if (portValue <= 0)
                    portValue = 100;
                
                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                    if (mode > 8)
                        mode = 0;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode--;
                    break;
                    if (mode < 0)
                        mode = 8;
                }
            }
        }
        
        // PROG 7 3-bitowy w??yk
        if (mode == 6)
        {
            PRINT_ClearScreen();
            PRINT_String("Program 7", 10);
            
            // Definicja kolejnych kroków wężyka jako liczby całkowitej
            // określająca zapalone diody
            int snake[] = {7, 14, 28, 56, 112, 224, 112, 56, 28, 14, 7};
            int counter = 0;
            // P?tla podprogramu
            while(1)
            {
                portValue = snake[counter];
                
                // Przekazanie na port
                LATA = portValue;
                
                // Opó?nienie
                __delay32(1000000);
                
                // Zmiana na kolejny krok
                counter++;
                if (counter > 10)
                    counter = 0;
                
                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode++;
                    break;
                    if (mode > 8)
                        mode = 0;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode--;
                    break;
                    if (mode < 0)
                        mode = 8;
                }
            }
        }
        
        // PROG 8 Kolejka
        if (mode == 7)
        {
            PRINT_ClearScreen();
            PRINT_String("Program 8", 10);
            
            int queueSteps[] = {0, 128, 192, 224, 240, 248, 252, 254, 255};
            int counter;
            int exitFlag = 0;
            // P?tla podprogramu
            while(!exitFlag)
            {
                int n;


                for (counter = 0; counter < 8; counter++)
                {
                    for (int i = 0; i < 8 - counter; i++)
                    {
                        n = 1 << i;
                        __delay32(1000000);
                        portValue = queueSteps[counter] | n;
                        LATA = portValue;
                        
                        // Obs?uga przycisku +
                        if (BUTTON_IsPressed(BUTTON_S6))
                        {
                           mode++;
                           exitFlag = 1;
                           if (mode > 8)
                               mode = 0;
                        }
                        // Obs?uga przycisku -
                        if (BUTTON_IsPressed(BUTTON_S3))
                        {
                           mode--;
                           exitFlag = 1;
                           if (mode < 0)
                               mode = 8;
                        }
                        if (exitFlag)
                            break;
                    }
                    if (exitFlag)
                        break;
                }          
            }
        }
        
       // PROG 9 RNG
        if (mode == 8)
        {
            PRINT_ClearScreen();
            PRINT_String("Program 9", 10);
            
            int queueSteps[] = {0, 128, 192, 224, 240, 248, 252, 254, 255};
            int counter;
            // P?tla podprogramu
            while(1)
            {

                lfsr_generate(&portValue, 0b111001);
                LATA  = portValue;
                
                __delay32(1000000);

                // Obs?uga przycisku +
                if (BUTTON_IsPressed(BUTTON_S6))
                {
                    mode = 0;
                    break;
                }
                // Obs?uga przycisku -
                if (BUTTON_IsPressed(BUTTON_S3))
                {
                    mode--;
                    break;
                    if (mode < 0)
                        mode = 8;
                }
            }
        }
        
        
    }

    return 0;
}
