/*
 * MUCI_bsp.c
 *
 *  Created on: 2019. máj. 6.
 *      Author: z003zn6d
 */

#include "F28x_Project.h"
#include "driverlib.h"
#include "MUCI_bsp.h"
#include "F021_F28003x_C28x.h"
#include "flash_programming_f28003x.h"
#include "MUCI.h"


#ifndef MUCI_BOOT_VERSION
    #include "MUCI_Recorder.h"
    #include "Term_defs.h"
#endif

MUCI_Packet_t Rec_Packet, Send_Packet;

void MUCI_bsp_Init(void)
{
    Rec_Packet.State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    Send_Packet.State = MUCI_PACKET_SEND_FINISHED;

#ifndef MUCI_BOOT_VERSION
    InitTermVars();
#endif

    MUCI_SetDeviceAddress(MUCI_DeviceAddress);
    MUCI_SetApplicationState(MUCI_DEVICESTATE_FLASHAPI_RUN);
    MUCI_Recorder_Init();

}

void MUCI_bsp_Background(void)
{
    uint8_t Data;
    uint32_t DataCount;

    MUCI_bsp_SCI_Recover();

#ifndef MUCI_BOOT_VERSION
    MUCI_Recorder_Background();
#endif

    if (SCI_FIFO_RX0 < SCI_getRxFIFOStatus(SCIA_BASE)) {
        DataCount = 1;
        Data = SCI_readCharNonBlocking(SCIA_BASE);
        MUCI_ReceivePacket(&Rec_Packet, &Data, DataCount);
    }

    //Check it is for own processing
    MUCI_ProcessPacket(&Rec_Packet, &Send_Packet);

    //Drop package because not mine address and no route to other DSP
    MUCI_PacketRouterNoMoreRoute(&Rec_Packet);

    DataCount = 1;
    while ((SCI_FIFO_TX16 > SCI_getTxFIFOStatus(SCIA_BASE)) && (0u != DataCount))
    {
        DataCount = 1;
        MUCI_SendPacket(&Send_Packet, &Data, &DataCount);
        if (0 != DataCount)
        {
            SCI_writeCharNonBlocking(SCIA_BASE, Data);
        }
    }
}

void MUCI_bsp_SCI_Recover(void)
{
    if (SCI_getRxStatus(SCIA_BASE) & SCI_RXSTATUS_BREAK)
    {
        SCI_performSoftwareReset(SCIA_BASE);
    }

    if (SCI_getRxStatus(SCIA_BASE) & SCI_RXSTATUS_OVERRUN)
    {
        SCI_performSoftwareReset(SCIA_BASE);
    }
}

uint32_t MUCI_bsp_Address_Calculation(uint32_t VariableAddress)
{
  uint32_t VariableAddress_TMP;

//  /* Recorder compatibility FUBU */
//  if ((VariableAddress >= (uint32_t)&MUCI_RecorderParameter) & (VariableAddress < ((uint32_t)&MUCI_RecorderParameter + sizeof(MUCI_RecorderParameter_t) / 1)))
//  {
//    VariableAddress_TMP = (uint32_t)&MUCI_RecorderParameter + ((VariableAddress - (uint32_t)&MUCI_RecorderParameter) * 2);
//  }
//  else
//  {
//    VariableAddress_TMP = VariableAddress;
//  }

  VariableAddress_TMP = VariableAddress;

  return VariableAddress_TMP;
}

void MUCI_bsp_Reboot()
{
    EALLOW;
    WdRegs.WDCR.all = 0x28;
    WdRegs.WDCR.all = 0x0;
    EDIS;
}

uint16_t MUCI_bsp_CLEAR_FLASH_SECTOR(uint8_t FlashSectorNumber)
{
	uint16_t Status = 0u;
   Fapi_StatusType oReturnCheck;

    switch(FlashSectorNumber)
    {
        case 0:
            /* Erase sector A */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector0_start);
            break;
        case 1:
            /* Erase sector B */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector1_start);
            break;
        case 2:
            /* Erase sector C */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector2_start);
            break;
        case 3:
            /* Erase sector D */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector3_start);
            break;
        case 4:
            /* Erase sector E */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector4_start);
            break;
        case 5:
            /* Erase sector F */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector5_start);
            break;
        case 6:
            /* Erase sector G */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector6_start);
            break;
        case 7:
            /* Erase sector H */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector7_start);
            break;
        case 8:
            /* Erase sector I */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector8_start);
            break;
        case 9:
            /* Erase sector J */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector9_start);
            break;
        case 10:
            /* Erase sector K */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector10_start);
            break;
        case 11:
            /* Erase sector L */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector11_start);
            break;
        case 12:
            /* Erase sector M */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector12_start);
            break;
        case 13:
            /* Erase sector N */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector13_start);
            break;
        case 14:
            /* Erase sector O */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector14_start);
            break;
        case 15:
            /* Erase sector P */
            oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector,(uint32 *)Bzero_Sector15_start);
            break;
        default :
            oReturnCheck=Fapi_Error_InvalidAddress;
            break;
    }

    // Wait until FSM is done with erase sector operation
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    Status = (uint16_t) oReturnCheck;

	return Status;
}

uint16_t MUCI_bsp_PROGRAM_FLASH(uint32_t StartAddress, uint32_t Quantity, uint8_t* Data)
{
	uint16_t Status = 0u;
    Fapi_StatusType oReturnCheck;
    uint32_t DataPosition, FlashDataPointer;
    uint16_t FlashData[8];

    EALLOW;

    FlashDataPointer = 0;
    oReturnCheck = Fapi_Status_Success;

    for(DataPosition = 0; (DataPosition < Quantity) && (oReturnCheck == Fapi_Status_Success); DataPosition += 2)
    {
        FlashData[FlashDataPointer] = ((uint32_t)Data[DataPosition] << 8) +
                                      ((uint32_t)Data[DataPosition + 1] << 0);
        FlashDataPointer++;
        if (0 == (FlashDataPointer % 8))
        {
            oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)(StartAddress),FlashData, 8, 0, 0, Fapi_AutoEccGeneration);
            while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
            StartAddress += FlashDataPointer;
            FlashDataPointer = 0;
        }
    }

    //Write remained data
    if (0 != (FlashDataPointer % 8))
    {
        oReturnCheck = Fapi_issueProgrammingCommand((uint32 *)(StartAddress),FlashData, FlashDataPointer % 8, 0, 0, Fapi_AutoEccGeneration);
        while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);
    }

	return Status;
}

uint16_t MUCI_bsp_READ_FLASH(uint32_t StartAddress, uint32_t Quantity, uint8_t* Data)
{
	uint16_t Status = 0u;

	return Status;
}
