// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SSM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SSM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once


#include "CUser.h"
#include "CPair.h"
#include "CContract.h"
#include "CCommodity.h"
#include "SSMCreator.h"
#include "CSpreadCostPair.h"

extern class CSSMCreator SSMcreator;

// This class is exported from the SSM.dll
class  CSSM {
public:
	CSSM(void);
	// TODO: add your methods here.
};

extern  int nSSM;

/*SSM启动入口*/
extern  int StartSSM(std::string username);
/*用户*/
extern  CUser* ssm_puser;
/*CRTMarketData对象的容器*/
extern  std::map<std::string, CRTMarketData*> ssm_prtmarketdatamap;
/*CPair对象的容器*/
extern  std::vector<CPair*> ssm_ppairvector;
/*CConstract对象的容器*/
extern  std::vector<CContract*> ssm_pcontractvector;
/*CCommodity对象的容器*/
extern  std::vector<CCommodity*> ssm_pcommodityvector;

extern CMySQLAPI SSMDatabase;
