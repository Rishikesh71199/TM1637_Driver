

#ifndef _TM1637_H_
#define _TM1637_H_

#include "numicro_8051.h"

/*************************************************************************
 *
 *  Modification to done by User for Attacting it their code
 *  */
// Defination of TM1637 IC
//#define TM1637_DATA_PIN					P05					// Pin for Data
//#define TM1637_CLK_PIN					P06					// pin for Clock

// pin Initialization Code
//#define TM1637_DATA_OutPut_Mode			P05_OPENDRAIN_MODE
//#define TM1637_DATA_InPut_Mode			TM1637_DATA_PIN = 1

//#define TM1637_CLK_OutPut_Mode			P06_OPENDRAIN_MODE

//************************************************************************
//----------------------------------------------------------------
// TM Key IDs
#define TM_KEY_S1_ID				`	0b11101111
#define TM_KEY_S2_ID					0b11110111
#define TM_KEY_S3_ID					0b11010111
#define TM_KEY_S4_ID					0b11001111
#define TM_KEY_S5_ID					0b01101111
#define TM_KEY_S6_ID					0b01110111
#define TM_KEY_S7_ID					0b01010111
#define TM_KEY_S8_ID					0b01001111
#define TM_KEY_S9_ID					0b10101111
#define TM_KEY_S10_ID					0b10110111
#define TM_KEY_S11_ID					0b10010111
#define TM_KEY_S12_ID					0b10001111
#define TM_KEY_S13_ID					0b00101111
#define TM_KEY_S14_ID					0b00110111
#define TM_KEY_S15_ID					0b00010111
#define TM_KEY_S16_ID					0b00001111

#define TM_Button_No_1					1U
#define TM_Button_No_2					2U
#define TM_Button_No_3					3U
#define TM_Button_No_4					4U
#define TM_Button_No_5					5U
#define TM_Button_No_6					6U
#define TM_Button_No_7					7U
#define TM_Button_No_8					8U
#define TM_Button_No_9					9U
#define TM_Button_No_10					10U
#define TM_Button_No_11					11U
#define TM_Button_No_12					12U
#define TM_Button_No_13					13U
#define TM_Button_No_14					14U
#define TM_Button_No_15					15U
#define TM_Button_No_16					16U

//---------------------------------------------------
//Decimal Point Macros
#define TM_Decimal_NULL					0b00000000
#define TM_Decimal_1					0b00000001
#define TM_Decimal_2					0b00000010
#define TM_Decimal_3					0b00000100
#define TM_Decimal_4					0b00001000
#define TM_Decimal_5					0b00010000
#define TM_Decimal_6					0b00100000

// Display Digit Macros
#define TM_Display_1					0U
#define TM_Display_2					1U
#define TM_Display_3					2U
#define TM_Display_4					3U
#define TM_Display_5					4U
#define TM_Display_6					5U


void TM1637_Delay ( uint32_t Delay );

void TM1637_Data_PIN( uint8_t Value );
uint8_t TM1637_Data_PIN_Read(void);
void TM1637_CLK_PIN( uint8_t Value );

void TM1637_Data_PIN_Input_Mode( void );

void TM1637_Start ( void );

void TM1637_Stop ( void );

void TM1637_ACK_Detect ( void );

uint8_t TM1637_Read_Data ( void );

void TM1637_Send_Data ( uint8_t Data );

uint8_t TM1637_Get_Number_Patern ( uint8_t Number );

uint8_t TM1637_Get_Char_Patern ( char Charector );

void TM1637_Set_Display ( uint8_t Display_Number, uint8_t Display_Patern, uint8_t Decimal_Poin );

void TM1637_Set_Display_2Digit ( uint8_t Display_Start_Number, uint16_t Value, uint8_t Decimal_Point_Number );

void TM1637_Set_Display_3Digit ( uint8_t Display_Start_Number, uint16_t Value, uint8_t Decimal_Point_Number );

void TM1637_Set_Display_4Digit ( uint8_t Display_Start_Number, uint16_t Value, uint8_t Decimal_Point_Number );

void TM1637_Set_Display_5Digit ( uint8_t Display_Start_Number, uint32_t Value, uint8_t Decimal_Point_Number );

void TM1637_Set_Display_6Digit ( uint8_t Display_Start_Number, uint32_t Value, uint8_t Decimal_Point_Number );

uint8_t TM1637_Get_Button_ID ( uint8_t* Return_Button_ID_ptr );

uint8_t TM1637_Get_Button_Number ( uint8_t* Return_Button_Number_ptr );

void TM1637_Init ( void );

#endif
