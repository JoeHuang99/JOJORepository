#include<stdio.h>
#include <stdlib.h>
#include<iostream>
#include<vector>
#include <fstream>
#include<string>
#include <sstream>
#include <iomanip>
#include <math.h>
using namespace std;

int tokenIsString = 0 ; // 紀錄當前這個Token是不是String 
int tokenIsInteger = 0 ; // 紀錄當前這個Token是不是16進為的Integer 
int tokenIsComment = 0 ; // 若設為1則代表之後直到換行時的字元都是Comment


struct Token { 
	int tableNum ; // 紀錄Table的編號 
	int entryNum ; // 紀錄在Table中的位置 
	string name ; // 紀錄該Token的名字 
};

struct TokenList { // 用來存一個Row的Token 
	vector<Token> list ;
};

struct TokenTable { // 用來存好幾個Row的Token 
	vector<TokenList> table ;
};

struct InstructionSet {
	int opcode ; // 紀錄該指令的Opcode 
	int operand ; // 紀錄該指令的運算元個數 
	int format ; // 格式 
	int isXE ;
};

struct AssemblerList { // statement用額外的TokenTable型別來存 
	int format ;
	int line ;
	int loc ;
	string objectCode ;
};

struct LiteralBuffer {
	int line ;
	int loc ;
	string statement ;
	string ASCIICode ;
	int serialNum ;
	string name ;
};
/*
struct AssemblerList_XE {
	int format ;
	int line ;
	int loc ;
	string objectCode ;
};
*/
struct TokenAndAddress { 
	int tableNum ; // 紀錄Table的編號 
	int entryNum ; // 紀錄在Table中的位置 
	string name ; // 紀錄該Token的名字 
	int address ; // 儲存該Token被定義的地址 
};

struct StandardType {
	int opcode ;
	int x ;
	int address ;
};

struct XEType {
	int format ;
	int opcode ;
	int r1 ;
	int r2 ;
	int n ;
	int i ;
	int x ;
	int b ;
	int p ;
	int e ;
	int disp ;
	int address ;
};

struct Literal {
	int tableNum ; // 紀錄Table的編號 
	int entryNum ; // 紀錄在Table中的位置 
	string name ; // 紀錄該Token的名字 
	int address ;
	int serialNum ;
};

class SICStructure { // 使用SIC進行語法切割 
public:
	string table1Name = "Table1.table" ;
	string table2Name = "Table2.table" ;
	string table3Name = "Table3.table" ;
	string table4Name = "Table4.table" ;
	
	string filename ; // 檔案名稱 
	vector<Token> table1 ; // 第一個Table 
	vector<Token> table2 ; // 第二個Table 
	vector<Token> table3 ; // 第三個Table 
	vector<Token> table4 ; // 第四個Table 
	Token symbolTable[100] ; // SymbolTable 
	Token integerTable[100] ; // IntegerTable
	Token stringTable[100] ; // StringTable
	vector<string> result ; // 用來存讀進來的Input 
	
	char ch = '\0' ; // 紀錄當前的字元 
	vector<char> aToken ; // 紀錄當前的Token
	 
	TokenTable answerTable ; // 紀錄所有的Token，也就是答案 
	TokenList answerList ; // 記錄這個Row上的Token 
	void addATokenInAnswerList() { // 將一個Token加入進answerList中 
		Token temp ;
		string s = "" ;
		for ( int i = 0 ; i < aToken.size() ; i++ ) {
			s = s + aToken[i] ;
		} // for
		
		temp.name = s ;
	 	findTableNumAndEntryNum( s, temp.tableNum, temp.entryNum ) ;
		answerList.list.push_back( temp ) ;
	} // addATokenInAnswerList()
	
	void addATokenInAnswerList( char ch ) { // 將一個Token加入進answerList中(用於delimiter)
		Token temp ;
		temp.name = "" ;
		temp.name.push_back( ch ) ;
	 	findTableNumAndEntryNum( temp.name, temp.tableNum, temp.entryNum ) ;
		answerList.list.push_back( temp ) ;
	} // addATokenInAnswerList()
	
	void addAnswerListInAnswerTable() { // 把一個Line的Token加入Table中 
		answerTable.table.push_back( answerList ) ;
	} // addAnswerListInAnswerTable()
	
	void toLower( string s, string & s_lower ) {
		for ( int i = 0 ; i < s.size() ; i++ ) {
			if ( ( s[i] >= 65 ) && ( s[i] <= 90 ) ) {
				s_lower[i] = s[i] + 32 ;
			} // if
			
			else {
				s_lower[i] = s[i] ;
			} // else
		} // for
	} // toLower()
	
	void toUpper( string s, string & s_upper ) {
		for ( int i = 0 ; i < s.size() ; i++ ) {
			if ( ( s[i] >= 97 ) && ( s[i] <= 122 ) ) {
				s_upper[i] = s[i] - 32 ;
			} // if
			
			else {
				s_upper[i] = s[i] ;
			} // else
		} // for
	} // toLower()
	
	void findTableNumAndEntryNum( string s, int & tableNum, int & entryNum ) { // 找到該Token對應的資訊 
		
		string s_lower = s ;
		string s_upper = s ;
		
		toLower( s, s_lower ) ;
		toUpper( s, s_upper ) ;
		
		for ( int i = 0 ; i < table1.size() ; i++ ) {
			if ( ( ( s == table1[i].name ) || ( s_lower == table1[i].name ) || ( s_upper == table1[i].name ) ) && ( tokenIsString == 0 ) ) {
				tableNum = 1 ;
				entryNum = i + 1 ;
				return ;
			} // if
		} // for
			
		for ( int i = 0 ; i < table2.size() ; i++ ) {
			if ( ( ( s == table2[i].name ) || ( s_upper == table2[i].name ) || ( s_lower == table2[i].name ) ) && ( tokenIsString == 0 ) ) {
				tableNum = 2 ;
				entryNum = i + 1 ;
				return ;
			} // if
		} // for
			
		for ( int i = 0 ; i < table3.size() ; i++ ) {
			if ( ( ( s == table3[i].name ) || ( s_upper == table3[i].name ) || ( s_lower == table3[i].name ) ) && ( tokenIsString == 0 ) ) {
				tableNum = 3 ;
				entryNum = i + 1 ;
				return ;
			} // if
		} // for
			
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( ( s == table4[i].name ) && ( ( tokenIsString == 0 ) || ( s == "\'" ) ) ) {
				tableNum = 4 ;
				entryNum = i + 1 ;
				return ;
			} // if
		} // for
		
		if ( tokenIsString == 1 ) {
			hashInStringTable( s, tableNum, entryNum ) ;
		} // if
		
		else if ( ( tokenIsInteger == 1 ) || isInteger(s) ) {
			hashInIntegerTable( s, tableNum, entryNum ) ;
		} // else if
		
		else {
			hashInSymbolTable( s, tableNum, entryNum ) ;
		} // else
		
	} // findEntryNum()
	
	bool isInteger( string s ) {
		bool x = true ;
		for ( int i = 0 ; i < s.size() ; i++ ) {
			if ( ( s[i] < 48 ) || ( s[i] > 57 ) ) {
				x = false ;
			} // if
		} // for
		
		return x ;
	} // isInteger()
	
	void hashInSymbolTable( string s, int & tableNum, int & entryNum ) {  
		int hashValue = getHashValue() ; // 取得hashValue 
		while ( ( symbolTable[hashValue].entryNum != -1 ) && ( symbolTable[hashValue].name != s ) ) { // 發生碰撞的情況 
			hashValue++ ;
			if ( hashValue == 100 ) {
				hashValue = hashValue % 100 ;
			} // if
		} // while
		
		symbolTable[hashValue].entryNum = hashValue ;
		symbolTable[hashValue].tableNum = 5 ;
		symbolTable[hashValue].name = s ;
		tableNum = 5 ;
		entryNum = hashValue ;
	} // hashInSymbolTable()
	
	void hashInIntegerTable( string s, int & tableNum, int & entryNum ) {
		int hashValue = getHashValue() ; // 取得hashValue 
		while ( ( integerTable[hashValue].entryNum != -1 ) && ( integerTable[hashValue].name != s ) ) { // 發生碰撞的情況 
			hashValue++ ;
			if ( hashValue == 100 ) {
				hashValue = hashValue % 100 ;
			} // if
		} // while
		
		integerTable[hashValue].entryNum = hashValue ;
		integerTable[hashValue].tableNum = 6 ;
		integerTable[hashValue].name = s ;
		tableNum = 6 ;
		entryNum = hashValue ;
	} // hashInIntegerTable()
	
	void hashInStringTable( string s, int & tableNum, int & entryNum ) {
		int hashValue = getHashValue() ; // 取得hashValue  
		while ( ( stringTable[hashValue].entryNum != -1 ) && ( stringTable[hashValue].name != s ) ) { // 發生碰撞的情況 
			hashValue++ ;
			if ( hashValue == 100 ) {
				hashValue = hashValue % 100 ;
			} // if
		} // while
		
		stringTable[hashValue].entryNum = hashValue ;
		stringTable[hashValue].tableNum = 7 ;
		stringTable[hashValue].name = s ;
		tableNum = 7 ;
		entryNum = hashValue ;
	} // hashInStringTable()
	
	void initializeTable() { // 將5, 6, 7給table初始化 
		for ( int i = 0 ; i < 100 ; i++ ) {
			symbolTable[i].entryNum = -1 ;
			symbolTable[i].tableNum = 5 ;
			symbolTable[i].name = "" ;
			integerTable[i].entryNum = -1 ;
			integerTable[i].tableNum = 6 ;
			integerTable[i].name = "" ;
			stringTable[i].entryNum = -1 ;
			stringTable[i].tableNum = 7 ;
			stringTable[i].name = "" ;
		} // for
	} // initializeTable()
	
	int getHashValue() { // 取得hashValue 
		int sum = 0;
		int hashValue = 0 ;
		for ( int i = 0 ; i < aToken.size() ; i++ ) {
			sum = sum + aToken[i] ;
		} // for
		
		hashValue = sum % 100 ;
		return hashValue ;
	} // getHashValue()
	
	bool isAToken() { // 判斷之前有沒有讀到Token 
		if ( aToken.empty() ) {
			return false ;
		} // if
		
		return true ;
	} // isAToken()
	
	void getTokens() { // 取得所有的Token 
		
		for ( int nowLine = 0 ; nowLine < result.size() ; nowLine++ ) {
			for ( int nowPos = 0 ; nowPos < result[nowLine].size() ; nowPos++ ) {
				getChar( nowLine, nowPos ) ;
			} // for
		} // for
	} // getTokens()
	
	void getChar( int nowLine, int nowPos ) {
		ch = result[nowLine][nowPos] ;
		if ( ( tokenIsComment == 1 ) && ( ch != '\n' ) ) { // 是註解的情況，且不是Enter 
			;
		} // if
		
		else if( ( ch == ' ' ) || ( ch == '\t' ) ) { // 是Space或是Tab 
			
			if ( tokenIsString == 1 ) { // 是String的情況 
				aToken.push_back( ch ) ;
			} // if
			
			else if ( isAToken() ) { // 前面有Token的情況 
				addATokenInAnswerList() ;
				//aToken.clear() ;
				while(!aToken.empty()) { // 清除Token內的元素 
					aToken.pop_back();  
				} // while
				vector<char>(aToken).swap(aToken) ; // 釋放記憶體，類似於換一個全新的Token 
			} // else if
			
			tokenIsInteger = 0 ; 
		} // if
		
		else if ( ch == '\n' ) { // 是Enter 
			tokenIsComment = 0 ;
			if ( !aToken.empty() ) {
				addATokenInAnswerList() ;
			} // if
			//aToken.clear() ;
			while(!aToken.empty()) {
				aToken.pop_back();
			} // while
			vector<char>(aToken).swap(aToken) ;
			addAnswerListInAnswerTable() ;
			//answerList.list.clear() ;
			while(!answerList.list.empty()) {
				answerList.list.pop_back();
			} // while
			vector<Token>(answerList.list).swap(answerList.list) ;
			tokenIsInteger = 0 ;
		} // else if
		
		else if ( ( !aToken.empty() ) && ( ( ( aToken.back() == 'C' ) && ( aToken.size() == 1 ) ) || ( ( aToken.back() == 'c') && ( aToken.size() == 1 ) ) ) && ( ch == '\'' ) && ( tokenIsString == 0 ) && ( tokenIsInteger == 0 ) ) { // Token是字串的情況 
			//aToken.clear() ;
			while(!aToken.empty()) {
				aToken.pop_back();
			} // while
			vector<char>(aToken).swap(aToken) ;
			addATokenInAnswerList( ch ) ;
			tokenIsString = 1 ;
		} // else if
		
		else if ( (!aToken.empty() ) && ( ( ( aToken.back() == 'X' ) && ( aToken.size() == 1 ) ) || ( ( aToken.back() == 'x' ) && ( aToken.size() == 1 ) ) ) && ( ch == '\'' ) && ( tokenIsString == 0 ) ) { // Token是16進位數字的情況 
			//aToken.clear() ;
			while(!aToken.empty()) {
				aToken.pop_back();
			} // while
			vector<char>(aToken).swap(aToken) ;
			addATokenInAnswerList( ch ) ;
			tokenIsInteger = 1 ;
		} // else if
		
		else if ( ( ch == '.' ) && ( tokenIsString == 0 ) ) { // Token是註解的情況 
			if ( !aToken.empty() ) {
				addATokenInAnswerList() ;
			} // if
			//aToken.clear() ;
			while(!aToken.empty()) {
				aToken.pop_back();
			} // while
			vector<char>(aToken).swap(aToken) ;
			addATokenInAnswerList( ch ) ;
			tokenIsComment = 1 ;
		} // else if
		
		else if ( isDelimiter( ch ) && ( ( tokenIsString == 0 ) || ( ch == '\'' ) ) ) {	// 是Delimiter，但是不被包含在字串裡面 
			if ( !aToken.empty() ) {
				addATokenInAnswerList() ;
			}	 // if	
			//aToken.clear() ;
			while(!aToken.empty()) {
				aToken.pop_back();
			} // while
			vector<char>(aToken).swap(aToken) ;
			addATokenInAnswerList( ch ) ;
			if ( ch == '\'' )
				tokenIsString = 0 ;
		} // else if
		
		else { // 添加到aToken中 
			aToken.push_back( ch ) ;
		} // else
		
	} // getChar()
	
	bool isDelimiter( char ch ) { // 判斷是不是Delimiter 
		string s ; 
		s.push_back(ch) ; // 字元轉成字串 
		for ( int i = 0 ; i < table4.size() ; i++ ) {
			if ( s == table4[i].name ) {
				return true ;
			} // if
		} // for
		
		return false ;
	} // isDelimiter()
	
	bool readInResult() { // 將輸入檔案以一行一行的方式存入資料結構中 
		fstream infile ;
		string s ;
    	infile.open( ( filename + ".txt" ).c_str() ) ;
		if ( !infile.is_open() ) { // 沒有這個檔案 
			cout << endl << filename << ".txt does not exist!" << endl ;
			return false ;
		} // if
		
		else { // 有這個檔案
			while( getline( infile, s ) ) {
		 		result.push_back( s ) ; 
			} // while
			
			setResultEnd() ; // 在每一行後面加入換行符號 
		} // else
			
		infile.close() ;
		return true ;
	} // readInResult()
	
	void setResultEnd() {
		for ( int i = 0 ; i < result.size() ; i++ ) {
			result[i].push_back('\n') ;
		} // for
	} // setResultEnd()
	
	bool setTables() { // 設定前面4個Table 
		readInTable( "Table1.table" ) ;
		readInTable( "Table2.table" ) ;
		readInTable( "Table3.table" ) ;
		readInTable( "Table4.table" ) ;
		return true ;
	} // setTable()
	
	int readInTable( string tableName ) {
		fstream infile ;
		string s ;
    	infile.open( tableName.c_str() ) ;
		if ( !infile.is_open() ) // 沒有這個檔案 
			cout << endl << tableName << " does not exist!" << endl ;
		else { // 有這個檔案
			while( getline( infile, s, '\n' ) ) {
				if ( tableName == "Table1.table" ) {
					AddInTable1( s ) ;
				} // if
				
				else if ( tableName == "Table2.table" ) {
					AddInTable2( s ) ;
				} // else if
				
				else if ( tableName == "Table3.table" ) {
					AddInTable3( s ) ;
				} // else if
				
				else if ( tableName == "Table4.table" ) {
					AddInTable4( s ) ;
				} // else if
				
			} // while
		} // else
		
		infile.close() ;
		if ( tableName == "Table1.table" ) {
			return 1 ;
		} // if
				
		else if ( tableName == "Table2.table" ) {
			return 2 ;
		} // else if
				
		else if ( tableName == "Table3.table" ) {
			return 3 ;
		} // else if
				
		else if ( tableName == "Table4.table" ) {
			return 4 ;
		} // else if
		
		else {
			return 0 ;
		} // else 
	} // readInTable()
	
	void AddInTable1( string s ) {
		Token temp ;
		temp.entryNum = table1.size() + 1 ;
		temp.tableNum = 1 ; 
		if ( s[s.size()-1] == ' ' ) {
			s = s.substr(0, s.length() - 1) ;
		} // if
		temp.name = s ;
		table1.push_back( temp ) ;
	} // AddInTable1()
	
	void AddInTable2( string s ) {
		Token temp ;
		temp.entryNum = table1.size() + 1 ;
		temp.tableNum = 1 ; 
		if ( s[s.size()-1] == ' ' ) {
			s = s.substr(0, s.length() - 1) ;
		} // if
		temp.name = s ;
		table2.push_back( temp ) ;
	} // AddInTable2()
	
	void AddInTable3( string s ) {
		Token temp ;
		temp.entryNum = table1.size() + 1 ;
		temp.tableNum = 1 ; 
		if ( s[s.size()-1] == ' ' ) {
			s = s.substr(0, s.length() - 1) ;
		} // if
		temp.name = s ;
		table3.push_back( temp ) ;
	} // AddInTable3()
	
	void AddInTable4( string s ) {
		Token temp ;
		temp.entryNum = table1.size() + 1 ;
		temp.tableNum = 1 ; 
		if ( s[s.size()-1] == ' ' ) {
			s = s.substr(0, s.length() - 1) ;
		} // if
		temp.name = s ;
		table4.push_back( temp ) ;
	} // AddInTable4()
	
	void writeFile() { // 寫檔輸出 
		fstream outFile ;
		outFile.open( ( filename + "_out.txt" ).c_str() , fstream::out ) ;
		if ( !outFile.is_open() ) {
			cout << "Error" << endl ;
			return ;
		} // if
		
		for ( int i = 0 ; i < result.size() ; i++ ) {
			outFile << result[i] ;
			for ( int j = 0 ; j < answerTable.table[i].list.size() ; j++ ) {
				outFile << "(" << answerTable.table[i].list[j].tableNum << "," << answerTable.table[i].list[j].entryNum << ")" ;
			} // for
			
			outFile << endl ;
		} // for
		
		outFile.close() ;
	} // writeFile()
	
	void printAnswer() {
		for ( int i = 0 ; i < result.size() ; i++ ) {
			cout << result[i] ;
			for ( int j = 0 ; j < answerTable.table[i].list.size() ; j++ ) {
				cout << "(" << answerTable.table[i].list[j].tableNum << "," << answerTable.table[i].list[j].entryNum << ")" ;
			} // for
			
			cout << endl ;
		} // for
	} // printAnswer()
};

class SICAssembler {
public:
	int instructionSetEntry = 0 ;
	int operandNumber = 0 ;
	int balance = -1 ;
	int lineTemp = 5 ;
	int nextLoc = 0x0000 ;
	int startLoc = 0x0000 ;
	int locTemp = 0x0000 ;
	int locTempForEQU = 0x0000 ;
	string ASCIICode = "" ;
	StandardType standardType ;
	bool isSTART = false ;
	bool isExcutable = false ;
	bool isBYTE = false ;
	bool isWORD = false ;
	bool isRESB = false ;
	bool isRESW = false ;
	bool isComment = false ;
	bool isPseudo = false ;
	bool isEND = false ;
	bool isEQU = false ;
	bool isBASE = false ;
	bool isLTORG = false ;
	bool isIndexRegister = false ;
	int indexRegister = 0 ;
	bool isXE = false ;
	// 以下為XE
	bool isF4 = false ;
	bool r1IsDone = false ;
	//vector<AssemblerList_XE> assemblerList_XE ; // 存放最終的結果 for XE 
	XEType xeType ;
	int BASERegister = 0 ;
	// 以上為XE 
	vector<AssemblerList> assemblerList ; // 存放最終的結果 
	
	TokenTable tokenTable ; // 紀錄第一階段所存的所有Token 
 
	vector<InstructionSet> instructionSet ; // 存指令集 
	
	vector<Token> table1 ; // 第一個Table 
	vector<Token> table2 ; // 第二個Table 
	vector<Token> table3 ; // 第三個Table 
	vector<Token> table4 ; // 第四個Table 
	TokenAndAddress symbolTable[100] ; // SymbolTable 
	TokenAndAddress integerTable[100] ; // IntegerTable
	TokenAndAddress stringTable[100] ; // StringTable
	Literal literalTable[100] ; // Literal Table
	vector<LiteralBuffer> literalBufferTable ; // 等到END的時候要印在END的最後面 
	
	vector<int> digitsVector ;
	int digits = 0 ;
	
	int literalNum = 0 ; // 紀錄有多少個literal
	void setLiteralBufferTable( int line, int loc, int serialNum, int tableNum, string name ) { // 設定literalBufferTable 
		LiteralBuffer temp ;
		temp.name = name ;
		temp.line = line ;
		temp.loc = loc ;
		temp.serialNum = serialNum ;
		if ( tableNum == 6 ) {
			temp.statement = "=x'" + name + "'" ;
			temp.ASCIICode = getByteASCIICode( temp.name, 6 ) ; 
			for ( int i = 0 ; i < temp.ASCIICode.size() ; i++ ) {
				if ( ( temp.ASCIICode[i] > 96 ) && ( temp.ASCIICode[i] < 123 ) ) {
					temp.ASCIICode[i] = temp.ASCIICode[i] - 32 ;
				} // if
			} // for
			//cout << temp.ASCIICode << endl ;
		} // if
			
		else if ( tableNum == 7 ) {
			temp.statement = "=c'" + name + "'" ;
			temp.ASCIICode = getByteASCIICode( temp.name, 7 ) ;
			temp.ASCIICode = decimalObjectCodeToHexadecimal_NbitString( temp.ASCIICode ) ;
			//cout << temp.ASCIICode << endl ;
		} // else if
		
		if ( hasNoNameInLiteralBufferTable( temp.name ) )
			literalBufferTable.push_back( temp ) ;
	} // setLiteralBuffer()
	
	bool hasNoNameInLiteralBufferTable( string name ) {
		bool decide = true ;
		for ( int i = 0 ; i < literalBufferTable.size() ; i++ ) {
			if ( literalBufferTable[i].name == name ) {
				decide = false ;
			} // if
		} // for
		
		return decide ;
	} // hasNoNameInLiteralBufferTable()
	
	int findAddressInLiteralTable( string s ) {
		int num = 0 ;
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( literalTable[i].name == s ) {
				num = literalTable[i].address ;
				//cout << literalTable[i].name << literalTable[i].address << endl ;
			} // if
		} // for
		
		return num ;
	} // findAddressInLiteralTable()
	
	void fixS( string & s, int tableNum ) { // 將所有的String轉成Table中對應的大小寫 
		string s_lower = s ;
		string s_upper = s ;
		for ( int i = 0 ; i < s.size() ; i++ ) { // 轉成小寫 
			if ( ( s[i] >= 65 ) && ( s[i] <= 90 ) ) {
				s_lower[i] = s[i] + 32 ;
			} // if
			
			else {
				s_lower[i] = s[i] ;
			} // else
		} // for

		for ( int i = 0 ; i < s.size() ; i++ ) { // 轉成大寫 
			if ( ( s[i] >= 97 ) && ( s[i] <= 122 ) ) {
				s_upper[i] = s[i] - 32 ;
			} // if
			
			else {
				s_upper[i] = s[i] ;
			} // else
		} // for
		
		if ( tableNum == 1 ) {
			s = s_lower ;
		} // if
		
		else if ( tableNum == 2 ) {
			s = s_upper ;
		} // else if
		
		else if ( tableNum == 3 ) {
			s = s_upper ;
		} // esle if
	} // fixS()
	
	void setTables( vector<Token> t1, vector<Token> t2, vector<Token> t3, vector<Token> t4, Token * t5, Token * t6, Token * t7 ) { // 設定7個Tables 
		table1.assign( t1.begin(), t1.end() ) ;
		table2.assign( t2.begin(), t2.end() ) ;
		table3.assign( t3.begin(), t3.end() ) ;
		table4.assign( t4.begin(), t4.end() ) ;
		for ( int i = 0 ; i < 100 ; i++ ) {
			symbolTable[i].tableNum = t5[i].tableNum ;
			symbolTable[i].entryNum = t5[i].entryNum ;
			symbolTable[i].name = t5[i].name ;
			symbolTable[i].address = -1 ;
		} // for
		
		for ( int i = 0 ; i < 100 ; i++ ) {
			integerTable[i].tableNum = t6[i].tableNum ;
			integerTable[i].entryNum = t6[i].entryNum ;
			integerTable[i].name = t6[i].name ;
			integerTable[i].address = -1 ;
		} // for
		
		for ( int i = 0 ; i < 100 ; i++ ) {
			stringTable[i].tableNum = t7[i].tableNum ;
			stringTable[i].entryNum = t7[i].entryNum ;
			stringTable[i].name = t7[i].name ;
			stringTable[i].address = -1 ;
		} // for
		
		for ( int i = 0 ; i < 100 ; i++ ) {
			literalTable[i].address = -1 ;
			literalTable[i].name = "-1" ;
			literalTable[i].tableNum = -1 ;
			literalTable[i].entryNum = -1 ;
			literalTable[i].serialNum = -1 ;
		} // for
	} // setTables()
	
	void setTokenTables( TokenTable t ) { // 將切Token的結果複製過來 
		tokenTable.table.assign( t.table.begin(), t.table.end() ) ;
	} // setTokenTable()
	
	void setInstructureSet() { // 設定指令集 
		InstructionSet set ;
		set.opcode = -1 ;
		set.operand = -1 ;
		set.format = -1 ;
		
		for ( int i = 0 ; i < 59 ; i++ ) {
			instructionSet.push_back( set ) ;
		} // for
		
		instructionSet[0].operand = 1 ;
		instructionSet[0].format = 3 ;
		instructionSet[0].opcode = 0x18 ;
		instructionSet[0].isXE = 0 ;
		instructionSet[1].operand = 1 ;
		instructionSet[1].format = 3 ;
		instructionSet[1].opcode = 0x58 ;
		instructionSet[1].isXE = 1 ;		
		instructionSet[2].operand = 2 ;
		instructionSet[2].format = 2 ;
		instructionSet[2].opcode = 0x90 ;
		instructionSet[2].isXE = 1 ;	
		instructionSet[3].operand = 1 ;
		instructionSet[3].format = 3 ;
		instructionSet[3].opcode = 0x40 ;
		instructionSet[3].isXE = 0 ;
		instructionSet[4].operand = 1 ;
		instructionSet[4].format = 2 ;
		instructionSet[4].opcode = 0xB4 ;
		instructionSet[4].isXE = 1 ;
		instructionSet[5].operand = 1 ;
		instructionSet[5].format = 3 ;
		instructionSet[5].opcode = 0x28 ;
		instructionSet[5].isXE = 0 ;
		instructionSet[6].operand = 1 ;
		instructionSet[6].format = 3 ;
		instructionSet[6].opcode = 0x88 ;
		instructionSet[6].isXE = 1 ;
		instructionSet[7].operand = 2 ;
		instructionSet[7].format = 2 ;
		instructionSet[7].opcode = 0xA0 ;
		instructionSet[7].isXE = 1 ;
		instructionSet[8].operand = 1 ;
		instructionSet[8].format = 3 ;
		instructionSet[8].opcode = 0x24 ;
		instructionSet[8].isXE = 0 ;
		instructionSet[9].operand = 1 ;
		instructionSet[9].format = 3 ;
		instructionSet[9].opcode = 0x64 ;
		instructionSet[9].isXE = 1 ;
		instructionSet[10].operand = 2 ;
		instructionSet[10].format = 2 ;
		instructionSet[10].opcode = 0x9C ;
		instructionSet[10].isXE = 1 ;
		instructionSet[11].operand = 0 ;
		instructionSet[11].format = 1 ;
		instructionSet[11].opcode = 0xC4 ;
		instructionSet[11].isXE = 1 ;
		instructionSet[12].operand = 0 ;
		instructionSet[12].format = 1 ;
		instructionSet[12].opcode = 0xC0 ;
		instructionSet[12].isXE = 1 ;
		instructionSet[13].operand = 0 ;
		instructionSet[13].format = 1 ;
		instructionSet[13].opcode = 0xF4 ;
		instructionSet[13].isXE = 1 ;
		
		instructionSet[14].operand = 1 ;
		instructionSet[14].format = 3 ;
		instructionSet[14].opcode = 0x3C ;
		instructionSet[14].isXE = 0 ;
		instructionSet[15].operand = 1 ;
		instructionSet[15].format = 3 ;
		instructionSet[15].opcode = 0x30 ;
		instructionSet[15].isXE = 0 ;
		instructionSet[16].operand = 1 ;
		instructionSet[16].format = 3 ;
		instructionSet[16].opcode = 0x34 ;
		instructionSet[16].isXE = 0 ;
		instructionSet[17].operand = 1 ;
		instructionSet[17].format = 3 ;
		instructionSet[17].opcode = 0x38 ;
		instructionSet[17].isXE = 0 ;
		instructionSet[18].operand = 1 ;
		instructionSet[18].format = 3 ;
		instructionSet[18].opcode = 0x48 ;
		instructionSet[18].isXE = 0 ;
		instructionSet[19].operand = 1 ;
		instructionSet[19].format = 3 ;
		instructionSet[19].opcode = 0x00 ;
		instructionSet[19].isXE = 0 ;
		instructionSet[20].operand = 1 ;
		instructionSet[20].format = 3 ;
		instructionSet[20].opcode = 0x68 ;
		instructionSet[20].isXE = 1 ;
		instructionSet[21].operand = 1 ;
		instructionSet[21].format = 3 ;
		instructionSet[21].opcode = 0x50 ;
		instructionSet[21].isXE = 0 ;
		instructionSet[22].operand = 1 ;
		instructionSet[22].format = 3 ;
		instructionSet[22].opcode = 0x70 ;
		instructionSet[22].isXE = 1 ;
		instructionSet[23].operand = 1 ;
		instructionSet[23].format = 3 ;
		instructionSet[23].opcode = 0x08 ;
		instructionSet[23].isXE = 0 ;
		instructionSet[24].operand = 1 ;
		instructionSet[24].format = 3 ;
		instructionSet[24].opcode = 0x6C ;
		instructionSet[24].isXE = 1 ;
		instructionSet[25].operand = 1 ;
		instructionSet[25].format = 3 ;
		instructionSet[25].opcode = 0x74 ;
		instructionSet[25].isXE = 1 ;
		instructionSet[26].operand = 1 ;
		instructionSet[26].format = 3 ;
		instructionSet[26].opcode = 0x04 ;
		instructionSet[26].isXE = 0 ;
		instructionSet[27].operand = 1 ;
		instructionSet[27].format = 3 ;
		instructionSet[27].opcode = 0xD0 ;
		instructionSet[27].isXE = 1 ;
		instructionSet[28].operand = 1 ;
		instructionSet[28].format = 3 ;
		instructionSet[28].opcode = 0x20 ;
		instructionSet[28].isXE = 0 ;
		
		instructionSet[29].operand = 1 ;
		instructionSet[29].format = 3 ;
		instructionSet[29].opcode = 0x60 ;
		instructionSet[29].isXE = 1 ;
		instructionSet[30].operand = 2 ;
		instructionSet[30].format = 2 ;
		instructionSet[30].opcode = 0x98 ;
		instructionSet[30].isXE = 1 ;
		instructionSet[31].operand = 0 ;
		instructionSet[31].format = 1 ;
		instructionSet[31].opcode = 0xC8 ;
		instructionSet[31].isXE = 1 ;
		instructionSet[32].operand = 1 ;
		instructionSet[32].format = 3 ;
		instructionSet[32].opcode = 0x44 ;
		instructionSet[32].isXE = 0 ;
		instructionSet[33].operand = 1 ;
		instructionSet[33].format = 3 ;
		instructionSet[33].opcode = 0xD8 ;
		instructionSet[33].isXE = 0 ;
		instructionSet[34].operand = 2 ;
		instructionSet[34].format = 2 ;
		instructionSet[34].opcode = 0xAC ;
		instructionSet[34].isXE = 1 ;
		instructionSet[35].operand = 0 ;
		instructionSet[35].format = 3 ;
		instructionSet[35].opcode = 0x4C ;
		instructionSet[35].isXE = 0 ;
		instructionSet[36].operand = 2 ;
		instructionSet[36].format = 2 ;
		instructionSet[36].opcode = 0xA4 ;
		instructionSet[36].isXE = 1 ;
		instructionSet[37].operand = 2 ;
		instructionSet[37].format = 2 ;
		instructionSet[37].opcode = 0xA8 ;
		instructionSet[37].isXE = 1 ;
		
		instructionSet[38].operand = 0 ;
		instructionSet[38].format = 1 ;
		instructionSet[38].opcode = 0xF0 ;
		instructionSet[38].isXE = 1 ;
		instructionSet[39].operand = 1 ;
		instructionSet[39].format = 3 ;
		instructionSet[39].opcode = 0xEC ;
		instructionSet[39].isXE = 1 ;
		instructionSet[40].operand = 1 ;
		instructionSet[40].format = 3 ;
		instructionSet[40].opcode = 0x0C ;
		instructionSet[40].isXE = 0 ;
		instructionSet[41].operand = 1 ;
		instructionSet[41].format = 3 ;
		instructionSet[41].opcode = 0x78 ;
		instructionSet[41].isXE = 1 ;
		instructionSet[42].operand = 1 ;
		instructionSet[42].format = 3 ;
		instructionSet[42].opcode = 0x54 ;
		instructionSet[42].isXE = 0 ;
		instructionSet[43].operand = 1 ;
		instructionSet[43].format = 3 ;
		instructionSet[43].opcode = 0x80 ;
		instructionSet[43].isXE = 1 ;
		instructionSet[44].operand = 1 ;
		instructionSet[44].format = 3 ;
		instructionSet[44].opcode = 0xD4 ;
		instructionSet[44].isXE = 1 ;
		instructionSet[45].operand = 1 ;
		instructionSet[45].format = 3 ;
		instructionSet[45].opcode = 0x14 ;
		instructionSet[45].isXE = 0 ;
		instructionSet[46].operand = 1 ;
		instructionSet[46].format = 3 ;
		instructionSet[46].opcode = 0x7C ;
		instructionSet[46].isXE = 1 ;
		instructionSet[47].operand = 1 ;
		instructionSet[47].format = 3 ;
		instructionSet[47].opcode = 0xE8 ;
		instructionSet[47].isXE = 0 ;
		instructionSet[48].operand = 1 ;
		instructionSet[48].format = 3 ;
		instructionSet[48].opcode = 0x84 ;
		instructionSet[48].isXE = 1 ;
		instructionSet[49].operand = 1 ;
		instructionSet[49].format = 3 ;
		instructionSet[49].opcode = 0x10 ;
		instructionSet[49].isXE = 0 ;
		instructionSet[50].operand = 1 ;
		instructionSet[50].format = 3 ;
		instructionSet[50].opcode = 0x1C ;
		instructionSet[50].isXE = 0 ;
		instructionSet[51].operand = 1 ;
		instructionSet[51].format = 3 ;
		instructionSet[51].opcode = 0x5C ;
		instructionSet[51].isXE = 1 ;
		
		instructionSet[52].operand = 2 ;
		instructionSet[52].format = 2 ;
		instructionSet[52].opcode = 0x94 ;
		instructionSet[52].isXE = 1 ;
		instructionSet[53].operand = 1 ;
		instructionSet[53].format = 2 ;
		instructionSet[53].opcode = 0xB0 ;
		instructionSet[53].isXE = 1 ;
		instructionSet[54].operand = 1 ;
		instructionSet[54].format = 3 ;
		instructionSet[54].opcode = 0xE0 ;
		instructionSet[54].isXE = 0 ;
		instructionSet[55].operand = 0 ;
		instructionSet[55].format = 1 ;
		instructionSet[55].opcode = 0xF8 ;
		instructionSet[55].isXE = 1 ;
		instructionSet[56].operand = 1 ;
		instructionSet[56].format = 3 ;
		instructionSet[56].opcode = 0x2C ;
		instructionSet[56].isXE = 0 ;
		instructionSet[57].operand = 1 ;
		instructionSet[57].format = 2 ;
		instructionSet[57].opcode = 0xB8 ;
		instructionSet[57].isXE = 1 ;
		instructionSet[58].operand = 1 ;
		instructionSet[58].format = 3 ;
		instructionSet[58].opcode = 0xDC ;
		instructionSet[58].isXE = 0 ;
	} // setInstructureSet()
	
	void pass1() { // pass1 
		for ( int i = 0 ; i < tokenTable.table.size() ; i++ ) { // 處理第i個Row 
			if ( !isSTART ) {
				locTemp = nextLoc ;
			} // if
			
			else {
				isSTART = false ;
			} // else 
			
			for ( int j = 0 ; j < tokenTable.table[i].list.size() ; j++ ) { // 處理第j個Column 
				
				
			    if ( isXE == 1 ) {
			    	analysis_pass1_XE( i, j ) ;
				} // if
				
				else {
					analysis_pass1( i, j ) ;
				} // else
					
			} // for
			
			if ( isSTART ) {
				locTemp = startLoc ;
				nextLoc = startLoc ;
			} // if
			
		
			AssemblerList row ;
			row.line = -1 ;
			row.loc = -1 ;
			row.objectCode = "-1" ;
			row.format = -1 ;
			if ( isXE == 0 ) {
				setARow_pass1( row, i ) ;
				assemblerList.push_back( row ) ;
			} // if
			
			else {
				if ( xeType.disp == -1 )
					xeType.disp = 0 ;
				setARow_pass1_XE( row, i ) ;
				assemblerList.push_back( row ) ;
			} // else
			
			
			if ( !tokenTable.table[i].list.empty() )
				lineTemp = lineTemp + 5 ;
			// 初始化 
			isExcutable = false ;
			isBYTE = false ;
			isWORD = false ;
			isRESB = false ;
			isRESW = false ;
			isPseudo = false ;
			isComment = false ;
			isEND = false ;
			isEQU = false ;
			isBASE = false ;
			isLTORG = false ;
			// XE
			xeType.format = 0 ;
			xeType.opcode = 0 ;
			xeType.r1 = 0 ;
			xeType.r2 = 0 ;
			xeType.n = 0 ;
			xeType.i = 0 ;
			xeType.x = 0 ;
			xeType.b = 0 ;
			xeType.p = 0 ;
			xeType.e = 0 ;
			xeType.disp = -1 ;
			xeType.address = -1 ;
			isF4 = false ;
		} // for
	} // pass1
	
	void setARow_pass1( AssemblerList & row , int i ) {
		row.line = lineTemp ; // 紀錄一個Row的line 
		if ( isComment && !isExcutable ) {
			row.loc = -1 ;
		} // if
		
		else if ( tokenTable.table[i].list.empty() ) {
			row.loc = -1 ;
			row.line = -1 ;
		} // else if
		
		else if ( isEQU ) {
			row.loc = locTempForEQU ;
		} // else if
		
		else if ( isEND ) {
			row.loc = -1 ;
		} // else if
		
		else {
			row.loc = locTemp ;
		} // else
	} // setARow_pass1()
	
	void setARow_pass1_XE( AssemblerList & row , int i ) {
		row.line = lineTemp ; // 紀錄一個Row的line
		row.format = xeType.format ;
		if ( isBASE ) {
			row.loc = -1 ;
		} // if
		
		else if ( isComment && !isExcutable ) {
			row.loc = -1 ;
		} // else if
		
		else if ( tokenTable.table[i].list.empty() ) {
			row.loc = -1 ;
			row.line = -1 ;
		} // else if
		
		else if ( isEQU ) {
			row.loc = locTempForEQU ;
		} // else if
		
		else if ( isEND ) {
			row.loc = -1 ;
		} // else if
		
		else {
			row.loc = locTemp ;
		} // else
	} // setARow_pass1_XE()
	
	void analysis_pass1( int i, int j ) {
		string s = tokenTable.table[i].list[j].name ;
		fixS( s, tokenTable.table[i].list[j].tableNum ) ;
		if ( tokenTable.table[i].list[j].tableNum == 1 ) { // 是可執行的指令 
			isExcutable = true ;
			nextLoc = nextLoc + 3 ;
		} // if
		
		else if ( tokenTable.table[i].list[j].tableNum == 2 ) { // 是Pseudo指令
			
			
			
			if ( s == "START" ) { // 後面會跟著Integer 
				isSTART = true ;
				startLoc = stringToInteger_hex( tokenTable.table[i].list[j+1].name ) ; 
			} // if
			
			else if ( s == "BYTE" ) {
				isBYTE = true ;
				nextLoc = nextLoc + getByteSize( tokenTable.table[i].list[j+2].name, tokenTable.table[i].list[j+2].tableNum ) ;
				
			} // else if
			
			else if ( s == "WORD" ) {
				isWORD = true ;
				nextLoc = nextLoc + 3 ; 
			} // else if
			
			else if ( s == "RESB" ) {
				isRESB = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) ;
			} // else if
			
			else if ( s == "RESW" ) {
				isRESW = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) * 3 ;
			} // else if
			
			else if ( s == "END" ) {
				isEND = true ;
				setLiteralTableAddress() ;
			} // esle if
			
			else if ( s == "EQU" ) {
				locTempForEQU = stringToInteger( tokenTable.table[i].list[j+1].name ) ;
				isEQU = true ;
			} // esle if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 3 ) { // 是Register 
			;
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 4 ) { // 是Delimiter 
			if ( s == "." ) {
				isComment = true ;
			} //if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 5 ) { // 是Symbol Table 
			if ( j == 0 ) { // 是該Row的第一個Token，也就是Symbol 
				if ( symbolTable[tokenTable.table[i].list[j].entryNum].address != -1 ) {
					cout << "多重定義！！！" << endl ; 
				} // if
				
				else {
					if ( ( j < tokenTable.table[i].list.size() ) && ( tokenTable.table[i].list[j+1].name == "EQU" ) ) {
						symbolTable[tokenTable.table[i].list[j].entryNum].address = stringToInteger( tokenTable.table[i].list[j+2].name ) ;
					} // if
					
					else {
						symbolTable[tokenTable.table[i].list[j].entryNum].address = locTemp ; // 設定 SymbolTable的地址
					} // else
				} // else

			} // if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 6 ) { // 是Integer Table 
			if ( ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) || ( ( j > 0 ) && ( tokenTable.table[i].list[j-1].name == "=" ) ) ) {
				hashInLiteralTable( tokenTable.table[i].list[j].name, tokenTable.table[i].list[j].tableNum ) ;
			} // if
			
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 7 ) { // 是String Table 
			if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
				hashInLiteralTable( tokenTable.table[i].list[j].name, tokenTable.table[i].list[j].tableNum ) ;
			} // if
		} // else if
	} // analysis_pass1()
	
	void analysis_pass1_XE( int i, int j ) {
		string s = tokenTable.table[i].list[j].name ;
		fixS( s, tokenTable.table[i].list[j].tableNum ) ;
		if ( ( tokenTable.table[i].list[j].tableNum == 1 ) ) { // 是可執行的指令 
			isExcutable = true ;
			int opcodeTemp = 0 ;
			xeType.opcode = instructionSet[tokenTable.table[i].list[j].entryNum-1].opcode ;
			xeType.format = instructionSet[tokenTable.table[i].list[j].entryNum-1].format ;
			if ( isFormat4( s, i, j ) ) {
				xeType.format = 4 ;
			} // if
			
			if ( xeType.format == 1 ) {
				nextLoc = nextLoc + 1 ;
			} // if
			
			else if ( xeType.format == 2 ) {
				nextLoc = nextLoc + 2 ;
			} // else if
			
			else if ( xeType.format == 3 ) {
				nextLoc = nextLoc + 3 ;
			} // else if
			
			else if ( xeType.format == 4 ) {
				nextLoc = nextLoc + 4 ;
			} // else if
		} // if
		
		
		
		else if ( tokenTable.table[i].list[j].tableNum == 2 ) { // 是Pseudo指令
			
			if ( s == "START" ) { // 後面會跟著Integer 
				isSTART = true ;
				startLoc = stringToInteger_hex( tokenTable.table[i].list[j+1].name ) ; 
			} // if
			
			else if ( s == "BYTE" ) {
				isBYTE = true ;
				nextLoc = nextLoc + getByteSize( tokenTable.table[i].list[j+2].name, tokenTable.table[i].list[j+2].tableNum ) ;
				
			} // else if
			
			else if ( s == "WORD" ) {
				isWORD = true ;
				nextLoc = nextLoc + 3 ; 
			} // else if
			
			else if ( s == "RESB" ) {
				isRESB = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) ;
			} // else if
			
			else if ( s == "RESW" ) {
				isRESW = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) * 3 ;
			} // else if
			
			else if ( s == "END" ) {
				isEND = true ;
				setLiteralTableAddress() ;
			} // esle if
			
			else if ( s == "EQU" ) {
				locTempForEQU = stringToInteger( tokenTable.table[i].list[j+1].name ) ;
				isEQU = true ;
			} // esle if
			
			else if ( s == "BASE" ) {
				isBASE = true ;
			} // esle if
			
			else if ( s == "LTORG" ) {
				isLTORG = true ;
				setLiteralTableAddress() ;
			} // esle if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 3 ) { // 是Register 
			;
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 4 ) { // 是Delimiter 
			if ( s == "." ) {
				isComment = true ;
			} //if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 5 ) { // 是Symbol Table 
			if ( j == 0 ) { // 是該Row的第一個Token，也就是Symbol
				if ( symbolTable[tokenTable.table[i].list[j].entryNum].address != -1 ) {
					cout << "多重定義！！！" << endl ; 
				} // if
				
				else {
					if ( ( j < tokenTable.table[i].list.size() ) && ( tokenTable.table[i].list[j+1].name == "EQU" ) ) {
						symbolTable[tokenTable.table[i].list[j].entryNum].address = stringToInteger( tokenTable.table[i].list[j+2].name ) ;
					} // if
					
					else {
						symbolTable[tokenTable.table[i].list[j].entryNum].address = locTemp ; // 設定 SymbolTable的地址
					} // else
				} // else
					
			} // if
			
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 6 ) { // 是Integer Table 
			if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
				//cout << 123456789 ;
				hashInLiteralTable( tokenTable.table[i].list[j].name, tokenTable.table[i].list[j].tableNum ) ;
			} // if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 7 ) { // 是String Table 
			if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
				//cout << 123456789 ;
				hashInLiteralTable( tokenTable.table[i].list[j].name, tokenTable.table[i].list[j].tableNum ) ;
			} // if
		} // else if
	} // analysis_pass1_XE()
	
	void setLiteralTableAddress() { // 位Literal Table中的Literal分配地址 
		//for ( int x = 0 ; x < literalNum ; x++ ) {
		int x = 1 ;
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( ( literalTable[i].name != "-1" ) && ( literalTable[i].tableNum == 6 ) ) {
				
				integerTable[literalTable[i].entryNum].address = nextLoc ;
				literalTable[i].address = nextLoc ;
				nextLoc = nextLoc + 3 ;
				//cout << literalTable[i].name << literalTable[i].address << endl ;
				setLiteralBufferTable( lineTemp+5*x, locTemp+3*(x-1), x, 6, literalTable[i].name ) ;
				x++ ;
			} // if
			
			if ( ( literalTable[i].name != "-1" ) && ( literalTable[i].tableNum == 7 ) ) {
				
				stringTable[literalTable[i].entryNum].address = nextLoc ;
				literalTable[i].address = nextLoc ;
				nextLoc = nextLoc + 3 ;
				//cout << literalTable[i].name << literalTable[i].address << endl ;
				setLiteralBufferTable( lineTemp+5*x, locTemp+3*(x-1), x, 7, literalTable[i].name ) ;
				x++ ;
			} // if
		} // for
		//} // for
		
	} // setLiteralTable()
	
	void hashInLiteralTable( string s, int tableNum ) {
		//int hashValue = getHashValue( s ) ; // 取得hashValue 
		int hashValue = 0 ;
		while ( ( literalTable[hashValue].name != "-1" ) && ( literalTable[hashValue].name != s ) ) { // 發生碰撞的情況 
			hashValue++ ;
			/*
			if ( hashValue == 100 ) {
				hashValue = hashValue % 100 ;
			} // if
			*/
		} // while
		
		literalTable[hashValue].entryNum = hashValue ;
		literalTable[hashValue].tableNum = tableNum ;
		literalTable[hashValue].name = s ;
		literalTable[hashValue].serialNum = literalNum ;
		literalNum++ ;
	} // hashInLiteralTable()
	
	int getHashValue( string s ) {
		int sum = 0;
		int hashValue = 0 ;
		for ( int i = 0 ; i < s.size() ; i++ ) {
			sum = sum + s[i] ;
		} // for
		
		hashValue = sum % 100 ;
		return hashValue ;
	} // getHashValue()
	
	int getByteSize( string s, int tableNum ) { // 目標String與屬於哪一個Table回傳其所占的空間大小 
		int size = 0 ;
		if ( tableNum == 6 ) {
			size = s.size() / 2 ;
		} // if
		
		else if ( tableNum == 7 ) {
			size = s.size() ;
		} // else if
		
		else {
			cout << "ERROR" << endl ; 
		} // else
		
		return size ;
	} // getByteSize()
	
	void pass2() { // pass2
		for ( int i = 0 ; i < tokenTable.table.size() ; i++ ) { // 處理第i個Row 
			
			if ( !isSTART ) {
				locTemp = nextLoc ;
			} // if
			
			else {
				isSTART = false ;
			} // else 
			
			for ( int j = 0 ; j < tokenTable.table[i].list.size() ; j++ ) { // 處理第j個Column 
				if ( isXE == 1 ) {
			    	analysis_pass2_XE( i, j ) ;
				} // if
				
				else {
					analysis_pass2( i, j ) ;
				} // else
			} // for
			
			if ( isSTART ) {
				locTemp = startLoc ;
				nextLoc = startLoc ;
			} // if
			
			if ( isXE == 0 ) {
				if ( operandNumber > 1 && isIndexRegister ) {
					standardType.x = 1 ;
					standardType.address = standardType.address + indexRegister ;
				} // if
				
				AssemblerList row ;
				setARow_pass2( row, i ) ;
				setassemblerList( assemblerList, row, i ) ;
				
			} // if
			
			else {
				if ( operandNumber == ( instructionSet[instructionSetEntry].operand + 1 ) && isIndexRegister && ( ( xeType.format == 3 ) || ( xeType.format == 4 ) ) ) {
					xeType.x = 1 ;
				} // if
				
				else {
					xeType.x = 0 ;
				} // else
				
				if ( xeType.disp == -1 )
					xeType.disp = 0 ;
					
				AssemblerList row ;
				setARow_pass2_XE( row, i ) ;
				setassemblerList_XE( assemblerList, row, i ) ;
			} // else
			
			if ( !tokenTable.table[i].list.empty() )
				lineTemp = lineTemp + 5 ;
			// 初始化 
			instructionSetEntry = 0 ;
			operandNumber = 0 ;
			balance = -1 ;
			isBYTE = false ;
			isWORD = false ;
			isRESB = false ;
			isRESW = false ;
			isPseudo = false ;
			isComment = false ;
			isExcutable = false ;
			isEND = false ;
			isEQU = false ;
			isBASE = false ;
			isLTORG = false ;
			standardType.address = 0 ;
			standardType.opcode = 0 ;
			standardType.x = 0 ;
			isIndexRegister = false ;
			// XE
			isIndexRegister = false ;
			xeType.format = 0 ;
			xeType.opcode = 0 ;
			xeType.r1 = 0 ;
			xeType.r2 = 0 ;
			xeType.n = 0 ;
			xeType.i = 0 ;
			xeType.x = 0 ;
			xeType.b = 0 ;
			xeType.p = 0 ;
			xeType.e = 0 ;
			xeType.disp = -1 ;
			xeType.address = -1 ;
			isF4 = false ;
			r1IsDone = false ;
		} // for
	} // pass2()
	
	void setARow_pass2( AssemblerList & row, int i ) {
		row.line = lineTemp ; // 紀錄一個Row的line 
		if ( isBYTE ) {
			row.objectCode = ASCIICode ;
		} // if
		
		else if ( isWORD ) {
			row.objectCode = ASCIICode ;
		} // else if
		
		else if ( isPseudo ) {
			row.objectCode = "-1" ;
		} // else if
		
		else if ( tokenTable.table[i].list.empty() ) {
			row.objectCode = "-1" ;
		} // else if
		
		else if ( isComment && !isExcutable ) {
			row.objectCode = "-1" ;
		} // else if
		
		else {
			row.objectCode = standardTypeToObjectCode( standardType ) ;
		} // else
	} // setARow_pass2()
	
	void setARow_pass2_XE( AssemblerList & row, int i ) {
		row.line = lineTemp ; // 紀錄一個Row的line 
		if ( isBYTE ) {
			row.objectCode = ASCIICode ;
		} // if
		
		else if ( isWORD ) {
			row.objectCode = ASCIICode ;
		} // else if
		
		else if ( isPseudo ) {
			row.objectCode = "-1" ;
		} // else if
		
		else if ( tokenTable.table[i].list.empty() ) {
			row.objectCode = "-1" ;
		} // else if
		
		else if ( isComment && !isExcutable ) {
			row.objectCode = "-1" ;
		} // else if
		
		else {
			
			if ( xeType.format == 1 ) {
				
				row.objectCode = XETypeToObjectCode_1( xeType ) ;
			} // if
			
			else if ( xeType.format == 2 ) {
				row.objectCode = XETypeToObjectCode_2( xeType ) ;
			} // else if
			
			else if ( xeType.format == 3 ) {
				row.objectCode = XETypeToObjectCode_3( xeType ) ;
			} // else if
			
			else if ( xeType.format == 4 ) {
				row.objectCode = XETypeToObjectCode_4( xeType ) ;
			} // else if
			
		} // else
	} // setARow_pass2_XE()
	
	void setassemblerList( vector<AssemblerList> & assemblerList, AssemblerList row, int i ) {
		assemblerList[i].objectCode = row.objectCode ;
	} // setassemblerList()
	
	string standardTypeToObjectCode( StandardType standardType ) { // 轉成機器碼 
		string s1 = "" ;
		string s2 = "" ;
		string s3 = "" ;
		int a = standardType.opcode; 
		int b = standardType.x ;
		int c = standardType.address ;
		
		char opcode[8]; 
		char x[1] ;
		char address[15] ;
		
		for ( int i = 0 ; i < 8 ; i++ ) {
			opcode[i] = '\0' ;
		} // for
		
		x[0] = '\0' ;
		
		for ( int i = 0 ; i < 15 ; i++ ) {
			address[i] = '\0' ;
		} // for
		
		itoa(a, opcode, 2); 
		for ( int i = 0 ; i < 8 ; i++ ) {
			if ( opcode[i] != '\0' ) {
				s1.push_back(opcode[i]) ;
			} // if
		} // for
		string new_opcode = string(8 - s1.size(), '0') + s1;
		//cout << new_opcode << endl ;
		
		itoa(b, x, 2); 
		s2.push_back(x[0]) ;
		string new_x = s2 ;
		//cout << new_x << endl ;
		
		itoa(c, address, 2); 
		for ( int i = 0 ; i < 15 ; i++ ) {
			if ( address[i] != '\0' ) {
				s3.push_back(address[i]) ;
			} // if
		} // for
		string new_address = string(15 - s3.size(), '0') + s3;
		//cout << new_address << endl ;
		
		string final = new_opcode + new_x + new_address ;
		//cout << final ;
		final = binaryObjectCodeToHexadecimal( final ) ;
		return final ;
	} // standardTypeToObjectCode()
	
	string binaryObjectCodeToHexadecimal( string s ) {
		string final = "" ;
		string nibble = "" ;
		for ( int i = 0 ; i < s.size() / 4 ; i++ ) {
			nibble = s.substr( 4 * i, 4 ) ; 
			//cout << nibble << "   " ;
			setNibble( nibble ) ;
			final = final + nibble ;	
		} // for
		//cout << endl ;
		return final ;
	} // binaryObjectCodeToHexadecimal()
	
	void setNibble( string & nibble ) {
		int sum = 0 ;
		for ( int i = 0 ; i < 4 ; i++ ) {
			if ( nibble[i] == '1' ) {
				sum =  sum + pow( 2, 3 - i ) ;
			} // if
		} // for
		
		nibble = integerToString( sum ) ;
		
		if ( sum == 10 )
			nibble = "A" ;
		else if ( sum == 11 )
			nibble = "B" ;
		else if ( sum == 12 )
			nibble = "C" ;
		else if ( sum == 13 )
			nibble = "D" ;
		else if ( sum == 14 )
			nibble = "E" ;
		else if ( sum == 15 )
			nibble = "F" ;
	} // setNibble()
	
	string decimalObjectCodeToHexadecimal( string s, int tableNum ) {
		string final = "" ;
		
		int dividend = 0, remainder = 0 ;
		if ( ( tableNum == 6 ) && isBYTE ) {
			final = s ;
		} // if
		
		else if ( ( tableNum == 6 ) && isWORD ) {
			dividend = stringToInteger( s ) ;
			final = decimalNumberToHexadecimalString( dividend ) ;
		} // else if
		
		else if ( tableNum == 7 ) {
			string temp = "" ;
			for ( int i = 0 ; i < s.size() ; i = i + 2 ) {
				temp.push_back( s[i] ) ;
				temp.push_back( s[i+1] ) ;
				//cout << temp << "    " << i << "    " ;
				dividend = stringToInteger( temp ) ;
				final = final + decimalNumberToHexadecimalString( dividend ) ;
				temp = "" ;
			} // for
		} // else if
		
		return final ;
	} // decimalObjectCodeToHexadecimal()
	
	string decimalObjectCodeToHexadecimal_NbitString( string s ) {
		string final = "" ;
		int index = 0 ;
		int dividend = 0 ;
		string temp = "" ;
		for ( int i = 0 ; i < s.size() ; i = i + 2 ) {
			temp.push_back( s[i] ) ;
			temp.push_back( s[i+1] ) ;
			if ( digitsVector[index] == 3 ) {
				temp.push_back( s[i+2] ) ;
				i++ ;
			} // if
				
			//cout << temp << "    " << i << "    " ;
			dividend = stringToInteger( temp ) ;
			final = final + decimalNumberToHexadecimalString( dividend ) ;
			temp = "" ;
			index++ ;
		} // for
		
		while ( !digitsVector.empty() ) {
			digitsVector.pop_back() ;
		} // while
		
		vector<int>(digitsVector).swap(digitsVector) ;
		return final ;
	} // decimalObjectCodeToHexadecimal_Nbit()
	
	string decimalNumberToHexadecimalString( int dividend ) { // 10->16
		string final = "" ;
		string temp = "" ;
		int remainder = 0 ;
		while ( dividend != 0 ) {
			remainder = dividend % 16 ;
			temp = integerToString( remainder ) ;
			if ( remainder == 10 )
				temp = "A" ;
			else if ( remainder == 11 )
				temp = "B" ;
			else if ( remainder == 12 )
				temp = "C" ;
			else if ( remainder == 13 )
				temp = "D" ;
			else if ( remainder == 14 )
				temp = "E" ;
			else if ( remainder == 15 )
				temp = "F" ;
			final = temp + final ;
			dividend = dividend / 16 ;
			
		} // while
		
		return final ;
	} // decimalNumberToHexadecimal()
	
	void analysis_pass2( int i, int j ) {
		string s = tokenTable.table[i].list[j].name ;
		fixS( s, tokenTable.table[i].list[j].tableNum ) ;
		if ( tokenTable.table[i].list[j].tableNum == 1 ) { // 是可執行的指令 
			standardType.opcode = instructionSet[tokenTable.table[i].list[j].entryNum-1].opcode ; // 設定Opcode 
			instructionSetEntry = tokenTable.table[i].list[j].entryNum-1 ;
			nextLoc = nextLoc + 3 ;
		} // if
		
		else if ( tokenTable.table[i].list[j].tableNum == 2 ) { // 是Pseudo指令
			isPseudo = true ;
			if ( s == "START" ) {
				isSTART = true ;
				startLoc = stringToInteger_hex( tokenTable.table[i].list[j+1].name ) ; 
			} // if
			
			else if ( s == "BYTE" ) {
				isBYTE = true ;
				nextLoc = nextLoc + getByteSize( tokenTable.table[i].list[j+2].name, tokenTable.table[i].list[j+2].tableNum ) ;
				ASCIICode = getByteASCIICode( tokenTable.table[i].list[j+2].name, tokenTable.table[i].list[j+2].tableNum ) ;
				ASCIICode = decimalObjectCodeToHexadecimal( ASCIICode, tokenTable.table[i].list[j+2].tableNum ) ;
			} // else if
			
			else if ( s == "WORD" ) {
				isWORD = true ;
				nextLoc = nextLoc + 3 ; 
				ASCIICode = tokenTable.table[i].list[j+1].name ;
				ASCIICode = decimalObjectCodeToHexadecimal( ASCIICode, tokenTable.table[i].list[j+1].tableNum ) ;
				ASCIICode = string(6 - ASCIICode.size(), '0') + ASCIICode;
				//cout << "ASCIICode:" << ASCIICode << "    " << tokenTable.table[i].list[j+1].name << endl ;
			} // else if
			
			else if ( s == "RESB" ) {
				isRESB = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) ;
			} // else if
			
			else if ( s == "RESW" ) {
				isRESW = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) * 3 ;
			} // else if
			
			else if ( s == "END" ) {
				isEND = true ;
				setLiteralTableAddress() ;
			} // esle if
			
			else if ( s == "EQU" ) {
				locTempForEQU = stringToInteger( tokenTable.table[i].list[j+1].name ) ;
				isEQU = true ;
			} // esle if
			
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 3 ) { // 是Register 
			if ( operandNumber == instructionSet[instructionSetEntry].operand ) {
				if ( s == "X" )
					isIndexRegister = true ;
				else
					cout << "Index Register使用錯誤！！！" << endl ; 
			} // if
			
			operandNumber++ ;
			balance++ ;
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 4 ) { // 是Delimiter 
			if ( s == "." ) {
				isComment = true ;
			} //if
			
			if ( s == "," ) {
				balance = balance - 2 ;
			} // if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 5 ) { // 是Symbol Table 
			if ( ( symbolTable[tokenTable.table[i].list[j].entryNum].address == -1 ) && !isEND ) {
				cout << "Symbol未定義！！！" << endl ;	
			} // if
			
			if ( j == 0 ) { // 是該Row的第一個Token，也就是Symbol 
				;//symbolTable[tokenTable.table[i].list[j].entryNum].address = locTemp ; // 設定 SymbolTable的地址
			} // if
			
			else {
				standardType.address = symbolTable[tokenTable.table[i].list[j].entryNum].address ;
				operandNumber++ ;
				balance++ ;
			} // else
				
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 6 ) { // 是Integer Table 
			
			
			if ( ( j > 0 ) && ( tokenTable.table[i].list[j-1].name == "'/" )  ) {
				if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
					standardType.address = getAddressInLitteralTable( tokenTable.table[i].list[j].name ) ;
				} // if
				
				else {
					standardType.address = stringToInteger_hex( tokenTable.table[i].list[j].name ) ;
				} // else
				
			} // if
			
			else {
				if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
					standardType.address = getAddressInLitteralTable( tokenTable.table[i].list[j].name ) ;
				} // if
				
				else {
					standardType.address = stringToInteger( tokenTable.table[i].list[j].name ) ;
				} // else
				
			} // else
			
			operandNumber++ ;
			balance++ ;
			//setIntegerTable() ; // 設定 IntegerTable的地址 
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 7 ) { // 是String Table 
			operandNumber++ ;
			balance++ ;
			//setStringTable() ; // 設定 StringTable的地址 
		} // else if
	} // analysis_pass2()
	
	void setN( int & opcode, int n ) {
		int bit = opcode / 2  ; 
		bit = opcode % 2  ; 
		if ( ( bit == 0 ) && ( n == 1 ) ) {
			opcode = opcode + 2 ;
		} // if
		else if ( ( bit == 1 ) && ( n == 0 ) ) {
			opcode = opcode - 2 ;
		} // else if
	} // setN()
	
	void setI( int & opcode, int i ) {
		int bit = opcode % 2  ; 
		if ( ( bit == 0 ) && ( i == 1 ) ) {
			opcode = opcode + 1 ;
		} // if
		else if ( ( bit == 1 ) && ( i == 0 ) ) {
			opcode = opcode - 1 ;
		} // else if
	} // setI()
	
	void analysis_pass2_XE( int i, int j ) {
		if ( tokenTable.table[i].list[j].name == "LDT" ) {
			int a = 0 ;
			a = 1 ;
		} // if
		string s = tokenTable.table[i].list[j].name ;
		fixS( s, tokenTable.table[i].list[j].tableNum ) ;
		if ( tokenTable.table[i].list[j].tableNum == 1 ) { // 是可執行的指令 
			xeType.opcode = instructionSet[tokenTable.table[i].list[j].entryNum-1].opcode ; // 設定Opcode 
			xeType.format = instructionSet[tokenTable.table[i].list[j].entryNum-1].format ;
			instructionSetEntry = tokenTable.table[i].list[j].entryNum-1 ;
			if ( isFormat4( s, i, j ) ) {
				xeType.format = 4 ;
			} // if
			
			if ( xeType.format == 1 ) {
				nextLoc = nextLoc + 1 ;
			} // if
			
			else if ( xeType.format == 2 ) {
				nextLoc = nextLoc + 2 ;
			} // else if
			
			else if ( xeType.format == 3 ) {
				xeType.n = 1 ;
				xeType.i = 1 ;
				xeType.e = 0 ;
				setN( xeType.opcode, xeType.n ) ;
				setI( xeType.opcode, xeType.i ) ;
				nextLoc = nextLoc + 3 ;
			} // else if
			
			else if ( xeType.format == 4 ) {
				xeType.n = 1 ;
				xeType.i = 1 ;
				xeType.b = 0 ;
				xeType.p = 0 ;
				xeType.e = 1 ;
				setN( xeType.opcode, xeType.n ) ;
				setI( xeType.opcode, xeType.i ) ;
				nextLoc = nextLoc + 4 ;
			} // else if
		} // if
		
		else if ( tokenTable.table[i].list[j].tableNum == 2 ) { // 是Pseudo指令
			isPseudo = true ;
			if ( s == "START" ) {
				isSTART = true ;
				startLoc = stringToInteger_hex( tokenTable.table[i].list[j+1].name ) ; 
			} // if
			
			else if ( s == "BYTE" ) {
				isBYTE = true ;
				nextLoc = nextLoc + getByteSize( tokenTable.table[i].list[j+2].name, tokenTable.table[i].list[j+2].tableNum ) ;
				ASCIICode = getByteASCIICode( tokenTable.table[i].list[j+2].name, tokenTable.table[i].list[j+2].tableNum ) ;
				ASCIICode = decimalObjectCodeToHexadecimal( ASCIICode, tokenTable.table[i].list[j+2].tableNum ) ;
			} // else if
			
			else if ( s == "WORD" ) {
				isWORD = true ;
				nextLoc = nextLoc + 3 ; 
				ASCIICode = tokenTable.table[i].list[j+1].name ;
				ASCIICode = decimalObjectCodeToHexadecimal( ASCIICode, tokenTable.table[i].list[j+1].tableNum ) ;
				ASCIICode = string(6 - ASCIICode.size(), '0') + ASCIICode;
				//cout << "ASCIICode:" << ASCIICode << "    " << tokenTable.table[i].list[j+1].name << endl ;
			} // else if
			
			else if ( s == "RESB" ) {
				isRESB = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) ;
			} // else if
			
			else if ( s == "RESW" ) {
				isRESW = true ;
				nextLoc = nextLoc + stringToInteger( tokenTable.table[i].list[j+1].name ) * 3 ;
			} // else if
			
			else if ( s == "END" ) {
				isEND = true ;
				setLiteralTableAddress() ;
				for ( int k = 0 ; k < 100 ; k++ ) {
					literalTable[k].address = -1 ;
					literalTable[k].entryNum = -1 ;
					literalTable[k].name = "-1" ;
					literalTable[k].serialNum = -1 ;
					literalTable[k].tableNum = -1 ;
				} // for
			} // esle if
			
			else if ( s == "EQU" ) {
				locTempForEQU = stringToInteger( tokenTable.table[i].list[j+1].name ) ;
				isEQU = true ;
			} // esle if
			
			else if ( s == "BASE" ) {
				isBASE = true ;
				BASERegister = stringToInteger( tokenTable.table[i].list[j+1].name ) ;
			} // esle if
			
			else if ( s == "LTORG" ) {
				isLTORG = true ;
				setLiteralTableAddress() ;
				for ( int k = 0 ; k < 100 ; k++ ) {
					literalTable[k].address = -1 ;
					literalTable[k].entryNum = -1 ;
					literalTable[k].name = "-1" ;
					literalTable[k].serialNum = -1 ;
					literalTable[k].tableNum = -1 ;
				} // for
			} // esle if
			
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 3 ) { // 是Register 
			if ( xeType.format == 2 ) {
				if ( ( xeType.r1 == 0 ) && r1IsDone == false ) {
					xeType.r1 = tokenTable.table[i].list[j].entryNum - 1 ;
					r1IsDone = true ;
				}  // if
				
				else {
					xeType.r2 = tokenTable.table[i].list[j].entryNum - 1 ;
				} // else
			} // if
			
			else if ( xeType.format == 3 ) {
				if ( ( operandNumber == instructionSet[instructionSetEntry].operand ) && s == "X" )
					isIndexRegister = true ;
				else
					cout << "Index Register使用錯誤！！！" << endl ;
			} // else if
			operandNumber++ ;
			balance++ ;
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 4 ) { // 是Delimiter 
			if ( s == "." ) {
				isComment = true ;
			} //if
			
			if ( s == "," ) {
				balance = balance - 2 ;
			} // if
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 5 ) { // 是Symbol Table 
			if ( ( symbolTable[tokenTable.table[i].list[j].entryNum].address == -1 ) && !isEND ) {
				cout << "Symbol未定義！！！" << endl ;	
			} // if
			
			if ( isBASE ) {
				BASERegister = symbolTable[tokenTable.table[i].list[j].entryNum].address ;
				//cout << BASERegister ;
			} // if
				
			
			if ( j == 0 ) { // 是該Row的第一個Token，也就是Symbol 
				;
			} // if
			
			else {
				if ( tokenTable.table[i].list[j-1].name == "#" ) {
					xeType.i = 1 ;
					xeType.n = 0 ;
					setN( xeType.opcode, xeType.n ) ;
					setI( xeType.opcode, xeType.i ) ;
				} // if
				else if ( tokenTable.table[i].list[j-1].name == "@" ) {
					xeType.i = 0 ;
					xeType.n = 1 ;
					setN( xeType.opcode, xeType.n ) ;
					setI( xeType.opcode, xeType.i ) ;
				} // else if
				
				if ( xeType.format == 3 ) {
					xeType.b = 0 ;
					xeType.p = 1 ;
					int dispTemp = symbolTable[tokenTable.table[i].list[j].entryNum].address - nextLoc ;
					if ( dispTemp < -2048 ) {
						xeType.b = 1 ;
						xeType.p = 0 ;
						dispTemp = symbolTable[tokenTable.table[i].list[j].entryNum].address - BASERegister ;
					} // if
					
					xeType.disp = dispTemp ;
					//cout << xeType.disp << endl ;
				} // if
				
				else if ( xeType.format == 4 ) {
					xeType.address = symbolTable[tokenTable.table[i].list[j].entryNum].address ;
				} // else if
				
				operandNumber++ ;
				balance++ ;
			} // else
				
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 6 ) { // 是Integer Table
			
			if ( xeType.format == 2 ) {
				if ( ( xeType.r1 == 0 ) && !r1IsDone ) {
					xeType.r1 = stringToInteger( s ) ;
				} // if
				
				else {
					xeType.r2 = stringToInteger( s ) ;
				} // else
				
			} // if
			
			else if ( ( tokenTable.table[i].list[j-1].tableNum == 1 ) && ( xeType.format == 3 ) ) {
				//cout << tokenTable.table[i].list[j-1].name << endl ;
				xeType.i = 0 ;
				xeType.n = 0 ;
				xeType.b = 0 ;
				xeType.p = 0 ;
				int dispTemp = stringToInteger( s ) ;
				xeType.disp = dispTemp ;
			} // else if
			
			else if ( ( tokenTable.table[i].list[j-1].name == "#" ) && ( xeType.format == 3 ) ) {
				xeType.i = 1 ;
				xeType.n = 0 ;
				xeType.b = 0 ;
				xeType.p = 0 ;
				setN( xeType.opcode, xeType.n ) ;
				setI( xeType.opcode, xeType.i ) ;
				int dispTemp = stringToInteger( s ) ;
				xeType.disp = dispTemp ;
			} // if
			
			else if ( ( xeType.format == 3 ) && ( tokenTable.table[i].list[j-1].name != "#" ) ) {
				if ( ( j > 0 ) & ( tokenTable.table[i].list[j-1].name == "/'" ) ) {
					xeType.b = 0 ;
					xeType.p = 1 ;
					if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
						int dispTemp = findAddressInLiteralTable( s ) - nextLoc ;
						xeType.disp = dispTemp ;
					} // if
					
					else {
						int dispTemp = stringToInteger_hex( s ) ;
						xeType.disp = dispTemp ;
					} // else
					
					//cout << xeType.disp << endl ;
				} // if
				
				else {
					xeType.b = 0 ;
					xeType.p = 1 ;
					if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
						int dispTemp = findAddressInLiteralTable( s ) - nextLoc ;
						xeType.disp = dispTemp ;
					} // if
					
					else {
						int dispTemp = stringToInteger( s ) ;
						xeType.disp = dispTemp ;
					} // else
			
				} // else
				
			} // else if
			
			else if ( ( tokenTable.table[i].list[j-1].name == "#" ) && ( xeType.format == 4 ) ) {
				xeType.i = 1 ;
				xeType.n = 0 ;
				xeType.b = 0 ;
				xeType.p = 0 ;
				setN( xeType.opcode, xeType.n ) ;
				setI( xeType.opcode, xeType.i ) ;
				int addressTemp = stringToInteger( s ) ;
				xeType.address = addressTemp ;
			} // else if
			
			else if ( ( xeType.format == 4 ) && ( tokenTable.table[i].list[j-1].name != "#" ) ) {
				if ( ( j > 0 ) & ( tokenTable.table[i].list[j-1].name == "/'" ) ) { // 16進位的情況 
					
					if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) { // 取Literal地址的時候 
						xeType.b = 0 ;
						xeType.p = 0 ;
						int addressTemp = findAddressInLiteralTable( s ) - nextLoc ;
						xeType.address = addressTemp ;
					} // if
					
					else { 
						xeType.b = 0 ;
						xeType.p = 0 ;
						int addressTemp = stringToInteger_hex( s ) ;
						xeType.address = addressTemp ;
					} // else
					
					//cout << xeType.disp << endl ;
				} // if
				
				else { // 10進位的情況 
					
					if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {  // 取Literal地址的時候 
						xeType.b = 0 ;
						xeType.p = 0 ;
						int addressTemp = findAddressInLiteralTable( s ) ;
						xeType.address = addressTemp ;
					} // if
					
					else {
						xeType.b = 0 ;
						xeType.p = 0 ;
						int addressTemp = stringToInteger( s ) ;
						xeType.address = addressTemp ;
					} // else
			
				} // else
				
			} // else if
			
			operandNumber++ ;
			balance++ ; 
		} // else if
		
		else if ( tokenTable.table[i].list[j].tableNum == 7 ) { // 是String Table 
			if ( xeType.format == 3 ) {
				
				
				if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
					xeType.b = 0 ;
					xeType.p = 1 ;
					int dispTemp = findAddressInLiteralTable( s ) - nextLoc ;
					xeType.disp = dispTemp ;
				} // if
					
				else {
					;
				} // else
			} // if
			
			else if ( xeType.format == 4 ) {
				
				if ( ( j > 1 ) && ( tokenTable.table[i].list[j-2].name == "=" ) ) {
					xeType.b = 0 ;
					xeType.p = 0 ;
					int addressTemp = findAddressInLiteralTable( s ) ;
					xeType.address = addressTemp ;
				} // if
					
				else {
					;
				} // else
				
			} // else if
			operandNumber++ ;
			balance++ ;
		} // else if
	} // analysis_pass2_XE()
	
	int stringToInteger( string s ) { // 10->10
		int sum = 0 ;
		for ( int i = 0 ; i < s.size() ; i++ ) {
			sum = sum * 10 + s[i] - 48 ;
		} // for
		
		return sum ;
	} // stringToInteger()
	
	int stringToInteger_hex( string s ) { // 16->10
		int sum = 0 ;
		for ( int i = 0 ; i < s.size() ; i++ ) {
			sum = sum * 16 + s[i] - 48 ;
		} // for
		
		return sum ;
	} // stringToInteger()
	
	string integerToString(int x) {
    	stringstream ss ;
    	ss << x ;
   		return ss.str() ;
	} // integerToString
	
	string getByteASCIICode( string s, int tableNum ) {
		int temp = 0 ;
		string code = "" ;
		if ( tableNum == 6 ) {
			code = s ;
		} // if
		
		else if ( tableNum == 7 ) {
			for ( int i = 0 ; i < s.size() ; i++ ) {
				temp = s[i] ;
				howManyDigits( temp, digits, digitsVector ) ;
				code = code + integerToString( temp ) ;
			} // for
		} // else if
		
		return code ;
	} // getByteASCIICode()
	
	void howManyDigits( int temp, int & digits, vector<int> & digitsVector ) {
		int times = 0 ;
		while ( temp != 0 ) {
			temp = temp / 10 ;
			times++ ;
		} // while
		
		digits = times ;
		digitsVector.push_back( digits ) ;
	} // howManyDigits()
	
	void writeFile( string filename, vector<string> result ) { // 寫檔輸出 
		fstream outFile ;
		string name = "" ;
		if ( isXE == 0 )
			name = "_out.txt" ;
		else
			name = "_outXE.txt" ;
		outFile.open( ( filename + name ).c_str() , fstream::out ) ;
		if ( !outFile.is_open() ) {
			cout << "Error" << endl ;
			return ;
		} // if
		
		outFile << "Line\tLoc\tSource statement\t\tObject code" << endl ;
		for ( int i = 0 ; i < result.size() ; i++ ) {
			if ( assemblerList[i].line != -1 ) {
				outFile << assemblerList[i].line << "\t" ;
			} // if
			
			else {
				outFile << "\t" ;
			} // else
			
			if ( assemblerList[i].loc != -1 ) {
				outFile << setiosflags(ios::uppercase) << hex ;
				outFile << setfill('0') << setw(4) << assemblerList[i].loc << "\t" ;
				outFile << dec ;
			} // if
			
			else {
				outFile << "\t" ;
			} // else
			result[i].erase(result[i].end() - 1) ;
			outFile << result[i] << "\t\t" ;
			if ( assemblerList[i].objectCode != "-1" ) {
				outFile << assemblerList[i].objectCode << endl ;
			} // if
			
			else {
				outFile << endl ;
			} // else
			
			if ( hasENDOrLTORG( result[i] ) ) {
				int times = 0 ;
				for ( int j = 0 ; j < literalBufferTable.size() ; j++ ) {
					outFile << literalBufferTable[j].line << "\t" << setfill('0') << setw(4) << hex << literalBufferTable[j].loc << dec ;
					outFile << "\t" << literalBufferTable[j].statement << "\t" << literalBufferTable[j].ASCIICode << endl ;
					times++ ;
				} // for
				if ( ( i + 1 ) < result.size() ) {
					assemblerList[i+1].line = assemblerList[i+1].line + times*5 ;
				} // if
			} // if
		} // for
		
		outFile.close() ;
	} // writeFile()
	
	bool hasENDOrLTORG( string s ) {
		bool decide = false ;
		if ( s.find("END") != string::npos ) {
			decide = true ;
		} // if
		
		else if ( s.find("LTORG") != string::npos ) {
			decide = true ;
		} // else if
		
		else {
			decide = false ;
		} // else
		
		return decide ;
	} // hasEndOrLiteral()
	
	bool isFormat4( string s, int i, int j ) {
		bool decide = false ;
		if ( j > 0 ) {
			if ( tokenTable.table[i].list[j-1].name == "+" ) {
				decide = true ;
				isF4 = true ;
			}  // if
		} // if
		
		return decide ;
	} // isFormat4()
	
	string XETypeToObjectCode_1( XEType xeType ) {
		string s1 = "" ;
		int a = xeType.opcode ; 
		char opcode[8]; 
		for ( int i = 0 ; i < 8 ; i++ ) { // 進行初始化 
			opcode[i] = '\0' ;
		} // for

		itoa(a, opcode, 2); // 將數字轉換為2進位的字元陣列 
		for ( int i = 0 ; i < 8 ; i++ ) { // 將字元陣列轉換成2進位字串 
			if ( opcode[i] != '\0' ) {
				s1.push_back(opcode[i]) ;
			} // if
		} // for
		
		string new_opcode = string(8 - s1.size(), '0') + s1; // 位字串補上0 
		string final = new_opcode ; // 最後結果 (2進位) 
		final = binaryObjectCodeToHexadecimal( final ) ; // 將2進位的結果轉換成16進位 
		return final ;
	} // XETypeToObjectCode_1()
	
	string XETypeToObjectCode_2( XEType xeType ) {
		string s1 = "" ;
		string s2 = "" ;
		string s3 = "" ;
		
		int a = xeType.opcode ; 
		int b = xeType.r1 ;
		int c = xeType.r2 ;
		
		char opcode[8]; 
		char r1[4] ;
		char r2[4] ;
		for ( int i = 0 ; i < 8 ; i++ ) { // 進行初始化 
			opcode[i] = '\0' ;
		} // for
		for ( int i = 0 ; i < 4 ; i++ ) { // 進行初始化 
			r1[i] = '\0' ;
		} // for
		for ( int i = 0 ; i < 4 ; i++ ) { // 進行初始化 
			r2[i] = '\0' ;
		} // for

		itoa(a, opcode, 2); // 將數字轉換為2進位的字元陣列 
		for ( int i = 0 ; i < 8 ; i++ ) { // 將字元陣列轉換成2進位字串 
			if ( opcode[i] != '\0' ) {
				s1.push_back(opcode[i]) ;
			} // if
		} // for
		itoa(b, r1, 2); // 將數字轉換為2進位的字元陣列 
		for ( int i = 0 ; i < 4 ; i++ ) { // 將字元陣列轉換成2進位字串 
			if ( r1[i] != '\0' ) {
				s2.push_back(r1[i]) ;
			} // if
		} // for
		itoa(c, r2, 2); // 將數字轉換為2進位的字元陣列 
		for ( int i = 0 ; i < 4 ; i++ ) { // 將字元陣列轉換成2進位字串 
			if ( r2[i] != '\0' ) {
				s3.push_back(r2[i]) ;
			} // if
		} // for
		
		
		string new_opcode = string(8 - s1.size(), '0') + s1; // 位字串補上0 
		string new_r1 = string(4 - s2.size(), '0') + s2; // 位字串補上0 
		string new_r2 = string(4 - s3.size(), '0') + s3; // 位字串補上0 
		
		string final = new_opcode + new_r1 + new_r2 ; // 最後結果 (2進位) 
		final = binaryObjectCodeToHexadecimal( final ) ; // 將2進位的結果轉換成16進位 
		return final ;
	} // XETypeToObjectCode_2()
	
	string XETypeToObjectCode_3( XEType xeType ) {
		string s1 = "" ;
		string s2 = "" ;
		
		int a = xeType.opcode ; 
		int b = xeType.disp ;
		
		char opcode[8]; 
		char disp[32] ;

		for ( int i = 0 ; i < 8 ; i++ ) { // 進行初始化 
			opcode[i] = '\0' ;
		} // for
		for ( int i = 0 ; i < 32 ; i++ ) { // 進行初始化 
			disp[i] = '\0' ;
		} // for
		
		
		itoa(a, opcode, 2); // 將數字轉換為2進位的字元陣列 
		for ( int i = 0 ; i < 8 ; i++ ) { // 將字元陣列轉換成2進位字串 
			if ( opcode[i] != '\0' ) {
				s1.push_back(opcode[i]) ;
			} // if
		} // for
		if ( b > 0 ) {
			itoa(b, disp, 2); // 將數字轉換為2進位的字元陣列 
			for ( int i = 0 ; i < 12 ; i++ ) { // 將字元陣列轉換成2進位字串 
				if ( disp[i] != '\0' ) {
					s2.push_back(disp[i]) ;
				} // if
			} // for
		} // if
		else {
			itoa(b, disp, 2); // 將數字轉換為2進位的字元陣列
			for ( int i = 20 ; i < 32 ; i++ ) { // 將字元陣列轉換成2進位字串 
				if ( disp[i] != '\0' ) {
					s2.push_back(disp[i]) ;
				} // if
			} // for
		} // else
		
		
		string new_opcode = string(8 - s1.size(), '0') + s1; // 位字串補上0 
		string new_disp = string(12 - s2.size(), '0') + s2; // 位字串補上0 
		
		string final = new_opcode + integerToString( xeType.x ) + integerToString( xeType.b ) + integerToString( xeType.p ) + integerToString( xeType.e ) + new_disp ; // 最後結果 (2進位) 
		final = binaryObjectCodeToHexadecimal( final ) ; // 將2進位的結果轉換成16進位 
		return final ;
	} // XETypeToObjectCode_3()
	
	string XETypeToObjectCode_4( XEType xeType ) {
		string s1 = "" ;
		string s2 = "" ;
		
		int a = xeType.opcode ; 
		int b = xeType.address ;
		
		char opcode[8]; 
		char address[20] ;

		for ( int i = 0 ; i < 8 ; i++ ) { // 進行初始化 
			opcode[i] = '\0' ;
		} // for
		for ( int i = 0 ; i < 20 ; i++ ) { // 進行初始化 
			address[i] = '\0' ;
		} // for
		
		itoa(a, opcode, 2); // 將數字轉換為2進位的字元陣列 
		for ( int i = 0 ; i < 8 ; i++ ) { // 將字元陣列轉換成2進位字串 
			if ( opcode[i] != '\0' ) {
				s1.push_back(opcode[i]) ;
			} // if
		} // for
		itoa(b, address, 2); // 將數字轉換為2進位的字元陣列 
		for ( int i = 0 ; i < 20 ; i++ ) { // 將字元陣列轉換成2進位字串 
			if ( address[i] != '\0' ) {
				s2.push_back(address[i]) ;
			} // if
		} // for
		
		string new_opcode = string(8 - s1.size(), '0') + s1; // 位字串補上0 
		string new_address = string(20 - s2.size(), '0') + s2; // 位字串補上0 
		
		//new_opcode[6] = '1' ;
		//new_opcode[7] = '1' ;
		string final = new_opcode + integerToString( xeType.x ) + integerToString( xeType.b ) + integerToString( xeType.p ) + integerToString( xeType.e ) + new_address ; // 最後結果 (2進位) 
		final = binaryObjectCodeToHexadecimal( final ) ; // 將2進位的結果轉換成16進位 
		return final ;
	} // XETypeToObjectCode_4()
	
	void setassemblerList_XE( vector<AssemblerList> & assemblerList, AssemblerList row, int i ) {
		assemblerList[i].objectCode = row.objectCode ;
	} // setassemblerList()
	
	int getAddressInLitteralTable( string name ) {
		int num = 0 ;
		for ( int i = 0 ; i < 100 ; i++ ) {
			if ( name == literalTable[i].name ) {
				num = literalTable[i].address ;
			} // if
		} // for
		
		return num ;
	} // getAddressInLitteralTable()
};

int main() {
start:	
	SICStructure sicStructure ;
	cout << "Please input a filename[此為SIC格式，輸入檔案時不需要輸入副檔名，輸入0代表離開]: " ;
	cin >> sicStructure.filename ;
	
	
	if ( sicStructure.filename == "0" )
		return 0 ;
	bool successReadInput = sicStructure.readInResult() ; // 將輸入檔轉存為資料結構，使用vector來儲存 
	if ( !successReadInput ) {
		goto start ;
	} // if
	bool successReadTable = sicStructure.setTables() ; // 將Table1到Table4讀進資料結構，使用vector來儲存 
	if ( !successReadTable ) {
		goto start ;
	} // if
	sicStructure.initializeTable() ; // 將Table5到Table7做初始化，使用大小100的陣列來儲存 
	sicStructure.getTokens() ; // 取得所有的Token，並設定對應的Table 
	//sicStructure.writeFile() ;
	//sicStructure.printAnswer() ;
	SICAssembler sicAssembler ;
	cout << "是SIC(輸入0)還是SICXE(輸入1)？" ;
	cin >> sicAssembler.isXE ;
	sicAssembler.setTables( sicStructure.table1, sicStructure.table2, sicStructure.table3, sicStructure.table4, sicStructure.symbolTable, sicStructure.integerTable, sicStructure.stringTable ) ;
	sicAssembler.setInstructureSet() ;
	sicAssembler.setTokenTables( sicStructure.answerTable ) ;
	sicAssembler.pass1() ;
	/*
	for ( int i = 0 ; i < sicAssembler.tokenTable.table.size() ; i++ ) {
		cout << sicAssembler.assemblerList[i].line << "   " << hex << sicAssembler.assemblerList[i].loc << dec << "   " << sicAssembler.assemblerList[i].format << endl ;
	} // for
	*/
	/*
	for ( int i = 0 ; i < 100 ; i++ ) {
		cout << sicAssembler.literalTable[i].name << sicAssembler.literalTable[i].address << endl ;
	} // for
	
	for ( int i = 0 ; i < sicAssembler.literalBufferTable.size() ; i++ ) {
		cout << sicAssembler.literalBufferTable[i].line << "\t" << sicAssembler.literalBufferTable[i].loc << "\t" << sicAssembler.literalBufferTable[i].statement << "\t" << sicAssembler.literalBufferTable[i].ASCIICode << endl ;
	} // for
	*/
	sicAssembler.lineTemp = 0 ;
	sicAssembler.nextLoc = 0x0000 ;
	sicAssembler.startLoc = 0x0000 ;
	sicAssembler.locTemp = 0x0000 ;
	sicAssembler.locTempForEQU = 0x0000 ;
	sicAssembler.pass2() ;
	sicAssembler.writeFile( sicStructure.filename ,sicStructure.result ) ;
	goto start ;
	return 0 ;
}  // main

