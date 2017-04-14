#include "stdafx.h"
#include "KeepAliveContext.h"
#include "AbstractKeepAliveStrategy.h"

void CKeepAliveContext::StartKeepAlive(int nKeepAliveGap)
{
	if (nullptr != m_pKeepAliveStrategy)
	{
		m_pKeepAliveStrategy->SetKeepAliveGap(nKeepAliveGap);
		m_pKeepAliveStrategy->StartKeepAlive();
	}
}

void CKeepAliveContext::StopKeepAlive(void)
{
	if (nullptr != m_pKeepAliveStrategy)
	{
		m_pKeepAliveStrategy->StopKeepAlive();
	}
}

