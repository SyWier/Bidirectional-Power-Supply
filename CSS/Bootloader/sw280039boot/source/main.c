/**
 * main.c
 */

//
// Included Files
//
#include "inic.h"
#include "MUCI_bsp.h"

#define APP_FLASH_START_ADDR  0x083000
#define APP_FLASH_END_ADDR    0x08FFFE
#define APP_FLASH_CRC_ADDR    0x08FFFF

uint32_t Counter;
uint16_t FlashCRC;
uint16_t ApplicationCRCError;

void ApplicationCRCCheck(void);


 void main(void)
 {
    void (* FirmwareStartAddress)(void) ;

    *(long*)&FirmwareStartAddress = APP_FLASH_START_ADDR;

	inic();

	MUCI_bsp_Init();
	ApplicationCRCCheck();

	for (;;)
	{
	    Counter++;

	    MUCI_bsp_Background();

        if (ApplicationCRCError == 0)
        {
            MUCI_SetApplicationState(MUCI_DEVICESTATE_BOOT_RUN_CHK_OK);
        }
        else
        {
            MUCI_SetApplicationState(MUCI_DEVICESTATE_BOOT_RUN_CHK_NOK);
        }

        if (Counter>2500000)
        {
            if (ApplicationCRCError == 0)
            {
                FirmwareStartAddress() ; // appr. after 5sec
            }
        }

	}
}

void ApplicationCRCCheck(void)
{
    Uint32 FlashAddress = APP_FLASH_START_ADDR;
    FlashCRC = 0xFFFF;
    ApplicationCRCError = 0;

    while (FlashAddress <= APP_FLASH_END_ADDR)
    {
        MUCI_CalcCheckSum(&FlashCRC, (*(Uint16*)FlashAddress)>>8);    // felsõ
        MUCI_CalcCheckSum(&FlashCRC, (*(Uint16*)FlashAddress)&0xFF);  // alsó
        FlashAddress++;
    }

    if (*(Uint16*)APP_FLASH_CRC_ADDR != FlashCRC)
    {   // Hibás az application CRC
        ApplicationCRCError = 1;
    }
}
