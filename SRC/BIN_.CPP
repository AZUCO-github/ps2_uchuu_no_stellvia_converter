#include	<windows.h>
#include	<stdio.h>

extern	HANDLE	NonOverWriteCreateFile(LPCSTR);

		void	decode_bin_(HANDLE,unsigned long,unsigned long,char *);



void	decode_bin_(HANDLE hndl,unsigned long addr,unsigned long length,char *name)
{
	char			buf[0x800];
	char			drive[MAX_PATH];
	char			dir[MAX_PATH];
	char			fname[MAX_PATH];
	char			ext[MAX_PATH];
	HANDLE			hbin;
	unsigned long	tmp;

	_splitpath(name,drive,dir,fname,ext);
	if(dir[0]!=NULL){
		strcpy(name,fname);
		strcat(name,ext);
	}

	hbin=NonOverWriteCreateFile((LPCSTR)name);
	if(hbin==(HANDLE)-1){
		printf("\tError\tcan't create bin file\n");
		exit(-1);	}

	SetFilePointer(hndl,addr,0,FILE_BEGIN);
	for(;;){
		if(length<0x800){
			ReadFile(hndl,buf,length,&tmp,NULL);
			WriteFile(hbin,buf,length,&tmp,NULL);
			break;
		}else{
			ReadFile(hndl,buf,0x800,&tmp,NULL);
			WriteFile(hbin,buf,0x800,&tmp,NULL);
			length-=0x800;
		}
	}
	CloseHandle(hbin);
}
