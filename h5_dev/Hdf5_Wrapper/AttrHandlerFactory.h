#pragma once
#include "Hdf5_Wrapper.h"
class CAbstractAttrHanlder;

CAbstractAttrHanlder* CreateAttrHandler(Hdf5_Wrapper::DATA_TYPE eumDataType,unsigned int nDataByte);

CAbstractAttrHanlder* CreateAttrHandler(int nHdf5DataType, unsigned int nDataByte);






