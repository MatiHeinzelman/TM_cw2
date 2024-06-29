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
int main2(void) ;














//Definicja struktury kwadrat z polami "upakowanymi" i wyrównanymi do 4-ech bajtów
typedef   struct {
	unsigned int x, y , size;
	int  dx , dy;
} __attribute__((packed,aligned(4))) kwadrat_t ;

#define ABS(x) ((unsigned int)(x>=0?x:-x))

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
HAL_StatusTypeDef HAL_Init(void);
void SystemClock_Config(void);
void handleGPIO(void);
void Clear_And_Reload_Screen(void);
void move_square_C(void);
void drawElements(void );
void printText(void);
void BSP_SDRAM_Init(void);
void BSP_LCD_Init(void);
void BSP_LCD_DisplayStringAt(uint16_t X, uint16_t Y, uint8_t *pText, Text_AlignModeTypdef mode);
void BSP_LCD_DisplayRot90StringAt(uint16_t X, uint16_t Y, uint8_t *pText, Text_AlignModeTypdef mode);

void move_square_asm(void);
void kopiuj_blok_pamieci(unsigned short int* FROM,unsigned short int* INTO,int SIZE);
void kasuj_blok_pamieci(unsigned short int* WHERE,int SIZE);
unsigned short int RGB16Pack(unsigned int B,unsigned int G,unsigned int R);
void fillMemory(void* adres_bazowy, unsigned long rozmiar,unsigned short int wartosc);
void SysTick_Handler(void);
void HAL_IncTick(void);
