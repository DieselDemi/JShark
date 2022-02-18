/*
**
**
*/

#pragma once
#include "j2534_v0404.h"

#define DllExport extern "C" long __stdcall

namespace JShark {
	void ShimStop();
	bool ShimStart();
}

// J2534 Functions
DllExport PassThruOpen(void *pName, unsigned long *pDeviceID);
DllExport PassThruClose(unsigned long DeviceID);
DllExport PassThruConnect(unsigned long DeviceID, unsigned long ProtocolID, unsigned long Flags, unsigned long Baudrate, unsigned long *pChannelID);
DllExport PassThruDisconnect(unsigned long ChannelID);
DllExport PassThruReadMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);
DllExport PassThruWriteMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);
DllExport PassThruStartPeriodicMsg(unsigned long ChannelID, PASSTHRU_MSG * pMsg, unsigned long *pMsgID, unsigned long TimeInterval);
DllExport PassThruStopPeriodicMsg(unsigned long ChannelID, unsigned long MsgID);
DllExport PassThruStartMsgFilter(unsigned long ChannelID, unsigned long FilterType, PASSTHRU_MSG *pMaskMsg, PASSTHRU_MSG *pPatternMsg, PASSTHRU_MSG *pFlowControlMsg, unsigned long *pFilterID);
DllExport PassThruStopMsgFilter(unsigned long ChannelID, unsigned long FilterID);
DllExport PassThruSetProgrammingVoltage(unsigned long DeviceID, unsigned long PinNumber, unsigned long Voltage);
DllExport PassThruReadVersion(unsigned long DeviceID, char *pFirmwareVersion, char *pDllVersion, char *pApiVersion);
DllExport PassThruGetLastError(char *pErrorDescription);
DllExport PassThruIoctl(unsigned long ChannelID, unsigned long IoctlID, void *pInput, void *pOutput);


