/* C:\Users\Samsung\workspace\Base\Base.c
 *  Project: Base
 *  Created on: 17 lut 2018  22:42:08
 *  Author: Borys Horoneskul
 *  boryshoroneskul@gmail.com		 */

#include "Base.h"
/*Brak volatile powodujê error "conflicting types"
 * */
volatile u08t *SignalPortAdr=NULL; 		//Jak jest NULL, to znaczy nie zosta³o zainicjowane
u08t SignalLine=255;

u08t BitNum(u08t Val)		//Pozycja bitu w liczbie w której jest jeden bit ustawiony, albo pozycja najwy¿szego bitu
{
	u08t Result=0;
	while (Val)
	{
		Val>>=1;
		++Result;
	};
	return Result;
};		//Zero oznacza brak bitu, jeden oznacza bit na pozycji zerowej czyli 0b00000001

u08t Reverse(u08t b)		//Odwraca kolejnoœæ bitów.
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;		//zamieni³ dwie czwórki
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;		//zamieni³ 4 dwójki
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;		//zamieni³ 8 jedynek
   return b;
};

//#define MSB2LSB(b) (((b)&1?128:0)|((b)&2?64:0)|((b)&4?32:0)|((b)&8?16:0)|((b)&16?8:0)|((b)&32?4:0)|((b)&64?2:0)|((b)&128?1:0))

u08t DecToBcd(u08t Val)		// Konwersja liczby na format BCD . Liczba 0 do 99, bo wiêksza siê w BCD nie mieœci.  Jak bêdzie 100, to wyjdzie 00
{
	return ((Val / 10)<<4) | (Val % 10);	// konwersja liczby dziesiêtnej na BCD = 4starsze bity - dziesi¹tki, 4m³odsze bity - jednostki
};

u08t BcdToDec(u08t Bcd)	// konwersja liczby BCD na dziesiêtn¹  //0 do 99
{
	return ((Bcd >> 4)  * 10) + (Bcd & 0x0F);
};

void BcdToStr(u08t Bcd, uchar *str)		//BcdToStr				(u08t Bcd, uchar* str)
{
	str[0]=(Bcd>>4)+CCZERO;
	str[1]=(Bcd & 0x0F)+CCZERO;
};

u08t StrToBcd (uchar* str)				//konwersja dwóch liter na bcd
{
	return ((str[0]-CCZERO)<<4)+(str[1]-CCZERO);
};

uchar ByteToChar(u08t Val)			//Zamienia liczbê na odp. jej znak ASCII
{
	return Val+CCZERO;
};

void MemClr (void *ptr, u08t len)
{
	memset(ptr,0,len);		//	while (len-- > 0) 		*((char*)dest+len) = 0;
};

void MemClr2(void *ptr, u08t len)  //Nie umiem tego jeszcze w asm napisaæ.
{

	__asm__ volatile(			//“code” : output operand list : input operand list : clobber list);
		"ldi r16, 0  \n"
		"sts (ptr), r16 \n");
	__asm__ volatile (
		"1: dec %0" "\n\t"
		"brne 1b" "\n\t"
		: "=r" (len)
		: "0" (len)
		:	);

};

void SignalProc (u08t val)				//Ta procedura zostanie wykonana np. w razie napotkania b³êdu. Val to numer b³êdu.
{
	if(SignalPortAdr)
		for(;;)
		{
			for (u08t var = 0; var < val; ++var)
			{//Pobranie wartoœci spod adresu lub nadanie wartoœci pod adresem.
				*SignalPortAdr|=(1<<SignalLine);	//== SETB(*SignalPortAdr,SignalLine);
				_delay_ms(200);
				*SignalPortAdr &=~(1<<SignalLine);
				_delay_ms(200);
			};
			_delay_ms(1000);
		}
};


