#define _XTAL_FREQ 8000000
#include "configbits.h"
#include <xc.h>
#include "flexlcd.h"

__bit modo = 0;

//Botões
#define bt_heater PORTBbits.RB0
#define bt_modo PORTBbits.RB1

void main(void) 
{
    ADCON0 = 0b00000000; 
    ADCON1 = 0b00001111; 
    INTCON2bits.RBPU = 0; 
    
    TRISB = 0b00000011; 
    TRISC = 0b00000000;
    TRISD = 0b00001111; 
    TRISE = 0b00000000;
    
    
    PORTB = 0;
    LATB = 0;
    PORTC = 0;
    LATC = 0;
    PORTD = 0;
    LATD = 0;
    PORTE = 0;
    LATE = 0;
    
    
    INTCON2bits.INTEDG0 = 0; 
    INTCONbits.INT0IF = 0; 
    INTCONbits.INT0IE = 1; 
    
    INTCON2bits.INTEDG1 = 0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
    
    Lcd_Init();
    Lcd_Cmd(LCD_CURSOR_OFF);
    
    Lcd_Out(1, 0,"Manual          ");
    Lcd_Out(2, 0, "                        ");
    
    modo = 0;
            
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    ei();
    
    while(1){
        //Modo automático
        if (modo == 1) {
            LATCbits.LATC5 = 1;
            Lcd_Out(1, 0,"Automatico          ");
            Lcd_Out(2, 0,"Status: ON      ");
            
            for (int i = 0; i < 100; i++) {
                __delay_ms(100);
                CLRWDT();
                if (modo == 0) break;
            }
            
            if (modo == 0) continue;
            
            LATCbits.LATC5 = 0;
            Lcd_Out(2, 0,"Status: OFF     ");
            
             for (int i = 0; i < 100; i++) {;
                __delay_ms(100);
                CLRWDT();
                if (modo == 0) break;
            }
        
        }
    }
    return;
}

void interrupt isr(void) {
    if (INTCON3bits.INT1IF == 1) {
        __delay_ms(20);
        
        if (bt_modo == 0) {
            modo = !modo;
            
            LATCbits.LATC5 = 0;
            
            if(modo == 1) {
                Lcd_Out(1, 0,"Automatico            ");
                Lcd_Out(2, 0, "                        ");
            } else {
                Lcd_Out(1, 0,"Manual            ");
                Lcd_Out(2, 0, "                        ");
            }
        }
        INTCON3bits.INT1IF = 0;
    }
     
     // Botão do aquecedor
    if (INTCONbits.INT0IF == 1){
        __delay_ms(20);

        if (bt_heater == 0) {
            if (modo == 0) {
                LATCbits.LATC5 = ~LATCbits.LATC5;

                if (LATCbits.LATC5 == 0) {
                    Lcd_Out(2, 0,"Status: OFF     ");
                } else {
                    Lcd_Out(2, 0,"Status: ON      ");
                }
            }
        }

        INTCONbits.INT0IF = 0;
    }
}
