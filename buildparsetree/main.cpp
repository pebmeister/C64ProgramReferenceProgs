
// written by Paul Baxter
#include <iostream>
#include <string>
#include <vector>

#include "Tokenizer.h"
#include "CsvReader.h"

struct Options
{
    std::string csvTokenFile;
    std::string outputHeaderFile;
    bool ignoreCase = false;
};

bool parseArgs(int argc, char* argv[], Options& opt)
{
    std::vector<std::string> files;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-i" || arg == "--ignore-case")
        {
            opt.ignoreCase = true;
        }
        else if (!arg.empty() && arg[0] == '-')
        {
            std::cerr << "Unknown option: " << arg << '\\n';
            return false;
        }
        else
        {
            files.push_back(arg);
        }
    }

    if (files.size() != 2)
    {
        std::cerr << "Usage: tool [-i] <csv token file> <output header file>\\n";
        return false;
    }

    opt.csvTokenFile = files[0];
    opt.outputHeaderFile = files[1];

    return true;
}

int main(int argc, char* argv[])
{
    Options opt;

    if (!parseArgs(argc, argv, opt))
        return 1;

    std::cout << "CSV token file : " << opt.csvTokenFile << '\\n';
    std::cout << "Output header  : " << opt.outputHeaderFile << '\\n';
    std::cout << "Ignore case    : " << std::boolalpha << opt.ignoreCase << '\\n';

	   // std::map<std::string, int> keywordToToken;
	   auto& csvfile = opt.cvsTokenFile;
	   auto& outfile = opt.outputHeaderFile;
				auto ignoreCase = opt.ignoreCase;

	   std::vector<std::pair<int, std::string>> toks;

	   try {

		      auto csvToks = csvReader::ReadCSV(csvfile);
		      for (auto& tok: csvToks) {
			         toks.push_back({tok.second, tok.first});
		      }

		      Tokenizer tokenizer(toks, outfile, ignoreCase);

		      std::cout << outfile << " created!\n";
	   }
	   catch (std::exception& ex) {
		      std::clog << ex.what();
        return 1;
	   }
	   catch (...) {
		      std::clog << "Unknown error";
        return 0;
	   }
	   return 0;
}
