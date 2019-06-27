#pragma once

#include <iostream>

#include <boost/filesystem.hpp>

#include <sqlite3.h>

namespace t2a
{
constexpr const int WORD_COLUMN_INDEX = 0;
constexpr const int MEANING_COLUMN_INDEX = 1;
constexpr const char* CREATE_VOCABULARY_TABLE =
    "CREATE TABLE Vocabulary (Word TEXT, Meaning TEXT);";

/// Sql utilies
std::string sql_insert_vocabulary(const char* word, const char* meaning);
void check_database(const boost::filesystem::path& database_path);
void check_output_database(const boost::filesystem::path& output_db_path);
}
