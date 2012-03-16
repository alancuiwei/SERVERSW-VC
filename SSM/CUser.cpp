/***********************************************************************
 * Module:  CUser.cpp
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:21:38
 * Purpose: Implementation of the class CUser
 ***********************************************************************/
#include "stdafx.h"
#include "CUser.h"
#include <time.h>
#include "SSM.h"

//#include <odbcinst.h> 
//#import "..//Debug//msado15.dll" 

////////////////////////////////////////////////////////////////////////
// Name:       CUser::CUser(std::string username)
// Purpose:    Implementation of CUser::CUser()
// Comment:    该构造函数对username进行赋值。
// Parameters:
// - username
// Return:     void
////////////////////////////////////////////////////////////////////////

CUser::CUser(std::string username)
{
   printf("%s\n",__FUNCTION__);
   this->username = username;
   computeallowpairs( );
}

////////////////////////////////////////////////////////////////////////
// Name:       CUser::initialization()
// Purpose:    Implementation of CUser::initialization()
// Comment:    Initialization对usertype 和 allowpairs进行计算赋值。
// Return:     void
////////////////////////////////////////////////////////////////////////

void CUser::initialization(void)
{
    cout<<"CUser:"<<username<<"用户初始化"<<endl;
	CMyODBC* pmyodbc = new CMyODBC();  
	pmyodbc->Connect();
	std::string sqlstr = "select usertype,ibbranchid from user_t where userid='" + username + "'";
	std::string* userinfo = pmyodbc->ExecuteSingleQuery(sqlstr.c_str()); 
	if(userinfo!=NULL)
	{
		usertype = atoi(userinfo[0].c_str());
		ibbranchid = userinfo[1];
		delete [] userinfo;
	}
	cout<<"usertype:"<<usertype<<endl;
	cout<<"ibbranchid:"<<ibbranchid<<endl;
	delete pmyodbc;
	//tqm_prtmarketdatamap = ssm_prtmarketdatamap;
	comm_prtmarketdatamap = ssm_prtmarketdatamap;
}

////////////////////////////////////////////////////////////////////////
// Name:       CUser::computeallowpairs()
// Purpose:    Implementation of CUser::computeallowpairs()
// Comment:    通过username查询该用户运行访问的套利对和套利类型集合，并逐一按此创建classes
//             并将这些classes指针存放在allowpairs数组中
// Return:     void
////////////////////////////////////////////////////////////////////////

void CUser::computeallowpairs(void)
{
	printf("%s\n",__FUNCTION__);
	/*取得当前日期*/
	struct tm newtime;
	time_t curtime = time(NULL); 
	localtime_s(&newtime,&curtime);
	char tmp[64];    
	strftime( tmp, sizeof(tmp), "'%Y-%m-%d'", &newtime );    
	std::string datestr = tmp;
	/*查询数据库，得到productid和对应合约名*/
 	std::string sqlstr = "select * from userright_t where userid='" +
		username + "'" + "and invaliddate > " + datestr;

	CMyODBC* pmyodbc = new CMyODBC();  
	pmyodbc->Connect();  
	vector<string* > userrightvector = pmyodbc->ExecuteQueryVector(sqlstr.c_str());  
    vector<string* >::iterator rightiter;  
	sqlstr = "select * from arbitragecontractpairs_t where rightid in  (";
    for(rightiter=userrightvector.begin();rightiter<userrightvector.end();rightiter++)  
    {  
       /*for(int i=0;i<=pmyodbc->GetColCount();i++)  
       {  
           cout<<(*rightiter)[i]<<" ";  
       }
       cout<<" "<<endl;*/
	   allowproductids.push_back((*rightiter)[1]);
	   sqlstr = sqlstr + "'" + (*rightiter)[1] + "',";
	   delete [] (*rightiter);
    } 
	sqlstr = sqlstr + "'')";
	userrightvector.clear();
	userrightvector = pmyodbc->ExecuteQueryVector(sqlstr.c_str()); 
    for(rightiter=userrightvector.begin();rightiter<userrightvector.end();rightiter++)  
    {  
	   CPair* ppair = SSMcreator.CreatePair((*rightiter)[0], (*rightiter)[1], (*rightiter)[2]);
	   ppair->uservector.push_back(this);
	   if( ppair != NULL )
	   {
		   ssm_ppairvector.push_back(ppair);
	   }
	   delete [] (*rightiter);
    }
	delete pmyodbc;
}

