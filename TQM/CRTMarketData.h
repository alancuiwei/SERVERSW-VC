/***********************************************************************
 * Module:  CRTMarketData.h
 * Author:  CUIWei
 * Modified: 2011��10��24������һ 22:14:04
 * Purpose: Declaration of the class CRTMarketData
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CRTMarketData_h)
#define __SSMClassDiagram_CRTMarketData_h
//#include "stdafx.h"
#include <string>


class  CRTMarketData
{
public:
   CRTMarketData(std::string name);

   /* ��Լ�� */
   //std::string constractname;
   char constractname[31];
   /* ʵʱ�۸� */
   double rtprice;
   /* ������һ */
   double askprice1;
   /* �����۶� */
   double askprice2;
   /* �������� */
   double askprice3;
   /* �������� */
   double askprice4;
   /* �������� */
   double askprice5;
   /* �����һ */
   double bidprice1;
   /* ����۶� */
   double bidprice2;
   /* ������� */
   double bidprice3;
   /* ������� */
   double bidprice4;
   /* ������� */
   double bidprice5;
   /* ���տ��̼� */
   double openprice;
   /* ��߼� */
   double highestprice;
   /* ��ͼ� */
   double lowestprice;
   /* �ɽ��� */
   long int volume;
   //std::string time;
   char time[9];


protected:
private:

};

#endif
