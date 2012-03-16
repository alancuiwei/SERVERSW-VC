// SSM.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SSM.h"

/*�ⲿ���ö���*/
CUser* ssm_puser;
std::map<std::string, CRTMarketData*> ssm_prtmarketdatamap;
std::vector<CPair*> ssm_ppairvector;
std::vector<CContract*> ssm_pcontractvector;
std::vector<CCommodity*> ssm_pcommodityvector;

/*�ڲ����󹹽���*/
CSSMCreator SSMcreator;


/*SSM�������*/
int StartSSM(std::string username)
{
	ssm_puser = SSMcreator.CreateUser(username);
	ssm_puser->initialization();

	/*��ʼ�������ԡ���Լ��Ʒ��*/
	std::vector<CCommodity*>::iterator commosityiter;
    std::vector<CContract*>::iterator contractiter;
    std::vector<CPair*>::iterator pairiter;

	for(commosityiter=ssm_pcommodityvector.begin();
		commosityiter<ssm_pcommodityvector.end(); 
		commosityiter++)
	{
		(*commosityiter)->initialization();
	}

	for(contractiter=ssm_pcontractvector.begin();
		contractiter<ssm_pcontractvector.end(); 
		contractiter++)
	{
		(*contractiter)->initialization( );
	}

	for(pairiter=ssm_ppairvector.begin();
		pairiter<ssm_ppairvector.end(); 
		pairiter++)
	{
		(*pairiter)->initialization( );
	}

	return 42;
}

// This is the constructor of a class that has been exported.
// see SSM.h for the class definition
CSSM::CSSM()
{
	return;
}


