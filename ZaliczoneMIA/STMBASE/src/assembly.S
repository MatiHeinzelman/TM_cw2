@Korzystaj ze składni UAL
   .syntax unified
@Definicje stałych
  .set PRESCALER_MAX,50
  .set PERIPH_BASE,0x40000000
  .set AHB1PERIPH_OFFSET, 0x00020000
  .set GPIOG_BASE,PERIPH_BASE+AHB1PERIPH_OFFSET+0x1800
  .set BSRR_OFFSET,0x18
  .set x_field_offset,0
  .set y_field_offset,4

@Definicja symboli globalnych (sprowadzanych do asemblera i udostępnianych dla języka C)
 .global move_square_asm
 .global RGB16Pack
 .global kopiuj_blok_pamieci
 .global kasuj_blok_pamieci
 .global kwadrat
  .global fillMemory

@Tworzenie sekcji danych na zmienne wykorzystywane w asemblerze
 .section .data
 .balign 4
 LEDSTATE_AT_0_AND_PRESCALER_AT_4:
 .word 0,PRESCALER_MAX
 dx_asm:
 .word 1
 dy_asm:
 .word 1


@Tworzenie sekcji programu z implementacją procedur i funkcji
.section .text

@Argumenty wejściowe procedury
@r0 - adres źródła
@r1 - adres przeznaczenia
@r2 - liczba słów do przekopiowania (wielokrotność 8)

kopiuj_blok_pamieci:
@Dzielebue R2 przez 8
      LSR   r2,r2,#3
@Odłożenie na stosie rejestrów, których procedura nie może modyfikować
      PUSH {r4-r10}
kopiuj_osiem_slow:
@Odczyt ośmiu słów źródłowych do rejestrów od r3 do r10
      LDM    r0!, {r3-r10}
@Zapis ośmiu rejestrów r3-r10 do miejsca przeznaczenia
      STM    r1!, {r3-r10}
      SUBS   r2, r2, #1
@Iteracja po wszystkich elementach kopiowanego bloku pamięci
      BNE    kopiuj_osiem_slow
@Odtworzenie stanu rejestrów, których procedura nie mogła modyfikować
      POP {r4-r10}
@Powrót z procedury
      MOV PC,LR



@Procedura konwersji składowych RBG do formatu RGB565
@R0 - składowa Blue
@R1 - składowa Green
@R2 - składowa Red

@R0 - Wyjście koloru w formacie RGB565

RGB16Pack:
@Składowa Blue
	AND r0,r0,#0x1f
@Składowa Green
	BFI r0,r1,#5,#6
@Składowa Red
	BFI r0,r2,#11,#5
	MOV PC,LR


@Procedura aktualizacji zmiennych globalnych określających współrzędne x,y ruchomego kwadratu na LCD
move_square_asm:

@Umieszcza w r0 i r2 stałe będące adresami zmiennych x oraz dx_asm
 ldr r0,=kwadrat+x_field_offset
 ldr r2,=dx_asm
@Odczytuje zmienne x oraz dx_asm i umieszcza je w r1 oraz r3
 ldr r3,[r2]
 ldr r1,[r0]
@r1=x+dx
 add r1,r1,r3
@Zapis zmiennej x
 str r1,[r0]
@Sprawdzenie u skok jeśli x<=1
 cmp r1, #1
 ble neg_x
@Sprawdzenie i zmiana znaku dx jeśli x>=(240-34) (skok dla x<240-34)
 cmp r1, #240-34
 blt no_neg_x
 neg_x:
@r3=0-r3=-dx
 rsb r3, r3, #0
@Zapis zmiennej dx
 str r3,[r2]
 no_neg_x:
 @Blok przetwarzania składowej y i dy. Analogiczny do bloku przetwarzania składowej x i dx.

@Umieszcza w r0 stałą będącą adresem zmiennej y
 ldr r0,=kwadrat+y_field_offset
 @Odczytuje zmienne y oraz dy_asm i umieszcza je w r1 oraz r3
 ldr r3,[r2,#4]
 ldr r1,[r0]
@r1=y+dy
 add r1,r1,r3
 str r1,[r0]
@Sprawdzenie i skok jeśli y<=1
 cmp r1, #1
 ble neg_y
@Sprawdzenie i zmiana znaku dy jeśli y>=(240-34) (skok dla y<240-34)
 cmp r1, #320-34
 blt no_neg_y
 neg_y:
@r3=0-r3=-dy
 rsb r3, r3, #0
@Zapis zmiennej dy
 str r3,[r2,#4]
 no_neg_y:
 mov pc,lr


@r0 - adres kasowanego obszaru
@r1 - liczba słów do skasowania (wielokrotnośc 8)

kasuj_blok_pamieci:
	  LSR   r1,r1,#3
	  PUSH {r4-r9}
	  LDR r2,=fill+4
	  LDM   r2, {r3-r9}
	  LDR r2,[r2]
wypelnij_osiem_slow:
      STM    r0!, {r2-r9}
      SUBS   r1, r1, #1
      BNE    wypelnij_osiem_slow
      POP {r4-r9}
      MOV PC,LR
fill:
	.word 0,0,0,0,0,0,0,0


@R0 -adres poczatku
@R1 - rozmiar obszaru
@R2 - wartosc 16-bitowa do wypelnienia
fillMemory:
 strh r2,[r0],#2
 subs r1,r1,#1
 bne fillMemory
 mov pc,lr
