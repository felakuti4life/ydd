//f3d_button.c
//by Ethan Geller
#include <stm32f30x.h>
#include <f3d_button.h>

void f3d_button_init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int f3d_button_state(void){
  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}
