#include "stdint.h"
#include "MUCI_Recorder.h"
#include "Term_defs.h"

uint32_t f_samp = 10000;    /*!< sampling frequency - should be in a spearate file */
const uint16_t mon_ver = 200;      /*!< monitor version */
const uint16_t en_fl_bbx = 1;  /*!< enable monitoring floating point variables in the recorder */
const uint16_t nr_tabs=MON_MAXTABS;/*!< maximum number of tabs */
const uint32_t Consy = 0; /*!< random number for consistency check */
const uint16_t dev_mode = 1;       /*!< if the value is non-zero password is not needed for the monitor */
const uint32_t len_ehist = 0; /*!< length of error history */
const uint16_t noofBBXch = MUCI_RECORDER_CHANNEL_NUMBER;
uint32_t serial_nr = 12345;

uint32_t BuildDate = 20201006;
uint32_t BuildTime = 100000;

/* extern variables */
 uint32_t c_date;
 uint32_t c_time;
 int32_t s_date;
 int32_t s_time;
 int16_t set_datetm;
 int16_t save_var;
 int16_t load_defv;


/* extern variables */
extern uint32_t BackgndCounter;

/* max, min values */
const int32_t max_s_date = 0x991231, min_s_date = 0x90101;
const int32_t max_s_time = 0x235959, min_s_time = 0x0;
const int16_t max_set_datetm = 1, min_set_datetm = 0;
const int16_t max_save_var = 1, min_save_var = 0;
const int16_t max_load_defv = 1, min_load_defv = 0;

uint16_t len_error_descriptor,len_mainsm_descriptor;

const var_type vars_misc[] = {
  { "en_fl_bbx", UNIT_NONE,  TYP_U16,                     CA_U32(en_fl_bbx),  0,              0                } ,  /*!< enable floating point monitoring in blackbox for all DSP */
  { "bbox_str",  UNIT_NONE,  TYP_U16,                     CA_U32(MUCI_RecorderParameter),0,              0                } ,  /*!< address of bbx struct for all DSP */
  { "f_samp",    UNIT_NONE,  TYP_U32,                     CA_U32(f_samp),     0,              0                } ,  /*!< sampling frequency by bbx_trigger() for all DSP */
  { "Consy",     UNIT_NONE,  TYP_U32,                     CA_U32(Consy),      0,              0                } ,  /*!< for the consistency check for all DSP */
  { "date",      UNIT_NONE,  TYP_HEX|TYP_U32,             CA_U32(c_date),     0,              0                } ,  /*!< current date for all DSP */
  { "time",      UNIT_NONE,  TYP_HEX|TYP_U32,             CA_U32(c_time),     0,              0                } ,  /*!< current time for all DSP */
  { "s_date",    UNIT_NONE,  TYP_ENWR|TYP_32BIT,          CA_U32(s_date),     CA_U32(max_s_date),CA_U32(min_s_date)} ,  /*!< for date setup for all DSP */
  { "s_time",    UNIT_NONE,  TYP_ENWR|TYP_32BIT,          CA_U32(s_time),     CA_U32(max_s_time),CA_U32(min_s_time)} ,  /*!< for time setup for all DSP */
  { "set_dt",    UNIT_NONE,  TYP_ENWR|TYP_16BIT,          CA_U32(set_datetm), CA_U32(max_set_datetm),CA_U32(min_set_datetm)} ,  /*!< for date,time setup for all DSP */
  { "save_var",  UNIT_NONE,  TYP_ENWR|TYP_16BIT,          CA_U32(save_var),   CA_U32(max_save_var),CA_U32(min_save_var)} ,  /*!< save variables for all DSP */
  { "load_defv", UNIT_NONE,  TYP_ENWR|TYP_16BIT,          CA_U32(load_defv),  CA_U32(max_load_defv),CA_U32(min_load_defv)} ,  /*!< load default variable values for all DSP */
  { "serial",    UNIT_NONE,  TYP_U32,                     CA_U32(serial_nr),  0,              0                } ,  /*!< serial number for the pwd generation */
  { "dev_mode",  UNIT_NONE,  TYP_U16,                     CA_U32(dev_mode),   0,              0                } ,  /*!< development mode, if the value is non-zero the Monitor goes immediatelly into dev mode */
  { "mon_ver",   UNIT_NONE,  TYP_U16,                     CA_U32(mon_ver),    0,              0                } ,  /*'< monitor version */
  { "l_ehist",   UNIT_NONE,  TYP_U32,                     CA_U32(len_ehist),  0,              0                } ,  /*!< length of error history in entries */
  { "l_errdesc", UNIT_NONE,  TYP_U16,                     CA_U32(len_error_descriptor),0,              0                } ,  /*'< length of the error texts in characters */
  { "l_msmdesc", UNIT_NONE,  TYP_U16,                     CA_U32(len_mainsm_descriptor),0,              0                } ,  /*'< length of the mainsm texts in characters */
  { "errdesc",   UNIT_NONE,  TYP_U16,                     CA_U32(error_descriptor),0,              0                } ,  /*'< used to get the pointer to the error history text */
  { "msmdesc",   UNIT_NONE,  TYP_U16,                     CA_U32(mainsm_descriptor),0,              0                } ,  /*'< used to get the pointer to the mainsm  text */
  { "noofBBXch", UNIT_NONE,  TYP_U16,                     CA_U32(noofBBXch),  0,              0                } ,  /*< number of bbx channels it can be only 4 or 8 */
  { "bbxmemlen", UNIT_NONE,  TYP_U32,                     CA_U32(MUCI_RecorderLength),0,              0                } ,  /*'< used to get the maximum record length in bbx */
};

const var_type vars_v1[] = {
  { "BackgndCounter",UNIT_NONE,  TYP_U32,                     CA_U32(BackgndCounter),0,              0                } ,  
};


/*! Titles of the disserent tabs: the 0th one is the Tab name, since it is not a real tab, but only used for the HiTerm.
*/
const uint8_t tabtitle[MON_MAXTABS][MAXCH_TABNAME+1] = { {"DCDC"},{"Common"}};
/*! Pointers for the tab descriptors. Used for all DSPs.*/
const var_type* tabptr[MON_MAXTABS] = {vars_misc, vars_v1};
/*! Size of the tab descriptors, calculated in run time.*/
uint16_t tabsize[MON_MAXTABS];

const uint8_t units[][MAXCH_UNIT+1] = { {""},
                                        {"A"},
                                        {"V"},
                                        {"kVA"},
                                        {"kW"},
                                        {"s"},
                                        {"hour"},
                                        {"Nm"},
                                        {"kWh"},
                                        {"Hz"},
                                        {"Vs"},
                                        {"NA"},
                                        {"W"},
                                        {"VAr"},
                                        {"VA"},
                                        {"rad/s"},
                                        {"DI"},
                                        {"rad"},
                                        {"Ver"},
                                        {"�C"},
                                        {"%"},
                                        {"BitF"},
                                        {"100ms"},
                                        {"ms"},
                                        {"mA"},
                                        };
const desc_type error_descriptor[LEN_ERR_DESC] = {
  {0   , "No error" },
  {1   , "Overcurrent R" },
  {2   , "Overcurrent S" },
  {3   , "Overcurrent T" },
  {4   , "Overv DC-link" },
  {5   , "Overvoltage R" },
  {6   , "Overvoltage S" },
  {7   , "Overvoltage T" },
  {8   , "IGBT sat prot R" },
  {9   , "IGBT sat prot S" },
  {10  , "IGBT sat prot T" },
  {11  , "Overload R" },
  {12  , "Overload S" },
  {13  , "Overload T" },
  {44  , "Unknown error" },
};

const desc_type mainsm_descriptor[LEN_MAINSM_DESC] = {
  {0   , "Stopped" },
  {1   , "Starting" },
  {2   , "Operational" },
  {3   , "Waiting" },
  {99  , "Crazy state" },
};

void InitTermVars()
{
tabsize[0] = sizeof(vars_misc);
tabsize[1] = sizeof(vars_v1);

len_error_descriptor = sizeof(error_descriptor);
len_mainsm_descriptor = sizeof(mainsm_descriptor);

}
