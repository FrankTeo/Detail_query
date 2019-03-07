
/********************************************************************
copyright (C), 1995-2005, Si-Tech Information Technology Ltd. 
file_name: commagent.c 
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 实现与通讯服务的接口
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ... 
********************************************************************/

#include "commagent.h"
#include "common.h"

extern struct CommCtrl databuf;

/**********************************************************
function: ConnAgent
description: 连接通讯服务
Input: szIP: 通讯服务所在服务器IP
       szPort: 通讯服务连接端口
       szExCode: 业务代码
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int ConnAgent( char * szIP, char * szPort, char * szExCode )
{
	int ret;
#ifdef	__COMM_AGENT__
	if( ( ret = conn_agent ( szIP, atol(szPort), atol(szExCode) ) ) <= 0 )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, 
						"conn_agent(%s,%s,%s) failed[%d, %d]\n", 
								szIP, szPort, szExCode, errno, ret );
		return 1;
	}
#endif	
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "成功建立与通讯服务的连接!\n\n" );
	
	return 0;
}

/**********************************************************
function: RecvMsg
description: 接收通讯服务发送的消息
Input: szMsg: 消息缓存区
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int RecvMsg( char * szMsg )
{
	int len, ret;
#ifdef	__COMM_AGENT__
    ret = recv_msg_srv( &len );
    if( ret == -100 )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "receive msg is timeout\n" );
		
		ret = recv_ready ();
		if( ret < 0 )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "recv_ready() failed[%d]\n", errno );
			return 1;
		}
		
		return 1;
    }
    else if( ret < 0 )
    {
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "recv_msg_serv() failed[%d]\n", errno );
		return 1;
    }
    else
    {
		databuf.msgbody[len] = 0x0;
	}

	strcpy( szMsg, databuf.msgbody );
#endif
	return 0;
}	

/**********************************************************
function: SendMsg
description: 向通讯服务发送消息
Input: szBuf: 消息缓存区
       lSize: 消息大小
       nCode: 状态代码
       nFlag: 发送标志
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int SendMsg( char * szBuf, long lSize, int nCode, int nFlag )
{
	int ret, flag;
	
	switch( nFlag )
	{
		case 0:
			flag = MSGBEGIN|MSGEND;	
			break;
		
		case 1:
			flag = MSGBEGIN;
			break;
		
		case 2:
			flag = MSGCONTINUE;
			break;
			
		case 3:
			flag = MSGCONTINUE|MSGEND;
			break;
		
		default:
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的标志\n" );
			return 1;
			break;	
	}

#ifdef	__COMM_AGENT__
	if( NULL != szBuf )
	{
		strcpy( databuf.msgbody, szBuf );
	}
	else
	{
		databuf.msgbody[0] = 0;
	}
	
	ret = send_msg_srv( lSize, nCode, flag );
	
	if( ret < 0 )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "send_msg_serv() failed[%d]\n", errno );
		return 1;
	}
#else
	if( NULL != szBuf )
	{
		printf( "%s\n", szBuf );
	}
#endif
	return 0;
}
