#include "stdafx.h"
#pragma warning(disable:4251)
#include "DataTypeFactory.h"
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/minmax_element.hpp>
#include <boost/checked_delete.hpp>
#include <string>
#include <vector>
#include "Utility.h"
#include "Log.h"
CAbstractDataTypeHandler* CDataTypeHandlerFactory::CreateDataTypeHandler(Hdf5_Wrapper::DATA_TYPE eumDataType)
{
	using namespace H5;

	CAbstractDataTypeHandler* pHandler = nullptr;

	switch (eumDataType)
	{
	case Hdf5_Wrapper::DATA_TYPE::FLOAT_TYPE:
		pHandler = new CFloatDataTypeHandler;
		break;
	case Hdf5_Wrapper::DATA_TYPE::INT_TYPE:
		pHandler = new CIntDataTypeHandler;
		break;
	case Hdf5_Wrapper::DATA_TYPE::STR_TYPE:
		pHandler = new CStrDataTypeHandler;
		break;
	default:
		break;
	}

	return pHandler;
}

/*********************************************************
FunctionName:HandleDataSet
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CFloatDataTypeHandler::HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	using namespace H5;
	using namespace boost;
	if (nullptr != pData->pData)
	{
		//参数合法性判断
		if (!pData->Header.DimensionArray.empty() && pData->Header.DimensionArray.size() == pData->Header.nRank)
		{
			//指定类型
			FloatType ElementDataType(PredType::NATIVE_DOUBLE);
			auto nDims = pData->Header.DimensionArray.size();
			//个数为空返回
			if (nDims > 0)
			{
				scoped_array<hsize_t> pDims(new hsize_t[nDims]);
				if (pDims)
				{
					for (int i = 0; i < (int)nDims; ++i)
					{
						pDims[i] = pData->Header.DimensionArray[i];
					}
					//创建空间
					DataSpace ElementDataSpace(pData->Header.nRank, pDims.get());
					DSetCreatPropList ElementPropList;
					ElementPropList.setChunk(pData->Header.nRank, pDims.get());
					ElementPropList.setDeflate(pData->nCompressLevel);
					std::string strDatasetKeyName = Utility::GB2312ToUTF8(pData->strKeyName);
					DataSet ChildDataSet = NodeGroup.createDataSet(strDatasetKeyName, ElementDataType, ElementDataSpace, ElementPropList);
					ChildDataSet.write(pData->pData, ElementDataType);
					//添加Attribute
					AddDataAttribute(ChildDataSet, pData->AttributeArray);			
				}
			}
		}	
	}
}

void CIntDataTypeHandler::HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	using namespace H5;
	using namespace boost;
	if (nullptr != pData->pData)
	{
		//参数合法性判断
		if (!pData->Header.DimensionArray.empty() && pData->Header.DimensionArray.size() == pData->Header.nRank)
		{
			//指定类型
			IntType ElementDataType(PredType::NATIVE_INT);
			auto nDims = pData->Header.DimensionArray.size();
			//个数为空返回
			if (nDims > 0)
			{
				scoped_array<hsize_t> pDims(new hsize_t[nDims]);
				if (pDims)
				{
					for (int i = 0; i < (int)nDims; ++i)
					{
						pDims[i] = pData->Header.DimensionArray[i];
					}
					//创建空间
					DataSpace ElementDataSpace(pData->Header.nRank, pDims.get());
					DSetCreatPropList ElementPropList;
					ElementPropList.setChunk(pData->Header.nRank, pDims.get());
					ElementPropList.setDeflate(pData->nCompressLevel);
					std::string strDatasetKeyName = Utility::GB2312ToUTF8(pData->strKeyName);
					DataSet ChildDataSet = NodeGroup.createDataSet(strDatasetKeyName, ElementDataType, ElementDataSpace, ElementPropList);
					scoped_array<hsize_t> pOffSet(new hsize_t[nDims]);
	
					ChildDataSet.write(pData->pData, ElementDataType);
					//添加Attribute
					AddDataAttribute(ChildDataSet, pData->AttributeArray);
					
				}
			}
		}	
	}
}

/*********************************************************
FunctionName:HandleDataSet
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CStrDataTypeHandler::HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	using namespace H5;
	using namespace boost;

	if (pData->stringData.empty())
	{
		return;
	}
	std::vector<std::string> ConvertResultArray;
	//字符集转换为UTF-8
	for (auto& ElementData : pData->stringData)
	{
		auto strConvertResult = Utility::GB2312ToUTF8(ElementData);
		ConvertResultArray.push_back(ElementData);
	}
	//找出最大的长度的字符串长度
	int nMaxLength = 0;
	for (auto& ElementData : ConvertResultArray)
	{
		if (ElementData.length() > nMaxLength)
		{
			nMaxLength = (int)ElementData.length();
		}
	}
	//遍历所有要写的字符串
	int nStringDataSize = (int)ConvertResultArray.size();
	int nElementSize = nMaxLength + 1;

	int nTotalSize = nStringDataSize * nElementSize;
	//申请内存
	char* pContentData = new char[nTotalSize];

	if (nullptr != pContentData)
	{
		for (int i = 0; i < nStringDataSize; ++i)
		{
			memset(pContentData + (i * nElementSize), 0, nElementSize);
			memcpy(pContentData + (i * nElementSize), ConvertResultArray[i].c_str(), ConvertResultArray[i].length());
		}
	}
	//指定类型
	StrType ElementDataType(PredType::C_S1, nElementSize);
	//创建空间
	hsize_t hStrTypeDims[1] = { 0 };
	hStrTypeDims[0] = nStringDataSize;
	DataSpace ElementDataSpace(1,hStrTypeDims);
	DSetCreatPropList ElementPropList;
	hsize_t hChunkDims[1] = { 0 };
	hChunkDims[0] = nStringDataSize;
	ElementPropList.setChunk(1, hChunkDims);
	ElementPropList.setDeflate(pData->nCompressLevel);
	std::string strDatasetKeyName = Utility::GB2312ToUTF8(pData->strKeyName);
	DataSet ChildDataSet = NodeGroup.createDataSet(strDatasetKeyName, ElementDataType, ElementDataSpace, ElementPropList);
	//写入数据
	ChildDataSet.write(pContentData, ElementDataType);
	//添加Attribute
	AddDataAttribute(ChildDataSet, pData->AttributeArray);

	boost::checked_array_delete(pContentData);
}

/*********************************************************
FunctionName:AddDataAttribute
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CAbstractDataTypeHandler::AddDataAttribute(H5::DataSet& ElementDataSet, const std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray)
{
	using namespace H5;
	StrType AttributeDataType(0, H5T_VARIABLE);
	DataSpace AttributeDataSpace(H5S_SCALAR);
	//插入属性
	for (auto& AttributeElement : AttributeArray)
	{
		std::string strAttributeKeyName = Utility::GB2312ToUTF8(AttributeElement.strKeyAttrName);
		Attribute ElementAttribute = ElementDataSet.createAttribute(strAttributeKeyName, AttributeDataType, AttributeDataSpace);
		ElementAttribute.write(AttributeDataType, Utility::GB2312ToUTF8(AttributeElement.strAttrValue));
	}
}
