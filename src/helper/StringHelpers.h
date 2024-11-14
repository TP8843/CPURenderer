#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H
#include <string>


struct StringHelpers {
#ifdef OS_Windows
    static std::string pathSeparator() { return "\\"; };
#else
    static std::string pathSeparator() { return "/"; };
#endif

    static std::string getFolderPath(const std::string &path);

    // Concatenate file and folder
    static std::string concatFolderFile(const std::string& path, const std::string& file);

    static std::string trimLine(const std::string &line);

private:
    static bool bothAreSpaces(char l, char r);
};



#endif //STRINGHELPERS_H
