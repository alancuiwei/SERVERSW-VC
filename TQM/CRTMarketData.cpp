/***********************************************************************
 * Module:  CRTMarketData.cpp
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:14:04
 * Purpose: Implementation of the class CRTMarketData
 ***********************************************************************/

#include "stdafx.h"
#include "CRTMarketData.h"

////////////////////////////////////////////////////////////////////////
// Name:       CRTMarketData::CRTMarketData(std::string constractname)
// Purpose:    Implementation of CRTMarketData::CRTMarketData()
// Parameters:
// - constractname
// Return:     void
////////////////////////////////////////////////////////////////////////

CRTMarketData::CRTMarketData(std::string constractname)
{
   this->constractname = constractname;
   //std::cout<<"constractname:"<<constractname<<"实时行情数据新建"<<std::endl;
}