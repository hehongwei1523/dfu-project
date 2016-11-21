/* 
 * File:   e_os_config.h
 * Author: sam
 *
 * Created on 2015年8月11日, 下午6:18
 */

#ifndef E_OS_CONFIG_H
#define	E_OS_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
 /**********************************************调试相关的选项*************************************************************************************/   
//如果定义OPEN_E_LIST_DEBUG宏，将会打开链表调试的LOG
//#define OPEN_E_LIST_DEBUG
    
//如果打开OPEN_MEM_MANAGER_DEBUG ,将会打开内存管理调试的LOG
//#define  OPEN_MEM_MANAGER_DEBUG

//#define OPEN_MEM_MANAGER_CALL_INFO 
//如果打开OPEN_SCHEDULER_DEBUG，将会打开任务调度调试的LOG
//#define OPEN_SCHEDULER_DEBUG
/*************************************************平台相关的选项***************************************************************************************/
//如果是在linux下或者运行
//#define LINUX
//如果是采用GCC进行编译
//#define GCC
#ifdef	__cplusplus
}
#endif

#endif	/* E_OS_CONFIG_H */

