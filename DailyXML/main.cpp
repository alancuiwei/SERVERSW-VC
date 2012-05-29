#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <string.h>
#include "../MyODBC/MySQLAPI.h"

CMySQLAPI Database;

using namespace std;

void CreateCommodityXML(void )
{
    ofstream xmlfs;
    ostringstream xmlss;
    std::string sqlstr = "select commodityid, exchtrademargin from commodity_t ";
	vector<string* > cmvector = Database.ExecuteQueryVector(sqlstr.c_str());
    vector<string* >::iterator iter;
    xmlss<<"<commodity>"<< endl;
    for(iter=cmvector.begin();iter<cmvector.end();iter++)
    {
       xmlss<<"<cm>"<<endl;
       xmlss<<"<commodityid>"<<(*iter)[0]<<"</commodityid>"<< endl;
       xmlss<<"<exchtrademargin>"<<(*iter)[1]<<"</exchtrademargin>"<< endl;
       xmlss<<"</cm>"<<endl;
	   delete [] (*iter);
    }
    xmlss<<"</commodity>"<< endl;
    char xmlfilename[128] =  "../tongtianshun/app/assets/images/commodity.xml";
    xmlfs.open(xmlfilename, ios::trunc);
    xmlfs<<xmlss.str();
    xmlfs.close();
}

void CreateValidContractsFile(void)
{
    ofstream xmlfs;
    ostringstream xmlss;
    std::string sqlstr = "select * from validcontracts_t ";
	vector<string* > ctvector = Database.ExecuteQueryVector(sqlstr.c_str());
    vector<string* >::iterator iter;
    for(iter=ctvector.begin();iter<ctvector.end();iter++)
    {
       xmlss<<(*iter)[0]<< endl;
	   delete [] (*iter);
    }
    char xmlfilename[128] =  "../tongtianshun/app/assets/images/validcontracts.fl";
    xmlfs.open(xmlfilename, ios::trunc);
    xmlfs<<xmlss.str();
    xmlfs.close();
}

void CreateContractInfo(void)
{
    ofstream xmlfs;
    ostringstream xmlss;
    xmlss<<"<contract>"<<endl;
    std::string sqlstr = "select vc.contractid, uct.contractmarginrate, ct.dailypricelimit from validcontracts_t as vc, contract_t as ct, usercontract_t as uct ";
    sqlstr += " where ct.contractid=vc.contractid and uct.contractid=vc.contractid and uct.userid='tester1'";
	vector<string* > ctvector = Database.ExecuteQueryVector(sqlstr.c_str());
    vector<string* >::iterator iter;
    for(iter=ctvector.begin();iter<ctvector.end();iter++)
    {
       xmlss<<"<ct>"<<endl;
       xmlss<<"<contractid>"<<(*iter)[0]<<"</contractid>"<< endl;
       xmlss<<"<marginrate>"<<(*iter)[1]<<"</marginrate>"<< endl;
       xmlss<<"<updownlimit>"<<(*iter)[2]<<"</updownlimit>"<< endl;
       xmlss<<"</ct>"<<endl;
	   delete [] (*iter);
    }
    xmlss<<"</contract>"<< endl;
    char xmlfilename[128] =  "../tongtianshun/app/assets/images/contract.xml";
    xmlfs.open(xmlfilename, ios::trunc);
    xmlfs<<xmlss.str();
    xmlfs.close();
}


int main()
{
    Database.Init();
    Database.Open();
    CreateCommodityXML();
    //CreateValidContractsFile();
    CreateContractInfo();
    Database.Close();
    return 0;
}

