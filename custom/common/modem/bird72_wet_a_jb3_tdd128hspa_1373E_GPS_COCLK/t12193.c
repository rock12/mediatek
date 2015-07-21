
#define _T12193_C_

#include "kal_public_api.h"
#include "t12193.h"

kal_uint16 const NVRAM_EF_AST_TL1_AFC_DATA_DEFAULT[]= 
{
    4500,   
#ifdef  MT6166_RF
	  460   //need check!
#else
	  538
#endif 

};

kal_int16 const NVRAM_EF_AST_TL1_PATHLOSS_33_35_37_39_DEFAULT[]= 
{

   9420,9440,9460,9480,9500,9520,9540,9560,9580,9601,0,0,0,0,0,
   12,12,12,12,12,12,12,12,12,12,12,12,12,12,12, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 

};

kal_int16 const NVRAM_EF_AST_TL1_PATHLOSS_34_DEFAULT[]= 
{
   10070,10087,10104,10122,0,0,0,0,0,0,0,0,0,0,0,
   20,20,20,20,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0    
};


kal_int16 const NVRAM_EF_AST_TL1_PATHLOSS_40_DEFAULT[]= 
{

   11600,11700,11800,11900,0,0,0,0,0,0,0,0,0,0,0,
   24,24,24,24,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0    

};


kal_int16 const NVRAM_EF_AST_TL1_TXDAC_33_35_37_39_DEFAULT[]= 
{
#ifdef  MT6166_RF
   8,0,0,0, 8,0,0,0, 0,0,0,0,//need check
#else
   7,0,0,0, 8,0,0,0, 0,0,0,0,
#endif
   515,0,0,0, 252,0,0,0, 0,0,0,0,
   9420,9440,9460,9480,9500,9520,9540,9560,9580,9601,0,0,0,0,0,
   -4,-5,-5,0,-2,0,0,3,6,12,0,0,0,0,0, 5,-5,3,0,-2,1,1,4,6,10,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
   14,14,84,84,
   32,0

};

kal_int16 const NVRAM_EF_AST_TL1_TXDAC_34_DEFAULT[]= 
{
#ifdef  MT6166_RF
   8,0,0,0, 8,0,0,0, 0,0,0,0,//need check
#else
   7,0,0,0, 8,0,0,0, 0,0,0,0,
#endif
   431,0,0,0, 235,0,0,0, 0,0,0,0,
   10070,10087,10104,10122,0,0,0,0,0,0,0,0,0,0,0,
   1,0,2,1,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,4,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 
   14,14,84,84,
   50,0 

};

kal_int16 const NVRAM_EF_AST_TL1_TXDAC_40_DEFAULT[]= 
{
#ifdef  MT6166_RF
   1,0,0,0, 2,0,0,0, 0,0,0,0,//need check
#else
   1,0,0,0, 2,0,0,0, 0,0,0,0,
#endif
   800,0,0,0, 140,0,0,0, 0,0,0,0,
   11733,11750,11767,11780,0,0,0,0,0,0,0,0,0,0,0,
   1,0,2,1,0,0,0,0,0,0,0,0,0,0,0, 0,0,1,4,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 
   12,12,24,24,
   0,0 

};

kal_int16 const NVRAM_EF_AST_TL1_ABB_CAL_DEFAULT[]= 
{
    1, /// first boot-up
    0, 0, 0, 0,
    0, 0, 
    0x0040, 0x0050, 
    0x0, 0, 0, 0, 
    0, 0, 0
};

kal_uint32 const NVRAM_EF_AST_TL1_CAP_DATA_DEFAULT[]= 
{
#ifdef  MT6166_RF
	149//need check
#else
	170
#endif
};



