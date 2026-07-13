#pragma once
// Generated ParseTree.h
// DO NOT EDIT

#include <memory>
#include <vector>
#include <unordered_map>

struct ParseNode {
    char ch;
    int token;
    std::vector<std::shared_ptr<ParseNode>> child;

    ParseNode(char ch) : ch(ch), token(0) {}
    ParseNode(char ch, int token) : ch(ch), token(token) {}
    ParseNode(char ch, int token, std::vector<std::shared_ptr<ParseNode>> child) : 
        ch(ch), token(token), child(child) {}
};

// Result structure to return from our matching function
struct MatchResult {
    int token_id;
    size_t length;
};

// Finds the longest matching prefix in the Trie starting at index 'start_pos'
static MatchResult match_longest_token(std::shared_ptr<ParseNode> root, const std::string& text, size_t start_pos)
{
    if (!root || start_pos >= text.length()) {
        return { 0, 0 };
    }

    std::shared_ptr<ParseNode> current = root;
    size_t current_pos = start_pos;

    // Track the best valid token state we've passed so far
    int last_valid_token = 0;
    size_t last_valid_length = 0;

    while (current_pos < text.length()) {
        char next_char = text[current_pos];
        std::shared_ptr<ParseNode> next_node = nullptr;

        // Linear search through the node's children for a character match
        for (const auto& child_node : current->child) {
            if (child_node->ch == next_char) {
                next_node = child_node;
                break;
            }
        }

        // If no matching child edge exists, we can't look further down the tree
        if (!next_node) {
            break;
        }

        // Advance into the matching child node
        current = next_node;
        current_pos++;

        // If this state represents a complete keyword/operator, log it!
        if (current->token != 0) {
            last_valid_token = current->token;
            last_valid_length = current_pos - start_pos;
        }
    }

    // If we found a reserved word/operator, return it
    if (last_valid_token != 0) {
        return { last_valid_token, last_valid_length };
    }

    // Fallback: If no tree path matched a valid token, treat the first char as a literal
    return { 0, 1 };
}

static const std::unordered_map<std::string, int> KeywordToToken
{
    {"*", 172},
    {"+", 170},
    {"-", 171},
    {"/", 173},
    {"<", 179},
    {"=", 178},
    {">", 177},
    {"ABS", 182},
    {"AND", 175},
    {"ASC", 198},
    {"ATN", 193},
    {"CHR$", 199},
    {"CLOSE", 160},
    {"CLR", 156},
    {"CMD", 157},
    {"CONT", 154},
    {"COS", 190},
    {"DATA", 131},
    {"DEF", 150},
    {"DIM", 134},
    {"END", 128},
    {"EXP", 189},
    {"FN", 165},
    {"FOR", 129},
    {"FRE", 184},
    {"GET", 161},
    {"GO", 203},
    {"GOSUB", 141},
    {"GOTO", 137},
    {"IF", 139},
    {"INPUT", 133},
    {"INPUT#", 132},
    {"INT", 181},
    {"LEFT$", 200},
    {"LEN", 195},
    {"LET", 136},
    {"LIST", 155},
    {"LOAD", 147},
    {"LOG", 188},
    {"MID$", 202},
    {"NEW", 162},
    {"NEXT", 130},
    {"NOT", 168},
    {"ON", 145},
    {"OPEN", 159},
    {"OR", 176},
    {"PEEK", 194},
    {"POKE", 151},
    {"POS", 185},
    {"PRINT", 153},
    {"PRINT#", 152},
    {"READ", 135},
    {"REM", 143},
    {"RESTORE", 140},
    {"RETURN", 142},
    {"RIGHT$", 201},
    {"RND", 187},
    {"RUN", 138},
    {"SAVE", 148},
    {"SGN", 180},
    {"SIN", 191},
    {"SPC(", 166},
    {"SQR", 186},
    {"STEP", 169},
    {"STOP", 144},
    {"STR$", 196},
    {"SYS", 158},
    {"TAB(", 163},
    {"TAN", 192},
    {"THEN", 167},
    {"TO", 164},
    {"USR", 183},
    {"VAL", 197},
    {"VERIFY", 149},
    {"WAIT", 146},
    {"^", 174},
    {"{BLK}", 144},
    {"{BLU}", 31},
    {"{BRN}", 149},
    {"{CASE LOW}", 14},
    {"{CASE UP}", 142},
    {"{CLR}", 147},
    {"{CURSR DOWN}", 17},
    {"{CURSR LEFT}", 157},
    {"{CURSR RIGHT}", 29},
    {"{CURSR UP}", 145},
    {"{CYN}", 159},
    {"{DEL}", 20},
    {"{DISABLE LOCK}", 8},
    {"{ENABLE LOCK}", 9},
    {"{F1}", 133},
    {"{F2}", 137},
    {"{F3}", 134},
    {"{F4}", 138},
    {"{F5}", 135},
    {"{F6}", 139},
    {"{F7}", 136},
    {"{F8}", 140},
    {"{FLASH ON}", 2},
    {"{GRN}", 30},
    {"{GRY1}", 151},
    {"{GRY2}", 152},
    {"{GRY3}", 155},
    {"{HOME}", 19},
    {"{INST}", 148},
    {"{LBLU}", 154},
    {"{LGRN}", 153},
    {"{LRED}", 150},
    {"{ORNG}", 129},
    {"{PI}", 255},
    {"{PUR}", 156},
    {"{RED}", 28},
    {"{RETURN}", 13},
    {"{RUN/STOP}", 3},
    {"{RVS OFF}", 146},
    {"{RVS ON}", 18},
    {"{SHIFT RETURN}", 141},
    {"{SHIFT RUN}", 131},
    {"{WHT}", 5},
    {"{YEL}", 158},
};
// Reverse dictionary
static std::unordered_map<int, std::string> TokenToKeyword = []
{
    std::unordered_map<int, std::string> rev;
    for (auto& p : KeywordToToken)
        rev[p.second] = p.first;
    return rev;
}();


// === Generated Initializer List ===
std::shared_ptr<ParseNode> root = 
std::make_shared<ParseNode>(
  0, 0,
  std::vector<std::shared_ptr<ParseNode>>{
        std::make_shared<ParseNode>(
          '*', 172,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          '+', 170,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          '-', 171,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          '/', 173,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          '<', 179,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          '=', 178,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          '>', 177,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          'A', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'B', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'S', 182,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'N', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'D', 175,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'S', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'C', 198,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'T', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'N', 193,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'C', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'H', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'R', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '$', 199,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'L', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'O', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'S', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'E', 160,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'R', 156,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'M', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'D', 157,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'O', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'N', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'T', 154,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'S', 190,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'D', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'A', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'T', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'A', 131,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'F', 150,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'I', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'M', 134,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'E', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'N', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'D', 128,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'X', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'P', 189,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'F', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'N', 165,
                  std::vector<std::shared_ptr<ParseNode>>{}
                ),
                std::make_shared<ParseNode>(
                  'O', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'R', 129,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'R', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 184,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'G', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'T', 161,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'O', 203,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'S', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'U', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'B', 141,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'T', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'O', 137,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'I', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'F', 139,
                  std::vector<std::shared_ptr<ParseNode>>{}
                ),
                std::make_shared<ParseNode>(
                  'N', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'P', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'U', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'T', 133,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '#', 132,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'T', 181,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'L', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'F', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'T', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '$', 200,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'N', 195,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        ),
                        std::make_shared<ParseNode>(
                          'T', 136,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'I', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'S', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'T', 155,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'O', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'A', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'D', 147,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'G', 188,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'M', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'I', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'D', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '$', 202,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'N', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'W', 162,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        ),
                        std::make_shared<ParseNode>(
                          'X', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'T', 130,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'O', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'T', 168,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'O', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'N', 145,
                  std::vector<std::shared_ptr<ParseNode>>{}
                ),
                std::make_shared<ParseNode>(
                  'P', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 159,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'R', 176,
                  std::vector<std::shared_ptr<ParseNode>>{}
                )
          }
        ),
        std::make_shared<ParseNode>(
          'P', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'K', 194,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'O', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'K', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'E', 151,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'S', 185,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'R', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'I', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'T', 153,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '#', 152,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'R', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'A', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'D', 135,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'M', 143,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        ),
                        std::make_shared<ParseNode>(
                          'S', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'T', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'O', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'R', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          'E', 140,
                                                          std::vector<std::shared_ptr<ParseNode>>{}
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'T', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'U', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'R', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'N', 142,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'I', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'G', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'H', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'T', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '$', 201,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'N', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'D', 187,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'U', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'N', 138,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'S', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'A', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'V', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'E', 148,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'G', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'N', 180,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'I', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'N', 191,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'P', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'C', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '(', 166,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'Q', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'R', 186,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'T', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'P', 169,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'O', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'P', 144,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'R', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '$', 196,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'Y', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'S', 158,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'T', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'A', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'B', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '(', 163,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'N', 192,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'H', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 167,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'O', 164,
                  std::vector<std::shared_ptr<ParseNode>>{}
                )
          }
        ),
        std::make_shared<ParseNode>(
          'U', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'S', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'R', 183,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'V', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'A', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'L', 197,
                          std::vector<std::shared_ptr<ParseNode>>{}
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'R', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'I', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'F', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'Y', 149,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          'W', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'A', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'I', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'T', 146,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        )
                  }
                )
          }
        ),
        std::make_shared<ParseNode>(
          '^', 174,
          std::vector<std::shared_ptr<ParseNode>>{}
        ),
        std::make_shared<ParseNode>(
          '{', 0,
          std::vector<std::shared_ptr<ParseNode>>{
                std::make_shared<ParseNode>(
                  'B', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'L', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'K', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 144,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                ),
                                std::make_shared<ParseNode>(
                                  'U', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 31,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'R', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 149,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'C', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'A', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'S', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'E', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  ' ', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          'L', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'O', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'W', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  '}', 14,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        ),
                                                        std::make_shared<ParseNode>(
                                                          'U', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'P', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          '}', 142,
                                                                          std::vector<std::shared_ptr<ParseNode>>{}
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'L', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'R', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 147,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'U', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'R', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'S', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'R', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          ' ', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'D', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'O', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  'W', 0,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                        std::make_shared<ParseNode>(
                                                                                          'N', 0,
                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                std::make_shared<ParseNode>(
                                                                                                  '}', 17,
                                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                                )
                                                                                          }
                                                                                        )
                                                                                  }
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                ),
                                                                std::make_shared<ParseNode>(
                                                                  'L', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'E', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  'F', 0,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                        std::make_shared<ParseNode>(
                                                                                          'T', 0,
                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                std::make_shared<ParseNode>(
                                                                                                  '}', 157,
                                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                                )
                                                                                          }
                                                                                        )
                                                                                  }
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                ),
                                                                std::make_shared<ParseNode>(
                                                                  'R', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'I', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  'G', 0,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                        std::make_shared<ParseNode>(
                                                                                          'H', 0,
                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                std::make_shared<ParseNode>(
                                                                                                  'T', 0,
                                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                                        std::make_shared<ParseNode>(
                                                                                                          '}', 29,
                                                                                                          std::vector<std::shared_ptr<ParseNode>>{}
                                                                                                        )
                                                                                                  }
                                                                                                )
                                                                                          }
                                                                                        )
                                                                                  }
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                ),
                                                                std::make_shared<ParseNode>(
                                                                  'U', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'P', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  '}', 145,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'Y', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 159,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'D', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'L', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 20,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'I', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'S', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'A', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'B', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          'L', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'E', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          ' ', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  'L', 0,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                        std::make_shared<ParseNode>(
                                                                                          'O', 0,
                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                std::make_shared<ParseNode>(
                                                                                                  'C', 0,
                                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                                        std::make_shared<ParseNode>(
                                                                                                          'K', 0,
                                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                                std::make_shared<ParseNode>(
                                                                                                                  '}', 8,
                                                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                                                )
                                                                                                          }
                                                                                                        )
                                                                                                  }
                                                                                                )
                                                                                          }
                                                                                        )
                                                                                  }
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'E', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'N', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'A', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'B', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'L', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          'E', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  ' ', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'L', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  'O', 0,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                        std::make_shared<ParseNode>(
                                                                                          'C', 0,
                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                std::make_shared<ParseNode>(
                                                                                                  'K', 0,
                                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                                        std::make_shared<ParseNode>(
                                                                                                          '}', 9,
                                                                                                          std::vector<std::shared_ptr<ParseNode>>{}
                                                                                                        )
                                                                                                  }
                                                                                                )
                                                                                          }
                                                                                        )
                                                                                  }
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'F', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          '1', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 133,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          '2', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 137,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          '3', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 134,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          '4', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 138,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          '5', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 135,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          '6', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 139,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          '7', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 136,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          '8', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 140,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'L', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'A', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'S', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'H', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          ' ', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'O', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'N', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  '}', 2,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'G', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'R', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 30,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                ),
                                std::make_shared<ParseNode>(
                                  'Y', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '1', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 151,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        ),
                                        std::make_shared<ParseNode>(
                                          '2', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 152,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        ),
                                        std::make_shared<ParseNode>(
                                          '3', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 155,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'H', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'O', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'M', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'E', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 19,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'I', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'N', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'S', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'T', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 148,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'L', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'B', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'L', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'U', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 154,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'G', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'R', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'N', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 153,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'R', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'E', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'D', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 150,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'O', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'R', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'G', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  '}', 129,
                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'P', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'I', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  '}', 255,
                                  std::vector<std::shared_ptr<ParseNode>>{}
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'U', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'R', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 156,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'R', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'D', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 28,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                ),
                                std::make_shared<ParseNode>(
                                  'T', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'U', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'R', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          'N', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  '}', 13,
                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'U', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'N', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '/', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'S', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          'T', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'O', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'P', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  '}', 3,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        ),
                        std::make_shared<ParseNode>(
                          'V', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'S', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          ' ', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'O', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          'F', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'F', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          '}', 146,
                                                                          std::vector<std::shared_ptr<ParseNode>>{}
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        ),
                                                        std::make_shared<ParseNode>(
                                                          'N', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  '}', 18,
                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'S', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'H', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'I', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          'F', 0,
                                          std::vector<std::shared_ptr<ParseNode>>{
                                                std::make_shared<ParseNode>(
                                                  'T', 0,
                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                        std::make_shared<ParseNode>(
                                                          ' ', 0,
                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                std::make_shared<ParseNode>(
                                                                  'R', 0,
                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                        std::make_shared<ParseNode>(
                                                                          'E', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  'T', 0,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                        std::make_shared<ParseNode>(
                                                                                          'U', 0,
                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                std::make_shared<ParseNode>(
                                                                                                  'R', 0,
                                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                                        std::make_shared<ParseNode>(
                                                                                                          'N', 0,
                                                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                                                std::make_shared<ParseNode>(
                                                                                                                  '}', 141,
                                                                                                                  std::vector<std::shared_ptr<ParseNode>>{}
                                                                                                                )
                                                                                                          }
                                                                                                        )
                                                                                                  }
                                                                                                )
                                                                                          }
                                                                                        )
                                                                                  }
                                                                                )
                                                                          }
                                                                        ),
                                                                        std::make_shared<ParseNode>(
                                                                          'U', 0,
                                                                          std::vector<std::shared_ptr<ParseNode>>{
                                                                                std::make_shared<ParseNode>(
                                                                                  'N', 0,
                                                                                  std::vector<std::shared_ptr<ParseNode>>{
                                                                                        std::make_shared<ParseNode>(
                                                                                          '}', 131,
                                                                                          std::vector<std::shared_ptr<ParseNode>>{}
                                                                                        )
                                                                                  }
                                                                                )
                                                                          }
                                                                        )
                                                                  }
                                                                )
                                                          }
                                                        )
                                                  }
                                                )
                                          }
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'W', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'H', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'T', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 5,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        )
                  }
                ),
                std::make_shared<ParseNode>(
                  'Y', 0,
                  std::vector<std::shared_ptr<ParseNode>>{
                        std::make_shared<ParseNode>(
                          'E', 0,
                          std::vector<std::shared_ptr<ParseNode>>{
                                std::make_shared<ParseNode>(
                                  'L', 0,
                                  std::vector<std::shared_ptr<ParseNode>>{
                                        std::make_shared<ParseNode>(
                                          '}', 158,
                                          std::vector<std::shared_ptr<ParseNode>>{}
                                        )
                                  }
                                )
                          }
                        )
                  }
                )
          }
        )
  }
);
