#include	<windows.h>
#include	<stdio.h>
#include	"le.h"

extern	HANDLE				NonOverWriteCreateFile(LPCSTR);

		bool	is_tips(unsigned char *,char *);
		void	decode_tips(unsigned char *,char *);

void	decode_tips(unsigned char *org,char *name)
{
	HANDLE				hbmp;
	BITMAPFILEHEADER	bf;
	BITMAPINFOHEADER	bi;
	unsigned char		*src;
	unsigned char		*dst;
	unsigned char		*tip;
	unsigned char		*tmp;
	unsigned long		sl,sx,sy,sp;
	unsigned long		dl,dx,dy,dp;
	unsigned long		tw,tx,ty,tz;
	unsigned long		   ux,uy;
			 long		   ox,oy;
	unsigned long		tn,tm,tc;

	char			tfn[0x100];
	char			drive[MAX_PATH];
	char			dir[MAX_PATH];
	char			fname[MAX_PATH];
	char			ext[MAX_PATH];

	if(*(unsigned long *)org==0x43a5b06d)	return;

	_splitpath(name,drive,dir,fname,ext);

	sl		=0x100;								//src側の定義
	sx		=*(unsigned short *)(org+0x10);
	sy		=*(unsigned short *)(org+0x12);
	ux		=*(unsigned char  *)(org+0x16);
	uy		=*(unsigned char  *)(org+0x17);
	src		=*(unsigned long  *)(org+0x0c)+org;

	dl		=ux*0x100;							//dst側の定義
	dx		=0;									//最大値を作業中にチェック
	dy		=0;
	dst		=new unsigned char[0x1000*0x1000*4];

	sp		=sx*sy*4;
	for(dp=0;dp<sp;dp+=4){						//BGRX > RGBX 変換
		tw			=ler4(src+dp+0);
		*(src+dp+0)	=(unsigned char)(tw>>8);
		*(src+dp+2)	=(unsigned char)(tw>>24);	}


	tn=1;
tip_start:
	if(*(unsigned long  *)(org+tn*0x40+0x00)!=0x40)		goto	tip_end;
	memset(dst,0,0x1000*0x1000*4);						//zero clear
	tm		=*(unsigned short *)(org+tn*0x40+0x14);		//tip data
	ox		=*(			short *)(org+tn*0x40+0x0c);		//offset
	oy		=*(			short *)(org+tn*0x40+0x0e);
	tip		=*(unsigned long  *)(org+tn*0x40+0x10)+org;

												//負のオフセットは想定外（汗
	if(ox<0)											goto	tip_pass;
	if(oy<0)											goto	tip_pass;

	for(tc=0;tc<tm;tc++){
		tmp=tip+tc*8;
		tz=*(tmp+4)*ux;

		tx=*(tmp+0)+*(tmp+4);					//コピー先の最大値のチェック
		ty=*(tmp+1)+1;
		if(dx<tx)	dx=tx;
		if(dy<ty)	dy=ty;

		for(ty=0;ty<uy;ty++){
			dp=dl*(	ty+	oy+	*(tmp+1)*uy)+
						ox+	*(tmp+0)*ux;
			sp=sl*(	ty+		*(tmp+3)*uy)+
							*(tmp+2)*ux;
			for(tx=0;tx<tz;tx++){
				tw=	*(unsigned long *)(src+4*(sp+tx));
				if(tw&0x80000000)
					*(unsigned long *)(dst+4*(dp+tx))=tw;
			}
		}
tip_pass:;
	}

	*((char *)&bf.bfType+0)='B';
	*((char *)&bf.bfType+1)='M';
	bf.bfSize=			sizeof(BITMAPFILEHEADER)+
						sizeof(BITMAPINFOHEADER)+
						dx*ux*dy*uy*4;							//image
	bf.bfOffBits=		sizeof(BITMAPFILEHEADER)+
						sizeof(BITMAPINFOHEADER);

	bi.biSize=			sizeof(BITMAPINFOHEADER);
	bi.biWidth=			dx*ux;
	bi.biHeight=		dy*uy;
	bi.biPlanes=		1;
	bi.biBitCount=		32;
	bi.biCompression=	0;
	bi.biSizeImage=		0;
	bi.biXPelsPerMeter=	0;
	bi.biYPelsPerMeter=	0;
	bi.biClrUsed=		0;
	bi.biClrImportant=	0;

	sprintf(tfn,"%s_%04X.bmp",fname,tn);
	hbmp=NonOverWriteCreateFile((LPCSTR)tfn);
	if(hbmp==(HANDLE)-1){
		printf("\tError\tcan't create bmp file\n");
		exit(-1);	}

	WriteFile(hbmp,&bf,sizeof(BITMAPFILEHEADER),&tw,NULL);
	WriteFile(hbmp,&bi,sizeof(BITMAPINFOHEADER),&tw,NULL);
	for(ty=dy*uy;ty>0;ty--)									//ひっくり返す
		WriteFile(hbmp,dst+dl*4*ty,dx*ux*4,&tw,NULL);
	CloseHandle(hbmp);

	tn++;	goto	tip_start;



tip_end:
	if(dst!=0)	delete[]	dst;
}



bool	is_tips(unsigned char *src,char *name)
{	return(TRUE);	}
