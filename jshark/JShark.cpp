/*
** This is the main DLL implementation
**
*/


#include "stdafx.h"
#include "JShark.h"
#include "Registry.h"
#include <string>

#include "Logger.h"
#include "Helpers.h"

namespace JShark
{
	// Pointers to J2534 API functions in the loaded library
	PTOPEN _PassThruOpen = 0;
	PTCLOSE _PassThruClose = 0;
	PTCONNECT _PassThruConnect = 0;
	PTDISCONNECT _PassThruDisconnect = 0;
	PTREADMSGS _PassThruReadMsgs = 0;
	PTWRITEMSGS _PassThruWriteMsgs = 0;
	PTSTARTPERIODICMSG _PassThruStartPeriodicMsg = 0;
	PTSTOPPERIODICMSG _PassThruStopPeriodicMsg = 0;
	PTSTARTMSGFILTER _PassThruStartMsgFilter = 0;
	PTSTOPMSGFILTER _PassThruStopMsgFilter = 0;
	PTSETPROGRAMMINGVOLTAGE _PassThruSetProgrammingVoltage = 0;
	PTREADVERSION _PassThruReadVersion = 0;
	PTGETLASTERROR _PassThruGetLastError = 0;
	PTIOCTL _PassThruIoctl = 0;

	static HINSTANCE hDLL = NULL;
	static bool fLibLoaded = false;

	static int last_error;

	static std::string logFilePath;
	static std::wstring wLogFilePath; 
	static std::wstring dllFilePath;

	static Logger logger(logFilePath);


	bool shim_loadLibrary(const std::wstring& szDLL)
	{
		// Can't load a library if the string is NULL
		// if (szDLL == NULL)
		// {
		// 	return false;
		// }

		// Can't load a library if there's one currently loaded
		if (fLibLoaded)
		{
			return false;
		}

		hDLL = LoadLibraryW(szDLL.c_str());
		if (hDLL == NULL)
		{
			// Try to get the error text
			// Set the internal error text based on the win32 message
			return false;
		}

		fLibLoaded = true;

		_PassThruOpen = (PTOPEN)GetProcAddress(hDLL, "PassThruOpen");
		_PassThruClose = (PTCLOSE)GetProcAddress(hDLL, "PassThruClose");
		_PassThruConnect = (PTCONNECT)GetProcAddress(hDLL, "PassThruConnect");
		_PassThruDisconnect = (PTDISCONNECT)GetProcAddress(hDLL, "PassThruDisconnect");
		_PassThruReadMsgs = (PTREADMSGS)GetProcAddress(hDLL, "PassThruReadMsgs");
		_PassThruWriteMsgs = (PTWRITEMSGS)GetProcAddress(hDLL, "PassThruWriteMsgs");
		_PassThruStartPeriodicMsg = (PTSTARTPERIODICMSG)GetProcAddress(hDLL, "PassThruStartPeriodicMsg");
		_PassThruStopPeriodicMsg = (PTSTOPPERIODICMSG)GetProcAddress(hDLL, "PassThruStopPeriodicMsg");
		_PassThruStartMsgFilter = (PTSTARTMSGFILTER)GetProcAddress(hDLL, "PassThruStartMsgFilter");
		_PassThruStopMsgFilter = (PTSTOPMSGFILTER)GetProcAddress(hDLL, "PassThruStopMsgFilter");
		_PassThruSetProgrammingVoltage = (PTSETPROGRAMMINGVOLTAGE)GetProcAddress(hDLL, "PassThruSetProgrammingVoltage");
		_PassThruReadVersion = (PTREADVERSION)GetProcAddress(hDLL, "PassThruReadVersion");
		_PassThruGetLastError = (PTGETLASTERROR)GetProcAddress(hDLL, "PassThruGetLastError");
		_PassThruIoctl = (PTIOCTL)GetProcAddress(hDLL, "PassThruIoctl");

		return true;
	}

	void shim_unloadLibrary()
	{
		// Can't unload a library if there's nothing loaded
		if (!fLibLoaded)
			return;

		fLibLoaded = false;

		// Invalidate the function pointers
		_PassThruOpen = NULL;
		_PassThruClose = NULL;
		_PassThruConnect = NULL;
		_PassThruDisconnect = NULL;
		_PassThruReadMsgs = NULL;
		_PassThruWriteMsgs = NULL;
		_PassThruStartPeriodicMsg = NULL;
		_PassThruStopPeriodicMsg = NULL;
		_PassThruStartMsgFilter = NULL;
		_PassThruStopMsgFilter = NULL;
		_PassThruSetProgrammingVoltage = NULL;
		_PassThruReadVersion = NULL;
		_PassThruGetLastError = NULL;
		_PassThruIoctl = NULL;

		BOOL fSuccess;
		fSuccess = FreeLibrary(hDLL);
		if (!fSuccess)
		{
			// Try to get the error text
			// Set the internal error text based on the win32 message
		}
	}

	bool shim_hasLibraryLoaded()
	{
		return fLibLoaded;
	}

	/**
	 * Called when JShark DLL is being unloaded from memory
	 */
	void ShimStop()
	{
		logger.CloseLog();

		if (shim_hasLibraryLoaded())
		{
			shim_unloadLibrary();
		}
	}

	

	bool ShimStart()
	{
		HKEY hKey;
		LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\PassThruSupport.04.04\\JSHARK", 0, KEY_READ, &hKey);

		if (lRes == ERROR_FILE_NOT_FOUND)
			return false;

		if (JSharkRegistry::GetStringRegKey(hKey, L"DllPath", dllFilePath, L"") != ERROR_SUCCESS)
			return false;
		if (JSharkRegistry::GetStringRegKey(hKey, L"LogPath", wLogFilePath, L"output.log") != ERROR_SUCCESS)
			return false;

		//FUCK WINDOWS AND EVERYTHING IT STANDS FOR
		logFilePath = Helpers::StringConversion::ToStdString(wLogFilePath); 

		logger.OpenLog(logFilePath);

		return true; 
	}


	///////////////////////////////////// PassThruFunctions /////////////////////////////////////////////////

	DllExport PassThruOpen(void* pName, unsigned long* pDeviceID)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruOpen(pName, pDeviceID);

		logger.LogData("PassThruOpen(%v, %u);", last_error, pName, *pDeviceID); 

		return last_error;
	}

	DllExport PassThruClose(unsigned long DeviceID)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruClose(DeviceID);

		logger.LogData("PassThruClose(%u);", last_error, DeviceID); 
		
		return last_error;
	}

	DllExport PassThruConnect(unsigned long DeviceID, unsigned long ProtocolID, unsigned long Flags,
	                          unsigned long Baudrate, unsigned long* pChannelID)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruConnect(DeviceID, ProtocolID, Flags, Baudrate, pChannelID);

		logger.LogData("PassThruConnect(%u, %r, %x, %u, %u);", last_error, DeviceID, ProtocolID, Flags, Baudrate, *pChannelID); 

		return last_error;
	}


	DllExport PassThruDisconnect(unsigned long ChannelID)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruDisconnect(ChannelID);

		logger.LogData("PassThruDisconnect(%u);", last_error, ChannelID); 

		return last_error;
	}


	DllExport PassThruReadMsgs(unsigned long ChannelID, PASSTHRU_MSG* pMsg, unsigned long* pNumMsgs,
	                           unsigned long Timeout)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruReadMsgs(ChannelID, pMsg, pNumMsgs, Timeout);

		logger.LogData("PassThruReadMsgs(%u, %p, %u, %u);", last_error, ChannelID, pMsg, *pNumMsgs, Timeout); 

		return last_error;
	}


	DllExport PassThruWriteMsgs(unsigned long ChannelID, PASSTHRU_MSG* pMsg, unsigned long* pNumMsgs,
	                            unsigned long Timeout)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruWriteMsgs(ChannelID, pMsg, pNumMsgs, Timeout);

		logger.LogData("PassThruWriteMsgs(%u, %p, %u, %u);", last_error, ChannelID, pMsg, *pNumMsgs, Timeout); 

		return last_error;
	}


	DllExport PassThruStartPeriodicMsg(unsigned long ChannelID, PASSTHRU_MSG* pMsg, unsigned long* pMsgID,
	                                   unsigned long TimeInterval)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruStartPeriodicMsg(ChannelID, pMsg, pMsgID, TimeInterval);

		logger.LogData("PassThruStartPeriodicMsg(%u, %p, %u, %u);", last_error, ChannelID, pMsg, *pMsgID, TimeInterval); 

		return last_error;
	}


	DllExport PassThruStopPeriodicMsg(unsigned long ChannelID, unsigned long MsgID)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruStopPeriodicMsg(ChannelID, MsgID);

		logger.LogData("PassThruStopPeriodicMsg(%u, %u);", last_error, ChannelID, MsgID); 

		return last_error;
	}


	DllExport PassThruStartMsgFilter(unsigned long ChannelID, unsigned long FilterType, PASSTHRU_MSG* pMaskMsg,
	                                 PASSTHRU_MSG* pPatternMsg, PASSTHRU_MSG* pFlowControlMsg, unsigned long* pFilterID)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}


		last_error = _PassThruStartMsgFilter(ChannelID, FilterType, pMaskMsg, pPatternMsg, pFlowControlMsg, pFilterID);

		logger.LogData("PassThruStartMsgFilter(%u, %u, %p, %p, %p, %u);", last_error, ChannelID, FilterType, pMaskMsg, pPatternMsg, pFlowControlMsg, *pFilterID); 

		return last_error;
	}


	DllExport PassThruStopMsgFilter(unsigned long ChannelID, unsigned long FilterID)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}


		last_error = _PassThruStopMsgFilter(ChannelID, FilterID);

		logger.LogData("PassThruStopMsgFilter(%u, %u);", last_error, ChannelID, FilterID);

		return last_error;
	}


	DllExport PassThruSetProgrammingVoltage(unsigned long DeviceID, unsigned long PinNumber, unsigned long Voltage)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruSetProgrammingVoltage(DeviceID, PinNumber, Voltage);

		logger.LogData("PassThruSetProgrammingVoltage(%u, %u, %u);", last_error, DeviceID, PinNumber, Voltage); 

		return last_error;
	}


	DllExport PassThruReadVersion(unsigned long DeviceID, char* pFirmwareVersion, char* pDllVersion, char* pApiVersion)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}


		last_error = _PassThruReadVersion(DeviceID, pFirmwareVersion, pDllVersion, pApiVersion);

		const auto firmwareVersion = static_cast<char*>(malloc(sizeof(char) * 4128));
		const auto dllVersion = static_cast<char*>(malloc(sizeof(char) * 4128));
		const auto apiVersion = static_cast<char*>(malloc(sizeof(char) * 4128));

		strcpy(firmwareVersion, pFirmwareVersion);
		strcpy(dllVersion, pDllVersion);
		strcpy(apiVersion, pApiVersion);


		// logger.LogData("PassThruReadVersion(%u, %s, %s, %s);", last_error, DeviceID, firmwareVersion, dllVersion, apiVersion); 

		free(firmwareVersion);
		free(dllVersion);
		free(apiVersion);

		return last_error;
	}


	// We don't alter any last_errors in this function
	DllExport PassThruGetLastError(char* pErrorDescription)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruGetLastError(pErrorDescription);

		logger.LogData("PassThruGetLastError(%s);", last_error, pErrorDescription); 

		return last_error;
	}


	DllExport PassThruIoctl(unsigned long ChannelID, unsigned long IoctlID, void* pInput, void* pOutput)
	{
		if (!shim_hasLibraryLoaded())
		{
			if (!shim_loadLibrary(dllFilePath))
			{
				logger.LogError("ERROR: could not load dll");
				return last_error = ERR_FAILED;
			}
		}

		last_error = _PassThruIoctl(ChannelID, IoctlID, pInput, pOutput);

		logger.LogData("PassThruIoctl(%u, %u, %v, %v);", ChannelID, IoctlID, pInput, pOutput); 

		return last_error;
	}
}
