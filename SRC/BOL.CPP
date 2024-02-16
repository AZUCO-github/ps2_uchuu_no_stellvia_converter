/*
	似非アルファ付きBMPオーバーレイ

	RGBA
		A	0x80	不透過
			0x00	　透過
*/
#include	<windows.h>
#include	<stdio.h>

extern	HANDLE				NonOverWriteCreateFile(LPCSTR);

		void	overlay_bmp(unsigned char *,char *);

void	overlay_bmp(unsigned char *ap,char *an)
{	HANDLE				bh;
	HANDLE				ch;
	BITMAPFILEHEADER	*tf;
	BITMAPINFOHEADER	*ti;

	unsigned char		*bp;
	char				*tp;

	unsigned long		az;
	unsigned long		bz;

	unsigned long		tul1;

	char				bn[MAX_PATH];
	char				cn[MAX_PATH];
	char				tn[MAX_PATH];

	char				af[MAX_PATH];
	char				bf[MAX_PATH];

	char				drive[MAX_PATH];
	char				dir[MAX_PATH];
	char				fname[MAX_PATH];
	char				ext[MAX_PATH];



	strcpy(tn,GetCommandLine());
	strcpy(an,strrchr(tn,' ')+1);
	tp=strrchr(an,'"');	if(tp!=NULL)	*tp=0;
	tp=strrchr(an,'>');	if(tp!=NULL)	*tp=0;
	_splitpath(an,drive,dir,fname,ext);
	strcpy(af,fname);

	strcpy(tn,GetCommandLine());
	tp=strstr(tn,"/b")+2;
	if(tp==(char *)2){
		printf("\tError\tcan't found add bmp file\n");
		exit(-1);	}
	*strrchr(tp,' ')=0;
	strcpy(bn,tp);

	_splitpath(bn,drive,dir,fname,ext);
	if(	(strcmp(ext,".bmp")==0)	&&
		(strcmp(ext,".BMP")==0)		){
		printf("\tError\tcan't found add bmp file\n");
		exit(-1);	}
	strcpy(bf,fname);

	bh=CreateFile(bn,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(bh==(HANDLE)-1){
		printf("\tError\tcan't open add bmp file\n");
		exit(-1);	}

	bz=GetFileSize(bh,NULL);
	bp=new unsigned char[bz];
	ReadFile(bh,bp,bz,&tul1,NULL);
	CloseHandle(bh);



	tf=(BITMAPFILEHEADER *)ap;
	ti=(BITMAPINFOHEADER *)(ap+sizeof(BITMAPFILEHEADER));

	sprintf(cn,"%s-%s.bmp",af,bf);
	ch=NonOverWriteCreateFile((LPCSTR)cn);
	if(ch==(HANDLE)-1){
		printf("\tError\tcan't create bmp file\n");
		exit(-1);	}

	WriteFile(ch,tf,sizeof(BITMAPFILEHEADER),&tul1,NULL);
	WriteFile(ch,ti,sizeof(BITMAPINFOHEADER),&tul1,NULL);

	tp=(char *)bp;
	ap+=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bp+=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	az=(ti->biWidth)*(ti->biHeight)*4;
	for(bz=0;bz<az;bz+=4){
		tul1=	*(unsigned long *)(bp+bz);
		if(tul1&0x80000000)
				*(unsigned long *)(ap+bz)=tul1;
	}

	WriteFile(ch,ap,az,&tul1,NULL);
	CloseHandle(ch);

	if(tp!=0)	delete[]	tp;
}
