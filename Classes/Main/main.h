/* 头文件保护宏 */
#ifndef __MAIN_H__
#define __MAIN_H__
//防止重复包含同一个头文件

#define WIN32_LEAN_AND_MEAN   
//排除Windows头文件中的不常用的内容 减少编译时间和可执行文件的大小

#include <windows.h> //Windows API的主要头文件
#include <tchar.h>   //提供对宽字符和多字符的兼容性支持
#include "platform/CCStdC.h" //包含一些C标准库的头文件

#endif //头文件结束处