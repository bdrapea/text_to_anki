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

std::string sql_insert_vocabulary(const char* word, const char* meaning)
{
    std::string retval = "INSERT INTO Vocabulary VALUES (";
    retval += '\'';
    retval += word;
    retval += '\'';
    retval += ',';
    retval += '\'';
    retval += meaning;
    retval += '\'';
    retval += ");";

    return retval;
}

void check_file(const boost::filesystem::path& file_path)
{
    if (boost::filesystem::exists(file_path) == false)
    {
        throw "Cannot open file, please check either the path or if the file"
        " can be read";
    }
}

void check_database(const boost::filesystem::path& database_path)
{
    sqlite3* tmp_db = nullptr;
    int sql_result = sqlite3_open(database_path.string().c_str(), &tmp_db);
    if (sql_result != SQLITE_OK)
    {
        throw sqlite3_errstr(sql_result);
    }
}
}
