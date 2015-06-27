//=============================================================================================================================================
//File: Lex.cpp
//Class: CPSC 323
//Section: MTR 1:30 PM
//Instructor: Prof. Song Choi
//Homework Assignment 1
//
// Description:
//		This program tokenizes from an input file. There will be no error handling in terms of syntax.
//==============================================================================================================================================
//Programmer: Craig Marroquin
//Date Due: 06/14/15
//==============================================================================================================================================
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <array>

using namespace std;

//========================= LIBRARY ===================================//
// Libray of special strings and characters needed for char comparison //
char charSeparator[13] = {' ', '\t', '\n', ';', '{', '}', '[', ']', '(', ')', '.', ',', '\"' };
char Operator[7] = { '=', '+', '-', '/', '*', '>', '<'};
char Spaces[3] = { ' ', '\t', '\n' };
string Keywords[12] = { "if", "fi", "else", "return", "write", "read", "while", "integer", "boolean", "real", "true", "false" };
//=====================================================================//

class Lexer {
private:
	int lineCounter = 1, charCounter = 1;

	vector<vector<int>> Table;
	ifstream f;
	string fileDestination;
	int State = 1;
	char tokenBU;
	string lexeme;
	char token;
	string tokenID;

	bool isSeparator, isOperator, isID, isInt, isReal, isKey, tokReady;

public:
	Lexer(){
		constructTable();
		GetTxtFile(f);
	}
	~Lexer(){}
	/******* Function Prototypes ********/
	void constructTable();
	void displayTable();
	void GetTxtFile(ifstream &file);
	void Parse(string theToken);
	void retError();
	void retError(string predict);
	int charToCol(char c);
	string outputState(int n);
	string OutputSeparator();
	string OutputOperator();
	string getTokenID();
	string GetToken();
	bool CheckSeparator(char c);
	bool CheckOperator(char c);
	bool isKeyword(string lex);
	bool isEOF();
};

/**********************************************************************
Program starts here
**********************************************************************/
int main2(){
	cout << "Program: Lex\nProgrammer: Craig Marroquin\nClass: CPSC 323 Summer 2015\n\n" << endl;
	Lexer RAT15SU; // Takes a file from user input //
	//while (!RAT15SU.isEOF()){
	//	RAT15SU.Parse(RAT15SU.GetToken());
	//}

	return 0;
}

// Constructs the DFSM table //
void Lexer::constructTable(){
	vector<int> tableRow(5);

	//--- Row 0: Error Row ---//
	Table.push_back(tableRow);

	//--- Row 1: Start Row ---//
	tableRow[0] = 2;
	tableRow[1] = 5;
	tableRow[2] = 0;
	tableRow[3] = 9;
	tableRow[4] = 0;
	Table.push_back(tableRow);

	//--- Row 2: ID Row ---//
	tableRow[0] = 2;
	tableRow[1] = 3;
	tableRow[2] = 0;
	tableRow[3] = 4;
	tableRow[4] = 0;
	Table.push_back(tableRow);

	//--- Row 3: ID with Num Row ---//
	tableRow[0] = 2;
	tableRow[1] = 3;
	tableRow[2] = 0;
	tableRow[3] = 0;
	tableRow[4] = 0;
	Table.push_back(tableRow);

	//--- Row 4: Accept ID ---//
	tableRow.assign(5, 1);
	Table.push_back(tableRow);

	//--- Row 5: Int Row ---//
	tableRow[0] = 0;
	tableRow[1] = 5;
	tableRow[2] = 6;
	tableRow[3] = 7;
	tableRow[4] = 0;
	Table.push_back(tableRow);

	//--- Row 6: Real Row ---//
	tableRow[0] = 0;
	tableRow[1] = 6;
	tableRow[2] = 0;
	tableRow[3] = 8;
	tableRow[4] = 0;
	Table.push_back(tableRow);

	//--- Row 7: Accept Int ---//
	tableRow.assign(5, 1);
	Table.push_back(tableRow);

	//--- Row 8: Accept Real ---//
	tableRow.assign(5, 1);
	Table.push_back(tableRow);

	//--- Row 9: Accept Separator ---//
	tableRow.assign(5, 1);
	Table.push_back(tableRow);

}

// Display the table for my own purposes //
void Lexer::displayTable(){
	const int row_count = 9;
	const int col_count = 5;

	for (int i = 0; i < row_count; i++){
		for (int j = 0; j < col_count; j++)
			cout << Table[i][j];
		cout << endl;
	}
}

// Takes a txt file from user input //
void Lexer::GetTxtFile(ifstream &file){
	cout << "Please input the directory where the source file resides.\nExample: C:\\Users\\Craig\\Downloads\\file.txt\n" << endl;
	cout << "Destination: ";

	getline(cin, fileDestination);
	f.open(fileDestination, ios::in);

	while (!f.is_open()){
		cout << "================================================================================";
		cout << "\n\nThe file you specified could not be found. Check the format and try again." << endl;
		f.close();

		cout << "Example: C:\\Users\\Craig\\Downloads\\file.txt\n" << endl;
		cout << "Destination: ";

		getline(cin, fileDestination);
		f.open(fileDestination, ios::in);
	}
	//f.close();
	cout << "Consumed file: " << fileDestination << '\n' << endl;

}

// Tokenizes the text file //
string Lexer::GetToken(){
	int col; // col will act as input for the DFSM //
	string retToken; // retToken is a token of what this function will return //

	// While file is not at end and state is not at 0 //
	while (!f.eof() && State != 0){
		if (isSeparator == 1){
			retToken = OutputSeparator();
			return retToken;
		}
		else if (isOperator == 1){
			retToken = OutputOperator();
			return retToken;
		}

		token = f.get();

		col = charToCol(token);

		State = Table[State][col];
		retToken = outputState(State);
		if(tokReady != 1) lexeme += tokenBU = token;
		else tokenBU = token;

		if (token == -1 && lexeme.size() == 0){
			retError();
			retToken = "";
			break;
		}
		charCounter++;
		if (tokReady == 1) break; // The token is ready to return //
	}
	return retToken;
}

// Takes char and determines the input to the machine //
int Lexer::charToCol(char c){
	if (isalpha(c)) return 0;            // char is alphabetical //
	else if (isalnum(c)) return 1;       // char is a number //
	else if (c == '.') return 2;         // char is a '.' //
	else if (c == -1) return 3;
	else {
		if (CheckSeparator(c)) return 3;
		else if (CheckOperator(c)) return 3;
		else  return 4;
	}
}

// outputState takes in a number from the results of a DFSM lookup and outputs the token and lexeme //
string Lexer::outputState(int n){
	switch (n){
	case(0) : retError();
		tokReady = 1;
		lexeme = "\0";
		State = 0;
		return lexeme;
	case(4) : if (isKeyword(lexeme)) return lexeme;
		tokReady = 1;
		isID = 1;
		State = 1;
		return lexeme;
	case(7) :
		tokReady = 1;
		isInt = 1;
		State = 1;
		return lexeme;
	case(8) :
		tokReady = 1;
		isReal = 1;
		State = 1;
		return lexeme;
	}
	return "";
}

// DisplayToken gets passed a string when a full token has been reached (determined by a separator or operator)//
void Lexer::Parse(string theToken){
	if (theToken == "\0") return;
	else if (isID == 1) tokenID = "IDENTIFIER";
	else if (isInt == 1) tokenID = "INTEGER";
	else if (isReal == 1) tokenID = "REAL";
	else if (isKey == 1) tokenID = "KEYWORD";
	else if (isSeparator == 1) {
		if (tokenBU == NULL){
			isSeparator = 0;
			lexeme = "";
			State = 1;
			return;
		}
		else{
			tokenID = "SEPARATOR";
			isSeparator = 0;
		}
	}
	else if (isOperator == 1) {
		tokenID = "OPERATOR";
		isOperator = 0;
	}

	//cout << left << setw(12) << tokenID << left << setw(7) << theToken << endl;
	isID = tokReady = isInt = isReal = isKey = 0;
	lexeme = "";
	State = 1;
}

// CheckSeparator checks if the char is an separator. If it is, it triggers for the output //
 bool Lexer::CheckSeparator(char c){
	 // error catch if tokenBU is '.' //
	 if (tokenBU == '.'){
		 State = 0;
		 return 0;
	 }
	 // Line counter incrementer //
	 if (c == '\n'){
		 lineCounter++;
		 charCounter = 1;
	 }

	// Check if char is a separator //
	char *temp;
	temp = find(charSeparator, charSeparator + 13, c);
	if (temp != charSeparator + 13){
		isSeparator = 1;
		return 1;
	}
	else if (c == '$'){
		char ahead = f.get();
		char lookAhead = f.get();
		char *spacer = find(charSeparator, charSeparator + 13, lookAhead);

		if (ahead == '$' && (spacer != (charSeparator + 13))){
			charCounter += 2;
			tokenBU = '$';
			isSeparator = 1;
			return 1;
		}
		else{
			f.unget();
			charCounter++;
			if (ahead == '$') lexeme = "$$";
			else lexeme = "$";
			retError();
			//State = 0;
			//return 0;
		}
	}
	else {
		temp = NULL;
		return 0;
	}
}

// CheckOperator checks if the char is an operator. If it is, it triggers for the output //
bool Lexer::CheckOperator(char c){

	// Check if char is a operator //
	char *temp;
	temp = find(Operator, Operator + 7, c);
	if (temp != Operator + 7){
		isOperator = 1;
		return 1;
	}

	// However, check if char is '!' in the case that we have an "!=" operator //
	else if (c == '!'){
		isOperator = 1;
		return 1;
	}

	else return 0;
	temp = NULL;
}

// OutputSeparator is triggered when the char taken in is a separator //
string Lexer::OutputSeparator(){
	string retTemp;
	retTemp += tokenBU;
	char *space;
	space = find(Spaces, Spaces + 3, tokenBU);
	if (space != Spaces + 3){
		tokenBU = NULL;
		return retTemp;
	}
	else if (token == '$'){
			retTemp += "$";
			return retTemp;
	}
	else {
		return retTemp;
	}
}


// OutputOperator is triggered when the char taken in is an operator //
string Lexer::OutputOperator(){
	string retTemp = "";
	retTemp += tokenBU; // The char that will be returned //
	// First check if the char belongs to a special operator //
	if (tokenBU == '!' || tokenBU == '='){
		char temp = f.get(); // Get the character ahead for checking if "!=" or "=="//
		if (temp == '='){
			retTemp += temp;
			tokenBU = NULL;
			State = 1;
			lexeme = "";
			return retTemp;
		}

		// Operator is a regular operator //
		else{
			f.unget(); // Must unget the f.get() we did earlier to check for "!=" and "==" //
			if (tokenBU == '!'){ // If the char was '!' but was not part of "!=" then we throw an error //
				lexeme = "";
				State = 0;
				retTemp = "\0";
				return retTemp;
			}
			// Operator '=' by itself //
			if (tokenBU == '='){
				retTemp = tokenBU;
				tokenBU = NULL;
				State = 1;
				lexeme = "";
				return retTemp;
			}
			else{
				f.unget();
				retTemp = tokenBU;
				tokenBU = NULL;
				State = 1;
				lexeme = "";
				return retTemp;
			}
		}
	}
	// Operator is a regular //
	else{
		retTemp = tokenBU;
		tokenBU = NULL;
		State = 1;
		lexeme = "";
		return retTemp;
	}
}

// isKeyword checks if the token is a keyword before marking it as identifier //
bool Lexer::isKeyword(string lex){
	string *temp = find(Keywords, Keywords + 12, lex);
	if (temp != Keywords + 12){
		//cout << left << setw(12) << "KEYWORD" << left << setw(7) << lex << endl;
		lexeme = lex;
		tokReady = 1;
		State = 1;
		isKey = 1;
		return 1;
	}
	else return 0;
}

bool Lexer::isEOF(){
	if (token == -1 || State == 0) return true;
	else return false;
}

string Lexer::getTokenID(){
	return tokenID;
}

void Lexer::retError(){
	char getter;
	getter = f.get();
	while (true)
	{
		if (getter == ' ' || getter == '\n' || getter == '\t' || getter == '[' || getter == ']' || getter == '(' || getter == ')' || getter == '{' ||
			getter == '}' || getter == -1) break;
		lexeme += getter;
		getter = f.get();
	}

	cout << "\n\nUnidentified token: " << lexeme << endl;
	system("PAUSE");
	exit(1);
}
void Lexer::retError(string predict){
	cout << "\n\nUnexpected token at line: " << lineCounter << " char: " << charCounter;
	cout << " Expected " << predict <<  endl;
	system("PAUSE");
	exit(1);
}
