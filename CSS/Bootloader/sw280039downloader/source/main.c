#include "main.h"

Uint16 Buffer[BUFF_SIZE];
Uint32 cnt;
Uint16 *p,*end,*start,*eredeticim;
Uint32 cim;

main()
{
	Uint16 j,Length,length;

	DeviceInit();

 	//Program blokk letoltese (m�g csak a RAM-ba)
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

				Buffer[j++] = RxDAT & 0x00ff;	//adatok bufferbe t�lt�se
				if (j==3)
				{
					Length = Buffer[0] ;		  //hossz kiolvas�sa
					Length += Buffer[1] << 8;	//Length word-�kben megadva
					length = (Length*2)+4;		//length = adatbyte-ok sz�ma + 4 cim byte
				}

				length--;
			}

		}		//let�lt�d�tt egy blokk

		//A blokk hely�nek meghat�roz�sa
		cim = Buffer[3];
		cim <<=8;
		cim += Buffer[2];
		cim <<=8;
		cim += Buffer[5];
		cim <<=8;
		cim += Buffer[4];

		//A MemCopy sz�m�ra sz�ks�ges param�terek meghat�roz�sa
		//a blokk els� adat�nak c�me (Buffer[6] helye a mem-ben)
		start=(Uint16 *)&Buffer+6;
		//A blokk utols� adat�nak c�me (Buffer[6]+Length helye a mem-ben)
		end=(Uint16 *)&Buffer+Length*2+6;
		//A lek�ld�tt blokk helye a mem-ben (hova kell m�solni!)
		p=(Uint16 *)cim;

		//A legels� blokk c�m�nek elment�se
		if (cnt == 0)
		{
			cnt++;
			//eredeticim = az �tm�sol�st k�vet� programv�grehajt�si c�m
			eredeticim = p;
		}

		//Length=0 jelzi, hogy nincs t�bb blokk, ilyenkor nincs m�sol�s
		if (Length != 0) MemCpy(start,end,p);


		//nyugt�z�s, minden lek�ld�tt blokk ut�n (Length=0 esetben is)
		while (!TxRDY) ;

		TxDAT = 0x0000;

	} while (Length !=0);	//Az utols� blokk is lej�tt

	while (!TxEMPTY) ;

	p = eredeticim;	//A DP �rt�k�t �llitja be a k�vetkez� utas�t�shoz!!

	asm(" MOVL XAR7,@_eredeticim ");
	asm(" LB *XAR7");	//Ugrik az eredeticim �ltal mutatott helyre
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
