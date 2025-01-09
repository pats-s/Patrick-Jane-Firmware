#ifndef  HARDWARE_H
#define  HARDWARE_H

#include "stdbool.h"    /*in order to use "bool" type*/

//List all IOs here:

// pins for first 7 seg
#define CT_D1_BCD0        PIN_E4
#define CT_D1_BCD1        PIN_E5
#define CT_D1_BCD2        PIN_E6
#define CT_D1_BCD3        PIN_E7
// pins for scnd 7 seg

#define CT_D2_BCD0        PIN_E0
#define CT_D2_BCD1        PIN_E1
#define CT_D2_BCD2        PIN_E2
#define CT_D2_BCD3        PIN_E3

#define CT_COL PIN_G4

// pins for third 7 seg
#define CT_D3_BCD0        PIN_D4
#define CT_D3_BCD1        PIN_D5
#define CT_D3_BCD2        PIN_D6
#define CT_D3_BCD3        PIN_D7

// pins for fourth 7 seg
#define CT_D4_BCD0        PIN_D0
#define CT_D4_BCD1        PIN_D1
#define CT_D4_BCD2        PIN_d2
#define CT_D4_BCD3        PIN_D3

//pins for floor segment
#define F_BCD_0 PIN_G0
#define F_BCD_1 PIN_G1
#define F_BCD_2 PIN_G2
#define F_BCD_3 PIN_G3

// #define SEG1_A_ON()       output_high(CT_D1_BCD0)
// #define SEG1_A_OFF()      output_low(CT_D1_BCD0)
// #define SEG1_B_ON()       output_high(CT_D1_BCD1)
// #define SEG1_B_OFF()      output_low(CT_D1_BCD1)
// #define SEG1_C_ON()       output_high(CT_D1_BCD2)
// #define SEG1_C_OFF()      output_low(CT_D1_BCD2)
// #define SEG1_D_ON()       output_high(CT_D1_BCD3)
// #define SEG1_D_OFF()      output_low(CT_D1_BCD3)

// /* Display 2 */
// #define SEG2_A_ON()       output_high(CT_D2_BCD0)
// #define SEG2_A_OFF()      output_low(CT_D2_BCD0)
// #define SEG2_B_ON()       output_high(CT_D2_BCD1)
// #define SEG2_B_OFF()      output_low(CT_D2_BCD1)
// #define SEG2_C_ON()       output_high(CT_D2_BCD2)
// #define SEG2_C_OFF()      output_low(CT_D2_BCD2)
// #define SEG2_D_ON()       output_high(CT_D2_BCD3)
// #define SEG2_D_OFF()      output_low(CT_D2_BCD3)

// /* Display 3 */
// #define SEG3_A_ON()       output_high(CT_D3_BCD0)
// #define SEG3_A_OFF()      output_low(CT_D3_BCD0)
// #define SEG3_B_ON()       output_high(CT_D3_BCD1)
// #define SEG3_B_OFF()      output_low(CT_D3_BCD1)
// #define SEG3_C_ON()       output_high(CT_D3_BCD2)
// #define SEG3_C_OFF()      output_low(CT_D3_BCD2)
// #define SEG3_D_ON()       output_high(CT_D3_BCD3)
// #define SEG3_D_OFF()      output_low(CT_D3_BCD3)

// /* Display 4 */
// #define SEG4_A_ON()       output_high(CT_D4_BCD0)
// #define SEG4_A_OFF()      output_low(CT_D4_BCD0)
// #define SEG4_B_ON()       output_high(CT_D4_BCD1)
// #define SEG4_B_OFF()      output_low(CT_D4_BCD1)
// #define SEG4_C_ON()       output_high(CT_D4_BCD2)
// #define SEG4_C_OFF()      output_low(CT_D4_BCD2)
// #define SEG4_D_ON()       output_high(CT_D4_BCD3)
// #define SEG4_D_OFF()      output_low(CT_D4_BCD3)

/* Colon LED */
#define COL_ON()          output_high(CT_COL)
#define COL_OFF()         output_low(CT_COL)



///////////////////////////////////////////////////////////////////////////////
bool init_hw(void);

///////////////////////////////////////////////////////////////////////////////
#endif
