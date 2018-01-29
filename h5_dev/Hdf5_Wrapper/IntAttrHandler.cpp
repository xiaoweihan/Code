#include "stdafx.h"
#pragma warning(disable:4251)
#include "IntAttrHandler.h"
#include <boost/smart_ptr.hpp>
#include "Utility.h"
#include "Log.h"
void CIntAttrHandler::WriteAttr(H5::DataSet& ElementDataSet, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement)
{
#if 1
	using namespace H5;
	using namespace boost;
	if (nullptr != AttributeElement.pData)
	{
		//参数合法性判断
		if (!AttributeElement.Header.DimensionArray.empty() && AttributeElement.Header.DimensionArray.size() == AttributeElement.Header.nRank)
		{
			//指定类型
			DataType ElementDataType(H5T_INTEGER, m_nDataByte);
			auto nDims = AttributeElement.Header.DimensionArray.size();
			//个数为空返回
			if (nDims > 0)
			{
				scoped_array<hsize_t> pDims(new hsize_t[nDims]);
				if (pDims)
				{
					for (int i = 0; i < (int)nDims; ++i)
					{
						pDims[i] = AttributeElement.Header.DimensionArray[i];
					}
					//创建空间
					DataSpace ElementDataSpace(AttributeElement.Header.nRank, pDims.get());
					/*DSetCreatPropList ElementPropList;
					ElementPropList.setChunk(AttributeElement.Header.nRank, pDims.get());
					ElementPropList.setDeflate(AttributeElement.nCompressLevel);*/
					std::string strAttrKeyName = Utility::GB2312ToUTF8(AttributeElement.strKeyAttrName);
					Attribute ElementAttribute = ElementDataSet.createAttribute(strAttrKeyName, ElementDataType, ElementDataSpace/*, ElementPropList*/);
					//scoped_array<hsize_t> pOffSet(new hsize_t[nDims]);
					ElementAttribute.write(ElementDataType, AttributeElement.pData);
				}
			}
}
		else
		{
			ERROR_LOG("param is invalid,the DimensionArraySize[%d],the Rank[%d].", AttributeElement.Header.DimensionArray.size(), AttributeElement.Header.nRank);
		}
	}
#else
	using namespace H5;
	DataType AttributeDataType(PredType::NATIVE_INT);
	DataSpace AttributeDataSpace(H5S_SCALAR);
	//插入属性

	std::string strAttributeKeyName = Utility::GB2312ToUTF8(AttributeElement.strKeyAttrName);
	Attribute ElementAttribute = ElementDataSet.createAttribute(strAttributeKeyName, AttributeDataType, AttributeDataSpace);
	ElementAttribute.write(AttributeDataType, AttributeElement.pData);
#endif

}

void CIntAttrHandler::WriteAttr(H5::Group& GroupNode, const Hdf5_Wrapper::DATA_ATTRIBUTE& AttributeElement)
{

#if 1
	using namespace H5;
	using namespace boost;
	if (nullptr != AttributeElement.pData)
	{
		//参数合法性判断
		if (!AttributeElement.Header.DimensionArray.empty() && AttributeElement.Header.DimensionArray.size() == AttributeElement.Header.nRank)
		{
			//指定类型
			DataType ElementDataType(H5T_INTEGER, m_nDataByte);
			auto nDims = AttributeElement.Header.DimensionArray.size();
			//个数为空返回
			if (nDims > 0)
			{
				scoped_array<hsize_t> pDims(new hsize_t[nDims]);
				if (pDims)
				{
					for (int i = 0; i < (int)nDims; ++i)
					{
						pDims[i] = AttributeElement.Header.DimensionArray[i];
					}
					//创建空间
					DataSpace ElementDataSpace(AttributeElement.Header.nRank, pDims.get());
					/*DSetCreatPropList ElementPropList;
					ElementPropList.setChunk(AttributeElement.Header.nRank, pDims.get());
					ElementPropList.setDeflate(AttributeElement.nCompressLevel);*/
					std::string strAttrKeyName = Utility::GB2312ToUTF8(AttributeElement.strKeyAttrName);
					Attribute ElementAttribute = GroupNode.createAttribute(strAttrKeyName, ElementDataType, ElementDataSpace/*, ElementPropList*/);
					//scoped_array<hsize_t> pOffSet(new hsize_t[nDims]);
					ElementAttribute.write(ElementDataType,AttributeElement.pData);
				}
			}
		}
		else
		{
			ERROR_LOG("param is invalid,the DimensionArraySize[%d],the Rank[%d].", AttributeElement.Header.DimensionArray.size(), AttributeElement.Header.nRank);
		}
	}
#else
	using namespace H5;
	DataType AttributeDataType(PredType::NATIVE_INT);
	DataSpace AttributeDataSpace(H5S_SCALAR);
	//插入属性

	std::string strAttributeKeyName = Utility::GB2312ToUTF8(AttributeElement.strKeyAttrName);
	Attribute ElementAttribute = GroupNode.createAttribute(strAttributeKeyName, AttributeDataType, AttributeDataSpace);
	ElementAttribute.write(AttributeDataType, AttributeElement.pData);
#endif	
}

void CIntAttrHandler::ReadAttr(H5::Attribute& Attr, H5::DataType& AttrDataType, std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray)
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
			DataAttribute.Header.eumDataType = Hdf5_Wrapper::DATA_TYPE::INT_TYPE;
			DataAttribute.Header.nDataByte = (unsigned int)AttrDataType.getSize();
			AttributeArray.push_back(DataAttribute);
		}
	}
	else if (H5S_SIMPLE == AttrDataSpace.getSimpleExtentType())
	{
		//获取维数
		int nDataRank = AttrDataSpace.getSimpleExtentNdims();

		//申请内存
		boost::scoped_array<hsize_t> pDimsArray(new hsize_t[nDataRank]);

		if (!pDimsArray)
		{
			ERROR_LOG("Allocate memory failed.");
			return;
		}
		Hdf5_Wrapper::DATA_ATTRIBUTE DataAttribute;
		//获取数据维度信息
		AttrDataSpace.getSimpleExtentDims(pDimsArray.get(), NULL);
		//设置数据头
		DataAttribute.Header.eumDataType = Hdf5_Wrapper::DATA_TYPE::INT_TYPE;
		//获取类型占用的字节数
		DataAttribute.Header.nDataByte = (unsigned int)AttrDataType.getSize();
		DataAttribute.strKeyAttrName = Utility::UTF8ToGB2312(Attr.getName());
		//获取维数
		DataAttribute.Header.nRank = nDataRank;
		//开始计算需要申请多大的内存空间
		int nTotalSize = (int)Attr.getInMemDataSize();
		for (int i = 0; i < nDataRank; ++i)
		{
			DataAttribute.Header.DimensionArray.push_back((int)pDimsArray[i]);
		}
		//申请内存
		char* pValueData = new char[nTotalSize];
		if (nullptr == pValueData)
		{
			ERROR_LOG("Allocate memory failed.");
			return;
		}
		//进行数据读取
		Attr.read(AttrDataType, pValueData);
		//指针赋值
		DataAttribute.pData = pValueData;
		AttributeArray.push_back(DataAttribute);
	}
	else
	{

	}
}
