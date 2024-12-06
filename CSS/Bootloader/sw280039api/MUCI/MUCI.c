#include "MUCI.h"
#include "MUCI_CRCGenerator.h"
#include "MUCI_MonitorCommands.h"

MUCI_Config_t MUCI_Config;
MUCI_Counters_t MUCI_Counters;

bool MUCI_ReceivePacket(MUCI_Packet_t* Message, uint8_t DataBuffer[], uint32_t FIFODataLength)
{
    uint32_t FIFODataPointer;
    uint8_t ReceivedData;

    for(FIFODataPointer = 0u; FIFODataPointer < FIFODataLength; FIFODataPointer++)
    {
        ReceivedData =  DataBuffer[FIFODataPointer];
        if (MUCI_DLE == ReceivedData)
        {
            Message->DLEWas = true;
            continue;
        }
        if (true == Message->DLEWas)
        {
            ReceivedData= ReceivedData ^ MUCI_XDT;
            Message->DLEWas = false;
        }

        switch (Message->State)
        {
            case MUCI_PACKET_IDLE:
                break;
            case MUCI_PACKET_RECEIVE_WAIT_FOR_STX:
                if (MUCI_STX == ReceivedData)
                {
                    Message->CRC = 0xFFFFu;
                    Message->DataPointer = 0u;
                    Message->State = MUCI_PACKET_RECEIVE_ADDRESS;
                }
                break;
            case MUCI_PACKET_RECEIVE_ADDRESS:
                Message->Address = ReceivedData;
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                Message->State = MUCI_PACKET_RECEIVE_COMMAND;
                break;
            case MUCI_PACKET_RECEIVE_COMMAND:
                Message->Command = (MUCI_Commands)ReceivedData;
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                Message->State = MUCI_PACKET_RECEIVE_KEY;
                break;
            case MUCI_PACKET_RECEIVE_KEY:
                Message->Key = ReceivedData;
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                Message->State = MUCI_PACKET_RECEIVE_LENGTH_HIGHBYTE;
                break;
            case MUCI_PACKET_RECEIVE_LENGTH_HIGHBYTE:
                Message->DataLength = ((uint16_t)ReceivedData) << 8;
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                Message->State = MUCI_PACKET_RECEIVE_LENGTH_LOWBYTE;
                break;
            case MUCI_PACKET_RECEIVE_LENGTH_LOWBYTE:
                Message->DataLength = Message->DataLength | (uint16_t)ReceivedData;
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                if (MUCI_DATA_LENGTH < Message->DataLength)
                {
                    /* Not enough buffer for the message */
                    MUCI_Counters.NotEnoughSpaceInTheBuffer++;
                    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
                }
                else if (0u == Message->DataLength)
                {
                    /* No data in the package */
                    Message->State = MUCI_PACKET_RECEIVE_CRC_LOWBYTE;
                }
                else
                {
                    Message->State = MUCI_PACKET_RECEIVE_DATA;
                }
                break;
            case MUCI_PACKET_RECEIVE_DATA:
                Message->Data[Message->DataPointer] = ReceivedData;
                Message->DataPointer = Message->DataPointer + 1u;
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                if (Message->DataPointer >= Message->DataLength)
                {
                    Message->State = MUCI_PACKET_RECEIVE_CRC_LOWBYTE;
                }
                break;
            case MUCI_PACKET_RECEIVE_CRC_LOWBYTE:
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                Message->State = MUCI_PACKET_RECEIVE_CRC_HIGHBYTE;
                break;
            case MUCI_PACKET_RECEIVE_CRC_HIGHBYTE:
                MUCI_CalcCheckSum(&Message->CRC, ReceivedData);
                if (0u == Message->CRC)
                {
                    Message->State = MUCI_PACKET_RECEIVE_ETX;
                }
                else
                {
                    MUCI_Counters.WrongCRC++;
                    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
                }
                break;
            case MUCI_PACKET_RECEIVE_ETX:
                if (MUCI_ETX == ReceivedData)
                {
                    /* Full package received with good CRC */
                    MUCI_Counters.GoodPacketReceived++;
                    Message->State = MUCI_PACKET_RECEIVE_FINISHED;
                }
                else
                {
                    MUCI_Counters.NoETXCharacterReceived++;
                    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
                }
                break;
            case MUCI_PACKET_RECEIVE_FINISHED:
                break;
            default:
                MUCI_Counters.WTF++;
                Message->State = MUCI_PACKET_IDLE;
                break;
        }
    }
    return true;
}

bool MUCI_ProcessPacket(MUCI_Packet_t* Message, MUCI_Packet_t* Answer)
{
    if ((MUCI_PACKET_RECEIVE_FINISHED == Message->State) && (MUCI_PACKET_SEND_FINISHED == Answer->State))
    {
        if (MUCI_GetDeviceAddress() == Message->Address)
        {
            /* Address is own */
            switch(Message->Command)
            {
            	case MUCI_GET_PROTOCOL_VERSION:
            		MUCI_Command_GET_PROTOCOL_VERSION(Message, Answer);
            		break;
                case MUCI_PING:
                    MUCI_Command_PING(Message, Answer);
                    break;
                case MUCI_READ:
                    MUCI_Command_READ(Message, Answer);
                    break;
                case MUCI_WRITE:
                    MUCI_Command_WRITE(Message, Answer);
                    break;
                case MUCI_READ_NEW:
                    MUCI_Command_READ_NEW(Message, Answer);
                    break;
                case MUCI_WRITE_NEW:
                    MUCI_Command_WRITE_NEW(Message, Answer);
                    break;
#ifndef MUCI_BOOT_VERSION
                case MUCI_READ_TABNAMES:
                    MUCI_Command_READ_TABNAMES(Message, Answer);
                    break;
                case MUCI_READ_MON_TAB0:
                case MUCI_READ_MON_TAB1:
                case MUCI_READ_MON_TAB2:
                case MUCI_READ_MON_TAB3:
                case MUCI_READ_MON_TAB4:
                case MUCI_READ_MON_TAB5:
                case MUCI_READ_MON_TAB6:
                case MUCI_READ_MON_TAB7:
                case MUCI_READ_MON_TAB8:
                case MUCI_READ_MON_TAB9:
                    MUCI_Command_READ_MON_TABS(Message, Answer);
                    break;
                case MUCI_READ_UNITS:
                    MUCI_Command_READ_UNITS(Message, Answer);
                    break;
                case MUCI_READ_TAB0VALUES:
                case MUCI_READ_TAB1VALUES:
                case MUCI_READ_TAB2VALUES:
                case MUCI_READ_TAB3VALUES:
                case MUCI_READ_TAB4VALUES:
                case MUCI_READ_TAB5VALUES:
                case MUCI_READ_TAB6VALUES:
                case MUCI_READ_TAB7VALUES:
                case MUCI_READ_TAB8VALUES:
                case MUCI_READ_TAB9VALUES:
                    MUCI_Command_READ_TABVALUES(Message, Answer);
                    break;
                case MUCI_READ_BBX:
                    MUCI_Command_MUCI_READ_BBX(Message, Answer);
                    break;
#endif
                case MUCI_REBOOT:
                    MUCI_Command_REBOOT(Message, Answer);
                    break;
                case MUCI_BOOT_SEND_API:
                    MUCI_Command_BOOT_SEND_API(Message, Answer);
                    break;
                case MUCI_BOOT_CLEAR_SECTOR:
                    MUCI_Command_BOOT_CLEAR_SECTOR(Message, Answer);
                    break;
                case MUCI_BOOT_READ_FLASH:
                	MUCI_Command_BOOT_READ_FLASH(Message, Answer);
                	break;
                case MUCI_BOOT_PROGRAM_FLASH:
                    MUCI_Command_BOOT_PROGRAM_FLASH(Message, Answer);
                    break;
                default:
                    MUCI_Counters.NotSupportedCommand++;
                    Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
                    break;
            }
        }
        else
        {
            /* Skip packet not own address */
            //Message->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
        }

    }
    return true;
}

bool MUCI_PacketRouter(MUCI_Packet_t* Source, MUCI_Packet_t* Destination, uint8_t Address)
{
    if ((MUCI_PACKET_RECEIVE_FINISHED == Source->State) && (MUCI_PACKET_SEND_FINISHED == Destination->State))
    {
        if (Address != Source->Address)
        {
            memcpy(Destination, Source, sizeof(MUCI_Packet_t));
            Source->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
            Destination->State = MUCI_PACKET_SEND_STX;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool MUCI_PacketRouterNoMoreRoute(MUCI_Packet_t* Source)
{
    if (MUCI_PACKET_RECEIVE_FINISHED == Source->State)
    {
        Source->State = MUCI_PACKET_RECEIVE_WAIT_FOR_STX;
    }
    return true;
}

bool MUCI_SendPacket(MUCI_Packet_t* Message, uint8_t DataBuffer[], uint32_t* DataLength)
{
    uint32_t DataPointer;
    uint8_t SendData;

    for (DataPointer = 0; DataPointer < *DataLength; DataPointer++)
    {
        switch(Message->State)
        {
            case MUCI_PACKET_IDLE:
                break;
            case MUCI_PACKET_SEND_STX:
                SendData = MUCI_STX;
                Message->CRC = 0xFFFFu;
                Message->DataPointer = 0u;
                Message->NextState = MUCI_PACKET_SEND_ADDRESS;
                break;
            case MUCI_PACKET_SEND_ADDRESS:
                SendData = Message->Address;
                Message->NextState = MUCI_PACKET_SEND_COMMAND;
                break;
            case MUCI_PACKET_SEND_COMMAND:
                SendData = Message->Command;
                Message->NextState = MUCI_PACKET_SEND_KEY;
                break;
            case MUCI_PACKET_SEND_KEY:
                SendData = Message->Key;
                Message->NextState = MUCI_PACKET_SEND_LENGTH_HIGHBYTE;
                break;
            case MUCI_PACKET_SEND_LENGTH_HIGHBYTE:
                SendData = (Message->DataLength >> 8u) & 0xFFu;
                Message->NextState = MUCI_PACKET_SEND_LENGTH_LOWBYTE;
                break;
            case MUCI_PACKET_SEND_LENGTH_LOWBYTE:
                SendData = Message->DataLength & 0xFFu;
                if (0u == Message->DataLength)
                {
                    /* No data in the command */
                    Message->NextState = MUCI_PACKET_SEND_CRC_LOWBYTE;
                }
                else
                {
                    Message->NextState = MUCI_PACKET_SEND_DATA;
                }
                break;
            case MUCI_PACKET_SEND_DATA:
                SendData = Message->Data[Message->DataPointer];
                Message->NextState = MUCI_PACKET_SEND_DATA;
                break;
            case MUCI_PACKET_SEND_CRC_LOWBYTE:
                SendData = Message->CRC & 0xFFu;
                Message->NextState = MUCI_PACKET_SEND_CRC_HIGHBYTE;
                break;
            case MUCI_PACKET_SEND_CRC_HIGHBYTE:
                SendData = (Message->CRC >> 8u) & 0xFFu;
                Message->NextState = MUCI_PACKET_SEND_ETX;
                break;
            case MUCI_PACKET_SEND_ETX:
                SendData = MUCI_ETX;
                Message->NextState = MUCI_PACKET_SEND_FINISHED;
                break;
            case MUCI_PACKET_SEND_FINISHED:
                break;
            default:
                MUCI_Counters.WTF++;
                Message->State = MUCI_PACKET_IDLE;
                break;
        }

        if ((MUCI_PACKET_SEND_STX == Message->State) || (MUCI_PACKET_SEND_ETX == Message->State))
        {
            DataBuffer[DataPointer] = SendData;
            Message->State = Message->NextState;
        }
        else if ((MUCI_PACKET_IDLE == Message->State) || (MUCI_PACKET_SEND_FINISHED == Message->State))
        {
            *DataLength = DataPointer;
            break;
        }
        else
        {
            if ((true != Message->DLEWas) && MUCI_NeedDataDLE(SendData))
            {
                DataBuffer[DataPointer] = MUCI_DLE;
                Message->DLEWas = true;
            }
            else if (MUCI_NeedDataDLE(SendData))
            {
                DataBuffer[DataPointer] =SendData ^ MUCI_XDT;
                Message->DLEWas = false;
                if ((MUCI_PACKET_SEND_CRC_LOWBYTE != Message->State) && (MUCI_PACKET_SEND_CRC_HIGHBYTE != Message->State))
                {
                    MUCI_CalcCheckSum(&Message->CRC, SendData & 0xFFu);
                }
                if (MUCI_PACKET_SEND_DATA == Message->State)
                {
                    Message->DataPointer++;
                    if (Message->DataPointer >= Message->DataLength)
                    {
                        Message->NextState = MUCI_PACKET_SEND_CRC_LOWBYTE;
                    }
                }
                Message->State = Message->NextState;
            }
            else
            {
                DataBuffer[DataPointer] = SendData;
                if ((MUCI_PACKET_SEND_CRC_LOWBYTE != Message->State) && (MUCI_PACKET_SEND_CRC_HIGHBYTE != Message->State))
                {
                    MUCI_CalcCheckSum(&Message->CRC, SendData & 0xFFu);
                }
                if (MUCI_PACKET_SEND_DATA == Message->State)
                {
                    Message->DataPointer++;
                    if (Message->DataPointer >= Message->DataLength)
                    {
                        Message->NextState = MUCI_PACKET_SEND_CRC_LOWBYTE;
                    }
                }
                Message->State = Message->NextState;
            }
        }
    }
    return true;
}

bool MUCI_NeedDataDLE(uint8_t data)
{
    bool ReturnValue = false;

    switch (data & 0xFFu)
    {
      case MUCI_STX:
      case MUCI_ETX:
      case MUCI_DLE:
          ReturnValue = true;
       break;
      default:
          ReturnValue = false;
       break;
    }
    return ReturnValue;
}

void MUCI_SetDeviceAddress(uint8_t DeviceAddressToSet)
{
    MUCI_Config.DeviceAddress = DeviceAddressToSet;
}

uint8_t  MUCI_GetDeviceAddress(void)
{
    return MUCI_Config.DeviceAddress;
}

void MUCI_SetApplicationState(MUCI_ApplicationState_t ApplicationStateToSet)
{
    MUCI_Config.ApplicationState = ApplicationStateToSet;
}

MUCI_ApplicationState_t MUCI_GetApplicationState(void)
{
    return MUCI_Config.ApplicationState;
}

MUCI_ErrorTypes_t MUCI_Push_uint8_MessageData(MUCI_Packet_t* Message, uint8_t MessageData)
{
	MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

	if (Message->DataPointer <= MUCI_DATA_LENGTH)
	{
		Message->Data[Message->DataPointer] = MessageData;
		Message->DataPointer = Message->DataPointer + 1u;

		Status = MUCI_ERROR_NO_ERROR;
	}
	else
	{
		Status = MUCI_ERROR_BUFFER_OVERFLOW;
	}

	return Status;
}

MUCI_ErrorTypes_t MUCI_Get_uint8_MessageData(MUCI_Packet_t* Message, uint8_t *MessageData)
{
	MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

	if (Message->DataPointer <= Message->DataLength)
	{
		*MessageData = Message->Data[Message->DataPointer];
		Message->DataPointer = Message->DataPointer + 1u;

		Status = MUCI_ERROR_NO_ERROR;
	}
	else
	{
		Status = MUCI_ERROR_BUFFER_OVERFLOW;
	}

	return Status;
}

MUCI_ErrorTypes_t MUCI_Push_uint16_MessageData(MUCI_Packet_t* Message, uint16_t MessageData)
{
	MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

	if ((Message->DataPointer + 2) <= MUCI_DATA_LENGTH)
	{
		Message->Data[Message->DataPointer++] = (MessageData >> 8u) & 0xFF;
		Message->Data[Message->DataPointer++] = MessageData & 0xFF;

		Status = MUCI_ERROR_NO_ERROR;
	}
	else
	{
		Status = MUCI_ERROR_BUFFER_OVERFLOW;
	}

	return Status;
}

MUCI_ErrorTypes_t MUCI_Get_uint16_MessageData(MUCI_Packet_t* Message, uint16_t *MessageData)
{
	uint16_t DataPointer = Message->DataPointer;
    MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

    if ((DataPointer + 2) <= Message->DataLength)
    {
    	*MessageData = (uint16_t)Message->Data[DataPointer++] << 8u;
    	*MessageData += (uint16_t)Message->Data[DataPointer++];

    	Message->DataPointer = DataPointer;

    	Status = MUCI_ERROR_NO_ERROR;
    }
    else
    {
    	Status = MUCI_ERROR_BUFFER_NO_MORE_DATA;
    }

    return Status;
}

MUCI_ErrorTypes_t MUCI_Push_uint32_MessageData(MUCI_Packet_t* Message, uint32_t MessageData)
{
	MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

	if ((Message->DataPointer + 4) <= MUCI_DATA_LENGTH)
	{
		Message->Data[Message->DataPointer++] = (MessageData >> 24u) & 0xFF;
		Message->Data[Message->DataPointer++] = (MessageData >> 16u) & 0xFF;
		Message->Data[Message->DataPointer++] = (MessageData >> 8u) & 0xFF;
		Message->Data[Message->DataPointer++] = MessageData & 0xFF;

		Status = MUCI_ERROR_NO_ERROR;
	}
	else
	{
		Status = MUCI_ERROR_BUFFER_OVERFLOW;
	}

	return Status;
}

MUCI_ErrorTypes_t MUCI_Get_uint32_MessageData(MUCI_Packet_t* Message, uint32_t *MessageData)
{
	uint16_t DataPointer = Message->DataPointer;
    MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

    if ((DataPointer + 4) <= Message->DataLength)
    {
    	*MessageData = (uint32_t)Message->Data[DataPointer++] << 24u;
    	*MessageData += (uint32_t)Message->Data[DataPointer++] << 16u;
    	*MessageData += (uint32_t)Message->Data[DataPointer++] << 8u;
    	*MessageData += (uint32_t)Message->Data[DataPointer++];

    	Message->DataPointer = DataPointer;

    	Status = MUCI_ERROR_NO_ERROR;
    }
    else
    {
    	Status = MUCI_ERROR_BUFFER_NO_MORE_DATA;
    }

    return Status;
}

MUCI_ErrorTypes_t MUCI_Push_uint64_MessageData(MUCI_Packet_t* Message, uint64_t MessageData)
{
	uint16_t DataPointer = Message->DataPointer;
	MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

	if ((Message->DataPointer + 8) <= MUCI_DATA_LENGTH)
	{
		Message->Data[DataPointer++] = (MessageData >> 56u) & 0xFF;
		Message->Data[DataPointer++] = (MessageData >> 48u) & 0xFF;
		Message->Data[DataPointer++] = (MessageData >> 40u) & 0xFF;
		Message->Data[DataPointer++] = (MessageData >> 32u) & 0xFF;
		Message->Data[DataPointer++] = (MessageData >> 24u) & 0xFF;
		Message->Data[DataPointer++] = (MessageData >> 16u) & 0xFF;
		Message->Data[DataPointer++] = (MessageData >> 8u) & 0xFF;
		Message->Data[DataPointer++] = MessageData & 0xFF;

		Message->DataPointer = DataPointer;

		Status = MUCI_ERROR_NO_ERROR;
	}
	else
	{
		Status = MUCI_ERROR_BUFFER_OVERFLOW;
	}

	return Status;
}

MUCI_ErrorTypes_t MUCI_Get_uint64_MessageData(MUCI_Packet_t* Message, uint64_t *MessageData)
{
	uint16_t DataPointer = Message->DataPointer;
    MUCI_ErrorTypes_t Status = MUCI_ERROR_NO_ERROR;

    if ((DataPointer + 8) <= Message->DataLength)
    {

    	*MessageData = (uint64_t)Message->Data[DataPointer++] << 56u;
    	*MessageData += (uint64_t)Message->Data[DataPointer++] << 48u;
    	*MessageData += (uint64_t)Message->Data[DataPointer++] << 40u;
    	*MessageData += (uint64_t)Message->Data[DataPointer++] << 32u;
    	*MessageData += (uint64_t)Message->Data[DataPointer++] << 24u;
    	*MessageData += (uint64_t)Message->Data[DataPointer++] << 16u;
    	*MessageData += (uint64_t)Message->Data[DataPointer++] << 8u;
    	*MessageData += (uint64_t)Message->Data[DataPointer++];

    	Message->DataPointer = DataPointer;

    	Status = MUCI_ERROR_NO_ERROR;
    }
    else
    {
    	Status = MUCI_ERROR_BUFFER_NO_MORE_DATA;
    }

    return Status;
}
