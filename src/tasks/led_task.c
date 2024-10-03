
#include "cmsis_os.h"
#include "main.h"

#include <stdint.h>

void ledTask(void *pv_arg) {
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  while (1) {
  }
}
