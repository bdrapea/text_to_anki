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
constexpr const char* GOI_PDF_FILENAME = "goi.pdf";
constexpr const int KANA_UNICODE_LOWER_BOUND = 0xe38180;
constexpr const int KANA_UNICODE_UPPER_BOUND = 0xe383bf;
constexpr const int KANJI_UNICODE_LOWER_BOUND = 0xe4b880;
constexpr const int KANJI_UNICODE_UPPER_BOUND = 0xe9bfbf;

void check_file(const boost::filesystem::path& file_path);
std::string load_file_in_string(const boost::filesystem::path& file_path);
void run_process(const char* command);
long seconds();
long millis();
long micros();
bool is_kanji(const char* character);
bool is_kana(const char* character);
bool is_jlpt_level(const char* character);
}
