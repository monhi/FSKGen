#pragma once
#include "General.h"

#define M_PI				3.14159265
#define ZERO_FREQ			2200.0
#define ZERO_RAD			(2.0*M_PI*ZERO_FREQ)
#define ONE_FREQ			1200.0
#define ONE_RAD				(2.0*M_PI*ONE_FREQ)
#define BITRATE				1200
#define FSK_BIT_ARRAY_SIZE	4096

class CFSKBitStrGen
{
public:
	CFSKBitStrGen(void);
public:
	~CFSKBitStrGen(void);
	void		setSampleRate(uint32 s			);
	void		setMode		 (uint8 m			);
	void		setName		 (uint8* n			);
	void		setDate		 (uint8 m, uint8 d	);
	void		setTime		 (uint8 h, uint8 m	);
	void		setTelNum	 (uint8* tN			);
	int			Generate	 (					);
	int			GetData		 (uint16* data		);
	uint32		GetBufferSize(void);
private:
	uint16		fsk_read_position;
	uint16		fsk_write_position;
	uint32		sampleRate;
	int16		*shortBuf;
	uint8		fskData[FSK_BIT_ARRAY_SIZE];
	uint16		dataLen;
	uint8		mode;
	uint8		name[256];
	uint8		month;
	uint8		day;
	uint8		hour;
	uint8		minute;
	uint8		telNum[256];
	uint32		bufferLen;
	void		ResetFSKArray();
	void		AddFSKDataByte(uint8 by);
	void		AddFSKByte(uint8 by );
	uint8		GetFSKBit();
	int16		GetFSKBitArraySize();
	void		AddFSKBit(uint8 b);
	uint8		crc;
	uint8		GetCRC();
	void		ResetFSKCRC();
};