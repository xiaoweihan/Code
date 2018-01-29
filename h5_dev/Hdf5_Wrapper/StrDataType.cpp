#include "stdafx.h"
#include "StrDataType.h"

CStrDataType::CStrDataType(unsigned int nDataByte):
	m_nDataByte(nDataByte)
{

}

CStrDataType::~CStrDataType()
{
}

std::string CStrDataType::GetDataTypeName()
{
	return "StrType";
}

H5::DataType* CStrDataType::CreateProperType()
{
	using namespace H5;
	return new StrType(PredType::C_S1, m_nDataByte);
}
