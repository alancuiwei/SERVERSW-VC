#include "CRTMarketData.h"
#include <string.h>
////////////////////////////////////////////////////////////////////////
// Name:       CRTMarketData::CRTMarketData(std::string constractname)
// Purpose:    Implementation of CRTMarketData::CRTMarketData()
// Parameters:
// - constractname
// Return:     void
////////////////////////////////////////////////////////////////////////

CRTMarketData::CRTMarketData(std::string name)
{
   strcpy(this->constractname, name.c_str());
   //std::cout<<"constractname:"<<constractname<<"实时行情数据新建"<<std::endl;
}
