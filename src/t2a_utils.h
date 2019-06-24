#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include <sqlite3.h>

namespace t2a
{
constexpr const int WORD_COLUMN_INDEX = 0;
constexpr const int MEANING_COLUMN_INDEX = 1;
constexpr const char* CREATE_VOCABULARY_TABLE =
    "CREATE TABLE Vocabulary (Word	TEXT, Meaning TEXT);";

std::string load_file_in_string(const boost::filesystem::path& file_path);
std::string sql_insert_vocabulary(const char* word, const char* meaning);
void check_file(const boost::filesystem::path& file_path);
void check_database(const boost::filesystem::path& database_path);
}
