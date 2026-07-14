
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
#include <filesystem>

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

// Mapping array: Index = ASCII value, Value = C64 PETSCII (Uppercase/Graphics Mode)
const unsigned char ascii_to_petscii[256] = {
    // 0x00 - 0x0F (Control Codes)
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    // 0x10 - 0x1F
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    // 0x20 - 0x2F (Space, Punctuation)
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    // 0x30 - 0x3F (Numbers 0-9, Basic Symbols)
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    // 0x40 - 0x4F (@, Uppercase A-O)
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    // 0x50 - 0x5F (Uppercase P-Z, Brackets, and C64 Arrow Equivalents)
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    // 0x60 - 0x6F (Grave Accent, Lowercase a-o mapped to Uppercase)
    0x27, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    // 0x70 - 0x7F (Lowercase p-z mapped to Uppercase, Curly braces/Tilde fallback)
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x20,

    // 0x80 - 0xFF: Extended ASCII / High codes padding out to 255. 
    // Usually unused in clean basic tokenizers, maps to standard matching blocks.
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

static struct LineOutput TokenizeLine(const int current_address, const std::string& str)
{
	const std::string quote = "\"";
	size_t pos = 0;
	LineOutput output;
	bool inQuote = false;
	bool inRem = false;
	unsigned char tok;

	// get line line lineNumber
	std::string linenum;
	while (pos < str.length() && std::isdigit(str[pos])) {
		linenum += str[pos];
		pos++;
	}

	output.lineNumber = std::stoi(linenum);

	while (std::isspace(static_cast<unsigned char>(str[pos])))
		pos++;

	while (pos < str.length()) {
	    auto pos_start =pos;
		auto match_result = match_longest_token(str, pos);

		auto ch = str[pos];
		auto pet = ascii_to_petscii[ch];

		if (!inRem && (!inQuote || (inQuote && str[pos] == '{'))) {
			tok = (match_result.token_id >0 )
			      ? match_result.token_id
			      : pet;
		}
		else {
			tok = pet;
			match_result.length = 1;
		}

		pos += match_result.length;
		output.bytes.push_back(tok);

		if (tok == 143) { // rem statement dont tokenize rest of line
			inRem = true;
		}
		else if (tok == '"') {
			inQuote = !inQuote;
		}
	}
	output.bytes.push_back(0);  // end of line marker
	output.next = static_cast<uint16_t>(current_address + output.bytes.size() + 4);
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
    d64 disk;
    disk.rename_disk("C64PROGREF");
    auto progno = 1;
	for (auto& prog: progs) {

		auto pr=prog.prog;
		std::stringstream ss;

		auto i = 0;
		while (i < pr.length())  {
			ss << pr[i];
			i++;
		}
		ss << "\n";

		auto str = ss.str();
		auto out = TokenizeString(str);

		std::string output_path = std::to_string(progno++) // + "[" +
		                           // std::to_string(prog.chapter) + "]["+
		                           // std::to_string(prog.page) + "][" +
		                           // std::to_string(prog.prog_number) + "]"
								   ;
		                          

        auto result = disk.addFile(output_path, c64FileType(d64FileTypes::PRG), out);
        if (result) {
            std::cout << "added " << output_path << "\n";
        }


	}


    std::string file = "C64ProgramRef.D64";
    bool result = std::filesystem::remove(file);
   disk.save(file);
    
    return 0;
}
