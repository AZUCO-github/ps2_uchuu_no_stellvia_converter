unsigned short	ler2(unsigned char *);
unsigned long	ler4(unsigned char *);
void	lew2(unsigned char *,unsigned short);
void	lew4(unsigned char *,unsigned long);

#pragma	warning(push,1)
__declspec(naked)
unsigned short	ler2(unsigned char *addr)
{	_asm{	mov			eax,dword ptr [esp+4]
			mov			ax,word ptr [eax]
			ror			ax,8
			ret									}	}

__declspec(naked)
unsigned long	ler4(unsigned char *addr)
{	_asm{	mov			eax,dword ptr [esp+4]
			mov			eax,dword ptr [eax]
			bswap		eax
			ret									}	}
#pragma	warning(pop)



/*
unsigned short	ler2(unsigned char *addr)
{	return(	(*(addr+0)<< 8)|
			(*(addr+1)<< 0)	);	}
unsigned long	ler4(unsigned char *addr)
{	return(	(*(addr+0)<<24)|
			(*(addr+1)<<16)|
			(*(addr+2)<< 8)|
			(*(addr+3)<< 0)	);	}
*/



__declspec(naked)
void	lew2(unsigned char *addr,unsigned short data)
{	_asm{
//		push		eax
//		push		edx
//		mov			eax,dword ptr [esp+16]
//		mov			edx,dword ptr [esp+12]
		mov			eax,dword ptr [esp+8]
		mov			edx,dword ptr [esp+4]
		ror			ax,8
		mov			word ptr [edx],ax
//		pop			edx
//		pop			eax
		ret
	}
}

__declspec(naked)
void	lew4(unsigned char *addr,unsigned long data)
{	_asm{
//		push		eax
//		push		edx
//		mov			eax,dword ptr [esp+16]
//		mov			edx,dword ptr [esp+12]
		mov			eax,dword ptr [esp+8]
		mov			edx,dword ptr [esp+4]
		bswap		eax
		mov			dword ptr [edx],eax
//		pop			edx
//		pop			eax
		ret
	}
}

