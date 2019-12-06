/**
  ***********************************************************************************************************
	*@file    : GSM_A6_Driver_STM32F407.h
	*@Author  : Sharath N
	*@Brief   : Header file for Driving GSM-A6 Module using STM32F407 Discovery Kit
  ***********************************************************************************************************
*/


#ifndef GSM_A6_Driver_STM32F407_H
#define GSM_A6_Driver_STM32F407_H

#include "stm32f4xx_hal.h"


#define RX_BUFFER_SIZE             128



/**
  *@brief    Configure UART
  *@param    None
  *@retval   None
*/
void UART_Config(void);


/**
  *@brief    Configure DMA
  *@param    None
  *@retval   None
*/
void DMA_Config(void);


/**
  *@brief    Compares the GSM-data strings with given String
  *@param    string : Character pointer to sting to be compared
  *@retval   if given string present in the GSM_String, returns 1
	           else returns 0.
*/
int GSM_Compare_GSMData_With(const char* string);


/**
  *@brief    Send AT Command to GSM A6 Module
  *@param    AT_CMD : command string to be transmitted
  *@retval   None
*/
void GSM_Send_AT_Command(char* AT_CMD);

/**
  *@brief    Make a call to given phone number
  *@param    phone_number 
  *@retval   None
*/
void GSM_Make_Call(char* phone_number);

/**
  *@brief    Receive Call. when this API is called it receives call 
             by sending "ATA" command to GSM module
  *@param    None
  *@retval   None
*/
void GSM_Receive_Call(void);


/**
  *@brief    Hang up the Call. when this API is called it terminates call 
             by sending "ATH" command to GSM module
  *@param    None
  *@retval   None
*/
void GSM_HangUp_Call(void);


/**
  *@brief    Send Text Mesaage 
  *@param    Message:  Text message to be sent
  *@param    phone_number : Phone number
  *@retval   None
*/
void GSM_Send_SMS(char* Message, char* phone_number);


/**
  *@brief    Receive SMS , Stores Message sender number in "Incoming_SMS_Phone_Num" and
	           Incoming Message in "Incoming_SMS_Message"
  *@param    None
  *@retval   None
*/
void GSM_Receive_SMS(void);


/**
  *@brief    Initialize the GSM A6 Module
  *@param    None
  *@retval   None
*/
void GSM_Init(void);

	

#endif
