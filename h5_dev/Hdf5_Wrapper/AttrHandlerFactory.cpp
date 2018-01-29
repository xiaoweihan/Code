#include "stdafx.h"
#pragma warning(disable:4251)
#include "AttrHandlerFactory.h"
#include "FloatAtrrHandler.h"
#include "IntAttrHandler.h"
#include "StrAttrHandler.h"
CAbstractAttrHanlder* CreateAttrHandler(Hdf5_Wrapper::DATA_TYPE eumDataType, unsigned int nDataByte)
{
	CAbstractAttrHanlder* pHandler = nullptr;
#if 1
	switch (eumDataType)
	{
	case Hdf5_Wrapper::DATA_TYPE::FLOAT_TYPE:
		pHandler = new CFloatAtrrHandler(nDataByte);
		break;

	case Hdf5_Wrapper::DATA_TYPE::INT_TYPE:
		pHandler = new CIntAttrHandler(nDataByte);
		break;
	case Hdf5_Wrapper::DATA_TYPE::STR_TYPE:
		pHandler = new CStrAttrHandler;
		break;
	default:
		break;
	}
#endif
	return pHandler;
}

CAbstractAttrHanlder* CreateAttrHandler(int nHdf5DataType, unsigned int nDataByte)
{
	using namespace H5;

	CAbstractAttrHanlder* pHandler = nullptr;

	switch (nHdf5DataType)
	{
	case H5T_INTEGER:
		pHandler = new CIntAttrHandler(nDataByte);
		break;
	case H5T_FLOAT:
		pHandler = new CFloatAtrrHandler(nDataByte);
		break;
	case H5T_STRING:
		pHandler = new CStrAttrHandler;
		break;
	default:
		break;
	}
	return pHandler;
}
