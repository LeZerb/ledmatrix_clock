#ifndef _DCF_H_
#define _DCF_H_

#include "date.h"

//typedefs

typedef enum {
    eDCF_OK,
    eDCF_TIME_SET,
    eDCF_ERROR
} TE_DCF_RC;

//functions

//add a bit of DCF data
//falling or rising edge (u8Edge) occured after u16After milliseconds
//when a time has been calculated set pstTime and pstDate accordingly
TE_DCF_RC eDCFAddBit(U8 u8Edge, U16 u16After, TS_TIME *pstTime, TS_DATE *pstDate);

#endif
