#include "main.h"

unsigned short int *const LCD_FB = (unsigned short int*) 0xD0000000;
unsigned short int *const LCD_BUF = (unsigned short int*) 0xD0100000;

unsigned short int menustat=01;
unsigned short int PozycjePionoweWpisowMenu[] = {20, 50, 80, 150};
uint8_t text0[]="Oswietlenie";
uint8_t text1[]="Ogrzewanie";
uint8_t text2[]="Alarm";
uint8_t text3[]="O aplikacji";
uint8_t* texty[]={text0,text1,text2,text3};


// Deklaracja zmiennej ExtraDelay
unsigned int ExtraDelay;
enum AppState_t {A_MenuGlowne, A_AnimPodswietleniaMenu, A_AktywacjaWyboruMenu, A_Oswietlenie, A_Ogrzewanie, A_Alarm, A_oAplikacji};
enum AppState_t AppState = A_MenuGlowne;
enum PozycjeMenu_t {M_Oswietlenie=0, M_Ogrzewanie=1, M_Alarm=2, M_oAplikacji=3};





enum PozycjeMenu_t KolejnePozycjeMenu[] = {M_Ogrzewanie, M_Alarm, M_oAplikacji, M_oAplikacji};
enum PozycjeMenu_t PoprzedniePozycjeMenu[] = {M_Oswietlenie, M_Oswietlenie, M_Ogrzewanie, M_Alarm};
enum PozycjeMenu_t AktualnaPozycjaMenu = M_Alarm;
enum PozycjeMenu_t KolejnaPozycjaMenu;

unsigned int MenuActivationTimer;
unsigned int MenuAnimationTimer;

// Prototypy funkcji


void WypiszMenu(void);
void RysujPodswietlenie(unsigned int pozycja);



void WypiszMenu(void) {
	for (unsigned int i=0;i<4;i++){
		if(1==1) { RysujPodswietlenie(PozycjePionoweWpisowMenu[i]); }
		BSP_LCD_DisplayStringAt(0, PozycjePionoweWpisowMenu[i], texty[i], 1);
	}
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





int main(void) {
System_Init();
while(1){
	WypiszMenu();
	Clear_And_Reload_Screen();
}




while (1) {
switch(AppState) {
case A_MenuGlowne:
RysujPodswietlenie(PozycjePionoweWpisowMenu[AktualnaPozycjaMenu]);
WypiszMenu();


if(1000 > 3000) {
MenuAnimationTimer = 100;
AppState = A_AnimPodswietleniaMenu;
KolejnaPozycjaMenu = KolejnePozycjeMenu[AktualnaPozycjaMenu];
}
if(1000 < 1000) {
MenuAnimationTimer = 100;
AppState = A_AnimPodswietleniaMenu;
KolejnaPozycjaMenu = PoprzedniePozycjeMenu[AktualnaPozycjaMenu];
}
break;
case A_AnimPodswietleniaMenu:
MenuAnimationTimer--;
// Animacja paska podświetlania
unsigned int y0=PozycjePionoweWpisowMenu[AktualnaPozycjaMenu];
unsigned int y1=PozycjePionoweWpisowMenu[KolejnaPozycjaMenu];
unsigned int opoznienie_animacji = 100; // Domyślna wartość
unsigned int y = (y0 * MenuAnimationTimer + y1 * (opoznienie_animacji - MenuAnimationTimer)) / opoznienie_animacji;
RysujPodswietlenie(y);
WypiszMenu();
if(MenuAnimationTimer == 0) {
AktualnaPozycjaMenu = KolejnaPozycjaMenu;
AppState = A_MenuGlowne;
}
break;
case A_AktywacjaWyboruMenu:
	MenuActivationTimer--;
// Mrugnięcie diodą LED
	if (MenuActivationTimer == 0) {
		switch(AktualnaPozycjaMenu) {
			case M_Oswietlenie:   AppState = A_Oswietlenie;  break;
			case M_Ogrzewanie:    AppState = A_Ogrzewanie;   break;
			case M_Alarm:         AppState = A_Alarm;        break;
			case M_oAplikacji:    AppState = A_oAplikacji;   break;
		}
	}
break;
case A_Oswietlenie:
// Obsługa wyboru oswietlenia w menu, z możliwością powrotu do menu głównego
break;
case A_Ogrzewanie:
// Obsługa wyboru ogrzewania w menu, z możliwością powrotu do menu głównego
break;
case A_Alarm:
// Obsługa wyboru alarmu w menu, z możliwością powrotu do menu głównego
break;
case A_oAplikacji:
// Obsługa wyboru opcji "o aplikacji" w menu, z możliwością powrotu do menu głównego
break;
}
Clear_And_Reload_Screen();
}
}
void SysTick_Handler(void) {
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





