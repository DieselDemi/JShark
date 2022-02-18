#pragma once

#include <string>

namespace JShark::Helpers {
    class MessageBoxHelper {
    public:
        static void ShowMessageBox(std::string message);
    };

    class StringConversion {
    public:
        static std::wstring ToWString(std::string);

        static std::string ToStdString(std::wstring);
    };
}
