///////////////////////////////////////////////////////////////////////////////
//	PS2 Stellvia converter
//		050325
//			by	AZUCO
///////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<stdio.h>

extern	void	decode_dat(HANDLE);
extern	void	decode_tips(unsigned char *,char *);
extern	void	overlay_bmp(unsigned char *,char *);
extern	void	decode_bin(unsigned char *,char *);
extern	void	decode_bd(unsigned char *,char *,unsigned long);



void	main(void)
{	char			file[_MAX_PATH];
	char			*tfpa;
	char			*tfpb;
	HANDLE			hndl;
	unsigned char	*buff=0;
	unsigned long	size;
	char			drive[MAX_PATH];
	char			dir[MAX_PATH];
	char			fname[MAX_PATH];
	char			ext[MAX_PATH];
	unsigned long	tmp;


	printf("PS2 Stellvia Converter\n");
	printf("\t\tProg. By AZUCO\n\n");



	strcpy(file,GetCommandLine());				//コマンドラインをコピーする
	tfpa=strrchr(file,' ');		//一番最後のスペース以降をファイル名と見なす
	tfpa++;

	tfpb=strrchr(tfpa,'"');		//ダブルクオートありの場合
	if(tfpb!=NULL)	*tfpb=0;

	tfpb=strrchr(tfpa,'>');		//リダイレクトの場合
	if(tfpb!=NULL)	*tfpb=0;



	tfpb=&file[0];
loop:
	if(*tfpa=='\\'){	*(tfpb+0)='\\';
						*(tfpb+1)='\\';
						tfpa++;
						tfpb+=2;
						goto loop;		}
	if(*tfpa>=0x20){	*tfpb=*tfpa;
						tfpa++;
						tfpb++;
						goto loop;		}
						*tfpb=0;


//strcpy(file,"E:\\tmp\\stellvia\\DATA1\\bd\\M_STBG21BK.BD");

	hndl=CreateFile(file,
					GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hndl==(HANDLE)-1){
		printf("Yayoi <data file>\n");
		printf("\tError\tsource file not found\n");
		exit(-1);	}

	_splitpath(file,drive,dir,fname,ext);
	if(strcmp(_strlwr(ext),".dat")==0)		decode_dat(hndl);
	else{
		size=GetFileSize(hndl,NULL);
		buff=new unsigned char[size];
		ReadFile(hndl,buff,size,&tmp,NULL);	}
	CloseHandle(hndl);

	if(	strcmp(_strlwr(ext),".tips")==0)	decode_tips(buff,file);
	if(	strcmp(_strlwr(ext),".tip")==0)		decode_tips(buff,file);
	if(	strcmp(_strlwr(ext),".bmp")==0)		overlay_bmp(buff,file);
	if(	strcmp(_strlwr(ext),".bin")==0)		decode_bin(buff,file);
	if(	strcmp(_strlwr(ext),".bd")==0)		decode_bd(buff,file,size);

	if(buff!=0)	delete[]	buff;
}
