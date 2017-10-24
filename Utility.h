/************************************************************************************
Copyright(C):
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
#include <boost/checked_delete.hpp>
namespace Utility
{
	//获取可执行文件的绝对路径
	std::string GetExeDirecory(void);

    //多字节向宽字节转换
    std::wstring MultiByte2WideChar(const char* szSrc);

    //宽字节向多字节转换
    std::string WideChar2MultiByte(const wchar_t* szSrc);

#if 0
	//从硬盘位图文件加载位图
	HBITMAP LoadBitmapFromFile(const char* szBitmapFile);

	int AfxBCGPMessageBox(const CString& strText,UINT nType = MB_OK);

	BYTE CalCRC8(BYTE* pBuf,int nsize);

	//UTF-8->GBK
	std::string ConverUTF8ToGB2312( const std::string& strContent );

	//UTF-8->ASCII
	std::string ConverUTF8ToASCII(std::string& strContent);
#endif
}

//删除对象的函数模板
template <typename T>
void SafeDeleter(T* p)
{
	using namespace boost;
	checked_delete(p);
}

template <typename T>
void SafeArrayDeleter(T* p)
{
	using namespace boost;
	checked_array_delete(p);
}
//计算数组大小
#define ARRAY_SIZE(array)     (sizeof(array) / sizeof(array[0]))
#endif