#include "StdAfx.h"
#include "FSKBitStrGen.h"
#include "General.h"
#include "math.h"

CFSKBitStrGen::CFSKBitStrGen(void)
{
	sampleRate	= 8000					;
	shortBuf	= NULL					;
	mode		= MODE_MDMF				;
	name[0]		= 0						;
	month		= 1						;
	day			= 1						;
	hour		= 0						;
	minute		= 0						;
	telNum[0]	= 0						;
	ResetFSKArray()						;
}

CFSKBitStrGen::~CFSKBitStrGen(void)
{
	if(shortBuf)
		delete shortBuf;
	shortBuf = NULL;
}
void CFSKBitStrGen::setSampleRate(uint32 s)
{
	sampleRate = s;
}

void CFSKBitStrGen::setMode(uint8 m)
{
	mode		= m;
}

void	CFSKBitStrGen::setName	 (uint8* n)
{
	name[0] = 0;
	if( strlen((char*)n) < STRING_SIZE )
		strcpy((char*)name,(char*)n);
	else
		strcpy((char*)name,"MR LONG NAME");
}

void	CFSKBitStrGen::setDate	 (uint8 m, uint8 d)
{
	month = m ;
	day	  = d ;
}

void	CFSKBitStrGen::setTime	 (uint8 h, uint8 m)
{
	hour   = h;
	minute = m;
}

void	CFSKBitStrGen::setTelNum (uint8* tN)
{
	telNum[0] = 0 ;
	if( strlen((char*)tN) < STRING_SIZE )
	{
		strcpy((char*)telNum,(char*)tN);
	}
	else
	{
		strcpy((char*)telNum,"007");
	}
}

int		CFSKBitStrGen::Generate	()
{

	uint16  cur;
	uint32	i;
    int	    t = 0;
	int		count = 0, residual = 0;
    int		br;

    uint8	bit;
	int		bits = 0;
    double	x;
    double	freq, oldfreq;
    int		samples		= sampleRate;
    int		rate		= sampleRate;
    int		bitrate		= BITRATE;
    double	one_freq	= ONE_RAD;
    double	zero_freq	= ZERO_RAD;
    
	if(telNum[0]==0)
		return FAILURE;

	ResetFSKArray();
	dataLen = 0;
	for( i=0 ; i < 29; i++)
		AddFSKByte(0x55);
	for(i=29; i < 50; i++)
		AddFSKByte(0xFF);
	switch( mode )
	{
		case MODE_MDMF:
			// check telnum and 
			dataLen  = 10;
			dataLen += strlen((char*)telNum)+2;

			if( strlen((char*)name) > 0 )
				dataLen += strlen((char*)name)+2;
			ResetFSKCRC();
			AddFSKDataByte(CODE_MDMF);
			AddFSKDataByte(dataLen);
			AddFSKDataByte(CODE_MDMF_DATE_TIME);
			AddFSKDataByte(8);// date len;
			AddFSKDataByte(month/10 + 0x30);
			AddFSKDataByte(month%10 + 0x30);
			AddFSKDataByte(day/10   + 0x30);
			AddFSKDataByte(day%10   + 0x30);
			AddFSKDataByte(hour/10  + 0x30);
			AddFSKDataByte(hour%10  + 0x30);
			AddFSKDataByte(minute/10+ 0x30);			
			AddFSKDataByte(minute%10+ 0x30);

			AddFSKDataByte(CODE_MDMF_TELNUM);
			AddFSKDataByte((uint8)strlen((char*)telNum));
			for(i=0;i<strlen((char*)telNum);i++)
				AddFSKDataByte(telNum[i]);

			if( strlen((char*)name) )
			{
				AddFSKDataByte(CODE_MDMF_NAME);
				AddFSKDataByte((uint8)strlen((char*)name));
				for(i=0;i<strlen((char*)name);i++)
					AddFSKDataByte(name[i]);
			}
			// calculating the crc.
		break;
		case MODE_SDMF:
			dataLen  = 8;
			dataLen += strlen((char*)telNum);

			
			AddFSKDataByte(CODE_SDMF);
			AddFSKDataByte(dataLen  );

			AddFSKDataByte(month/10+ 0x30);
			AddFSKDataByte(month%10+ 0x30);
			AddFSKDataByte(day/10  + 0x30);
			AddFSKDataByte(day%10  + 0x30);
			AddFSKDataByte(hour/10 + 0x30);
			AddFSKDataByte(hour%10 + 0x30);
			AddFSKDataByte(minute/10+ 0x30);
			AddFSKDataByte(minute%10+ 0x30);

			for(i=0;i<strlen((char*)telNum);i++)
				AddFSKDataByte(telNum[i]);
		break;
	}
	AddFSKDataByte((uint8)(256-crc));
	if(shortBuf)
		delete []shortBuf;
	bufferLen = 5*sampleRate;
	shortBuf = new int16[5*sampleRate];
	for(i=0; i < sampleRate; i++)
		shortBuf[i]=0	;
	cur = sampleRate	;
// main LOOP.
    bit = GetFSKBit();
    if (bit)
       freq = one_freq	;
    else    
	   freq = zero_freq;

	for (i = 0; bits < GetFSKBitArraySize(); i++)
    {
       x = (((double)t/rate) * freq);
       shortBuf[cur++] = (8191 * sin(x));
       t++;
       count++;
       br = (rate / count) - residual;
       if (br <= bitrate)
       {
			residual = bitrate - br;
			count = 0;
			if (bit != GetFSKBit()) 
			{
				oldfreq = freq;
				if (bit == 0) 
				{
					bit = 1;
					freq = one_freq;
				}
				else 
				{
					bit = 0;
					freq = zero_freq;
				}
				double temp = 0.5 + ((double)t * (oldfreq / freq)) ;
				t = (int) temp;
			}
			bits++;
       }
    }
	for(i=cur; i <bufferLen; i++)
		shortBuf[i] = 0;
	//data is ready in short buffer.
	return SUCCESS;
}

int		CFSKBitStrGen::GetData		 (uint16* data)
{
	memcpy(data,shortBuf,bufferLen*2);
	return SUCCESS;
}

uint32 CFSKBitStrGen::GetBufferSize(void)
{
	return bufferLen*2;
}

void CFSKBitStrGen::ResetFSKArray()
{
	memset(fskData,0,FSK_BIT_ARRAY_SIZE);
	fsk_read_position  = 0;
	fsk_write_position = 0;
	ResetFSKCRC();
}

void CFSKBitStrGen::AddFSKBit(uint8 b)
{
	fskData[fsk_write_position++] = b ;
}

int16 CFSKBitStrGen::GetFSKBitArraySize()
{
	return fsk_write_position;
}

uint8 CFSKBitStrGen::GetFSKBit()
{
	return fskData[fsk_read_position++];
}
void  CFSKBitStrGen::AddFSKByte(uint8 by )
{
	for(int i=0 ; i<8 ; i++)
	{
		AddFSKBit((uint8)( (by&(1<<i))>>i   ));
	}
}
void CFSKBitStrGen::AddFSKDataByte(uint8 by)
{
	AddFSKBit(0);// start bit
	AddFSKByte(by);
	crc+= by;
	AddFSKBit(1);// stop  bit
//	AddFSKBit(1);// stop  bit
}

void CFSKBitStrGen::ResetFSKCRC()
{
	crc = 0;
}
uint8 CFSKBitStrGen::GetCRC()
{
	return crc;
}


