#pragma once

#include <cstring>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include <sqlite3.h>

#include "t2a_anki.h"
#include "t2a_sqlite.h"
#include "t2a_utils.h"

namespace t2a
{
void generate_sub_database_from_string(
    const boost::filesystem::path& reference_database_path,
    const std::string& str,
    const boost::filesystem::path& output_database_path);

void generate_anki_apkg_from_database(
    const boost::filesystem::path& database_path,
    const boost::filesystem::path& anki_apkg_path);
}
