#pragma once
#include "QueryService.h"
using ThriftModule::QueryServiceIf;
class CConcreteService : virtual public QueryServiceIf
{
public:
	CConcreteService(void);
	virtual ~CConcreteService(void);

public:
	void QueryNameByID(std::string& _return, const int32_t nID) override;
};

