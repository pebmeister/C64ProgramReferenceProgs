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

static std::map<std::string, int> KeywordToToken;

class Tokenizer {
public:
    struct ParseNode {
        char ch;
        int token;
        int state_id; // Used to map this node to a 2D array index
        std::vector<std::shared_ptr<ParseNode>> child;

        ParseNode(char ch) : ch(ch), token(0), state_id(0) {}
        ParseNode(char ch, int token) : ch(ch), token(token), state_id(0) {}
    };

    void generateInitializerList(std::shared_ptr<ParseNode>& root, std::string& filename)
    {
        std::ofstream out_file(filename);
        if (!out_file) {
            throw std::runtime_error("Failed to create output file " + filename );
        }

        // 1. Flatten the tree to assign state IDs
        std::vector<ParseNode*> states;
        states.push_back(nullptr); // State 0 is the "dead end" / invalid state

        // Assign IDs using a lambda for simple DFS traversal
        std::function<void(std::shared_ptr<ParseNode>)> assign_states = [&](std::shared_ptr<ParseNode> n) {
            if (!n) return;
            n->state_id = static_cast<int>(states.size());
            states.push_back(n.get());
            for (auto& child : n->child) {
                assign_states(child);
            }
        };
        assign_states(root); // Root becomes state 1

        size_t num_states = states.size();

        // 2. Output the header and includes
        out_file << "#pragma once\n"
                 << "// Generated " << filename << "\n"
                 << "// DO NOT EDIT - Generated via BuildTokTree\n\n"
                 << "#include <cstdint>\n"
                 << "#include <cstddef>\n"
                 << "#include <string>\n"
                 << "#include <unordered_map>\n\n";

        // 3. Replicate your Keyword maps
        std::vector<std::string> keys;
        for (auto& kt : KeywordToToken) keys.push_back(kt.first);
        std::sort(keys.begin(), keys.end());

        out_file << "static const std::unordered_map<std::string, int> KeywordToToken = {\n";
        for (auto& k : keys) {
            out_file << "    {\"" << k << "\", " << KeywordToToken.at(k) << "},\n";
        }
        out_file << "};\n\n";

        out_file << "static const std::unordered_map<int, std::string> TokenToKeyword = [] {\n"
                 << "    std::unordered_map<int, std::string> rev;\n"
                 << "    for (auto& p : KeywordToToken) rev[p.second] = p.first;\n"
                 << "    return rev;\n"
                 << "}();\n\n";

        // 4. Output structures
        out_file << "struct MatchResult {\n"
                 << "    int token_id;\n"
                 << "    size_t length;\n"
                 << "};\n\n";

        // 5. Output the 2D Transition Table
        // Using int16_t saves binary space (allows up to 32,767 states)
        out_file << "static const int16_t transition_table[" << num_states << "][256] = {\n";
        
        for (size_t i = 0; i < num_states; ++i) {
            out_file << "    { ";
            if (i == 0) {
                // State 0: All transitions point to 0
                for (int c = 0; c < 256; ++c) out_file << "0,";
            } else {
                ParseNode* n = states[i];
                // Map the children by character index
                int next_state[256] = {0};
                for (auto& child : n->child) {
                    unsigned char c = static_cast<unsigned char>(child->ch);
                    next_state[c] = child->state_id;
                }
                // Print the row
                for (int c = 0; c < 256; ++c) {
                    out_file <<  next_state[c] << ",";
                }
            }
            out_file << " },\n";
        }
        out_file << "};\n\n";

        // 6. Output the Token Array (maps State ID to Token ID)
        out_file << "static const int token_for_state[" << num_states << "] = {\n    0, ";
        for (size_t i = 1; i < num_states; ++i) {
            out_file << states[i]->token << ", ";
            if (i % 20 == 0) out_file << "\n    "; // formatting
        }
        out_file << "\n};\n\n";

        // 7. Output the new, highly optimized Match Function
        out_file << "// Finds the longest matching prefix using the state machine\n"
                 << "static MatchResult match_longest_token(const std::string& text, size_t start_pos)\n"
                 << "{\n"
                 << "    if (start_pos >= text.length()) {\n"
                 << "        return { 0, 0 };\n"
                 << "    }\n\n"
                 << "    int current_state = 1; // 1 is the Root State\n"
                 << "    size_t current_pos = start_pos;\n"
                 << "    int last_valid_token = 0;\n"
                 << "    size_t last_valid_length = 0;\n\n"
                 << "    while (current_pos < text.length()) {\n"
                 << "        // Use unsigned char to prevent negative array indexing\n"
                 << "        unsigned char next_char = static_cast<unsigned char>(text[current_pos]);\n\n"
                 << "        // O(1) table lookup\n"
                 << "        current_state = transition_table[current_state][next_char];\n\n"
                 << "        // State 0 means dead end - no further match is possible\n"
                 << "        if (current_state == 0) {\n"
                 << "            break;\n"
                 << "        }\n\n"
                 << "        current_pos++;\n\n"
                 << "        // Check if the new state completes a valid token\n"
                 << "        int token = token_for_state[current_state];\n"
                 << "        if (token != 0) {\n"
                 << "            last_valid_token = token;\n"
                 << "            last_valid_length = current_pos - start_pos;\n"
                 << "        }\n"
                 << "    }\n\n"
                 << "    if (last_valid_token != 0) {\n"
                 << "        return { last_valid_token, last_valid_length };\n"
                 << "    }\n\n"
                 << "    return { 0, 1 }; // Fallback\n"
                 << "}\n";

        out_file.close();
    }

private:
    void insertToken(std::shared_ptr<ParseNode> root, int token, const std::string& word)
    {
        auto curnode = root;
        for (auto& ch : word)
        {
            auto it = std::lower_bound(curnode->child.begin(), curnode->child.end(), ch,
            [](const std::shared_ptr<ParseNode>& node, char c) {
                return node->ch < c;
            });

            if (it != curnode->child.end() && (*it)->ch == ch) {
                curnode = *it;
            }
            else {
                auto newNode = std::make_shared<ParseNode>(ch);
                curnode->child.insert(it, newNode);
                curnode = newNode;
            }
        }

        curnode->token = token;
    }

public:
    void buildtoktree(std::shared_ptr<ParseNode>& root, std::vector<std::pair<int, std::string>>& toks)
    {
        std::sort(toks.begin(), toks.end(), [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

        for (const auto& [tok, str] : toks) {
            insertToken(root, tok, str);
        }
    }
};


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

std::string trim(std::string s)
{
    // Trim ASCII whitespace
    auto is_space = [](unsigned char ch) {
        return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
    };
    while (!s.empty() && is_space(static_cast<unsigned char>(s.front()))) s.remove_prefix(1);
    while (!s.empty() && is_space(static_cast<unsigned char>(s.back()))) s.remove_suffix(1);
    return s;
}

static void ReadCSV(std::string filename)
{
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
		auto cols = cvsSplitLine(line);
		if (cols.size() < 2) continue;

		std::string keyword = trim(csvUnescapeField(cols[0]));
		std::string token_str = trim(cvsUnescapeFie;d(cols[1]));

		// Trim whitespace (but don't strip quotes anymore!)
		keyword.erase(0, keyword.find_first_not_of(" \t\r\n"));
		keyword.erase(keyword.find_last_not_of(" \t\r\n") + 1);

		token_str.erase(0, token_str.find_first_not_of(" \t\r\n"));
		token_str.erase(token_str.find_last_not_of(" \t\r\n") + 1);

		if (keyword.empty() || token_str.empty()) continue;

		int token = parseIntStrict(token_str);
		KeywordToToken[keyword] = token;
	}
	file.close();
}
]
int main(int argc, char* argv[])
{
    if (argc != 3) {
		std::clog << "This builds a Trie state machine parser from a CSV file of keywords and tokens.\n\n"                    
                << "\n\nThe user can then call:\n"
                << "static MatchResult match_longest_token(const std::string & text, size_t start_pos)\n"
                << "Usage: " << argv[0] << " <inputfile.csv> <outputfile.h>\n";
    }
 
    std::string csvfile = argv[1];
    std::string outfile = argv[2];
    try {
    	ReadCSV(csvfile);

    	std::vector<std::pair<int, std::string>> toks;
		for (const auto& [keyword, token] : KeywordToToken) {
			toks.emplace_back(token, keyword);
		}

		std::shared_ptr<Tokenizer::ParseNode> root = std::make_shared<Tokenizer::ParseNode>(0);

		Tokenizer tokenizer;
		tokenizer.buildtoktree(root, toks);
		tokenizer.generateInitializerList(root, outfile);

		std::cout << outfile << " created!\n";
	}
	catch (std::exception ex) {
		std::clog << ex.what();
	}
	catch (...) {
		std::clog << "Unknown error";
	}
	return 0;
}
