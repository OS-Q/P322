

/*******************************************************************************
* Name  :  Qitas soft for CC1101
*******************************************************************************/
#include "stm32f10x_spi.h"
#include <stdio.h>
#include  "regs.h"
#include  "SPI.h"
/*******************************************************************************
* Function Name  : RF_HalInit
*******************************************************************************/
void Hal_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
    // CSn + SCLK + MOSI
    GPIO_InitStructure.GPIO_Pin   = SPI_CS | SPI_SCK | SPI_MOSI;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);
    // MISO
    GPIO_InitStructure.GPIO_Pin   = SPI_MISO;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GDO0_Pin | GDO2_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GDO_PORT, &GPIO_InitStructure);			
}
/*******************************************************************************
* Function Name  :
*******************************************************************************/
u8 GDO0_IRQ(void) 
{	
		EXTI_InitTypeDef EXTI_InitStructure;   
    NVIC_InitTypeDef NVIC_InitStructure; 	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
    EXTI_ClearITPendingBit(EXTI_Line10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);  	
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 
}
/*******************************************************************************
* Function Name  :
*******************************************************************************/
u8 GDO2_IRQ(void) 
{	
		EXTI_InitTypeDef EXTI_InitStructure;   
    NVIC_InitTypeDef NVIC_InitStructure; 	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
    EXTI_ClearITPendingBit(EXTI_Line11);
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;    
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);  	
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	  
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure); 
}


/*******************************************************************************
* Function Name  : halSpiByteWrite
* Description    : send 8 bit data to SDO line
*******************************************************************************/
void halSpiByteWrite(u8 cData)
{
    u8 i;
    for(i=0; i<8; i++)
    { 
         SCLK_LOW();  
         if((cData&0x80)==0x80)
         {
             SDO_HIGH();    
         }
         else
         {
             SDO_LOW();  
         }
         cData<<=1;
         Delay_us(3);
         SCLK_HIGH();    
         Delay_us(2);
    }
    SCLK_LOW();        
}

void halRfByteWrite(u8 cData)
{
    u8 i;
    for(i=0; i<8; i++)
    { 
         SCLK_LOW();  
         if((cData&0x80)==0x80)
         {
             SDO_HIGH();    
         }
         else
         {
             SDO_LOW();  
         }
         cData<<=1;
         Delay_us(3); 
         SCLK_HIGH();    
         Delay_us(2);
    }
    SCLK_LOW();        
}
/*******************************************************************************
* Function Name  : halSpiByteRead
* Description    : get 8 bit data from SDI line
*******************************************************************************/
u8 halSpiByteRead(void)
{
    u8 i, result;
    result = 0;
    SCLK_LOW();  
    Delay_us(3);
    for(i=0; i<8; i++)
    {
        SCLK_HIGH();
        result<<=1;
        if(SDI_IN())
        {
            result |= 0x01; 
        }
        Delay_us(2);
        SCLK_LOW(); 
        Delay_us(2);
    }
    return result;
}