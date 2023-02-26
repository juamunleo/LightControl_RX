#include "main.h"
#include "nrf24l01p_driver.h"
#include "mcc_generated_files/mcc.h"

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    //TMR5_StopTimer();
    //TMR3_StopTimer();
    
    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    SPI_Open(SPI_DEFAULT);
    
    
    //App code:
    Payload_t payload = buildPayload(0);
    uint8_t data, nPulsation = 0, lastPulsation;
    
    RF24_t radio = radio_init();
    radio_setChannel(&radio, 5);
    
    radio_openReadingPipe(&radio, 0, 2, buildRegisterData(5, 0xEF, 0xAB, 0xC8, 0xF3, 0xD7));
    radio_powerUp(&radio);
    
    LED_SetHigh();
    __delay_ms(300);
    LED_SetLow();
    
    radio_startRx(&radio);
    while(1){
        if(checkFIFO(&radio)){
            payload = receiveBytes(2);
            data = payload.data[0];
            nPulsation = payload.data[1];
            if(nPulsation != lastPulsation && data == 0xAA){
                if(RELE_GetValue()){
                    RELE_SetLow();
                }else{
                    RELE_SetHigh();
                }
                lastPulsation = nPulsation;
            }
            Flush_RX();
        }
    }
}

/**
 End of File
*/
