/* C:\Users\Samsung\workspace\Base\Base.h
   Project: Base
   Created on: 14 mar 2017  21:43:20
   Author: Borys Horoneskul
   boryshoroneskul@gmail.com
   In project properties / C Buils/ Settings /ToolSettings/ Char is unsigned.
   Project properties / C Buils/ Settings /ToolSettings/Miscellanous:
   -Wno-char-subscripts -Wfatal-errors -Wno-pointer-sign
   Project / Clean must be done after each frequency change
	 Its not ready . Dont take it too serious!

 ATmega8:

(RESET) PC6							1			SIGN		28   PC5 (ADC5/SCL)
(RXD) PD0           		2     				27   PC4 (ADC4/SDA)
(TXD) PD1           		3     				26   PC3 (ADC3)
(INT0) PD2          		4     				25   PC2 (ADC2)
(INT1) PD3          		5     				24   PC1 (ADC1)
(XCK/T0) PD4       			6     				23   PC0 (ADC0)
VCC                 		7     				22   GND
GND                 		8     				21   AREF
(XTAL1/TOSC1) PB6				9     				20   AVCC
(XTAL2/TOSC2) PB7				10    				19   PB5 (SCK)
(T1) PD5           	 		11   	 				18   PB4 (MISO)
(AIN0) PD6          		12   	 				17   PB3 (MOSI/OC2)
(AIN1) PD7          		13   	 				16   PB2 (SS/OC1B)
(ICP1) PB0          		14   	 				15   PB1 (OC1A)
*/

#ifndef BASE_H_
	#define BASE_H_

	#include <avr/io.h>                 //definicje adresów portów i bitów
	#include <stdint.h>					//Definicje typów np. int16_t, in8_t
	#include <util/delay.h>            //tylko funkcje _delay_ms i _delay_us
	#include <stdlib.h>					//Np. itoa,ltoa,atoi,malloc,free,realloc
	#include <stdbool.h> 				//_Bool jest bez tego, ale true i false nie. Tam jest te¿ "bool"
	#include <string.h>					//memcpy, memmov,memset...

	//Makra do obs³ugi portów - obs³uga wielu pinów na raz:
	#define PORT(x)				_PORT(x)			//Do ustawianie napiêcia pinu.
	#define _PORT(x)			(PORT##x)
	#define PIN(x)					_PIN(x)			//Do odczytu stanu pinu.
	#define _PIN(x)				(PIN##x)
	#define DDR(x)				_DDR(x)				//Do ustawiania kierunku  dzia³ania portu,    1-wyjœcie     Definicje PB0 itd s¹ np.w pliku IOm8.h
	#define _DDR(x)				(DDR##x)			//Musz¹ byæ dwustopniowe bo  operator ## nie rozwija argumentu.

	#define PDIR(x)				_DDR(x)				//Kierunek pinu.  x oznacze nie portu - np. B, 1-wyjœcie, 0-wejœcie
	#define POUT(x)				_PORT(x)			//Ustawianie stanu pinu.

	//Makra do obs³ugi portów ( tylko jeden pin na raz mo¿na przestawiæ):
	#define STATE(p,n)		_STATE(p,n)							//Odczyt stanu pinu n w porcie p.
	#define _STATE(p,n)		(PIN##p & (1<<n))				//Daje wartoœæ 0 gdy pin=0, oraz wartoœæ ró¿n¹ od zera (np. 4), gdy pin=1;

	#define DIRIN(p,n)			_DIRIN(p,n)							//Ustawia pin n w porcie p na wejœcie.
	#define _DIRIN(p,n)		(DDR##p &=~(1<<n))

	#define DIROUT(p,n)		_DIROUT(p,n)						//Ustawia pin n w porcie p na wyjœcie.
	#define _DIROUT(p,n)	(DDR##p |=(1<<n))

	#define SET(p,n)				_SET(p,n)							//Ustawia lub podci¹ga pin n w porcie p
	#define _SET(p,n)			(PORT##p |=(1<<n))

	#define CLR(p,n)			_CLR(p,n)								//Zeruje lub likwiduje podci¹ganie pinu n w porcie p
	#define _CLR(p,n)			(PORT##p &= ~(1<<n))

	#define CHNG(p,n)			_CHNG(p,n)							//Zmienia stan pinu n w porcie p
	#define _CHNG(p,n)		(PORT##p ^=(1<<n))

	//#define SETL(p,n,l) 		_SETL(p,n,l)							//Ustawia pin n na poziom L=0 lub L=1									No sense!
	//#define _SETL(p,n,l)		{if(l) {PORT##p |=(1<<n);} else {PORT##p &= ~(1<<n);}} //Uwaga! wstawia if.			No sense!

	#define TOSTRING(x)		STRINGIFY(x)						//Rozwija makro i zamienia na string constant przed kompilacj¹.
	#define STRINGIFY(x)	#x

	//Makra do obs³ugi portów, rejestrów i zmiennych (tylko jeden pin na raz mo¿na przestawiæ):
	#define CLRB(r,n)			(r &= ~(1<<n))		//Zerowanie bitu n w zmiennej r np CLRB(PORTB,0);
	#define SETB(r,n)			(r |=(1<<n))			//Ustawianie bitu n w zmiennej r
	#define CHNGB(r,n)		(r ^=(1<<n))			//Zmiana stanu bitu n w zmiennej r
	#define STATEB(r,n)		(r & (1<<n))			//Stan bitu n w zmiennej r
//#define bit_is_set(sfr, bit)		(_SFR_BYTE(sfr) & _BV(bit))		Test whether bit \c bit in IO register \c sfr is set.		<sfr_defs.h>
//#define bit_is_clear(sfr, bit)	(!(_SFR_BYTE(sfr) & _BV(bit)))  Sposób wywo³ania jest taki:  bit_is_clear(PINB,1);
//	#define bit_set(sfr, bit)			_SFR_BYTE(sfr)|=(1<<bit)		//Sposób wywo³ania jest taki: bit_set(PORTB,0);
//	#define bit_clr(sfr, bit)			_SFR_BYTE(sfr)&=~(1<<bit) Czym to siê ró¿ni od CLRB(r,n) ?


	#define BS(x)					(1<<x)								//Bit shift.

	#define MAX(x,y)			(((x)>(y)) ? (x):(y))
	#define MIN(x,y)			(((x)>(y)) ? (y):(x))

	#define LO(x)				(x&0xF)		//Czyli tylko dla jednobajtowej zmiennej.
	#define HI(x)					(x>>4)


	#define ON						1
	#define OFF					0

	#define OK						1
	#define BAD					0

	#define READY				1
	#define BUSY					0

	#define NOP					asm volatile("nop \n")
	#define NOT					~
	#define OR						|
	#define AND					&

	#define RUN					for(;;)
	#define ADR(x)				&x			//Pobiera adres iksa
	#define VALFROM(x)		*x				//Zmienna spod adresu x

	//Nazwy bajtów steruj¹cych ASCII (0 do 31):
	//ASCII control character or non-printing character.

	#define CCNUL		00	//Null
	#define CCSTX		01	//Start of header
	#define CCSOT		02	//Start of text
	#define CCETX		03	//End of text							 ^C  -  to interrupt the running process
	#define CCEOT		04	//End of transmission			^D
	#define CCENQ		05	//Enquiry								^E		Zapytanie
	#define CCACK		06	//Acknowledge					^F			Potwierdzenie
	#define CCBEL		07	//Bell									^G
	#define CCBS			08	//BackSpace						^H
	#define CCHT			09	//Horizontal tabulation			^I
	#define CCLF			10	//Line feed							^J
	#define CCVT			11	//Vertical tabulation				^K
	#define CCFF			12	//Form feed							^L  cause a printer to eject paper to the top of the next page, or a video terminal to clear the screen.
	#define CCCR			13	//Carriage return					^M
	#define CCSO			14	//Shift out
	#define CCSI			15	//Shift in
	#define CCDLE		16	//DatalLink escape
	#define CCDC1		17	//Device control 1 (XON)			Pause transmission
	#define CCDC2		18	//Device control 2
	#define CCDC3		19	//Device control 3 (XOFF)		Resume transmission
	#define CCDC4		20	//Device control 4
	#define CCNAK		21	//Negative acknowledge
	#define CCSYN		22	//Synchronous idle
	#define CCETB		23	//End of transmission block
	#define CCCAN		24	//Cancel
	#define CCEM		25	//End of medium
	#define CCSUB		26	//Substitute								^Z. Acts as an end-of-file for the Windows text-mode file i/o
	#define CCESC		27	//Escape									^[. Introduces an escape sequence.
	#define CCFS			28	//File separator
	#define CCGS			29	//Group separator
	#define CCRS			30	//Record separator
	#define CCUS			31	//Unit separator
	#define CCSPACE	32	//
	#define CCZERO 	'0'		//0x30=48			//Numer znaku zero w tablicy znaków ascii
	#define CCA	 		'A'		//0x41=65			//Numer znaku A w tablicy znaków ascii
	#define	CCDEL		127

	typedef		uint8_t		u08t;
	typedef		int8_t		s08t;
	typedef		uint16_t	u16t;
	typedef		int16_t		s16t;
	typedef		unsigned char uchar;

	typedef struct
	{
		char a0:1;
		char a1:1;
		char a2:1;
		char a3:1;
		char a4:1;
		char a5:1;
		char a6:1;
		char a7:1;
	} TBits;

	typedef struct
	{
		char H:4;
		char L:4;
	} TBCD;

/*	Ta zmienna jest rejestrem, czyli mo¿e siê zmieniæ spoza programu, dlatego musi byæ volatile.
 * Trzeba jej nadaæ wartoœæ w pliku main: SignalPortAdr=&PORTB;
 * Czyli port jest zmienn¹ pod okreœlonym na sta³e adresem. Pobieramy ten adres, a potem wp³ywamy na zmienn¹ przez ten adres.
 */
	volatile extern u08t	*SignalPortAdr;
	extern u08t	SignalLine;

	/*Dwustopniowe makro jest potrzebne tylko wtedy, gdy argument P trzeba rozwin¹æ - operatory # i ## nie rozwijaj¹ argumentu,
	 * wiêc potrzebne jest wstêpne makro tylko dla rozwiniêcia argumentu.
	 */
	#define		INIT_SIGNAL(P,L)		_INIT_SIGNAL(P,L)		//Teraz g³ówne makro dostanie rozwiniête ju¿ argumenty.
	#define		_INIT_SIGNAL(P,L) 	{SignalLine=L;SignalPortAdr=&PORT##P;DDR##P|=(1<<L);}
	#define		SIGNAL_ON				{*SignalPortAdr|=(1<<SignalLine);}		//Ustawia DDR na wyjœcie i podnosi.
	#define		SIGNAL_OFF				{*SignalPortAdr &=~(1<<SignalLine);}											//Opuszcza
	#define		SIGNAL_BLINK(T)			{SIGNAL_ON;	_delay_ms(T);SIGNAL_OFF}		//Zapala diodê na  czas T ms.

	u08t 			BitNum				(u08t Val);						//Pozycja bitu w liczbie w której jest jeden bit ustawiony, albo pozycja najwy¿szego bitu.
	u08t				Reverse				(u08t b);							//Odwraca kolejnoœæ bitów.
	u08t				DecToBcd			(u08t Val);						//Konwersja liczby (0 do 99) na BCD
	u08t				BcdToDec			(u08t Bcd);						// konwersja liczby BCD na dziesiêtn¹
	void				BcdToStr				(u08t Bcd, uchar *str);		//konwersja na tekst (2 litery)
	u08t				StrToBcd				(uchar *str);						//konwersja dwóch liter na bcd
	uchar			ByteToChar			(u08t Val);						//Zamienia cyfrê na odp. jej znak ASCII
	void				MemClr				(void *ptr, u08t Size);
	void				SignalProc			(u08t val);						//Ta procedura zostanie wykonana w razie napotkania b³êdu. Val to numer b³êdu. 170 bajtów.
#endif /* BASE_H_ */
