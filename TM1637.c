/*
 * TM1637.c
 *
 *  Created on: 24-Oct-2025
 *
 *      Author: Rishiesh Das, +91 9511705410,	rishe71199@gmail.com
 */

#include "numicro_8051.h"
#include "TM1637.h"

/* " 0 - 9 " Number '7 Segment Pattern' Array */
const uint8_t TM_Number_Arr[ 10 ] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };

/* Button IC Array */
const uint8_t TM_Button_ID_Arr[ 16 ] = { TM_KEY_S1_ID, TM_KEY_S2_ID, TM_KEY_S3_ID, TM_KEY_S4_ID,
										 TM_KEY_S5_ID, TM_KEY_S6_ID, TM_KEY_S7_ID, TM_KEY_S8_ID,
										 TM_KEY_S9_ID, TM_KEY_S10_ID, TM_KEY_S11_ID, TM_KEY_S12_ID,
										 TM_KEY_S13_ID, TM_KEY_S14_ID, TM_KEY_S15_ID, TM_KEY_S16_ID };

const uint8_t TM_ASCII[30];

// Delay for TM1637 Processes
void TM1637_Delay( uint32_t Delay ){
	uint32_t LV_Counter;

	while( Delay-- ) {
		LV_Counter = 10;
		while( LV_Counter-- ) {
			__asm__( "NOP" );
		}
	}
}

/* *********************************************************************************
 * ******************************************************************************* */
// Modefy this set of Code for interoperatibility with other Hardwair
void TM1637_Data_PIN( uint8_t Value ) {

	P05 = Value;

}

uint8_t TM1637_Data_PIN_Read( void ) {

	uint8_t LV_Return = P05;

	return LV_Return;
}

void TM1637_CLK_PIN( uint8_t Value ) {
	P06 = Value;
}

/* ***********************************************************************************/

void TM1637_Data_PIN_Input_Mode( void ) {
	TM1637_Data_PIN(1);
}

/* Start Condition */
void TM1637_Start( void ) {

	TM1637_CLK_PIN(1);		// Clock HIGH
	TM1637_Data_PIN(1);	// DATA HIGH
	TM1637_Delay( 2 );

	TM1637_Data_PIN(0);	// Clock LOW
	TM1637_Delay( 2 );
	TM1637_CLK_PIN(0);		// Data LOW
}

/* Stop Condition */
void TM1637_Stop( void ) {

	TM1637_CLK_PIN(0);		// Clock LOW
	TM1637_Delay( 2 );
	TM1637_Data_PIN(0);	// Data LOW
	TM1637_Delay( 2 );

	TM1637_CLK_PIN(1);		// Clock HIGH
	TM1637_Delay( 2 );
	TM1637_Data_PIN(1);	// Data HIGH

	TM1637_Delay( 5 );
}

/* Function to ACK */
void TM1637_ACK_Detect( void ) {
	TM1637_Data_PIN_Input_Mode();		// Set Data pin as Input Mode

	TM1637_CLK_PIN(0);		// Clock Pin LOW
	TM1637_Delay( 5 );

	while( TM1637_Data_PIN_Read() != 0 ) {}		// Waite for Data pin to Pull LOW

	// One Clock Signal
	TM1637_CLK_PIN(1);		// Clock HIGH
	TM1637_Delay( 2 );
	TM1637_CLK_PIN(0);		// Clock LOW

}

/* Function is to Send 8-bit Data to TM1637 IC
 * @Data Argument is where you load you 8-bit data which you want to send to IC
 * */
void TM1637_Send_Data( uint8_t Data ) {

	uint8_t LV_Counter;
	uint8_t LV_Data = Data;

	// Sending 8-bit Data
	for(LV_Counter = 0; LV_Counter < 8; LV_Counter++) {


		TM1637_Data_PIN(LV_Data & 0x1);	// Send Bit

		LV_Data >>= 1;		// Left Shift by 1
		//-----------------------------------------

		TM1637_Delay( 1 );
		TM1637_CLK_PIN(1);		// Clock HIGH

		TM1637_Delay( 2 );
		TM1637_CLK_PIN(0);		// Clock LOW
	}
}

/* Function is to generate 7 Segment Pattern return the 7 segment Pattern
 * @ Number => Pass the 0 - 9 valid pattern number to return the Decimal Pattern */
uint8_t TM1637_Get_Number_Patern( uint8_t Number ) {

	uint8_t LV_Return = 0;		// Load Blank Pattern

	// Confirm Number is Less then 9
	if( Number <= 9 ) {
		LV_Return = TM_Number_Arr[ Number ];		// Load Number data Pattern from Array to Return variable
	}

	return LV_Return;
}

uint8_t TM1637_Get_Char_Patern( char Charector ) {
	uint8_t LV_Return = Charector;

	return LV_Return;
}

/* Function is set the single bit of 7 segment Display
 * @ Display_Number => 0-5 valid display number has to be enter for selection of display
 * @ Display_Patern => provide 8 bit display Pattern
 * */

void TM1637_Set_Display ( uint8_t Display_Number, uint8_t Display_Patern, uint8_t Decimal_Poin ) {

	TM1637_Start();
	TM1637_Send_Data( 0XC0 + Display_Number );		// Set Display ID
	TM1637_ACK_Detect();

	// Update Display
	if(Decimal_Poin != TM_Decimal_NULL) {
		TM1637_Send_Data( Display_Patern | 0x80 );
	}
	else {
		TM1637_Send_Data( Display_Patern );
	}
	TM1637_ACK_Detect();

	TM1637_Stop();
}

void TM1637_Set_Display_2Digit ( uint8_t Display_Start_Number, uint16_t Value, uint8_t Decimal_Point_Number ) {

	uint8_t LV_Arr[ 2 ];
	int8_t LV_Counter;

	// Seperating indivisual Numbers
	for(LV_Counter = 1; LV_Counter >= 0; LV_Counter--) {
		LV_Arr[ LV_Counter ] = Value % 10;
		Value /= 10;
	}

	TM1637_Start();
	TM1637_Send_Data( 0XC0 + Display_Start_Number);
	TM1637_ACK_Detect();

	for( LV_Counter = 0; LV_Counter < 2; LV_Counter++ ) {
		if(Decimal_Point_Number >> LV_Counter & 0x1) {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) | 0x80 );
		}
		else {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) );
		}
		TM1637_ACK_Detect();
	}

	TM1637_Stop();
}

void TM1637_Set_Display_3Digit ( uint8_t Display_Start_Number, uint16_t Value, uint8_t Decimal_Point_Number ) {
	
	uint8_t LV_Arr[ 3 ];
	int8_t LV_Counter;

	// Seperating indivisual Numbers
	for(LV_Counter = 2; LV_Counter >= 0; LV_Counter--) {
		LV_Arr[ LV_Counter ] = Value % 10;
		Value /= 10;
	}

	TM1637_Start();
	TM1637_Send_Data( 0XC0 + Display_Start_Number);
	TM1637_ACK_Detect();

	for( LV_Counter = 0; LV_Counter < 3; LV_Counter++ ) {
		if(Decimal_Point_Number >> LV_Counter & 0x1) {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) | 0x80 );
		}
		else {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) );
		}
		TM1637_ACK_Detect();
	}
	TM1637_Stop();
}


void TM1637_Set_Display_4Digit ( uint8_t Display_Start_Number, uint16_t Value, uint8_t Decimal_Point_Number ) {

	uint8_t LV_Arr[ 4 ];
	int8_t LV_Counter;

	// Seperating indivisual Numbers
	for(LV_Counter = 3; LV_Counter >= 0; LV_Counter--) {
		LV_Arr[ LV_Counter ] = Value % 10;
		Value /= 10;
	}

	TM1637_Start();
	TM1637_Send_Data( 0XC0 + Display_Start_Number);
	TM1637_ACK_Detect();

	for( LV_Counter = 0; LV_Counter < 4; LV_Counter++ ) {
		if(Decimal_Point_Number >> LV_Counter & 0x1) {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) | 0x80 );
		}
		else {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) );
		}
		TM1637_ACK_Detect();
	}
	TM1637_Stop();
}

void TM1637_Set_Display_5Digit ( uint8_t Display_Start_Number, uint32_t Value, uint8_t Decimal_Point_Number ) {

	uint8_t LV_Arr[ 5 ];
	int8_t LV_Counter;

	// Seperating indivisual Numbers
	for(LV_Counter = 4; LV_Counter >= 0; LV_Counter--) {
		LV_Arr[ LV_Counter ] = Value % 10;
		Value /= 10;
	}

	TM1637_Start();
	TM1637_Send_Data( 0XC0 + Display_Start_Number);
	TM1637_ACK_Detect();

	for( LV_Counter = 0; LV_Counter < 5; LV_Counter++ ) {
		if( Decimal_Point_Number >> LV_Counter & 0x1 ) {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) | 0x80 );
		}
		else {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) );
		}
		TM1637_ACK_Detect();
	}
	TM1637_Stop();
}

void TM1637_Set_Display_6Digit ( uint8_t Display_Start_Number, uint32_t Value, uint8_t Decimal_Point_Number ) {

	uint8_t LV_Arr[ 6 ];
	int8_t LV_Counter;

	// Seperating indivisual Numbers
	for( LV_Counter = 5; LV_Counter >= 0; LV_Counter-- ) {
		LV_Arr[ LV_Counter ] = Value % 10;
		Value /= 10;
	}

	TM1637_Start();
	TM1637_Send_Data( 0XC0 + Display_Start_Number);
	TM1637_ACK_Detect();

	for( LV_Counter = 0; LV_Counter < 6; LV_Counter++ ) {
		if(Decimal_Point_Number >> LV_Counter & 0x1) {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) | 0x80 );
		}
		else {
			TM1637_Send_Data( TM1637_Get_Number_Patern( LV_Arr[ LV_Counter ] ) );
		}
		TM1637_ACK_Detect();
	}
	TM1637_Stop();
}



/* Function Return Success (1) or Not-Success (0) in Valid Button Read
 * 
 * @ Return_Button_ID_ptr = Pointer to return Button ID which was Read
 * 
 * */
uint8_t TM1637_Get_Button_ID( uint8_t* Return_Button_ID_ptr ) {

	uint8_t i, LV_Key_ID = 0, Pass_Flag = 0;

	TM1637_Start();
	TM1637_Send_Data( 0x42 );			// Key Address	send
	TM1637_ACK_Detect();

	TM1637_Data_PIN_Input_Mode();

	TM1637_Delay( 2 );

	// reading 8 bit Data
	for(i = 0; i < 8; i++) {

		TM1637_CLK_PIN(1);		// Clock HIGH
		TM1637_Delay( 10 );

		LV_Key_ID <<= 1;
		if( TM1637_Data_PIN_Read() ) {
			LV_Key_ID |= 0x01;		// Load bit
		}

		TM1637_Delay( 10 );
		TM1637_CLK_PIN(0);		// Clock LOW
		TM1637_Delay( 10 );
	}

	TM1637_ACK_Detect();
	TM1637_Stop();

	// Verifying the Key Read
	if( LV_Key_ID != 0xFF ) {
		*Return_Button_ID_ptr = LV_Key_ID;

		Pass_Flag = 1;
	}

	return Pass_Flag;
}


uint8_t TM1637_Get_Button_Number ( uint8_t* Return_Button_Number_ptr ) {
	uint8_t LV_Button_ID = 0;
	uint8_t LV_Button_Number = 0xFF;		// Button Null
	uint8_t LV_Flag = 0;
	uint8_t LV_Counter;

	LV_Flag = TM1637_Get_Button_ID( &LV_Button_ID );

	if( LV_Flag ) {
		for( LV_Counter = 0; LV_Counter <= 16; LV_Counter++ ) {
			if( TM_Button_ID_Arr[ LV_Counter ] == LV_Button_ID ) {
				LV_Button_Number = LV_Counter + 1;

				break;
			}
		}
	}

	*Return_Button_Number_ptr = LV_Button_Number;

	return LV_Flag;
}



// Initialization of TM1637 IC
void TM1637_Init( void ) {

	uint8_t LV_Counter;

	//TM1637_CLK_OutPut_Mode;
	//TM1637_DATA_OutPut_Mode;
	P05_OPENDRAIN_MODE;		// Data Pin
	P06_OPENDRAIN_MODE;		// Clock Oin

	TM1637_CLK_PIN(1);
	TM1637_Data_PIN(0);


	//-------------------------------------
	TM1637_Start();
	TM1637_Send_Data( 0x40 );
	TM1637_ACK_Detect();
	TM1637_Stop();


	TM1637_Start();
	TM1637_Send_Data( 0XC0 );
	TM1637_ACK_Detect();

	for( LV_Counter = 0; LV_Counter < 6; LV_Counter++ ) {
		TM1637_Send_Data( TM1637_Get_Number_Patern( 8 ) );
		TM1637_ACK_Detect();
	}

	TM1637_Stop();

	TM1637_Start();
	TM1637_Send_Data( 0x8F );
	TM1637_ACK_Detect();
	TM1637_Stop();
}
