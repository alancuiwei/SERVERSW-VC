#include <algorithm>
#include "COMM.h"
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>


std::map<std::string, CRTMarketData*> comm_prtmarketdatamap;

CCOMM::CCOMM(char *pserveraddr, int serverportid)
{
	memset(&serveraddr, 0, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = PF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(pserveraddr);
	serveraddr.sin_port = htons(serverportid);
}

CCOMM::~CCOMM()
{
	// �ͷ����Ӻͽ��н�������
	close(localsocket);
	//WSACleanup();
}
// ����Accept
void* CCOMM::AgentAccept(void *p)
{
    CCOMM *agt = (CCOMM *)p;
    agt->RunAccept();
}

// ����Receive
void* CCOMM::AgentReceive(void *p)
{
    CCOMM *agt = (CCOMM *)p;
    agt->RunReceive();
}

CCOMMServer* CCOMMServer::instance = NULL;
CCOMMServer* CCOMMServer::getinstance( )
{
	if(instance!=NULL)
	{
	    return instance;
	}
	else
	{
	     //instance = new CCOMMServer("192.168.0.103", 6000);
		 //instance->Start();
		 instance = new CCOMMServer();
		 return instance;
	}
}

// ���캯��
CCOMMServer::CCOMMServer(char *pserveraddr, int serverportid) :
      CCOMM(pserveraddr, serverportid)
{
	commtype = 1;
	clientnum = 0;
}

CCOMMServer::CCOMMServer()
{
	memset(&serveraddr, 0, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = PF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(6000);
	commtype = 1;
	clientnum = 0;
}

CCOMMServer::~CCOMMServer()
{

}
// ����ͨѶ����
void CCOMMServer::Start( )
{
	//WSADATA     wsaData;
	//WSAStartup(0x0202, &wsaData); //Initialize Windows socket library

	//localsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���������������׽��֡�TCPЭ��
	localsocket = SocketOpen(SOCK_STREAM);
	//bind(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN));//��ַ�󶨵��׽���
     //���ͻ�����
    SocketBind(localsocket, &serveraddr);
    int nSendBuf=32*1024;//����Ϊ32K
    setsockopt(localsocket,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
	listen(localsocket, clientmaxnum);//��ʼ����
	CLightThread::CreateThread(AgentAccept, (void *)this);
	//hThreadAccept =(HANDLE)_beginthread(AgentAccept, 0, (void *)this);
	//CloseHandle(hThreadAccept);

}


// Accept
void CCOMMServer::RunAccept( )
{
	SOCKET      sclient;
	SOCKADDR_IN dclient;
	//int iaddrSize = sizeof(SOCKADDR_IN);
	while (true)
	{
	   //sclient = accept(localsocket, (struct sockaddr *)&dclient, &iaddrSize);//��������
	   sclient = SocketAccept(localsocket, &dclient);
	   vclientsocket.push_back(sclient);
	   //vclientaddr.push_back(dclient);
	   CLightThread::CreateThread(AgentReceive, (void *)this);
	   //hThreadReceive =(HANDLE)_beginthread(AgentReceive, 0, (void *)this);
	   //CloseHandle(hThreadReceive);

	   //if(clientnum<clientmaxnum)
	   //{
		  //clientnum++;
		  //clientaddr[clientnum] = dclient;
		  //clientsocket[clientnum] = sclient;
	   //}
	}
}


// Receive
void CCOMMServer::RunReceive( )
{
	SOCKET      sclient = vclientsocket.back();
	//int iaddrSize = sizeof(SOCKADDR_IN);
	char c_Message[msgsize]; //�շ�������
	int ret; //�����ֽڵĸ���

	while (true)
	{
		ret = recv(sclient, c_Message, msgsize, 0);//��������
		if((ret==SOCKET_ERROR)||(ret==INVALID_SOCKET))
	    {
			printf("\nclient is closed!");
			vector<SOCKET>::iterator result = find( vclientsocket.begin( ), vclientsocket.end( ), sclient ); //����
			if (result!=vclientsocket.end( ))
			{
				vclientsocket.erase(result);
				SocketClose(sclient);
			}
			return;
	    }
		else
		{
			//printf("\nclient is forbiden!");
			//vector<SOCKET>::iterator result = find( vclientsocket.begin( ), vclientsocket.end( ), sclient ); //����
			//vclientsocket.erase(result);
		 //   return;
		}
	}
}

// ���͸����пͻ���
void CCOMMServer::SendToAllClient(char * buf, int len)
{
	//for(int i=1;i<=clientnum;i++)
 //  {
 //      send(clientsocket[i], buf, len, 0);
 //  }
	for(vector<SOCKET>::iterator iter=vclientsocket.begin();
		iter<vclientsocket.end();
		iter++)
	{
		send(*iter, buf, len, 0);
	}
}


CCOMMClient::CCOMMClient(char *pserveraddr, int serverportid) :
      CCOMM(pserveraddr, serverportid)
{
	commtype = 0;
}

// ����ͨѶ����
void CCOMMClient::Start( )
{
	//WSADATA     wsaData;
	//WSAStartup(0x0202, &wsaData); //Initialize Windows socket library

	//localsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���������������׽��֡�TCPЭ��
	localsocket = SocketOpen(SOCK_STREAM);
    int nRecvBuf=32*1024;//����Ϊ32K
    setsockopt(localsocket,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
	//unsigned long ul = 0;
 //   ioctlsocket(localsocket, FIONBIO, (unsigned long*)&ul);

	connect(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN)); //�����ղ�ָ���ķ�������

		//select ģ�ͣ������ó�ʱ
	//struct timeval timeout ;
	//fd_set r;
	//FD_ZERO(&r);
	//FD_SET(localsocket, &r);
	//timeout.tv_sec = 150; //���ӳ�ʱ15��
	//timeout.tv_usec =0;
	//select(0, 0, &r, 0, &timeout);

	//int TimeOut=10000;   //���ý��ܳ�ʱ3��
	//::setsockopt(localsocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

    //hThreadReceive =(HANDLE)_beginthread(AgentReceive, 0, (void *)this);
	//CloseHandle(hThreadReceive);
	CLightThread::CreateThread(AgentReceive, (void *)this);
}

// Receive
void CCOMMClient::RunReceive( )
{
	char   s_Message[msgsize];
	int    ret, errornum=0;
	isnewsocket = true;
	// �߳�Ƕ�ף�����һ��
	//Sleep(1000);
	while (true)
	{
	   ret = recv(localsocket, s_Message, msgsize, 0);
	   if((ret==SOCKET_ERROR)||(ret==INVALID_SOCKET))
	   {
	        //exit(1);
	        sleep(1);
			if (errornum>3)
			{
			    exit(1);
				errornum = 0;
				printf("\nServer is closed!\n");
				// ÿ���ӳ�������
				if (!isnewsocket)
				{
					isnewsocket = true;
					SocketClose(localsocket);
					//localsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���������������׽��֡�TCPЭ��
					//localsocket = SocketOpen(SOCK_STREAM);
                    //int nRecvBuf=32*1024;//����Ϊ32K
                    //setsockopt(localsocket,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
					//connect(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN)); //�����ղ�ָ���ķ�������
					//hThreadReceive =(HANDLE)_beginthread(AgentReceive, 0, (void *)this);
					//CloseHandle(hThreadReceive);
					//CLightThread::CreateThread(AgentReceive, (void *)this);
					Start();
					return;
				}
				//connect(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN)); //�����ղ�ָ���ķ�������
			}
			errornum++;
			continue;
	   }
	   errornum=0;
	   isnewsocket = false;
	   CRTMarketData* pmarketdata=(CRTMarketData* )s_Message;
	   //cout << pmarketdata->constractname<<endl;
       std::string ctname(pmarketdata->constractname);
	   try
	   {
			if((pmarketdata!=NULL)&&(comm_prtmarketdatamap[ctname]!=NULL))
			{
				*comm_prtmarketdatamap[ctname] = *pmarketdata;
                /*cout << pmarketdata->constractname <<endl;
                cout << pmarketdata->bidprice1 <<endl;
                cout << pmarketdata->askprice1 <<endl;
                cout << pmarketdata->openprice <<endl;
                cout << pmarketdata->highestprice <<endl;
                cout << pmarketdata->lowestprice <<endl;
                cout << pmarketdata->volume <<endl;
                cout << pmarketdata->time <<endl;
                cout << pmarketdata->rtprice <<endl;*/
			}
	   }
	   catch(exception& e)
	   {
	       continue;
	   }
	}
}
