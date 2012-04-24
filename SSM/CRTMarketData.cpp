/***********************************************************************
 * Module:  CRTMarketData.cpp
 * Author:  CUIWei
 * Modified: 2011��10��24������һ 22:14:04
 * Purpose: Implementation of the class CRTMarketData
 ***********************************************************************/

#include "stdafx.h"
#include "CContract.h"
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
   //this->constractname = constractname;
   strcpy(this->constractname, name.c_str());
   cout<<"constractname:"<<constractname<<"ʵʱ���������½�"<<endl;
}
