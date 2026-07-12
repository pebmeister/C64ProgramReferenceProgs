#pragma once

struct Prog {
	int chapter;
	int page;
	int page_end;
	int prog_number;
	std::string prog;
	std::vector<std::string>tags;
};

std::vector<struct Prog> progs {
	{
		1,
		18,
		18,
		1,
		R"(10 PRINT "YOUR NAME": INPUT N$
20 PRINT "HELLO," N$)",
		{ "input", "string", "name"}

	},
	{
		1,
		19,
		19,
		2,
		R"(10 PRINT "ENTER A NUMBER":INPUT A
20 PRINT A)",
		{ "input", "integer" }
	},
	{
		1,
		19,
		19,
		3,
		R"(10 PRINT "ENTER A WORD":INPUT A$
20 PRINT A$)",
		{ "input", "string" }
	},
	{
		1,
		19,
		19,
		4,
		R"(10 PRINT ENTER A NUMBER" : INPUT A
20 PRINT A "TIMES 5 EQUALS" A*5)",
		{ "input", "number", "multiplication" }
	},
	{
		1,
		20,
		20,
		1,
		R"(5 PRINT "[147]"
10 PRINT "MONTHLY INCOME":INPUT IN
20 PRINT
30 PRINT "EXPENSE CATEGORY 1": INPUT E1$
40 PRINT "EXPENSE AMOUNT": INPUT E1
50 PRINT
60 PRINT "EXPENSE CATEGORY 2": INPUT E2$
70 PRINT "EXPENSE AMOUNT": INPUT E2
80 PRINT
90 PRINT "EXPENSE CATEGORY 3": INPUT E3$
100 PRINT "EXPENSE AMOUNT": INPUT E3
110 PRINT "[147]"
120 E=E1+E2+E3
130 EP=E/IN
140 PRINT "MONTHLY INCOME $"IN
150 PRINT "TOTAL EXPENSES $"E
160 PRINT "BALANCE EQUALS $"IN-E
170 PRINT
180 PRINT E1$"="(E1/E)*100"% OF TOTAL EXPENSES"
190 PRINT E2$"="(E2/E)*100"% OF TOTAL EXPENSES"
200 PRINT E3$"="(E3/E)*100"% OF TOTAL EXPENSES"
210 PRINT
220 PRINT "YOUR EXPENSES="EP*100"% OF YOUR TOTAL INCOME"
230 FOR X=1TO5000:NEXT:PRINT
240 PRINT "REPEAT (Y OR N) " : INPUT Y$: IF Y$="Y" THEN 5
250 PRINT "[147]": END)",
		{"income","expense","budget"}
	},
	{
		1,
		23,
		24,
		1,
		R"(10 GET A$:IF A$="" THEN 10
20 IF A$=CHR$(133) THEN POKE 53280,8: GOTO 10
30 IF A$=CHR$(134) THEN POKE 53281,8: GOTO 10
40 IF A$=CHR$(135) THEN A$="DEAR SIR:"+CHR$(13)
50 IF A$=CHR$(136) THEN A$="SINCERLY,"+CHR$(13)
100 PRINT A$;: GOTO10)",
		{"screen", "edit"}
	}

};

