#pragma once
#include "AbstractAttrHanlder.h"
class CDoubleAttrHandler :
	public CAbstractAttrHanlder
{
public:
	CDoubleAttrHandler() = default;
	~CDoubleAttrHandler() =default;

public:
	//∂¡ Ù–‘
	virtual void WriteAttr(H5::DataSet& ElementDataSet, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) override;

	//–¥ Ù–‘
	virtual void WriteAttr(H5::Group& GroupNode, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) override;

	//∂¡ Ù–‘
	virtual void ReadAttr(H5::Attribute& Attr, H5::DataType& AttrDataType, std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray) override;

};

