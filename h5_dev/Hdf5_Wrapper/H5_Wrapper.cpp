/*********************************************************
Copyright(C):
FileName:H5_Wrapper.cpp
Descripton:HDF5 封装
Author:xiaowei.han
Data:2017/11/28
Others:
History:
Version:1.0
*********************************************************/
#include "stdafx.h"
#include "H5_Wrapper.h"
#include <H5Cpp.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/scope_exit.hpp>
#include <boost/checked_delete.hpp>
#include <boost/smart_ptr.hpp>
#include "Log.h"
#include "Utility.h"
#include "DataTypeFactory.h"
#include "ErrorCode.h"
#include "AttrHandlerFactory.h"
#include "AbstractAttrHanlder.h"

static void RecursiveWriteData(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData);

//递归遍历hdf5节点
static void RecursiveVisitNode(H5::Group& Node, Hdf5_Wrapper::LP_HDF5_DATA pData);
/*********************************************************
FunctionName:WriteHdf5File
FunctionDesc:把数据写入HDF5 文件
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CH5Wrapper::WriteHdf5File(void)
{
	using namespace H5;
	//参数合法性判断
	if (nullptr == m_pData || m_strFilePath.empty())
	{
		ERROR_LOG("the param is invalid.");
		return -ERROR_PARAM_INVALID;
	}
	try
	{
		Exception::dontPrint();
		H5File Writer(m_strFilePath, H5F_ACC_TRUNC);
		BOOST_SCOPE_EXIT(&Writer)
		{
			Writer.close();
		}BOOST_SCOPE_EXIT_END;	
		//递归写入数据
		RecursiveWriteData(Writer, m_pData);
		Writer.flush(H5F_SCOPE_GLOBAL);
	}
	catch (const Exception& e)
	{
		ERROR_LOG("WriteHdf5File raise a exception,the exception is [%s].", e.getDetailMsg().c_str());
		return ERROR_WRITE_HDF5_FAILED;
	}

	return ERROR_NO_ERROR;
}

/*********************************************************
FunctionName:ReadHdf5File
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
int CH5Wrapper::ReadHdf5File(void)
{
	using namespace H5;
	using namespace Hdf5_Wrapper;
	//判断文件是否存在
	try
	{
		using namespace boost::filesystem;

		if (!exists(m_strFilePath))
		{
			ERROR_LOG("the [%s] is not exits.", m_strFilePath.c_str());
			return ERROR_FILE_NOT_EXIST;
		}
	}
	catch (const boost::filesystem::filesystem_error& e)
	{
		ERROR_LOG("catch a exception [%s].", e.what());
		return ERROR_JUDGE_FILE_EXCEPTION;
	}

	//判断文件是否是Hdf5格式的
	if (!H5File::isHdf5(m_strFilePath.c_str()))
	{
		ERROR_LOG("the file [%s] is not hdf5 format.", m_strFilePath.c_str());
		return ERROR_FILE_FORMAT_INVALID;
	}

	try
	{
		Exception::dontPrint();
		H5File Reader(m_strFilePath, H5F_ACC_RDONLY);
		BOOST_SCOPE_EXIT(&Reader)
		{
			Reader.close();
		}BOOST_SCOPE_EXIT_END;

#if 0
		//递归写入数据
		int nGroupCount = (int)Reader.getNumObjs();
		NECESSARY_LOG("the group size is [%d]", nGroupCount);
		for (int i = 0; i < nGroupCount; ++i)
		{		
			NECESSARY_LOG("the node name is [%s],the type is [%d].", Reader.getObjnameByIdx(i).c_str(),Reader.getObjTypeByIdx(i));
		}
#endif
		m_pData = new HDF5_DATA;
		if (nullptr != m_pData)
		{
			RecursiveVisitNode(Reader,m_pData);
		}
	}
	catch (const Exception& e)
	{
		ERROR_LOG("ReadHdf5File raise a exception,the exception is [%s].", e.getDetailMsg().c_str());
		return ERROR_READ_HDF5_FAILED;
	}
	return ERROR_NO_ERROR;
}

/*********************************************************
FunctionName:SetData
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CH5Wrapper::SetData(Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	m_pData = pData;
}

/*********************************************************
FunctionName:SetFilePath
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CH5Wrapper::SetFilePath(const std::string& strFilePath)
{
	m_strFilePath = strFilePath;
}

/*********************************************************
FunctionName:FreeData
FunctionDesc:
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void CH5Wrapper::FreeData(void)
{

	//RecursiveFreeData(m_pData);

	////释放内存
	//if (nullptr != m_pData)
	//{
	//	delete m_pData;
	//	m_pData = nullptr;
	//}
	
	
}


void CH5Wrapper::RecursiveFreeData(Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	if (nullptr == pData)
	{
		return;
	}
	using namespace Hdf5_Wrapper;
	//没有子节点了
	if (pData->SubDataArray.empty())
	{

		if (nullptr != pData->pData)
		{
			//浮点类型
			if (pData->Header.eumDataType == DATA_TYPE::FLOAT_TYPE)
			{
				double* pTempData = static_cast<double*>(pData->pData);
				if (nullptr != pTempData)
				{
					delete[] pTempData;
					pData->pData = nullptr;
				}
			}
			//整型
			else if (pData->Header.eumDataType == DATA_TYPE::INT_TYPE)
			{
				int* pTempData = static_cast<int*>(pData->pData);
				if (nullptr != pTempData)
				{
					delete[] pTempData;
					pData->pData = nullptr;
				}
			}
			else
			{

			}
		}
	}
	else
	{
		//还有子节点
		for (auto& ElementData : pData->SubDataArray)
		{
			RecursiveFreeData(ElementData);
			//释放自己
			if (nullptr != ElementData)
			{
				delete ElementData;
				ElementData = nullptr;
			}
		}
		pData->SubDataArray.clear();
	}
}

/*********************************************************
FunctionName:RecursiveVisitNode
FunctionDesc:递归创建Group
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void RecursiveVisitNode(H5::Group& Node, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	using namespace H5;
	if (nullptr == pData)
	{
		return;
	}
	//获取节点名称
	pData->strKeyName = Utility::UTF8ToGB2312(Node.getObjName());
	//获取属性个数
	int nAttrNum = static_cast<int>(Node.getNumAttrs());
	for (int i = 0; i < nAttrNum; ++i)
	{
		auto childAttr = Node.openAttribute(i);

		//解析读取属性数据
		//获取数据类型
		auto AttrDataType = childAttr.getDataType();
		//获取数据类型大小
		unsigned int nDataByte = (unsigned int)AttrDataType.getSize();

		boost::scoped_ptr<CAbstractAttrHanlder> pHandler(CreateAttrHandler(childAttr.getTypeClass(), nDataByte));

		if (pHandler)
		{
			pHandler->ReadAttr(childAttr, AttrDataType, pData->AttributeArray);
		}
		childAttr.close();
	}

	//获取节点对象个数
	int nObjNum = static_cast<int>(Node.getNumObjs());

	//遍历子节点信息
	for (int i = 0; i < nObjNum; ++i)
	{
		//NECESSARY_LOG("the node name is [%s],the type is [%d].", Node.getObjnameByIdx(i).c_str(), Node.getObjTypeByIdx(i));
		auto SubNodeType = Node.getObjTypeByIdx(i);
		switch (SubNodeType)
		{
		case H5G_GROUP:
		{
			Hdf5_Wrapper::LP_HDF5_DATA pSubData = new Hdf5_Wrapper::HDF5_DATA;
			if (nullptr != pSubData)
			{
				//添加进去
				pData->SubDataArray.push_back(pSubData);
				auto ChildGroupNode = Node.openGroup(Node.getObjnameByIdx(i));
				RecursiveVisitNode(ChildGroupNode,pSubData);
				ChildGroupNode.close();
			}
		}
		break;
		case H5G_DATASET:
		{
			auto childDataset = Node.openDataSet(Node.getObjnameByIdx(i));

			Hdf5_Wrapper::LP_HDF5_DATA pSubData = new Hdf5_Wrapper::HDF5_DATA;
			if (nullptr != pSubData)
			{
				pSubData->strKeyName = childDataset.getObjName();
				//添加进去
				pData->SubDataArray.push_back(pSubData);
				//获取数据类型
				auto ChildDataSetDataType = childDataset.getDataType();
				//获取类型占用字节数
				unsigned int nDataByte = (unsigned int)ChildDataSetDataType.getSize();
				//判断数据类型
				boost::scoped_ptr<CAbstractDataTypeHandler> pHandlder(CDataTypeHandlerFactory::CreateDataTypeHandler(childDataset.getTypeClass(),nDataByte));
				if (pHandlder)
				{
					pHandlder->ReadDataSet(childDataset, pSubData);
				}
			}
		}
		break;
		default:
			break;
		}	
	}
}

/*********************************************************
FunctionName:RecursiveWriteData
FunctionDesc:递归创建Group
InputParam:
OutputParam:
Return:
Author:xiaowei.han
*********************************************************/
void RecursiveWriteData(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData)
{
	using namespace H5;
	//参数合法性判断
	if (nullptr == pData)
	{
		return;
	}



	//如果不带子节点
	if (pData->SubDataArray.empty())
	{
		//判断数据类型
		boost::scoped_ptr<CAbstractDataTypeHandler> pHandlder(CDataTypeHandlerFactory::CreateDataTypeHandler(pData->Header.eumDataType, pData->Header.nDataByte));
		if (pHandlder)
		{
			pHandlder->WriteDataSet(NodeGroup, pData);
		}
	}
	//如果有子节点
	else
	{
		//创建一个Group
		std::string strGroupKeyName = Utility::GB2312ToUTF8(pData->strKeyName);
		auto ChildGroupNode = NodeGroup.createGroup(strGroupKeyName);

		//插入属性
		for (auto& AttributeElement : pData->AttributeArray)
		{
			boost::scoped_ptr<CAbstractAttrHanlder> p(CreateAttrHandler(AttributeElement.Header.eumDataType, AttributeElement.Header.nDataByte));
			if (p)
			{
				p->WriteAttr(ChildGroupNode, AttributeElement);
			}
		}

		for (auto& SubGroupElement : pData->SubDataArray)
		{
			RecursiveWriteData(ChildGroupNode, SubGroupElement);
		}
	}
}