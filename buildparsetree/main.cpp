
// written by Paul Baxter

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

	// std::map<std::string, int> keywordToToken;
	std::string csvfile = argv[1];
	std::string outfile = argv[2];

	std::vector<std::pair<int, std::string>> toks;

	try {

		auto csvToks = csvReader::ReadCSV(csvfile);
		for (auto& tok: csvToks) {
			toks.push_back({tok.second, tok.first});
		}

		Tokenizer tokenizer(toks, outfile);

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