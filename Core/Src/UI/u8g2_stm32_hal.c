#include "u8g2.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;


uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  (void)u8x8;
  (void)arg_ptr;

  switch (msg)
  {
  case U8X8_MSG_DELAY_MILLI:
    HAL_Delay(arg_int);
    break;
  case U8X8_MSG_GPIO_CS:
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, arg_int);
    break;
  case U8X8_MSG_GPIO_DC:
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, arg_int);
    break;
  case U8X8_MSG_GPIO_RESET:
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, arg_int);
    break;
  }

  return 1;
}

uint8_t u8x8_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  (void)u8x8;

  switch (msg)
  {
  case U8X8_MSG_BYTE_SET_DC:
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, arg_int);
    break;
  case U8X8_MSG_BYTE_SEND:
    HAL_SPI_Transmit(&hspi1, (uint8_t *)arg_ptr, arg_int, 1000);
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
    break;
  }

  return 1;
}