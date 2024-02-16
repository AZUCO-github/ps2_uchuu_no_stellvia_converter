///////////////////////////////////////////////////////////////////////////////
//	PS2 Stellvia converter
//		DAT format
//			050325
//				by	AZUCO
///////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<stdio.h>

extern	void	decode_bin_(HANDLE,unsigned long,unsigned long,char *);



/*****メモ*****DAT フォーマット************************************************
長さ0x40固定
0x??+0x00	アイテムのアドレス/0x800(0x800alignに揃っている)
0x??+0x04	アイテムの長さ
0x??+0x08	アイテムのフォルダ付き名前
******************************************************************************/



void	decode_dat(HANDLE hndl)
{
	char			ITB[0x40];	//Item Table Buffer
	unsigned long	INO;		//Item NO.
	unsigned long	IDA;		//Item Data Address
	unsigned long	ILN;		//Item LeNgth
	unsigned long	tmp;

	for(INO=0;;INO++){
		SetFilePointer(hndl,INO*0x40,0,FILE_BEGIN);
		ReadFile(hndl,ITB,0x40,&tmp,NULL);
		IDA=*(unsigned long*)(ITB)*0x800;
		ILN=*(unsigned long*)(ITB+4);

		if(IDA==0)	break;

printf("No.%04X %08X %08X %s\n",INO,IDA,ILN,ITB+8);
		decode_bin_(hndl,IDA,ILN,ITB+8);
		goto CONV_DONE;

CONV_DONE:
		NULL;
	}
}
