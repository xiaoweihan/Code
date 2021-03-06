/************************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:Utility.h
Description:公共接口
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
************************************************************************************/
#ifndef UTILITY_H
#define UTILITY_H
#include <string>
namespace Utility
{
	//获取可执行文件的绝对路径
	std::string GetExeDirecory(void);

    //多字节向宽字节转换
    std::wstring MultiByte2WideChar(const char* szSrc);

    //宽字节向多字节转换
    std::string WideChar2MultiByte(const wchar_t* szSrc);
}

//删除数组的宏
#define  DELETE_ARRAY_POINTER(p)              if (nullptr != p)    \
{                    \
	delete []p;   \
	p = nullptr;  \
}

//删除非数组指针
#define  DELETE_POINTER(p)                             if (nullptr != p)    \
{                   \
	delete p;   \
	p = nullptr;  \
}   

//计算数组大小
#define ARRAY_SIZE(array)     (sizeof(array) / sizeof(array[0]))
#endif