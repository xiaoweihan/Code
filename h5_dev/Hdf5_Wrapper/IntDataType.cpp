#include "stdafx.h"
#include "IntDataType.h"

CIntDataType::CIntDataType(unsigned int nDataBytes, bool bSign):
	m_nDataBytes(nDataBytes),
	m_bSign(bSign)
{

}

CIntDataType::~CIntDataType()
{
}

std::string CIntDataType::GetDataTypeName()
{
	return "IntType";
}

H5::DataType* CIntDataType::CreateProperType()
{
	using namespace H5;

	DataType* pDataType = nullptr;

	if (m_bSign)
	{
		switch (m_nDataBytes)
		{
			//1个字节
		case 1:
			pDataType = new IntType(PredType::STD_I8LE);
			break;
			//2个字节
		case 2:
			pDataType = new IntType(PredType::STD_I16LE);
			break;
			//4个字节
		case 4:
			pDataType = new IntType(PredType::STD_I32LE);
			break;
			//8个字节
		case 8:
			pDataType = new IntType(PredType::STD_I64LE);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (m_nDataBytes)
		{
			//1个字节
		case 1:
			pDataType = new IntType(PredType::STD_I8LE);
			break;
			//2个字节
		case 2:
			pDataType = new IntType(PredType::STD_U16LE);
			break;
			//4个字节
		case 4:
			pDataType = new IntType(PredType::STD_U32LE);
			break;
			//8个字节
		case 8:
			pDataType = new IntType(PredType::STD_U64LE);
			break;
		default:
			break;
		}
	}

	return pDataType;
}
