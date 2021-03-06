/* 6-and-2 code, used in Apple 16-sector data fields */

static unsigned char dec_6and2_tbl[]={
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 00-0f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 10-1f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 20-2f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 30-3f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 40-4f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 50-5f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 60-6f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 70-7f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff, /* 80-8f invalid */
0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x04,0xff,0xff,0x08,0x0c,0xff,0x10,0x14,0x18, /* 90-9f */
0xff,0xff,0xff,0xff,0xff,0xff,0x1c,0x20,0xff,0xff,0xff,0x24,0x28,0x2c,0x30,0x34, /* a0-af */
0xff,0xff,0x38,0x3c,0x40,0x44,0x48,0x4c,0xff,0x50,0x54,0x58,0x5c,0x60,0x64,0x68, /* b0-bf */
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6c,0xff,0x70,0x74,0x78, /* c0-cf */
0xff,0xff,0xff,0x7c,0xff,0xff,0x80,0x84,0xff,0x88,0x8c,0x90,0x94,0x98,0x9c,0xa0, /* d0-df */
0xff,0xff,0xff,0xff,0xff,0xa4,0xa8,0xac,0xff,0xb0,0xb4,0xb8,0xbc,0xc0,0xc4,0xc8, /* e0-ef */
0xff,0xff,0xcc,0xd0,0xd4,0xd8,0xdc,0xe0,0xff,0xe4,0xe8,0xec,0xf0,0xf4,0xf8,0xfc /* f0-ff */
};

int dec_6and2(unsigned char *out, unsigned char *in, int count) {
	int status=0;
	int i;
	unsigned char acc=0;
	unsigned char c,d;
	unsigned char csum;
	unsigned char aux[86];

	if(count!=256)
		return 1;

	for(i=0;i<86;i++) {
		aux[i]=dec_6and2_tbl[*in];
			if(aux[i]==0xff)
				status=1; /* 0xff = invalid gcr nibble */
		in++;
	}
	for(i=0;i<256;i++) {
		out[i]=dec_6and2_tbl[*in];
			if(out[i]==0xff)
				status=1;
		in++;
	}
	csum=dec_6and2_tbl[*in];

	if(in[1]!=0xde || in[2]!=0xaa) /* epilogue */
		status=1;

	for(i=0;i<86;i++) {
		aux[i]^=acc;
		acc=aux[i];
	}
	for(i=0;i<256;i++) {
		out[i]^=acc;
		acc=out[i];
	}

	if(out[255]!=csum)
		status=1;

	for(i=0;i<86;i++) {
		c=aux[i];
		c>>=2;
		d=0; if(c&1) d|=2; if(c&2) d|=1;
		out[i]|=d;
		c>>=2;
		d=0; if(c&1) d|=2; if(c&2) d|=1;
		out[i+86]|=d;
		c>>=2;
		d=0; if(c&1) d|=2; if(c&2) d|=1;
		if(i+172<256)
			out[i+172]|=d;
	}

	return status;
}
