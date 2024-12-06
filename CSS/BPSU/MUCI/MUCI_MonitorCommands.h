/*
 * MUCI_MonitorCommands.h
 *
 *  Created on: 2018. szept. 11.
 *      Author: Szako
 */

#ifndef MUCI_MONITORCOMMANDS_H_
    #define MUCI_MONITORCOMMANDS_H_
    #include "MUCI.h"

	typedef enum {
		MUCI_TYPE_BOOL = 0u,
		MUCI_TYPE_CHAR,
		MUCI_TYPE_UINT8,
		MUCI_TYPE_INT8,
		MUCI_TYPE_UINT16,
		MUCI_TYPE_INT16,
		MUCI_TYPE_UINT32,
		MUCI_TYPE_INT32,
		MUCI_TYPE_FLOAT,
		MUCI_TYPE_UINT64,
		MUCI_TYPE_INT64,
		MUCI_TYPE_DOUBLE,
		MUCI_TYPE_STRING,
		MUCI_TYPE_COUNT
	} MUCI_Types_t;

	bool MUCI_Command_GET_PROTOCOL_VERSION(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_PING(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_READ(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_WRITE(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_READ_NEW(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_WRITE_NEW(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_READ_TABNAMES(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_READ_MON_TABS(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_READ_UNITS(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_READ_TABVALUES(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_REBOOT(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_BOOT_SEND_API(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_MUCI_READ_BBX(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);

    bool MUCI_Command_BOOT_CLEAR_SECTOR(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_BOOT_PROGRAM_FLASH(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);
    bool MUCI_Command_BOOT_READ_FLASH(MUCI_Packet_t* Message, MUCI_Packet_t* Answer);

#endif /* MUCI_MONITORCOMMANDS_H_ */
