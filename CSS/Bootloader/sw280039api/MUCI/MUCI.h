/*
 * MUCI.h
 *
 *  Created on: 2018. szept. 4.
 *      Author: Szako
 */

#ifndef MUCI_H_
    #define MUCI_H_

    #include "stdint.h"
    #include "stdbool.h"
	#include "MUCI_bsp.h"

    #ifndef MUCI_DATA_LENGTH
        #define MUCI_DATA_LENGTH    1024u
        #warning "MUCI buffer data length not defined default value used!"
    #endif

	#define MUCI_STX 0x3Cu                         /*!< Start of message character*/
    #define MUCI_DLE 0x3Du                         /*!< Escape of message character*/
    #define MUCI_ETX 0x3Eu                         /*!< End of message character*/
    #define MUCI_XDT 0x30u                         /*!< Start of message character*/

	#define MUCI_DEFAULT_DEVICE_ADDRESS 	0u
    #define MUCI_ANSWER_BASE_ADDRESS    	0x80u

	#define MUCI_MAJOR_VERSION	2u
	#define MUCI_MINOR_VERSION	1u
	#define MUCI_PATCH_LEVEL	1u

    typedef enum {
    	MUCI_GET_PROTOCOL_VERSION	= 0x01,		/*!< Get protocol version*/
        MUCI_BOOT_SEND_API  	    = 0x02,     /*!< Send FLASH API @ boot time*/
        MUCI_READ              		= 0x10,     /*!< Read single/multiple variables */
		MUCI_READ_NEW 				= 0x11,
		MUCI_WRITE_NEW 				= 0x12,
        MUCI_BOOT_READ         		= 0x16,     /*!< Read single/multiple variables @ boot time*/
        MUCI_BOOT_WRITE         	= 0x20,     /*!< Write single/multiple variables @ boot time*/
        MUCI_WRITE              	= 0x23,     /*!< Write single/multiple variables */
        MUCI_READ_BBX           	= 0x24,     /*!< read black box content */
        MUCI_READ_MON_TAB0	     	= 0x40,     /*!< Read the 0. tab descriptor*/
        MUCI_READ_MON_TAB1      	= 0x41,     /*!< Read the 1. tab descriptor*/
        MUCI_READ_MON_TAB2      	= 0x42,     /*!< Read the 2. tab descriptor*/
        MUCI_READ_MON_TAB3      	= 0x43,     /*!< Read the 3. tab descriptor*/
        MUCI_READ_MON_TAB4      	= 0x44,     /*!< Read the 4. tab descriptor*/
        MUCI_READ_MON_TAB5      	= 0x45,     /*!< Read the 5. tab descriptor*/
        MUCI_READ_MON_TAB6      	= 0x46,     /*!< Read the 6. tab descriptor*/
        MUCI_READ_MON_TAB7      	= 0x47,     /*!< Read the 7. tab descriptor*/
        MUCI_READ_MON_TAB8      	= 0x48,     /*!< Read the 8. tab descriptor*/
        MUCI_READ_MON_TAB9      	= 0x49,     /*!< Read the 9. tab descriptor*/
        MUCI_READ_ERROR_HISTORY 	= 0x51,     /*!< read error history */
        MUCI_BOOT_CLEAR_SECTOR  	= 0x53,
        MUCI_READ_TABNAMES      	= 0x55,     /*!< Read the name of the tabs in user mode (only for 0th address DSP)*/
        MUCI_CLEAR_EEPROM       	= 0x58,
        MUCI_BOOT_READ_FLASH    	= 0x5A,
        MUCI_BOOT_PROGRAM_FLASH 	= 0x5F,
        MUCI_MODBUS_READ        	= 0x60,
        MUCI_MODBUS_WRITE       	= 0x61,
        MUCI_READ_SPI_RAM       	= 0x75,     /*!< read spi ram (bbx, error history */
        MUCI_READ_UNITS         	= 0x79,     /*!< read unit table */
        MUCI_REBOOT             	= 0x80,     /*!< Reboot DSP */
        MUCI_PING               	= 0x99,     /*!< ping DSP */
        MUCI_READ_TAB0VALUES    	= 0xA0u,    /*!< read all tabvalues in 0. tab */
        MUCI_READ_TAB1VALUES    	= 0xA1,     /*!< read all tabvalues in 1. tab */
        MUCI_READ_TAB2VALUES    	= 0xA2,     /*!< read all tabvalues in 2. tab */
        MUCI_READ_TAB3VALUES    	= 0xA3,     /*!< read all tabvalues in 3. tab */
        MUCI_READ_TAB4VALUES   		= 0xA4,     /*!< read all tabvalues in 4. tab */
        MUCI_READ_TAB5VALUES    	= 0xA5,     /*!< read all tabvalues in 5. tab */
        MUCI_READ_TAB6VALUES    	= 0xA6,     /*!< read all tabvalues in 6. tab */
        MUCI_READ_TAB7VALUES    	= 0xA7,     /*!< read all tabvalues in 7. tab */
        MUCI_READ_TAB8VALUES    	= 0xA8,     /*!< read all tabvalues in 8. tab */
        MUCI_READ_TAB9VALUES    	= 0xA9,     /*!< read all tabvalues in 9. tab */
		MUCI_COMMAND_ERROR			= 0xFF		/*!< Error at command processing */
    } MUCI_Commands;

    typedef enum {
        MUCI_PACKET_IDLE,

        MUCI_PACKET_RECEIVE_WAIT_FOR_STX,
        MUCI_PACKET_RECEIVE_ADDRESS,
        MUCI_PACKET_RECEIVE_COMMAND,
        MUCI_PACKET_RECEIVE_KEY,
        MUCI_PACKET_RECEIVE_LENGTH_HIGHBYTE,
        MUCI_PACKET_RECEIVE_LENGTH_LOWBYTE,
        MUCI_PACKET_RECEIVE_DATA,
        MUCI_PACKET_RECEIVE_CRC_LOWBYTE,
        MUCI_PACKET_RECEIVE_CRC_HIGHBYTE,
        MUCI_PACKET_RECEIVE_ETX,
        MUCI_PACKET_RECEIVE_FINISHED,

        MUCI_PACKET_SEND_STX,
        MUCI_PACKET_SEND_ADDRESS,
        MUCI_PACKET_SEND_COMMAND,
        MUCI_PACKET_SEND_KEY,
        MUCI_PACKET_SEND_LENGTH_HIGHBYTE,
        MUCI_PACKET_SEND_LENGTH_LOWBYTE,
        MUCI_PACKET_SEND_DATA,
        MUCI_PACKET_SEND_CRC_LOWBYTE,
        MUCI_PACKET_SEND_CRC_HIGHBYTE,
        MUCI_PACKET_SEND_ETX,
        MUCI_PACKET_SEND_FINISHED
    } MUCI_PacketState;

    typedef struct {
        uint8_t Address;
        MUCI_Commands Command;
        uint8_t Key;
        uint16_t DataLength;
        uint8_t Data[MUCI_DATA_LENGTH];
        uint16_t DataPointer;
        uint16_t CRC;
        bool DLEWas;
        MUCI_PacketState State;
        MUCI_PacketState NextState;
    } MUCI_Packet_t;

    typedef enum {
        MUCI_DEVICESTATE_APP_RUN           = 0u,
        MUCI_DEVICESTATE_BOOT_RUN_CHK_OK   = 1u,
        MUCI_DEVICESTATE_BOOT_RUN_CHK_NOK  = 2u,
        MUCI_DEVICESTATE_FLASHAPI_RUN      = 3u
    } MUCI_ApplicationState_t;

    typedef enum {
    	MUCI_ERROR_NO_ERROR = 0u,
		MUCI_ERROR_BUFFER_OVERFLOW,
		MUCI_ERROR_BUFFER_NO_MORE_DATA
    } MUCI_ErrorTypes_t;

    typedef struct {
        uint8_t DeviceAddress;
        MUCI_ApplicationState_t ApplicationState;
    } MUCI_Config_t;

    typedef struct {
        uint16_t NotEnoughSpaceInTheBuffer;
        uint16_t WrongCRC;
        uint16_t NoETXCharacterReceived;
        uint16_t GoodPacketReceived;
        uint16_t WTF;
        uint16_t NotSupportedCommand;
    } MUCI_Counters_t;

    typedef enum {
    	MUCI_ENDIANNES_8BIT_BIG_ENDIAN = 0u,
		MUCI_ENDIANNES_8BIT_LITTLE_ENDIAN,
		MUCI_ENDIANNES_16BIT_BID_ENDIAN,
		MUCI_ENDIANNES_16BIT_LITTLE_ENDIAN
    } MUCI_Endianness_t;

    extern MUCI_Counters_t MUCI_Counters;

    bool MUCI_ReceivePacket(MUCI_Packet_t* Message, uint8_t DataBuffer[], uint32_t FIFODataLength);
    bool MUCI_ProcessPacket(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_PacketRouter(MUCI_Packet_t* Source, MUCI_Packet_t* Destination, uint8_t Address);
    bool MUCI_PacketRouterNoMoreRoute(MUCI_Packet_t* Source);
    bool MUCI_SendPacket(MUCI_Packet_t* Message, uint8_t DataBuffer[], uint32_t* DataLength);
    void MUCI_CalcCheckSum(uint16_t *chksum, uint8_t data);
    bool MUCI_NeedDataDLE(uint8_t data);
    void MUCI_SetDeviceAddress(uint8_t DeviceAddressToSet);
    uint8_t  MUCI_GetDeviceAddress(void);
    void MUCI_SetApplicationState(MUCI_ApplicationState_t ApplicationStateToSet);
    MUCI_ApplicationState_t MUCI_GetApplicationState(void);

    MUCI_ErrorTypes_t MUCI_Push_uint8_MessageData(MUCI_Packet_t* Message, uint8_t MessageData);
    MUCI_ErrorTypes_t MUCI_Get_uint8_MessageData(MUCI_Packet_t* Message, uint8_t *MessageData);
    MUCI_ErrorTypes_t MUCI_Push_uint16_MessageData(MUCI_Packet_t* Message, uint16_t MessageData);
    MUCI_ErrorTypes_t MUCI_Get_uint16_MessageData(MUCI_Packet_t* Message, uint16_t *MessageData);
    MUCI_ErrorTypes_t MUCI_Push_uint32_MessageData(MUCI_Packet_t* Message, uint32_t MessageData);
    MUCI_ErrorTypes_t MUCI_Get_uint32_MessageData(MUCI_Packet_t* Message, uint32_t *MessageData);
    MUCI_ErrorTypes_t MUCI_Push_uint64_MessageData(MUCI_Packet_t* Message, uint64_t MessageData);
    MUCI_ErrorTypes_t MUCI_Get_uint64_MessageData(MUCI_Packet_t* Message, uint64_t *MessageData);
#endif /* MUCI_H_ */
