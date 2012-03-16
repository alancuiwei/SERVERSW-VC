/***********************************************************************
 * Module:  CPair.cpp
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:21:38
 * Purpose: Implementation of the class CPair
 ***********************************************************************/
#include "stdafx.h"
#include "CUser.h"
#include "CPair.h"
#include "SSM.h"

//class CContract;
////////////////////////////////////////////////////////////////////////
// Name:       CPair::CPair(std::string firstcontractname, std::string secondcontractname, std::string rightid)
// Purpose:    Implementation of CPair::CPair()
// Parameters:
// - firstcontractname
// - secondcontractname
// - rightid
// Return:     void
////////////////////////////////////////////////////////////////////////

CPair::CPair(std::string firstcontractname, std::string secondcontractname, std::string productid)
{
   this->firstcontractname = firstcontractname;
   this->secondcontractname = secondcontractname;
   this->productid = productid;
   pairid = ssm_ppairvector.size();
   strategyid = productid.substr(0,6);
   /*填写symbol*/
   symbol = "SP " + firstcontractname + "&" + secondcontractname;
   strategytypeid = 0;
   /*先查看是否时候已存在该contract,之后再加入*/
   std::vector<CContract*>::iterator iter;
   CContract* findfirstcontract = NULL;
   CContract* findsecondcontract = NULL;
   for(iter=ssm_pcontractvector.begin(); iter<ssm_pcontractvector.end(); iter++)
   {
       if( (*iter)->contractname == firstcontractname )
	   {
	       findfirstcontract = *iter;
	   }
	   else if( (*iter)->contractname == secondcontractname )
	   {
	       findsecondcontract = *iter;
	   }
   }
   if( findfirstcontract==NULL )
   {
       findfirstcontract = SSMcreator.CreateContract(firstcontractname);
	   if( findfirstcontract != NULL )
	   {
	       ssm_pcontractvector.push_back(findfirstcontract);
	   }
   }
   if( findsecondcontract==NULL )
   {
       findsecondcontract = SSMcreator.CreateContract(secondcontractname);
	   if( findsecondcontract != NULL )
	   {
		   ssm_pcontractvector.push_back(findsecondcontract);
	   }
   }
   /*记录调用该合约的pair信息*/
   firstcontract = findfirstcontract;
   if(std::find(firstcontract->pairvector.begin(),firstcontract->pairvector.end(),this) == firstcontract->pairvector.end())
   {
	   firstcontract->pairvector.push_back(this);
   }
   secondcontract = findsecondcontract;
   if(std::find(secondcontract->pairvector.begin(),secondcontract->pairvector.end(),this) == secondcontract->pairvector.end())
   {
	   secondcontract->pairvector.push_back(this);
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       CPair::initialization()
// Purpose:    Implementation of CPair::initialization()
// Return:     void
////////////////////////////////////////////////////////////////////////

void CPair::initialization(void)
{
    cout<<"CPair:"<<pairid<<"套利对初始化"<<endl;
	CMyODBC* pmyodbc = new CMyODBC();
	pmyodbc->Connect();
	std::string sqlstr = "select isinstrumentsupport from commodityright_t where rightid='" + productid + "'";
	std::string* pairinfo = pmyodbc->ExecuteSingleQuery(sqlstr.c_str()); 
	if(pairinfo!=NULL)
	{
		isinstrumentsupport = atoi(pairinfo[0].c_str())>0 ? true:false;
		delete [] pairinfo;
	}
	delete pmyodbc;
}
