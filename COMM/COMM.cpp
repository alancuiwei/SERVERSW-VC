// COMM.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <algorithm>
#include "COMM.h"

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
	// 释放连接和进行结束工作
	closesocket(localsocket);
	WSACleanup();
}
// 调用Accept
void CCOMM::AgentAccept(void *p)
{
    CCOMM *agt = (CCOMM *)p;
    agt->RunAccept();
}

// 调用Receive
void CCOMM::AgentReceive(void *p)
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
	     instance = new CCOMMServer("192.168.0.115", 6000);
		 //instance->Start();
		 return instance;
	}
}

// 构造函数
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
	serveraddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	serveraddr.sin_port = htons(6000); 
	commtype = 1;
	clientnum = 0;
}

CCOMMServer::~CCOMMServer()
{

}
// 激活通讯对象
void CCOMMServer::Start( )
{
	WSADATA     wsaData;
	WSAStartup(0x0202, &wsaData); //Initialize Windows socket library

	localsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建服务器监听套节字。TCP协议
	bind(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN));//地址绑定到套接字

	listen(localsocket, clientmaxnum);//开始监听
	hThreadAccept =(HANDLE)_beginthread(AgentAccept, 0, (void *)this);
	CloseHandle(hThreadAccept);

}


// Accept
void CCOMMServer::RunAccept( )
{
	SOCKET      sclient;
	SOCKADDR_IN dclient;
	int iaddrSize = sizeof(SOCKADDR_IN); 
	while (TRUE)
	{  
	   sclient = accept(localsocket, (struct sockaddr *)&dclient, &iaddrSize);//建立连接   
	   vclientsocket.push_back(sclient);
	   //vclientaddr.push_back(dclient);
	   hThreadReceive =(HANDLE)_beginthread(AgentReceive, 0, (void *)this);
	   CloseHandle(hThreadReceive);

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
	int iaddrSize = sizeof(SOCKADDR_IN); 
	char c_Message[msgsize]; //收发缓冲区
	int ret; //接收字节的个数

	while (TRUE)
	{  
		ret = recv(sclient, c_Message, msgsize, 0);//接收数据
		if(ret==SOCKET_ERROR)
	    {
			printf("\nclient is closed!");		
			vector<SOCKET>::iterator result = find( vclientsocket.begin( ), vclientsocket.end( ), sclient ); //查找
			if (result!=vclientsocket.end( ))
			{
				vclientsocket.erase(result);
				closesocket(sclient);
			}		
			return;
	    }
		else
		{
			//printf("\nclient is forbiden!");
			//vector<SOCKET>::iterator result = find( vclientsocket.begin( ), vclientsocket.end( ), sclient ); //查找
			//vclientsocket.erase(result);
		 //   return;
		}
	}
}

// 发送给所有客户端
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

// 激活通讯对象
void CCOMMClient::Start( )
{
	WSADATA     wsaData;
	WSAStartup(0x0202, &wsaData); //Initialize Windows socket library

	localsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建服务器监听套节字。TCP协议

	//unsigned long ul = 0;
 //   ioctlsocket(localsocket, FIONBIO, (unsigned long*)&ul);

	connect(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN)); //连到刚才指明的服务器上

		//select 模型，即设置超时
	//struct timeval timeout ;
	//fd_set r;
	//FD_ZERO(&r);
	//FD_SET(localsocket, &r);
	//timeout.tv_sec = 150; //连接超时15秒
	//timeout.tv_usec =0;
	//select(0, 0, &r, 0, &timeout);

	//int TimeOut=10000;   //设置接受超时3秒 
	//::setsockopt(localsocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

    hThreadReceive =(HANDLE)_beginthread(AgentReceive, 0, (void *)this);
	CloseHandle(hThreadReceive);
}

// Receive
void CCOMMClient::RunReceive( )
{
	char   s_Message[msgsize];
	int    ret, errornum=0;
	isnewsocket = true;
	// 线程嵌套，缓冲一秒
	//Sleep(1000);
	while (TRUE)
	{
	   ret = recv(localsocket, s_Message, msgsize, 0);
	   if(ret==SOCKET_ERROR)
	   {
		   Sleep(1000);
			if (errornum>3)
			{
				errornum = 0;
				printf("\nServer is closed!\n");
				// 每秒钟尝试重连
				if (!isnewsocket)
				{
					isnewsocket = true;
					closesocket(localsocket);
					localsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建服务器监听套节字。TCP协议
					connect(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN)); //连到刚才指明的服务器上
					hThreadReceive =(HANDLE)_beginthread(AgentReceive, 0, (void *)this);
					CloseHandle(hThreadReceive);
					return;
				}
				connect(localsocket, (struct sockaddr *)&serveraddr, sizeof(SOCKADDR_IN)); //连到刚才指明的服务器上
			}
			errornum++;
			continue;
	   }
	   errornum=0;
	   isnewsocket = false;
	   CRTMarketData* pmarketdata=(CRTMarketData* )s_Message;
		if( comm_prtmarketdatamap[ pmarketdata->constractname ] != NULL)
		{
			*comm_prtmarketdatamap[pmarketdata->constractname] = *pmarketdata;

		}
	}
}
