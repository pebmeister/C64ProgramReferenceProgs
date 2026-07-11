/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby,
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "d64.h"
#include "d64_types.h"
#include "ParseTree.h"
#include "progs.h"


struct LineOutput
{
	uint16_t next;
	uint16_t lineNumber;
	std::vector<uint8_t> bytes;
};

static struct LineOutput TokenizeLine(const int current_address, const std::string& str)
{
	const std::string quote = "\"";
	size_t pos = 0;
	LineOutput output;

	// get line line lineNumber
	std::string linenum;
	while (pos < str.length() && std::isdigit(str[pos])) {
		linenum += str[pos];
		pos++;
	}

	output.lineNumber = std::stoi(linenum);

	while (pos < str.length()) {
		auto result = match_longest_token(root, str, pos);
		std::string_view tok{ str.data() + pos, result.length };
		if (result.token_id != 0) {
			output.bytes.push_back(result.token_id);
			if (result.token_id == 143) { // rem statement dont tokenize rest of line
				pos += result.length;
				while (pos < str.length() && str[pos] != '\n') {
					output.bytes.push_back(str[pos]);
					pos++;
				}
				// output.bytes.push_back(0);
				continue;
			}
		}
		else {
			output.bytes.push_back(tok[0]);
			if (tok == quote) {
				do
				{
					pos++;
					output.bytes.push_back(str[pos]);
				} while (pos < str.length() && str[pos] != quote[0]);
			}
		}
		pos += result.length;
	}
	output.bytes.push_back(0);  // end of line marker
	output.next = current_address + output.bytes.size() + 4;
	return output;
}

std::vector<uint8_t> TokenizeString(std::string& str)
{
	std::vector<uint8_t> output;
	std::stringstream ss(str);

	std::string line;
	auto current_address = 0x0801;

	output.push_back(current_address & 0xFF);
	output.push_back((current_address >> 8) & 0xFF);

	// Read line by line until the end of the file

	while (std::getline(ss, line)) {
		if (line.empty())
			continue;

		auto tokline = TokenizeLine(current_address, line);
		output.push_back(tokline.next & 0xFF);
		output.push_back((tokline.next >> 8) & 0xFF);
		output.push_back(tokline.lineNumber & 0xFF);
		output.push_back((tokline.lineNumber >> 8) & 0xFF);

		output.insert(output.end(), tokline.bytes.begin(), tokline.bytes.end());
		current_address = tokline.next;
	}
	
	// mark end of file
	output.push_back(0);
	output.push_back(0);

	// File stream closes automatically when it goes out of scope
	return output;
}

int main()
{
    d64 disk(thirty_five_track);
    disk.rename_disk("c64progref");

	for (auto& prog: progs) {

		auto pr=prog.prog;
		std::stringstream ss;

		auto i = 0;
		while (i < pr.length())  {
			if (i>0 && pr[i-1] == '\"' && pr[i] == '[') {
				i++;
				auto ch = 0;

				while (i+1 < pr.length()) {
					if (pr[i] == ']' && pr[i+1]=='\"') {
						ss << static_cast<char>(ch);
						break;
					}
					if (std::isdigit(pr[i])) {
						ch *=10;
						ch += pr[i]-'0';
					}
					else {
						ss << "[" << pr[i];
						break;
					}
					i++;
				}
			}
			else {
				ss << pr[i];
			}
			i++;
		}
		ss << "\n";

		auto str = ss.str();
		auto out = TokenizeString(str);

		std::string output_path = "CH" + std::to_string(prog.chapter) + "-" +
		                          "PG" + std::to_string(prog.page) + "-" +
		                          "PROGRAM" + std::to_string(prog.prog_number);

        auto result = disk.addFile(output_path, c64FileType(d64FileTypes::PRG), out);
        if (result) {
            std::cout << "added " << output_path << "\n";
        }


	}
	
   // disk.save("C64ProgramRef.D64");
    
    return 0;
}