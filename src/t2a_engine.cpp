#include "t2a_engine.h"

namespace t2a
{
boost::filesystem::path generate_sub_database_from_string(
        const boost::filesystem::path& reference_database,
        const std::string& str)
{
    sqlite3* ref_db = nullptr;
    sqlite3_open(reference_database.string().c_str(), &ref_db);
    return boost::filesystem::path();
}
}
