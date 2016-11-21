/* 
 * File:   e_type.h
 * Author: Sam
 *
 * Created on 2015年7月13日, 下午2:09
 */

#ifndef E_TYPE_H
#define	E_TYPE_H
#include "e_os_config.h"
#include<stdint.h>
#ifdef	__cplusplus
extern "C" {
#endif
	/*
//平台相关的基本变量类型定义
typedef unsigned char uint8 ;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;
*/

	typedef unsigned char uint8;
	typedef signed char int8;
	typedef unsigned short uint16;
	typedef signed short int16;
#ifdef __LP64__                   /*This flag is defined in Linux 64*/
	typedef unsigned int uint32;
	typedef signed int int32;
#else /*assume ILP32LL*/
	typedef unsigned long uint32;
	typedef signed long int32;
#endif
	/*
	* [u]int24 is defined as [u]int32. This is the only data type which is not
	* "exactly the size" that it represents.
	*/

	typedef unsigned long long uint64;
	typedef signed long long int64;
	typedef uint32 uint24;
	typedef int32 int24;

	typedef double float64;         /*used in test engine apis*/

#ifndef NULL
#define NULL     0
#endif
#ifdef	__cplusplus
}
#endif

#endif	/* E_TYPE_H */



