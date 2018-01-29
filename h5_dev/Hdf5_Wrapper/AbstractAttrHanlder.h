#pragma once
#include <H5Cpp.h>
#include "Hdf5_Wrapper.h"

class CAbstractAttrHanlder
{
public:
	CAbstractAttrHanlder() {};
	virtual ~CAbstractAttrHanlder() {};

public:
	//∂¡ Ù–‘
	virtual void WriteAttr(H5::DataSet& ElementDataSet, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) = 0;

	//–¥ Ù–‘
	virtual void WriteAttr(H5::Group& GroupNode, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) = 0;

	//∂¡ Ù–‘
	virtual void ReadAttr(H5::Attribute& Attr,H5::DataType& AttrDataType,std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray) = 0;

protected:
	unsigned int m_nDataByte = 0;
};

