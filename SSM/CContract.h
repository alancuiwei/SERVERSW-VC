/***********************************************************************
 * Module:  CContract.h
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:21:23
 * Purpose: Declaration of the class CContract
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CConstract_h)
#define __SSMClassDiagram_CConstract_h

#ifdef SSM_EXPORTS
#define SSM_API __declspec(dllexport)
#else
#define SSM_API __declspec(dllimport)
#endif

#include "CCommodity.h"

class CCommodity;
class CPair;
//class CRTMarketData;

class SSM_API CContract
{
public:
   CContract(std::string contractname);
   ~CContract();
   void initialization(void);
   /* 合约名 */
   std::string contractname;

   vector<CPair*> pairvector;


   /* 所属商品类 */
   CCommodity* commodity;
   /* 商品名 */
   std::string commodityname;
   /* 市场数据 */
   CRTMarketData* marketdata;
   /* 明日涨停价 */
   double uplimitprice;
   /* 明日跌停价 */
   double downlimitprice;
   /* 距离最后交易日还有多少天 */
   int daystolasttradedate;
   /* 持仓限额 */
   int oilimit;
   /* 距离交割还有多少天 */
   int daystolastdeliverdate;
   /* 贷款利率 */
   double lendrate;
   /* 实际保证金率 */
   double actualmarginrate;


};

#endif