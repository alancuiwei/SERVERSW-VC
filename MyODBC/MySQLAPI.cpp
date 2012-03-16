#include "StdAfx.h"
#include "MySQLAPI.h"



CMySQLAPI::CMySQLAPI(void)
{
	pServer="localhost";  //������
    pUName="root";       //�û���   
    pUPassword="123456";  //����   
    pDSN="futuretest";         //����Դ�� 
	pPort=3306;
    Init(); 
}


CMySQLAPI::~CMySQLAPI(void)
{
	Close();
}

//����ʱ��ʾ������Ϣ����ʽʹ��ʱ�Ѵ˺�������ע�͵�����
void CMySQLAPI::ReportResult(ENUM_SQLREPORTTYPE flag)
{
	char* message;
	switch (flag)
	{
		case SQLCONNECT_SUCCESS:
			message="Connect successful!";
			break;
		case SQLCONNECT_FAILURE:
			message="Connect error!";
			break;
		case SQLQUERY_SUCCESS:
			message="SQL Query OK!";
			break;
		case SQLQUERY_FAILURE:
			message="SQL Query error!";
			break;
		case SQLEXPECTSINGLEROW_FAILURE:
			message="SQL Query expect single row error!";
			break;
		default:
			message=NULL;
			break;
	}
	if ( message!=NULL )
	{
		cout<<message<<endl;
		CheckError();
	}
}

//�ռ�������Ϣ
void CMySQLAPI::CheckError()
{
	errorNum = mysql_errno(&mydata);
	errorInfo = mysql_error(&mydata);
	cout<<"errorNum:"<<errorNum<<endl;
	cout<<"errorInfo:"<<errorInfo<<endl;
}

//ִ�в�ѯ���
void CMySQLAPI::ExecuteNonQuery(const char* pSql)
{
	if( NULL != mysql_query(&mydata,pSql) )
	//	ReportResult(SQLQUERY_SUCCESS);
	//else
		ReportResult(SQLQUERY_FAILURE);
}


//�������ݿ�
bool CMySQLAPI::Open()
{
	if( mysql_real_connect(&mydata,pServer,pUName,pUPassword,pDSN,pPort,0,0) != NULL)
		return true;
	else
		return false;
}

void CMySQLAPI::Init()
{
	mysql_library_init(0,NULL,NULL);
	mysql_init(&mydata);
	mysql_options(&mydata,MYSQL_SET_CHARSET_NAME,"gbk");

	rowCount=0;
	colCount=0;
	autoNum=0;
	errorNum=0;
	errorInfo="ok";
	affectedRow=0;
}


vector<string*> CMySQLAPI::ExecuteQueryVector(const char* pSql)
{
	vector<string* > v;
	if( 0==mysql_query(&mydata,pSql) )
		ReportResult(SQLQUERY_SUCCESS);
	else
		ReportResult(SQLQUERY_FAILURE);

	affectedRow = (int)mysql_affected_rows(&mydata);
	autoNum = (int)mysql_insert_id(&mydata);
	result = mysql_store_result(&mydata);

	rowCount = (int)mysql_num_rows(result);//��������
	colCount = (int)mysql_num_fields(result);//��������


	MYSQL_ROW line=NULL;
	line=mysql_fetch_row(result);//ȡ��һ�н��

	int j=0;
	while( NULL!=line )//�鿴�Ƿ�Ϊ���У����������������һ�е�����ת�浽data��һ���в�ȡ��һ��
	{	 
		string* rowData=new string[colCount]; 
		for(int i=0; i<colCount;i++)
		{
			rowData[i]=line[i];
		}
		j++;
		v.push_back(rowData); 
		line=mysql_fetch_row(result);
	}

	CheckError();
	return v;
}


string* CMySQLAPI::ExecuteSingleQuery(const char* pSql)
{
	if( 0==mysql_query(&mydata,pSql) )
		ReportResult(SQLQUERY_SUCCESS);
	else
		ReportResult(SQLQUERY_FAILURE);

	affectedRow = (int)mysql_affected_rows(&mydata);
	autoNum = (int)mysql_insert_id(&mydata);
	result = mysql_store_result(&mydata);

	rowCount = (int)mysql_num_rows(result);//��������
	colCount = (int)mysql_num_fields(result);//��������

	if(rowCount != 1 )
	{
	    ReportResult(SQLEXPECTSINGLEROW_FAILURE);
		return NULL;
	}

	string* rowData=new string[colCount];
	MYSQL_ROW line=NULL;
	line=mysql_fetch_row(result);//ȡ��һ�н��

	for(int i=0; i<colCount;i++)
	{
		rowData[i]=line[i];
	}

	return rowData;  
}

int CMySQLAPI::ExecuteQueryNum(const char* pSql)  //ִ�в�ѯ��Ŀ
{
	if( 0==mysql_query(&mydata,pSql) )
		ReportResult(SQLQUERY_SUCCESS);
	else
		ReportResult(SQLQUERY_FAILURE);

	affectedRow = (int)mysql_affected_rows(&mydata);
	autoNum = (int)mysql_insert_id(&mydata);
	result = mysql_store_result(&mydata);

	rowCount = (int)mysql_num_rows(result);//��������
	colCount = (int)mysql_num_fields(result);//��������

	return rowCount;
}


void CMySQLAPI::Close()
{
	mysql_close(&mydata);
}