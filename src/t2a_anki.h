#pragma once

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/sha1.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <sqlite3.h>

#include "t2a_sqlite.h"
#include "t2a_utils.h"

namespace t2a
{
constexpr const char* TEMPLATE_DIR_NAME = "templates";
constexpr const char* ANKI_SQL_TEMPLATE_FILENAME =
    "anki2_default_sql_template.sql";
constexpr const unsigned char* NOTES_SHA1_TEXT =
    reinterpret_cast<const unsigned char*>("sfld");
constexpr const int NOTES_SHA1_DIGITS = 8;
constexpr const char* DEFAULT_CONF_FILENAME = "default_conf.json";
constexpr const char* DEFAULT_MODELS_FILENAME = "default_models.json";
constexpr const char* DEFAULT_DECKS_FILENAME = "default_decks.json";
constexpr const char* DEFAULT_JSON_KEY = "1398130078204";
constexpr const char* DEFAULT_DCONF_FILENAME = "default_dconf.json";

void create_empty_anki_media_file(
    const boost::filesystem::path& anki_media_file_path);
void create_default_anki2_file(const boost::filesystem::path& anki2_path);
void add_anki_collection(
    const boost::filesystem::path& vocabulary_db_path,
    const boost::filesystem::path& anki2_path,
    const char* collection_name);
std::string sql_insert_anki_collection(
    const int id,
    const char* collection_name,
    std::string* deck_id);
std::string sql_insert_vocabulary(
    const boost::filesystem::path& vocabulary_db_path,
    const std::string& deck_id);

void create_apkg(
    const boost::filesystem::path& anki2_path,
    const boost::filesystem::path& media_path,
    const char* apkg_name);
void check_output_anki_apkg(const boost::filesystem::path& anki_apkg_path);
}
