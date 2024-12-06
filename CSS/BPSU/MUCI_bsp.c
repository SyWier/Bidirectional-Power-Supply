/*
 * MUCI_bsp.c
 *
 *  Created on: 2019. máj. 6.
 *      Author: z003zn6d
 */

#include "F28x_Project.h"
#include "driverlib.h"
#include "MUCI_bsp.h"

MUCI_Packet_t Rec_Packet, Send_Packet;

void MUCI_bsp_Init(void)
{
    Rec_Packet.State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    Send_Packet.State = MUCI_PACKET_SEND_FINISHED;

    InitTermVars();
    MUCI_SetDeviceAddress(MUCI_DeviceAddress);
    MUCI_SetApplicationState(MUCI_DEVICESTATE_APP_RUN);
    MUCI_Recorder_Init();

}

void MUCI_bsp_Background(void)
{
    uint8_t Data;
    uint32_t DataCount;

    MUCI_bsp_SCI_Recover();
    MUCI_Recorder_Background();

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

	//FlashErase(&QspiInstance, 0x0, NUM_SECTORS * SECTOR_SIZE);

	return Status;
}

uint16_t MUCI_bsp_PROGRAM_FLASH(uint32_t StartAddress, uint32_t Quantity, uint8_t* Data)
{
	uint16_t Status = 0u;

	//FlashWrite(&QspiInstance, StartAddress, Quantity, Data, WRITE_CMD);

	return Status;
}

uint16_t MUCI_bsp_READ_FLASH(uint32_t StartAddress, uint32_t Quantity, uint8_t* Data)
{
	uint16_t Status = 0u;

	return Status;
}
