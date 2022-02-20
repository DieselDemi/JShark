#pragma once

#include <string>
#include <vector>

namespace JShark::Helpers {
    namespace MessageBoxHelpers {
        void ShowMessageBox(const std::string&, const std::string&);

    }

    namespace StringHelpers {
        std::vector<std::string> Split(std::string& in, char delim);
        std::string Join(std::vector<std::string>& list, char delim);

        std::string FileNameToPath(std::string& fileName);

        std::wstring ToWString(const std::string&);
        std::string ToStdString(const std::wstring&);
    }
}
