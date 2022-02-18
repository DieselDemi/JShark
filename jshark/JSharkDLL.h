/*
**
*/

#pragma once
#include "j2534_v0404.h"

/*
** Define all the PassThru function prototypes for the J2534 API.
*/

/*
typedef long (CALLBACK* PTOPEN)(void *pName, unsigned long *pDeviceID);
typedef long (CALLBACK* PTCLOSE)(unsigned long pDeviceID);
typedef long (CALLBACK* PTCONNECT)(unsigned long ProtocolID, unsigned long Flags, unsigned long *pChannelID);
typedef long (CALLBACK* PTDISCONNECT)(unsigned long ChannelID);
typedef long (CALLBACK* PTREADMSGS)(unsigned long ChannelID, void * pMsg, unsigned long * pNumMsgs, unsigned long Timeout);
typedef long (CALLBACK* PTWRITEMSGS)(unsigned long ChannelID, void *  pMsg, unsigned long * pNumMsgs, unsigned long Timeout);
typedef long (CALLBACK* PTSTARTPERIODICMSG)(unsigned long ChannelID, void * pMsg, unsigned long * pMsgID, unsigned long TimeInterval);
typedef long (CALLBACK* PTSTOPPERIODICMSG)(unsigned long ChannelID, unsigned long MsgID);
typedef long (CALLBACK* PTSTARTMSGFILTER)(unsigned long ChannelID, unsigned long FilterType, void * pMaskMsg, void * pPatternMsg, void * pFlowControlMsg, unsigned long * pFilterID);
typedef long (CALLBACK* PTSTOPMSGFILTER)(unsigned long ChannelID, unsigned long FilterID);
typedef long (CALLBACK* PTSETPROGRAMMINGVOLTAGE)(unsigned long PinNumber, unsigned long Voltage);
typedef long (CALLBACK* PTREADVERSION)(char *pFirmwareVersion, char *pDllVersion, char *pApiVersion);
typedef long (CALLBACK* PTGETLASTERROR)(char *pErrorDescription);
typedef long (CALLBACK* PTIOCTL)(unsigned long ChannelID, unsigned long IoctlID, void *pInput, void * pOutput);
*/

/*
** Define permanent storage for all the PassThru function addresses.
*/
extern PTOPEN	PassThruOpen;
extern PTCLOSE	PassThruClose;
extern PTCONNECT PassThruConnect;
extern PTDISCONNECT PassThruDisconnect;
extern PTREADMSGS PassThruReadMsgs;
extern PTWRITEMSGS PassThruWriteMsgs;
extern PTSTARTPERIODICMSG PassThruStartPeriodicMsg;
extern PTSTOPPERIODICMSG PassThruStopPeriodicMsg;
extern PTSTARTMSGFILTER PassThruStartMsgFilter;
extern PTSTOPMSGFILTER PassThruStopMsgFilter;
extern PTSETPROGRAMMINGVOLTAGE PassThruSetProgrammingVoltage;
extern PTREADVERSION PassThruReadVersion;
extern PTGETLASTERROR PassThruGetLastError;
extern PTIOCTL PassThruIoctl;

