/***********************************************************************
 * Module:  CSpreadCostPair.h
 * Author:  Administrator
 * Modified: 2011年10月26日星期三 23:04:22
 * Purpose: Declaration of the class CSpreadCostPair
 * Comment: 无风险跨期套利
 ***********************************************************************/

#if !defined(__Arbitrage_Client_CSpreadCostPair_h)
#define __Arbitrage_Client_CSpreadCostPair_h


#include "CPair.h"

class   CSpreadCostPair : public CPair
{
public:
	CSpreadCostPair();
	~CSpreadCostPair();
   CSpreadCostPair(std::string firstconstractname, std::string secondconstractname, std::string productid);
   virtual void initialization(void);
   /* 计算存储费用 */
   double computestoragefee(void);
   /* 计算存储天数 */
   double computestoragedays(void);
   /* 计算增值税率 */
   double computevatrate(void);
   /* 计算前期合约保证金比例 */
   double computefirstconstractmargin(void);
   /* 计算后期合约保证金比例 */
   double computesecondconstractmargin(void);
   /* 计算交易手续费用 */
   double computetransfee(void);
   /* 计算交割手续费用 */
   double computedeliverfee(void);
   /* 计算交割保证金占用成本 */
   double computedelivermarginfee(void);
   /* 计算交易保证金占用成本 */
   double computetrademarginfee(void);
   /* 计算增值税率费用 */
   double computevatfee(void);
   /* 计算套利费用 */
   double computearbfee(void);
   /* 计算总收入 */
   double computetotalincome(void);
   /* 计算收益率
    * 保证交割收益率：
    * float return_ratio = profit/((r_price+marginamount_forward)*primary_number);
    * 不交割的收益率：
    * float return_ratio = profit/((marginamount_recent+marginamount_forward)*primary_number) */
   double computerateofreturn(void);
   /* 计算年化收益率 */
   double computeannualrateofreturn(void);
   /* 计算当前平仓收益率 */
   double computecurrentrateofreturn(void);
   double computeinvestmoneyamount(void);


   /* 存储天数 */
   double storagedays;
   /* 近期合约保证金比例 */
   double firstcontractmargin;
   /* 远期合约保证金比例 */
   double secondcontractmargin;
   /* 商品增值税率 */
   double vatrate;
   /* 交易手续费用 */
   double transfee;
   /* 交割手续费用 */
   double deliverfee;

   CCommodity* commodity;


};

#endif
