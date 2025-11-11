/*
 * MUCI_MonitorCommands.c
 *
 *  Created on: 2018. szept. 11.
 *      Author: Szako
 */

#include "MUCI_MonitorCommands.h"
#include "MUCI_Recorder.h"
#include "MUCI_bsp.h"

/* Temp */
#ifndef MUCI_BOOT_VERSION
    #include "Term_defs.h"
#endif
/* End Temp */

bool MUCI_Command_GET_PROTOCOL_VERSION(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    MUCI_Push_uint8_MessageData(Answer, MUCI_MAJOR_VERSION);
    MUCI_Push_uint8_MessageData(Answer, MUCI_MINOR_VERSION);
    MUCI_Push_uint8_MessageData(Answer, MUCI_PATCH_LEVEL);

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;

    return true;
}

bool MUCI_Command_PING(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    bool retval_b = true;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    Answer->Data[Answer->DataPointer] = MUCI_GetApplicationState();
    Answer->DataPointer++;

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;

    return retval_b;
}

bool MUCI_Command_READ_NEW(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
	uint32_t VariableAddress;
	MUCI_Types_t VariableType;

	uint8_t TMPuint8_t;
	uint16_t TMPuint16_t;
	uint32_t TMPuint32_t;
	uint64_t TMPuint64_t;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0;

    Message->DataPointer = 0;
    MUCI_Get_uint32_MessageData(Message, &VariableAddress);
    MUCI_Get_uint8_MessageData(Message, &TMPuint8_t);
    VariableType = (MUCI_Types_t)TMPuint8_t;

    MUCI_Push_uint32_MessageData(Answer, VariableAddress);
    MUCI_Push_uint8_MessageData(Answer, (uint8_t)VariableType);

    switch(VariableType) {
		case MUCI_TYPE_BOOL:
		case MUCI_TYPE_CHAR:
		case MUCI_TYPE_UINT8:
		case MUCI_TYPE_INT8:
			TMPuint8_t = *((uint8_t*)VariableAddress);
			MUCI_Push_uint8_MessageData(Answer, TMPuint8_t);
			break;
		case MUCI_TYPE_UINT16:
		case MUCI_TYPE_INT16:
			TMPuint16_t = *((uint16_t*)VariableAddress);
			MUCI_Push_uint16_MessageData(Answer, TMPuint16_t);
			break;
		case MUCI_TYPE_UINT32:
		case MUCI_TYPE_INT32:
		case MUCI_TYPE_FLOAT:
			TMPuint32_t = *((uint32_t*)VariableAddress);
			MUCI_Push_uint32_MessageData(Answer, TMPuint32_t);
			break;
		case MUCI_TYPE_UINT64:
		case MUCI_TYPE_INT64:
		case MUCI_TYPE_DOUBLE:
			TMPuint64_t = *((uint64_t*)VariableAddress);
			MUCI_Push_uint64_MessageData(Answer, TMPuint64_t);
			break;
		case MUCI_TYPE_STRING:
			break;
		default:
			break;
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_WRITE_NEW(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
	uint32_t VariableAddress;
	MUCI_Types_t VariableType;

	uint8_t TMPuint8_t;
	uint16_t TMPuint16_t;
	uint32_t TMPuint32_t;
	uint64_t TMPuint64_t;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0;

    Message->DataPointer = 0;
    MUCI_Get_uint32_MessageData(Message, &VariableAddress);
    MUCI_Get_uint8_MessageData(Message, &TMPuint8_t);
    VariableType = (MUCI_Types_t)TMPuint8_t;

    switch(VariableType) {
		case MUCI_TYPE_BOOL:
		case MUCI_TYPE_CHAR:
		case MUCI_TYPE_UINT8:
		case MUCI_TYPE_INT8:
			MUCI_Get_uint8_MessageData(Message, &TMPuint8_t);
			*((uint8_t*)VariableAddress) = TMPuint8_t;
			break;
		case MUCI_TYPE_UINT16:
		case MUCI_TYPE_INT16:
			MUCI_Get_uint16_MessageData(Message, &TMPuint16_t);
			*((uint16_t*)VariableAddress) = TMPuint16_t;
			break;
		case MUCI_TYPE_UINT32:
		case MUCI_TYPE_INT32:
		case MUCI_TYPE_FLOAT:
			MUCI_Get_uint32_MessageData(Message, &TMPuint32_t);
			*((uint32_t*)VariableAddress) = TMPuint32_t;
			break;
		case MUCI_TYPE_UINT64:
		case MUCI_TYPE_INT64:
		case MUCI_TYPE_DOUBLE:
			MUCI_Get_uint64_MessageData(Message, &TMPuint64_t);
			*((uint64_t*)VariableAddress) = TMPuint64_t;
			break;
		case MUCI_TYPE_STRING:
			break;
		default:
			break;
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_READ(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t VariableAddress;
    uint32_t VariableAddress_TMP;
    uint16_t VariableSize;
    uint32_t VariableValueTMP;
    uint32_t MessagePosition = 0;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0;

    while (MessagePosition < Message->DataLength)
    {
        VariableAddress = ((uint32_t)Message->Data[MessagePosition++] << 24);
        VariableAddress += ((uint32_t)Message->Data[MessagePosition++] << 16);
        VariableAddress += ((uint32_t)Message->Data[MessagePosition++] << 8);
        VariableAddress += ((uint32_t)Message->Data[MessagePosition++] << 0);
        VariableSize = Message->Data[MessagePosition++];

        Answer->Data[Answer->DataPointer++] = (VariableAddress >> 24) & 0xFF;
        Answer->Data[Answer->DataPointer++] = (VariableAddress >> 16) & 0xFF;
        Answer->Data[Answer->DataPointer++] = (VariableAddress >> 8) & 0xFF;
        Answer->Data[Answer->DataPointer++] = VariableAddress & 0xFF;
        Answer->Data[Answer->DataPointer++] = VariableSize;

        while (0 != VariableSize)
        {
            if (VariableSize >= 2)
            {
                /* Variable is 32 bit */
                VariableAddress_TMP = MUCI_bsp_Address_Calculation(VariableAddress);
                VariableValueTMP = *(uint32_t *)VariableAddress_TMP;
                Answer->Data[Answer->DataPointer++] = (VariableValueTMP >> 24) & 0xFF;
                Answer->Data[Answer->DataPointer++] = (VariableValueTMP >> 16) & 0xFF;
                Answer->Data[Answer->DataPointer++] = (VariableValueTMP >> 8) & 0xFF;
                Answer->Data[Answer->DataPointer++] = VariableValueTMP & 0xFF;
                VariableAddress = VariableAddress + 2;
                VariableSize = VariableSize - 2;
            }
            else
            {
                /* Variable is 16 bit */
                VariableAddress_TMP = MUCI_bsp_Address_Calculation(VariableAddress);
                VariableValueTMP = *(uint16_t *)VariableAddress_TMP;
                Answer->Data[Answer->DataPointer++] = (VariableValueTMP >> 8) & 0xFF;
                Answer->Data[Answer->DataPointer++] = VariableValueTMP & 0xFF;
                VariableAddress = VariableAddress + 1;
                VariableSize = VariableSize - 1;
            }
        }
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_WRITE(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t VariableAddress;
    uint32_t VariableAddress_TMP;
    uint16_t VariableSize;
    uint16_t VariableValueTMP_u16;
    uint32_t VariableValueTMP_u32;
    uint32_t MessagePosition = 0;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0;

    while (MessagePosition < Message->DataLength)
    {
        VariableAddress = ((uint32_t)Message->Data[MessagePosition++] << 24);
        VariableAddress += ((uint32_t)Message->Data[MessagePosition++] << 16);
        VariableAddress += ((uint32_t)Message->Data[MessagePosition++] << 8);
        VariableAddress += ((uint32_t)Message->Data[MessagePosition++] << 0);
        VariableSize = Message->Data[MessagePosition++];

        while (0 != VariableSize)
        {
            if (VariableAddress & 0x01)
            {
                /* odd address (cannot be 32 bit long variable) */
                VariableValueTMP_u16 =  ((uint16_t)Message->Data[MessagePosition++] << 8);
                VariableValueTMP_u16 += ((uint16_t)Message->Data[MessagePosition++]);
                VariableAddress_TMP = MUCI_bsp_Address_Calculation(VariableAddress);
                *(uint16_t *)VariableAddress_TMP = VariableValueTMP_u16;
                VariableAddress = VariableAddress + 1;
                VariableSize = VariableSize - 1;
            }
            else
            {
                if (VariableSize >= 2)
                {
                    /* Variable is 32 bit */
                    VariableValueTMP_u32 =  ((uint32_t)Message->Data[MessagePosition++] << 24);
                    VariableValueTMP_u32 += ((uint32_t)Message->Data[MessagePosition++] << 16);
                    VariableValueTMP_u32 += ((uint32_t)Message->Data[MessagePosition++] << 8);
                    VariableValueTMP_u32 += ((uint32_t)Message->Data[MessagePosition++]);
                    VariableAddress_TMP = MUCI_bsp_Address_Calculation(VariableAddress);
                    *(uint32_t *)VariableAddress_TMP = VariableValueTMP_u32;
                    VariableAddress = VariableAddress + 2;
                    VariableSize = VariableSize - 2;
                }
                else
                {
                    /* Variable is 16 bit */
                    VariableValueTMP_u16 =  ((uint16_t)Message->Data[MessagePosition++] << 8);
                    VariableValueTMP_u16 += ((uint16_t)Message->Data[MessagePosition++]);
                    VariableAddress_TMP = MUCI_bsp_Address_Calculation(VariableAddress);
                    *(uint16_t *)VariableAddress_TMP = VariableValueTMP_u16;
                    VariableAddress = VariableAddress + 1;
                    VariableSize = VariableSize - 1;
                }
            }
        }
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

#ifndef MUCI_BOOT_VERSION
bool MUCI_Command_READ_TABNAMES(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t MessagePosition = 0;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    for (MessagePosition = 0u; MessagePosition < sizeof(tabtitle); MessagePosition++)
    {
        Answer->Data[Answer->DataPointer++] = ((uint8_t *)tabtitle[0])[MessagePosition];
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_READ_MON_TABS(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t MessagePosition = 0;
    uint32_t TabRowPointer = 0;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    //Send Name
    for (TabRowPointer = 0; TabRowPointer < (tabsize[Message->Command - MUCI_READ_MON_TAB0]/ sizeof(var_type)); TabRowPointer++)
    {
      //Name
      for (MessagePosition = 0; MessagePosition <= MAX_CHAR_LEN; MessagePosition++)
      {
        Answer->Data[Answer->DataPointer++] = 0u; /* Dummy */
        Answer->Data[Answer->DataPointer++] = ((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].name[MessagePosition];
      }

      //Unit
      Answer->Data[Answer->DataPointer++] = ((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].unit >> 8;
      Answer->Data[Answer->DataPointer++] = ((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].unit & 0xFF;

      //Type
      Answer->Data[Answer->DataPointer++] = ((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].type >> 8;
      Answer->Data[Answer->DataPointer++] = ((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].type & 0xFF;

      //prt
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].ptr) >> 8) & 0xFF;
      Answer->Data[Answer->DataPointer++] = (((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].ptr) & 0xFF;
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].ptr) >> 24) & 0xFF;
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].ptr) >> 16) & 0xFF;


      //pmax
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmax) >> 8) & 0xFF;
      Answer->Data[Answer->DataPointer++] = (((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmax) & 0xFF;
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmax) >> 24) & 0xFF;
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmax) >> 16) & 0xFF;


      //pmin
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmin) >> 8) & 0xFF;
      Answer->Data[Answer->DataPointer++] = (((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmin) & 0xFF;
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmin) >> 24) & 0xFF;
      Answer->Data[Answer->DataPointer++] = ((((var_type *)tabptr[Message->Command - MUCI_READ_MON_TAB0])[TabRowPointer].pmin) >> 16) & 0xFF;

    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_READ_UNITS(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t MessagePosition = 0;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    for (MessagePosition = 0u; MessagePosition < sizeof(units); MessagePosition++)
    {
        Answer->Data[Answer->DataPointer++] = ((uint8_t *)units)[MessagePosition];
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_READ_TABVALUES(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t MessagePosition = 0;
    uint32_t VariableValueTMP;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    for (MessagePosition = 0; MessagePosition < tabsize[Message->Command - MUCI_READ_TAB0VALUES] / sizeof(var_type); MessagePosition++)
    {
        if ((tabptr[Message->Command - MUCI_READ_TAB0VALUES][MessagePosition].type & TYP_BIT_MASK) == TYP_32BIT)
        {
            VariableValueTMP = *(uint32_t*)(tabptr[Message->Command - MUCI_READ_TAB0VALUES][MessagePosition].ptr);
        }
        else
        {
            VariableValueTMP = *(uint16_t*)(tabptr[Message->Command - MUCI_READ_TAB0VALUES][MessagePosition].ptr);
        }

        Answer->Data[Answer->DataPointer++] = (VariableValueTMP >> 24) & 0xFF;
        Answer->Data[Answer->DataPointer++] = (VariableValueTMP >> 16) & 0xFF;
        Answer->Data[Answer->DataPointer++] = (VariableValueTMP >>  8) & 0xFF;
        Answer->Data[Answer->DataPointer++] = (VariableValueTMP >>  0) & 0xFF;
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_MUCI_READ_BBX(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t StartAddress = 0u;
    uint32_t DataLength = 0u;
    uint32_t MessagePosition = 0u;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    StartAddress =  ((uint32_t)Message->Data[0] << 24) +
                    ((uint32_t)Message->Data[1] << 16) +
                    ((uint32_t)Message->Data[2] << 8) +
                    ((uint32_t)Message->Data[3] << 0);

    DataLength =    ((uint32_t)Message->Data[4] << 8) +
                    ((uint32_t)Message->Data[5] << 0);

    Answer->Data[Answer->DataPointer] = (StartAddress >> 24u) & 0xFFu;
    Answer->DataPointer++;
    Answer->Data[Answer->DataPointer++] = (StartAddress >> 16) & 0xFF;
    Answer->Data[Answer->DataPointer++] = (StartAddress >> 8) & 0xFF;
    Answer->Data[Answer->DataPointer++] = StartAddress  & 0xFF;

    Answer->Data[Answer->DataPointer++] = (DataLength >> 8) & 0xFF;
    Answer->Data[Answer->DataPointer++] = DataLength & 0xFF;

    StartAddress = StartAddress / 2u;
    DataLength = DataLength / 2u;

    for (MessagePosition = 0u; MessagePosition < (DataLength); MessagePosition++)
    {
        Answer->Data[Answer->DataPointer++] = (((uint32_t*)(MUCI_RecorderData))[(StartAddress + MessagePosition) % (MUCI_RECORDER_DATA_LENGTH * MUCI_RECORDER_CHANNEL_NUMBER)] >>  8) & 0xFF;
        Answer->Data[Answer->DataPointer++] = ((uint32_t*)(MUCI_RecorderData))[(StartAddress + MessagePosition) % (MUCI_RECORDER_DATA_LENGTH * MUCI_RECORDER_CHANNEL_NUMBER)]  & 0xFF;
        Answer->Data[Answer->DataPointer++] = (((uint32_t*)(MUCI_RecorderData))[(StartAddress + MessagePosition) % (MUCI_RECORDER_DATA_LENGTH * MUCI_RECORDER_CHANNEL_NUMBER)] >>  24) & 0xFF;
        Answer->Data[Answer->DataPointer++] = (((uint32_t*)(MUCI_RecorderData))[(StartAddress + MessagePosition) % (MUCI_RECORDER_DATA_LENGTH * MUCI_RECORDER_CHANNEL_NUMBER)] >>  16) & 0xFF;
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}
#endif

bool MUCI_Command_REBOOT(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    bool REBOOT_RetVal = true;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    MUCI_bsp_Reboot();

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return REBOOT_RetVal;
}

void (* API_start_address)(void) ;
uint16_t *copy_start_address;

bool MUCI_Command_BOOT_SEND_API(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    bool BOOT_SEND_API_RetVal = true;
    uint32_t StartAddress;
    uint32_t MessagePosition = 0;
    uint16_t DataTMP;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0u;

    if(Message->DataLength)
    {
        StartAddress =  ((uint32_t)Message->Data[0] << 24) +
                        ((uint32_t)Message->Data[1] << 16) +
                        ((uint32_t)Message->Data[2] << 8) +
                        ((uint32_t)Message->Data[3] << 0);

        copy_start_address=(uint16_t *)StartAddress;

        if (0u == Message->Key)
        {
            *(long*)&API_start_address =(long)StartAddress;
        }

        for(MessagePosition = 4; MessagePosition < (Message->DataLength); MessagePosition += 2)
        {
            DataTMP = ((uint16_t)Message->Data[MessagePosition] << 8) +
                      ((uint16_t)Message->Data[MessagePosition + 1] << 0);
            *copy_start_address++= DataTMP;
        }
    }
    else
    {
        API_start_address();
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return BOOT_SEND_API_RetVal;
}



bool MUCI_Command_BOOT_CLEAR_SECTOR(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint8_t FlashSectorNumber;
    uint16_t Status;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0;

    FlashSectorNumber = Message->Data[0];

    Status = MUCI_bsp_CLEAR_FLASH_SECTOR(FlashSectorNumber);

    Answer->Data[Answer->DataPointer++] = (Status >> 8) & 0xFF;
    Answer->Data[Answer->DataPointer++] = Status & 0xFF;

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_BOOT_PROGRAM_FLASH(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t StartAddress;
    uint32_t Quantity;
    uint16_t Status;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0;

    if(Message->DataLength)
    {
        StartAddress =  ((uint32_t)Message->Data[0] << 24) +
                        ((uint32_t)Message->Data[1] << 16) +
                        ((uint32_t)Message->Data[2] << 8) +
                        ((uint32_t)Message->Data[3] << 0);

        Quantity = Message->DataLength - 4;

        Status = MUCI_bsp_PROGRAM_FLASH(StartAddress, Quantity, &Message->Data[4]);
    }

    Answer->Data[Answer->DataPointer++] = Message->Key + 1;

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}

bool MUCI_Command_BOOT_READ_FLASH(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    uint32_t StartAddress;
    uint32_t Quantity;
    uint16_t Status;

    uint16_t x;
    uint16_t Data;

    Answer->Address = MUCI_ANSWER_BASE_ADDRESS + MUCI_GetDeviceAddress();
    Answer->Command = Message->Command;
    Answer->Key = Message->Key;
    Answer->DataPointer = 0;

    StartAddress =  ((uint32_t)Message->Data[0] << 24) +
                    ((uint32_t)Message->Data[1] << 16) +
                    ((uint32_t)Message->Data[2] << 8) +
                    ((uint32_t)Message->Data[3] << 0);

    Quantity =  ((uint16_t)Message->Data[4] << 8) +
                ((uint16_t)Message->Data[5] << 0);

    //Status = MUCI_bsp_READ_FLASH(StartAddress, Quantity, &Message->Data[4]);


    Answer->Data[Answer->DataPointer++] = Message->Data[0];
    Answer->Data[Answer->DataPointer++] = Message->Data[1];
    Answer->Data[Answer->DataPointer++] = Message->Data[2];
    Answer->Data[Answer->DataPointer++] = Message->Data[3];

    Answer->Data[Answer->DataPointer++] = Message->Data[4];
    Answer->Data[Answer->DataPointer++] = Message->Data[5];

    for (x = 0; x < Quantity; x++)
    {
        Data = *(uint16_t*)(StartAddress + x);
        Answer->Data[Answer->DataPointer++] = (Data >> 8) & 0xFF;
        Answer->Data[Answer->DataPointer++] = (Data >> 0) & 0xFF;
    }

    Answer->DataLength = Answer->DataPointer;
    Answer->State = MUCI_PACKET_SEND_STX;
    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    return true;
}
