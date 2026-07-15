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


class Tokenizer {
private:
   std::map<std::string, int> KeywordToToken;

public:
    Tokenizer(std::map<std::string, int>keyWordToken)
    {
        KeywordToToken = keyWordToken;
    }
    
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

