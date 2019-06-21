#pragma once

#include <string>

#include <sqlite3.h>

#include <boost/filesystem.hpp>

namespace t2a
{
boost::filesystem::path generate_sub_database_from_string(
        const boost::filesystem::path& reference_database,
        const std::string& str);
}
