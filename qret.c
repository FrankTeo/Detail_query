 
/********************************************************************
copyright (C), 1995-2005, Si-Tech Information Technology Ltd. 
file_name: qret.c 
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 实现查询数据初始化、格式化查询结果集
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ... 
********************************************************************/


#include "qret.h"

/********************************************************** 
function: DebugOutIndex
description: 输出详单存储路径
Input: psData: 描述系统全局结构的结构体
Output:
Return: 0 成功
others:
***********************************************************/

int DebugOutIndex( PSEARCH_DATA psData )
{
	PSEARCH_HEAD psHead;
	PSEARCH_INDEX psIndex;
	
	int nCount, nLoop; 

	printf( "\t== File Index ==\n" );	
	
	for( nCount = 0; nCount < MAX_MOBILEHEAD_NUM; nCount++ )
	{
		psHead = psData->psHead + nCount;
		
		for( nLoop = 0; nLoop < MAX_HEAD_NUM; nLoop++ )
		{
			if( NULL != psHead->psIndex[nLoop] )
			{
				printf( "[%s] [%s] [%s] [%s] [%s] [%s] [%s]\n", 
							psHead->psIndex[nLoop]->szFileSys, 
							psHead->psIndex[nLoop]->szSubDir,
							psHead->psIndex[nLoop]->szH1H2H3,
							psHead->psIndex[nLoop]->szChgDate,
							psHead->psIndex[nLoop]->szOldSys,
							psHead->psIndex[nLoop]->szOldSub,
							psHead->psIndex[nLoop]->szStoreMode );
			}
		}			
	}
	
	psIndex = psData->psiCard;
	
	while( NULL != psIndex )
	{
		printf( "[%s] [%s] [%s] [%s] [%s] [%s]\n", 
						psIndex->szFileSys, 
						psIndex->szSubDir,
						psIndex->szH1H2H3,
						psIndex->szChgDate,
						psIndex->szOldSys,
						psIndex->szOldSub,
						psIndex->szStoreMode );
		
		psIndex = psIndex->psiNext;
	}
	
	printf( "\t== File Index ==\n" );
	
	return 0;
}

/********************************************************** 
function: DebugOutKeyInfo
description: 输出关键字路径
Input: psKey: 描述查询关键字信息的结构体
Output:
Return: 0 成功
others:
***********************************************************/

int DebugOutKeyInfo( PSEARCH_KEY psKey )
{
	int nLoop;
	PKEY_RECORD pkRec;
	
	pkRec = psKey->pkRecord;
	
	printf( "\n\t=== Search Key ===\n\n" );
	
	for( nLoop = 0; nLoop < psKey->nKeyCount; nLoop++, pkRec++ )
	{
		printf( "[%s] [%s] [%s] [%2d] [%3d] [%3d] [%d] [%3d] [%d] [%3d] [%d] [%d] [%3d] [%4d] [%d] [%d]\n",
										pkRec->szFileHead, 
										pkRec->szSysType, 
										pkRec->szDataType, 
										pkRec->nOperNo,	
										pkRec->nRecLen,	
										pkRec->nUserLoc, 
										pkRec->nUserLen, 
										pkRec->nTimeLoc, 
										pkRec->nTimeLen, 
										pkRec->nDealTimeLoc,
										pkRec->nDealTimeOff,
										pkRec->nDealTimeLen, 
										pkRec->nDetachFlag, 
										pkRec->nFeeUnit,
										pkRec->nStoreMode,
										pkRec->nStatMode );
	}
	
	printf( "\n\t=== Search Key ===\n\n" );
		
	return 0;
}

int DebugOutKeyControlInfo( PKEY_CONTROL pkControl )
{
	PSEARCH_KEY psKey;
	int nLoop;
	
	psKey = pkControl->psKey;
	
	printf( "\n\t== Key Control File ==\n\n" );	
	for( nLoop = 0; nLoop < pkControl->nKeyCount; nLoop++, psKey++ )
	{
		printf( "[%d] [%s]\n", psKey->nFileNo, psKey->szFileName );
	}
	printf( "\n\t== Key Control File ==\n\n" );	
	
	return 0;
}

int DebugOutDtlFmt( PFMT_FILE pfFile )
{
	int nLoop;
	PFIELD_FMT pfFmt;
	
	pfFmt = pfFile->pfFmt;

	printf( "\n\t== Dtl Fmt %s ==\n\n", pfFile->szFileName );	
	for( nLoop = 0; nLoop < pfFile->nFieldCount; nLoop++, pfFmt++ )
	{
		printf( "[%3d] [%-20s] [%3d] [%3d] [%3d] [%d] [%s]\n",
					pfFmt->nFieldNo, pfFmt->szFieldName, 
					pfFmt->nStart, pfFmt->nEnd, pfFmt->nLen,
					pfFmt->nType, pfFmt->szMapNo );
	}
	printf( "\n\t== Dtl Fmt %s ==\n\n", pfFile->szFileName );
	
	return 0;	
}

int DebugOutFmtCtrlInfo( PFMT_CTRL pfCtrl )
{
	int nLoop;
	PFMT_FILE pfFile;
	
	pfFile = pfCtrl->pfFile;

	printf( "\n\t== Fmt Ctrl ==\n\n" );	
	for( nLoop = 0; nLoop < pfCtrl->nFmtNum; nLoop++, pfFile++ )
	{
		printf( "[%s] [%s] [%s] [%s] [%s] [%s]\n",
					pfFile->szSysType, pfFile->szStartTime, 
					pfFile->szEndTime, pfFile->szFileName,
					pfFile->szKeyFileNo, pfFile->szFieldMapNo );
	}
	printf( "\n\t== Fmt Ctrl ==\n\n" );	
	
	return 0;	
}

int DebugOutMapRec( PMAP_FILE pmFile )
{
	int nLoop;
	PMAP_RECORD pmRec;
	
	pmRec = pmFile->pmRecord;

	printf( "\n\t== Map Rec %s ==\n\n", pmFile->szFileName );	
	for( nLoop = 0; nLoop < pmFile->nMapCount; nLoop++, pmRec++ )
	{
		printf( "[%s] [%s]\n",
					pmRec->szFieldValue, pmRec->szMapValue );
	}
	printf( "\n\t== Map Rec %s ==\n\n", pmFile->szFileName );	
	
	return 0;	
}

int DebugOutMapFileInfo( PMAP_INFO pmInfo )
{
	int nLoop;
	PMAP_FILE pmFile;
	
	pmFile = pmInfo->pmFile;
	
	printf( "\n\t== Map File ==\n\n" );	
	for( nLoop = 0; nLoop < pmInfo->nFileCount; nLoop++, pmFile++ )
	{
		printf( "[%d] [%s]\n",
					pmFile->nFileNo, pmFile->szFileName );
	}
	printf( "\n\t== Map File ==\n\n" );	
	
	return 0;
}

int DebugOutFieldMapRec( PFMAP_FILE pfmFile )
{
	int nLoop;
	PFMAP_RECORD pfmRec;
	
	pfmRec = pfmFile->pfmRecord;

	printf( "\n\t== Field Map Rec %s ==\n\n", pfmFile->szFileName );	
	for( nLoop = 0; nLoop < pfmFile->nFieldCount; nLoop++, pfmRec++ )
	{
		printf( "[%s] [%s]\n",
					pfmRec->szFieldNo, pfmRec->szMapNo );
	}
	printf( "\n\t== Field Map Rec %s ==\n\n", pfmFile->szFileName );	
	
	return 0;	
}

int DebugOutFieldMapInfo( PFIELD_MAP pfMap )
{
	int nLoop;
	PFMAP_FILE pfmFile;
	
	pfmFile = pfMap->pfmFile;
	
	printf( "\n\t== Field Map File ==\n\n" );	
	for( nLoop = 0; nLoop < pfMap->nFileCount; nLoop++, pfmFile++ )
	{
		printf( "[%d] [%s]\n",
					pfmFile->nFileNo, pfmFile->szFileName );
	}
	printf( "\n\t== Field Map File ==\n\n" );	
	
	return 0;
}

int DebugOutSelField( PSEL_INFO psInfo )
{
	PSEL_FIELD psField;
	int nLoop;
	
	psField = psInfo->psField;;
	
	printf( "\t== Sel Info ==\n" );	
	for( nLoop = 0; nLoop < psInfo->nSelCount; psField++, nLoop++ )
	{
		printf( "%d\n", psField->nFieldNo );
	}
	printf( "\t== Sel Info ==\n" );	
	
	return 0;
}

int DebugOutCondField( PCOND_INFO pcInfo )
{
	PCOND_FIELD pcField;
	int nLoop;
	
	pcField = pcInfo->pcField;
	
	printf( "\t== Cond Fieldo ==\n" );	
	for( nLoop = 0; nLoop < pcInfo->nCondCount; pcField++, nLoop++ )
	{
		printf( "%d : [%d] [%s]\n", 
						nLoop, pcField->nFieldNo, pcField->szValue );
	}
	printf( "\t== Cond Fieldo ==\n" );	
	
	return 0;
}

/********************************************************** 
function: FreeSearchData
description: 释放内存空间
Input: psData: 描述系统全局结构的结构体
Output:
Return: 0 成功
others:
***********************************************************/

int FreeSearchData( PSEARCH_DATA psData )
{
	PSEARCH_HEAD psHead;
	
	int nCount, nLoop;
	
	for( nCount = 0; nCount < MAX_MOBILEHEAD_NUM; nCount++ )
	{
		psHead = psData->psHead + nCount;
	
		for( nLoop = 0; nLoop < MAX_HEAD_NUM; nLoop++ )
		{
			if( NULL != psHead->psIndex[nLoop] )
			{
				free( psHead->psIndex[nLoop] );
				psHead->psIndex[nLoop] = NULL;
			}
		}
		
		memset( psHead->szHeadFlag, 0, sizeof( psHead->szHeadFlag ) );
	}
	
	for( nLoop = 0; nLoop < (psData->kControl).nKeyCount; nLoop++ )
	{
		if( NULL != ((psData->kControl).psKey + nLoop)->pkRecord )
		{
			free( ((psData->kControl).psKey + nLoop)->pkRecord );
			((psData->kControl).psKey + nLoop)->pkRecord = NULL;
		}
	}
	
	if( NULL != psData->kControl.psKey )
	{
		free( psData->kControl.psKey );
		psData->kControl.psKey = NULL;
	}

	for( nLoop = 0; nLoop < (psData->fCtrl).nFmtNum; nLoop++ )
	{
		if( NULL != ((psData->fCtrl).pfFile + nLoop)->pfFmt )
		{
			free( ((psData->fCtrl).pfFile + nLoop)->pfFmt );
			((psData->fCtrl).pfFile + nLoop)->pfFmt = NULL;
		}
	}
	
	if( NULL != (psData->fCtrl).pfFile )
	{
		free( (psData->fCtrl).pfFile );
		(psData->fCtrl).pfFile = NULL;
	}
	
	for( nLoop = 0; nLoop <(psData->mInfo).nFileCount; nLoop++ )
	{
		if( NULL != ((psData->mInfo).pmFile + nLoop)->pmRecord )
		{
			free( ((psData->mInfo).pmFile + nLoop)->pmRecord );
			((psData->mInfo).pmFile + nLoop)->pmRecord = NULL;
		}
	}
	
	if( NULL != (psData->mInfo).pmFile )
	{
		free( (psData->mInfo).pmFile );
		(psData->mInfo).pmFile = NULL;
	}
	
	for( nLoop = 0; nLoop < (psData->fMap).nFileCount; nLoop++ )
	{
		if( NULL != ((psData->fMap).pfmFile + nLoop)->pfmRecord )
		{
			free( ((psData->fMap).pfmFile + nLoop)->pfmRecord );
			((psData->fMap).pfmFile + nLoop)->pfmRecord = NULL;
		}
	}
	
	if( NULL != (psData->fMap).pfmFile )
	{
		free( (psData->fMap).pfmFile );
		(psData->fMap).pfmFile = NULL;
	}
	
	if( NULL != psData->szPkgBuff )
	{
		free( psData->szPkgBuff );
		psData->szPkgBuff = NULL;
	}
	
	return 0;
}

int FreeDetailData( PDETAIL_DATA pdData )
{
	if( NULL != (pdData->sInfo).psField )
	{
		free( (pdData->sInfo).psField );
		(pdData->sInfo).psField = NULL;
	}
	
	return 0;
}

int GetSelField( PSEL_INFO psInfo, const char * szSelInfo )
{
	PSEL_FIELD psField;
	char * szPos;
	char szSel[512];
	
	memset( psInfo, 0, sizeof( SEL_INFO ) );
	memset( szSel, 0, sizeof( szSel ) );
	
	szPos = NULL;	
	psInfo->nSelCount = 0;
	strcpy( szSel, szSelInfo );
	
	szPos = strtok( szSel, SEL_FIELD_SEP );
	while( NULL != szPos )
	{
		psInfo->nSelCount++;
		szPos = strtok( NULL, SEL_FIELD_SEP );
	}

	if( 0 == psInfo->nSelCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "查询字段为空!\n" );
		return MY_FAILED;
	}

	psInfo->psField = ( PSEL_FIELD ) malloc( psInfo->nSelCount * sizeof( SEL_FIELD ) );
	if( NULL == psInfo->psField )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "申请保存查询字段的内存空间失败!\n" );
		return MY_FAILED;
	}
	memset( psInfo->psField, 0, psInfo->nSelCount * sizeof( SEL_FIELD ) );

	szPos = NULL;
	psField = psInfo->psField;
	strcpy( szSel, szSelInfo );
	
	szPos = strtok( szSel, SEL_FIELD_SEP );
	while( NULL != szPos )
	{
		psField->nFieldNo = atoi( szPos );
		psField++;
		szPos = strtok( NULL, SEL_FIELD_SEP );
	}

	/*DebugOutSelField( psInfo );*/
	
	return MY_SUCCEED;
}

int RebuildField( char * szField, PCOND_FIELD pcField )
{
	char szLValue[128], szRValue[128];
	char * szEnd;
	int nOptLen;
	
	memset( szLValue, 0, sizeof( szLValue ) );
	memset( szRValue, 0, sizeof( szRValue ) );
	memset( pcField, 0, sizeof( COND_FIELD ) );
	
	if( NULL != ( szEnd = ( char * ) strstr( szField, "<=" ) ) )
	{
		pcField->nOptFlag = 2;
		nOptLen = 2;
	}
	else if( NULL != ( szEnd = ( char * ) strstr( szField, ">=" ) ) )
	{
		pcField->nOptFlag = 3;
		nOptLen = 2;
	}
	else if( NULL != ( szEnd = ( char * ) strstr( szField, "=" ) ) )
	{
		pcField->nOptFlag = 0;
		nOptLen = 1;
	}
	else if( NULL != ( szEnd = ( char * ) strstr( szField, "!" ) ) )
	{
		pcField->nOptFlag = 1;
		nOptLen = 1;
	}
	else if( NULL != ( szEnd = ( char * ) strstr( szField, "<" ) ) )
	{
		pcField->nOptFlag = 4;
		nOptLen = 1;
	}
	else if( NULL != ( szEnd = ( char * ) strstr( szField, ">" ) ) )
	{
		pcField->nOptFlag = 5;
		nOptLen = 1;
	}
	else
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "条件表达式 [%s] 无效!\n", szField );
		return MY_FAILED;
	}

	strncpy( szLValue, szField, szEnd - szField ); 
	szLValue[szEnd - szField] = 0;
	TrimLeft( TrimRight( szLValue ) );
	
	strcpy( szRValue, szEnd + nOptLen ); 
	TrimLeft( TrimRight( szRValue ) );
	
	pcField->nFieldNo = atoi( szLValue );
	strcpy( pcField->szValue, szRValue );
	
	memset( szField, 0, sizeof( szField ) );

	return MY_SUCCEED;
}

int GetCondField( PDETAIL_DATA pdData, const char * szCondInfo )
{
	PCOND_INFO pcInfo;
	PCOND_FIELD pcField;
	char szField[MAX_FIELD_LEN + 1];
	char * szEnd;
	int nLeftP, nRightP;
	int nLen;

	if( 0 == *( szCondInfo ) )
	{
		pdData->bCond = MY_FALSE;
		return MY_SUCCEED;
	}
	
	pdData->bCond = MY_TRUE;
	
	pcInfo = &(pdData->cInfo);
	pcField = pcInfo->pcField;
	
	nLen    = 0;	
	nLeftP  = 0;
	nRightP = 0;
	pcInfo->nCondCount = 0;
	
	memset( pdData->szCondExpr, 0, sizeof( pdData->szCondExpr ) );

	while( nLen < strlen( szCondInfo ) )
	{
		if( pcInfo->nCondCount > MAX_COND_NUM )
		{
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "条件表达式字段超过最大值!\n" );
			return MY_FAILED;
		}
		
		switch( szCondInfo[nLen] )
		{
			case '&':
			case '|':
				strncat( pdData->szCondExpr, szCondInfo + nLen, 1 );
				nLen++;
				break;
				
			case '(':
				nLeftP++;
				strncat( pdData->szCondExpr, szCondInfo + nLen, 1 );
				nLen++;
				break;
				
			case ')':
				nRightP++;
				strncat( pdData->szCondExpr, szCondInfo + nLen, 1 );
				nLen++;
				break;
				
			case '[':
				szEnd = ( char * ) strchr( szCondInfo + nLen + 1, ']' );
				if( NULL == szEnd )
				{
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "方括号不匹配!\n" );
					return MY_FAILED;	
				}
	
				memset( szField, 0, sizeof( szField ) );
				strncpy( szField, szCondInfo + nLen + 1, 
							szEnd - (szCondInfo + nLen + 1) );
				nLen += strlen(szField) + 2;	

				if( RebuildField( szField, (pcInfo->pcField + pcInfo->nCondCount) ) )
				{
					return MY_FAILED;
				}

				sprintf( pdData->szCondExpr, "%s[%d]", pdData->szCondExpr, pcInfo->nCondCount );
				pcInfo->nCondCount++;
				
				break;

			case ' ':
				nLen++;
				break;
				
			default:
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的字符 [%c]\n", szCondInfo[nLen] );
				return MY_FAILED;
				break;
		}
		
	}
	
	if( nLeftP != nRightP )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "圆括号不匹配\n" );
		return MY_FAILED;
	}
	
	/*printf( "%d - [%s]\n", pcInfo->nCondCount, pdData->szCondExpr );*/

	/*DebugOutCondField( pcInfo );*/

	return MY_SUCCEED;
}

/********************************************************** 
function: ComLogicExpr
description: 计算逻辑表达式的逻辑值
Input: szLeft: 左值
       szRight: 右值
       bOper: 逻辑运算符
Output:
Return: MY_TRUE: 真值 MY_FALSE: 假值
others:
***********************************************************/

int ComLogicExpr( char * szLeft, char * szRight, char bOper )
{
	if( '&' == bOper )
	{
		return ( atoi(szLeft) && atoi(szRight) );
	}
	else if( '|' == bOper )
	{
		return ( atoi(szLeft) || atoi(szRight) );
	}
}

/********************************************************** 
function: GetLogicVal
description: 获取逻辑表达式的逻辑值
Input: szExpr: 逻辑表达式
       psData: 描述系统全局数据的结构体
Output:
Return: RIGHT: 成功 ERROR: 失败
others:
***********************************************************/

int GetLogicVal( char * szRecord, char * szExpr, PDETAIL_DATA pdData )
{
	PCOND_INFO pcInfo;
	PCOND_FIELD pcField;
	PFMT_FILE pfFile;
	PFIELD_FMT pfFmt;
	char szField[MAX_FIELD_LEN + 1];
	char szVar[MAX_FIELD_LEN + 1];
	int ncfNo, nFieldNo;
	
	pcInfo = &(pdData->cInfo);
	pfFile = pdData->pfFile;
	
	pcField = NULL;
	pfFmt = NULL;
	
	ncfNo = atoi( szExpr );
	pcField = (pcInfo->pcField + ncfNo);
	
	nFieldNo = GetMapFieldNo( pdData, pcField->nFieldNo );
	
	pfFmt = pfFile->pfFmt + nFieldNo;

	memcpy( szField, szRecord + RECORD_VALID_LEN + pfFmt->nStart, pfFmt->nLen );
	szField[pfFmt->nLen] = 0;
	TrimLeft( TrimRight( szField ) );

	if( 0 == *(szField) )	/* NULL or IS NULL ? */
	{
		return MY_FALSE;
	}
	else
	{
		if( 0 == strncmp( pcField->szValue, FIELD_VAR_FLAG, strlen( FIELD_VAR_FLAG ) ) )
		{
			ncfNo = atoi( pcField->szValue + strlen( FIELD_VAR_FLAG ) );
			pfFmt = pfFile->pfFmt + ncfNo;

			memcpy( szVar, szRecord + RECORD_VALID_LEN + pfFmt->nStart, pfFmt->nLen );
			szVar[pfFmt->nLen] = 0;
			TrimLeft( TrimRight( szVar ) );
			
			if( 0 == pcField->nOptFlag )
			{
				if( 0 == strcmp( szField, szVar ) )
				{
					return MY_TRUE;
				}
				else
				{
					return MY_FALSE;
				}
			}
			else if( 1 == pcField->nOptFlag )
			{
				if( 0 != strcmp( szField, szVar ) )
				{
					return MY_TRUE;
				}
				else
				{
					return MY_FALSE;
				}
			}
			else if( 2 == pcField->nOptFlag )
			{
				if( 0 >= strcmp( szField, szVar ) )
				{
					return MY_TRUE;
				}
				else
				{
					return MY_FALSE;
				}
			}
			else if( 3 == pcField->nOptFlag )
			{
				if( 0 <= strcmp( szField, szVar ) )
				{
					return MY_TRUE;
				}
				else
				{
					return MY_FALSE;
				}
			}
			else if( 4 == pcField->nOptFlag )
			{
				if( 0 > strcmp( szField, szVar ) )
				{
					return MY_TRUE;
				}
				else
				{
					return MY_FALSE;
				}
			}
			else if( 5 == pcField->nOptFlag )
			{
				if( 0 < strcmp( szField, szVar ) )
				{
					return MY_TRUE;
				}
				else
				{
					return MY_FALSE;
				}
			}
		}
		else if( 0 == pcField->nOptFlag )
		{
			if( 0 == strcmp( szField, pcField->szValue ) )
			{
				return MY_TRUE;
			}
			else
			{
				return MY_FALSE;
			}
		}
		else if( 1 == pcField->nOptFlag )
		{
			if( 0 != strcmp( szField, pcField->szValue ) )
			{
				return MY_TRUE;
			}
			else
			{
				return MY_FALSE;
			}
		}	
		else if( 2 == pcField->nOptFlag )
		{
			if( 0 >= strcmp( szField, pcField->szValue ) )
			{
				return MY_TRUE;
			}
			else
			{
				return MY_FALSE;
			}
		}
		else if( 3 == pcField->nOptFlag )
		{
			if( 0 <= strcmp( szField, pcField->szValue ) )
			{
				return MY_TRUE;
			}
			else
			{
				return MY_FALSE;
			}
		}
		else if( 4 == pcField->nOptFlag )
		{
			if( 0 > strcmp( szField, pcField->szValue ) )
			{
				return MY_TRUE;
			}
			else
			{
				return MY_FALSE;
			}
		}
		else if( 5 == pcField->nOptFlag )
		{
			if( 0 < strcmp( szField, pcField->szValue ) )
			{
				return MY_TRUE;
			}
			else
			{
				return MY_FALSE;
			}
		}
	}	
}

int GetCondValue( char * szRecord, PDETAIL_DATA pdData )
{
	PCOND_INFO pcInfo;
	PCOND_FIELD pcField;
	char bOptStack[200];
	char szParaStack[200][256];
	char szResult[128];
	char * szExpr;
	char * szEnd;
	int nLoop, nLen;
	int nOptTop, nParaTop;
	
	if( !(pdData->bCond) )
	{
		return MY_TRUE;
	}
	
	pcInfo = &(pdData->cInfo);
	pcField = pcInfo->pcField;
	
	nLen = 0;
	nOptTop = 0;
	nParaTop = 0;
	
	szExpr = pdData->szCondExpr;

	while( nLen < strlen( szExpr ) )
	{
		switch( (szExpr[nLen]) )
		{
			case '&':
			case '|':
			case '(':
				bOptStack[nOptTop] = szExpr[nLen];
				nOptTop++;
				nLen++;

				break;

			case ')':
				while( '(' != bOptStack[nOptTop - 1] )
				{
					memset( szResult, 0, sizeof( szResult ) );
					sprintf( szResult, "%d", 
									ComLogicExpr( szParaStack[nParaTop - 1], 
													szParaStack[nParaTop - 2],
													bOptStack[nOptTop - 1] ) );
					strcpy( szParaStack[nParaTop - 2 ], szResult );

					nParaTop--;
					nOptTop--;
				}
				
				nOptTop--;
				nLen++;
				
				break;
			
			case '[':
				szEnd = strchr( szExpr + nLen + 1, ']' );
				if( NULL == szEnd )
				{
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "不匹配的方括号!\n" );
					return MY_FALSE;	
				}
				
				memset( szParaStack[nParaTop], 0, sizeof( szParaStack[nParaTop] ) );
				strncpy( szParaStack[nParaTop], szExpr + nLen + 1, 
							szEnd - (szExpr + nLen + 1) );
				nLen += strlen(szParaStack[nParaTop]) + 2;	

				sprintf( szParaStack[nParaTop], "%d", 
								GetLogicVal( szRecord, szParaStack[nParaTop], pdData ) );

				nParaTop++;
				
				break;
			
			case ' ':
				nLen++;
				break;
				
			default:
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的字符!\n" );
				return MY_FALSE;	
				break;  
		}
	}

	memset( szResult, 0, sizeof( szResult ) );
	strcpy( szResult, szParaStack[nParaTop - 1] );
	nParaTop--;
	
	while( nOptTop )
	{
		sprintf( szResult, "%d", 
						ComLogicExpr( szResult,
										szParaStack[nParaTop - 1],
										bOptStack[nOptTop - 1] ) );
			
		nParaTop--;
		nOptTop--;
	}

	/* printf("result = [%s]\n", szResult ); */

	return ( atoi( szResult ) );
}

/********************************************************** 
function: IsQueryDetail
description: 判断详单记录是否符合查询条件
Input: szRecord: 详单记录
	   pdData: 描述查询系统的数据结构
Output:
Return: MY_TURE: 是 MY_FALSE: 不是
others:
***********************************************************/

int IsQueryDetail( char * szRecord, PDETAIL_DATA pdData )
{
	PKEY_RECORD pkRec;
	char szCallTime[14 + 1];
	char szDealTime[14 + 1];
	int nTail;
	
	pkRec = &(pdData->kRec);

    if( 0 == strncmp( szRecord + RECORD_VALID_START, RECORD_VALID_FLAG, strlen( RECORD_VALID_FLAG ) ) )
    {
        return MY_FALSE;
    }

	if( 0 != strncmp( pdData->szSysType, szRecord + RECORD_VALID_LEN, strlen( pkRec->szSysType ) ) )
	{
		return MY_FALSE;
	}
	
	if( pdData->bCurrQuery || STORE_MODE_MOBILE != pdData->nStoreMode )
	{
		if( 0 != strncmp( pdData->szSvcID, szRecord + RECORD_VALID_LEN + pkRec->nUserLoc, strlen( pdData->szSvcID ) ) )
		{
			return MY_FALSE;
		}
	}

	if( 0 > strncmp( pdData->szQueryDate, OLD_END_TIME, 
								min( strlen( pdData->szQueryDate ), strlen( OLD_END_TIME ) ) ) )
    {
        memset( szDealTime, 0, sizeof( szDealTime ) );
        memset( szCallTime, 0, sizeof( szCallTime ) );

        strncpy( szCallTime, szRecord + RECORD_VALID_LEN + pkRec->nTimeLoc, pkRec->nTimeLen );
        szCallTime[pkRec->nTimeLen] = 0;

        TransDealTime( szCallTime, szDealTime );
        
        if( 0 >= strcmp( szDealTime, pdData->szStartTime ) 
		||  0 < strcmp( szDealTime, pdData->szEndTime ) )
		{
			return MY_FALSE;
		}
    }
    else
    {
        memset( szDealTime, 0, sizeof( szDealTime ) );

   		strncat( szDealTime, pdData->szQueryMonth, pkRec->nDealTimeOff );
        strncat( szDealTime, szRecord + RECORD_VALID_LEN + pkRec->nDealTimeLoc, pkRec->nDealTimeLen );

		nTail = pkRec->nDealTimeOff + pkRec->nDealTimeLen;
		
		while( nTail < DATE_STD_LEN )
		{
        	strcat( szDealTime, "0" );
        	nTail++;
        }
		
        szDealTime[DATE_STD_LEN] = 0;
		
		/* printf( "%s %s %s\n", szDealTime, pdData->szStartTime, pdData->szEndTime ); */
		
		if( 0 > strcmp( szDealTime, pdData->szStartTime ) 
		||  0 < strcmp( szDealTime, pdData->szEndTime ) )
		{
			return MY_FALSE;
		}
	}	

	return GetCondValue( szRecord, pdData );	
}

int MapCmp( PMAP_RECORD pmRec1, PMAP_RECORD pmRec2 )
{
	char szKey1[MAX_FIELD_LEN + 1];
	char szKey2[MAX_FIELD_LEN + 1];
	
	strcpy( szKey1, pmRec1->szFieldValue );
	strcpy( szKey2, pmRec2->szFieldValue );
	
	return ( strcmp( szKey1, szKey2 ) );
}

int FieldMapCmp( PFMAP_RECORD pfmRec1, PFMAP_RECORD pfmRec2 )
{
	char szKey1[MAX_FIELD_LEN + 1];
	char szKey2[MAX_FIELD_LEN + 1];
	
	strcpy( szKey1, pfmRec1->szFieldNo );
	strcpy( szKey2, pfmRec2->szFieldNo );
	
	return ( strcmp( szKey1, szKey2 ) );
}

int ConvertNegativeValue( char * szField )
{
	char szValue[1024 + 1];
	char * szPos;
	
	szPos = NULL;
	
	szPos = strrchr( szField, '-' );
	if( NULL != szPos )
	{
		memset( szValue, 0, sizeof( szValue) );
		
		strcpy( szValue, szPos );
		
		strncpy( szField, szValue, strlen( szValue ) );
		szField[strlen( szValue )] = 0;
	}
	
	return MY_SUCCEED;
}

int MapFieldValue( char * szField, PFIELD_FMT pfFmt, PSEARCH_DATA psData )
{
	PMAP_INFO pmInfo;
	PMAP_FILE pmFile;
	PMAP_RECORD pmRec;
	MAP_RECORD mRec;
	
	if( 0 == strcmp( pfFmt->szMapNo, NOT_NEED_MAP ) )
	{
		if( 1 == pfFmt->nType )
		{
			ConvertNegativeValue( szField );
		}
		
		return MY_SUCCEED;
	}
	
	pmInfo = &(psData->mInfo);
	pmFile = pmInfo->pmFile + atoi( pfFmt->szMapNo );
	
	pmRec = NULL;
	memset( &mRec, 0, sizeof( MAP_RECORD ) );
	
	strcpy( mRec.szFieldValue, szField );
	
	pmRec = bsearch( &mRec, pmFile->pmRecord, pmFile->nMapCount, sizeof( MAP_RECORD ), (int (*) (const void *,const void *))MapCmp );
	if( NULL != pmRec )
	{
		strcpy( szField, pmRec->szMapValue );
		return MY_SUCCEED;
	}
	else
	{
		memset( &mRec, 0, sizeof( MAP_RECORD ) );
	
		strcpy( mRec.szFieldValue, MAP_DEFAULT_VALUE );

		pmRec = bsearch( &mRec, pmFile->pmRecord, pmFile->nMapCount, sizeof( MAP_RECORD ), (int (*) (const void *,const void *))MapCmp );
		if( NULL != pmRec )
		{
			strcpy( szField, pmRec->szMapValue );
			return MY_SUCCEED;
		}
	}
	
	return MY_SUCCEED;
}

int GetMapFieldNo( PDETAIL_DATA pdData, int nFieldNo )
{
	PFMT_FILE pfFile;
	PFMAP_FILE pfmFile;
	PFMAP_RECORD pfmRecord;
	FMAP_RECORD fmRecord;
	int nMapNo;
	
	pfFile = pdData->pfFile;
	pfmFile = pdData->pfmFile;
	
	if( 0 == strcmp( pfFile->szFieldMapNo, FIELD_NO_MAP ) )
	{
		return nFieldNo;
	}
			
	pfmRecord = NULL;
	memset( &fmRecord, 0, sizeof( FMAP_RECORD ) );
	
	sprintf( fmRecord.szFieldNo, "%d", nFieldNo );
	
	pfmRecord = bsearch( &fmRecord, pfmFile->pfmRecord, pfmFile->nFieldCount, sizeof( FMAP_RECORD ), (int (*) (const void *,const void *))FieldMapCmp );
	if( NULL != pfmRecord )
	{
		return ( atoi( pfmRecord->szMapNo ) );
	}
	else
	{
		return nFieldNo;
	}
}

int GetSelValue( char * szRecord, PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	PSEL_INFO psInfo;
	PSEL_FIELD psField;
	PFMT_FILE pfFile;
	PFIELD_FMT pfFmt;
	char szField[MAX_FIELD_LEN + 1];
	int nFieldNo;
	int nLoop;
	
	psInfo = &(pdData->sInfo);
	psField = psInfo->psField;
	pfFile = pdData->pfFile;
	
	/* DebugOutSelField( psInfo ); */
	
	memset( pdData->szResult, 0, sizeof( pdData->szResult ) );
	
	for( nLoop = 0; nLoop < psInfo->nSelCount; nLoop++, psField++ )
	{
		memset( szField, 0, sizeof( szField ) );

		nFieldNo = GetMapFieldNo( pdData, psField->nFieldNo );
		
		if( nFieldNo >= pfFile->nFieldCount )
		{
			memset( szField, 0, sizeof( szField ) );
		}
		else
		{
			pfFmt = pfFile->pfFmt + nFieldNo;
			
			memcpy( szField, szRecord + RECORD_VALID_LEN + pfFmt->nStart, pfFmt->nLen );
			szField[pfFmt->nLen] = 0;
			
			/* TrimLeft( TrimRight( szField ) ); */
			
			TrimRight( szField );
			TrimLeft( szField );

			MapFieldValue( szField, pfFmt, psData );
		}
		
		strcat( pdData->szResult, szField );
		strcat( pdData->szResult, RET_FIELD_SEP );
	}

	strcat( pdData->szResult, EET_RECORD_SEP );	

	return MY_SUCCEED;
}

/************************************************************
function: GetGroupKeyInfo
description: 获取存储键值内容
input: psIndex: 详单存储信息
output:
return: MY_SUCCEED: 成功, 失败退出
others:
***********************************************************/

int GetGroupKeyInfo ( PSEARCH_INDEX psIndex )
{
	char szH1H2H3[16 + 1];
	char * szPos;
	int nFieldNum;
	
	memset( szH1H2H3, 0, sizeof( szH1H2H3 ) );
	strcpy( szH1H2H3, psIndex->szH1H2H3 );
	
	memset( psIndex->szKey, 0, sizeof( psIndex->szKey ) );
	
	nFieldNum = 0;
	
	szPos = strtok( szH1H2H3, "/" );
	while( NULL != szPos )
	{
		switch( nFieldNum )
		{
			case 0:		/* KEY 值 */
			
				strcpy( psIndex->szKey, szPos );
				break;
				
			case 1:		/* 开始键值 */
			
				psIndex->nBeginKey = atoi(szPos );
				break;
			
			case 2:
				psIndex->nEndKey = atoi(szPos );
				break;
			
			default:    /* 结束键值 */
				return MY_FAILED;
		}
		
		nFieldNum++;
		szPos = strtok( NULL, "/" );
	}
	
	if( 3 != nFieldNum )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "键值 %s 字段数 %d 不足!\n", psIndex->szH1H2H3, nFieldNum );
		return MY_FAILED;
	}
		
	return MY_SUCCEED;
}

/********************************************************** 
function: GetFileIndex
description: 获取详单文件存储目录信息
Input: psData: 描述系统全局结构的结构体
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int	GetFileIndex( PSEARCH_DATA psData )
{
	PSEARCH_HEAD psHead;
	PSEARCH_INDEX psIndex;
	SEARCH_INDEX sInd;
	FILE * fp;
	
	char szRec[512];
	char szPath[512];
	char szHeadFlag[HEAD_FLAG_LEN + 1];
	char * szPos;
	
	int nIndexNum;
	int nFieldNum;
	int nCount, nLoop;
	
	for( nCount = 0; nCount < MAX_MOBILEHEAD_NUM; nCount++ )
	{
		psHead = psData->psHead + nCount;
		
		for( nLoop = 0; nLoop < MAX_HEAD_NUM; nLoop++ )
		{
			psHead->psIndex[nLoop] = NULL;
		}
		
		memset( psHead->szHeadFlag, 0, sizeof( psHead->szHeadFlag ) );
	}
	
	psData->psiCard = NULL;

	strcpy( szPath, szIndFile );
	if( NULL == ( fp = fopen( szPath, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "文件 %s 打开失败\n", szPath );
		return MY_FAILED;
	}

	memset( szRec, 0, 512 );
	
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		szPos = NULL;
		memset( &sInd, 0, sizeof( SEARCH_INDEX ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					strcpy( sInd.szFileSys, szPos );
					break;
				case 1:
					strcpy( sInd.szSubDir, szPos );
					break;
				case 2:
					strcpy( sInd.szH1H2H3, szPos );
					break;
				case 3:
					strcpy( sInd.szChgDate, szPos );
					break;
				case 4:
					strcpy( sInd.szOldSys, szPos );
					break;
				case 5:
					strcpy( sInd.szOldSub, szPos );
					break;
				case 6:
					strcpy( sInd.szStoreMode, szPos );
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_PATH != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单文件存储目录信息文件记录格式错误\n" );
			return MY_FAILED;
		}
		
		memset( szHeadFlag, 0, sizeof( szHeadFlag ) );
		
		if( IsMobileHead( sInd.szH1H2H3 ) )
		{
			strncpy( szHeadFlag, sInd.szH1H2H3, HEAD_FLAG_LEN );
			szHeadFlag[HEAD_FLAG_LEN] = 0;
			
			for( nCount = 0; nCount < MAX_MOBILEHEAD_NUM; nCount++ )
			{
				psHead = psData->psHead + nCount;
				
				if( 0 == *( psHead->szHeadFlag ) )
				{
					strcpy( psHead->szHeadFlag, szHeadFlag );
					break;
				}
				else if( 0 == strcmp( psHead->szHeadFlag, szHeadFlag ) )
				{
					break;
				}
			}
			
			if( nCount >= MAX_MOBILEHEAD_NUM )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "最大号码头个数 %s 不足!\n\n", MAX_HEAD_NUM );
				return MY_FAILED;
			}
			
			if( MAX_HEAD_NUM <= atoi( sInd.szH1H2H3 + 2 ) )
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无效的号段 %s\n", sInd.szH1H2H3 );
				return MY_FAILED;
			}
			
			if( NULL == psHead->psIndex[atoi( sInd.szH1H2H3 + 2 )] )
			{
				psHead->psIndex[atoi( sInd.szH1H2H3 + 2 )] = (PSEARCH_INDEX) malloc( sizeof( SEARCH_INDEX ) );
				if( NULL == psHead->psIndex[atoi( sInd.szH1H2H3 + 2 )] )
				{
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "申请保存移动号码详单存储位置信息的内存空间失败!\n" );
					return MY_FAILED;
				}
				
				memset( psHead->psIndex[atoi( sInd.szH1H2H3 + 2 )], 0, sizeof( SEARCH_INDEX ) );
				
				memcpy( psHead->psIndex[atoi( sInd.szH1H2H3 + 2 )], &sInd, sizeof( SEARCH_INDEX ) );
			}
			else
			{
				MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "号段 %s 存储索引信息重复\n", sInd.szH1H2H3 );
				return MY_FAILED;
			}
		}
		else
		{
			if( NULL == psData->psiCard )
			{
				psData->psiCard = (PSEARCH_INDEX) malloc( sizeof( SEARCH_INDEX ) );
				if( NULL == psData->psiCard )
				{
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "申请保存固定号码详单存储位置信息的内存空间失败!\n" );
					return MY_FAILED;
				}
				
				memset( psData->psiCard, 0, sizeof( SEARCH_INDEX ) );
				
				psIndex = psData->psiCard;
			}
			else
			{
				psIndex = psData->psiCard;
				
				while( NULL != psIndex )
				{
					if( NULL == psIndex->psiNext )
					{
						break;
					}
					
					psIndex = psIndex->psiNext;
				}
				
				psIndex->psiNext = (PSEARCH_INDEX) malloc( sizeof( SEARCH_INDEX ) );
				if( NULL == psIndex->psiNext )
				{
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "申请保存固定号码详单存储位置信息的内存空间失败!\n" );
					return MY_FAILED;
				}
				
				memset( psIndex->psiNext, 0, sizeof( SEARCH_INDEX ) );
				
				psIndex = psIndex->psiNext;
			}
			
			memcpy( psIndex, &sInd, sizeof( SEARCH_INDEX ) );
			
			if( STORE_MODE_MODULUS == atoi( psIndex->szStoreMode ) )
			{
				if( MY_SUCCEED != GetGroupKeyInfo( psIndex ) )
				{
					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "存储模式 %s 的分组字段 [%s] 无效!\n", psIndex->szStoreMode , psIndex->szH1H2H3 );
					return MY_FAILED;
				}
			}
		}
		
		nIndexNum++;
		memset( szRec, 0, 512 );
	}
	
	fclose( fp );
	
	if( 0 >= nIndexNum )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单存储信息文件内容为空\n" );
		return MY_FAILED;
	}
		
	/* DebugOutIndex( psData ); */
	
	return MY_SUCCEED;
}

/********************************************************** 
function: KeyCmp
description: 比较两个查询关键字的查询类型的值
Input: pkRec1: 描述第一个查询关键字信息
       pkRec2: 描述第二个查询关键字信息
Output:
Return: 0: 相等 非0: 不等
others:
***********************************************************/

int KeyCmp( PKEY_RECORD pkRec1, PKEY_RECORD pkRec2 )
{
	char szKey1[32 + SYS_TYPE_LEN + 1];
	char szKey2[32 + SYS_TYPE_LEN + 1];
	
	sprintf( szKey1, "%s", pkRec1->szSysType );
	sprintf( szKey2, "%s", pkRec2->szSysType );
	
	return ( strcmp( szKey1, szKey2 ) );
}

/********************************************************** 
function: GetKeyInfo
description: 获取查询关键字信息
Input: psKey: 描述查询关键字信息的结构体
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int GetKeyInfo( PSEARCH_KEY psKey )
{
	FILE * fp;
	char szRec[512];
	char szPath[512];
	char * szPos;
	int nFieldNum;
	
	PKEY_RECORD pkRec;
	
	psKey->nKeyCount = 0;
	psKey->pkRecord = NULL;
	strcpy( szPath, psKey->szFileName );
	
	psKey->nKeyCount = GetFileCount( szPath );
	if( 0 >= psKey->nKeyCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "关键字信息文件 %s 内容为空\n", szPath );
		return 1;
	}

	psKey->pkRecord = ( PKEY_RECORD ) malloc( ( psKey->nKeyCount ) * sizeof(KEY_RECORD) );
	if( NULL == psKey->pkRecord )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( psKey->pkRecord, 0, ( psKey->nKeyCount ) * sizeof(KEY_RECORD) );
	
	if( NULL == ( fp = fopen( szPath, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "文件 %s 打开失败\n", szPath );
		return 1;
	}

	szPos = NULL;
	pkRec = psKey->pkRecord;
	psKey->nKeyCount = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( pkRec, 0, sizeof( KEY_RECORD ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
			#ifdef	_SICHUAN_VERSION_
				case 0:		/* 系统类型 */
				
					strcpy( pkRec->szSysType, szPos );
					break;
					
				case 1:		/* 数据类型 */
					strcpy( pkRec->szDataType, szPos );
					break;
				
				case 2:		/* 增量统计字段配置编号 */
				
					pkRec->nOperNo = atoi( szPos );
					break;
					
				case 3:		/* 记录长度 */
				
					pkRec->nRecLen = atoi( szPos );
					break;
					
				case 4:		/* 用户字段偏移 */
				
					pkRec->nUserLoc = atoi( szPos );
					break;
					
				case 5:		/* 用户字段长度 */
				
					pkRec->nUserLen = atoi( szPos );
					
					if( 0 >= pkRec->nUserLen || pkRec->nUserLen > MAX_SUBSCRIBER_LEN )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "用户字段长度 %d 超出定义!\n", pkRec->nUserLen );
						return 1;
					}
					
					break;
					
				case 6:		/* 时间字段位置 */
				
					pkRec->nTimeLoc = atoi( szPos );
					break;
					
				case 7:		/* 时间字段长度 */
				
					pkRec->nTimeLen = atoi( szPos );	
					
					if( 0 >= pkRec->nTimeLen || pkRec->nTimeLen > DATE_STD_LEN )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "时间字段长度 %d 超出定义!\n", pkRec->nTimeLen );
						return 1;
					}
					
					break;
				
				case 8:		/* 处理时间字段位置 */
				
					pkRec->nDealTimeLoc = atoi( szPos );
					break;
				
				case 9:	/* 处理时间字段偏移 */
				
					pkRec->nDealTimeOff = atoi( szPos );
					break;
					
				case 10:	/* 处理时间字段长度 */
				
					pkRec->nDealTimeLen = atoi( szPos );	
					
					if( 0 >= pkRec->nDealTimeLen || pkRec->nDealTimeLen > DATE_STD_LEN )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "处理时间字段长度 %s 超出定义!\n", szPos );
						return 1;
					}
					
					break;
						
				case 11:	/* 拆分标志 */
				
					pkRec->nDetachFlag = atoi( szPos );
					break;
				
				case 12:	/* 费用基本单位 */
				
					pkRec->nFeeUnit = atoi( szPos );
					break;
				
				case 13:	/* 存储方式 */
					
					pkRec->nStoreMode = atoi( szPos );
					break;

				case 14:
					
					pkRec->nStatMode = atoi( szPos );
					break;
			#else
				case 0:		/* 文件头 */
				
					strcpy( pkRec->szFileHead, szPos );
					break;
				
				case 1:		/* 系统类型 */
				
					strcpy( pkRec->szSysType, szPos );
					break;
					
				case 2:		/* 数据类型 */
					strcpy( pkRec->szDataType, szPos );
					break;
				
				case 3:		/* 增量统计字段配置编号 */
				
					pkRec->nOperNo = atoi( szPos );
					break;
					
				case 4:		/* 记录长度 */
				
					pkRec->nRecLen = atoi( szPos );
					break;
					
				case 5:		/* 用户字段偏移 */
				
					pkRec->nUserLoc = atoi( szPos );
					break;
					
				case 6:		/* 用户字段长度 */
				
					pkRec->nUserLen = atoi( szPos );
					
					if( 0 >= pkRec->nUserLen || pkRec->nUserLen > MAX_SUBSCRIBER_LEN )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "用户字段长度 %d 超出定义!\n", pkRec->nUserLen );
						return 1;
					}
					
					break;
					
				case 7:		/* 时间字段位置 */
				
					pkRec->nTimeLoc = atoi( szPos );
					break;
					
				case 8:		/* 时间字段长度 */
				
					pkRec->nTimeLen = atoi( szPos );	
					
					if( 0 >= pkRec->nTimeLen || pkRec->nTimeLen > DATE_STD_LEN )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "时间字段长度 %d 超出定义!\n", pkRec->nTimeLen );
						return 1;
					}
					
					break;
				
				case 9:		/* 处理时间字段位置 */
				
					pkRec->nDealTimeLoc = atoi( szPos );
					break;
				
				case 10:	/* 处理时间字段偏移 */
				
					pkRec->nDealTimeOff = atoi( szPos );
					break;
					
				case 11:	/* 处理时间字段长度 */
				
					pkRec->nDealTimeLen = atoi( szPos );	
					
					if( 0 >= pkRec->nDealTimeLen || pkRec->nDealTimeLen > DATE_STD_LEN )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "处理时间字段长度 %s 超出定义!\n", szPos );
						return 1;
					}
					
					break;
						
				case 12:	/* 拆分标志 */
				
					pkRec->nDetachFlag = atoi( szPos );
					break;
				
				case 13:	/* 费用基本单位 */
				
					pkRec->nFeeUnit = atoi( szPos );
					break;
				
				case 14:	/* 存储方式 */
					
					pkRec->nStoreMode = atoi( szPos );
					break;
				
				case 15:	/* 增量数据统计方式 */
					
					pkRec->nStatMode = atoi( szPos );
					break;
			#endif		
				default:    /* 无效字段 */

					MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "记录域字段数超出定义值!\n" );
					return 1;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}

		if( FIELD_NUM_KEY != nFieldNum ) 
		{
			fclose( fp ); 
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "查询关键字位置信息文件记录格式错误\n" ); 
			return 1; 
		}

		pkRec++;		
		psKey->nKeyCount++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	qsort( psKey->pkRecord, psKey->nKeyCount, sizeof( KEY_RECORD ), (int (*) (const void *,const void *))KeyCmp );
	
	/* DebugOutKeyInfo( psKey ); */
	
	return 0;	
}

int GetKeyControlInfo( PKEY_CONTROL pkControl )
{
	FILE * fp;
	char szRec[512];
	char szPath[512];
	char * szPos;
	int nFieldNum;
	
	PSEARCH_KEY psKey;
	
	pkControl->nKeyCount = 0;
	pkControl->psKey = NULL;
	strcpy( szPath, szKeyFile );
	
	pkControl->nKeyCount = GetFileCount( szPath );
	if( 0 >= pkControl->nKeyCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "查询关键字控制文件内容为空\n" );
		return 0;
	}

	pkControl->psKey = ( PSEARCH_KEY ) malloc( ( pkControl->nKeyCount ) * sizeof(SEARCH_KEY) );
	if( NULL == pkControl->psKey  )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( pkControl->psKey, 0, pkControl->nKeyCount * sizeof(SEARCH_KEY) );
	
	if( NULL == ( fp = fopen( szPath, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "文件 %s 打开失败\n", szPath );
		return 1;
	}

	szPos = NULL;
	psKey = pkControl->psKey;
	pkControl->nKeyCount = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( psKey, 0, sizeof( SEARCH_KEY ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					psKey->nFileNo = atoi( szPos );
					break;
				case 1:
					strcpy( psKey->szFileName, szPos );
					break;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_KCTRL != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "查询关键字控制文件记录格式错误\n" );
			return MY_FAILED;
		}

		if( GetKeyInfo( psKey ) )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "查询关键字控制文件内容错误\n" );
			return MY_FAILED;
		}

		psKey++;		
		pkControl->nKeyCount++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	/* DebugOutKeyControlInfo( pkControl ); */
	
	return 0;
}

/********************************************************** 
function: GetKeyRecord
description: 获取关键字信息
Input: psData: 描述系统全局结构的结构体
	   pdData: 描述查询系统的数据结构
Output:
Return: 0: 成功 1: 失败
others:
***********************************************************/

int GetKeyRecord( PSEARCH_DATA psData, PDETAIL_DATA pdData )
{
	PKEY_RECORD pkRec;
	KEY_RECORD kRecord;
	
	pkRec = NULL;
	memset( &kRecord, 0, sizeof( KEY_RECORD ) );
	memset( &(pdData->kRec), 0, sizeof( KEY_RECORD ) );
	
	strcpy( kRecord.szSysType, pdData->szSysType );
	
	pkRec = bsearch( &kRecord, pdData->psKey->pkRecord, pdData->psKey->nKeyCount, sizeof( KEY_RECORD ), (int (*) (const void *,const void *))KeyCmp );
	if( NULL != pkRec )
	{
		memcpy( &(pdData->kRec), pkRec, sizeof( KEY_RECORD ) );
	/*
		printf( "[%s] [%s] [%s] [%2d] [%3d] [%3d] [%d] [%3d] [%d] [%3d] [%d] [%d] [%3d] [%4d] [%d] [%d]\n",
										pdData->kRec.szFileHead, 
										pdData->kRec.szSysType, 
										pdData->kRec.szDataType, 
										pdData->kRec.nOperNo,	
										pdData->kRec.nRecLen,	
										pdData->kRec.nUserLoc, 
										pdData->kRec.nUserLen, 
										pdData->kRec.nTimeLoc, 
										pdData->kRec.nTimeLen, 
										pdData->kRec.nDealTimeLoc,
										pdData->kRec.nDealTimeOff,
										pdData->kRec.nDealTimeLen, 
										pdData->kRec.nDetachFlag, 
										pdData->kRec.nFeeUnit,
										pdData->kRec.nStoreMode,
										pdData->kRec.nStatMode );
	*/
		return MY_SUCCEED;
	}
	
	return MY_FAILED;
}
	
int GetDetailFmt( PFMT_FILE pfFile )
{
	PFIELD_FMT pfFmt;
	FILE * fp;
	char szRec[512];
	char * szPos;
	int nFieldNum;
	
	pfFmt = pfFile->pfFmt;

	pfFile->nFieldCount = GetFileCount( pfFile->szFileName );
	if( 0 >= pfFile->nFieldCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单格式描述文件 %s 内容为空\n", pfFile->szFileName );
		return 1;
	}
	
	pfFile->pfFmt = ( PFIELD_FMT ) malloc( ( pfFile->nFieldCount ) * sizeof(FIELD_FMT) );
	if( NULL == pfFile->pfFmt  )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( pfFile->pfFmt , 0, ( pfFile->nFieldCount ) * sizeof(FIELD_FMT) );
	
	if( NULL == ( fp = fopen( pfFile->szFileName, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单格式描述文件 %s 打开失败\n", pfFile->szFileName );
		return 1;
	}

	szPos = NULL;
	pfFmt = pfFile->pfFmt;
	pfFile->nFieldCount = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( pfFmt, 0, sizeof( FIELD_FMT ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					pfFmt->nFieldNo = atoi( szPos );
					break;
				case 1:
					strcpy( pfFmt->szFieldName, szPos );
					break;
				case 2:
					pfFmt->nStart = atoi( szPos );
					break;
				case 3:
					pfFmt->nEnd = atoi( szPos );
					break;
				case 4:
					pfFmt->nLen = atoi( szPos );
					break;
				case 5:
					pfFmt->nType = atoi( szPos );
					break;
				case 6:
					strcpy( pfFmt->szMapNo, szPos );
					if( 0 != strncmp( pfFmt->szMapNo, FIELD_NO_MAP, strlen( FIELD_NO_MAP ) )
					&&  !IsNum( pfFmt->szMapNo ) )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "错误的映射文件编号 [%s]\n", pfFmt->szMapNo );
						return MY_FAILED;
					}	
					break;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_DTL != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单格式描述文件记录格式错误\n" );
			return MY_FAILED;
		}
	
		pfFmt++;		
		pfFile->nFieldCount++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	/* DebugOutDtlFmt( pfFile ); */
	
	return 0;
}

int CtrlCmp( PFMT_FILE pfFile1, PFMT_FILE pfFile2 )
{
	char szKey1[SYS_TYPE_LEN + 14 + 14 + 1];
	char szKey2[SYS_TYPE_LEN + 14 + 14 + 1];
	
	sprintf( szKey1, "%s%s%s", pfFile1->szSysType, pfFile1->szStartTime, pfFile1->szEndTime );
	sprintf( szKey2, "%s%s%s", pfFile2->szSysType, pfFile2->szStartTime, pfFile2->szEndTime );
	
	return ( strcmp( szKey1, szKey2 ) );
}

/********************************************************** 
function: GetFmtCtrlInfo
description: 获取详单格式控制信息
Input: pfCtrl: 描述详单格式控制信息信息的结构体
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int GetFmtCtrlInfo( PFMT_CTRL pfCtrl )
{
	FILE * fp;
	char szRec[512];
	char szPath[512];
	char * szPos;
	int nFieldNum;
	
	PFMT_FILE pfFile;
	
	pfCtrl->nFmtNum = 0;
	pfCtrl->pfFile = NULL;
	strcpy( szPath, szFmtCtrl );
	
	pfCtrl->nFmtNum = GetFileCount( szPath );
	if( 0 >= pfCtrl->nFmtNum )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单格式控制文件 %s 内容为空\n", szPath );
		return 1;
	}

	pfCtrl->pfFile = ( PFMT_FILE ) malloc( ( pfCtrl->nFmtNum ) * sizeof(FMT_FILE) );
	if( NULL == pfCtrl->pfFile  )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( pfCtrl->pfFile, 0, ( pfCtrl->nFmtNum ) * sizeof(FMT_FILE) );
	
	if( NULL == ( fp = fopen( szPath, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "文件 %s 打开失败\n", szPath );
		return 1;
	}

	szPos = NULL;
	pfFile = pfCtrl->pfFile;
	pfCtrl->nFmtNum = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( pfFile, 0, sizeof( FMT_FILE ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					strcpy( pfFile->szSysType, szPos );
					break;
				case 1:
					strcpy( pfFile->szStartTime, szPos );
					break;
				case 2:
					strcpy( pfFile->szEndTime, szPos );
					break;
				case 3:
					strcpy( pfFile->szFileName, szPos );
					break;
				case 4:
					strcpy( pfFile->szKeyFileNo, szPos );
					if( !IsNum( pfFile->szKeyFileNo ) )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "错误的信息文件编号 [%s]\n", pfFile->szKeyFileNo );
						return MY_FAILED;
					}
					break;
				case 5:
					strcpy( pfFile->szFieldMapNo, szPos );
					if( 0 != strncmp( pfFile->szFieldMapNo, FIELD_NO_MAP, strlen( FIELD_NO_MAP ) )
					&&  !IsNum( pfFile->szFieldMapNo ) )
					{
						MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "错误的映射文件编号 [%s]\n", pfFile->szFieldMapNo );
						return MY_FAILED;
					}
					break;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_CTRL != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单格式控制文件记录格式错误\n" );
			return MY_FAILED;
		}

		if( GetDetailFmt( pfFile ) )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单格式控制文件内容错误\n" );
			return MY_FAILED;
		}
		
		pfFile++;		
		pfCtrl->nFmtNum++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	qsort( pfCtrl->pfFile, pfCtrl->nFmtNum, sizeof( FMT_FILE ), (int (*) (const void *,const void *))CtrlCmp );
	
	/* DebugOutFmtCtrlInfo( pfCtrl ); */
	
	return 0;	
}

int GetMapRecord( PMAP_FILE pmFile )
{
	PMAP_RECORD pmRec;
	FILE * fp;
	char szRec[512];
	char * szPos;
	int nFieldNum;
	
	pmFile->nMapCount = GetFileCount( pmFile->szFileName );
	if( 0 >= pmFile->nMapCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段内容映射文件 %s 内容为空\n", pmFile->szFileName );
		return 1;
	}
	
	pmFile->pmRecord = ( PMAP_RECORD ) malloc( ( pmFile->nMapCount ) * sizeof(MAP_RECORD) );
	if( NULL ==pmFile->pmRecord  )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( pmFile->pmRecord , 0, ( pmFile->nMapCount ) * sizeof(MAP_RECORD) );
	
	if( NULL == ( fp = fopen( pmFile->szFileName, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段内容映射文件 %s 打开失败\n", pmFile->szFileName );
		return 1;
	}

	szPos = NULL;
	pmRec = pmFile->pmRecord;
	pmFile->nMapCount = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( pmRec, 0, sizeof( MAP_RECORD ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					strcpy( pmRec->szFieldValue, szPos );
					break;
				case 1:
					strcpy( pmRec->szMapValue, szPos );
					break;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_MAPREC != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段内容映射文件记录格式错误\n" );
			return MY_FAILED;
		}
	
		pmRec++;		
		pmFile->nMapCount++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	qsort( pmFile->pmRecord, pmFile->nMapCount, sizeof( MAP_RECORD ), (int (*) (const void *,const void *))MapCmp );

	/* DebugOutMapRec( pmFile ); */
	
	return 0;
}

int GetMapFileInfo( PMAP_INFO pmInfo )
{
	FILE * fp;
	char szRec[512];
	char szPath[512];
	char * szPos;
	int nFieldNum;
	
	PMAP_FILE pmFile;
	
	pmInfo->nFileCount = 0;
	pmInfo->pmFile = NULL;
	strcpy( szPath, szMapFile );
	
	pmInfo->nFileCount = GetFileCount( szPath );
	if( 0 >= pmInfo->nFileCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段内容映射控制文件内容为空\n" );
		return 0;
	}

	pmInfo->pmFile = ( PMAP_FILE ) malloc( ( pmInfo->nFileCount ) * sizeof(MAP_FILE) );
	if( NULL == pmInfo->pmFile  )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( pmInfo->pmFile , 0, ( pmInfo->nFileCount ) * sizeof(MAP_FILE) );
	
	if( NULL == ( fp = fopen( szPath, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "文件 %s 打开失败\n", szPath );
		return 1;
	}

	szPos = NULL;
	pmFile = pmInfo->pmFile;
	pmInfo->nFileCount = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( pmFile, 0, sizeof( MAP_FILE ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					pmFile->nFileNo = atoi( szPos );
					break;
				case 1:
					strcpy( pmFile->szFileName, szPos );
					break;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_MAP != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段内容映射控制文件记录格式错误\n" );
			return MY_FAILED;
		}

		if( GetMapRecord( pmFile ) )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段内容映射控制文件内容错误\n" );
			return MY_FAILED;
		}

		pmFile++;		
		pmInfo->nFileCount++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	/* DebugOutMapFileInfo( pmInfo ); */
	
	return 0;	
}

int GetFieldMapRecord( PFMAP_FILE pfmFile )
{
	PFMAP_RECORD pfmRec;
	FILE * fp;
	char szRec[512];
	char * szPos;
	int nFieldNum;
	
	pfmFile->nFieldCount = GetFileCount( pfmFile->szFileName );
	if( 0 >= pfmFile->nFieldCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单序号映射文件 %s 内容为空\n", pfmFile->szFileName );
		return 1;
	}
	
	pfmFile->pfmRecord = ( PFMAP_RECORD ) malloc( ( pfmFile->nFieldCount ) * sizeof(FMAP_RECORD) );
	if( NULL ==pfmFile->pfmRecord  )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( pfmFile->pfmRecord , 0, ( pfmFile->nFieldCount ) * sizeof(FMAP_RECORD) );
	
	if( NULL == ( fp = fopen( pfmFile->szFileName, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单序号映射文件 %s 打开失败\n", pfmFile->szFileName );
		return 1;
	}

	szPos = NULL;
	pfmRec = pfmFile->pfmRecord;
	pfmFile->nFieldCount = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( pfmRec, 0, sizeof( FMAP_RECORD ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					strcpy( pfmRec->szFieldNo, szPos );
					break;
				case 1:
					strcpy( pfmRec->szMapNo, szPos );
					break;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_FMAPREC != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单序号映射文件记录格式错误\n" );
			return MY_FAILED;
		}
	
		pfmRec++;		
		pfmFile->nFieldCount++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	qsort( pfmFile->pfmRecord, pfmFile->nFieldCount, sizeof( FMAP_RECORD ), (int (*) (const void *,const void *))FieldMapCmp );

	/* DebugOutFieldMapRec( pfmFile ); */
	
	return 0;
}

int GetFieldMapInfo( PFIELD_MAP pfMap )
{
	FILE * fp;
	char szRec[512];
	char szPath[512];
	char * szPos;
	int nFieldNum;
	
	PFMAP_FILE pfmFile;
	
	pfMap->nFileCount = 0;
	pfMap->pfmFile = NULL;
	strcpy( szPath, szFieldMapFile );
	
	pfMap->nFileCount = GetFileCount( szPath );
	if( 0 >= pfMap->nFileCount )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段序号映射控制文件内容为空\n" );
		return 0;
	}

	pfMap->pfmFile = ( PFMAP_FILE ) malloc( ( pfMap->nFileCount ) * sizeof(FMAP_FILE) );
	if( NULL == pfMap->pfmFile  )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请内存失败\n" );
		return 1;
	}
	memset( pfMap->pfmFile , 0, ( pfMap->nFileCount ) * sizeof(FMAP_FILE) );
	
	if( NULL == ( fp = fopen( szPath, "r" ) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "文件 %s 打开失败\n", szPath );
		return 1;
	}

	szPos = NULL;
	pfmFile = pfMap->pfmFile;
	pfMap->nFileCount = 0;

	memset( szRec, 0, 512 );
		
	while( NULL != fgets( szRec, 512, fp ) )
	{
		if( !IsRecordLine( szRec ) )
		{
			continue;
		}
		
		nFieldNum = 0;
		memset( pfmFile, 0, sizeof( FMAP_FILE ) );
		
		szPos = strtok( szRec, FILE_FIELD_SEP );
		while( NULL != szPos )
		{
			switch( nFieldNum )
			{
				case 0:
					pfmFile->nFileNo = atoi( szPos );
					break;
				case 1:
					strcpy( pfmFile->szFileName, szPos );
					break;
			}
			
			nFieldNum++;
			szPos = strtok( NULL, FILE_FIELD_SEP );
		}
		
		if( FIELD_NUM_FMAP != nFieldNum )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段序号映射控制文件记录格式错误\n" );
			return MY_FAILED;
		}

		if( GetFieldMapRecord( pfmFile ) )
		{
			fclose( fp );
			MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "详单字段序号映射控制文件内容错误\n" );
			return MY_FAILED;
		}

		pfmFile++;		
		pfMap->nFileCount++;
		memset( szRec, 0, 512 );
	}

	fclose( fp );
	
	/* DebugOutFieldMapInfo( pfMap ); */
	
	return 0;	
}

/********************************************************** 
function: GetSearchData
description: 获取查询配置信息
Input: psData: 描述系统全局结构的结构体
Output:
Return: 0 成功 1: 失败
others:
***********************************************************/

int GetSearchData( PSEARCH_DATA psData )
{
	/* 获取索引文件信息 */
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "获取详单数据文件存储信息\n" );
	if( GetFileIndex( psData ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法获取详单数据文件存储信息\n" );
		return MY_FAILED;
	}
	
	/* 获取查询关键字在该业务详单中的位置 */
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "获取查询关键字位置信息!\n" );
	if( GetKeyControlInfo( &(psData->kControl) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法获取查询关键字信息!\n\n" );
		return MY_FAILED;
	}

	/* 获取详单格式控制信息 */
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "获取详单格式控制信息!\n" );
	if( GetFmtCtrlInfo( &(psData->fCtrl) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法获取详单格式控制信息!\n\n" );
		return MY_FAILED;
	}
		
	/* 获取详单字段内容映射文件控制信息 */
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "获取详单字段内容映射控制文件信息!\n\n" );
	if( GetMapFileInfo( &(psData->mInfo) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法获取详单字段内容映射控制文件信息!\n\n" );
		return MY_FAILED;
	}
	
	/* 获取详单字段序号内容映射控制文件信息 */
	MyLog( INFO_SOURCE_APP, INFO_LEVEL_GEN, "获取详单字段序号映射控制文件信息!\n\n" );
	if( GetFieldMapInfo( &(psData->fMap) ) )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_WARN, "无法获取详单字段序号映射控制文件信息!\n\n" );
		return MY_FAILED;
	}
	
	/* 申请保存返回结果包的存储空间 */
	psData->szPkgBuff = ( char * ) malloc( atol( psData->szPkgSize ) + 1 );
	if( NULL == psData->szPkgBuff )
	{
		MyLog( INFO_SOURCE_APP, INFO_LEVEL_EXIT, "申请返回结果包存储空间失败!\n\n" );
		return MY_FAILED;
	}

	return MY_SUCCEED;
}
