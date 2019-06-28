#pragma once

#include <iostream>

#include <boost/filesystem.hpp>

#include <sqlite3.h>

#include "t2a_sqlite.h"
#include "t2a_utils.h"

namespace t2a
{
constexpr const char* TEMPLATE_DIR_NAME = "templates";
constexpr const char* ANKI_SQL_TEMPLATE_FILENAME =
    "anki2_default_sql_template.sql";

void create_empty_anki_media_file(
    const boost::filesystem::path& anki_media_file_path);
void create_default_anki2_file(const boost::filesystem::path& anki2_path);
void add_anki_collection(
    const boost::filesystem::path& vocabulary_db_path,
    const boost::filesystem::path& anki2_path,
    const char* collection_name);
std::string sql_insert_anki_collection(
    const int id,
    const char* collection_name);
std::string sql_insert_vocabulary(
    const boost::filesystem::path& vocabulary_db_path);

void create_apkg(
    const boost::filesystem::path& anki2_path,
    const boost::filesystem::path& media_path,
    const char* apkg_name);
void check_output_anki_apkg(const boost::filesystem::path& anki_apkg_path);
}
