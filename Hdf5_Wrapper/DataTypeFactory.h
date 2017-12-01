#ifndef __DATA_TYPE_FACTORY_H__
#define __DATA_TYPE_FACTORY_H__
#include <H5Cpp.h>
#include "Hdf5_Wrapper.h"

class CAbstractDataTypeHandler
{
public:
	CAbstractDataTypeHandler(void) = default;
	virtual ~CAbstractDataTypeHandler(void) = default;

	virtual void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) = 0;

protected:
	void AddDataAttribute(H5::DataSet& ElementDataSet, const std::vector<Hdf5_Wrapper::DATA_ATTRIBUTE>& AttributeArray);
};

class CFloatDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CFloatDataTypeHandler() = default;
	~CFloatDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

class CIntDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CIntDataTypeHandler() = default;
	~CIntDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};

class CStrDataTypeHandler : public CAbstractDataTypeHandler
{
public:
	CStrDataTypeHandler() = default;
	~CStrDataTypeHandler() = default;

	void HandleDataSet(H5::Group& NodeGroup, Hdf5_Wrapper::LP_HDF5_DATA pData) override;
};


class CDataTypeHandlerFactory
{
public:
	CDataTypeHandlerFactory() = default;
	~CDataTypeHandlerFactory() = default;

	static CAbstractDataTypeHandler* CreateDataTypeHandler(Hdf5_Wrapper::DATA_TYPE eumDataType);
};
#endif
