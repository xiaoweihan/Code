#include "stdafx.h"
#include "FloatDataType.h"


CFloatDataType::CFloatDataType(unsigned int nDataByte):
	m_nDataBytes(nDataByte)
{

}

CFloatDataType::~CFloatDataType()
{
}

std::string CFloatDataType::GetDataTypeName()
{
	return "FloatType";
}

H5::DataType* CFloatDataType::CreateProperType()
{
	using namespace H5;
	DataType* pDataType = nullptr;
	switch (m_nDataBytes)
	{
	case 4:
		pDataType = new FloatType(PredType::IEEE_F32LE);
		break;
	case 8:
		pDataType = new FloatType(PredType::IEEE_F64LE);
		break;
	default:
		break;
	}

	return pDataType;
}
