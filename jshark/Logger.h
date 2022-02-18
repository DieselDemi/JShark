#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "j2534_v0404.h"

#define ulong unsigned long

namespace JShark {
	class Logger
	{
	public:
		explicit Logger(std::string fileName);

		~Logger() = default;

		bool OpenLog(); 
		bool OpenLog(const std::string& fileName); 
		void LogData(std::string fmt, ulong retCode, ...); 
		void LogError(std::string Message); 
		void CloseLog();
	protected:
	private:
		size_t pMsgIdx = 0;

		std::ofstream file;
		std::string fileName; 
		static std::string ToReturnCode(ulong ReturnCode);
		static std::string ToProtocolId(ulong ProtocolId);
		static std::string ToTxFlags(ulong TxFlags); 
	};
}
