#pragma once

// written by Paul Baxter
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <chrono>
#include <fstream>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <stdexcept>
#include <charconv>

class csvReader {
private:

	static std::string csvUnescapeField(std::string_view rawField) {
		// If quoted, remove outer quotes and unescape "" -> "
		if (rawField.size() >= 2 && rawField.front() == '"' && rawField.back() == '"') {
			std::string out;
			out.reserve(rawField.size() - 2);

			for (size_t i = 1; i + 1 < rawField.size(); ++i) {
				if (rawField[i] == '"' && rawField[i + 1] == '"') {
					out.push_back('"');
					++i; // consume second quote
				} else {
					out.push_back(rawField[i]);
				}
			}
			return out;
		}

		// Unquoted: use as-is
		return std::string(rawField);
	}

	static std::vector<std::string> csvSplitLine(std::string_view line) {
		std::vector<std::string> fields;
		fields.reserve(2);

		std::string current;
		bool inQuotes = false;

		for (size_t i = 0; i < line.size(); ++i) {
			char c = line[i];

			if (c == '"') {
				if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
					// Escaped quote inside a quoted field: ""
					current.push_back('"');
					++i; // skip second quote
				} else {
					// Toggle quoted state
					inQuotes = !inQuotes;
				}
			} else if (c == ',' && !inQuotes) {
				fields.push_back(std::move(current));
				current.clear();
			} else {
				current.push_back(c);
			}
		}

		fields.push_back(std::move(current));

		if (fields.size() != 2) {
			throw std::invalid_argument("Expected exactly 2 CSV fields");
		}
		return fields;
	}

	static int parseIntStrict(std::string_view s) {
		// Trim ASCII whitespace
		auto is_space = [](unsigned char ch) {
			return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
		};
		while (!s.empty() && is_space(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
		while (!s.empty() && is_space(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);

		int value{};
		auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, 10);
		if (ec != std::errc{} || ptr != s.data() + s.size()) {
			throw std::invalid_argument("Invalid int field");
		}
		return value;
	}

	static std::string trim(std::string_view s)
	{
		// Trim ASCII whitespace
		auto is_space = [](unsigned char ch) {
			return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
		};
		while (!s.empty() && is_space(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
		while (!s.empty() && is_space(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
		return std::string(s);
	}

public:
	static std::vector<std::pair<std::string,int>> ReadCSV(std::string filename)
	{
	    std::vector<std::pair<std::string, int>> out;
		std::ifstream file(filename);
		if (!file) {
			throw std::runtime_error("Failed to open " + filename);
		}

		std::string header;
		std::getline(file, header); // consume header line
		std::string line;

		while (std::getline(file, line)) {
			line = trim(line);
			if (line.empty()) continue;

			// Use the new parser
			auto cols = csvSplitLine(line);
			if (cols.size() < 2) continue;

			std::string keyword = trim(csvUnescapeField(cols[0]));
			std::string token_str = trim(csvUnescapeField(cols[1]));

			// Trim whitespace (but don't strip quotes anymore!)
			keyword.erase(0, keyword.find_first_not_of(" \t\r\n"));
			keyword.erase(keyword.find_last_not_of(" \t\r\n") + 1);

			token_str.erase(0, token_str.find_first_not_of(" \t\r\n"));
			token_str.erase(token_str.find_last_not_of(" \t\r\n") + 1);

			if (keyword.empty() || token_str.empty()) continue;

			int token = parseIntStrict(token_str);
			out.push_back({keyword, token});
		}
		file.close();
		return out;
	}
};
