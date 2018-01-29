#pragma once
#include "AbstractAttrHanlder.h"
class CIntAttrHandler :
	public CAbstractAttrHanlder
{
public:
	CIntAttrHandler(unsigned int nDataByte)
	{
		m_nDataByte = nDataByte;
	}
	~CIntAttrHandler() =default;
public:
	//������
	virtual void WriteAttr(H5::DataSet& ElementDataSet, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) override;

	//д����
	virtual void WriteAttr(H5::Group& GroupNode, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement) override;

	//������
	virtual void ReadAttr(H5::Attribute& Attr, H5::DataType& AttrDataType, std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray) override;

};

