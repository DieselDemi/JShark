#include "stdafx.h"
#include "Helpers.h"

#include <codecvt>
#include <locale>

namespace JShark::Helpers {
    void MessageBoxHelper::ShowMessageBox(std::string message) {
        MessageBoxW(
                NULL,
                StringConversion::ToWString(message).c_str(),
                (LPCWSTR) L"Account Details",
                MB_ICONWARNING | MB_OK
        );
    }

    std::wstring StringConversion::ToWString(std::string str) {
        using convert_t = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_t, wchar_t> strconverter;

        return strconverter.from_bytes(str);
    }

    std::string StringConversion::ToStdString(std::wstring wstr) {
        using convert_t = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_t, wchar_t> strconverter;

        return strconverter.to_bytes(wstr);
    }
}
