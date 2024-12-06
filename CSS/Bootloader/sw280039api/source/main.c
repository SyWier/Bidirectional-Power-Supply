/**
 * main.c
 */

//
// Included Files
//
#include "f28x_Project.h"
#include "f28003x_globalprototypes.h"
#include "driverlib.h"
#include "device.h"
#include "MUCI_bsp.h"
#include "F021_F28003x_C28x.h"


uint32_t Counter;

void Example_Error(Fapi_StatusType status);

void main(void)
{

    Fapi_StatusType oReturnCheck;

    //
    // Initialize device clock and peripherals
    //
    InitSysCtrl();
    InitFlash();
    //
    // Initialize GPIO
    //
    InitGpio();

    //
    //  Unlock CSM
    //
    //  If the API functions are going to run in unsecured RAM
    //  then the CSM must be unlocked in order for the flash
    //  API functions to access the flash.
    //  If the flash API functions are executed from secure memory
    //  then this step is not required.
    //
    //CsmUnlock();

    //
    // For this example, only init the pins for the SCI-A port.
    // GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
    // GPIO_SetupPinOptions() - Sets the direction and configuration of GPIOs
    // These functions are found in the F28X7x_Gpio.c file.
    //
    GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);

    //
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR)
    //

    InitPieVectTable();

    EnableInterrupts();

    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    //
    // Initialize SCIA and its FIFO.
    //
    SCI_performSoftwareReset(SCIA_BASE);

    //
    // Configure SCIA for echoback.
    //
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, 57600, (SCI_CONFIG_WLEN_8 |
                                                        SCI_CONFIG_STOP_ONE |
                                                        SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE);
    SCI_resetTxFIFO(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

	MUCI_bsp_Init();

    //
    // Note that wait-states are already configured in the Device_init().
    // However, if INTOSC is used as the clock source and
    // if the CPUCLK falls in the range (97,100] (check other ranges given in DS),
    // then an extra wait state is needed for FSM operations (erase/program).
    // Hence, below function call should be uncommented in case INTOSC is used.
    // At 100MHz, execution wait-states for external oscillator is 4 and hence
    // in this example, a wait-state of 5 is used below.
    // This example is using external oscillator as the clock source and hence
    // below is commented.
    //
    // This wait-state setting impacts both Flash banks. Applications which
    // perform simultaneous READ/FETCH of one bank and PROGRAM or ERASE of the other
    // bank must use the higher RWAIT setting during the PROGRAM or ERASE operation. OR
    // use a clock source or frequency with a common wait state setting
    // Example: Use 97MHz instead of 100MHz if it is acceptable for the application.
    //
    // In case, if user application increments wait-state before using API,
    // then remember to revert back to the original wait-state after the API usage
    // to avoid extra wait-state during application execution from Flash.
    //
    //
    // Flash_setWaitstates(FLASH0CTRL_BASE, 5);

    // Initialize the Flash API by providing the Flash register base address
    // and operating frequency.
    // This function is required to initialize the Flash API based on System frequency
    // before any other Flash API operation can be performed.
    // This function must also be called whenever System frequency or RWAIT is changed.
    EALLOW;
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 120);

    if(oReturnCheck != Fapi_Status_Success)
    {
        // Check Flash API documentation for possible errors
        Example_Error(oReturnCheck);
    }

    // Initialize the Flash banks and FMC for erase and program operations.
    // Fapi_setActiveFlashBank() function sets the Flash banks and FMC for further
    // Flash operations to be performed on the banks.
    // Note: It does not matter which bank is passed as the parameter to initialize.
    //       Both Banks and FMC get initialized with one function call unlike F2837xS.
    //       Hence there is no need to execute Fapi_setActiveFlashBank() for each bank.
    //       Executing for one bank is enough.
    EALLOW;
    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);

    if(oReturnCheck != Fapi_Status_Success)
    {
        // Check Flash API documentation for possible errors
        Example_Error(oReturnCheck);
    }


    for(;;)
    {

        MUCI_bsp_Background();
        Counter++;
    }

}

//
// Example_Error - For this example, if an error is found just stop here
//

void Example_Error(Fapi_StatusType status)
{
    //
    //  Error code will be in the status parameter
    //
    __asm("    ESTOP0");
}

