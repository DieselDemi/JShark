#include "stdafx.h"
#include "Logger.h"
#include "Helpers.h"

#include <iomanip>
#include <sstream>
#include <utility>

namespace JShark {
    Logger::Logger(std::string fileName)
            : fileName(std::move(fileName)) {/*No Construction*/}

    Logger::Logger(std::string fileName, LogSettings &logSettings)
            : fileName(std::move(fileName)), logSettings(logSettings) {/*No Construction*/}

    bool Logger::OpenLog() {
        if (!this->fileName.empty()) {
            file.open(this->fileName, std::fstream::out | std::fstream::app);
            file << "------ NEW LOG ------" << std::endl << std::flush;
        } else
            return false;
        return true;
    }

    bool Logger::OpenLog(const std::string &fileName, LogSettings logSettings) {
        this->fileName = fileName;
        file.open(fileName, std::fstream::out | std::fstream::app);
        file << "------ NEW LOG ------" << std::endl << std::flush;

        this->logSettings = logSettings;

        return true;
    }

    void Logger::LogData(const char *fmt, ...) {
        va_list args;
                va_start(args, fmt);

        while (*fmt != '\0') {
            if (*fmt == '%') {
                switch (*(fmt + 1)) {
                    case 'c': //Return Code
                    {
                        const ulong ret = va_arg(args, ulong);
                        file << ToReturnCode(ret) << std::endl << std::flush;
                        ++fmt;
                        break;
                    }
                    case 'r': //ULONG to ProtocolID
                    {
                        const ulong out = va_arg(args, ulong);
                        file << ToProtocolId(out) << std::flush;
                        ++fmt;
                        break;
                    }
                    case 'u': //ULONG
                    {
                        const ulong out = va_arg(args, ulong);
                        file << std::to_string(out) << std::flush;
                        ++fmt;
                        break;
                    }
                    case 'x': //ULONG as HEX
                    {
                        const ulong out = va_arg(args, ulong);
                        file << "0x" << std::hex << std::setw(sizeof(ulong) / 8) << std::setfill('0') <<
                             std::to_string(out) << std::flush;
                        ++fmt;
                        break;
                    }
                    case 'v': //Void Pointer
                    {
                        file << "*void" << std::flush;
                        ++fmt;
                        break;
                    }
                    case 'p': //PassThru Message
                    {
                        auto *pMsg = va_arg(args, PASSTHRU_MSG*);

                        if (pMsg == nullptr) {
                            ++fmt;
                            break;
                        }

                        LogPassThruMessage(pMsg);

                        ++fmt;
                        break;
                    }
                    case 's': //char* string
                    {
                        std::string string = va_arg(args, std::string);
                        file << "\"" << string << "\"" << std::flush;
                        ++fmt;
                        break;
                    }
                    default:
                        file << *fmt << std::flush;
                        break;
                }
            } else {
                file << *fmt << std::flush;
            }
            ++fmt;
        }

                va_end(args);

        file << std::endl << std::flush;
    }

    void Logger::LogMessage(const std::string &Message) {
        file << Message << std::endl << std::flush;
    }

    void Logger::LogMessage(const std::string &Tag, const std::string &Message) {
        file << Tag << ": " << Message << std::endl;
    }

    void Logger::CloseLog() {
        file << "-----END LOG-----" << std::endl << std::flush;
        file.close();
    }

    std::string Logger::ToReturnCode(unsigned long ReturnCode) {
        switch (ReturnCode) {
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

    std::string Logger::ToProtocolId(unsigned long ProtocolId) {
        switch (ProtocolId) {
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

    std::string Logger::ToTxFlags(unsigned long TxFlags) {
        //TODO: Too lazy to implement correctly at the moment

//        if (TxFlags > (ISO15765_FRAME_PAD | ISO15765_ADDR_TYPE | CAN_29BIT_ID | WAIT_P3_MIN_ONLY | SW_CAN_HV_TX |
//                       SCI_MODE | SCI_TX_VOLTAGE | DT_PERIODIC_UPDATE))
//            return "";

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
        if (TxFlags & CAN_ID_BOTH)
            retStream << "(CAN_ID_BOTH)" << std::flush;

        return retStream.str();
    }

    std::string Logger::ToRxStatus(unsigned long RxStatus) {

//        if (RxStatus > (START_OF_MESSAGE | ISO15765_FIRST_FRAME | ISO15765_EXT_ADDR | RX_BREAK | TX_INDICATION |
//                ISO15765_PADDING_ERROR | ISO15765_ADDR_TYPE | CAN_29BIT_ID |
//                SW_CAN_NS_RX | SW_CAN_HS_RX | SW_CAN_HV_RX ))
//            return "";

        std::stringstream retStream;

        if (RxStatus & TX_MSG_TYPE)
            retStream << "(TX_MSG_TYPE)" << std::flush;
        if (RxStatus & START_OF_MESSAGE)
            retStream << "(START_OF_MESSAGE)" << std::flush;
        if (RxStatus & ISO15765_FIRST_FRAME)
            retStream << "(ISO15765_FIRST_FRAME)" << std::flush;
        if (RxStatus & ISO15765_EXT_ADDR)
            retStream << "(ISO15765_EXT_ADDR)" << std::flush;
        if (RxStatus & RX_BREAK)
            retStream << "(RX_BREAK)" << std::flush;
        if (RxStatus & TX_INDICATION)
            retStream << "(TX_INDICATION)" << std::flush;
        if (RxStatus & ISO15765_PADDING_ERROR)
            retStream << "(ISO15765_PADDING_ERROR)" << std::flush;
        if (RxStatus & ISO15765_ADDR_TYPE)
            retStream << "(ISO15765_ADDR_TYPE)" << std::flush;
        if (RxStatus & CAN_29BIT_ID)
            retStream << "(CAN_29BIT_ID)" << std::flush;
        if (RxStatus & SW_CAN_NS_RX)
            retStream << "(SW_CAN_NS_RX)" << std::flush;
        if (RxStatus & SW_CAN_HS_RX)
            retStream << "(SW_CAN_HS_RX)" << std::flush;
        if (RxStatus & SW_CAN_HV_RX)
            retStream << "(SW_CAN_HV_RX)" << std::flush;


        return retStream.str();
    }

    void Logger::LogPassThruMessage(PASSTHRU_MSG *pMsg) {
        file << "{" << std::endl << std::flush;

        //Protocol ID
        file << "\tProtocol ID: 0x" << std::hex << pMsg->ProtocolID << " (" <<
             ToProtocolId(pMsg->ProtocolID) << ")" << std::endl << std::flush;

        //Rx Status
        if (logSettings.MessageType == TransmitMessageType::RX)
            file << "\tRxStatus: 0x" << std::hex << pMsg->RxStatus <<
                 " (" << ToRxStatus(pMsg->RxStatus) << ")" << std::endl << std::flush;
        else
            file << "\tRxStatus:" << std::endl << std::flush;

        //Tx Flags
        if (logSettings.MessageType == TransmitMessageType::TX)
            file << "\tTxFlags: 0x" << std::hex << pMsg->TxFlags <<
                 " (" << ToTxFlags(pMsg->TxFlags) << ")" << std::endl << std::flush;
        else
            file << "\tTxFlags:" << std::endl << std::flush;

        //Timestamp
        file << "\tTimeStamp: " << std::to_string(pMsg->Timestamp) << std::endl << std::flush;

        //DataSize
        file << "\tDataSize: " << std::to_string(pMsg->DataSize) << std::endl << std::flush;

        //ExtraData Index
        file << "\tExtraData Idx: " << std::to_string(pMsg->ExtraDataIndex) << std::endl
             << std::flush;

        file << "\tData: " << std::flush;

        if (pMsg->DataSize == 0 || pMsg->DataSize > 4128) {
            file << "\t[]\n" << std::flush;
        } else {
            if (logSettings.SaveDataOutput) {
                std::stringstream fileNameSS;

                fileNameSS << "data" << ++passThruMessageId << ".bin" << std::flush;

                LogMessageData(fileNameSS.str(), pMsg);
                if(logSettings.SaveDataToSingleFile)
                    file << "data.bin" << std::flush;
                else
                    file << fileNameSS.str() << std::flush;
            } else {
                file << std::endl << std::flush;
                if (!(logSettings.BinaryDisplayType & DataDisplayType::NO_DISPLAY)) {
                    file << "\t[\t" << std::flush;
                    for (size_t i = 0; i < pMsg->DataSize; i++) {
                        if (i % 0x0F == 0)
                            file << std::endl << "\t" << std::flush;

                        if (logSettings.BinaryDisplayType & DataDisplayType::PRECEDING_ZERO_X)
                            file << "0x" << std::flush;
                        if (logSettings.BinaryDisplayType & DataDisplayType::ASCII_HEX_VALUES)
                            file << std::hex << (pMsg->Data[i] - '0') << std::flush;
                        if (logSettings.BinaryDisplayType & DataDisplayType::HEX_VALUES_IN_TEXT)
                            file << std::hex << pMsg->Data[i] << std::flush;
                        if (logSettings.BinaryDisplayType & DataDisplayType::COMMA_DELIM)
                            file << ", " << std::flush;
                        if ((logSettings.BinaryDisplayType & DataDisplayType::RAW) &&
                            !(logSettings.BinaryDisplayType &
                              (DataDisplayType::ASCII_HEX_VALUES | DataDisplayType::HEX_VALUES_IN_TEXT)))
                            file << pMsg->Data[i] << std::flush;
                    }
                    file << "\n\t]" << std::flush;
                } else {
                    file << "[]" << std::flush;
                }
            }
        }

        file << std::endl << std::flush;

        file << "}" << std::flush;
    }

    void Logger::LogMessageData(const std::string &fileNameId, PASSTHRU_MSG *pMsg) {
        std::string path;

        if(logSettings.SaveDataToSingleFile)
            path = JShark::Helpers::StringHelpers::FileNameToPath(fileName) + "data.bin";
        else
            path = JShark::Helpers::StringHelpers::FileNameToPath(fileName) + fileNameId;

        dataFile.open(path, std::ios::binary | std::ios::out);

        if (pMsg->DataSize > 4128)
            return; //Don't process a struct that is too big, fail and bail

        for (size_t i = 0; i < pMsg->DataSize; i++)
            dataFile << pMsg->Data[i] << std::flush;

        if(logSettings.SaveDataToSingleFile)
            dataFile << "---DATA-BLOCK---" << std::flush;

        dataFile.close();
    }

    void Logger::SetMessageType(TransmitMessageType messageType) {
        this->logSettings.MessageType = messageType;
    }
}
