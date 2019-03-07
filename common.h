
/******************************************************************** 
Copyright (C), 1995-2005, Si-Tech Information Technology Ltd. 
file_name: common.h
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 定义公用数据、函数
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ... 
********************************************************************/


#ifndef	_COMMON_H_
#define	_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdarg.h>


#define	MY_TRUE					1				/* 自定义逻辑真值 */
#define	MY_FALSE				0				/* 自定义逻辑假值 */
#define	MY_SUCCEED				0				/* 自定义成功状态值 */
#define	MY_FAILED				1				/* 自定义失败状态值 */

#define	DATE_YEAR_LEN			4				/* 年标准长度 */
#define	DATE_MON_LEN			2				/* 月标准长度 */
#define	DATE_DAY_LEN			2				/* 日标准长度 */
#define	DATE_HOUR_LEN			2				/* 小时标准长度 */
#define	DATE_MIN_LEN			2				/* 分钟标准长度 */
#define	DATE_SEC_LEN			2				/* 秒标准长度 */
#define	DATE_STD_LEN			14				/* 日期标准长度 */

#define	INFO_SOURCE_APP			"app"			/* 进程自身产生的信息 */
#define	INFO_SOURCE_DB			"db"			/* 数据库产生的信息 */
#define	INFO_SOURCE_SYS			"sys"			/* 除进程自身、数据库以外的信息, 如因进程调度等其他原因产生的信息 */

#define	INFO_LEVEL_GEN			"gen"			/* 普通信息 */
#define	INFO_LEVEL_WARN			"warn"			/* 告警信息 */
#define	INFO_LEVEL_EXIT			"exit"			/* 严重告警 */

#define	FILE_NOTE_FLAG			"#"				/* 文件记录注释标志 */

#ifndef ulong
#define	ulong					unsigned long
#endif

char szLogPath[2056];							/* 日志文件存放路径 */
char szCfgPath[2056];							/* 配置文件存放路径 */
char szIndFile[2056];							/* 数据文件存储路径文件 */
char szKeyFile[2056];							/* 查询关键字信息文件 */
char szFmtCtrl[2056];							/* 详单格式控制文件 */
char szMapFile[2056];							/* 映射文件信息 */
char szFieldMapFile[2056];						/* 字段序号映射文件 */
char szCmdInfo[2056];							/* 查询指令(测试用) */

int nDebugFlag;									/*  */

#ifndef	max
#define	max( a, b )			( a < b ) ? b : a	/* 求两个数的最大值 */
#endif
#ifndef	min
#define	min( a, b )			( a > b ) ? b : a	/* 求两个数的最小值 */
#endif

#endif
