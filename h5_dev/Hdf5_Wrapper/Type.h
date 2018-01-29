/*********************************************************
Copyright(C):
FileName:Type.h
Descripton:内部类型定义
Author:xiaowei.han
Data:2018/01/26
Others:
History:
Version:1.0
*********************************************************/
#ifndef TYPE_H
#define TYPE_H
#include <vector>
#include "Utility.h"
namespace InnerType
{

	//要存储的数据类型
	enum class DATA_TYPE
	{
		NO_TYPE = 0,
		//浮点类型
		FLOAT_TYPE = 1,
		//整形
		INT_TYPE,
		//字符串类型
		STR_TYPE,
		//符合类型
		COMPOSE_TYPE
	};

	//数据描述头
	struct _data_header
	{
		//维数
		int nRank;
		//维度信息
		int* pDimensionArray;
		//数据类型
		DATA_TYPE eumDataType;
		//数据类型占用的字节数
		unsigned int nDataByte;
		//是否带符号 只针对整型
		bool bSign;

		_data_header()
		{
			nDataByte = 0;
			nRank = 0;
			pDimensionArray = nullptr;
			eumDataType = DATA_TYPE::NO_TYPE;
			bSign = true;
		}

		//移动构造函数
		_data_header(_data_header&& CopyValue)
		{
			nRank = CopyValue.nRank;
			pDimensionArray = CopyValue.pDimensionArray;
			eumDataType = CopyValue.eumDataType;
			nDataByte = CopyValue.nDataByte;
			bSign = CopyValue.bSign;
			CopyValue.pDimensionArray = nullptr;
			
		}
		_data_header& operator=(_data_header&& CopyValue)
		{
			if (this == &CopyValue)
			{
				return *this;
			}
			nRank = CopyValue.nRank;
			pDimensionArray = CopyValue.pDimensionArray;
			eumDataType = CopyValue.eumDataType;
			nDataByte = CopyValue.nDataByte;
			bSign = CopyValue.bSign;
			CopyValue.pDimensionArray = nullptr;
			return *this;
		}

		~_data_header()
		{
			DELETE_ARRAY_POINTER(pDimensionArray);
		}

	};

	using DATA_HEADER = _data_header;
	using LP_DATA_HEADER = _data_header*;

	//数据属性
	struct _data_attribute
	{
		std::string strKeyAttrName;
		//数据头
		DATA_HEADER Header;
		//数据
		void* pData;
		//字符串信息
		std::string strAttrValue;

		_data_attribute()
		{
			pData = nullptr;
		}

		_data_attribute(_data_attribute&& CopyValue)
		{
			strKeyAttrName = CopyValue.strKeyAttrName;
			Header = std::move(CopyValue.Header);
			pData = CopyValue.pData;
			CopyValue.pData = nullptr;
		}

		_data_attribute& operator=(_data_attribute&& CopyValue)
		{
			if (this == &CopyValue)
			{
				return *this;
			}
			strKeyAttrName = CopyValue.strKeyAttrName;
			Header = std::move(CopyValue.Header);
			pData = CopyValue.pData;
			CopyValue.pData = nullptr;
			return *this;
		}
		~_data_attribute()
		{
			DELETE_ARRAY_POINTER(pData);
		}

	};
	using DATA_ATTRIBUTE = _data_attribute;
	using LP_DATA_ATTRIBUTE = _data_attribute;


	//数据定义(外部负责释放内存)
	struct _hdf5_write_data
	{
		//hash表中的key
		std::string strKeyName;
		//数据属性
		std::vector<DATA_ATTRIBUTE> AttributeArray;
		//数据头
		DATA_HEADER Header;
		//数据信息
		void* pData;
		//压缩比
		int nCompressLevel;
		//子信息
		std::vector<_hdf5_write_data*> SubDataArray;

		_hdf5_write_data()
		{
			pData = nullptr;
			SubDataArray.clear();
			nCompressLevel = -1;
			AttributeArray.clear();
		}

		_hdf5_write_data(_hdf5_write_data&& CopyValue)
		{
			strKeyName = CopyValue.strKeyName;
			AttributeArray = std::move(CopyValue.AttributeArray);
			Header = std::move(Header);
			pData = CopyValue.pData;
			CopyValue.pData = nullptr;
			nCompressLevel = CopyValue.nCompressLevel;
			SubDataArray.swap(CopyValue.SubDataArray);
		}

		_hdf5_write_data& operator=(_hdf5_write_data&& CopyValue)
		{
			if (this == &CopyValue)
			{
				return *this;
			}

			strKeyName = CopyValue.strKeyName;
			AttributeArray = std::move(CopyValue.AttributeArray);
			Header = std::move(Header);
			pData = CopyValue.pData;
			CopyValue.pData = nullptr;
			nCompressLevel = CopyValue.nCompressLevel;
			SubDataArray.swap(CopyValue.SubDataArray);
			return *this;
		}

		~_hdf5_write_data()
		{
			DELETE_ARRAY_POINTER(pData);

			for (auto& SubElement : SubDataArray)
			{
				DELETE_POINTER(SubElement);
			}
		}

	};

	using HDF5_WRITE_DATA = _hdf5_write_data;
	using LP_HDF5_WRITE_DATA = _hdf5_write_data*;

}

#endif

