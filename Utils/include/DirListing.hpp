#include <string>

class DirListing
{
    private:
        static std::string GenerateFileRow(const std::string &parent, const std::string &name ,  std::string vt_path);
        static std::string MakeHtml(std::string DirName, std::string HtmlContent);
    public :
       static  bool getDirlistigHtml(const std::string path, std::string &output,  std::string vt_path);
};


