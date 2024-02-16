short	comp_adpcm(	char	v,
					long	*pp,
					long	*p,
					int		s,
					int		i	)
{	long	t;
	int		f0[5]={0x0000,0x00F0, 0x01CC, 0x0188, 0x01E8};	//filter table
	int		f1[5]={0x0000,0x0000,-0x00D0,-0x00DC,-0x00F0};

	if(v>=8)	v-=16;										//4bit sign extend
	t=(v<<s)+((*p*f0[i]+*pp*f1[i])>>8);
	if(t> 0x7fffL)	t= 0x7fffL;								//cliping
	if(t<-0x8000L)	t=-0x8000L;
	*pp=*p;	*p=t;											//val swap & write
	return((short)t);
}


void decode_adpcm(	unsigned char	*src,
					short			*wav,
					long			*pp,
					long			*p		)
{	int	s=0x0C-(src[0x0]&0x0f);
	int	i=src[0x0]>>4;
	int	j;

	for(j=0;j<14;j++){										//0x10 type
		wav[j*2]	=comp_adpcm((char)(src[j+2]&0x0f),pp,p,s,i);
		wav[j*2+1]	=comp_adpcm((char)(src[j+2]>>4)  ,pp,p,s,i);
	}
}
