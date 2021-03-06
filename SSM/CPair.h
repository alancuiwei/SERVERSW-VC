/***********************************************************************
 * Module:  CPair.h
 * Author:  CUIWei
 * Modified: 2011年10月24日星期一 22:21:38
 * Purpose: Declaration of the class CPair
 ***********************************************************************/

#if !defined(__SSMClassDiagram_CPair_h)
#define __SSMClassDiagram_CPair_h

#include "CContract.h"

class CUser;

class  CPair
{
public:
	CPair(){};
	~CPair(){};
   CPair(std::string firstcontractname, std::string secondcontractname, std::string rightid);
   virtual void initialization(void);

   vector<CUser*> uservector;
   std::string firstcontractname;
   std::string secondcontractname;
   int pairid;
   std::string productid;
   std::string strategyid;
   CContract* firstcontract;
   CContract* secondcontract;
   bool isinstrumentsupport;
   int strategytypeid;
   std::string symbol;

protected:
private:



};

#endif
