#include "main.h"

Uint16 Buffer[BUFF_SIZE];
Uint32 cnt;
Uint16 *p,*end,*start,*eredeticim;
Uint32 cim;

main()
{
	Uint16 j,Length,length;

	DeviceInit();

 	//Program blokk letoltese (még csak a RAM-ba)
 	Length = 0xff;
 	cnt=0;

	do
	{
		j=0;
		length=0xff;

		while(length != 0)
		{
			if (RxRDY)
			{

				Buffer[j++] = RxDAT & 0x00ff;	//adatok bufferbe töltése
				if (j==3)
				{
					Length = Buffer[0] ;		  //hossz kiolvasása
					Length += Buffer[1] << 8;	//Length word-ökben megadva
					length = (Length*2)+4;		//length = adatbyte-ok száma + 4 cim byte
				}

				length--;
			}

		}		//letöltõdött egy blokk

		//A blokk helyének meghatározása
		cim = Buffer[3];
		cim <<=8;
		cim += Buffer[2];
		cim <<=8;
		cim += Buffer[5];
		cim <<=8;
		cim += Buffer[4];

		//A MemCopy számára szükséges paraméterek meghatározása
		//a blokk elsõ adatának címe (Buffer[6] helye a mem-ben)
		start=(Uint16 *)&Buffer+6;
		//A blokk utolsó adatának címe (Buffer[6]+Length helye a mem-ben)
		end=(Uint16 *)&Buffer+Length*2+6;
		//A leköldött blokk helye a mem-ben (hova kell másolni!)
		p=(Uint16 *)cim;

		//A legelsõ blokk címének elmentése
		if (cnt == 0)
		{
			cnt++;
			//eredeticim = az átmásolást követõ programvégrehajtási cím
			eredeticim = p;
		}

		//Length=0 jelzi, hogy nincs több blokk, ilyenkor nincs másolás
		if (Length != 0) MemCpy(start,end,p);


		//nyugtázás, minden leküldött blokk után (Length=0 esetben is)
		while (!TxRDY) ;

		TxDAT = 0x0000;

	} while (Length !=0);	//Az utolsó blokk is lejött

	while (!TxEMPTY) ;

	p = eredeticim;	//A DP értékét állitja be a következõ utasításhoz!!

	asm(" MOVL XAR7,@_eredeticim ");
	asm(" LB *XAR7");	//Ugrik az eredeticim által mutatott helyre
}

void MemCpy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
	while(SourceAddr < SourceEndAddr)
	{
		*DestAddr = *SourceAddr++;
		(*DestAddr++) += (*SourceAddr++) <<8;//A Bufferben az adatok byte-ben vannak
	}
	return;
}
