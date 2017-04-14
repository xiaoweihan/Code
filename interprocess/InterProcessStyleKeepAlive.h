/**************************************************************************
Copyright(C):
FileName:InterProcessStyleKeepAlive.h
Description:使用进程间通信方式保持心跳
Author:xiaowei.han
Date:2017/03/31
Others:
Histroy:
**************************************************************************/
#ifndef INTER_PROCESS_STYLE_KEEP_ALIVE_H
#define INTER_PROCESS_STYLE_KEEP_ALIVE_H
#include <string>
#include "AbstractKeepAliveStrategy.h"
class CInterProcessStyleKeepAlive : public CAbstractKeepAliveStrategy
{

public:
	CInterProcessStyleKeepAlive(const std::string& strSharedMemoryName, const std::string& strNamedMutexName);
	~CInterProcessStyleKeepAlive(void);
	//实现KeepAlive的虚函数
	void KeepAlive(void) override;


private:
	//初始化共享内存
	void InitSharedMemory(void);
	//反初始化共享内存
	void UnInitSharedMemory(void);
	//初始化命名互斥体
	void InitNamedMutex(void);
	//反初始化命名互斥体
	void UnInitNamedMutex(void);
private:
	//用于进程同步的命名互斥体名称
	std::string m_strNamedMutexName;
	//用于进程通信的共享内存名称
	std::string m_strSharedMemoryName;
};
#endif