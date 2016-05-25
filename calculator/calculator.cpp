#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <vector>
using namespace std;

/*
 *单词种别，分别为不能识别的符号，标识符，整数，
 *浮点数，保留字int、float、write，
 *运算符号加减乘除，等号
 *左右括号，逗号、分号、句号。
 */
enum symbols 
{
	Nul, Id, IntNum, FloatNum, 
	Int, Float, Write, Add, 
	Sub, Mul, Div, Assign, 
	Lparen, Rparen, Comma, Semicolon, Period
};

string symbol_strs[] =
{
	"Nul", "Id", "IntNum", "FloatNum",
	"Int", "Float", "Write", "Add",
	"Sub", "Mul", "Div", "Assign",
	"Lparen", "Rparen", "Comma", "Semicolon", "Period"
};

string source;                                    // 源文件
string token_value;                               // 标识符的名字
int symbol;                                       // token类别
int int_value;                                    // int型变量的值
float float_value;                                // float型变量的值
int line = 1;                                     // 行号
int postion = 1;                                  // 行内位置
int index = 0;                                    // 当前字符下标
string current_char;                              // 当前字符
bool source_end = false;
map<string, float> float_vars;
map<string, int> int_vars;
map<string, int> ids;

void next();
// 表达式计算
double expr();

// 获取下一个字符
void get_next_char() 
{
	if (index != source.size()) 
	{
		current_char = source[index];
		index++;
		postion++;
		cout << "get_next_char(): " << current_char << "    symbol: " << symbol_strs[symbol] << endl;
	} 
	else 
	{
		source_end = true;
	}
}

// 词法分析
void token_parser() 
{
	if (current_char.c_str()[0] == '\n') 
	{
		line++;
		postion = 1;
		get_next_char();
		next();
	}
	else if (current_char == " ")
	{
		get_next_char();
		next();
	}
	// 保留字和标识符的分析
	else if (isalpha(current_char.c_str()[0])) 
	{
		cout << "first step in parser word, current char: " << current_char << endl;
		string temp_word = current_char;
		get_next_char();
		while (isalnum(current_char.c_str()[0])) 
		{
			temp_word += current_char;
			get_next_char();
		}
		cout << "temp word: " << temp_word << endl;
		if (temp_word == "float")
		{
			symbol = Float;
			return;
		}
		else if (temp_word == "int")
		{
			symbol = Int;
			return;
		}
		else if (temp_word == "write")
		{
			symbol = Write;
			return;
		}
		else
		{
			cout << "before be Id, temp word: " << temp_word << " symbol:"<< symbol_strs[symbol]<<endl;
			symbol = Id;
			cout << "after be Id, symbol: " << symbol_strs[symbol] << endl;
			token_value = temp_word;
			return;
		}
	}
	// 数字的分析
	else if (isdigit(current_char.c_str()[0]))
	{
		string temp_number = current_char;
		cout << "print from number parser before get_next_char()" << endl;
		get_next_char();
		while (isdigit(current_char.c_str()[0]) || current_char == ".")
		{
			temp_number += current_char;
			get_next_char();
		}
		cout << "print from number parser and parser end.symbol: "<<symbol_strs[symbol] << endl;
		if (temp_number.find(".") != string::npos)
		{
			symbol = FloatNum;
			float_value = atof(temp_number.c_str());
			return;
		}
		else 
		{
			symbol = IntNum;
			int_value = atoi(temp_number.c_str());
			return;
		}
	}
	else if (current_char == "+")
	{
		symbol = Add;
		get_next_char();
		return;
	}
	else if (current_char == "-")
	{
		symbol = Sub;
		get_next_char();
		return;
	}
	else if (current_char == "*")
	{
		symbol = Mul;
		get_next_char();
		return;
	}
	else if (current_char == "/")
	{
		symbol = Div;
		get_next_char();
		return;
	}
	else if (current_char == "=")
	{
		symbol = Assign;
		get_next_char();
		return;
	}
	else if (current_char == "(")
	{
		symbol = Lparen;
		get_next_char();
		return;
	}
	else if (current_char == ")")
	{
		symbol = Rparen;
		cout << "print in token_parser and current_char: " << current_char << " symbol:" << symbol_strs[symbol] << endl;
		get_next_char();
		return;
	}
	else if (current_char == ",")
	{
		symbol = Comma;
		get_next_char();
		return;
	}
	else if (current_char == ";")
	{
		symbol = Semicolon;
		get_next_char();
		return;
	}
	else if (current_char == ".")
	{
		symbol = Period;
		exit(0);
		return;
	}
	else
	{
		get_next_char();
		next();
		return;
	}
}

void match(int expectSymbol)
{
	if (symbol != expectSymbol)
	{
		//printf("line%d, position %d: unexcepted token!%s, excpet: %s\n", line, postion, symbol_strs[symbol], symbol_strs[expectSymbol]);
		system("pause");
		exit(1);
	}
	else
	{
		token_parser();
	}
}

void next()
{
	token_parser();
}

void init()
{
	get_next_char();
}

// 变量声明
void var_declaration()
{
	while (symbol == Float || symbol == Int) 
	{
		if (symbol == Float)
		{
			cout << "befor match Float in var_declaration(), symbol: " << symbol_strs[symbol] << endl;
			match(Float);
			cout << "match Float success, symbol: " << symbol_strs[symbol] << endl;
			match(Id);
			cout << "match Id success, symbol: " << symbol_strs[symbol] << endl;
			if (ids.find(token_value) != ids.end())
			{
				printf("line%d, position%d: variable \"%s\" had been declared!", line, postion, token_value);
				exit(2);
			}
			ids.insert(pair<string, int>(token_value, Float));
			match(Semicolon);
		}
		else if (symbol == Int)
		{
			match(Int);
			match(Id);
			if (ids.find(token_value) != ids.end())
			{
				printf("line%d, position%d: variable \"%s\" had been declared!", line, postion, token_value);
				exit(2);
			}
			ids.insert(pair<string, int>(token_value, Int));
			match(Semicolon);
		}
	}
}

double factor()
{
	double result;
	if (symbol == Lparen)
	{
		match(Lparen);
		result = expr();
		cout << "error" << endl;
		match(Rparen);
	}
	else if (symbol == FloatNum)
	{
		result = float_value;
		match(FloatNum);
	}
	else if(symbol == IntNum)
	{
		result = int_value;
		match(IntNum);
	}
	else if (symbol == Id)
	{
		if (float_vars.find(token_value) != float_vars.end())
		{
			auto res = float_vars.find(token_value);
			result = res->second;
			match(Id);
		}
		else if (int_vars.find(token_value) != int_vars.end())
		{
			auto res = int_vars.find(token_value);
			result = res->second;
			match(Id);
		}
		else
		{
			printf("line%d, undefined variable %s", line, token_value);
			exit(3);
		}
	}
	return result;
}

double term_tail(double lvalue) 
{
	if (symbol == Mul) 
	{
		cout << "print from get term_tail and there symbol should be Mul, symbol: "<<symbol_strs[symbol] << endl;
		match(Mul);
		double value = lvalue * factor();
		return term_tail(value);
	}
	else if (symbol == Div) 
	{
		match(Div);
		double value = lvalue / factor();
		return term_tail(value);
	}
	else 
	{
		return lvalue;
	}
}

double term() 
{
	double lvalue = factor();
	return term_tail(lvalue);
}

double expr_tail(double lvalue) 
{
	if (symbol == Add) 
	{
		match(Add);
		double value = lvalue + term();
		return expr_tail(value);
	}
	else if (symbol == Sub) 
	{
		match(Sub);
		double value = lvalue - term();
		return expr_tail(value);
	}
	else 
	{
		return lvalue;
	}
}

double expr() 
{
	double lvalue = term();
	double result = expr_tail(lvalue);
	next();
	return result;
}

// 语法分析
void program()
{
	init();
	next();    // 获取第一个token
	var_declaration();
	cout << "var declaration end!" << endl;
	// 变量赋值
	do
	{
		match(Id);
		string temp_name = token_value;
		if (ids.find(token_value) == ids.end())
		{
			printf("line%d, undefined variable %s", line, token_value);
			exit(3);
		}
		match(Assign);
		double result = expr();
		auto iter = ids.find(token_value);
		if (iter->second == Float)
		{
			float_vars.insert(pair<string, float>(temp_name, result));
		}
		else
		{
			int_vars.insert(pair<string, float>(temp_name, result));
		}
	} while (symbol == Id);
	
	
	do 
	{
		if (symbol == Write)
		{
			match(Write);
			match(Lparen);
			if (symbol == Id)
			{
				if (ids.find(token_value) != ids.end())
				{
					printf("line%d, undefined variable %s", line, token_value);
					exit(3);
				}
				auto iter = ids.find(token_value);
				if (iter->second == Float)
				{
					cout << float_vars.find(token_value)->second << endl;
				}
				else
				{
					cout << int_vars.find(token_value)->second << endl;
				}
			}
			next();
			match(Rparen);
		}
	} while (symbol != Period);
}

int main(int argc, char* argv[])
{
	source = "float a; int b; a = (10.44*356+1.28) / 2 + 1024 * 1.6;b = a * 2 – c/2;write(b).";
	cout << source << endl;
	program();
	system("pause");
	return 0;
}