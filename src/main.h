#include <stdint.h>
#include "stm32f429xx.h"
#include "stm32f4xx_gpio.h"

//Wymiary wyświetlacza
#define LCD_WIDTH 320U
#define LCD_HEIGHT 240U
#define LCD_LAST_X 318U
#define LCD_LAST_Y 239U

//Maksymalne składowe komponentów RGB
#define MAX_R 31U
#define MAX_G 63U
#define MAX_B 31U


void WypiszMenu(void);
unsigned short int  nextMenu( unsigned short int current );

void RysujPodswietlenie(unsigned int pozycja);
void SysTick_Handler(void);
void HAL_IncTick(void);



typedef enum
{
  CENTER_MODE             = 0x01,    /* center mode */
  RIGHT_MODE              = 0x02,    /* right mode  */
  LEFT_MODE               = 0x03,    /* left mode   */
}Text_AlignModeTypdef;
typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

void System_Init(void);
void Clear_And_Reload_Screen(void);
void BSP_SDRAM_Init(void);
void BSP_LCD_Init(void);
void BSP_LCD_DisplayStringAt(uint16_t X, uint16_t Y, uint8_t *pText, Text_AlignModeTypdef mode);



