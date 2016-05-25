#include <iostream>
#include <string>
#include <cctype>
#include <map>
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

string source;                                    // 源文件
string words[3] = { "float", "int", "write" };    // 保留字
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


// 获取下一个字符
void get_next_char() 
{
	if (index != source.size()) 
	{
		current_char = source[index];
		index++;
		postion++;
	}
	else 
	{
		source_end = true;
	}
}

void match(int expectSymbol)
{
	if (symbol != expectSymbol)
	{
		printf("line%d, position%d: unexcepted token!", line, postion);
		exit(1);
	}
	else
	{
		token_parser();
	}
}

// 词法分析
void token_parser() 
{
	if (current_char == "\n") 
	{
		line++;
		postion = 1;
	}
	else if (current_char == " ")
	{
		get_next_char();
	}
	// 保留字和标识符的分析
	else if (isalpha(current_char.c_str()[0])) 
	{
		string temp_word = current_char;
		get_next_char();
		while (isalnum(current_char.c_str()[0])) 
		{
			temp_word += current_char;
			get_next_char();
			if (source_end)
			{
				break;
			}
		}
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
			symbol = Id;
			token_value = temp_word;
			return;
		}
	}
	// 数字的分析
	else if (isdigit(current_char.c_str()[0]))
	{
		string temp_number = current_char;
		get_next_char();
		while (isdigit(current_char.c_str()[0]) || current_char == ".")
		{
			temp_number += current_char;
			get_next_char();
			if (source_end)
			{
				break;
			}
		}
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
		return;
	}
	else if (current_char == "-")
	{
		symbol = Sub;
		return;
	}
	else if (current_char == "*")
	{
		symbol = Mul;
		return;
	}
	else if (current_char == "/")
	{
		symbol = Div;
		return;
	}
	else if (current_char == "=")
	{
		symbol = Assign;
		return;
	}
	else if (current_char == "(")
	{
		symbol = Lparen;
		return;
	}
	else if (current_char == ")")
	{
		symbol = Rparen;
		return;
	}
	else if (current_char == ",")
	{
		symbol = Comma;
		return;
	}
	else if (current_char == ";")
	{
		symbol = Semicolon;
		return;
	}
	else if (current_char == ".")
	{
		symbol = Period;
		return;
	}
	else
	{
		symbol = Nul;
		return;
	}
}

void init()
{
	get_next_char();
	token_parser();
}

// 变量声明
void var_declaration()
{
	while (symbol == Float || symbol == Int) 
	{
		if (symbol == Float)
		{
			match(Float);
			match(Id);
			if (float_vars.find(token_value) != float_vars.end() || int_vars.find(token_value) != int_vars.end())
			{
				printf("line%d, position%d: variable \"%s\" had been declared!", line, postion, token_value);
				exit(2);
			}
			float_vars.insert(pair<string, int>(token_value, float_value));
			match(Semicolon);
		}
		else if (symbol == Int)
		{
			match(Int);
			match(Id);
			if (float_vars.find(token_value) != float_vars.end() || int_vars.find(token_value) != int_vars.end())
			{
				printf("line%d, position%d: variable \"%s\" had been declared!", line, postion, token_value);
				exit(2);
			}
			int_vars.insert(pair<string, int>(token_value, int_value));
			match(Semicolon);
		}
	}
}

// 语法分析
void program()
{
	init();
}

int main(int argc, char* argv[])
{
	system("pause");
	return 0;
}