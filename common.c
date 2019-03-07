
/********************************************************************
copyright (C), 1995-2005, Si-Tech Information Technology Ltd. 
file_name: common.c 
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 实现公用函数
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ... 
********************************************************************/


#include "common.h"


/**********************************************************
function: GetCurrTime
description: 获取当前系统时间
Input: szStr: 保存时间的字符串
Output:
Return: 0: 成功
others:
***********************************************************/

int GetCurrTime( char * szStr )
{
	time_t tTime;
	struct tm * tmTime;
	
	time(&tTime);
	tmTime = localtime(&tTime);

	sprintf(szStr, "%04d%02d%02d%02d%02d%02d",
		tmTime->tm_year + 1900, tmTime->tm_mon + 1, 
		tmTime->tm_mday, tmTime->tm_hour, 
		tmTime->tm_min, tmTime->tm_sec);

	return MY_SUCCEED;
}

/**********************************************************
function: MyLog
description: 写日志信息
Input: szSource: 信息来源
       szLevel: 信息级别
       szFmt: 信息格式
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int MyLog( char * szSource, char * szLevel, char * szFmt, ... )
{
	va_list vaList;
	char szBuf[2048];	
	char szTime[14 + 1];
	FILE * fp;

	memset( szBuf, 0, sizeof( szBuf ) );
	
	GetCurrTime( szTime );
	sprintf( szBuf, "%s/wqd%8.8s.log", szLogPath, szTime );

	fp = fopen( szBuf, "a" );
	if( NULL == fp )
	{
		fprintf( stderr, "%4.4s/%2.2s/%2.2s %2.2s:%2.2s:%2.2s\t%s\t%s\t%s\n", 
					szTime,
					szTime + 4,
					szTime + 6,
					szTime + 8, 
					szTime + 10, 
					szTime + 12,
					"sys",
					"exit",
					"不能打开日志文件" );
		exit(0);
	}

	memset( szBuf, 0, sizeof( szBuf ) );
	
	sprintf( szBuf, "%4.4s/%2.2s/%2.2s %2.2s:%2.2s:%2.2s\t%s\t%s\t", 
					szTime,
					szTime + 4,
					szTime + 6,
					szTime + 8, 
					szTime + 10, 
					szTime + 12,
					szSource,
					szLevel );

	fprintf( fp, "%s", szBuf );
	printf( szBuf );

	va_start( vaList, szFmt );

	vfprintf( fp, szFmt, vaList );
	vsprintf( szBuf, szFmt, vaList );
	printf( szBuf );	

	va_end( vaList );

	if( 0 == strcmp( szLevel, INFO_LEVEL_EXIT ) )
	{
		memset( szBuf, 0, sizeof( szBuf ) );
		
		sprintf( szBuf, "%4.4s/%2.2s/%2.2s %2.2s:%2.2s:%2.2s\t%s\t%s\t%s\n", 
					szTime,
					szTime + 4,
					szTime + 6,
					szTime + 8, 
					szTime + 10, 
					szTime + 12,
					"sys",
					"exit",
					"发生异常情况, 程序停止运行, 退出!" );
		
		fprintf( fp, "%s", szBuf );
		printf( szBuf );
		
		fclose( fp );
		exit(0);
	}
	
	fclose( fp );
	
	return MY_SUCCEED;
}

/**********************************************************
function: TrimLeft
description: 去字符串左端空格
Input: szTrim: 待处理字符串
Output:
Return: szTrim: 处理后的字符串
others:
***********************************************************/

char * TrimLeft( char * szTrim )
{
	char * pLoc;
	int nLen;

	if( NULL == szTrim || 0 == *( szTrim ) )
	{ 
		return NULL;
	}

	nLen = strlen( szTrim );
	if ( nLen <= 0 )
	{
		return szTrim;
	}

	pLoc = szTrim;

	while( isspace( (int)*pLoc ) ) pLoc++;

	strcpy( szTrim, pLoc );

	return szTrim;
}

/**********************************************************
function: TrimRight
description: 去字符串右端空格
Input: szTrim: 待处理字符串
Output:
Return: szTrim: 处理后的字符串
others:
***********************************************************/

char * TrimRight( char * szTrim )
{
	int nLen;

	if( NULL == szTrim || 0 == *( szTrim ) )
	{ 
		return NULL;
	}
	
	nLen = strlen( szTrim );
	if ( nLen <= 0 )
	{
		return szTrim;
	}

	while( nLen > 0 && isspace( (int)szTrim[ nLen - 1 ] ) ) nLen--;

	szTrim[nLen] = '\0';

	return szTrim;
}

/**********************************************************
function: TrimLZero
description: 去字符串左端0字符
Input: szTrim: 待处理字符串
Output:
Return: szTrim: 处理后的字符串
others:
***********************************************************/

char * TrimLZero( char * szTrim )
{
	char * pLoc;

	pLoc = szTrim;

	while( (*pLoc) == '0' ) pLoc++;

	if( 0 == *(pLoc) || 0 == strlen( pLoc ) )
	{
		strcpy( szTrim, "0" );
	}
	else
	{
		strcpy( szTrim, pLoc );
	}	

	return szTrim;
}

/**********************************************************
function: GetIniInfo
description: 获取配置文件某一字段取值
Input: szFileName: 配置文件名
	   szLValue: 字段名
	   szRValue: 字段值
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetIniInfo( char * szFileName, char * szLValue, char * szRValue )
{
	FILE * fp;
	char szRec[2056], szVal[2056];
	char * szPos;
	
	fp = fopen( szFileName, "r" );
	if( NULL == fp )
	{
		printf( "Can't Open Ini File\n" );
		return MY_FAILED;
	}
	
	while( NULL != fgets( szRec, 2056, fp ) )
	{
		szPos = NULL;
		if( NULL != ( szPos = (char *) strchr( szRec, '=' ) ) )
		{

			memcpy( szVal, szRec, szPos - szRec );
			szVal[szPos - szRec] = 0;
			TrimLeft( TrimRight( szVal ) );
			if( strcmp( szVal, szLValue ) == 0 )
			{
				strcpy( szRValue, szPos + 1 );
				TrimLeft( TrimRight( szRValue ) );
				fclose( fp );
				return MY_SUCCEED;
			}
			else
			{
				continue;
			}
		}
	}
	
	fclose( fp );

	return MY_FAILED;
}

/**********************************************************
function: IsNum
description: 判断字符串值是否为数值
Input: szNum: 字符串
Output:
Return: MY_TRUE: 是 MY_FALSE: 不是
others:
***********************************************************/

int IsNum( char * szNum )
{
	if ( 0 == *(szNum) || 0 == strlen( szNum ) )
	{
		return MY_FALSE;
	}

	while ( *szNum )
	{
		if ( isdigit( (int)(*szNum) ) )
		{
			szNum++;
		}
		else
		{
			return MY_FALSE;
		}
	}

	return MY_TRUE;
}

/**********************************************************
function: IsLeapYear
description: 判断是否为闰年
Input: nYear: 年
Output:
Return: MY_TRUE: 是 MY_FALSE: 不是
others:
***********************************************************/

int IsLeapYear( int nYear )
{
	if( ( nYear % 4 == 0 && nYear % 100 != 0 ) || nYear % 400 == 0 )
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/**********************************************************
function: IsRecordLine
description: 判断记录是否为有效记录
Input: szRecord: 记录内容
Output: 
Return: MY_TRUE: 是 MY_FALSE: 否
others:
***********************************************************/

int IsRecordLine( char * szRecord )
{
	if( 0 == strncmp( szRecord, FILE_NOTE_FLAG, strlen( FILE_NOTE_FLAG ) ) )
	{
		return MY_FALSE;;
	}
		
	TrimLeft( TrimRight( szRecord ) );
		
	if( 0 == *( szRecord ) )
	{
		return MY_FALSE;;
	}

	return MY_TRUE;
}

/**********************************************************
function: GetFileCount
description: 获取文件记录数
Input: szPath: 文件路径
Output:
Return: lCount: 记录数
others:
***********************************************************/

long GetFileCount( char * szPath )
{
	FILE * fp;
	int lCount;
	char szBuf[1024 + 1];
	
	fp = fopen( szPath, "r" );
	
	if( NULL == fp )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "打开文件 %s 失败!\n", szPath );
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "ErrorInfo: %d : %s !\n", errno, strerror( errno ) );
		return -1;
	}
	
	lCount = 0;
	
	while( NULL != fgets( szBuf, sizeof( szBuf ), fp ) )
	{
		if( 0 != strncmp( szBuf, FILE_NOTE_FLAG, strlen( FILE_NOTE_FLAG ) ) )
		{
			lCount++;
		}
	}
	
	fclose( fp );
	
	return lCount;
}

/**********************************************************
function: GetLastDay
description: 获取参照时间的上一天的时间
Input: szLastTime: 上一天
       szCurrTime: 参照时间
Output:
Return: 0: 成功
others:
***********************************************************/

int GetLastDay( char * szLastTime, char * szCurrTime )
{
	time_t tTime;
	struct tm * tmTime;
	struct tm otmTime;
	char szBuf[128];
	char szTime[14 + 1];
	int nLen;
	int nYear, nMon, nDay, nHour, nMin, nSec;

	memset( &otmTime, 0, sizeof( struct tm ) );	
	
	nLen = strlen( szCurrTime );
	
	if( nLen > 12 )
	{
		memcpy( szBuf, szCurrTime + 12, 2 ); szBuf[2] = 0;
		otmTime.tm_sec = atoi( szBuf );
	}
	else
	{
		otmTime.tm_sec = 0;
	}
	
	if( nLen > 10 )
	{
		memcpy( szBuf, szCurrTime + 10, 2 ); szBuf[2] = 0;
		otmTime.tm_min = atoi( szBuf );
	}
	else
	{
		otmTime.tm_min = 0;
	}
	
	if( nLen > 8 )
	{
		memcpy( szBuf, szCurrTime + 8, 2 ); szBuf[2] = 0;
		otmTime.tm_hour = atoi( szBuf );
	}
	else
	{
		otmTime.tm_hour = 0;
	}
	
	if( nLen > 6 )
	{
		memcpy( szBuf, szCurrTime + 6, 2 ); szBuf[2] = 0;
		otmTime.tm_mday = atoi( szBuf );
	}
	else
	{
		otmTime.tm_mday = 1;
	}
	
	if( nLen > 4 )
	{
		memcpy( szBuf, szCurrTime + 4, 2 ); szBuf[2] = 0;
		otmTime.tm_mon = atoi( szBuf ) - 1;
	}
	
	memcpy( szBuf, szCurrTime, 4 ); szBuf[4] = 0;
	otmTime.tm_year = atoi( szBuf ) - 1900;

	tTime =  mktime( &otmTime );
	/* tTime -= 86400; */
	tTime -= 1;
	tmTime = localtime( &tTime );

	sprintf( szTime, "%04d%02d%02d%02d%02d%02d",
							tmTime->tm_year + 1900, 
							tmTime->tm_mon + 1, 
							tmTime->tm_mday, 
							tmTime->tm_hour, 
							tmTime->tm_min, 
							tmTime->tm_sec);
	
	strcpy( szLastTime, szTime );

	return MY_SUCCEED;
}

/**********************************************************
function: GetNextDay
description: 获取参照时间的下一天的时间
Input: szNextDay: 下一天
       szCurrTime: 参照时间
Output:
Return: 0: 成功
others:
***********************************************************/

int GetNextDay( char * szNextDay, char * szCurrDate )
{
	time_t tTime;
	struct tm * tmTime;
	struct tm otmTime;
	char szBuf[128];

	memset( &otmTime, 0, sizeof( struct tm ) );

	memcpy( szBuf, szCurrDate + 6, 2 ); szBuf[2] = 0;
	otmTime.tm_mday = atoi( szBuf );
	
	memcpy( szBuf, szCurrDate + 4, 2 ); szBuf[2] = 0;
	otmTime.tm_mon = atoi( szBuf ) - 1;
	
	memcpy( szBuf, szCurrDate, 4 ); szBuf[4] = 0;
	otmTime.tm_year = atoi( szBuf ) - 1900;

	tTime =  mktime( &otmTime );
	tTime += 86400;
	tmTime = localtime( &tTime );

	sprintf( szNextDay, "%04d%02d%02d",
							tmTime->tm_year + 1900, 
							tmTime->tm_mon + 1, 
							tmTime->tm_mday );

	return MY_SUCCEED;
}

/**********************************************************
function: GetNextMonth
description: 获取参照时间的下一个月的时间
Input: szNextMonth: 下一个月
       szCurrTime: 参照时间
Output:
Return: 0: 成功
others:
***********************************************************/

int GetNextMonth( char * szNextMonth, char * szCurrDate )
{
	int nYear, nMon, nLastDay;
	char szYear[DATE_YEAR_LEN + 1];
	char szMonth[DATE_MON_LEN + 1];

	strncpy( szYear, szCurrDate, DATE_YEAR_LEN );
	szYear[DATE_YEAR_LEN] = 0;
	nYear = atoi( szYear );
	strncpy( szMonth, szCurrDate + DATE_YEAR_LEN, DATE_MON_LEN );
	szMonth[DATE_MON_LEN] = 0;
	nMon = atoi( szMonth );
	
	nMon++;
	
	if( nMon > 12 )
	{
		nMon = 1;
		nYear++;
	}
	
	sprintf( szNextMonth, "%04d%02d", nYear, nMon );
	
	return MY_SUCCEED;
}

/**********************************************************
function: GetMonthLastDay
description: 获取某一月的最后一天
Input: szLastDay: 最后一天
       szTime: 参照时间
Output:
Return: 0: 成功
others:
***********************************************************/

int GetMonthLastDay( char * szLastDay, char * szTime )
{
	int nYear, nMon, nLastDay;
	char szYear[DATE_YEAR_LEN + 1];
	char szMonth[DATE_MON_LEN + 1];

	strncpy( szYear, szTime, DATE_YEAR_LEN );
	szYear[DATE_YEAR_LEN] = 0;
	nYear = atoi( szYear );
	strncpy( szMonth, szTime + DATE_YEAR_LEN, DATE_MON_LEN );
	szMonth[DATE_MON_LEN] = 0;
	nMon = atoi( szMonth );
	
	if( nMon == 4 || nMon == 6 || nMon == 9 || nMon == 11 )
	{
		nLastDay = 30;
	}
	else if( nMon == 2 )
	{
		if( IsLeapYear( nYear ) )
		{
			nLastDay = 29;
		}
		else
		{
			nLastDay = 28;
		}
	}
	else
	{
		nLastDay = 31;
	}
	
	sprintf( szLastDay, "%04d%02d%02d", nYear, nMon, nLastDay );
	
	return MY_SUCCEED;
}

/**********************************************************
function: FileIsExist
description: 判断文件是否存在
Input: szFile: 文件名
Output:
Return: MY_TRUE: 存在 MY_FALSE: 不存在
others:
***********************************************************/

int FileIsExist( char * szFile )
{
	if( 0 == access( szFile, F_OK ) )
	{
		return MY_TRUE;
	}
	else
	{
		return MY_FALSE;
	}
}

/**********************************************************
function: GetFileSize
description: 获取文件的大小
Input: szFile: 文件名
Output:
Return: 文件大小
others:
***********************************************************/

long GetFileSize( char * szFile )
{
	struct stat statBuf;
	
	lstat( szFile, &statBuf );
	
	return ( statBuf.st_size );
}

/**********************************************************
function: IsDir
description: 判断文件是否为目录文件
Input: szDirName: 文件名
Output:
Return: MY_TRUE: 是 MY_FALSE: 不是
others:
***********************************************************/

int IsDir( char * szDirName )
{
	struct stat statMode;

	if ( (lstat( szDirName, &statMode ) == 0) && (statMode.st_mode & S_IFDIR) )
	{
		return (MY_TRUE);
	}
	else
	{
		return (MY_FALSE);
	}
}

/**********************************************************
function: ChkLogPath
description: 检验日志文件路径有效性
Input: szPath: 日志文件目录
Output:
Return: 0: 有效 1: 无效
others:
***********************************************************/

int ChkLogPath( char * szPath )
{
	FILE * fp;
	char szBuf[256];
	char szTime[14 + 1];
	
	if( !IsDir( szPath ) )
	{
		return MY_FAILED;
	}

	GetCurrTime( szTime );
	sprintf( szBuf, "%s/wqd%8.8s.log", szLogPath, szTime );
	
	fp = fopen( szBuf, "a" );
	if( NULL == fp ) 
	{
		return MY_FAILED;
	}
	
	fclose( fp );
	
	return MY_SUCCEED;
}

/**********************************************************
function: BackGround
description: 程序转入后台运行
Input: 
Output:
Return: 0: 成功, 失败则程序退出
others:
***********************************************************/

int BackGround( void )
{
	pid_t pid;

	pid = fork();
	
	if ( 0 > pid )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "程序转入后台失败\n" );
		exit( 1 );
	}
	else if ( pid > 0 )
	{
		exit( 0 );
	}
	else
	{
		setsid( );
		chdir( "/" );
		umask( 0 );
	}

	return MY_SUCCEED;
}

/**********************************************************
function: TransTime
description: 时间格式转换(测试用)
Input: szTime: 转换的时间
Output:
Return: 0: 成功
others:
***********************************************************/

int TransTime( char * szTime )
{
	char szYear[4 + 1], szMon[3 + 1], szDay[2 + 1];
	char szHour[2 + 1], szMin[2 + 1], szSec[2 + 1];
	char szTimeFmt[2 + 1];
	int nYear, nMon, nDay, nHour, nMin, nSec;
	
	strncpy( szYear, szTime + 7, 4 ); szYear[4] = 0;
	strncpy( szMon, szTime, 3 ); szMon[3] = 0;
	strncpy( szDay, szTime + 4, 2 ); szDay[2] = 0;
	strncpy( szHour, szTime + 12, 2 ); szHour[2] = 0; TrimLeft( szHour );
	strncpy( szMin, szTime + 15, 2 ); szMin[2] = 0;
	strncpy( szSec, szTime + 18, 2 ); szSec[2] = 0;
	strncpy( szTimeFmt, szTime + 24, 2 ); szTimeFmt[2] = 0;
	
	nYear = atoi( szYear );
	nDay = atoi( szDay );
	nHour = atoi( szHour );
	nMin = atoi( szMin );
	nSec = atoi( szSec );
	
	if( strcmp( szTimeFmt, "PM" ) == 0 )
	{
		nHour = atoi( szHour ) + 12;
	}
	
	if( strcmp( szMon, "Jul" ) == 0 )
	{
		nMon = 7;
	}
	
	sprintf( szTime, "%04d%02d%02d%02d%02d%02d", 
				nYear, nMon, nDay, nHour, nMin, nSec );

	return MY_SUCCEED;
}

/**********************************************************
function: IsMobileHead
description: 判断号码是否为移动号码
Input: szHead: 号码头
Output:
Return: MY_TRUE: 是 MY_FALSE: 不是
others:
***********************************************************/

int IsMobileHead( char * szHead )
{
	if( !IsNum( szHead ) )
	{
		return MY_FALSE;
	}
	
	if( 7 != strlen( szHead ) )
	{
		return MY_FALSE;
	}

	return MY_TRUE;
}

int IsMobileUser( char * szUser )
{
	if( !IsNum( szUser ) )
	{
		return MY_FALSE;
	}
	
	if( 11 != strlen( szUser ) )
	{
		return MY_FALSE;
	}
	
	return MY_TRUE;
}

int TransDealTime( char * szDealTime, char * szCallTime )
{
    char szCode[7][1 + 1];
    int nLoop;

    for( nLoop = 0; nLoop < 7; nLoop++ )
    {
        memset( szCode[nLoop], 0, sizeof( szCode[nLoop] ) );

        strncpy( szCode[nLoop], szDealTime + nLoop, 1 );
        szCode[nLoop][1] = 0;
    }

    sprintf( szCallTime, "%02d%02d%02d%02d%02d%02d%02d",
                                szCode[0][0] - '0',
                                szCode[1][0] - '0',
                                szCode[2][0] - '0',
                                szCode[3][0] - '0',
                                szCode[4][0] - '0',
                                szCode[5][0] - '0',
                                szCode[6][0] - '0' );
    szCallTime[14] = 0;

    return 0;
}
