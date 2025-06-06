/*--------------------------------------------------------*/
/* F02\Types.h                                            */
/* F021 Flash API v1.58                                   */
/*--------------------------------------------------------*/

// $TI Release: F28003x Support Library v4.01.00.00 $
// $Release Date: 03-03-2022 $
// $Copyright:
// Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $


/*!
    \file F021\Types.h
    \brief Types used by the F021 API.

    These are the types used by the F021 API.

*/

#ifndef F021_TYPES_H_
#define F021_TYPES_H_

/*****************************************************************************/
/* GLOBAL DEFINITIONS                                                        */
/*****************************************************************************/
#if (defined(__TMS320C28xx__) && __TI_COMPILER_VERSION__ < 6004000)
#if !defined(__GNUC__)
#error �F021 Flash API requires GCC language extensions. Use the �gcc option.�
#endif
#endif


#if defined(__ICCARM__)             /* IAR EWARM Compiler */
#define ATTRIBUTE_PACKED  __packed
#elif defined(__TMS320C28XX__)      /* TI CGT C28xx compilers */
#define ATTRIBUTE_PACKED
#else                               /* all other compilers */
#define ATTRIBUTE_PACKED    __attribute__((packed))
#endif

#define HIGH_BYTE_FIRST     0
#define LOW_BYTE_FIRST      1

#define NULLPTR (void *)0U

#ifndef TRUE
   #define TRUE              1
#endif

#ifndef FALSE
   #define FALSE             0
#endif

/*****************************************************************************/
/* ENDIANESS                                                                 */
/*****************************************************************************/
#if defined(_LITTLE_ENDIAN)
   #define CPU_BYTE_ORDER    LOW_BYTE_FIRST
#else
   #define CPU_BYTE_ORDER    HIGH_BYTE_FIRST
#endif

/*****************************************************************************/
/* TYPE DEFINITIONS                                                          */
/*****************************************************************************/
#if defined(__TMS320C28XX__)
typedef unsigned char          boolean;
typedef unsigned int           uint8; //This is 16bits in C28x
typedef unsigned int           uint16;
typedef unsigned long int      uint32;
typedef unsigned long long int uint64;
#endif


typedef struct
{
   uint32 au32StatusWord[4];
}  ATTRIBUTE_PACKED Fapi_FlashStatusWordType;

/*!
    \brief This contains all the possible modes used in the Fapi_IssueAsyncProgrammingCommand().
*/
typedef enum
{
   Fapi_AutoEccGeneration, /* This is the default mode for the command and will auto generate the ecc for the provided data buffer */
   Fapi_DataOnly,       /* Command will only process the data buffer */
   Fapi_EccOnly,        /* Command will only process the ecc buffer */
   Fapi_DataAndEcc         /* Command will process data and ecc buffers */
}  ATTRIBUTE_PACKED Fapi_FlashProgrammingCommandsType;

/*!
    \brief This is used to indicate which Flash bank is being used.
*/
typedef enum
{
   Fapi_FlashBank0,
   Fapi_FlashBank1,
   Fapi_FlashBank2
}  ATTRIBUTE_PACKED Fapi_FlashBankType;


/*!
    \brief This is used to indicate which Flash sector is being used.
*/
typedef enum
{
   Fapi_FlashSector0,
   Fapi_FlashSector1,
   Fapi_FlashSector2,
   Fapi_FlashSector3,
   Fapi_FlashSector4,
   Fapi_FlashSector5,
   Fapi_FlashSector6,
   Fapi_FlashSector7,
   Fapi_FlashSector8,
   Fapi_FlashSector9,
   Fapi_FlashSector10,
   Fapi_FlashSector11,
   Fapi_FlashSector12,
   Fapi_FlashSector13,
   Fapi_FlashSector14,
   Fapi_FlashSector15
}  ATTRIBUTE_PACKED Fapi_FlashSectorType;

/*!
    \brief This contains all the possible Flash State Machine commands.
*/
typedef enum
{
   Fapi_ProgramData    = 0x0002,
   Fapi_EraseSector    = 0x0006,
   Fapi_EraseBank      = 0x0008,
   Fapi_ClearStatus    = 0x0010,
   Fapi_ProgramResume  = 0x0014,
   Fapi_EraseResume    = 0x0016,
   Fapi_ClearMore      = 0x0018
}  ATTRIBUTE_PACKED Fapi_FlashStateCommandsType;

typedef  uint32 Fapi_FlashStatusType;

/*!
    \brief This contains all the possible Flash State Machine commands.
*/
typedef enum
{
   Fapi_NormalRead = 0x0
}  ATTRIBUTE_PACKED Fapi_FlashReadMarginModeType;

/*!
    \brief This is the master type containing all possible returned status codes.
    Design: C2000FLASHAPI-139
*/
typedef enum
{
   Fapi_Status_Success=0,           /* Function completed successfully */
   Fapi_Status_FsmBusy,             /* FSM is Busy */
   Fapi_Status_FsmReady,            /* FSM is Ready */
   Fapi_Status_AsyncBusy,           /* Async function operation is Busy */
   Fapi_Status_AsyncComplete,       /* Async function operation is Complete */
   Fapi_Error_Fail=500,             /* Generic Function Fail code */
   Fapi_Error_OtpChecksumMismatch,  /* Returned if OTP checksum does not match expected value */
   Fapi_Error_InvalidDelayValue,    /* Returned if the Calculated RWAIT value exceeds 15  - Legacy Error */
   Fapi_Error_InvalidHclkValue,     /* Returned if FClk is above max FClk value - FClk is a calculated from HClk and RWAIT/EWAIT */
   Fapi_Error_InvalidCpu,           /* Returned if the specified Cpu does not exist */
   Fapi_Error_InvalidBank,          /* Returned if the specified bank does not exist */
   Fapi_Error_InvalidAddress,       /* Returned if the specified Address does not exist in Flash or OTP */
   Fapi_Error_InvalidReadMode,      /* Returned if the specified read mode does not exist */
   Fapi_Error_AsyncIncorrectDataBufferLength,
   Fapi_Error_AsyncIncorrectEccBufferLength,
   Fapi_Error_AsyncDataEccBufferLengthMismatch,
   Fapi_Error_FeatureNotAvailable,  /* FMC feature is not available on this device */
   Fapi_Error_FlashRegsNotWritable, /* Returned if Flash registers are not writable */
   Fapi_Error_InvalidCPUID,          /* Returned if OTP has an invalid CPUID */
   Fapi_Error_NoActiveCommand,       /* Returned if FSM does not have a command to suspend */
   Fapi_Error_InvalidBaseRegCntlAddress,     /* Returned if base address of register control is incorrect */
   Fapi_Warning_BaseRegCntlAddressMismatch     /* Returned if base address of register control is incorrect */
}  ATTRIBUTE_PACKED Fapi_StatusType;


typedef union
{
    volatile struct
   {
      uint16 ChecksumLength:16;  /* 0x150 bits 15:0 */
      uint16 OtpVersion:16;      /* 0x150 bits 31:16 */
      uint32 OtpChecksum;        /* 0x154 bits 31:0 */
      uint16 NumberOfBanks:16;   /* 0x158 bits 15:0 */
      uint16 NumberOfSectors:16; /* 0x158 bits 31:16 */
      uint16 MemorySize:16;      /* 0x15C bits 15:0 */
      uint16 Package:16;         /* 0x15C bits 31:16 */
      uint16 SiliconRevision:8;  /* 0x160 bits 7:0 */
      uint16 AsicNumber_23_8:8; /* 0x160 bits 31:8 */
      uint16 AsicNumber_31_24:16; /* 0x160 bits 31:8 */
      uint32 LotNumber;          /* 0x164 bits 31:0 */
      uint16 WaferNumber:16;     /* 0x168 bits 15:0 */
      uint16 Flowbits:16;        /* 0x168 bits 31:16 */
      uint16 YCoordinate:16;     /* 0x16C bits 15:0 */
      uint16 XCoordinate:16;     /* 0x16C bits 31:16 */
      uint16 EVSU:8;             /* 0x170 bits 7:0 */
      uint16 PVSU:8;             /* 0x170 bits 15:8 */
      uint16 ESU:8;              /* 0x170 bits 23:16 */
      uint16 PSU:8;              /* 0x170 bits 31:24 */
      uint16 CVSU:12;            /* 0x174 bits 11:0 */
      uint16 Add_EXEZSU:4;       /* 0x174 bits 15:12 */
      uint16 PVAcc:8;            /* 0x174 bits 23:16 */
      uint16 RVSU:8;             /* 0x174 bits 31:24 */
      uint16 PVH2:8;             /* 0x178 bits 7:0 */
      uint16 PVH:8;              /* 0x178 bits 15:8 */
      uint16 RH:8;               /* 0x178 bits 23:16 */
      uint16 PH:8;               /* 0x178 bits 31:24 */
      uint16 SmFrequency:12;     /* 0x17C bits 11:0 */
      uint16 VSTAT:4;            /* 0x17C bits 15:12 */
      uint16 Sequence:8;         /* 0x17C bits 23:16 */
      uint16 EH:8;               /* 0x17C bits 31:24 */
      uint16 VHV_EStep:16;       /* 0x180 bits 15:0 */
      uint16 VHV_EStart:16;      /* 0x180 bits 31:16 */
      uint16 MAX_PP:16;          /* 0x184 bits 15:0 */
      uint16 OtpReserved1:16;    /* 0x184 bits 31:16 */
      uint16 PROG_PW:16;         /* 0x188 bits 15:0 */
      uint16 MAX_EP:16;          /* 0x188 bits 31:16 */
      uint32 ERA_PW;             /* 0x18C bits 31:0 */
      uint16 VHV_E;              /* 0x190 bits 15:0 */
      uint16 VHV_P;              /* 0x190 bits 31:16 */
      uint16 VINH:8;             /* 0x194 bits 7:0 */
      uint16 VCG:8;              /* 0x194 bits 15:8 */
      uint16 VHV_PV;             /* 0x194 bits 31:16 */
      uint16 OtpReserved2:8;     /* 0x198 bits 7:0 */
      uint16 VRead:8;            /* 0x198 bits 15:8 */
      uint16 VWL_P:8;            /* 0x198 bits 23:16 */
      uint16 VSL_P:8;            /* 0x198 bits 31:24 */
      uint32 ApiChecksum;        /* 0x19C bits 15:0 */
      uint32 OtpReserved3;    /* 0x1A0 bits 31:0 */
      uint32 OtpReserved4;    /* 0x1A4 bits 31:0 */
      uint32 OtpReserved5;    /* 0x1A8 bits 31:0 */
      uint32 OtpReserved6;    /* 0x1AC bits 31:0 */
   } OTP_VALUE;
   volatile uint32 au32OtpWord[0x18];
}Fapi_TiOtpBytesType;

/* Union for uint32/uint32* /volatile uint32* to fix MISRAC error */
typedef union
{
   uint32   u32Value; /* uint32 Address */
   uint32 * u32poValue; /* Pointer address */
   volatile uint32 * volu32poValue; /* Pointer address */
   struct
   {
       //Structure to hold 2 uint16 *
      uint16* u16poValue1;
      uint16* u16poValue2;
   }pou16ValueStruct;
   struct
   {
       //Structure to split uint32 into 2 uint16
      uint16 u16Value1;
      uint16 u16Value2;
   }u16Struct;
   struct
   {
       //Structure to split uint32 into 8 bit pieces
      uint16 u8Value1:8;
      uint16 u8Value2:8;
      uint16 u8Value3:8;
      uint16 u8Value4:8;
   }u8Struct;
   struct
   {
       //Structure to save 9LSB bits
      uint16 u9Value:9;
      uint16 u7Value:7;
   }u9Struct;
} u32PtrValue;

/* Union for commandType to fix MISRAC error */
typedef union
{
    Fapi_FlashStateCommandsType commandType;
    struct
    {
        uint16 ucommandType:    6;          /* command type stored as uint16*/
        uint16 reserved:        10;         /* reserved */
    }commandTypeSt;
}UnicommandType;

/* Union for uint16/uint16* /volatile uint16* to fix MISRAC error */
typedef union
{
   uint16   u16Address; /* uint16 Address */
   uint16 * u16poAddress; /* Pointer address */
   volatile uint16 * volu16poAddress; /* volatile Pointer address */
} u16PtrAddr;

/*!
    \brief
*/
typedef enum
{
   Alpha_Internal,          /* For internal TI use only.  Not intended to be used by customers */
   Alpha,                   /* Early Engineering release.  May not be functionally complete */
   Beta_Internal,           /* For internal TI use only.  Not intended to be used by customers */
   Beta,                    /* Functionally complete, to be used for testing and validation */
   Production               /* Fully validated, functionally complete, ready for production use */
}  ATTRIBUTE_PACKED Fapi_ApiProductionStatusType;

typedef struct
{
   uint8  u8ApiMajorVersion;
   uint8  u8ApiMinorVersion;
   uint8  u8ApiRevision;
   Fapi_ApiProductionStatusType oApiProductionStatus;
   uint32 u32ApiBuildNumber;
   uint8  u8ApiTechnologyType;
   uint8  u8ApiTechnologyRevision;
   uint8  u8ApiEndianness;
   uint32 u32ApiCompilerVersion;
} Fapi_LibraryInfoType;


#endif /*F021_TYPES_H_*/
