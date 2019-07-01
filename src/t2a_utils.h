#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include <sqlite3.h>

#include <poppler/cpp/poppler-document.h>

namespace t2a
{
void check_file(const boost::filesystem::path& file_path);
void check_pdf(const boost::filesystem::path& pdf_path);
std::string load_file_in_string(const boost::filesystem::path& file_path);
void run_process(const char* command);
long seconds();
long millis();
long micros();
}
