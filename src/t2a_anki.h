#pragma once

#include <iostream>

#include <boost/filesystem.hpp>

#include <sqlite3.h>

#include "t2a_utils.h"

namespace t2a
{
constexpr const char* TEMPLATE_DIR_NAME = "templates";
constexpr const char* ANKI_SQL_TEMPLATE_FILENAME =
        "anki2_default_sql_template.txt";

void create_empty_anki_media_file(
    const boost::filesystem::path& anki_media_file_path);
void create_default_anki2_file(const boost::filesystem::path& anki2_path);
void check_output_anki_apkg(const boost::filesystem::path& anki_apkg_path);
}
