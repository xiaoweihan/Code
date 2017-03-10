#pragma once
#include "ZipService.h"
using ThriftModule::ZipServiceIf;
class CConcreteZip : virtual public ZipServiceIf
{
public:
	CConcreteZip(void);
	virtual ~CConcreteZip(void);

	void Zip() override;
};

