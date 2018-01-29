#pragma once
#include "AbstractDataType.h"
class CStrDataType :
	public CAbstractDataType
{
public:
	CStrDataType(unsigned int nDataByte);
	~CStrDataType();



public:
	std::string GetDataTypeName() override;

	H5::DataType* CreateProperType() override;

private:
	unsigned int m_nDataByte = 0;
};

