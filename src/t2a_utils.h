#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include <sqlite3.h>

namespace t2a
{
/// Constant values
constexpr const int WORD_COLUMN_INDEX = 0;
constexpr const int MEANING_COLUMN_INDEX = 1;
constexpr const char* CREATE_VOCABULARY_TABLE =
    "CREATE TABLE Vocabulary (Word TEXT, Meaning TEXT);";

/// Sql utilies
std::string sql_insert_vocabulary(const char* word, const char* meaning);
void check_database(const boost::filesystem::path& database_path);
void check_output_database(const boost::filesystem::path& output_db_path);

/// Anki utilities
void create_empty_anki_media_file(
    const boost::filesystem::path& anki_media_file_path);
void create_default_anki2_file(const boost::filesystem::path& anki2_path);
void check_output_anki_apkg(const boost::filesystem::path& anki_apkg_path);

/// File utilities
void check_file(const boost::filesystem::path& file_path);
std::string load_file_in_string(const boost::filesystem::path& file_path);
}
