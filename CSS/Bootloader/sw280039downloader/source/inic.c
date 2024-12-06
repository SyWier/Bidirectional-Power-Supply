#include "inic.h"

//--------------------------------------------------------------------
//  Configure Device for target Application Here
//--------------------------------------------------------------------

void DeviceInit(void)
{
    DisableDog();   // Disable the watchdog initially

    //
    // Initialize the SYSPLL control  to generate a 200Mhz clock
    //
    // Defined options to be passed as arguments to this function are defined
    // in f28003x_examples.h.
    //
    // Note: The internal oscillator CANNOT be used as the PLL source if the
    // PLLSYSCLK is configured to frequencies above 194 MHz.
    //
    //  PLLSYSCLK = (XTAL_OSC) * (IMULT) /(REFDIV) * (ODIV) * (PLLSYSCLKDIV)
    //
    InitSysPll(XTAL_OSC, IMULT_48, REFDIV_2, ODIV_4, PLLCLK_BY_1, SYSCTL_DCC_BASE0);

    EALLOW; // below registers are "protected", allow access.

    // PERIPHERAL CLOCK ENABLES
    //---------------------------------------------------
    // If you are not using a peripheral you may want to switch
    // the clock off to save power, i.e. set to =0
    //
    // Note: not all peripherals are available on all 280x derivates.
    // Refer to the datasheet for your particular device.

    CpuSysRegs.PCLKCR7.bit.SCI_A = 1;    // SCI-A

    //--------------------------------------------------------------------------------------
    // GPIO (GENERAL PURPOSE I/O) CONFIG
    //--------------------------------------------------------------------------------------
    //-----------------------
    // QUICK NOTES on USAGE:
    //-----------------------
    // If GpioCtrlRegs.GP?MUX?bit.GPIO?= 1, 2 or 3 (i.e. Non GPIO func), then leave
    //  rest of lines commented
    // If GpioCtrlRegs.GP?MUX?bit.GPIO?= 0 (i.e. GPIO func), then:
    //  1) uncomment GpioCtrlRegs.GP?DIR.bit.GPIO? = ? and choose pin to be IN or OUT
    //  2) If IN, can leave next to lines commented
    //  3) If OUT, uncomment line with ..GPACLEAR.. to force pin LOW or
    //             uncomment line with ..GPASET.. to force pin HIGH or
    //--------------------------------------------------------------------------------------
    //  GPIO-28 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;    // 0=GPIO, 1=SCIRXA, 2=SDAA, 3=TZ2
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;     // 1=OUTput,  0=INput
    //  GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;   // uncomment if --> Set Low initially
    //  GpioDataRegs.GPASET.bit.GPIO28 = 1;     // uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-29 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;    // 0=GPIO, 1=SCITXDA, 2=SCLA, 3=TZ3
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;     // 1=OUTput,  0=INput
    //  GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;   // uncomment if --> Set Low initially
    //  GpioDataRegs.GPASET.bit.GPIO29 = 1;     // uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------

    SetSCITMSmon(&SciaRegs);    //Setup HiTerm communications port

    EDIS;   // Disable register access
}

/******************************************************************************/
void SetSCITMSmon(volatile struct SCI_REGS *ptr)
{
  int i;

  ptr->SCICCR.all = 7;  //1 stop bit,no parity, no test, idle–line protocol, 8bits
  ptr->SCICTL1.bit.SWRESET = 0;

  for (i=0;i<5000;i++) {} ;

  ptr->SCICTL1.all = 0x23;

  ptr->SCILBAUD.bit.BAUD = (((30*1000000L)/38400-8)/8) & 0xff;
  ptr->SCIHBAUD.bit.BAUD = (((30*1000000L)/38400-8)/8) >> 8;
}

