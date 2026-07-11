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
    {"RIGHT$", 201},
    {"ASC", 198},
    {"LEN", 195},
    {"PEEK", 194},
    {"ATN", 193},
    {"TAN", 192},
    {"COS", 190},
    {"EXP", 189},
    {"RND", 187},
    {"CONT", 154},
    {"VERIFY", 149},
    {"WAIT", 146},
    {"SIN", 191},
    {"^", 174},
    {"ON", 145},
    {"PRINT#", 152},
    {"STOP", 144},
    {"SAVE", 148},
    {"STEP", 169},
    {"REM", 143},
    {"CLR", 156},
    {"POS", 185},
    {"PRINT", 153},
    {"RETURN", 142},
    {"*", 172},
    {"GOSUB", 141},
    {"END", 128},
    {"FOR", 129},
    {"DEF", 150},
    {"RESTORE", 140},
    {"POKE", 151},
    {"CHR$", 199},
    {"INPUT#", 132},
    {"THEN", 167},
    {"LOAD", 147},
    {"<", 179},
    {"NEXT", 130},
    {"INPUT", 133},
    {"RUN", 138},
    {"OR", 176},
    {"FRE", 184},
    {"MID$", 202},
    {"USR", 183},
    {"LET", 136},
    {"DIM", 134},
    {"STR$", 196},
    {"LOG", 188},
    {"IF", 139},
    {"CMD", 157},
    {"SYS", 158},
    {"+", 170},
    {"INT", 181},
    {"OPEN", 159},
    {"DATA", 131},
    {"LIST", 155},
    {"NEW", 162},
    {"SPC(", 166},
    {"TO", 164},
    {"CLOSE", 160},
    {"GET", 161},
    {"TAB(", 163},
    {"FN", 165},
    {"NOT", 168},
    {"-", 171},
    {"GOTO", 137},
    {"/", 173},
    {"READ", 135},
    {"ABS", 182},
    {"AND", 175},
    {"VAL", 197},
    {">", 177},
    {"LEFT$", 200},
    {"=", 178},
    {"SGN", 180},
    {"SQR", 186},
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
                  'O', 0,
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
        )
  }
);
