#include "StdAfx.h"
#include "SSMCreator.h"
#include "CSpreadCostPair.h"
#include "CRTMarketData.h"


CSSMCreator::CSSMCreator(void)
{
}


CSSMCreator::~CSSMCreator(void)
{
}

CUser* CSSMCreator::CreateUser(std::string username)
{
	if(username=="")
	{
		return NULL;
	}
	return new CUser(username); 
}

CPair* CSSMCreator::CreatePair(std::string firstconstractname, 
		std::string secondconstractname, std::string rightid)
{
	if((firstconstractname=="")||(secondconstractname=="")||(rightid==""))
	{
		return NULL;
	}
	CPair* ppair = NULL;
	if(rightid.compare(0, 6,"010001") == 0 )
	{
		ppair = new CSpreadCostPair(firstconstractname, secondconstractname, rightid);
	}

	return ppair;
}

CContract* CSSMCreator::CreateContract(std::string contractname)
{
	if(contractname=="")
	{
		return NULL;
	}
	return new CContract(contractname); 
}

CCommodity* CSSMCreator::CreateCommodity(std::string commodityname)
{
	if(commodityname=="")
	{
		return NULL;
	}
	return new CCommodity(commodityname); 
}

CRTMarketData* CSSMCreator::CreateRTMarketData(std::string constractname)
{
	if(constractname=="")
	{
		return NULL;
	}
	return new CRTMarketData(constractname); 
}

