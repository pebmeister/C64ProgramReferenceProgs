
#pragma once
// written by Paul Baxter
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <memory>

class Tokenizer {
private:

	bool ignoreCase = false;
	struct ParseNode {
		char ch;
		int token;
		int state_id; // Used to map this node to a 2D array index
		std::vector<std::shared_ptr<ParseNode>> child;

		ParseNode(char ch) : ch(ch), token(0), state_id(0) {}
		ParseNode(char ch, int token) : ch(ch), token(token), state_id(0) {}
	};

	std::shared_ptr<Tokenizer::ParseNode> root = std::make_shared<Tokenizer::ParseNode>(0);

	void generateTokemizer(std::string& filename)
	{
		std::ofstream out_file(filename);
		if (!out_file) {
			throw std::runtime_error("Failed to create output file " + filename);
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

		// Determine the smallest possible data type for the transition table
		auto num_states = states.size();
		std::string table_type;
		if (num_states < static_cast<size_t>(0xFF)) {
			table_type = "uint8_t";
		} else if (num_states < static_cast<size_t>(0xFFFF)) {
			table_type = "uint16_t";
		} else if (num_states < static_cast<size_t>(0xFFFFFFFF)) {
			table_type = "uint32_t";
		}	else { // what the heck are we parsing!!!!!
			table_type = "uint64_t";
		}

		// ==============================================================
		// 2. EQUIVALENCE CLASS COMPRESSION
		// ==============================================================

		// a. Build virtual transition table [num_states][256]
		std::vector<std::vector<int>> virtual_table(num_states, std::vector<int>(256, 0));
		for (size_t i = 1; i < num_states; ++i) {
			for (const auto& child : states[i]->child) {
				unsigned char c = static_cast<unsigned char>(child->ch);
				virtual_table[i][c] = child->state_id;
			}
		}

		// b. Extract character signatures (columns of the virtual table)
		std::vector<std::vector<int>> char_signatures(256, std::vector<int>(num_states, 0));
		for (int c = 0; c < 256; ++c) {
			for (size_t s = 0; s < num_states; ++s) {
				char_signatures[c][s] = virtual_table[s][c];
			}
		}

		// c. Group identical signatures into classes
		std::map<std::vector<int>, int> signature_to_class;
		std::vector<int> char_class(256);
		int num_classes = 0;

		for (int c = 0; c < 256; ++c) {
			const auto& sig = char_signatures[c];
			if (signature_to_class.find(sig) == signature_to_class.end()) {
				signature_to_class[sig] = num_classes++;
			}
			char_class[c] = signature_to_class[sig];
			if (ignoreCase) {
					char_class[std::toupper(c)] = signature_to_class[sig];
			}
			
		}

		// d. Build the compressed table [num_states][num_classes]
		std::vector<std::vector<int>> compressed_table(num_states, std::vector<int>(num_classes, 0));
		for (int c = 0; c < 256; ++c) {
			int cls = char_class[c];
			for (size_t s = 0; s < num_states; ++s) {
				compressed_table[s][cls] = char_signatures[c][s];
			}
		}

		// ==============================================================
		// 3. GENERATE C++ OUTPUT
		// ==============================================================

		out_file << "#pragma once\n"
		         << "// Generated " << filename << "\n"
		         << "// DO NOT EDIT - Generated via BuildTokTree\n\n"
		         << "#include <cstdint>\n"
		         << "#include <cstddef>\n"
		         << "#include <string>\n\n";

		out_file << "struct MatchResult {\n"
		         << "    int token_id;\n"
		         << "    size_t length;\n"
		         << "};\n\n";

		// Output the Equivalence Class Mapping Array
		out_file << "static const uint8_t char_class[256] = {\n    ";
		for (int c = 0; c < 256; ++c) {
			out_file << char_class[c] << ", ";
			if ((c + 1) % 16 == 0 && c != 255) out_file << "\n    ";
		}
		out_file << "\n};\n\n";

		// Output the Compressed Transition Table
		out_file << "static const " << table_type << " transition_table[" << num_states << "][" << num_classes << "] = {\n";
		for (size_t s = 0; s < num_states; ++s) {
			out_file << "    { ";
			for (int cls = 0; cls < num_classes; ++cls) {
				out_file << compressed_table[s][cls] << ",";
			}
			out_file << " },\n";
		}
		out_file << "};\n\n";

		// Output the Token Array (maps State ID to Token ID)
		out_file << "static const int token_for_state[" << num_states << "] = {\n    0, ";
		for (size_t i = 1; i < num_states; ++i) {
			out_file << states[i]->token << ", ";
			if (i % 20 == 0) out_file << "\n    "; // formatting
		}
		out_file << "\n};\n\n";

		// Output the optimized Match Function
		out_file << "// Finds the longest matching prefix using the compressed state machine\n"
		         << "static MatchResult match_longest_token(const std::string& text, size_t start_pos)\n"
		         << "{\n"
		         << "    if (start_pos >= text.length()) {\n"
		         << "        return { 0, 0 };\n"
		         << "    }\n\n"
		         << "    " << table_type << " current_state = 1; // 1 is the Root State\n"
		         << "    size_t current_pos = start_pos;\n"
		         << "    auto last_valid_token = 0;\n"
		         << "    size_t last_valid_length = 0;\n\n"
		         << "    while (current_pos < text.length()) {\n"
		         << "        unsigned char next_char = static_cast<unsigned char>(text[current_pos]);\n"
		         << "        uint8_t cls = char_class[next_char];\n\n"
		         << "        // O(1) table lookup using equivalence class\n"
		         << "        current_state = transition_table[current_state][cls];\n\n"
		         << "        // State 0 means dead end - no further match is possible\n"
		         << "        if (current_state == 0) {\n"
		         << "            break;\n"
		         << "        }\n\n"
		         << "        current_pos++;\n\n"
		         << "        // Check if the new state completes a valid token\n"
		         << "        auto token = token_for_state[current_state];\n"
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

	void buildtoktree(std::vector<std::pair<int, std::string>>& toks)
	{
		std::sort(toks.begin(), toks.end(), [](const auto& a, const auto& b) {
			return a.second < b.second;
		});

		for (const auto& [tok, str] : toks) {

			insertToken(root, tok, str);
		}
	}

public:

	Tokenizer(std::vector<std::pair<int, std::string>> toks, std::string outfile, bool ignorecase)
	{
		ignoreCase = ignorecase;
		for (const auto& [tok, s] : toks) {
			auto str = s;
			if (ignoreCase) {
				std::transform(str.begin(), str.end(), str.begin(),
				[](unsigned char c) {
					return std::tolower(c);
				});
			}
			insertToken(root, tok, str);
		}

		buildtoktree(toks);
		generateTokenizer(outfile);
	}

};
