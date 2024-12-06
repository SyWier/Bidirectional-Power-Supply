#ifndef __MAIN_H_
	#define __MAIN_H_

	#include "F28x_Project.h"
	#include "inic.h"

	#define BUFF_SIZE 1000

	#define RxRDY ( SciaRegs.SCIRXST.bit.RXRDY )
	#define RxDAT ( SciaRegs.SCIRXBUF.bit.SAR & 0xFF )
	#define TxRDY ( SciaRegs.SCICTL2.bit.TXRDY )
	#define TxDAT ( SciaRegs.SCITXBUF.bit.TXDT )
	#define TxEMPTY ( SciaRegs.SCICTL2.bit.TXEMPTY )

	void MemCpy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);
#endif
