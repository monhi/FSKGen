#pragma once

#define		MAJOR			 1
#define		MINOR			 0

#define		MODE_MDMF		 0
#define		MODE_SDMF		 1

#define		SUCCESS			 0
#define		FAILURE			-1

#define		STRING_SIZE		 256

#define		CODE_MDMF		0x80
#define		CODE_SDMF	    0x04

#define		CODE_MDMF_DATE_TIME		1
#define		CODE_MDMF_TELNUM		2
#define		CODE_MDMF_NAME			7

typedef unsigned char 	uint8	;
typedef unsigned short	uint16	;
typedef unsigned int	uint32  ;
typedef short			int16	;

#define		NUMBER_PRESENT			0
#define		NUMBER_NOT_PRESENT		1

#define		NAME_PRESENT			0
#define		NAME_NOT_PRESENT		1
#define		NO_NAME					2




#define		SAMPLE_TYPE_8BIT_ULAW			7
//#define	SAMPLE_TYPE_8BIT_ALAW			1
#define		SAMPLE_TYPE_16BIT_PCM_INTEL		1
//#define		SAMPLE_TYPE_16BIT_PCM_MOTOROLA	4

