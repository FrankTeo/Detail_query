
/********************************************************************
copyright (C), 1995-2005, Si-Tech Information Technology Ltd. 
file_name: qdetail.c 
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 实现查询功能
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ... 
********************************************************************/


#include "qdetail.h"

int RecIntoPkg( char * szRecord, PDETAIL_DATA pdData, PSEARCH_DATA psData )
{
	pdData->lTotalRec++;
	pdData->lPageNum++;

	GetSelValue( szRecord, psData, pdData );

	if( ( pdData->lPageNum > 0 && ( pdData->lPageNum % atoi( psData->szPageSize ) ) == 0 ) 
	||  pdData->lOffset + strlen( pdData->szResult ) > atol( psData->szPkgSize ) )
	{
		/*MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "返回结果包满, 发送包中数据\n" );*/
		if( 0 == pdData->nPkgNum )
		{
			if( SendMsg( psData->szPkgBuff, strlen( psData->szPkgBuff ), 0, 1 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "发送查询结果信息失败\n" );
			}
			pdData->nPkgNum++;
		}
		else
		{
			if( SendMsg( psData->szPkgBuff, strlen( psData->szPkgBuff ), 0, 2 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "发送查询结果信息失败\n" );
			}
			pdData->lOffset++;
		}
		
		memset( psData->szPkgBuff, 0, atol( psData->szPkgSize ) + 1 );
		pdData->lOffset = 0;
	}

	strcat( psData->szPkgBuff, pdData->szResult );
	pdData->lOffset += strlen( pdData->szResult );

	return MY_SUCCEED;
}

/********************************************************** 
function: GetArchIndex
description: 获取索引信息
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetArchIndex( PDETAIL_DATA pdData )
{
	BASE_INDEX bIndex;
	char szPath[1024];
	int nBase;
	int nLoop;
	FILE * fp;
	
	memset( szPath, 0, sizeof( szPath ) );
	
	pdData->sIndex.ulDetailLen = 0;
	
	nBase = atoi( pdData->szSvcID + strlen( pdData->szSvcID ) - BASE_ADDR_LEN );
	sprintf( szPath, "%s/%s/%s.%s%s", pdData->szFilePath, INDEX_DIR_NAME, pdData->szFileHead, pdData->szQueryMonth, INDEX_FILE_TAIL );
	
	fp = fopen( szPath, "r" );
	if( NULL == fp )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法打开归档详单索引文件 %s\n", szPath );
		return MY_FAILED;;
	}
	
	if( 0 != fseek( fp, nBase * sizeof( BASE_INDEX ), SEEK_SET ) )
	{
		fclose( fp );
		return MY_FAILED;;
	}
	
	memset( &bIndex, 0, sizeof( BASE_INDEX ) );
	
	if( 1 != fread( &bIndex, sizeof( BASE_INDEX ), 1, fp ) )
	{
		fclose( fp );
		return MY_FAILED;;
	}
	
	if( 0 != fseek( fp, bIndex.ulOffset, SEEK_SET ) )
	{
		fclose( fp );
		return MY_FAILED;;
	}
	
	for( nLoop = 0; nLoop < bIndex.nService; nLoop++ )
	{
/*
		if( 0 != fseek( fp, bIndex.ulOffset + nLoop * sizeof( SERVICE_INDEX ), SEEK_SET ) )
		{
			fclose( fp );
			return MY_FAILED;;
		}
*/
		memset( &(pdData->sIndex), 0, sizeof( SERVICE_INDEX ) );

		if( 1 != fread( &(pdData->sIndex), sizeof( SERVICE_INDEX ), 1, fp ) )
		{
			fclose( fp );
			return MY_FAILED;;
		}
		
		if( 0 == strcmp( pdData->szSysType, pdData->sIndex.szSysType ) )
		{
			break;
		}
	}
	
	if( nLoop >= bIndex.nService )
	{
		pdData->sIndex.ulDetailLen = 0;
		fclose( fp );
		return MY_SUCCEED;
	}
	
	fclose( fp );	
/*
	printf( "%04d, %02d, %s %6ld %6ld %6ld\n", 
											pdData->sIndex.nUserNo, pdData->sIndex.nDay, pdData->sIndex.szSysType, 
											pdData->sIndex.ulDetailOff , pdData->sIndex.ulDetailLen , pdData->sIndex.ulOriginLen );
*/
	return MY_SUCCEED;
}

int UnCompress( Byte * szCompr, uLong lComprLen, Byte * szUncompr, uLong lUncomprLen )
{
    int nErr;

    nErr = uncompress( szUncompr, &lUncomprLen, szCompr, lComprLen );
    if( Z_OK != nErr ) 
    {
    	MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "解压缩失败, eroro: %d\n" , nErr );
		return MY_FAILED;
    }

/*	printf("[%s]\n", szUncompr);*/

	return MY_SUCCEED;
}

/********************************************************** 
function: GetArchBlock
description: 获取压缩数据块内容
Input: pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetArchBlock( char * pBlock, PDETAIL_DATA pdData )
{
	PSERVICE_INDEX psIndex;
	char * szBlock;
	long lBufLen;
	
	psIndex = &(pdData->sIndex);
	
	fseek( pdData->fpCurr, psIndex->ulDetailOff, SEEK_SET );

	szBlock = ( char * ) malloc( psIndex->ulDetailLen + 1 );
	if( NULL == szBlock )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
       	return MY_FAILED;;
	}
	memset( szBlock, 0, psIndex->ulDetailLen + 1 );

	if( psIndex->ulDetailLen != fread( szBlock, sizeof( char ), psIndex->ulDetailLen, pdData->fpCurr ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "归档压缩文件读失败\n" );
		return MY_FAILED;;
	}
	
	lBufLen = psIndex->ulOriginLen + 1 ;
		
	if( MY_SUCCEED != UnCompress( (Byte *) szBlock, psIndex->ulDetailLen, (Byte *) pBlock, lBufLen ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "解压缩归档详单文件失败\n" );
		if( NULL != szBlock )
		{
			free( szBlock );
			szBlock = NULL;
		}
		
		return MY_FAILED;;
	}

	if( strlen( pBlock ) != psIndex->ulOriginLen )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "归档详单文件解压缩后尺寸与压缩前不符\n" );
		if( NULL != szBlock )
		{
			free( szBlock );
			szBlock = NULL;
		}
		
		return MY_FAILED;;
	}
		
	if( NULL != szBlock )
	{
		free( szBlock );
		szBlock = NULL;
	}
		
	return MY_SUCCEED;
}

/********************************************************** 
function: GetArchRec
description: 检索归档详单存储文件详单记录
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetArchRec( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	PSERVICE_INDEX psIndex;
	char szRecord[REC_SIZE + 1];
	char * szBlock;
	char * szEndPos;
	long lCurrPos;
	
	if( MY_SUCCEED != GetArchIndex( pdData ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "不能获取归档文件索引信息\n" );
		return MY_FAILED;
	}

	if( 0 >= pdData->sIndex.ulDetailLen )
	{
		return MY_SUCCEED;
	}
	
	psIndex = &(pdData->sIndex);
	
	if( psIndex->ulDetailLen <= 0 )
	{
		return MY_SUCCEED;
	}
	
	szBlock = ( char * ) malloc( psIndex->ulOriginLen + 1  );
	if( NULL == szBlock )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请保存归档压缩详单记录的内存空间失败\n" );
		return MY_FAILED;;
	}
	memset( szBlock, 0, psIndex->ulOriginLen + 1 );
		
	/* 解压缩归档文件, 获取用户详单 */
	if( MY_SUCCEED != GetArchBlock( szBlock, pdData ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "解压缩归档详单记录失败\n" );
		return MY_FAILED;;
	}

	/* 定位详单记录 */	
	lCurrPos = 0;
	
	while( lCurrPos < psIndex->ulOriginLen /*&& FLLL_CHAR != *(szBlock + lCurrPos)*/ )
	{
		szEndPos = NULL;
		memset( szRecord, 0, sizeof( szRecord ) );
		
		szEndPos = ( char * ) strchr( szBlock + lCurrPos, END_FLAG );
		if( NULL == szEndPos )
		{
			break;
		}
		
		memcpy( szRecord, szBlock + lCurrPos, szEndPos - ( szBlock + lCurrPos ) );
		
		lCurrPos = szEndPos - szBlock + 1;

		/* 判断是否符合查询条件, 未进行Trim */
		if( IsQueryDetail( szRecord, pdData ) )
		{
			RecIntoPkg( szRecord, pdData, psData );
		}
	}
		
	if( NULL != szBlock )
	{
		free( szBlock );
		szBlock = NULL;
	}
	
	return MY_SUCCEED;
}

/********************************************************** 
function: GetArchDetail
description: 检索归档详单文件
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetArchDetail( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	char szPath[512];

	memset( pdData->szCurrFile, 0, sizeof( pdData->szCurrFile ) );
	memset( pdData->szQueryDate, 0, sizeof( pdData->szQueryDate ) );
			
	sprintf( pdData->szCurrFile, "%s.%s%s", pdData->szFileHead, pdData->szQueryMonth, MONTH_FILE_TAIL );	
	sprintf( szPath, "%s/%s/%s", pdData->szFilePath, ARCH_DIR_NAME, pdData->szCurrFile );

	strcpy( pdData->szQueryDate, pdData->szQueryMonth );
	
	if( NULL != ( pdData->fpCurr = fopen( szPath, "r" ) ) )
	{
		if( MY_SUCCEED != GetArchRec( psData, pdData ) )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "检索归档详单文件 %s 失败\n", pdData->szCurrFile );
			return MY_FAILED;
		}
		
		if( NULL != pdData->fpCurr )
		{		
			fclose( pdData->fpCurr );
			pdData->fpCurr = NULL;
		}	
	}
	else
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "归档详单文件 %s 打开失败\n", pdData->szCurrFile );
		return MY_FAILED;
	}
	
	return MY_SUCCEED;
}

/********************************************************** 
function: GetUserIndex
description: 获取用户索引信息
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetUserIndex( PDETAIL_DATA pdData )
{
	BASE_INDEX bIndex;
	char szPath[512];
	int nBase, nLoop;
	FILE * fp;
	
	memset( &(pdData->sIndex), 0, sizeof( SERVICE_INDEX ) );
	
	pdData->sIndex.ulDetailLen = 0;
	
	nBase = atoi( pdData->szSvcID + strlen( pdData->szSvcID ) - BASE_ADDR_LEN );
	sprintf( szPath, "%s/%s/%s.%s%s", pdData->szFilePath, INDEX_DIR_NAME, pdData->szFileHead, pdData->szQueryDate, INDEX_FILE_TAIL );
	
	fp = fopen( szPath, "r" );
	if( NULL == fp )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法打开用户详单索引文件 %s\n", szPath );
		return MY_FAILED;;
	}

	if( 0 != fseek( fp, nBase * sizeof( BASE_INDEX ), SEEK_SET ) )
	{
		fclose( fp );
		return MY_FAILED;;
	}
	
	memset( &bIndex, 0, sizeof( BASE_INDEX ) );
	
	if( 1 != fread( &bIndex, sizeof( BASE_INDEX ), 1, fp ) )
	{
		fclose( fp );
		return MY_FAILED;;
	}
	
	if( 0 >= bIndex.nService )
	{
		fclose( fp );
		return MY_SUCCEED;
	}
	
	if( 0 != fseek( fp, bIndex.ulOffset, SEEK_SET ) )
	{
		fclose( fp );
		return MY_FAILED;;
	}
	
	for( nLoop = 0; nLoop < bIndex.nService; nLoop++ )
	{
/*
		if( 0 != fseek( fp, bIndex.ulOffset + nLoop * sizeof( SERVICE_INDEX ), SEEK_SET ) )
		{
			fclose( fp );
			return MY_FAILED;;
		}
*/
		memset( &(pdData->sIndex), 0, sizeof( SERVICE_INDEX ) );
	
		if( 1 != fread( &(pdData->sIndex), sizeof( SERVICE_INDEX ), 1, fp ) )
		{
			fclose( fp );
			return MY_FAILED;;
		}
		
		if( 0 == strcmp( pdData->szSysType, pdData->sIndex.szSysType ) )
		{
			break;
		}
	}
	
	if( nLoop >= bIndex.nService )
	{
		pdData->sIndex.ulDetailLen = 0;
		fclose( fp );
		return MY_SUCCEED;
	}
	
	fclose( fp );	
/*
	printf( "%04d, %02d, %s %6ld %6ld %6ld\n", 
											pdData->sIndex.nUserNo, pdData->sIndex.nDay, pdData->sIndex.szSysType, 
											pdData->sIndex.ulDetailOff , pdData->sIndex.ulDetailLen , pdData->sIndex.ulOriginLen );
*/
	return MY_SUCCEED;
}

/********************************************************** 
function: GetCommonRec
description: 检索普通详单存储文件详单记录
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetCommonRec( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	char * szBlock;
	char szRecord[REC_SIZE + 1];
	char szBaseAddr[BASE_ADDR_LEN + 1];
	char * szEndPos;
	long lCurrPos;
	long lBaseAddr;
	
	if( MY_SUCCEED != GetUserIndex( pdData ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "不能获取普通文件索引信息\n" );
		return MY_FAILED;
	}
	
	if( 0 >= pdData->sIndex.ulDetailLen )
	{
		return MY_SUCCEED;
	}
	
	memset( szBaseAddr, 0, sizeof( szBaseAddr ) );
	
	/* 计算详单记录偏移 */
	if( BASE_ADDR_LEN > strlen( pdData->szSvcID ) )
	{
		strcpy( szBaseAddr, pdData->szSvcID );
	}
	else
	{
		strncpy( szBaseAddr, pdData->szSvcID + strlen( pdData->szSvcID ) - BASE_ADDR_LEN, BASE_ADDR_LEN );
		szBaseAddr[BASE_ADDR_LEN] = 0;
	}	

	lBaseAddr = pdData->sIndex.ulDetailOff;

	/* 获取该数据文件详单记录 */

	/* 定位详单记录 */
	fseek( pdData->fpCurr, lBaseAddr, SEEK_SET );

	lCurrPos = 0;

	szBlock = ( char * ) malloc( pdData->sIndex.ulDetailLen + 1 );
	if( NULL == szBlock )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
       	return MY_FAILED;;
	}
	memset( szBlock, 0, pdData->sIndex.ulDetailLen + 1 );
			
	if( pdData->sIndex.ulDetailLen != fread( szBlock, sizeof( char ), pdData->sIndex.ulDetailLen, pdData->fpCurr ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "普通详单存储文件数据块 %s 读失败\n", pdData->szCurrFile );
		return MY_FAILED;
	}

	/* 获取一条详单记录 */
	while( lCurrPos < pdData->sIndex.ulDetailLen /*&& FLLL_CHAR != *(szBlock + lCurrPos)*/ )
	{
		memset( szRecord, 0, sizeof( szRecord ) );
		szEndPos = NULL;
			
		szEndPos = ( char * ) strchr( szBlock + lCurrPos, END_FLAG );
		if( NULL == szEndPos )
		{
			break;
		}
			
		memcpy( szRecord, szBlock + lCurrPos, szEndPos - ( szBlock + lCurrPos ) );

		lCurrPos = szEndPos - szBlock + 1;

		/* 判断是否符合查询条件, 未进行Trim */
		if( IsQueryDetail( szRecord, pdData ) )
		{
			RecIntoPkg( szRecord, pdData, psData );
		}
	}

	if( NULL != szBlock )
	{
		free( szBlock );
		szBlock = NULL;
	}

	return MY_SUCCEED;
}

int GetCurrRec( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	char szRecord[REC_SIZE + 1];
	char szBuff[REC_SIZE + 1];
	char * szRecBuff;
	char * szEndPos;
	long lCurrPos;
	
	szRecBuff = ( char * ) malloc( pdData->lFileSize + 1 );
	if( NULL == szRecBuff )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "申请保存当日详单记录的内存空间失败\n" );
		return MY_FAILED;
	}
	/* memset( szRecBuff, 0, pdData->lFileSize + 1 ); */
	
	szRecBuff[pdData->lFileSize] = 0;
	
	memset( szBuff, 0, sizeof( szBuff ) );
	
	if( pdData->lFileSize != read( pdData->nCurrHandle, szRecBuff, pdData->lFileSize ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "读取当日详单记录失败\n" );
		return MY_FAILED;
	}
	
	lCurrPos = 0;
	
	while( lCurrPos < pdData->lFileSize )
	{
		memset( szRecord, 0, sizeof( szRecord ) );
		szEndPos = NULL;
			
		szEndPos = ( char * ) strchr( szRecBuff + lCurrPos, END_FLAG );
		if( NULL == szEndPos )
		{
			break;
		}
			
		memcpy( szRecord, szRecBuff + lCurrPos, szEndPos - ( szRecBuff + lCurrPos ) );
			
		lCurrPos = szEndPos - szRecBuff + 1;
		
		if( 0 == strncmp( szRecord, FILE_NAME_FLAG, strlen( FILE_NAME_FLAG ) ) )
		{
			continue;
		}
		
		/* 判断是否符合查询条件, 未进行Trim */
		if( IsQueryDetail( szRecord, pdData ) )
		{
			RecIntoPkg( szRecord, pdData, psData );
		}

		memset( szBuff, 0, sizeof( szBuff ) );
	}
	
	if( NULL != szRecBuff )
	{
		free( szRecBuff );
		szRecBuff = NULL;
	}
	
	return MY_SUCCEED;
}

/********************************************************** 
function: GetCommDetail
description: 检索普通详单文件
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/
	
int GetCommDetail( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	char szDay[8 + 1];
	char szLastDay[8 + 1];
	char szPath[512];
	int nDay;
	
	memset( szDay, 0, sizeof( szDay ) );
	memset( pdData->szQueryDate, 0, sizeof( pdData->szQueryDate ) );
	
	nDay = 1;

	sprintf( szDay, "%s%02d", pdData->szQueryMonth, nDay );
	/*GetMonthLastDay( szLastDay, pdData->szQueryMonth );*/
	sprintf( szLastDay, "%s31", pdData->szQueryMonth );
	
	while( 0 >= strncmp( szDay, pdData->szEndTime, 8 ) && 0 >= strncmp( szDay, szLastDay, 8 ) )
	{
		pdData->bCurrQuery = MY_FALSE;
		strcpy( pdData->szQueryDate, szDay );
		
		if( 0 <= strncmp( szDay, pdData->szStartTime, 8 ) )
		{
			memset( pdData->szCurrFile, 0, sizeof( pdData->szCurrFile ) );
			
			sprintf( pdData->szCurrFile, "%s.%s%s", pdData->szFileHead, szDay, DAY_FILE_TAIL );
			sprintf( szPath, "%s/%s/%s", pdData->szFilePath, COMM_DIR_NAME, pdData->szCurrFile );
		
			if( FileIsExist( szPath ) )
			{
				if( NULL != ( pdData->fpCurr = fopen( szPath, "r" ) ) )
				{
					/*pdData->lFileSize = GetFileSize( szPath );*/
					
					if( MY_SUCCEED != GetCommonRec( psData, pdData ) )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "检索普通详单文件 %s 失败\n", pdData->szCurrFile );
						return MY_FAILED;
					}

					if( NULL != pdData->fpCurr )
					{
						fclose( pdData->fpCurr );
						pdData->fpCurr = NULL;
					}	
				}
				else
				{
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "普通详单文件 %s 打开失败\n", pdData->szCurrFile );
					return MY_FAILED;
				}
			}
			else
			{
				pdData->bCurrQuery = MY_TRUE;
				
				memset( pdData->szCurrFile, 0, sizeof( pdData->szCurrFile ) );
			
				if( STORE_MODE_MODULUS == pdData->nStoreMode )
				{
					sprintf( pdData->szCurrFile, "%s.%s", pdData->szFileHead, szDay );
					sprintf( szPath, "%s/%6.6s/day%2.2s/%s", pdData->szFilePath, szDay, szDay + 6, pdData->szCurrFile );
				}
				else
				{
					sprintf( pdData->szCurrFile, "%s.%s", pdData->szFileHead, szDay );
					sprintf( szPath, "%s/%s/%s", pdData->szFilePath, CURR_DIR_NAME, pdData->szCurrFile );
				}	

				if( FileIsExist( szPath ) )
				{
					if( -1 != ( pdData->nCurrHandle = open( szPath, O_RDONLY ) ) )
					{
						pdData->lFileSize = GetFileSize( szPath );
						
						if( MY_SUCCEED != GetCurrRec( psData, pdData ) )
						{
							MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "检索当日详单文件 %s 失败\n", pdData->szCurrFile );
							return MY_FAILED;
						}
				
						if( -1 != pdData->nCurrHandle )
						{
							close( pdData->nCurrHandle );
							pdData->nCurrHandle = -1;
						}	
					}
					else
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "当日详单文件 %s 打开失败\n", pdData->szCurrFile );
						return MY_FAILED;
					}
				}			
				else
				{
				#ifdef	__DEBUG_INFO__
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "文件 %s 不存在\n", pdData->szCurrFile );
				#endif
				}	
			}
		}
		
		/* GetNextDay( szDay, szDay ); */
		nDay++;

		sprintf( szDay, "%s%02d", pdData->szQueryMonth, nDay );
	}
	
	return MY_SUCCEED;
}

int HasArchFile( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	char szFile[512];

	if( STORE_MODE_MODULUS == pdData->nStoreMode )
	{
		return MY_FALSE;
	}

	memset( szFile, 0, sizeof( szFile ) );
	
	sprintf( szFile, "%s/%s/%s.%s%s", pdData->szFilePath, ARCH_DIR_NAME, pdData->szFileHead, pdData->szQueryMonth, MONTH_FILE_TAIL );

	return ( FileIsExist( szFile ) );
}

int GetMonthDetail ( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	pdData->bCurrQuery = MY_FALSE;

	if( HasArchFile( psData, pdData ) )
	{
		if( MY_SUCCEED != GetArchDetail( psData, pdData ) )
		{
			return MY_FAILED;
		}
	}
	else
	{
		if( MY_SUCCEED != GetCommDetail( psData, pdData ) )
		{
			return MY_FAILED;
		}
	}
		
	return MY_SUCCEED;
}

/********************************************************** 
function: GetDetailRecord
description: 检索详单记录
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetDetailRecord( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	char szCurrTime[14 + 1];
	
	if( SHOW_DEGUG_INFO == nDebugFlag )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "检索用户 %s 从 %s 到 %s 之间的详单\n", 
					pdData->szSvcID, 
					pdData->szStartTime,
					pdData->szEndTime );
	}
	
	strcpy( pdData->szQueryMonth, pdData->szStartTime );
	pdData->szQueryMonth[DATE_YEAR_LEN + DATE_MON_LEN] = 0;

	memset( szCurrTime, 0, sizeof( szCurrTime ) );
	GetCurrTime( szCurrTime );
	
	while( strncmp( pdData->szQueryMonth, pdData->szEndTime, DATE_YEAR_LEN + DATE_MON_LEN ) <= 0
	&& strncmp( pdData->szQueryMonth, szCurrTime, DATE_YEAR_LEN + DATE_MON_LEN ) <= 0 )
	{
		if( MY_SUCCEED != GetMonthDetail( psData, pdData ) )
		{
			GetNextMonth( pdData->szQueryMonth, pdData->szQueryMonth );
			continue;
			/*return MY_FAILED;*/
		}
		
		GetNextMonth( pdData->szQueryMonth, pdData->szQueryMonth );

		if( NULL != pdData->fpCurr )
		{
			fclose( pdData->fpCurr );
			pdData->fpCurr = NULL;
		}
		
		if( -1 != pdData->nCurrHandle )
		{
			close( pdData->nCurrHandle );
			pdData->nCurrHandle = -1;
		}
	}

	return MY_SUCCEED;
}


int GetDetailData( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	char szPath[512];
	char szTime[14 + 1];
	
	if( 0 == *(pdData->szChgDate) 
	|| strncmp( pdData->szStartTime, pdData->szChgDate, strlen( pdData->szStartTime ) ) >= 0 )
	{
		if( MY_SUCCEED != GetDetailRecord( psData, pdData ) )
		{
			return MY_FAILED;
		}
	}
	else if( strncmp( pdData->szStartTime, pdData->szChgDate, strlen( pdData->szStartTime ) ) < 0 )
	{
		if( strncmp( pdData->szEndTime, pdData->szChgDate, strlen( pdData->szStartTime ) ) < 0 )
		{
			strcpy( pdData->szFilePath, pdData->szOldPath );
			if( MY_SUCCEED != GetDetailRecord( psData, pdData ) )
			{
				return MY_FAILED;
			}
		}
		else if( strncmp( pdData->szEndTime, pdData->szChgDate, strlen( pdData->szStartTime ) ) >= 0 )
		{
			/* 检索old文件系统的详单 */
			strcpy( szTime, pdData->szEndTime );
			GetLastDay( pdData->szEndTime, pdData->szChgDate );
			strcpy( szPath, pdData->szFilePath );
			strcpy( pdData->szFilePath, pdData->szOldPath );
			
			if( MY_SUCCEED != GetDetailRecord( psData, pdData ) )
			{
				return MY_FAILED;
			}
			
			strcpy( pdData->szFilePath, szPath );
			
			/* 检索当前文件系统的详单 */
			strcpy( pdData->szEndTime, szTime );
			strcpy( szTime, pdData->szStartTime );
			strcpy( pdData->szStartTime, pdData->szChgDate );

			if( MY_SUCCEED != GetDetailRecord( psData, pdData ) )
			{
				return MY_FAILED;
			}
			
			strcpy( pdData->szStartTime, szTime );
		}
	}
	
	return MY_SUCCEED;
}

/********************************************************** 
function: GetFilePath
description: 获取当前检索详单存储路径
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0 成功 1 失败
others:
***********************************************************/

int GetFilePath( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	PSEARCH_HEAD psHead;
	PSEARCH_INDEX psIndex;
	PSEARCH_CMD psCmd;
	PKEY_RECORD pkRecord;
	
	char szHeadFlag[HEAD_FLAG_LEN + 1], szLoc[5 + 1];
	
	int nCount, nLoop;
	
	psCmd    = &(psData->sCmd);
	pkRecord = &(pdData->kRec);
	
	if( STORE_MODE_MOBILE == psCmd->nStoreMode )
	{
		strncpy( szHeadFlag, pdData->szSvcID, HEAD_FLAG_LEN );
		szHeadFlag[HEAD_FLAG_LEN] = 0;
		
		strncpy( szLoc, pdData->szSvcID + HEAD_FLAG_LEN, 5 );
		szLoc[5] = 0;
		
		for( nCount = 0; nCount < MAX_MOBILEHEAD_NUM; nCount++ )
		{
			psHead = psData->psHead + nCount;
			
			if( 0 == strcmp( psHead->szHeadFlag, szHeadFlag ) )
			{
				break;
			}
		}
		
		if( nCount>= MAX_MOBILEHEAD_NUM )
		{
			return MY_FAILED;
		}
		
		if( NULL != psHead->psIndex[atoi( szLoc )] )
		{
			psIndex = psHead->psIndex[atoi( szLoc )];
			
			sprintf( pdData->szFilePath, "%s/%s/%s", 
								psIndex->szFileSys,
								psIndex->szSubDir,
								psIndex->szH1H2H3 );
			
			sprintf( pdData->szOldPath, "%s/%s/%s", 
								psIndex->szOldSys,
								psIndex->szOldSub,
								psIndex->szH1H2H3 );

			strcpy( pdData->szChgDate, psIndex->szChgDate );
			
			strncpy( pdData->szFileHead, pdData->szSvcID, FILE_HEAD_LEN );
			pdData->szFileHead[FILE_HEAD_LEN] = 0;
		}
		else
		{
			return MY_FAILED;
		}
	}
	else
	{	
		psIndex = psData->psiCard;
		
		while( NULL != psIndex )
		{
			if( atoi( psIndex->szStoreMode ) == psCmd->nStoreMode )
			{
				if( STORE_MODE_MODULUS == psData->sCmd.nStoreMode )
				{
					if( psIndex->nBeginKey <= psCmd->nDetachNo 
					&&  psIndex->nEndKey   >= psCmd->nDetachNo )
					{
						
						sprintf( pdData->szFilePath, "%s/%s", 
								psIndex->szFileSys,
								psIndex->szSubDir );
			
						sprintf( pdData->szOldPath, "%s/%s", 
								psIndex->szOldSys,
								psIndex->szOldSub );

						strcpy( pdData->szChgDate, psIndex->szChgDate );
						sprintf( pdData->szFileHead, "%s%0*d", psIndex->szKey, SVC_OFF_LEN, psCmd->nDetachNo );
						
						break;
					}
				}
				else
				{
					sprintf( pdData->szFilePath, "%s/%s/%s", 
								psIndex->szFileSys,
								psIndex->szSubDir,
								psIndex->szH1H2H3 );
			
					sprintf( pdData->szOldPath, "%s/%s/%s", 
								psIndex->szOldSys,
								psIndex->szOldSub,
								psIndex->szH1H2H3 );

					strcpy( pdData->szChgDate, psIndex->szChgDate );
					
					if( 0 != pkRecord->nDetachFlag )
					{
						sprintf( pdData->szFileHead, "%s.%s%d", FIXED_FILE_PREFIX, pdData->szSysType, psCmd->nDetachNo );
					}
					else
					{
						sprintf( pdData->szFileHead, "%s.%s", FIXED_FILE_PREFIX, pdData->szSysType );
					}

					break;
				}
			}
			
			psIndex = psIndex->psiNext;
		}
		
		if( NULL == psIndex )
		{
			return MY_FAILED;
		}
	}
	
	/* printf( "%s - %s\n", pdData->szFilePath, pdData->szFileHead ); */
	
	return MY_SUCCEED;
}

int GetStoreMode( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	PSEARCH_CMD psCmd;
	PSEARCH_KEY psKey;
	PKEY_RECORD pkRec;
	int nLoop;

	psCmd = &(psData->sCmd);
	psKey = pdData->psKey;
	pkRec = psKey->pkRecord;
	
	for( nLoop = 0; nLoop < psKey->nKeyCount; nLoop++, pkRec++ )
	{
		if( 0 == strcmp( psCmd->szSysType, pkRec->szSysType ) )
		{
			psCmd->nStoreMode = pkRec->nStoreMode;
			
			if( STORE_MODE_MODULUS != psCmd->nStoreMode )
			{
				if( !IsMobileUser( psCmd->szSvcID ) )
				{
					psCmd->nStoreMode = STORE_MODE_FIXED;
				}
			}
			
			psCmd->nDetachNo = 0;
	
			memset( psCmd->szBaseAddr, 0, sizeof( psCmd->szBaseAddr ) );
			memset( psCmd->szOffset  , 0, sizeof( psCmd->szOffset ) );
		
			if( SVC_OFF_LEN < strlen( psCmd->szSvcID ) )
			{
				strncpy( psCmd->szBaseAddr, psCmd->szSvcID, strlen( psCmd->szSvcID ) - SVC_OFF_LEN );
				psCmd->szBaseAddr[strlen( psCmd->szSvcID ) - SVC_OFF_LEN] = 0;
		
				strncpy( psCmd->szOffset, psCmd->szSvcID + strlen( psCmd->szSvcID ) - SVC_OFF_LEN, SVC_OFF_LEN );
				psCmd->szOffset[SVC_OFF_LEN] = 0;
			}
			else
			{
				strcpy( psCmd->szBaseAddr, psCmd->szSvcID );
				strcpy( psCmd->szOffset  , psCmd->szSvcID );
			}
			
			if( 0 != pkRec->nDetachFlag )
			{
				psCmd->nDetachNo = atoi( psCmd->szOffset ) % pkRec->nDetachFlag;
			}
			
			return MY_SUCCEED;
		}
	}

	return MY_FAILED;
}

/********************************************************** 
function: InitDtlData
description: 查询信息初始化
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int InitDtlData( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	int nRetCode;
	PSEARCH_CMD psCmd;
	PKEY_RECORD pkRec;
	char szBaseAddr[BASE_ADDR_LEN + 1];
	
	psCmd = &(psData->sCmd);	
	pkRec = &(pdData->kRec);
	
	pdData->lOffset = 0;
	pdData->lPageNum = 0;
	pdData->nPkgNum = 0;
	pdData->lTotalRec = 0;
	pdData->fpCurr = NULL;
	pdData->nCurrHandle = -1;
	
	pdData->psKey = NULL;
	pdData->pfFile = NULL;
	pdData->pfmFile = NULL;
	
	memset( psData->szPkgBuff, 0, atol( psData->szPkgSize ) + 1 );
	
	strcpy( pdData->szSvcID, psCmd->szSvcID );
	strcpy( pdData->szStartTime, psCmd->szStartTime );
	strcpy( pdData->szEndTime, psCmd->szEndTime );
	strcpy( pdData->szSysType, psCmd->szSysType );
	strcpy( pdData->szRecType, psCmd->szRecType );
	
	pdData->nStoreMode = psData->sCmd.nStoreMode;
	
	return MY_SUCCEED;
}

int InitSeachInfo( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	pdData->psKey = psData->kControl.psKey + atoi( pdData->pfFile->szKeyFileNo );
	
	if( 0 == strcmp( pdData->pfFile->szFieldMapNo, FIELD_NO_MAP ) )
	{
		pdData->pfmFile = NULL;
	}	
	else
	{
		pdData->pfmFile = psData->fMap.pfmFile + atoi( pdData->pfFile->szFieldMapNo );
	}
	
	if( MY_SUCCEED != GetStoreMode( psData, pdData ) )
	{
		return MY_FAILED;
	}
	
	if( MY_SUCCEED != GetKeyRecord( psData, pdData ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "没有匹配的查询关键字信息\n" );
		return MY_FAILED;
	}
	
	/* 定位详单文件路径 */
	if( MY_SUCCEED != GetFilePath( psData, pdData ) )
	{
	#ifdef	__DEBUG_INFO__
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "用户详单文件存储路径不存在\n" );
	#endif
		return MY_FAILED;
	}
	
	pdData->nStoreMode = psData->sCmd.nStoreMode;
	
	return MY_SUCCEED;
}

/********************************************************** 
function: GetDetailInfo
description: 获取符合查询条件的用户详单记录
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetDetailInfo( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	PFMT_CTRL pfCtrl;
	PFMT_FILE pfFile;
	char szStartTime[14 + 1];
	char szEndTime[14 + 1];
	int nLoop;

	pfCtrl = &(psData->fCtrl);
	pfFile = pfCtrl->pfFile;
	
	/* 查询前初始化 */
	if( InitDtlData( psData, pdData ) )
	{
		return MY_FAILED;
	}

	/* 根据不同的话单格式查询不同存储位置的不同日期的话单 */
	for( nLoop = 0; nLoop < pfCtrl->nFmtNum; nLoop++, pfFile++ )
	{
		if( 0 == strcmp( pdData->szSysType, pfFile->szSysType ) )
		{
			pdData->pfFile = pfFile;
			
			if( MY_SUCCEED != InitSeachInfo( psData, pdData ) )
			{
				continue;
			}
			
			if( 0 < strncmp( pdData->szStartTime, pfFile->szEndTime,
									min( strlen( pdData->szStartTime ), strlen( pfFile->szEndTime ) ) ) 
			||  0 > strncmp( pdData->szEndTime, pfFile->szStartTime,
									min( strlen( pdData->szEndTime ), strlen( pfFile->szStartTime ) ) ) )
			{
				continue;
			}
			else if( 0 >= strncmp( pdData->szStartTime, pfFile->szEndTime,
									min( strlen( pdData->szStartTime ), strlen( pfFile->szEndTime ) ) ) 
			      && 0 <= strncmp( pdData->szStartTime, pfFile->szStartTime,
			      					min( strlen( pdData->szStartTime ), strlen( pfFile->szStartTime ) ) ) )
			{
				if( 0 >= strncmp( pdData->szEndTime, pfFile->szEndTime,
									min( strlen( pdData->szEndTime ), strlen( pfFile->szEndTime ) ) ) )
				{
					GetDetailData( psData, pdData );
				}
				else
				{
					memset( szEndTime, 0, sizeof( szEndTime ) );
					
					strcpy( szEndTime, pdData->szEndTime );
					strcpy( pdData->szEndTime, pfFile->szEndTime );
					
					GetDetailData( psData, pdData );
					
					strcpy( pdData->szEndTime, szEndTime );
				}	
			}
			else if( 0 > strncmp( pdData->szStartTime, pfFile->szStartTime,
									min( strlen( pdData->szStartTime ), strlen( pfFile->szStartTime ) ) ) )
			{
				if( 0 < strncmp( pdData->szEndTime, pfFile->szEndTime, 
									min( strlen( pdData->szEndTime ), strlen( pfFile->szEndTime ) ) ) )
				{
					memset( szStartTime, 0, sizeof( szStartTime ) );
					memset( szEndTime, 0, sizeof( szEndTime ) );
					
					strcpy( szStartTime, pdData->szStartTime );
					strcpy( szEndTime, pdData->szEndTime );
					
					strcpy( pdData->szStartTime, pfFile->szStartTime );
					strcpy( pdData->szEndTime, pfFile->szEndTime );
					
					GetDetailData( psData, pdData );
					
					strcpy( pdData->szStartTime, szStartTime );
					strcpy( pdData->szEndTime, szEndTime );
				}
				else if( 0 >= strncmp( pdData->szEndTime, pfFile->szEndTime,
									min( strlen( pdData->szEndTime ), strlen( pfFile->szEndTime ) ) ) )
				{
					memset( szStartTime, 0, sizeof( szStartTime ) );
					
					strcpy( szStartTime, pdData->szStartTime );
					strcpy( pdData->szStartTime, pfFile->szStartTime );
					
					GetDetailData( psData, pdData );
					
					strcpy( pdData->szStartTime, szStartTime );
				}
			}
		}
	}

	if( pdData->lOffset )
	{
		/* MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "返回结果包中剩余数据\n" ); */
		
		if( 0 == pdData->nPkgNum )
		{
			if( SendMsg( psData->szPkgBuff, strlen( psData->szPkgBuff ), 0, 0 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "发送查询结果信息失败\n" );
			}
		}
		else
		{
			if( SendMsg( psData->szPkgBuff, strlen( psData->szPkgBuff ), 0, 3 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "发送查询结果信息失败\n" );
			}
		}
				
		memset( psData->szPkgBuff, 0, atol( psData->szPkgSize ) + 1 );
	}
	else
	{
		if( SendMsg( NULL, 0, 0, 3 ) )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "发送结束包失败\n" );
		}
	}
	
	if( SHOW_DEGUG_INFO == nDebugFlag )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "共有 %ld 条符合查询条件的详单\n", pdData->lTotalRec );
	}

	return MY_SUCCEED;
}
