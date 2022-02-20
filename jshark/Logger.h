#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "j2534_v0404.h"

#define ulong unsigned long
#define uchar unsigned char

namespace JShark {
    enum DataDisplayType {
        NO_DISPLAY,
        ASCII_HEX_VALUES,
        HEX_VALUES_IN_TEXT,
        PRECEDING_ZERO_X,
        COMMA_DELIM,
        RAW
    };

    enum TransmitMessageType {
        RX,
        TX
    };

    class LogSettings {
    public:
        bool IgnoreRead = false;
        bool IgnoreWrite = false;
        bool IgnoreIoctl = false;
        bool IgnoreVersion = false;
        bool IgnoreOpen = false;
        bool IgnoreError = false;
        bool IgnoreSetVoltage = false;
        bool IgnoreFilters = false;
        bool IgnorePeriodics = false;
        bool IgnoreConnect = false;
        bool SaveDataOutput = true;
        bool SaveDataToSingleFile = true;
        DataDisplayType BinaryDisplayType = static_cast<DataDisplayType>(NO_DISPLAY);
        TransmitMessageType MessageType = TransmitMessageType::TX;
//        DataDisplayType BinaryDisplayType = static_cast<DataDisplayType>(HEX_VALUES_IN_TEXT | PRECEDING_ZERO_X | COMMA_DELIM);
    };


    class Logger
	{
	public:
		explicit Logger(std::string fileName);
        explicit Logger(std::string  fileName, LogSettings& logSettings);

		~Logger() = default;

		bool OpenLog(); 
		bool OpenLog(const std::string& fileName, LogSettings logSettings);

        //Logging data
        void LogData(const char* fmt, ...);

        //Basic message logging
        void LogMessage(const std::string& Message);
        void LogMessage(const std::string& Tag, const std::string& Message);
		void CloseLog();

		static std::string ToReturnCode(ulong ReturnCode);
		static std::string ToProtocolId(ulong ProtocolId);
		static std::string ToTxFlags(ulong TxFlags);
        static std::string ToRxStatus(ulong RxFlags);

        void SetMessageType(TransmitMessageType messageType);

	protected:
	private:
		size_t pMsgIdx = 0;

		std::ofstream file;
        std::fstream dataFile;

		std::string fileName;

        LogSettings logSettings;

        ulong passThruMessageId = 0;

        void LogPassThruMessage(PASSTHRU_MSG* pMsg);
        void LogMessageData(const std::string& fileNameId, PASSTHRU_MSG* pMsg);

	};
}
