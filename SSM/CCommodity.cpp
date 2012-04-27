/***********************************************************************
 * Module:  CCommodity.cpp
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:20:55
 * Purpose: Implementation of the class CCommodity
 ***********************************************************************/

#include "stdafx.h"
#include "CContract.h"
#include "CCommodity.h"

////////////////////////////////////////////////////////////////////////
// Name:       CCommodity::CCommodity(std::string commodityname)
// Purpose:    Implementation of CCommodity::CCommodity()
// Parameters:
// - commodityname
// Return:     void
////////////////////////////////////////////////////////////////////////

CCommodity::CCommodity(std::string commodityname)
{
   this->commodityname = commodityname;
   cout<<"commodityname:"<<commodityname<<"品种新建"<<endl;
}

////////////////////////////////////////////////////////////////////////
// Name:       CCommodity::initialization()
// Purpose:    Implementation of CCommodity::initialization()
// Return:     void
////////////////////////////////////////////////////////////////////////

void CCommodity::initialization(void)
{
    cout<<"commodityname:"<<commodityname<<"品种初始化"<<endl;
	//CMySQLAPI* pmyodbc = new CMySQLAPI();
    //pmyodbc->Init();
    //pmyodbc->Open();
	std::string sqlstr = "select exchangeid,tradeunit,todayexitdiscount,cancelmonth,";
	sqlstr = sqlstr + "delivermonth,issinglemargin,tick,tradechargetype,";
    sqlstr = sqlstr + "tradecharge,vatrate,storagedailyfee,u.lendrate,deliverchargebyhand, b.lendrate, u.trademargingap, exchtrademargin ";
	sqlstr = sqlstr + "from commodity_t c, usercommodity_t u, bank_t b where c.commodityid='" + commodityname + "' ";
	sqlstr = sqlstr + "and u.userid='" + ssm_puser->username + "' and u.commodityid='" + commodityname + "' ";
	//std::string* commodityinfo = pmyodbc->ExecuteSingleQuery(sqlstr.c_str());
	std::string* commodityinfo = SSMDatabase.ExecuteSingleQuery(sqlstr.c_str());
	if(commodityinfo!=NULL)
	{
		exchangeid = commodityinfo[0];
		tradeunit = atoi(commodityinfo[1].c_str());
		todayexitdiscount = atof(commodityinfo[2].c_str());
		cancelmonth = commodityinfo[3];
		delivermonth = commodityinfo[4];
		issinglemargin = atoi(commodityinfo[5].c_str())>0 ? true:false;
		tick = atoi(commodityinfo[6].c_str());
		transtype = atoi(commodityinfo[7].c_str());
		transfee = atof(commodityinfo[8].c_str());
		vatrate = atof(commodityinfo[9].c_str());
		storagedailyfee = atof(commodityinfo[10].c_str());
		lendrate = (atof(commodityinfo[11].c_str()) < 0)? atof(commodityinfo[13].c_str()):atof(commodityinfo[11].c_str());
		deliverchargebyhand = atof(commodityinfo[12].c_str());
		trademargingap = atof(commodityinfo[14].c_str());
		exchtrademargin = atof(commodityinfo[15].c_str());
		delete [] commodityinfo;
	}
	//delete pmyodbc;
}

