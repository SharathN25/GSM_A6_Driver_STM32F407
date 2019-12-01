/**
  ***********************************************************************************************************
	*@file    : GSM_A6_Driver_STM32F407.c
	*@Author  : Sharath N
	*@Brief   : Source file for Driving GSM-A6 Module using STM32F407 Discovery Kit
  ***********************************************************************************************************
*/


#include "GSM_A6_Driver_STM32F407.h"
#include "string.h"
#include "stdint.h"

UART_HandleTypeDef myUARThandle;
DMA_HandleTypeDef myDMA_Uart2Handle;
char RX_Buffer[RX_BUFFER_SIZE];



/**
  *@brief    Configure UART
	*@param    None
	*@retval   None
*/

void UART_Config(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	
	GPIO_InitTypeDef myUartDef;
	myUartDef.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	myUartDef.Mode = GPIO_MODE_AF_PP;
	myUartDef.Pull = GPIO_PULLUP;
	myUartDef.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	myUartDef.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &myUartDef);
	//UART Configuration
	myUARThandle.Instance = USART2;
	myUARThandle.Init.BaudRate = 115200;
	myUARThandle.Init.Mode = UART_MODE_TX_RX;
	myUARThandle.Init.WordLength = UART_WORDLENGTH_8B;
	myUARThandle.Init.StopBits = UART_STOPBITS_1;
	myUARThandle.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&myUARThandle);
	
	//Systick interrupt enable
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/**
  *@brief    Configure DMA
	*@param    None
	*@retval   None
*/
void DMA_Config(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	myDMA_Uart2Handle.Instance = DMA1_Stream5;
	myDMA_Uart2Handle.Init.Channel = DMA_CHANNEL_4;
	myDMA_Uart2Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	myDMA_Uart2Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	myDMA_Uart2Handle.Init.MemInc = DMA_MINC_ENABLE;
	myDMA_Uart2Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	myDMA_Uart2Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	myDMA_Uart2Handle.Init.Mode = DMA_CIRCULAR;
	myDMA_Uart2Handle.Init.Priority = DMA_PRIORITY_LOW;
	myDMA_Uart2Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&myDMA_Uart2Handle);
	
	__HAL_LINKDMA(&myUARThandle,hdmarx,myDMA_Uart2Handle);
	
	//Enable DMA1 Stream 5 interrupt
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	
}



/**
  *@brief    Compares the GSM-data strings with given String
	*@param    string : Character pointer to sting to be compared
	*@retval   if given string present in the GSM_String, returns 1
	           else returns 0.
*/
int GSM_Compare_GSMData_With(const char* string)
{
	char* ptr = NULL;
  /*Compare given string with GSM data*/
	ptr = strstr(RX_Buffer, string);
	/* if ptr = NULL, then no match found else match found*/
  if(ptr!=NULL)
		return 1;
	else
		return 0;
}



/**
  *@brief    Send AT Command to GSM A6 Module
	*@param    AT_CMD : command string to be transmitted
	*@retval   None
*/
void GSM_Send_AT_Command(char* AT_CMD)
{
	HAL_UART_Transmit(&myUARThandle,(uint8_t*)AT_CMD, strlen(AT_CMD),2000);
}



/**
  *@brief    Receive Call. when this API is called its receive call 
             by sending "ATA" command to GSM module
	*@param    None
	*@retval   None
*/
void GSM_Receive_Call(void)
{
	int check = 0;
	
	HAL_Delay(1000);
	GSM_Send_AT_Command("ATA\r");  //ATA - Accept call command

	while(check == 0)
	{
		HAL_Delay(500);
		check = GSM_Compare_GSMData_With("ERROR");
	}
	
  HAL_Delay(100);
}




/**
  *@brief    Make a call to given phone number
	*@param    phone_number 
	*@retval   None
*/
void GSM_Make_Call(char* phone_number)
{
	char AT_CMD[16] ="ATD+91";    // AT command with +91(India code)
	char CR[] = "\r";             // Carriage return
	strcat(AT_CMD, phone_number); // Cancatenate AT_CMD and Phone number
	strcat(AT_CMD,CR);           //finally append "/r" at end
	GSM_Send_AT_Command(AT_CMD); //Send Command to make call
}



/**
  *@brief    Send Text Mesaage 
	*@param    Message:  Text message to be sent
  *@param    phone_number : Phone number
	*@retval   None
*/
void GSM_Send_SMS(char* Message, char* phone_number)
{
	char SMS_AT_CMD1[] = "AT+CMGF=1\r"; 
	char SMS_AT_CMD2[21] = "AT+CMGS=+91";
	char CR[] = "\r";
	uint8_t MSG_END[] ={26};
	
	strcat(SMS_AT_CMD2, phone_number);
	strcat(SMS_AT_CMD2,CR);
	
	GSM_Send_AT_Command(SMS_AT_CMD1);
	//HAL_UART_Transmit(&myUARThandle,(uint8_t *)SMS_AT_CMD1, strlen(SMS_AT_CMD1),2000); 
	HAL_Delay(750);
	GSM_Send_AT_Command(SMS_AT_CMD2);
	//HAL_UART_Transmit(&myUARThandle,(uint8_t *)SMS_AT_CMD2, strlen(SMS_AT_CMD2),2000);
	HAL_Delay(750);
	
	GSM_Send_AT_Command(Message);
	//HAL_UART_Transmit(&myUARThandle,(uint8_t *)Message, strlen(Message),2000);
	HAL_Delay(500);
	
	GSM_Send_AT_Command((char *)MSG_END);
	//HAL_UART_Transmit(&myUARThandle,(uint8_t *)MSG_END, sizeof(MSG_END),2000);
}




/**
  *@brief    Receive SMS
	*@param    None
	*@retval   None
*/
void GSM_Receive_SMS(void)
{
	
	///Receive code here
	//Pending
}




/**
  *@brief    Initialize the GSM A6 Module
	*@param    None
	*@retval   None
*/
void GSM_Init(void)
{
	/*Initialize UART*/
	UART_Config();
	
	/*Initialize DMA */
	DMA_Config();
	
	/* Start rceiving data as soon as data is available */
	HAL_UART_Receive_DMA(&myUARThandle,(uint8_t *)RX_Buffer, RX_BUFFER_SIZE);
	
  /* Wait 15 to 20 Seconds - Let the GSM module get connected to network */
	HAL_Delay(20000);
}

	

/*Sys tick handler for delay */
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}



/* DMA specidifc IRQ hnadler */
void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&myDMA_Uart2Handle);
}



