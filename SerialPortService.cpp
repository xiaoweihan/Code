#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
//接收缓冲区的大小(1MB)
const int MAX_BUFFER_SIZE = (1 << 20);

CSerialPortService::CSerialPortService():
	m_Work(m_IoService),
	m_SerialPort(m_IoService),
	m_nUseBufferBytes(0),
	m_pRecvBuffer(nullptr)
{

	if (nullptr == m_pRecvBuffer)
	{
		m_pRecvBuffer = new BYTE[MAX_BUFFER_SIZE];
	}

}


CSerialPortService::~CSerialPortService()
{
	if (nullptr != m_pRecvBuffer)
	{
		delete[]m_pRecvBuffer;
		m_pRecvBuffer = nullptr;
	}
	m_nUseBufferBytes = 0;
}

void CSerialPortService::StartSerialPortService(void)
{
	//初始化串口
	if (!InitSerialPort())
	{
		return;
	}
	//开始异步读取数据
	m_SerialPort.async_read_some(boost::asio::buffer(m_pRecvBuffer + m_nUseBufferBytes, MAX_BUFFER_SIZE - m_nUseBufferBytes), boost::bind(&CSerialPortService::ReadHandler, this, boost::asio::placeholders::error,
		                                              boost::asio::placeholders::bytes_transferred));
	
	boost::system::error_code ec;
	m_IoService.run(ec);
}

void CSerialPortService::StopSerialPortService(void)
{
	m_IoService.stop();
}


void CSerialPortService::AsyncWriteData(BYTE* pData, int nDataLength)
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	boost::this_thread::sleep(boost::posix_time::seconds(1));
	m_SerialPort.async_write_some(boost::asio::buffer(pData,nDataLength),boost::bind(&CSerialPortService::WriteHandler,this,pData,nDataLength, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

//任务通知完成回调
void CSerialPortService::ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	if (ec)
	{
		return;
	}
	//分析是否有整的数据包
	int nRecvBytes = static_cast<int>(bytes_transferred);
	m_nUseBufferBytes += nRecvBytes;
	//处理数据
	int nHandledBytes = HandlerData(m_pRecvBuffer,m_nUseBufferBytes);
	//移动数据块
	memmove(m_pRecvBuffer, m_pRecvBuffer + nHandledBytes, (m_nUseBufferBytes - nHandledBytes));
	m_nUseBufferBytes -= nHandledBytes;
	m_SerialPort.async_read_some(boost::asio::buffer(m_pRecvBuffer + m_nUseBufferBytes, MAX_BUFFER_SIZE - m_nUseBufferBytes), boost::bind(&CSerialPortService::ReadHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

/**************************************************************************
@FunctionName:HandlerData
@FunctionDestription:处理数据
@InputParam:
@OutPutParam:
@ReturnValue:返回已经处理的字节数
**************************************************************************/
int CSerialPortService::HandlerData(BYTE* pData, int nDataLength)
{
	int nHandledBytes = 0;
	if (nullptr == pData || 0 == nDataLength)
	{
		return 0;
	}
	char szSensorName[100] = { 0 };
	int nIndex = 0;

	while (nIndex < nDataLength)
	{	
		//传感器上下线通知
		if (pData[nIndex] == 0xBA)
		{
			//确保数组不越界
			if (nIndex + 1 < nDataLength)
			{
				//获取数据长度
				int nFrameLength = (int)pData[nIndex + 1];
				//判断整包长度
				if ((nIndex + nFrameLength + 1) < nDataLength)
				{
					if (CalCRC8(&pData[nIndex], nFrameLength + 1) == pData[nIndex + nFrameLength + 1])
					{
						int nSensorNameLength = (int)pData[nIndex + 1 + 1];
						//获取传感器名称
						memcpy(szSensorName, &pData[nIndex + 1 + 1 + 1], nSensorNameLength);
						if (pData[nIndex + 1 + 1 + 1 + nSensorNameLength] == 0x01)
						{
							std::cout << szSensorName << " is on! " << "thread ID = ["<<boost::this_thread::get_id()<<"]"<<std::endl;
						}
						else
						{
							std::cout << szSensorName << " is off! " << "thread ID = [" << boost::this_thread::get_id() << "]" << std::endl;
						}
						nIndex += (nFrameLength + 2);
						//修改数据的处理
						nHandledBytes += (nFrameLength + 2);
						continue;
					}
				}
			}
		}

		//传感器上报数据通知
		else if (pData[nIndex] == 0xBD)
		{
			//处理数据上报通知
			if (nIndex + 1 < nDataLength)
			{
				//获取数据长度
				int nFrameLength = (int)pData[nIndex + 1];
				//判断整包长度
				if ((nIndex + nFrameLength + 1) < nDataLength)
				{
					if (CalCRC8(&pData[nIndex], nFrameLength + 1) == pData[nIndex + nFrameLength + 1])
					{
						int nSensorNameLength = (int)pData[nIndex + 1 + 1];
						//获取传感器名称
						memcpy(szSensorName, &pData[nIndex + 1 + 1 + 1], nSensorNameLength);
						float fValue = 0.0f;
						memcpy(&fValue, &pData[nIndex + 1 + 1 + 1 + nSensorNameLength], 4);
						nIndex += (nFrameLength + 2);
						//修改数据的处理
						nHandledBytes += (nFrameLength + 2);
						boost::posix_time::ptime p = boost::posix_time::second_clock::local_time();
						std::cout << "["<<p<<"] "<<"the sensor [" << szSensorName << "] value is [" << fValue << "]" << std::endl;
						continue;
					}
				}
			}
		}
		else
		{		
		}
		++nIndex;
	}
	return nHandledBytes;
}


BYTE CSerialPortService::CalCRC8(BYTE* pBuf, unsigned int nsize)
{
	BYTE crc = 0;

	if (nullptr == pBuf || 0 == nsize)
	{
		return crc;
	}

	while (nsize--)
	{
		crc ^= *pBuf;
		pBuf++;
	}
	return crc;
}

void CSerialPortService::WriteHandler(BYTE* pData, int nDataLength, const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	using namespace std;
	if (ec)
	{
		return;
	}

	if (nDataLength != bytes_transferred)
	{
		cout << "write serial failed!" << endl;
		return;
	}

	if (nullptr != pData)
	{
		delete[] pData;
		pData = nullptr;
	}
}

bool CSerialPortService::InitSerialPort(void)
{

	boost::system::error_code ec;
	m_SerialPort.open(m_SerialOption.strSerialPort, ec);
	if (ec)
	{
		return false;
	}
	//波特率
	m_SerialPort.set_option(boost::asio::serial_port::baud_rate(m_SerialOption.nBaudRate),ec);    
	if (ec)
	{
		return false;
	}

	if (m_SerialOption.bFlowControl)
	{
		m_SerialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware));  //流控制
	}
	else
	{
		m_SerialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));  //流控制
	}
	if (ec)
	{
		return false;
	}

	switch (m_SerialOption.nPairty)
	{
		//不使用校验
	case 0:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none),ec); //奇偶校验
		break;
		//奇数校验
	case 1:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::odd), ec);
		break;
		//偶数校验
	case 2:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::even), ec);
		break;
	default:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none), ec);
		break;
	}
	if (ec)
	{
		return false;
	}

	switch (m_SerialOption.nStopBits)
	{
	case 0:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one),ec); //停止位
		break;
	case 1:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::onepointfive),ec); //停止位
		break;
	case 2:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::two),ec); //停止位
		break;
	default:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one), ec);
		break;
	}

	if (ec)
	{
		return false;
	}

	m_SerialPort.set_option(boost::asio::serial_port::character_size(m_SerialOption.nDataBits),ec);// 通信字节位数，4—8
	if (ec)
	{
		return false;
	}

	return true;
}

