#include "t2a_utils.h"

namespace t2a
{
std::string load_file_in_string(const boost::filesystem::path& file_path)
{
    std::ifstream file(file_path.string().c_str(),
                       std::ios::in | std::ios::binary | std::ios::ate);
    long file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string final_str(static_cast<size_t>(file_size), 0);
    file.read(const_cast<char*>(final_str.data()), file_size);
    file.close();
    return final_str;
}

void check_file(const boost::filesystem::path& file_path)
{
    if (boost::filesystem::exists(file_path) == false)
    {
        throw "Cannot open file, please check either the path or if the file"
        " can be read";
    }
}
}
