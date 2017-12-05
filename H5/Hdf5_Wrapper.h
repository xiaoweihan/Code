#ifndef __HDF5_WRAPPER_H__
#define __HDF5_WRAPPER_H__
#include <vector>
#include <string>
namespace Hdf5_Wrapper
{
	//要存储的数据类型
	enum class DATA_TYPE
	{
		//浮点类型
		FLOAT_TYPE = 1,
		//整形
		INT_TYPE,
		//字符串类型
		STR_TYPE
	};

	//数据属性头
	struct _data_attribute
	{
		std::string strKeyAttrName;
		std::string strAttrValue;
	};
	using DATA_ATTRIBUTE = _data_attribute;
	using LP_DATA_ATTRIBUTE = _data_attribute;

	//数据描述头
	struct _data_header
	{
		//维数
		int nRank;
		//维度信息
		std::vector<int> DimensionArray;
		//数据类型
		DATA_TYPE eumDataType;

		_data_header()
		{
			nRank = 0;
			DimensionArray.clear();
			eumDataType = DATA_TYPE::INT_TYPE;
		}

		void Reset()
		{
			nRank = 0;
			DimensionArray.clear();
			eumDataType = DATA_TYPE::INT_TYPE;
		}
	};

	using DATA_HEADER = _data_header;
	using LP_DATA_HEADER = _data_header*;

	//数据定义(外部负责释放内存)
	struct _hdf5_data
	{
		//hash表中的key
		std::string strKeyName;
		//数据属性
		std::vector<DATA_ATTRIBUTE> AttributeArray;
		//数据头
		DATA_HEADER Header;
		//字符串信息
		std::vector<std::string> stringData;
		//数据信息
		void* pData;
		//压缩比
		int nCompressLevel;
		//子信息
		std::vector<_hdf5_data*> SubDataArray;


		_hdf5_data()
		{
			pData = nullptr;
			SubDataArray.clear();
			nCompressLevel = 4;
			stringData.clear();
		}

		void Reset()
		{
			pData = nullptr;
			SubDataArray.clear();
			Header.Reset();
			AttributeArray.clear();
			strKeyName.clear();
			stringData.clear();
		}

	};

	using HDF5_DATA = _hdf5_data;
	using LP_HDF5_DATA = _hdf5_data*;


	//内存数据->HDF5文件
	int Data2Hdf5(const char* szFilePath,const LP_HDF5_DATA pData);

	//HDF5->内存数据
	int Hdf52Data(const char* szFilePath, LP_HDF5_DATA* pData);

	//回收内存
	int RecyleData(void);

}





#endif
