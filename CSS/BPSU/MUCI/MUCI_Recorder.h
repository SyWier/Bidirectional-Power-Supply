/*
 * MUCI_Recorder.h
 *
 *  Created on: 2018. okt. 10.
 *      Author: z003zn6d
 */

#ifndef MUCI_INCLUDE_MUCI_RECORDER_H_
    #define MUCI_INCLUDE_MUCI_RECORDER_H_
    #include "MUCI.h"
    #include "MUCI_bsp.h"

    #ifndef MUCI_RECORDER_DATA_LENGTH
        #define MUCI_RECORDER_DATA_LENGTH       512u    /*!< length of the black box in x (time) direction) */
        #warning "MUCI recorder data length not defined default value used!"
    #endif

    #ifndef  MUCI_RECORDER_CHANNEL_NUMBER
        #define MUCI_RECORDER_CHANNEL_NUMBER    4u      /*!< number of channels used for data collecting (blackbox function) only implemented for 4*/
        #warning "MUCI recorder channel number not defined default value used!"
    #endif

    #define BBX_BASE         0   /*!< blackbox do nothing */
    #define BBX_WAIT4TRIGGER 2   /*!< blackbox is waiting for trigger */
    #define BBX_TRIGGERED    4   /*!< blackbox triggered */
    #define BBX_FINISHED     5   /*!< blackbox finished data collection */
    #define BBX_FORCE_START  1   /*!< blackbox starting - written to this state by HiTerm */
    #define BBX_WT_WREN      6   /*!< blackbox wait write enable spi IC */
    #define BBX_WT_ADDR      7   /*!< blackbox wait address set up */
    #define BBX_ALL_FINISHED 3   /*!< blackbox finished data collection, spi closed */

    #define BBX_TV_SIGNED    0x01   /*!< blackbox triggered value signed */
    #define BBX_TV_UNSIGNED  0x00   /*!< blackbox triggered value unsigned */
    #define BBX_TV_16BIT     0x02   /*!< blackbox triggered value 16bit */
    #define BBX_TV_32BIT     0x00   /*!< blackbox triggered value 32bit */
    #define BBX_TV_16B_UPPER 0x04   /*!< blackbox triggered value at upper16 bit (only valid for 16 bit variable) */
    #define BBX_TV_FLOAT     0x08   /*!< blackbox triggered value float */

    typedef struct {
        uint32_t  val_addr[MUCI_RECORDER_CHANNEL_NUMBER]; /*!< address of the variables to be sampled */
        uint32_t  sampled_date;    /*!< date when the data was sampled */
        uint32_t  sampled_time;    /*!< time when the data was sampled - time of the trigger point (as much as possible)*/
        uint32_t  sample_time;     /*!< undersampling of the variables (not sampling in every call of \a bbx_trigger */
        int32_t   trig_level;      /*!< trigger level in raw (non-iq non floating point format */

        uint16_t  state;           /*!< state of the FSM of the bbx*/
        uint16_t  start_idx;       /*!< first sample of the bbx*/
        uint16_t  trigger_ch;      /*!< trigger channel 0-NO_CH-1: sampled channel; NO_CH: trigger for Errorcode */
        uint16_t  pretrigger;      /*!< number of samples collected before the trigger occurs*/
        uint16_t  trig_slope;      /*!< rising (0); falling (1) edge */
        uint16_t  tnow;            /*!< force triggering immediatelly */
        uint16_t  tlevel_type;     /*!< trigger level type, for definition see TYP_FLOAT, TYP_UNSIGNED, TYP_32BIT etc */

        uint16_t  idx;             /*!< current id of data collecting */
        uint32_t  div;             /*!< variable used for undersampling */
        int32_t  trig_ch_val_old;  /*!< used to detect rising edge/falling edge */
        int32_t  trig_ch_val;      /*!< value of the triggered channel */
        uint16_t  trig_delay;      /*!< to ensure that enough data are already in the pretrigger part of the buffer */
        uint16_t  end_idx;         /*!< last data id */
    } MUCI_RecorderParameter_t;

    extern MUCI_RecorderParameter_t MUCI_RecorderParameter;
    extern uint32_t MUCI_RecorderData[MUCI_RECORDER_DATA_LENGTH][MUCI_RECORDER_CHANNEL_NUMBER]; /*!< 32 bit buffer for storing the data */
    extern uint32_t MUCI_RecorderLength;

    /* BBX functions */
    void MUCI_Recorder_Init(void);
    int16_t CalcTrigValueGreater(int32_t tval);
    void MUCI_Recorder_Trigger(void);
    inline unsigned int sat_arr_idx(unsigned int ii);
    inline void push_bbx();
    inline void push_single_bbx_32(uint16_t i);
    void MUCI_Recorder_Background(void);

#endif /* MUCI_INCLUDE_MUCI_RECORDER_H_ */
