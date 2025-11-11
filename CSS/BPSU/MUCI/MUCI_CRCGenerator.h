/*
 * MUCI_CRCGenerator.h
 *
 *  Created on: 2019. febr. 20.
 *      Author: z003zn6d
 */

#ifndef MUCI_INCLUDE_MUCI_CRCGENERATOR_H_
    #define MUCI_INCLUDE_MUCI_CRCGENERATOR_H_

    #include "stdint.h"
    #include "stdbool.h"
    #include "MUCI_bsp.h"

    void MUCI_CalcCheckSum(uint16_t *chksum, uint8_t data);
#endif /* MUCI_INCLUDE_MUCI_CRCGENERATOR_H_ */
