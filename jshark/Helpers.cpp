#include "stdafx.h"
#include "Helpers.h"

#include <codecvt>
#include <locale>
#include <sstream>

namespace JShark::Helpers {
    namespace MessageBoxHelpers {
        void ShowMessageBox(const std::string &title, const std::string &message) {
            MessageBoxW(
                    nullptr,
                    StringHelpers::ToWString(message).c_str(),
                    (LPCWSTR) StringHelpers::ToWString(title).c_str(),
                    MB_ICONWARNING | MB_OK
            );
        }
    }

    namespace StringHelpers {

        std::vector<std::string> Split(std::string& in, char delim) {
            std::vector<std::string> output;

            std::string::size_type prev_pos = 0, pos = 0;

            while((pos = in.find(delim, pos)) != std::string::npos)
            {
                std::string substring(in.substr(prev_pos, pos-prev_pos) );

                output.push_back(substring);

                prev_pos = ++pos;
            }

            output.push_back(in.substr(prev_pos, pos-prev_pos)); // Last word

            return output;
        }

        std::string FileNameToPath(std::string& fileName) {
            std::vector<std::string> filePathList = Split(fileName, '/');
            filePathList.pop_back();

            return Join(filePathList, '/');
        }

        std::wstring ToWString(const std::string& str) {
            using convert_t = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_t, wchar_t> strconverter;

            return strconverter.from_bytes(str);
        }

        std::string ToStdString(const std::wstring& wstr) {
            using convert_t = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_t, wchar_t> strconverter;

            return strconverter.to_bytes(wstr);
        }

        std::string Join(std::vector<std::string> &list, char delim) {
            std::stringstream ss;

            for(const auto& str : list){
                ss << str << delim << std::flush;
            }

            return ss.str();
        }
    }
}
