#include "stdafx.h"
#include "Logger.h"

#include <iomanip>
#include <sstream>

namespace JShark
{
	Logger::Logger(std::string fileName) : fileName(std::move(fileName))
	{}
	
	bool Logger::OpenLog()
	{
		if (!this->fileName.empty())
		{
			file.open(this->fileName, std::fstream::out | std::fstream::app);
			file << "------ NEW LOG ------" << std::endl << std::flush;
		}
		else
			return false;
		return true;
	}

	bool Logger::OpenLog(const std::string& fileName)
	{
		this->fileName = fileName;
		file.open(fileName, std::fstream::out | std::fstream::app);
		file << "------ NEW LOG ------" << std::endl << std::flush;

		return true; 
	}

	void Logger::LogData(std::string fmt, ulong retCode, ...)
	{
		va_list args;
		va_start(args, fmt);

		for (auto iter = fmt.begin(); iter != fmt.end(); ++iter)
		{
			if (*iter == '%')
			{
				switch (*(iter + 1))
				{
				case 'r': //ULONG to ProtocolID
					{
						const ulong out = va_arg(args, ulong);
						file << ToProtocolId(out) << std::flush;
						++iter;
					}
					break;
				case 'u': //ULONG
					{
						const ulong out = va_arg(args, ulong);
						file << std::to_string(out) << std::flush;
						++iter;
					}
					break;
				case 'x': //ULONG as HEX
					{
						const ulong out = va_arg(args, ulong);
						file << "0x" << std::hex << std::setw(sizeof(ulong) / 8) << std::setfill('0') <<
							std::to_string(out) << std::flush;
						++iter;
					}
					break;
				case 'v': //Void Pointer
					{
						file << "*void" << std::flush;
						++iter;
					}
					break;
				case 'p': //PassThru Message 
					{
						PASSTHRU_MSG* pMsg = &va_arg(args, PASSTHRU_MSG);

						if (pMsg == nullptr)
						{
							++iter;
							break;
						}


						file << "{" << std::endl << std::flush;
						file << "\tProtocol ID: " << std::hex << pMsg->ProtocolID << " (" <<
							ToProtocolId(pMsg->ProtocolID) << ")" << std::endl << std::flush;
						file << "\tRxStatus: " << std::hex << pMsg->RxStatus << std::endl << std::flush;
						file << "\tTxFlags: " << std::hex << pMsg->TxFlags << " (" << ToTxFlags(pMsg->TxFlags) << ")" <<
							std::endl << std::flush;
						file << "\tExtraData Idx: " << std::hex << pMsg->ExtraDataIndex << std::endl << std::flush;
						file << "\tDataSize: " << pMsg->DataSize << std::endl << std::flush;
						file << "\tTimeStamp: " << pMsg->Timestamp << std::endl << std::flush;
						file << "\tData: " << std::endl << std::flush;
						if (pMsg->DataSize > 4128)
						{
							file << "\t{\n\t\tINVALID PASSTHRU DATA\n\t}\n" << std::flush;
						}
						else if (pMsg->DataSize == 0)
						{
							file << "\t{}\n" << std::flush;
						}
						else
						{
							for (size_t i = 0; i < pMsg->DataSize; i++)
							{
								if (i % 0x0F == 0)
									file << std::endl << std::flush;

								file << "\t" << std::hex << pMsg->Data[i] << std::flush;
							}
						}

						file << std::endl << std::flush;

						file << "}" << std::endl << std::flush;

						++iter;
					}
					break;
				case 's': //char* string
					{
						char* string = va_arg(args, char*);

						if (string == nullptr)
						{
							++iter;
							break;
						}

						file << "\"" << std::flush;
						while (*string != '\0')
						{
							file << *string << std::flush;
						}
						file << "\"" << std::flush;
						// }string << "\"" << std::flush;
						++iter;
					}
					break;
				default:
					++iter;
				}
			}
			else
			{
				file << *iter << std::flush;
			}
		}

		va_end(args);

		file << " " << ToReturnCode(retCode) << std::endl << std::flush;
	}

	void Logger::LogError(std::string Message)
	{
		file << Message << std::endl << std::flush;
	}

	void Logger::CloseLog()
	{
		file << "-----END LOG-----" << std::endl << std::flush;
		file.close();
	}

	std::string Logger::ToReturnCode(unsigned long ReturnCode)
	{
		switch (ReturnCode)
		{
		case STATUS_NOERROR:
			return "STATUS_NOERROR";
		case ERR_NOT_SUPPORTED:
			return "ERR_NOT_SUPPORTED";
		case ERR_INVALID_CHANNEL_ID:
			return "ERR_INVALID_CHANNEL_ID";
		case ERR_INVALID_PROTOCOL_ID:
			return "ERR_INVALID_PROTOCOL_ID";
		case ERR_NULL_PARAMETER:
			return "ERR_NULL_PARAMETER";
		case ERR_INVALID_IOCTL_VALUE:
			return "ERR_INVALID_IOCTL_VALUE";
		case ERR_INVALID_FLAGS:
			return "ERR_INVALID_FLAGS";
		case ERR_FAILED:
			return "ERR_FAILED";
		case ERR_DEVICE_NOT_CONNECTED:
			return "ERR_DEVICE_NOT_CONNECTED";
		case ERR_TIMEOUT:
			return "ERR_TIMEOUT";
		case ERR_INVALID_MSG:
			return "ERR_INVALID_MSG";
		case ERR_INVALID_TIME_INTERVAL:
			return "ERR_INVALID_TIME_INTERVAL";
		case ERR_EXCEEDED_LIMIT:
			return "ERR_EXCEEDED_LIMIT";
		case ERR_INVALID_MSG_ID:
			return "ERR_INVLALID_MSG_ID";
		case ERR_DEVICE_IN_USE:
			return "ERR_DEVICE_IN_USE";
		case ERR_INVALID_IOCTL_ID:
			return "ERR_INVLAID_IOCTL_ID";
		case ERR_BUFFER_EMPTY:
			return "ERR_BUFFER_EMPTY";
		case ERR_BUFFER_FULL:
			return "ERR_BUFFER_FULL";
		case ERR_BUFFER_OVERFLOW:
			return "ERR_BUFFER_OVERFLOW";
		case ERR_PIN_INVALID:
			return "ERR_PIN_INVALID";
		case ERR_CHANNEL_IN_USE:
			return "ERR_CHANNEL_IN_USE";
		case ERR_MSG_PROTOCOL_ID:
			return "ERR_MSG_PROTOCOL_ID";
		case ERR_INVALID_FILTER_ID:
			return "ERR_INVALID_FILTER_ID";
		case ERR_NO_FLOW_CONTROL:
			return "ERR_NO_FLOW_CONTROL";
		case ERR_NOT_UNIQUE:
			return "ERR_NOT_UNIQUE";
		case ERR_INVALID_BAUDRATE:
			return "ERR_INVALID_BAUDRATE";
		case ERR_INVALID_DEVICE_ID:
			return "ERR_INVALID_DEVICE_ID";
		default:
			return "";
		}
	}

	std::string Logger::ToProtocolId(unsigned long ProtocolId)
	{
		switch (ProtocolId)
		{
		case J1850VPW:
			return "J1850VPW";
		case J1850PWM:
			return "J1850PWM";
		case ISO9141:
			return "ISO9141";
		case ISO14230:
			return "ISO14230";
		case CAN:
			return "CAN";
		case ISO15765:
			return "ISO15765";
		case SCI_A_ENGINE:
			return "SCI_A_ENGINE";
		case SCI_A_TRANS:
			return "SCI_A_TRANS";
		case SCI_B_ENGINE:
			return "SCI_B_ENGINE";
		case SCI_B_TRANS:
			return "SCI_B_TRANS";
		default:
			return "";
		}
	}

	std::string Logger::ToTxFlags(unsigned long TxFlags)
	{
		//TODO: Too lazy to implement correctly at the moment

		if (TxFlags > (ISO15765_FRAME_PAD | ISO15765_ADDR_TYPE | CAN_29BIT_ID | WAIT_P3_MIN_ONLY | SW_CAN_HV_TX |
			SCI_MODE | SCI_TX_VOLTAGE | DT_PERIODIC_UPDATE))
			return "";

		std::stringstream retStream;

		if (TxFlags & ISO15765_FRAME_PAD)
			retStream << "(ISO15765_FRAME_PAD)" << std::flush;
		if (TxFlags & ISO15765_ADDR_TYPE)
			retStream << "(ISO15765_ADDR_TYPE)" << std::flush;
		if (TxFlags & CAN_29BIT_ID)
			retStream << "(CAN_29BIT_ID)" << std::flush;
		if (TxFlags & WAIT_P3_MIN_ONLY)
			retStream << "(WAIT_P3_MIN_ONLY)" << std::flush;
		if (TxFlags & SW_CAN_HV_TX)
			retStream << "(SW_CAN_HV_TX)" << std::flush;
		if (TxFlags & SCI_MODE)
			retStream << "(SCI_MODE)" << std::flush;
		if (TxFlags & SCI_TX_VOLTAGE)
			retStream << "(SCI_TX_VOLTAGE)" << std::flush;
		if (TxFlags & DT_PERIODIC_UPDATE)
			retStream << "(DT_PERIODIC_UPDATE)" << std::flush;

		return retStream.str();
	}
}
