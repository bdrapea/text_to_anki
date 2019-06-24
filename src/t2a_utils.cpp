#include "t2a_utils.h"

namespace t2a
{
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

void check_database(const boost::filesystem::path& database_path)
{
    sqlite3* tmp_db = nullptr;
    int sql_result = sqlite3_open(database_path.string().c_str(), &tmp_db);
    if (sql_result != SQLITE_OK)
    {
        throw sqlite3_errstr(sql_result);
    }
}

void check_output_database(const boost::filesystem::path& output_db_path)
{
    if (boost::filesystem::exists(output_db_path))
    {
        std::cerr << "Output database already present: deleting it ..."
                  << std::endl;
        remove(output_db_path);
    }
}

void create_empty_anki_media_file(
    const boost::filesystem::path& anki_media_file_path)
{
    std::ofstream media_file(
        anki_media_file_path.string().c_str(),
        std::ios::out);
    media_file << "{}";
    media_file.close();
}

void create_default_anki2_file(const boost::filesystem::path& anki2_path)
{
    sqlite3* anki2_db = nullptr;
    sqlite3_open(anki2_path.string().c_str(), &anki2_db);
    sqlite3_close(anki2_db);
}

void check_output_anki_apkg(const boost::filesystem::path& anki_apkg_path)
{
    if (boost::filesystem::exists(anki_apkg_path))
    {
        std::cerr << "Output database already present: deleting it ..."
                  << std::endl;
        remove(anki_apkg_path);
    }
}

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
