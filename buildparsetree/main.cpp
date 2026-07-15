
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
#include <charconv>

#include "Tokenizer.h"
#include "CsvReader.h"

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::clog
			<< "This builds a Trie state machine parser from a CSV file of keywords and tokens.\n"
			<< "Usage: " << argv[0] << " <inputfile.csv> <outputfile.h>\n"
			<< "\n\nThe user can then call:\n"
			<< "static MatchResult match_longest_token(const std::string & text, size_t start_pos)\n"
			;
		return 1;
	}

    std::map<std::string, int> keywordToToken;
    std::string csvfile = argv[1];
	std::string outfile = argv[2];
	try {
		std::vector<std::pair<int, std::string>> toks;
	    
		auto csvToks = csvReader::ReadCSV(csvfile);
        for (auto& tok: csvToks) {
            keywordToToken[tok.first] = tok.second;
 			toks.emplace_back(tok.second, tok.first);
		}

		std::shared_ptr<Tokenizer::ParseNode> root = std::make_shared<Tokenizer::ParseNode>(0);
		Tokenizer tokenizer(keywordToToken);
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
