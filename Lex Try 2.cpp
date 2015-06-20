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
	int lineCounter = 1, charCounter = 0;

	vector<vector<int>> Table;
	ifstream f;
	string fileDestination;
	int State = 1;
	char tokenBU;
	string lexeme;
	char token;

	bool isSeparator, isOperator;

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
	void GetToken();
	int charToCol(char c);
	void outputState(int n);
	void OutputSeparator();
	void OutputOperator();
	bool CheckSeparator(char c);
	bool CheckOperator(char c);
	bool isKeyword(string lex);
};

/**********************************************************************
Program starts here
**********************************************************************/
int main(){
	cout << "Program: Lex\nProgrammer: Craig Marroquin\nClass: CPSC 323 Summer 2015\n\n" << endl;
	Lexer RAT15SU; // Takes a file from user input //
	RAT15SU.GetToken();
	

	system("Pause");
	return 0;
}

/**********************************************************************
Functions
**********************************************************************/
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
void Lexer::GetToken(){
	int col;

	while (!f.eof() && State != 0){
		token = f.get();
		charCounter++;

		col = charToCol(token);

		State = Table[State][col];
		outputState(State);
		lexeme += tokenBU = token;

		if (isSeparator == 1) OutputSeparator();
		else if (isOperator == 1) OutputOperator();

		if (token == -1 && lexeme.size() == 0){
			cout << "Unidentified token at line: " << lineCounter << " char: " << charCounter << endl;
		}

	}
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
void Lexer::outputState(int n){
	switch (n){
	case(0) : cout << "Unidentified token at line: " << lineCounter << " char: " << charCounter << endl;
		lexeme = "";
		State = 0;
		break; 
	case(4) : if (isKeyword(lexeme)) break;
		cout << left << setw(12) << "IDENTIFIER" << left << setw(7) << lexeme << endl;
		lexeme = "";
		State = 1;
		break;
	case(7) : cout << left << setw(12) << "INTEGER" << left << setw(7) << lexeme << endl;
		lexeme = "";
		State = 1;
		break;
	case(8) : cout << left << setw(12) << "REAL" << left << setw(7) << lexeme << endl;
		lexeme = "";
		State = 1;
		break;
	}
}

// CheckSeparator checks if the char is an separator. If it is, it triggers for the output //
bool Lexer::CheckSeparator(char c){
	// Check if char is a separator //
	char *temp;
	temp = find(charSeparator, charSeparator + 13, c);
	if (temp != charSeparator + 13){
		isSeparator = 1;
		return 1;
	}
	else if (c == '$'){
		char ahead = f.get();
		if (ahead == '$'){
			isSeparator = 1;
			return 1;
		}
		else{
			State = 0;
			return 0;
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
void Lexer::OutputSeparator(){
	string temp; // A temporary string to display the blank spacing that was used //
	char *space;

	/*if (tokenBU == '\t') temp = "\\t";
	else if (tokenBU == '\n'){
		temp = "\\n";
		lineCounter++;
		charCounter = 1;
	}
	else if (tokenBU == ' ') temp = "\' \'";
	else if (tokenBU == '$') temp = "$$";*/
	space = find(Spaces, Spaces + 3, token);
	if (space != Spaces + 3){
		tokenBU = NULL;
		State = 1;
		isSeparator = 0;
		lexeme = "";
	}
	else if (tokenBU == '$') temp = "$$";
	else temp = tokenBU;

	if (tokenBU != NULL){
		cout << left << setw(12) << "SEPARATOR" << left << setw(7) << temp << endl;
		tokenBU = NULL;
		State = 1;
		isSeparator = 0;
		lexeme = "";
	}
}

// OutputOperator is triggered when the char taken in is an operator //
void Lexer::OutputOperator(){
	// First check if the char belongs to a special operator //
	if (tokenBU == '!' || tokenBU == '='){
		char temp = f.get(); // Get the character ahead for checking if "!=" or "=="//
		if (temp == '='){
			cout << left << setw(12) << "OPERATOR" << tokenBU << temp << endl;
			tokenBU = NULL;
			State = 1;
			isOperator = 0;
			lexeme = "";
		}

		// Operator is a regular operator //
		else{
			f.unget(); // Must unget the f.get() we did earlier to check for "!=" and "==" //
			if (tokenBU == '!'){ // If the char was '!' but was not part of "!=" then we throw an error //
				cout << "Unidentified token at line: " << lineCounter << " char: " << charCounter << endl;
				lexeme = "";
				State = 0;
			}
			// Operator '=' by itself //
			if (tokenBU == '='){
				cout << left << setw(12) << "OPERATOR" << left << setw(7) << tokenBU << endl;
				tokenBU = NULL;
				State = 1;
				isOperator = 0;
				lexeme = "";
			}
		}
	}
			// Operator is a regular //
			else{
				cout << left << setw(12) << "OPERATOR" << left << setw(7) << tokenBU << endl;
				tokenBU = NULL;
				State = 1;
				isOperator = 0;
				lexeme = "";
			}
		}

// isKeyword checks if the token is a keyword before marking it as identifier //
bool Lexer::isKeyword(string lex){
	string *temp = find(Keywords, Keywords + 12, lex);
	if (temp != Keywords + 12){
		cout << left << setw(12) << "KEYWORD" << left << setw(7) << lex << endl;
		lexeme = "";
		State = 1;
		return 1;
	}
	else return 0;
}