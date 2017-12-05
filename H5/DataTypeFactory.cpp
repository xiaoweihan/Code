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

/*********************************************************
FunctionName:CreateDataTypeHandler
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
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
FunctionName:CreateDataTypeHandler
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
CAbstractDataTypeHandler* CDataTypeHandlerFactory::CreateDataTypeHandler(int nHdf5DataType)
{
	using namespace H5;

	CAbstractDataTypeHandler* pHandler = nullptr;

	switch (nHdf5DataType)
	{
	case H5T_INTEGER:
		pHandler = new CIntDataTypeHandler;
		break;
	case H5T_FLOAT:
		pHandler = new CFloatDataTypeHandler;
		break;
	case H5T_STRING:
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

/*********************************************************
FunctionName:ParseDataSet
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CFloatDataTypeHandler::ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	if (nullptr == pData)
	{
		return;
	}
	//获取数据类型
	auto ElementDataType = NodeDataSet.getFloatType();
	//获取类型所占用的字节
	int nDataSize = static_cast<int>(ElementDataType.getSize());

	//获取DataSpace
	auto ElementDataspace = NodeDataSet.getSpace();
	//获取维数
	int nDataRank = ElementDataspace.getSimpleExtentNdims();

	//申请内存
	boost::scoped_array<hsize_t> pDimsArray(new hsize_t[nDataRank]);

	if (!pDimsArray)
	{
		ERROR_LOG("Allocate memory failed.");
		return;
	}
	ElementDataspace.getSimpleExtentDims(pDimsArray.get(), NULL);

	//设置数据头
	pData->Header.eumDataType = Hdf5_Wrapper::DATA_TYPE::FLOAT_TYPE;
	pData->Header.nRank = nDataRank;
	//开始计算需要申请多大的内存空间
	int nTotalSize = 1;
	for (int i = 0; i < nDataRank; ++i)
	{
		pData->Header.DimensionArray.push_back((int)pDimsArray[i]);
		nTotalSize *= (int)pDimsArray[i];
	}
	//申请内存
	double* pValueData = new double[nTotalSize];
	if (nullptr == pValueData)
	{
		ERROR_LOG("Allocate memory failed.");
		return;
	}
	//进行数据读取
	NodeDataSet.read(pValueData, ElementDataType);

	pData->pData = pValueData;

	//进行属性读取
	ParseDataAttribute(NodeDataSet, pData->AttributeArray);
}

/*********************************************************
FunctionName:HandleDataSet
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
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
			IntType ElementDataType(PredType::NATIVE_INT32);
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
FunctionName:ParseDataSet
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CIntDataTypeHandler::ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	if (nullptr == pData)
	{
		return;
	}
	//auto strKeyName = pData->strKeyName;
	//获取数据类型
	auto ElementDataType = NodeDataSet.getIntType();
	//获取数据
	int nDataSize = static_cast<int>(ElementDataType.getSize());
	//获取数据空间
	auto ElementDataspace = NodeDataSet.getSpace();
	//获取数据维数
	int nDataRank = ElementDataspace.getSimpleExtentNdims();
	//申请内存
	boost::scoped_array<hsize_t> pDimsArray(new hsize_t[nDataRank]);

	if (!pDimsArray)
	{
		ERROR_LOG("Allocate memory failed.");
		return;
	}
	ElementDataspace.getSimpleExtentDims(pDimsArray.get(), NULL);
	//设置数据头
	pData->Header.eumDataType = Hdf5_Wrapper::DATA_TYPE::INT_TYPE;
	pData->Header.nRank = nDataRank;
	//开始计算需要申请多大的内存空间
	int nTotalSize = 1;
	for (int i = 0; i < nDataRank; ++i)
	{
		pData->Header.DimensionArray.push_back((int)pDimsArray[i]);
		nTotalSize *= (int)pDimsArray[i];
	}
	//申请内存
	int* pValueData = new int[nTotalSize];
	if (nullptr == pValueData)
	{
		ERROR_LOG("Allocate memory failed.");
		return;
	}
	//进行数据读取
	NodeDataSet.read(pValueData, ElementDataType);

	pData->pData = pValueData;

	//进行属性读取
	ParseDataAttribute(NodeDataSet, pData->AttributeArray);

	ParseCompressProperty(NodeDataSet, pData->nCompressLevel);
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
FunctionName:ParseDataSet
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CStrDataTypeHandler::ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	if (nullptr == pData)
	{
		return;
	}
	//auto strKeyName = pData->strKeyName;
	//获取数据类型
	auto ElementDataType = NodeDataSet.getStrType();
	//获取数据的最大长度
	int nDataMaxSize = static_cast<int>(ElementDataType.getSize());
	//获取维度
	auto ElementDataspace = NodeDataSet.getSpace();
	int nDataRank = ElementDataspace.getSimpleExtentNdims();

	//申请内存
	boost::scoped_array<hsize_t> pDimsArray(new hsize_t[nDataRank]);

	if (!pDimsArray)
	{
		ERROR_LOG("Allocate memory failed.");
		return;
	}
	ElementDataspace.getSimpleExtentDims(pDimsArray.get(), NULL);
	//设置数据头
	pData->Header.eumDataType = Hdf5_Wrapper::DATA_TYPE::STR_TYPE;
	pData->Header.nRank = nDataRank;
	//开始计算需要申请多大的内存空间
	int nTotalSize = nDataMaxSize;
	for (int i = 0; i < nDataRank; ++i)
	{
		pData->Header.DimensionArray.push_back((int)pDimsArray[i]);
		nTotalSize *= (int)pDimsArray[i];
	}
	char* pValueData = new char[nTotalSize];
	if (nullptr == pValueData)
	{
		ERROR_LOG("Allocate memory failed.");
		return;
	}
	memset(pValueData, 0, nTotalSize);
	NodeDataSet.read(pValueData, ElementDataType);
	pData->pData = nullptr;
	for (int i = 0; i < nTotalSize; i += nDataMaxSize)
	{
		//NECESSARY_LOG("the value is [%s].", pValueData + i);
		pData->stringData.push_back(Utility::UTF8ToGB2312(std::string(pValueData + i)));
	}
	//释放内存
	delete[] pValueData;
	pValueData = nullptr;
	ParseDataAttribute(NodeDataSet, pData->AttributeArray);
	//解析压缩信息
	ParseCompressProperty(NodeDataSet, pData->nCompressLevel);
	//NECESSARY_LOG("the keyName [%s],the data size = [%d],the Rank = [%d].", strKeyName.c_str(), nSize, nDataRank);
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

void CAbstractDataTypeHandler::ParseDataAttribute(H5::DataSet& ElementDataSet, std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray)
{
	using namespace H5;
	AttributeArray.clear();
	//获取属性个数
	int nAttrNum = static_cast<int>(ElementDataSet.getNumAttrs());
	for (int i = 0; i < nAttrNum; ++i)
	{
		auto childAttr = ElementDataSet.openAttribute(i);
		//读取attr
		StrType AttributeDataType(0, H5T_VARIABLE);
		std::string strAttriValue;
		childAttr.read(AttributeDataType, strAttriValue);
		//NECESSARY_LOG("the name is [%s],the value is [%s].", childAttr.getName().c_str(), strAttriValue.c_str());
		Hdf5_Wrapper::DATA_ATTRIBUTE DataAttribute;
		DataAttribute.strKeyAttrName = Utility::UTF8ToGB2312(childAttr.getName());
		DataAttribute.strAttrValue = Utility::UTF8ToGB2312(strAttriValue);
		AttributeArray.push_back(std::move(DataAttribute));
		childAttr.close();
	}
}

void CAbstractDataTypeHandler::ParseCompressProperty(H5::DataSet& ElementDataSet, int& nCompressLevel)
{
	auto NodePropList = ElementDataSet.getCreatePlist();

	auto nFillterNum = NodePropList.getNfilters();

	unsigned int nFlag = 0;
	size_t cd_nelmts = 0;
	unsigned int cd_values[1] = { 0 };
	char szName[1] = { 0 };
	int nNameLen = 1;
	unsigned int nFilter_info = 0;
	for (int i = 0; i < nFillterNum; ++i)
	{
		cd_nelmts = 0;
		int filter_type = NodePropList.getFilter(i, nFlag, cd_nelmts, cd_values, nNameLen, szName, nFilter_info);
		switch (filter_type) 
		{
			//Gzip
		case H5Z_FILTER_DEFLATE:
			nCompressLevel = nFilter_info + 1;
			break;
		case H5Z_FILTER_SZIP:
			break;
		default:
			break;
		}
	}

	NodePropList.close();
}
