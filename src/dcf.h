#ifndef _DCF_H_
#define _DCF_H_

//typedefs
typedef enum
{
  eDCF_OK,
  eDCF_TIME_SET,
  eDCF_ALL_DONE,
  eDCF_ERROR
}TE_DCF_RC;

typedef enum
{
  eDCF_GET_INITIAL_TIME,
  eDCF_UPDATE_TIME
}TE_DCF_ACTION;

//functions

//add a bit of DCF data
//eDCF_GET_TIME is used for setting the initial time
//eDCF_UPDATE_TIME is used to update the time
//falling or rising edge (u8Edge) occured after u16After milliseconds
//when a time has been calculated set pu24Time accordingly
TE_DCF_RC eDCFAddBit(TE_DCF_ACTION eAction, U8 u8Edge, U16 u16After, U24 *pu24Time);

#endif