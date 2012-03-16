#pragma once

#include "SSM.h"

class CPair;

class CSSMCreator
{
public:
	CSSMCreator(void);
	~CSSMCreator(void);
	CUser* CreateUser(std::string username);
	bool DeleteUser(std::string username);
	CPair* CreatePair(std::string firstcontractname, 
		std::string secondcontractname, std::string rightid);
	bool DeletePair(std::string pairid);
	CContract* CreateContract(std::string constractname);
	bool DeleteContract(std::string constractname);
	CCommodity* CreateCommodity(std::string commodityname);
	bool DeleteCommodity(std::string commodityname);
	CRTMarketData* CreateRTMarketData(std::string constractname);
	bool DeleteCreateRTMarketData(std::string constractname);
};

