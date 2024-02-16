#include	<windows.h>
#include	<stdio.h>
#include	"le.h"

extern	HANDLE	NonOverWriteCreateFile(LPCSTR);
extern	void	decode_adpcm(unsigned char *,short *,long *,long *);
		void	decode_bin(unsigned char *);

typedef	struct	{
	char	hriff[4];
	long	totalsize;

	char	hwave[8];
	long	hdrsize;
	short	format;
	short	channel;
	long	freq;
	long	byte_per_sec;
	short	blocksize;
	short	bits;

	char	hdata[4];
	long	datasize;
}	WAVHDR;



void	decode_bin(unsigned char *src,char *name)
{
	char			tfn[0x100];
	unsigned long	tul1=0;
	unsigned long	s,w;
	unsigned long	sample_cnt=(*(unsigned long *)(src+0x0c)-0x30)>>4;
	unsigned long	sample_max=sample_cnt*14*2;
	unsigned char	*wav;
	long			ppl,pl,ppr,pr;
	WAVHDR			wavhdr;
	HANDLE			hwav;

	if(*(unsigned long *)(src+0x0c)==0)	return;		//header only

		sprintf(wavhdr.hriff,	"RIFF");
		sprintf(wavhdr.hwave,	"WAVEfmt ");
		sprintf(wavhdr.hdata,	"data");
				wavhdr.hdrsize		=0x10;
				wavhdr.format		=1;		//PCM
				wavhdr.bits			=16;

				wavhdr.channel		=*(src+0x7e8);
				wavhdr.freq			=*(unsigned long *)(src+0x7e4);
				wavhdr.blocksize	=wavhdr.channel<<1;
				wavhdr.byte_per_sec =wavhdr.freq*wavhdr.blocksize;
				wavhdr.datasize		=wavhdr.blocksize*sample_max;
				wavhdr.totalsize	=wavhdr.datasize+sizeof(wavhdr)-8;

	wav=new unsigned char[(sample_max+0x20)*wavhdr.channel*2];

	sprintf(tfn,"%s.wav",name);
	hwav=NonOverWriteCreateFile((LPCSTR)tfn);
	if(hwav==(HANDLE)-1){
		printf("\tError\tcan't create wav file\n");
		exit(-1);									}
	WriteFile(hwav,&wavhdr,sizeof(WAVHDR),&tul1,NULL);

	s=0x810;
	w=0;

//	switch(wavhdr.channel){
//		case	1:
			ppl=0;	pl=0;
			while(sample_cnt) {
				decode_adpcm(src+s,(short *)(wav+w),&ppl,&pl);
				s+=16;	w+=14*2*2;
				sample_cnt--;											}
//			break;
/*		case	2:
			while(sample_cnt) {
				convert(buf,		src+p,		prev);
				convert(buf+0x20,	src+p+18,	prev+1);
				for(tul1=0;tul1<32;tul1++){
					*(short *)(wav+q+tul1*4)	=buf[tul1];
					*(short *)(wav+q+tul1*4+2)	=buf[tul1+0x20];	}
				p+=36;	q+=128;
				if(size>32)	wsize=32;
					else	wsize=size;
				size-=wsize;										}
			break;
	}
*/

	WriteFile(hwav,wav,sample_max*2*wavhdr.channel,&tul1,NULL);
	CloseHandle(hwav);
	delete[]	wav;
}



