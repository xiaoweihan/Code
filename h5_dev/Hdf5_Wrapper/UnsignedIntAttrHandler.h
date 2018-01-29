#pragma once
#include "AbstractAttrHanlder.h"
class CUnsignedIntAttrHandler :
	public CAbstractAttrHanlder
{
public:
	CUnsignedIntAttrHandler() = default;
	~CUnsignedIntAttrHandler() = default;

public:
	//∂¡ Ù–‘
	virtual void WriteAttr(H5::DataSet& ElementDataSet, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) override;

	//–¥ Ù–‘
	virtual void WriteAttr(H5::Group& GroupNode, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) override;

	//∂¡ Ù–‘
	virtual void ReadAttr(H5::Attribute& Attr, H5::DataType& AttrDataType, std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray) override;
};

