/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
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

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_button.h>
#include <f3d_uart.h>
#include <queue.h>

#define INT_PER_SEC 12

extern queue_t txbuf, rxbuf;

static __IO uint32_t TimingDelay;

volatile int systick_flag = 0;

void f3d_systick_init(void) {
  NVIC_InitTypeDef NVIC_InitStructure ;
// No StructInit call in API
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  SysTick_Config(SystemCoreClock/INT_PER_SEC);
}

void SysTick_Handler(void) {
  if (!queue_empty(&txbuf)) {
    //flush_uart();
  }
}

void f3d_systick_delay(uint32_t nTime){
  //static __IO uint32_t TimingDelay;
  while(TimingDelay != 0) SysTick_Handler();
}

void f3d_systick_cycle_leds(){

}
/* f3d_systick.c ends here */
