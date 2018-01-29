#include "stdafx.h"
#pragma warning(disable:4251)
#include "LongAttrHandler.h"
#include "Utility.h"

void CLongAttrHandler::WriteAttr(H5::DataSet& ElementDataSet, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement)
{
	using namespace H5;
	DataType AttributeDataType(PredType::NATIVE_LONG);
	DataSpace AttributeDataSpace(H5S_SCALAR);
	//插入属性

	std::string strAttributeKeyName = Utility::GB2312ToUTF8(AttributeElement.strKeyAttrName);
	Attribute ElementAttribute = ElementDataSet.createAttribute(strAttributeKeyName, AttributeDataType, AttributeDataSpace);
	ElementAttribute.write(AttributeDataType, AttributeElement.pData);
	
}

void CLongAttrHandler::WriteAttr(H5::Group& GroupNode, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement)
{
	using namespace H5;
	DataType AttributeDataType(PredType::NATIVE_LONG);
	DataSpace AttributeDataSpace(H5S_SCALAR);
	//插入属性

	std::string strAttributeKeyName = Utility::GB2312ToUTF8(AttributeElement.strKeyAttrName);
	Attribute ElementAttribute = GroupNode.createAttribute(strAttributeKeyName, AttributeDataType, AttributeDataSpace);
	ElementAttribute.write(AttributeDataType, AttributeElement.pData);
	
}

void CLongAttrHandler::ReadAttr(H5::Attribute& Attr, H5::DataType& AttrDataType, std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray)
{
	using namespace H5;
	//获取字节
	auto nAttrValueLen = Attr.getInMemDataSize();
	auto AttrDataSpace = Attr.getSpace();
	if (H5S_SCALAR == AttrDataSpace.getSimpleExtentType())
	{
		char* pData = new char[nAttrValueLen];

		if (nullptr != pData)
		{
			Attr.read(AttrDataType, pData);
			Hdf5_Wrapper::DATA_ATTRIBUTE DataAttribute;
			DataAttribute.strKeyAttrName = Utility::UTF8ToGB2312(Attr.getName());
			DataAttribute.pData = pData;
			DataAttribute.eumatttype = Hdf5_Wrapper::DATA_TYPE::LONG_TYPE;
			AttributeArray.push_back(DataAttribute);
		}
	}
}
