/***********************************************************************
 * Module:  CContract.cpp
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:21:23
 * Purpose: Implementation of the class CContract
 ***********************************************************************/

#include "stdafx.h"
#include "CPair.h"
#include "CContract.h"
#include "SSM.h"
#include <regex>   
#include <string>   



////////////////////////////////////////////////////////////////////////
// Name:       CContract::CContract(std::string contractname)
// Purpose:    Implementation of CContract::CContract()
// Parameters:
// - contractname
// Return:     void
////////////////////////////////////////////////////////////////////////

CContract::CContract(std::string contractname)
{
   this->contractname = contractname;
   cout<<"contractname:"<<contractname<<"合约新建"<<endl;
   /*计算commodity*/
    const char *first = contractname.c_str(); 
    const char *last = first + strlen(first); 
    std::cmatch mr; 
	std::regex rx("^[A-Za-z][A-Za-z][0-9]"); 

	if(std::regex_search(first, last, mr, rx))
	{
		std::string tempstr(first, 2);
		commodityname=tempstr;
	}
	else
	{
		std::string tempstr(first, 1);
		commodityname=tempstr;	
	}
	/*创建commodity对象*/
   std::vector<CCommodity*>::iterator iter;
   CCommodity* findcommodity = NULL;
   for(iter=ssm_pcommodityvector.begin(); iter<ssm_pcommodityvector.end(); iter++)
   {
       if( (*iter)->commodityname == commodityname )
	   {
	       findcommodity = *iter;
	   }
   }
   if( findcommodity==NULL )
   {
       findcommodity = SSMcreator.CreateCommodity(commodityname);
	   ssm_pcommodityvector.push_back(findcommodity);
   }
   /*记录该品种所有的合约*/
   commodity = findcommodity;
   if(std::find(commodity->contractvector.begin(),commodity->contractvector.end(),this) == commodity->contractvector.end())
   {
	   commodity->contractvector.push_back(this);
   }

   /*创建RTMarketdata对象*/
   //if(tqm_prtmarketdatamap[contractname] != NULL)
   //{
   marketdata = SSMcreator.CreateRTMarketData(contractname);
   //}
   ssm_prtmarketdatamap.insert( pair<std::string, CRTMarketData*>(contractname, marketdata) );
}

CContract::~CContract()
{
	//delete marketdata;
}


////////////////////////////////////////////////////////////////////////
// Name:       CContract::initialization()
// Purpose:    Implementation of CContract::initialization()
// Return:     void
////////////////////////////////////////////////////////////////////////

void CContract::initialization(void)
{
    cout<<"contractname:"<<contractname<<"合约初始化"<<endl;
   	CMyODBC* pmyodbc = new CMyODBC();  
	pmyodbc->Connect(); 
	std::string sqlstr = "select daystolasttradedate,daystolastdeliverdate,uplimitprice,downlimitprice,\
						 contractmarginrate, lendrate from contract_t c, usercontract_t u \
						 where c.contractid='" + contractname + "' and u.userid='" + \
						 ssm_puser->username + "' and u.contractid='" + contractname + "'";
	std::string* contractinfo = pmyodbc->ExecuteSingleQuery(sqlstr.c_str()); 
	if(contractinfo!=NULL)
	{
		daystolasttradedate = atoi(contractinfo[0].c_str());
		daystolastdeliverdate = atoi(contractinfo[1].c_str());
		uplimitprice = atof(contractinfo[2].c_str());
		downlimitprice = atof(contractinfo[3].c_str());
		actualmarginrate = atof(contractinfo[4].c_str());
		lendrate = (contractinfo[5] == "")?commodity->lendrate:atof(contractinfo[5].c_str());
		oilimit = 0;
		delete [] contractinfo;
	}
	delete pmyodbc;
}

