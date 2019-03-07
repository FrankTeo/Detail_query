 
/********************************************************************
Copyright (C), 1995-2005, Si-Tech Information Technology Ltd. 
file_name: qret.h
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 定义查询数据参数
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ... 
********************************************************************/


#ifndef	_QRET_H_
#define	_QRET_H_

#include "qdef.h"

#define	FILE_FIELD_SEP		"\t "		/* 数据文件记录字段的分隔符 */
#define	SEL_FIELD_SEP		"|"			/* */
#define	RET_FIELD_SEP		"|"			/* */
#define	EET_RECORD_SEP		"\n"		/* */
#define	NOT_NEED_MAP		"*"			/* */
#define	FIELD_VAR_FLAG		"$"			/* */

#define	MAX_SUBSCRIBER_LEN	25			/* 用户号码最大长度 */

#define	FIELD_NUM_PATH		7			/* 详单文件存储目录信息文件记录字段数 */

#ifdef	_SICHUAN_VERSION_
#define	FIELD_NUM_KEY		15			/* 查询关键字位置信息文件记录字段数 */
#else
#define	FIELD_NUM_KEY		16			/* 查询关键字位置信息文件记录字段数 */
#endif

#define	FIELD_NUM_SCHEMA	7			/* 查询方案信息文件记录字段数 */
#define	FIELD_NUM_CTRL		6			/*  */
#define	FIELD_NUM_DTL		7			/*  */
#define	FIELD_NUM_KCTRL		2			/*  */
#define	FIELD_NUM_KREC		2			/*  */
#define	FIELD_NUM_MAP		2			/*  */
#define	FIELD_NUM_MAPREC	2			/*  */
#define	FIELD_NUM_FMAP		2			/*  */
#define	FIELD_NUM_FMAPREC	2			/*  */

#define RECORD_VALID_START	0			/* 详单有效标志开始位置 */
#define RECORD_VALID_LEN	2			/* 详单有效标志长度 */
#define RECORD_VALID_FLAG	"*"			/* 详单无效标志 */

#define	NOT_CMP_SYSTYPE		"*"			/*  */
#define	NOT_CMP_RECTYPE		"**"		/*  */
#define OLD_END_TIME        "20040302"	/*  */

#define	MAP_DEFAULT_VALUE	"*"			/*  */

#endif
