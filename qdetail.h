
/******************************************************************** 
Copyright (C), 1995-2005, Si-Tech Information Technology Ltd. 
file_name: qdetail.h
author: 张凡 version: V07.00.000 date: 2004-09-20
description: 定义查询参数
others:
history: 1. date: 修改时间 version: 版本 author: 作者
            modification: 修改内容
	 2. ... 
********************************************************************/


#ifndef	_QDETAIL_H_
#define	_QDETAIL_H_

#include "qdef.h"
#include "zlib.h"

#define	INDEX_FILE_TAIL				".index"		/* 索引文件后缀 */
#define	FLLL_CHAR					'*'				/* 填充符号 */
#define	END_FLAG					'\n'			/* 结束标志 */
#define	FIXED_FILE_PREFIX			"fixed"			/*  */

#define CURR_FILE_TAIL				".cur"			/* 当日详单存储文件扩展名 */
#define DAY_FILE_TAIL				".day"			/* 往日详单存储文件扩展名 */
#define OLD_FILE_TAIL				".old"			/* old文件扩展名 */
#define MONTH_FILE_TAIL				".month"		/* 往月详单存储文件扩展名 */
#define INDEX_FILE_TAIL				".index"		/* 索引文件扩展名 */

#define	CURR_DIR_NAME				"curr"			/* 当天详单文件存储目录 */
#define	COMM_DIR_NAME				"day"			/* 当月详单文件存储目录 */
#define	ARCH_DIR_NAME				"month"			/* 往月详单文件存储目录 */
#define	INDEX_DIR_NAME				"index"			/* 索引文件存储目录 */

#define	MAX_USER_NUM				10000			/*  */

#define	BLOCK_SIZE					3000			/* 存储数据块大小 */
#define	BASE_ADDR_LEN				4				/* 基址长度 */
#define	BLOCK_HEAD_LEN				24				/* 数据块头长度 */
#define BHEAD_FLAG_LEN              1               /* 数据块操作标志长度 */
#define BHEAD_VERIFY_LEN            4               /* 校验位长度 */
#define BHEAD_SIZE_LEN              9               /* 保存数据块大小的数据长度 */
#define BHEAD_LINK_LEN              9               /* 保存数据块连接指针的数据长度 */
#define BHEAD_END_LEN               1               /* 数据块头信息结束标志 */

#define BLOCK_FREE_FLAG             "D"
#define BLOCK_FULL_FLAG             "O"

#define	REC_HOLD_CHAR				"  "			/* 详单头部保留字段 */
#define	FILE_NAME_FLAG				"FILENAME:"		/*  */

#define	FILE_HEAD_LEN				7				/*  */

#endif
