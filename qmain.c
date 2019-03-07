 
/********************************************************************
copyright (C), 1995-2005, Si-Tech Information Technology Ltd.
file_name: qmain.c
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 实现详单查询系统调度
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ...
********************************************************************/


#include "qret.h"
#include "qdetail.h"

/********************************************************** 
function: DebugOutCmd
description: 输出查询指令信息
Input:	psCmd: 描述查询指令各项内容的结构体
Output:
Return: 0 成功
others:
***********************************************************/

int DebugOutCmd( PSEARCH_CMD psCmd )
{
	printf( "\n" );
	printf( "\t-- Query Command --\n" );
	printf( "\tServiceID   = [%s]\n", psCmd->szSvcID );
	printf( "\tStartTime   = [%s]\n", psCmd->szStartTime );
	printf( "\tEndTime     = [%s]\n", psCmd->szEndTime );
	printf( "\tSysType     = [%s]\n", psCmd->szSysType );
	printf( "\tRecType     = [%s]\n", psCmd->szRecType );
	printf( "\tSearchField = [%s]\n", psCmd->szSearchField );
	printf( "\tSearchCond  = [%s]\n", psCmd->szSearchCond );
	/* printf( "\tStoreMode   = [%d]\n", psCmd->nStoreMode ); */
	printf( "\t-- Query Command--\n\n" );

	return 0;
}

/**********************************************************
function: GetHelpInfo
description: 获取帮助信息
Input: argc: 参数数目
       argv: 参数值
Output:
Return:
others:
***********************************************************/

void GetHelpInfo( int argc, char * argv[] )
{
	extern char * optarg;
	int opt;

	/* 获取查询命令 */
	while( -1 != ( opt = getopt( argc, argv, "Vv" ) ) )
	{
		switch( opt )
		{
			case 'V':
			case 'v':
				fprintf( stderr, "\n\tiBOSS v1.8 qdetail\n\n" );
			/*
				fprintf( stderr, "\n\tQuery Detail Record Base On File System\n\n" );
				fprintf( stderr, "\tVERSION 1.0.0 (1 Sep, 2003)\n\n" );
        		fprintf( stderr, "\tCompiled at %s %s\n\n", __DATE__, __TIME__ );
        		fprintf( stderr, "\tCopyright Hold By SI-TECH Co.Ltd.\n\n" );
				break;
			*/
			default:
				break;
		}
	}

	exit(0);
}

/**********************************************************
function: GetCtrlFlag
description: 获取程序运行标志
Input: szCtrlFile: 控制文件绝对路径
Output:
Return: 程序运行标志
others:
***********************************************************/

int GetCtrlFlag( char * szCtrlFile, char * szFieldName )
{
	char szFieldValue[128];

	if( GetIniInfo( szCtrlFile, szFieldName, szFieldValue ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法获取程序运行控制标志\n" );
		return PROC_CTRL_EXIT;
	}

	return atoi( szFieldValue );
}

/********************************************************** 
function: ChkCmdInfo
description: 检验查询指令合法性
Input: psCmd: 描述查询指令各项内容的结构体
Output:
Return: 0 合法 1 不合法
others:
***********************************************************/

int ChkCmdInfo( PSEARCH_CMD psCmd )
{
	char szTime[14 + 1];

	GetCurrTime( szTime );
/*	
	if( !IsNum( psCmd->szSvcID ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的用户查询号码 %s \n", psCmd->szSvcID );
		return 1;
	}

	if( !IsStdTime( psCmd->szStartTime ) 
	|| strncmp( psCmd->szStartTime, szTime, min( strlen( psCmd->szStartTime ), strlen( szTime ) ) ) > 0 )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "查询开始时间 %s 格式错误\n", psCmd->szStartTime );
		return 1;
	}

	if( !IsStdTime( psCmd->szEndTime ) 
	|| strncmp( psCmd->szEndTime, szTime, min( strlen( psCmd->szEndTime), strlen( szTime ) ) ) > 0 )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "查询结束时间 %s 格式错误\n", psCmd->szEndTime );
		return 1;
	}
*/
/*
	if( strncmp( psCmd->szEndTime, szTime, strlen( psCmd->szEndTime ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, 
					"大于查询结束时间 %s 大于系统当前时间 %s 格式错误\n", 
										psCmd->szEndTime, szTime );
		return 1;
	}
	
	if( strcmp( psCmd->szStartTime, psCmd->szEndTime ) > 0 )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, 
					"查询开始时间 %s 大于查询结束时间 %s 格式错误\n", 
										psCmd->szStartTime, psCmd->szEndTime );
		return 1;
	}
*/

	if( STORE_MODE_MOBILE  != psCmd->nStoreMode
	&&  STORE_MODE_FIXED   != psCmd->nStoreMode
	&&  STORE_MODE_MODULUS != psCmd->nStoreMode )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单存储模式 %d 无效!\n", psCmd->nStoreMode );
		return 1;
	}

	return 0;
}

/********************************************************** 
function: GetCmdInfo
description: 获取查询指令信息
Input: psData: 描述系统全局结构的结构体
       szCmd: 查询指令
Output:
Return: 0 成功 1 失败
others:
***********************************************************/

int GetCmdInfo( PSEARCH_DATA psData, char * szCmd )
{
	PSEARCH_CMD psCmd;
	int nCount;
	char * szPos;
		
	psCmd = &(psData->sCmd);
	
	memset( psCmd, 0, sizeof( SEARCH_CMD ) );

	nCount = 0;
	szPos = NULL;

	psCmd->nStoreMode = STORE_MODE_MOBILE;
	
	szPos = strtok( szCmd, CMD_FIELD_SEP );
	while( NULL != szPos )
	{
		switch( nCount )
		{
			case 0:
				strcpy( psCmd->szSvcID, szPos );
				break;
			case 1:
				strcpy( psCmd->szStartTime, szPos );
				break;
			case 2:
				strcpy( psCmd->szEndTime, szPos );
				break;
			case 3:
				strcpy( psCmd->szSysType, szPos );
				break;
			case 4:
				strcpy( psCmd->szRecType, szPos );
				break;
			case 5:
				strcpy( psCmd->szSearchField, szPos );
				break;
			case 6:
				strcpy( psCmd->szSearchCond, szPos );
				break;
		}
			
		nCount++;
		szPos = strtok( NULL, CMD_FIELD_SEP );
	}

	DebugOutCmd( psCmd );
	
	/* 参数有效性检测 */
	if( nCount == 6 )
	{
		*(psCmd->szSearchCond) = 0;
	}
	else if( nCount != 7 || ChkCmdInfo( psCmd ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "命令参数格式错误\n" );
		return MY_FAILED;
	}

	return MY_SUCCEED;
}

/**********************************************************
function: main
description: 主控函数
Input: argc: 参数数目
       argv: 参数值
Output:
Return:
others:
***********************************************************/

int main( int argc, char * argv[] )
{
	char szIniName[128];
	char szIP[32], szPort[32], szExCode[32];
	char szCtrlFile[256];
	char szCmd[2056];
	int nLoop;

	SEARCH_DATA	sData;
	DETAIL_DATA dData;

	if( argc > 1 )
	{
		GetHelpInfo( argc, argv );
	}

	memset( &sData, 0, sizeof( SEARCH_DATA ) );
	memset( &dData, 0, sizeof( DETAIL_DATA ) );
	
	strcpy( szIniName, argv[0] );
	strcat( szIniName, ".ini" );
	
	nDebugFlag = SHIELD_DEBUG_INFO;
	
	/* 获取配置文件数据 */
	if( GetIniInfo( szIniName, "LOGPATH" , szLogPath )
	||	GetIniInfo( szIniName, "CFGPATH" , szCfgPath )
	||	GetIniInfo( szIniName, "INDFILE" , szIndFile )
	||	GetIniInfo( szIniName, "KEYFILE" , szKeyFile )
	||	GetIniInfo( szIniName, "FMTCTRL" , szFmtCtrl )
	||	GetIniInfo( szIniName, "MAPFILE" , szMapFile )
	||	GetIniInfo( szIniName, "FIELDMAP", szFieldMapFile )
	||	GetIniInfo( szIniName, "CTRLFILE", szCtrlFile )

	||	GetIniInfo( szIniName, "PAGESIZE", sData.szPageSize )
	||	GetIniInfo( szIniName, "PKGSIZE" , sData.szPkgSize )

	||	GetIniInfo( szIniName, "IP", szIP )
	||	GetIniInfo( szIniName, "PORT", szPort )
	||	GetIniInfo( szIniName, "EXCODE", szExCode )

	||	GetIniInfo( szIniName, "CMD", szCmdInfo ) )
	{
		printf( "app\texit\t无法获取配置文件信息!\n\n" );
		exit(0);
	}

	if( ChkLogPath( szLogPath ) )
	{
		printf( "app\texit\t无效的日志文件路径!\n\n" );
		exit(0);
	}

	if( PROC_CTRL_RUN != GetCtrlFlag( szCtrlFile, PRCO_CTRL_FIELD ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "程序运行标志为禁止运行, 查询程序退出!\n" );
		exit(0);
	}
	
	if( atol( sData.szPageSize ) <= 0 || atol( sData.szPkgSize ) <= 0 )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "无效的缓冲区大小!\n\n" );
		exit(0);
	}

	BackGround( );

	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "启动详单查询系统!\n\n" );
	
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "连接通讯服务器!\n" );
	
	if( ConnAgent( szIP, szPort, szExCode ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "连接通讯服务器失败!\n\n" );
		exit(0);
	}

	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "初始化系统数据!\n" );
	
	if( MY_SUCCEED != GetSearchData( &sData ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "不能获取初始化数据!\n\n" );
		FreeSearchData( &sData );
		exit(0);
	}

	while( PROC_CTRL_RUN == GetCtrlFlag( szCtrlFile, PRCO_CTRL_FIELD ) )
	{
		memset( szCmd, 0, sizeof( szCmd ) );

		nDebugFlag = GetCtrlFlag( szCtrlFile, DEBUG_FLAG_FIELD );
		
		if( RecvMsg( szCmd ) )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "接受查询指令失败!\n\n" );
		#ifndef  __COMM_AGENT__
			break;
		#else
			continue;
		#endif	
		}
	#ifndef  __COMM_AGENT__
		strcpy( szCmd, szCmdInfo );
	#endif
		
		if( SHOW_DEGUG_INFO == nDebugFlag )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "查询指令为\n\t[%s]\n", szCmd );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "分解查询指令\n" );	
		}
	
		/* 分析查询指令, 提取查询关键字 */			
		if( GetCmdInfo( &sData, szCmd ) )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的查询指令!\n\n" );
			FreeDetailData( &dData );
			if( SendMsg( NULL, 0, 1, 0 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "应答失败!\n\n" );
			}
		#ifndef  __COMM_AGENT__
			break;
		#else
			continue;
		#endif	
		}
		
		if( GetSelField( &(dData.sInfo), (sData.sCmd).szSearchField ) )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的查询字段!\n\n" );
			FreeDetailData( &dData );
			if( SendMsg( NULL, 0, 1, 0 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "应答失败!\n\n" );
			}
		#ifndef  __COMM_AGENT__
			break;
		#else
			continue;
		#endif	
		}
		
		if( GetCondField( &dData, (sData.sCmd).szSearchCond ) )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的查询条件!\n\n" );
			FreeDetailData( &dData );
			if( SendMsg( NULL, 0, 1, 0 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "应答失败!\n\n" );
			}
		#ifndef  __COMM_AGENT__
			break;
		#else
			continue;
		#endif	
		}
		
		if( GetDetailInfo( &sData, &dData ) )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单查询失败!\n\n" );
			FreeDetailData( &dData );
			if( SendMsg( NULL, 0, 2, 0 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "应答失败!\n\n" );
			}
		#ifndef  __COMM_AGENT__
			break;
		#else
			continue;
		#endif	
		}

		FreeDetailData( &dData );
	#ifndef  __COMM_AGENT__
		break;
	#endif
	}
	
	FreeSearchData( &sData );
	
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "详单查询系统运行结束!\n\n" );

	return 0;
}
