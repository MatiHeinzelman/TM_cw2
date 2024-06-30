#include "main.h"

unsigned short int *const LCD_FB = (unsigned short int*) 0xD0000000;
unsigned short int *const LCD_BUF = (unsigned short int*) 0xD0100000;

 short int menustat=1;
unsigned short int t=0;
 short int direct=1;
unsigned short int PozycjePionoweWpisowMenu[] = {20, 50, 80, 150};
uint8_t text0[]="Oswietlenie";
uint8_t text1[]="Ogrzewanie";
uint8_t text2[]="Alarm";
uint8_t text3[]="O aplikacji";
uint8_t* texty[]={text0,text1,text2,text3};

void WypiszMenu(void) {
	Clear_And_Reload_Screen();
	for ( int i=0;i<4;i++){
		if( menustat == i ) { RysujPodswietlenie(PozycjePionoweWpisowMenu[i]); }
		BSP_LCD_DisplayStringAt(0, PozycjePionoweWpisowMenu[i], texty[i], 1);
	}
	t=0;
}

void RysujPodswietlenie(unsigned int pozycja) {
	unsigned short int colors[] = {0xF800, 0x07E0, 0x001F}; // Kolejne kolory: czerwony, zielony, niebieski
	unsigned int colorIndex = 0;
	unsigned int x_start = 0;
	unsigned int x_end = LCD_WIDTH;
	for ( unsigned int x = x_start; x < x_end; x++) {
		unsigned int segmentHeight = 16 / 5; // Wysokość jednego segmentu dla każdego koloru
		for ( unsigned int y = pozycja; y < pozycja + 16; y++) {
			unsigned int segmentIndex = (y - pozycja) / segmentHeight;
			// Ustawianie kolorów na podstawie indeksu segmentu
			if (segmentIndex == 0) {
				LCD_BUF[y * LCD_WIDTH + x] = colors[0]; // Czerwony segment
			} else if (segmentIndex == 1 || segmentIndex == 3) {
				LCD_BUF[y * LCD_WIDTH + x] = 0x0000; // Czarna cienka linia
			} else if (segmentIndex == 2) {
				LCD_BUF[y * LCD_WIDTH + x] = colors[1]; // Zielony segment
			} else if (segmentIndex == 4) {
				LCD_BUF[y * LCD_WIDTH + x] = colors[2]; // Niebieski segment
			}
		}
		colorIndex = (colorIndex + 1) % 3; // Cykliczne przełączanie się między kolorami
	}
}

 short int  nextMenu(  short int current ){
	switch( current ){
		case 0: { direct=1; } break;
		case 3: { direct=-1; } break;
	}
	return current+direct;
}

int main(void) {
System_Init();
WypiszMenu();
t=0;
	while(1){
		if ( t>500 ) {
			t=0;
			menustat=nextMenu( menustat );
			WypiszMenu();
		}
	}
}

void SysTick_Handler(void) {
	t++;
	HAL_IncTick();
}


void System_Init() {
	HAL_Init();
	SystemClock_Config();
	BSP_SDRAM_Init();
	BSP_LCD_Init();
}

void Clear_And_Reload_Screen() {
	for (int off = 0; off < 320*240; off++) {
		LCD_FB[off] = LCD_BUF[off];
		LCD_BUF[off] = 0;
	}
}
