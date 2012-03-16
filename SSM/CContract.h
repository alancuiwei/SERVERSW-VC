/***********************************************************************
 * Module:  CContract.h
 * Author:  CUIWei
 * Modified: 2011��10��24������һ 22:21:23
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
   /* ��Լ�� */
   std::string contractname;

   vector<CPair*> pairvector;


   /* ������Ʒ�� */
   CCommodity* commodity;
   /* ��Ʒ�� */
   std::string commodityname;
   /* �г����� */
   CRTMarketData* marketdata;
   /* ������ͣ�� */
   double uplimitprice;
   /* ���յ�ͣ�� */
   double downlimitprice;
   /* ����������ջ��ж����� */
   int daystolasttradedate;
   /* �ֲ��޶� */
   int oilimit;
   /* ���뽻��ж����� */
   int daystolastdeliverdate;
   /* �������� */
   double lendrate;
   /* ʵ�ʱ�֤���� */
   double actualmarginrate;


};

#endif