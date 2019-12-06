/**
  *******************************************************************************************************
	*@file    : main.c
	*@author  : Sharath N
	*@brief   : Sample example to "Make a call" and  "Receive a call" on STM32F407 using GSM A6 module
	            using --> "GSM_A6_Driver_STM32F407" driver code
  *******************************************************************************************************
*/	



#include "GSM_A6_Driver_STM32F407.h" 

int main(void)
{
	HAL_Init();
  GSM_Init();

	
  /*If you want to make a call, uncomment below code and give phone number */
 //GSM_Make_Call("<---Provide 10 digit phone number here---->");
	
	
 while(1)
 {
	 if(GSM_Compare_GSMData_With("RING"))
		 GSM_Receive_Call();
	 
	 if(GSM_Compare_GSMData_With("MESSAGE"))
		 GSM_Receive_SMS();
	 
	 HAL_Delay(500);
 
 }
	
}
