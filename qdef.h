
/********************************************************************
Copyright (C), 1995-2005, Si-Tech Information Technology Ltd.
file_name: qdef.h
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 定义查询数据结构
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ...
********************************************************************/


#ifndef	_Q_DEF_H_
#define	_Q_DEF_H_

#include "common.h"

#define	MAX_MOBILEHEAD_NUM			20					/*  */

#define	MAX_HEAD_NUM				100000				/* 号段最大个数 */
#define	MAX_FIELDNAME_LEN			32					/* 字段名最大长度 */
#define	MAX_FIELD_LEN				128					/* 字段取值最大长度 */
#define	MAX_COND_NUM				100					/* 条件表达式最大项 */

#define	REC_SIZE					2048				/* 记录最大长度 */

#define	SYS_TYPE_LEN				8					/* 业务类型字段长度 */
#define	REC_TYPE_LEN				2					/* 记录类型字段长度 */
#define	SVC_ID_LEN					100					/* 服务号码长度 */
#define	SVC_OFF_LEN					4

#define	HEAD_FLAG_LEN				2

#define	PRCO_CTRL_FIELD				"RUNFLAG"			/* 程序运行控制字段名 */
#define	DEBUG_FLAG_FIELD			"DEBUGFLAG"			/* 程序运行控制字段名 */
#define	PROC_CTRL_RUN				0					/* 程序继续运行 */
#define	PROC_CTRL_EXIT				1					/* 程序停止运行 */
#define	FIELD_NO_MAP				"*"					/*  */

#define	CMD_FIELD_SEP				"/"

#define	SHOW_DEGUG_INFO				1
#define	SHIELD_DEBUG_INFO			0

#define	STORE_MODE_MOBILE			0					/* 按手机号码存储 */
#define	STORE_MODE_FIXED			1					/* 按固定号码分业务取模存储 */
#define	STORE_MODE_MODULUS			2					/* 按号码不分业务取模存储 */

typedef	struct
{
	char szSvcID[SVC_ID_LEN + 1];				/* 服务号码 */
	char szStartTime[14 + 1];					/* 查询开始时间 */
	char szEndTime[14 + 1];						/* 查询结束时间 */
	char szSysType[SYS_TYPE_LEN + 1];			/* 业务类型 */
	char szRecType[REC_TYPE_LEN + 1];			/* 记录类型 */
	char szSearchField[512];					/* 数据项选择表达式 */
	char szSearchCond[2056];					/* 查询条件 */

	char szBaseAddr[SVC_ID_LEN + 1];			/* 用户号码基址 */
	char szOffset[SVC_OFF_LEN + 1];				/* 用户号码偏移 */
	
	int nStoreMode;								/* 详单记录存储模式 */
	int nDetachNo;
} SEARCH_CMD;
typedef	SEARCH_CMD * PSEARCH_CMD;

typedef	struct
{
	char szFileHead[32 + 1];					/* 文件头 */
	char szSysType[SYS_TYPE_LEN + 1];			/* 系统代码 */
	char szDataType[16 + 1];					/* 数据类型 */
	
	int nOperNo;								/* 操作编号 */
	int nRecLen;								/* 记录长度 */
	int nUserLoc;								/* 用户字段位置 */
	int nUserLen;								/* 用户字段长度 */
	int nTimeLoc;								/* 时间字段位置 */
	int nTimeLen;								/* 时间字段长度 */
	int nDealTimeLoc;							/* 处理时间字段位置 */
	int nDealTimeOff;							/* 处理时间字段偏移 */
	int nDealTimeLen;							/* 处理时间字段长度 */
	int nDetachFlag;							/* 拆分标志 */
	int nFeeUnit;								/* 基本费用单位 */
	int nStoreMode;								/* 存储模式 */
	int nStatMode;								/* 增量数据统计方式 */
	
} KEY_RECORD;
typedef	KEY_RECORD * PKEY_RECORD;

typedef struct
{
	PKEY_RECORD pkRecord;
	int nKeyCount;
	
	char szFileName[256];
	int nFileNo;
	
} SEARCH_KEY;
typedef	SEARCH_KEY * PSEARCH_KEY;

typedef struct	_KEY_CONTROL
{
	PSEARCH_KEY psKey;
	
	int nKeyCount;

} KEY_CONTROL;
typedef KEY_CONTROL * PKEY_CONTROL;
	
typedef	struct _SEARCH_INDEX
{
	char szHostName[32 + 1];
	char szFileSys[64 + 1];						/* 文件系统 */
	char szSubDir[64 + 1];						/* 子目录 */
	char szH1H2H3[16 + 1];						/* 号码头 */
	
	char szKey[16 + 1];
	int nBeginKey;
	int nEndKey;
	
	char szChgDate[14 + 1];						/* 变更日期 */
	char szOldSys[64 + 1];						/* 旧文件系统 */
	char szOldSub[64 + 1];						/* 旧子目录 */
	char szStoreMode[1 + 1];					/* 详单存储模式 */
	
	struct _SEARCH_INDEX * psiNext;
	
} SEARCH_INDEX;
typedef	SEARCH_INDEX * PSEARCH_INDEX;

typedef struct _SEARCH_HEAD
{
	PSEARCH_INDEX psIndex[MAX_HEAD_NUM];
	
	char szHeadFlag[128 + 1];
	
} SEARCH_HEAD;
typedef SEARCH_HEAD * PSEARCH_HEAD;

typedef	struct	_FIELD_FMT
{
	int nFieldNo;								/* 字段编号 */
	char szFieldName[MAX_FIELDNAME_LEN + 1];	/* 字段名 */
	int nStart;									/* 字段在详单中的开始位置 */
	int nEnd;									/* 字段在详单中的结束位置 */
	int nLen;									/* 字段在详单中的长度 */
	int nType;									/* 字段数据类型 */
	char szMapNo[3 + 1];						/* 映射文件编号 */
} FIELD_FMT;
typedef	FIELD_FMT * PFIELD_FMT;

typedef	struct
{
	PFIELD_FMT pfFmt;
	int nFieldCount;							/* 详单字段数目 */
	
	char szSysType[SYS_TYPE_LEN + 1];			/* 业务类型 */
	char szStartTime[14 + 1];					/* 生效开始时间 */
	char szEndTime[14 + 1];						/* 生效结束时间 */
	char szFileName[256];						/* 详单格式描述文件 */
	char szKeyFileNo[3 + 1];					/* 查询关键字位置信息文件 */
	char szFieldMapNo[3 + 1];					/* 详单字段序号映射文件 */
} FMT_FILE;
typedef FMT_FILE * PFMT_FILE;

typedef struct
{
	PFMT_FILE pfFile;
	int nFmtNum;
} FMT_CTRL;
typedef FMT_CTRL * PFMT_CTRL;

typedef	struct
{
	char szFieldValue[MAX_FIELD_LEN + 1];
	char szMapValue[128 + 1];
} MAP_RECORD;
typedef MAP_RECORD * PMAP_RECORD;

typedef	struct
{
	PMAP_RECORD pmRecord;
	int nMapCount;
		
	char szFileName[256];
	char szFieldValue[MAX_FIELD_LEN + 1];	/* LU */
	char szMapValue[128 + 1];
	int nFileNo;
} MAP_FILE;
typedef MAP_FILE * PMAP_FILE;

typedef	struct
{
	PMAP_FILE pmFile;
	int nFileCount;
} MAP_INFO;
typedef MAP_INFO * PMAP_INFO;


typedef	struct _FMAP_RECORD
{
	char szFieldNo[3 + 1];
	char szMapNo[3 + 1];
} FMAP_RECORD;
typedef FMAP_RECORD * PFMAP_RECORD;

typedef	struct _FMAP_FILE
{
	PFMAP_RECORD pfmRecord;
	int nFieldCount;
	
	char szFileName[256];
	int nFileNo;
} FMAP_FILE;
typedef FMAP_FILE * PFMAP_FILE;

typedef	struct _FIELD_MAP
{
	PFMAP_FILE pfmFile;
	int nFileCount;
} FIELD_MAP;
typedef FIELD_MAP * PFIELD_MAP;


typedef	struct _SEL_FIELD
{
	int nFieldNo;									/* 字段编号 */
} SEL_FIELD;
typedef	SEL_FIELD * PSEL_FIELD;

typedef	struct _SEL_INFO
{
	PSEL_FIELD psField;
	int nSelCount;									/* 统计字段数目 */
} SEL_INFO;
typedef	SEL_INFO * PSEL_INFO;

typedef	struct _COND_FIELD
{
	int nFieldNo;									/* 字段编号 */
	int nOptFlag;									/* 操作标志 */
	char szValue[128];								/* 操作值 */
} COND_FIELD;
typedef	COND_FIELD * PCOND_FIELD;

typedef	struct _COND_INFO
{
	COND_FIELD pcField[MAX_COND_NUM];
	int nCondCount;								/* 条件字段个数 */
} COND_INFO;
typedef	COND_INFO * PCOND_INFO;

typedef	struct _SERVICE_INDEX
{
	char szSysType[SYS_TYPE_LEN + 1];							/* 系统代码 */
	
	ulong ulDetailOff;
	ulong ulDetailLen;
	ulong ulOriginLen;
	
	int nUserNo;
	int nDay;
	
	struct _SERVICE_INDEX * psiUser;
	struct _SERVICE_INDEX * psiNext;
	
} SERVICE_INDEX;
typedef	SERVICE_INDEX * PSERVICE_INDEX;

typedef	struct _BASE_INDEX
{
	PSERVICE_INDEX psIndex;
	
	ulong ulOffset;
	
	int nUserNo;
	int nDay;
	int nService;
	
} BASE_INDEX;
typedef BASE_INDEX * PBASE_INDEX;

typedef	struct
{
	KEY_RECORD kRec;
	SEL_INFO sInfo;
	COND_INFO cInfo;
	PSEARCH_KEY psKey;
	PFMT_FILE pfFile;
	PFMAP_FILE pfmFile;
	SERVICE_INDEX sIndex;

	char bCond;
	char bCurrQuery;
	
	char szSvcID[SVC_ID_LEN + 1];				/* 服务号码 */
	char szStartTime[14 + 1];					/* 查询开始时间 */
	char szEndTime[14 + 1];						/* 查询结束时间 */
	char szSysType[SYS_TYPE_LEN + 1];			/* 业务类型 */
	char szRecType[REC_TYPE_LEN + 1];			/* 记录类型 */	
	char szCondExpr[2056];
	
	char szFilePath[512];						/* 详单文件绝对路径 */
	char szOldPath[512];						/* 详单文件绝对路径 */
	char szChgDate[14 + 1];						/* 变更日期 */
	char szCurrFile[512];						/* 当前检索的详单文件 */
	
	char szFileHead[32 + 1];					/* 当前检索文件标志 */
	char szQueryMonth[14 + 1];					/* 当前检索的月份 */
	char szQueryDate[14 + 1];					/* 当前检索的日期 */
	
	char szResult[REC_SIZE + 1];				/*  */
	
	int nCurrHandle;							/* 当前处理文件句柄 */
	
	FILE * fpCurr;
	
	long lFileSize;								/* 文件大小 */
	long lTotalRec;								/* 符合检索条件的所有详单记录数 */
	long lOffset;								/* 结果包偏移 */
	
	long lPageNum;
	int nPkgNum;
	
	int nStoreMode;
	
} DETAIL_DATA;
typedef	DETAIL_DATA * PDETAIL_DATA;

typedef	struct
{
	SEARCH_CMD sCmd;							/* 保存查询指令 */
	KEY_CONTROL kControl;						/* 保存查询关键字信息 */
	
	SEARCH_HEAD psHead[MAX_MOBILEHEAD_NUM];		/* 保存详单存储路径 */
	PSEARCH_INDEX psiCard;						/* 保存非手机号码详单存储路径 */
	FMT_CTRL fCtrl;								/* 保存详单格式控制信息 */
	MAP_INFO mInfo;								/* 映射文件信息 */
	FIELD_MAP fMap;								/* 详单字段序号映射文件信息 */

	char szPageSize[16];						/* 前台一个显示页面最大显示记录数 */
	char szPkgSize[16];							/* 结果包大小 */
	char * szPkgBuff;							/* 指向保存查询结果的数据包的指针 */
	
} SEARCH_DATA;
typedef SEARCH_DATA * PSEARCH_DATA;

#endif
