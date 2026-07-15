#pragma once
// written by Paul Baxter (Optimized)
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <charconv>
#include <algorithm>

class csvReader {
private:
    // Zero-allocation trim: simply shrinks the string_view window!
    static std::string_view trim(std::string_view s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string_view::npos) return {};
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    // In-place unescape: Returns either a slice of the original buffer,
    // or a newly allocated string ONLY if we actually have escaped double-quotes to resolve.
    static std::string csvUnescapeField(std::string_view rawField, std::string& scratchBuffer) {
        rawField = trim(rawField);

        if (rawField.size() >= 2 && rawField.front() == '"' && rawField.back() == '"') {
            std::string_view inner = rawField.substr(1, rawField.size() - 2);

            // Check if we actually need to unescape any double-quotes.
            // If not, we can avoid allocating a new string entirely!
            if (inner.find("\"\"") == std::string_view::npos) {
                return std::string(inner);
            }

            // Otherwise, clean up the escaped quotes using a reused scratch buffer
            scratchBuffer.clear();
            scratchBuffer.reserve(inner.size());
            for (size_t i = 0; i < inner.size(); ++i) {
                if (inner[i] == '"' && i + 1 < inner.size() && inner[i + 1] == '"') {
                    scratchBuffer.push_back('"');
                    ++i; // skip second quote
                }
                else {
                    scratchBuffer.push_back(inner[i]);
                }
            }
            return scratchBuffer;
        }

        return std::string(rawField);
    }

    static int parseIntStrict(std::string_view s) {
        s = trim(s);
        int value{};
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, 10);
        if (ec != std::errc{} || ptr != s.data() + s.size()) {
            throw std::invalid_argument("Invalid int field");
        }
        return value;
    }

public:
    static std::vector<std::pair<std::string, int>> ReadCSV(const std::string& filename) {
        // Read the entire file into memory for ultra-fast, zero-copy buffer scanning
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file) {
            throw std::runtime_error("Failed to open " + filename);
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string buffer;
        buffer.resize(size);
        if (!file.read(&buffer[0], size)) {
            throw std::runtime_error("Failed to read " + filename);
        }
        file.close();

        std::vector<std::pair<std::string, int>> out;
        out.reserve(128); // Pre-allocate a starting size to minimize vector reallocations

        std::string_view content(buffer);

        // Skip Header Line
        size_t pos = content.find_first_of("\r\n");
        if (pos != std::string_view::npos) {
            if (pos + 1 < content.size() && content[pos] == '\r' && content[pos + 1] == '\n') {
                content.remove_prefix(pos + 2);
            }
            else {
                content.remove_prefix(pos + 1);
            }
        }

        std::string scratch; // Reusable buffer for unescaping to avoid constant heap allocations
        size_t i = 0;
        size_t n = content.size();

        while (i < n) {
            std::string_view field1, field2;

            // --- Parse Field 1 ---
            size_t start1 = i;
            bool inQuotes = false;
            while (i < n) {
                char c = content[i];
                if (c == '"') {
                    inQuotes = !inQuotes;
                }
                else if (c == ',' && !inQuotes) {
                    field1 = content.substr(start1, i - start1);
                    ++i; // skip ','
                    break;
                }
                else if ((c == '\n' || c == '\r') && !inQuotes) {
                    throw std::invalid_argument("Expected exactly 2 CSV fields (line ended early)");
                }
                ++i;
            }

            // --- Parse Field 2 ---
            size_t start2 = i;
            inQuotes = false;
            while (i < n) {
                char c = content[i];
                if (c == '"') {
                    inQuotes = !inQuotes;
                }
                else if ((c == '\n' || c == '\r') && !inQuotes) {
                    field2 = content.substr(start2, i - start2);
                    // Handle CRLF newlines
                    if (c == '\r' && i + 1 < n && content[i + 1] == '\n') {
                        ++i;
                    }
                    ++i; // move past newline
                    break;
                }
                ++i;
            }

            // If we reached EOF while reading the last field
            if (i == n && field2.empty() && start2 < n) {
                field2 = content.substr(start2, n - start2);
            }

            if (field1.empty() && field2.empty()) {
                continue; // empty row
            }

            // Process and Unescape
            std::string keyword = csvUnescapeField(field1, scratch);
            std::string token_str = csvUnescapeField(field2, scratch);

            if (keyword.empty() || token_str.empty()) continue;

            int token = parseIntStrict(token_str);
            out.push_back({ std::move(keyword), token });
        }

        return out;
    }
};
