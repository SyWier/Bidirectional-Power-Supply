//###########################################################################
//
// FILE:    f28003x_analogsubsys.h
//
// TITLE:   Definitions for the ANALOGSUBSYS registers.
//
//###########################################################################
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
//###########################################################################

#ifndef F28003X_ANALOGSUBSYS_H
#define F28003X_ANALOGSUBSYS_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// ANALOGSUBSYS Individual Register Bit Definitions:

struct CONFIGLOCK_BITS {                // bits description
    Uint16 rsvd1:1;                     // 0 Reserved
    Uint16 rsvd2:1;                     // 1 Reserved
    Uint16 rsvd3:1;                     // 2 Reserved
    Uint16 AGPIOCTRL:1;                 // 3 Locks all AGPIOCTRL Register
    Uint16 rsvd4:1;                     // 4 Reserved
    Uint16 rsvd5:11;                    // 15:5 Reserved
    Uint16 rsvd6:16;                    // 31:16 Reserved
};

union CONFIGLOCK_REG {
    Uint32  all;
    struct  CONFIGLOCK_BITS  bit;
};

struct TSNSCTL_BITS {                   // bits description
    Uint16 ENABLE:1;                    // 0 Temperature Sensor Enable
    Uint16 rsvd1:15;                    // 15:1 Reserved
};

union TSNSCTL_REG {
    Uint16  all;
    struct  TSNSCTL_BITS  bit;
};

struct ANAREFCTL_BITS {                 // bits description
    Uint16 ANAREFSEL:1;                 // 0 Analog Reference Select
    Uint16 rsvd1:1;                     // 1 Reserved
    Uint16 rsvd2:1;                     // 2 Reserved
    Uint16 rsvd3:5;                     // 7:3 Reserved
    Uint16 ANAREF2P5SEL:1;              // 8 Analog Reference Select
    Uint16 rsvd4:1;                     // 9 Reserved
    Uint16 rsvd5:1;                     // 10 Reserved
    Uint16 rsvd6:5;                     // 15:11 Reserved
};

union ANAREFCTL_REG {
    Uint16  all;
    struct  ANAREFCTL_BITS  bit;
};

struct VMONCTL_BITS {                   // bits description
    Uint16 rsvd1:1;                     // 0 Reserved
    Uint16 rsvd2:1;                     // 1 Reserved
    Uint16 rsvd3:6;                     // 7:2 Reserved
    Uint16 BORLVMONDIS:1;               // 8 Disable BORL(ow) feature on VDDIO
    Uint16 rsvd4:7;                     // 15:9 Reserved
};

union VMONCTL_REG {
    Uint16  all;
    struct  VMONCTL_BITS  bit;
};

struct CMPHPMXSEL_BITS {                // bits description
    Uint16 CMP1HPMXSEL:3;               // 2:0 CMP1HPMXSEL bits
    Uint16 CMP2HPMXSEL:3;               // 5:3 CMP2HPMXSEL bits
    Uint16 CMP3HPMXSEL:3;               // 8:6 CMP3HPMXSEL bits
    Uint16 CMP4HPMXSEL:3;               // 11:9 CMP4HPMXSEL bits
    Uint16 rsvd1:3;                     // 14:12 Reserved
    Uint16 rsvd2:1;                     // 15 Reserved
    Uint16 rsvd3:3;                     // 18:16 Reserved
    Uint16 rsvd4:3;                     // 21:19 Reserved
    Uint16 rsvd5:10;                    // 31:22 Reserved
};

union CMPHPMXSEL_REG {
    Uint32  all;
    struct  CMPHPMXSEL_BITS  bit;
};

struct CMPLPMXSEL_BITS {                // bits description
    Uint16 CMP1LPMXSEL:3;               // 2:0 CMP1LPMXSEL bits
    Uint16 CMP2LPMXSEL:3;               // 5:3 CMP2LPMXSEL bits
    Uint16 CMP3LPMXSEL:3;               // 8:6 CMP3LPMXSEL bits
    Uint16 CMP4LPMXSEL:3;               // 11:9 CMP4LPMXSEL bits
    Uint16 rsvd1:3;                     // 14:12 Reserved
    Uint16 rsvd2:1;                     // 15 Reserved
    Uint16 rsvd3:3;                     // 18:16 Reserved
    Uint16 rsvd4:3;                     // 21:19 Reserved
    Uint16 rsvd5:10;                    // 31:22 Reserved
};

union CMPLPMXSEL_REG {
    Uint32  all;
    struct  CMPLPMXSEL_BITS  bit;
};

struct CMPHNMXSEL_BITS {                // bits description
    Uint16 CMP1HNMXSEL:1;               // 0 CMP1HNMXSEL bits
    Uint16 CMP2HNMXSEL:1;               // 1 CMP2HNMXSEL bits
    Uint16 CMP3HNMXSEL:1;               // 2 CMP3HNMXSEL bits
    Uint16 CMP4HNMXSEL:1;               // 3 CMP4HNMXSEL bits
    Uint16 rsvd1:1;                     // 4 Reserved
    Uint16 rsvd2:1;                     // 5 Reserved
    Uint16 rsvd3:1;                     // 6 Reserved
    Uint16 rsvd4:9;                     // 15:7 Reserved
};

union CMPHNMXSEL_REG {
    Uint16  all;
    struct  CMPHNMXSEL_BITS  bit;
};

struct CMPLNMXSEL_BITS {                // bits description
    Uint16 CMP1LNMXSEL:1;               // 0 CMP1LNMXSEL bits
    Uint16 CMP2LNMXSEL:1;               // 1 CMP2LNMXSEL bits
    Uint16 CMP3LNMXSEL:1;               // 2 CMP3LNMXSEL bits
    Uint16 CMP4LNMXSEL:1;               // 3 CMP4LNMXSEL bits
    Uint16 rsvd1:1;                     // 4 Reserved
    Uint16 rsvd2:1;                     // 5 Reserved
    Uint16 rsvd3:1;                     // 6 Reserved
    Uint16 rsvd4:9;                     // 15:7 Reserved
};

union CMPLNMXSEL_REG {
    Uint16  all;
    struct  CMPLNMXSEL_BITS  bit;
};

struct ADCDACLOOPBACK_BITS {            // bits description
    Uint16 ENLB2ADCA:1;                 // 0 Enable DACA loopback to ADCA
    Uint16 ENLB2ADCB:1;                 // 1 Enable DACA loopback to ADCB
    Uint16 ENLB2ADCC:1;                 // 2 Enable DACA loopback to ADCC
    Uint16 rsvd1:13;                    // 15:3 Reserved
    Uint16 KEY:16;                      // 31:16 Key to enable writes
};

union ADCDACLOOPBACK_REG {
    Uint32  all;
    struct  ADCDACLOOPBACK_BITS  bit;
};

struct LOCK_BITS {                      // bits description
    Uint16 TSNSCTL:1;                   // 0 TSNSCTL Register lock bit
    Uint16 ANAREFCTL:1;                 // 1 ANAREFCTL Register lock bit
    Uint16 VMONCTL:1;                   // 2 VMONCTL Register lock bit
    Uint16 rsvd1:1;                     // 3 Reserved
    Uint16 rsvd2:1;                     // 4 Reserved
    Uint16 CMPHPMXSEL:1;                // 5 CMPHPMXSEL Register lock bit
    Uint16 CMPLPMXSEL:1;                // 6 CMPLPMXSEL Register lock bit
    Uint16 CMPHNMXSEL:1;                // 7 CMPHNMXSEL Register lock bit
    Uint16 CMPLNMXSEL:1;                // 8 CMPLNMXSEL Register lock bit
    Uint16 VREGCTL:1;                   // 9 VREGCTL Register lock bit
    Uint16 rsvd3:6;                     // 15:10 Reserved
    Uint16 rsvd4:16;                    // 31:16 Reserved
};

union LOCK_REG {
    Uint32  all;
    struct  LOCK_BITS  bit;
};

struct AGPIOCTRLA_BITS {                // bits description
    Uint16 rsvd1:1;                     // 0 Reserved
    Uint16 rsvd2:1;                     // 1 Reserved
    Uint16 rsvd3:1;                     // 2 Reserved
    Uint16 rsvd4:1;                     // 3 Reserved
    Uint16 rsvd5:1;                     // 4 Reserved
    Uint16 rsvd6:1;                     // 5 Reserved
    Uint16 rsvd7:1;                     // 6 Reserved
    Uint16 rsvd8:1;                     // 7 Reserved
    Uint16 rsvd9:1;                     // 8 Reserved
    Uint16 rsvd10:1;                    // 9 Reserved
    Uint16 rsvd11:1;                    // 10 Reserved
    Uint16 rsvd12:1;                    // 11 Reserved
    Uint16 rsvd13:1;                    // 12 Reserved
    Uint16 rsvd14:1;                    // 13 Reserved
    Uint16 rsvd15:1;                    // 14 Reserved
    Uint16 rsvd16:1;                    // 15 Reserved
    Uint16 rsvd17:1;                    // 16 Reserved
    Uint16 rsvd18:1;                    // 17 Reserved
    Uint16 rsvd19:1;                    // 18 Reserved
    Uint16 rsvd20:1;                    // 19 Reserved
    Uint16 GPIO20:1;                    // 20 AGPIOCTRL for GPIO20
    Uint16 GPIO21:1;                    // 21 AGPIOCTRL for GPIO21
    Uint16 rsvd21:1;                    // 22 Reserved
    Uint16 rsvd22:1;                    // 23 Reserved
    Uint16 rsvd23:1;                    // 24 Reserved
    Uint16 rsvd24:1;                    // 25 Reserved
    Uint16 rsvd25:1;                    // 26 Reserved
    Uint16 rsvd26:1;                    // 27 Reserved
    Uint16 rsvd27:1;                    // 28 Reserved
    Uint16 rsvd28:1;                    // 29 Reserved
    Uint16 rsvd29:1;                    // 30 Reserved
    Uint16 rsvd30:1;                    // 31 Reserved
};

union AGPIOCTRLA_REG {
    Uint32  all;
    struct  AGPIOCTRLA_BITS  bit;
};

struct ANALOG_SUBSYS_REGS {
    Uint16                                   rsvd1[94];                    // Reserved
    union   CONFIGLOCK_REG                   CONFIGLOCK;                   // Lock Register for all the config registers.
    union   TSNSCTL_REG                      TSNSCTL;                      // Temperature Sensor Control Register
    Uint16                                   rsvd2[7];                     // Reserved
    union   ANAREFCTL_REG                    ANAREFCTL;                    // Analog Reference Control Register
    Uint16                                   rsvd3[7];                     // Reserved
    union   VMONCTL_REG                      VMONCTL;                      // Voltage Monitor Control Register
    Uint16                                   rsvd4[17];                    // Reserved
    union   CMPHPMXSEL_REG                   CMPHPMXSEL;                   // Bits to select one of the many sources on CompHP inputs. Refer to Pimux diagram for details.
    union   CMPLPMXSEL_REG                   CMPLPMXSEL;                   // Bits to select one of the many sources on CompLP inputs. Refer to Pimux diagram for details.
    union   CMPHNMXSEL_REG                   CMPHNMXSEL;                   // Bits to select one of the many sources on CompHN inputs. Refer to Pimux diagram for details.
    union   CMPLNMXSEL_REG                   CMPLNMXSEL;                   // Bits to select one of the many sources on CompLN inputs. Refer to Pimux diagram for details.
    union   ADCDACLOOPBACK_REG               ADCDACLOOPBACK;               // Enabble loopback from DAC to ADCs
    Uint16                                   rsvd5[4];                     // Reserved
    union   LOCK_REG                         LOCK;                         // Lock Register
    Uint16                                   rsvd6[114];                   // Reserved
    union   AGPIOCTRLA_REG                   AGPIOCTRLA;                   // AGPIO Control Register
};

//---------------------------------------------------------------------------
// ANALOGSUBSYS External References & Function Declarations:
//
extern volatile struct ANALOG_SUBSYS_REGS AnalogSubsysRegs;
#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif

//===========================================================================
// End of file.
//===========================================================================
