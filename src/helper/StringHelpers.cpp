#include "StringHelpers.h"

#include <algorithm>

std::string StringHelpers::getFolderPath(const std::string& path)
{
    const auto lastSlashPos = path.find_last_of("/\\");
    std::string parentPath = (std::string::npos == lastSlashPos)
                                 ? ""
                                 : path.substr(0, lastSlashPos);

    return parentPath;
}

std::string StringHelpers::concatFolderFile(const std::string& path, const std::string& file)
{
    if (path.empty()) return file;

    const bool fileHasPathSeparator = file.find_first_of("/\\") == 0;
    const bool folderHasPathSeparator = path.find_last_of("/\\") == std::string::npos;

    const std::string finalFolderPath =
        folderHasPathSeparator ? path : path + pathSeparator();

    const std::string finalFilePath =
        fileHasPathSeparator ? file.substr(1, file.length() - 1) : file;

    return finalFolderPath + finalFilePath;
}

std::string StringHelpers::trimLine(const std::string& line)
{
    auto result = std::string(line);

    if (result.empty()) return result;

    // Trim whitespace from start and end of line
    const unsigned long newStartPos = result.find_first_not_of(" \t\r");
    unsigned long newEndPos = result.find_last_not_of(" \t\r") + 1;

    if (newStartPos > result.size()) return { "" };

    result = result.substr(newStartPos, newEndPos - newStartPos);

    // Remove duplicate spaces from line. From https://stackoverflow.com/questions/8362094/replace-multiple-spaces-with-one-space-in-a-string
    const auto newEnd = std::unique(result.begin(), result.end(), bothAreSpaces);
    result.erase(newEnd, result.end());

    // Trim away comments
    newEndPos = result.find_first_of('#');
    result = result.substr(0, newEndPos);

    return result;
}

bool StringHelpers::bothAreSpaces(const char l, const char r)
{
    return (l == r && l == ' ');
}
