#pragma once
#include <string>
#include <H5Cpp.h>
class CAbstractDataType
{
public:
	CAbstractDataType();
	virtual ~CAbstractDataType();

public:
	virtual std::string GetDataTypeName() = 0;

	//生产正确的类型
	virtual H5::DataType* CreateProperType() = 0;

};

