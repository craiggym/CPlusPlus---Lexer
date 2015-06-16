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
using namespace std;

class Lexer {
private: 
	//========================= LIBRARY ===================================//
	// Libray of special strings and characters needed for char comparison //
	string Keyword = "$$if else fi return write read while integer boolean real true false";
	string Operator = "!=-+*/=><==";
	string Separator = " ;{}[]().,'\"";
	//=====================================================================//

	vector<vector<int>> Table;
	ifstream f;
	string fileDestination;
	int State = 1;
	string tokenBU;
	string lexeme;

public:
	Lexer(){
		constructTable();
		GetTxtFile(f);
		Analyzer(f);
	}
	~Lexer(){}
	/******* Function Prototypes ********/
	void constructTable();
	void displayTable();
	void GetTxtFile(ifstream &file);
	void Analyzer(ifstream &file);
	int charToCol(char c);
	void outputState(int n);
};

/**********************************************************************
Program starts here
**********************************************************************/
int main(){
	cout << "Program: Lex\nProgrammer: Craig Marroquin\nClass: CPSC 323 Summer 2015\n\n" << endl;
	Lexer RAT15SU;

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
void Lexer::Analyzer(ifstream &file){
	char token;
	int col;
	while (!f.eof() && State != 0){
		token = f.get();
		col = charToCol(token);

		State = Table[State][col];
		outputState(State);

		tokenBU = lexeme += token;
	}
}

// Takes char and determines which input to the machine //
int Lexer::charToCol(char c){
	if (isalpha(c)) return 0;
	else if (isalnum(c)) return 1;
	else if (c == '.') return 2;
	else if (Separator.find(c)) return 3;
	else return 3;

	}

void Lexer::outputState(int n){
	switch (n){
	case(0) : cout << "Unidentified token at " << tokenBU << endl;
		lexeme = "";
		State = 0;
		break;
	case(4) : cout << left << setw(12) << "IDENTIFIER" << left << setw(7) << lexeme << endl;
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
	case(9) : cout << left << setw(12) << "SEPARATOR" << left << setw(7) << lexeme << endl;
		lexeme = "";
		State = 1;
		break;
	}
}