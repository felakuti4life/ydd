/* f3d_i2c.c --- 
 * 
 * Filename: f3d_i2c.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Oct 31 09:10:53 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <f3d_i2c.h>
#include <f3d_delay.h>

void f3d_i2c1_init() {
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  // Enable the clock to the I2C peripheral 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  // Port B - SDA (7) and SCL (6)

  // I2C GPIO Initialization and Alternate Function Selection 
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_4);
  
  
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = 0x00902025;

  I2C_Init(I2C1, &I2C_InitStructure);
  
  I2C_Cmd(I2C1, ENABLE);
}

void f3d_i2c1_read(uint8_t device, uint8_t reg, uint8_t* buffer, uint16_t numbytes) {

  while (I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET);                                   // Wait for I2C to become free

  I2C_TransferHandling(I2C1, device, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);        // Send start and device address 
  while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);                                   // Confirm that the transmitter empty

  if (numbytes>1) {
    reg |=0x80;                                                                             // indicate that the transfer will involve multiple bytes
  }
  I2C_SendData(I2C1,reg);                                                                   // Send the register address
  while (I2C_GetFlagStatus(I2C1, I2C_ISR_TC) == RESET);                                   

  I2C_TransferHandling(I2C1, device, numbytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  while (numbytes--) {
    while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
    *buffer++ = I2C_ReceiveData(I2C1);
  }

  while (I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);                                   // Wait for stop flag generation

  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}

void f3d_i2c1_write(uint8_t device, uint8_t reg, uint8_t* value) {
  //printf("f3d write called... \n");
  //printf("f3d write Get Flag Status BUSY... \n");
  while (I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET);
  //printf("f3d write: Transfer Handling GenStart... \n");
  I2C_TransferHandling(I2C1, device, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  //printf("f3d write Get Flag Status TXIS 1... \n");
  while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET); 
  //printf("f3d write Send Data reg... \n");
  I2C_SendData(I2C1,reg);
  //printf("f3d write Get Flag Status TCR... \n");
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET);
  //printf("f3d write Transfer Handling NoStart... \n");
  I2C_TransferHandling(I2C1, device, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
  //printf("f3d write Get Flag Status TXIS... \n");
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
  //printf("f3d write Send Data value... \n");
  I2C_SendData(I2C1, *value);
  //printf("f3d write Get Flag Status STOPF... \n");
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);
  //printf("f3d write Clear Flag... \n");
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
  //printf("f3d write complete... \n");
}

void f3d_i2c1_read_nunchuk (uint8_t device, uint8_t* buffer, uint16_t numbytes) {

  while (I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET);                                   
  I2C_TransferHandling(I2C1, 0xA4, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);          
  while (I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);                                   
  I2C_SendData(I2C1,0x00);                                                                  
  while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
  delay(1);
  while (I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET);                                   
  I2C_TransferHandling(I2C1, 0xA4, 6, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  while (numbytes--) {
    while(I2C_GetFlagStatus(I2C1, I2C_ISR_RXNE) == RESET);
    *buffer++ = I2C_ReceiveData(I2C1);
  }
  while (I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);                                  
  I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
}

/* f3d_i2c.c ends here */
