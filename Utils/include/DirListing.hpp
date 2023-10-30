#include <string>

class DirListing
{
    private:
        std::string GenerateFileRow(const std::string &parent, const std::string &name);
        std::string MakeHtml(std::string DirName, std::string HtmlContent);
    public :
        bool getDirlistigHtml(const std::string path, std::string &output);
};


