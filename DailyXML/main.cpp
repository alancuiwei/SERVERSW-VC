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


int main()
{
    Database.Init();
    Database.Open();
    CreateCommodityXML();
    CreateValidContractsFile();
    return 0;
}

