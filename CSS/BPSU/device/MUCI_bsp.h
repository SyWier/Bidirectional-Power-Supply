/*
 * MUCI_bsp.h
 *
 *  Created on: 2019. máj. 6.
 *      Author: z003zn6d
 */

#ifndef INCLUDE_MUCI_BSP_H_
	#define INCLUDE_MUCI_BSP_H_

    #include "stdint.h"
    #include "stdbool.h"
    #include "hw_types.h"

	#include "Term_defs.h"

    #include "MUCI.h"
    #include "MUCI_Recorder.h"

    #define MUCI_DeviceAddress      (0u)

    #define CAN_ID_MUCI    0x700u

    #define MUCI_DATA_LENGTH    1024u
    #define MUCI_RECORDER_DATA_LENGTH       512u
    #define MUCI_RECORDER_CHANNEL_NUMBER    4u

    void MUCI_bsp_Init(void);
    void MUCI_bsp_Background(void);
    void MUCI_bsp_SCI_Recover(void);
    uint32_t MUCI_bsp_Address_Calculation(uint32_t VariableAddress);
	void MUCI_bsp_Reboot();
	uint16_t MUCI_bsp_CLEAR_FLASH_SECTOR(uint8_t FlashSectorNumber);
	uint16_t MUCI_bsp_PROGRAM_FLASH(uint32_t StartAddress, uint32_t Quantity, uint8_t* Data);
	uint16_t MUCI_bsp_READ_FLASH(uint32_t StartAddress, uint32_t Quantity, uint8_t* Data);
#endif /* INCLUDE_MUCI_BSP_H_ */
