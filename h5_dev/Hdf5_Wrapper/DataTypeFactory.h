#ifndef __DATA_TYPE_FACTORY_H__
#define __DATA_TYPE_FACTORY_H__
#include <H5Cpp.h>
#include "Hdf5_Wrapper.h"

class CAbstractDataTypeHandler
{
public:
	CAbstractDataTypeHandler(void) = default;
	virtual ~CAbstractDataTypeHandler(void) = default;
	//写数据
	virtual void WriteDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) = 0;
	//读数据
	virtual void ReadDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) = 0;
protected:
	//写Dataset属性
	void WriteAttribute(H5::DataSet& ElementDataSet, const std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray);
	//解析Dataset属性
	void ReadAttribute(H5::DataSet& ElementDataSet, std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray);
	//解析压缩属性
	void ParseCompressProperty(H5::DataSet& ElementDataSet, int& nCompressLevel);
protected:
	//数据类型占用的字节数
	unsigned int m_nDataBytes;
};

//处理浮点
class CFloatDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CFloatDataTypeHandler(unsigned int nDataBytes)
	{
		m_nDataBytes = nDataBytes;
	}
	~CFloatDataTypeHandler() = default;

	void WriteDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ReadDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

#if 0
//处理双精度
class CDoubleDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CDoubleDataTypeHandler() = default;
	~CDoubleDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

//处理短整型
class CShortDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CShortDataTypeHandler() = default;
	~CShortDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

//处理长整型
class CLongDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CLongDataTypeHandler() = default;
	~CLongDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

//处理无符号整型
class CUnsignedIntDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CUnsignedIntDataTypeHandler() = default;
	~CUnsignedIntDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

//处理无符号整型
class CUnsignedShortDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CUnsignedShortDataTypeHandler() = default;
	~CUnsignedShortDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

//处理无符号长整型
class CUnsignedLongDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CUnsignedLongDataTypeHandler() = default;
	~CUnsignedLongDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

//处理无符号整型
class CUnsignedLongLongDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CUnsignedLongLongDataTypeHandler() = default;
	~CUnsignedLongLongDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

//处理无符号整型
class CLongLongDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CLongLongDataTypeHandler() = default;
	~CLongLongDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ParseDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};
#endif

//处理整型
class CIntDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CIntDataTypeHandler(unsigned int nDataBytes)
	{
		m_nDataBytes = nDataBytes;
	}
	~CIntDataTypeHandler() = default;

	void WriteDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ReadDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

class CStrDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CStrDataTypeHandler(unsigned int nDataBytes)
	{
		m_nDataBytes = nDataBytes;
	}
	~CStrDataTypeHandler() = default;

	void WriteDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;

	//解析DataSet
	void ReadDataSet(H5::DataSet& NodeDataSet, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};


class CDataTypeHandlerFactory
{
public:
	CDataTypeHandlerFactory() = default;
	~CDataTypeHandlerFactory() = default;

	static CAbstractDataTypeHandler* CreateDataTypeHandler(Hdf5_Wrapper::DATA_TYPE eumDataType,unsigned int nDataBytes);

	static CAbstractDataTypeHandler* CreateDataTypeHandler(int nHdf5DataType, unsigned int nDataBytes);
};
#endif
