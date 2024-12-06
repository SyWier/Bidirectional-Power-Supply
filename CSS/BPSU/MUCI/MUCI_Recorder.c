/*
 * MUCI_Recorder.c
 *
 *  Created on: 2018. okt. 10.
 *      Author: z003zn6d
 */


/*! \file BBX.c
 *  \brief ("BlackBoX.c")Data collecting for the recorder function.
 *  \author Originally Gal Istvan; further development: Bilau Zoltan Tamas (IQ, float, shifting, time, now, SPI)
 *  \version 2.0
 *  \date    2010-06-25
 */

#include "MUCI_Recorder.h"

MUCI_RecorderParameter_t MUCI_RecorderParameter;
uint32_t MUCI_RecorderData[MUCI_RECORDER_DATA_LENGTH][MUCI_RECORDER_CHANNEL_NUMBER]; /*!< 32 bit buffer for storing the data */


uint32_t FieldWatchError;

#define VAR5TH (FieldWatchError) /*!< 5th variable just for triggering */
extern uint16_t PVC_Error_Code;
extern uint32_t c_date ;
extern uint32_t c_time ;

uint32_t MUCI_RecorderLength = MUCI_RECORDER_DATA_LENGTH;

/*!
 *  \brief Sets the id into a given value
 *  Handles overflow of idx. It handles it differently if it is a power of two (ANDing is possible), and if it is not.
 *  \param[in] ii to be saturated
 *  \return saturated value
 */
inline unsigned int sat_arr_idx(unsigned int ii)
{
  unsigned int jj = ii;
  #if ((MUCI_RecorderLength & (MUCI_RecorderLength - 1)) != 0)
  while (jj >= MUCI_RecorderLength)
    jj -= MUCI_RecorderLength;
  #else
  jj &= (MUCI_RecorderLength - 1);
  #endif
  return jj;
}


void MUCI_Recorder_Init(void)
{
  uint16_t k;
  for (k = 0; k < MUCI_RECORDER_CHANNEL_NUMBER; k++)
  {
      MUCI_RecorderParameter.val_addr[k] = 0;
  }
  MUCI_RecorderParameter.tnow = 0;
  MUCI_RecorderParameter.state = BBX_BASE;
}

#pragma CODE_SECTION(CalcTrigValueGreater, ".TI.ramfunc")
int16_t CalcTrigValueGreater(int32_t tval)
{
  int16_t i16,i16_tlevel;
  int16_t ret_par;
  switch (MUCI_RecorderParameter.tlevel_type)
  {
  case (BBX_TV_SIGNED | BBX_TV_32BIT):
    ret_par = tval > MUCI_RecorderParameter.trig_level ? 1 : 0;
    break;
  case (BBX_TV_UNSIGNED | BBX_TV_32BIT):
    ret_par = *(uint32_t*)&tval > *(uint32_t*)&MUCI_RecorderParameter.trig_level ? 1 : 0;
    break;
  case (BBX_TV_SIGNED | BBX_TV_16BIT):
    i16 = tval & 0xFFFF;
    i16_tlevel = MUCI_RecorderParameter.trig_level & 0xFFFF;
    ret_par = i16 > i16_tlevel ? 1:0;
    break;
  case (BBX_TV_SIGNED | BBX_TV_16BIT | BBX_TV_16B_UPPER):
    i16 = tval >> 16;
    i16_tlevel = MUCI_RecorderParameter.trig_level;
    ret_par = i16 > i16_tlevel ? 1:0;
    break;
  case (BBX_TV_UNSIGNED | BBX_TV_16BIT | BBX_TV_16B_UPPER):
    i16 = tval >> 16;
    i16_tlevel = MUCI_RecorderParameter.trig_level;
    ret_par = *(uint16_t*)&i16 > *(uint16_t*)&i16_tlevel ? 1:0;
    break;
  case (BBX_TV_UNSIGNED | BBX_TV_16BIT):
    i16 = tval & 0xFFFF;
    i16_tlevel = MUCI_RecorderParameter.trig_level & 0xFFFF;
    ret_par = *(uint16_t*)&i16 > *(uint16_t*)&i16_tlevel ? 1:0;
    break;
  case (BBX_TV_FLOAT):
    ret_par = *(float*)&tval > *(float*)&MUCI_RecorderParameter.trig_level ? 1 : 0;
    break;
  default:
    ret_par = 1;
    break;
  }
  return ret_par;
}


/*!
 *  \brief Data Collection
 *  It collects the data in \a bbox[][] or in the SPI RAM.
 *  \author Weitzl Zoltan
 *  \version 1.0
 *  \date 2010. 07. 01.
 */
#pragma CODE_SECTION(MUCI_Recorder_Trigger, ".TI.ramfunc")
void MUCI_Recorder_Trigger(void)
{
  /* division of the sampling frequency */
  if (++MUCI_RecorderParameter.div >= MUCI_RecorderParameter.sample_time)
      MUCI_RecorderParameter.div = 0;
  else
    return ;

  if (MUCI_RecorderParameter.state == BBX_WAIT4TRIGGER)
  // wait for the trigger
  {
    push_bbx();
    if (MUCI_RecorderParameter.trig_delay)
        MUCI_RecorderParameter.trig_delay--;
    else
    {
      if (!MUCI_RecorderParameter.trig_slope)
      // rising edge
      {
        if ((!CalcTrigValueGreater(MUCI_RecorderParameter.trig_ch_val_old) && CalcTrigValueGreater(MUCI_RecorderParameter.trig_ch_val)) || (MUCI_RecorderParameter.tnow) || (MUCI_RecorderParameter.trigger_ch == MUCI_RECORDER_CHANNEL_NUMBER && MUCI_RecorderParameter.trig_ch_val))
        {
            MUCI_RecorderParameter.start_idx = MUCI_RecorderParameter.idx;
            MUCI_RecorderParameter.end_idx = sat_arr_idx(MUCI_RecorderParameter.start_idx + (MUCI_RecorderLength - 1 - MUCI_RecorderParameter.pretrigger));
            MUCI_RecorderParameter.state = BBX_TRIGGERED;
            MUCI_RecorderParameter.sampled_date = c_date;
            MUCI_RecorderParameter.sampled_time = c_time;
            MUCI_RecorderParameter.tnow = 0;
        }
      }
      else
      // falling edge
      {
        if ((CalcTrigValueGreater(MUCI_RecorderParameter.trig_ch_val_old) && !CalcTrigValueGreater(MUCI_RecorderParameter.trig_ch_val)) || (MUCI_RecorderParameter.tnow) || (MUCI_RecorderParameter.trigger_ch == MUCI_RECORDER_CHANNEL_NUMBER && MUCI_RecorderParameter.trig_ch_val))
        {
            MUCI_RecorderParameter.start_idx = MUCI_RecorderParameter.idx;
            MUCI_RecorderParameter.end_idx = sat_arr_idx(MUCI_RecorderParameter.start_idx + (MUCI_RecorderLength - 1 - MUCI_RecorderParameter.pretrigger));
            MUCI_RecorderParameter.state = BBX_TRIGGERED;
            MUCI_RecorderParameter.sampled_date = c_date;
            MUCI_RecorderParameter.sampled_time = c_time;
            MUCI_RecorderParameter.tnow = 0;
        }
      }
    }
  }
  else if (MUCI_RecorderParameter.state == BBX_TRIGGERED)
  {
    if (MUCI_RecorderParameter.idx == MUCI_RecorderParameter.end_idx) // end of bbx
        MUCI_RecorderParameter.state = BBX_FINISHED;
    push_bbx();
  }
  else// wait for starting
    MUCI_RecorderParameter.trig_delay = sat_arr_idx(MUCI_RecorderParameter.pretrigger + 1);
}

/*!
 *  \brief Pushes the NO_CH data into the buffer \a bbox / \a SPI RAM.
 *  In case of SPI RAM is used (USE_SPI_RAM), bbox data is pushed into it.
 *  \author Weitzl Zoltan
 *  \version 1.0
 *  \date 2010. 07. 01.
 */

inline void push_bbx()
{
    MUCI_RecorderParameter.trig_ch_val_old = MUCI_RecorderParameter.trig_ch_val;
    #if (MUCI_RECORDER_CHANNEL_NUMBER == 4)
        push_single_bbx_32(0);
        push_single_bbx_32(1);
        push_single_bbx_32(2);
        push_single_bbx_32(3);
    #elif (MUCI_RECORDER_CHANNEL_NUMBER == 8)
        push_single_bbx_32(0);
        push_single_bbx_32(1);
        push_single_bbx_32(2);
        push_single_bbx_32(3);
        push_single_bbx_32(4);
        push_single_bbx_32(5);
        push_single_bbx_32(6);
        push_single_bbx_32(7);
    #else
        #error "MUCI recorder channel number invalid!"
    #endif

    MUCI_RecorderParameter.trig_ch_val = (MUCI_RecorderParameter.trigger_ch == MUCI_RECORDER_CHANNEL_NUMBER) ? VAR5TH : *(int32_t*)(MUCI_RecorderParameter.val_addr[MUCI_RecorderParameter.trigger_ch]);
    MUCI_RecorderParameter.idx = sat_arr_idx(MUCI_RecorderParameter.idx + 1);
}

inline void push_single_bbx_32(uint16_t i)
{
    MUCI_RecorderData[MUCI_RecorderParameter.idx][i] = *(uint32_t*)(MUCI_RecorderParameter.val_addr[i]);
}

 /*!
  *  \brief Blackbox management in the background
  *  \author Weitzl Zoltan
  *  \version 1.0
  * \date 2010. 07. 01.
 */
void MUCI_Recorder_Background(void)
{
  switch (MUCI_RecorderParameter.state)
  {
    case BBX_FORCE_START:
        // init memory
        MUCI_RecorderParameter.state = BBX_WT_WREN;
        break;
    case BBX_WT_WREN:
        // set up address
        MUCI_RecorderParameter.state = BBX_WT_ADDR;
        break;
    case BBX_WT_ADDR:
        MUCI_RecorderParameter.idx = 0;
        MUCI_RecorderParameter.state = BBX_WAIT4TRIGGER;
        break;
    case BBX_FINISHED:
        MUCI_RecorderParameter.state = BBX_ALL_FINISHED;
        break;
  }
}
