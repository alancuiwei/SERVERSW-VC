/***********************************************************************
 * Module:  CSpreadCostPair.cpp
 * Author:  Administrator
 * Modified: 2011年10月26日星期三 23:04:22
 * Purpose: Implementation of the class CSpreadCostPair
 * Comment: 无风险跨期套利
 ***********************************************************************/

#include "stdafx.h"
#include "CSpreadCostPair.h"

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::CSpreadCostPair(std::string firstcontractname, std::string secondcontractname, std::string productid)
// Purpose:    Implementation of CSpreadCostPair::CSpreadCostPair()
// Parameters:
// - firstcontractname
// - secondcontractname
// - productid
// Return:     
////////////////////////////////////////////////////////////////////////

CSpreadCostPair::CSpreadCostPair(std::string firstcontractname, std::string secondcontractname, std::string productid)
	:CPair(firstcontractname, secondcontractname, productid)
{
   // TODO : implement
   cout<<"CSpreadCostPair:"<<firstcontractname<<"和"<<secondcontractname<<"套利对新建"<<endl;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::initialization()
// Purpose:    Implementation of CSpreadCostPair::initialization()
// Return:     void
////////////////////////////////////////////////////////////////////////

void CSpreadCostPair::initialization(void)
{
    // TODO : implement
	CPair::initialization();
	commodity = firstcontract->commodity;
   /* 计算存储费用 */
   computestoragefee();
   /* 计算存储天数 */
   computestoragedays();
   /* 计算增值税率 */
   computevatrate();
   /* 计算前期合约保证金比例 */
   computefirstconstractmargin();
   /* 计算后期合约保证金比例 */
   computesecondconstractmargin();
   /* 计算交易手续费用 */
   computetransfee();
   /* 计算交割手续费用 */
   computedeliverfee();
   cout<<"CSpreadCostPair:"<<firstcontractname<<"和"<<secondcontractname<<"套利对初始化"<<endl;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computestoragefee()
// Purpose:    Implementation of CSpreadCostPair::computestoragefee()
// Comment:    计算存储费用
// Return:     float
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computestoragefee(void)
{
	double storagecost=0;
    storagecost = storagedays * (commodity->storagedailyfee);
    return storagecost;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computestoragedays()
// Purpose:    Implementation of CSpreadCostPair::computestoragedays()
// Comment:    计算存储天数
// Return:     int
////////////////////////////////////////////////////////////////////////

int CSpreadCostPair::computestoragedays(void)
{ 
   storagedays = (secondcontract->daystolastdeliverdate - firstcontract->daystolastdeliverdate);
   return storagedays;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computevatrate()
// Purpose:    Implementation of CSpreadCostPair::computevatrate()
// Comment:    计算增值税率
// Return:     float
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computevatrate(void)
{
    vatrate = commodity->vatrate;
    return vatrate;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computefirstconstractmargin()
// Purpose:    Implementation of CSpreadCostPair::computefirstconstractmargin()
// Comment:    计算前期合约保证金比例
// Return:     float
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computefirstconstractmargin(void)
{
    firstcontractmargin = firstcontract->actualmarginrate;
    return firstcontractmargin;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computesecondconstractmargin()
// Purpose:    Implementation of CSpreadCostPair::computesecondconstractmargin()
// Comment:    计算后期合约保证金比例
// Return:     float
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computesecondconstractmargin(void)
{
    secondcontractmargin = secondcontract->actualmarginrate;
    return secondcontractmargin;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computetransfee()
// Purpose:    Implementation of CSpreadCostPair::computetransfee()
// Comment:    计算交易手续费用
// Return:     float
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computetransfee(void)
{
    transfee = commodity->transfee * 2;
    return transfee;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computedeliverfee()
// Purpose:    Implementation of CSpreadCostPair::computedeliverfee()
// Comment:    计算交割手续费用
// Return:     float
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computedeliverfee(void)
{
    deliverfee = commodity->deliverchargebyhand * 2;
    return deliverfee;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computedelivermarginfee()
// Purpose:    Implementation of CSpreadCostPair::computedelivermarginfee()
// Comment:    计算交割保证金占用成本
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computedelivermarginfee(void)
{
    return (firstcontract->marketdata->rtprice * commodity->tradeunit
		* firstcontract->lendrate * storagedays)/365;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computetrademarginfee()
// Purpose:    Implementation of CSpreadCostPair::computetrademarginfee()
// Comment:    计算交易保证金占用成本
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computetrademarginfee(void)
{
    double ret;
	if(commodity->issinglemargin)
	{
	    ret = firstcontract->marketdata->rtprice * commodity->tradeunit * firstcontractmargin
			  * firstcontract->lendrate * firstcontract->daystolastdeliverdate / 365
			  + secondcontract->marketdata->rtprice * commodity->tradeunit * secondcontractmargin
			  * secondcontract->lendrate * storagedays / 365;
	}
	else
	{
	    ret = ( firstcontract->marketdata->rtprice * commodity->tradeunit * firstcontractmargin
			  * firstcontract->lendrate 
			  + secondcontract->marketdata->rtprice * commodity->tradeunit * secondcontractmargin
			  * secondcontract->lendrate) * firstcontract->daystolastdeliverdate / 365
			  + secondcontract->marketdata->rtprice * commodity->tradeunit * secondcontractmargin
			  * secondcontract->lendrate * storagedays / 365;	
	}
    return ret;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computevatfee()
// Purpose:    Implementation of CSpreadCostPair::computevatfee()
// Comment:    计算增值税率费用
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computevatfee(void)
{
    return abs(secondcontract->marketdata->rtprice - firstcontract->marketdata->rtprice)
		    * commodity->tradeunit * vatrate / (1 + vatrate);
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computearbfee()
// Purpose:    Implementation of CSpreadCostPair::computearbfee()
// Comment:    计算套利费用
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computearbfee(void)
{
    return (computevatfee()
		+ computestoragefee() 
		+ computetransfee() 
		+ computedeliverfee() 
		+ computetrademarginfee() 
		+ computedelivermarginfee());
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computetotalincome()
// Purpose:    Implementation of CSpreadCostPair::computetotalincome()
// Comment:    计算总收入
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computetotalincome(void)
{
    return (secondcontract->marketdata->rtprice - firstcontract->marketdata->rtprice)
		    * commodity->tradeunit;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computerateofreturn()
// Purpose:    Implementation of CSpreadCostPair::computerateofreturn()
// Comment:    计算收益率
//             保证交割收益率：
//             float return_ratio = profit/((r_price+marginamount_forward)*primary_number);
//             不交割的收益率：
//             float return_ratio = profit/((marginamount_recent+marginamount_forward)*primary_number)
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computerateofreturn(void)
{
    return (computetotalincome() - computearbfee()) 
		    / computeinvestmoneyamount();
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computeannualrateofreturn()
// Purpose:    Implementation of CSpreadCostPair::computeannualrateofreturn()
// Comment:    计算年化收益率
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computeannualrateofreturn(void)
{
    return computerateofreturn() * 365 / secondcontract->daystolastdeliverdate;
}

////////////////////////////////////////////////////////////////////////
// Name:       CSpreadCostPair::computecurrentrateofreturn()
// Purpose:    Implementation of CSpreadCostPair::computecurrentrateofreturn()
// Comment:    计算当前平仓收益率
// Return:     double
////////////////////////////////////////////////////////////////////////

double CSpreadCostPair::computecurrentrateofreturn(void)
{
   // TODO : implement
    return NULL;
}

double CSpreadCostPair::computeinvestmoneyamount(void)
{
    return (secondcontract->marketdata->rtprice * secondcontractmargin + firstcontract->marketdata->rtprice)
		    * commodity->tradeunit;
}