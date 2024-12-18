#ifndef _TERM_DEFS_
#define _TERM_DEFS_

/*-----------------------------------------------------------------------------*/

#define MAX_CHAR_LEN      11        /*!< Maximal length of the variable name */
#define MAXCH_TABNAME     11        /*!< Maximal length of the tab name */
#define MAXCH_DSPNAME     11        /*!< Maximal length of the DSP name */
#define MAXCH_UNIT         5        /*!< Maximal length of the unit string */

#define MON_MAXTABS        2 /** First tab is not visible */

#define TYP_32BIT       0x00        /*!< Variable is 32bit */
#define TYP_16BIT       0x01        /*!< Variable is 16bit */
#define TYP_SIGNED      0x00        /*!< Variable is signed */
#define TYP_UNSIGNED    0x02        /*!< Variable is unsigned */
#define TYP_IQ(n)       (4*n)       /*!< Variable is _IQn type 0-30.*/
#define TYP_HEX         0x80        /*!< Variable is hexadecimal */
#define TYP_FLOAT       0x100       /*!< Variable is floating type */
#define TYP_ENWR        0x200       /*!< Variable is writable */
#define TYP_SHIFT(n)    (0x400 * n) /*!< Variable is needs to be shifted with n for the recorder n <= 16 */

#define TYP_U32         (TYP_UNSIGNED+TYP_32BIT)
#define TYP_S32         (TYP_SIGNED+TYP_32BIT)
#define TYP_U16         (TYP_UNSIGNED+TYP_16BIT)
#define TYP_S16         (TYP_SIGNED+TYP_16BIT)

#define TYP_IQ_MASK     0x7C        /*!< Mask to get IQ number */
#define TYP_IQ_SHIFT    2           /*!< Shift to get IQ number */
#define TYP_SIGN_MASK   0x02        /*!< Mask to get the sign of variable */
#define TYP_BIT_MASK    0x01        /*!< Mask to get the type for the variable (bit nr) */
#define TYP_DISP_MASK   (4*0x1F)    /*!< Mask to get the Variable is 32bit */
#define TYP_SHIFT_MASK  0x3C00      /*!< Mask to get the shifting */
#define TYP_SH_SHIFT    10          /*!< Shifting used to get the shift values */
#define TYP_SH_IQ       2           /*!< Shifting used to get the iq values */


#define CA_U32(ad) (uint32_t)(&ad)  /*!< Macro to generate the address to store it in the flash of the processor */

typedef struct {
  uint8_t   name[MAX_CHAR_LEN + 1]; /*!< name of the variable, it is uint8_t type, but stored as if it were uint16_t type */
  uint16_t  unit;                   /*!< unit for the given variable */
  uint16_t  type;                   /*!< type of variable (bit length, display format, R/RW */
  uint32_t  ptr;                    /*!< address of the variable */
  uint32_t  pmax;                   /*!< pointer to the maximal value of the variable valid, if writable */
  uint32_t  pmin;                   /*!< pointer to the minimal value of the variable valid, if writable */
} var_type;

extern const var_type vars_misc[];

#define UNIT_NONE      0
#define UNIT_A         1
#define UNIT_V         2
#define UNIT_KVA       3
#define UNIT_KW        4
#define UNIT_S         5
#define UNIT_H         6
#define UNIT_NM        7
#define UNIT_KWH       8
#define UNIT_HZ        9
#define UNIT_VS       10
#define UNIT_NA       11
#define UNIT_W        12
#define UNIT_Var      13
#define UNIT_VA       14
#define UNIT_RADPS    15
#define UNIT_DI       16
#define UNIT_RAD      17
#define UNIT_VER      18
#define UNIT_DEGC     19
#define UNIT_PERC     20
#define UNIT_BITF     21
#define UNIT_100MS    22
#define UNIT_MS       23
#define UNIT_MA       24

#define MAXCH_DESC 16               /*!< length of error descriptors in characters */
typedef struct {
  uint8_t desc_number;              /*!< descriptor number - eg error code */
  uint8_t desc_text[MAXCH_DESC+1];  /*!< descriptor text */
} desc_type;

#define LEN_ERR_DESC    15
#define LEN_MAINSM_DESC 5

extern const desc_type error_descriptor[LEN_ERR_DESC],mainsm_descriptor[LEN_MAINSM_DESC];

extern const uint8_t tabtitle[MON_MAXTABS][MAXCH_TABNAME+1];
extern uint16_t tabsize[MON_MAXTABS];
extern const var_type* tabptr[MON_MAXTABS];
extern const uint8_t units[25][MAXCH_UNIT+1];

/* Term_defs functions */
void InitTermVars();

#endif

