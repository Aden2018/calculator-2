#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

/*
 *�����ֱ𣬷ֱ�Ϊ����ʶ��ķ��ţ���ʶ����������
 *��������������int��float��write��
 *������żӼ��˳����Ⱥ�
 *�������ţ����š��ֺš���š�
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

string source;                                    // Դ�ļ�
string token_value;                               // ��ʶ��������
int symbol;                                       // token���
int int_value;                                    // int�ͱ�����ֵ
float float_value;                                // float�ͱ�����ֵ
int line = 1;                                     // �к�
int index = 0;                                    // ��ǰ�ַ��±�
string current_char;                              // ��ǰ�ַ�
bool source_end = false;
map<string, float> float_vars;
map<string, int> int_vars;
map<string, int> ids;
ofstream out;

void next();
// ���ʽ����
double expr();

// ��ȡ��һ���ַ�
void get_next_char() 
{
	if (index != source.size()) 
	{
		current_char = source[index];
		index++;
	} 
	else 
	{
		source_end = true;
	}
}

// �ʷ�����
void token_parser() 
{
	if (current_char.c_str()[0] == '\n') 
	{
		line++;
		get_next_char();
		next();
	}
	else if (current_char == " ")
	{
		get_next_char();
		next();
	}
	// �����ֺͱ�ʶ���ķ���
	else if (isalpha(current_char.c_str()[0])) 
	{
		string temp_word = current_char;
		get_next_char();
		while (isalnum(current_char.c_str()[0])) 
		{
			temp_word += current_char;
			get_next_char();
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
	// ���ֵķ���
	else if (isdigit(current_char.c_str()[0]))
	{
		string temp_number = current_char;
		get_next_char();
		while (isdigit(current_char.c_str()[0]) || current_char == ".")
		{
			temp_number += current_char;
			get_next_char();
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
		cout << "(error)line" << line << ", unexpected token \"" << symbol_strs[symbol] << "\"" << endl;
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

// ��������
void var_declaration()
{
	while (symbol == Float || symbol == Int) 
	{
		if (symbol == Float)
		{
			match(Float);
			match(Id);
			// �����ids�У�����ʾ�ñ����Ѿ�����
			if (ids.find(token_value) != ids.end())	
			{
				cout << "(error)line " << line << ", variable " << token_value << " had been defined." << endl;
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
				cout << "(error)line " << line << ", variable " << token_value << " had been defined." << endl;
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
			cout << "(error)line " << line << ", undefined variable " << token_value << "." << endl;
			exit(3);
		}
	}
	return result;
}

double term_tail(double lvalue) 
{
	if (symbol == Mul) 
	{
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
	return result;
}

// �﷨����
void program()
{
	init();
	next();    // ��ȡ��һ��token
	var_declaration();
	
	// ������ֵ
	do
	{
		match(Id);
		string temp_name = token_value;
		if (ids.find(token_value) == ids.end())
		{
			cout << "(error)line " << line << ", undefined variable " << token_value << "." << endl;
			exit(3);
		}
		match(Assign);
		double result = expr();
		auto iter = ids.find(temp_name);
		if (iter->second == Float)
		{
			float_vars.insert(pair<string, float>(temp_name, result));
		}
		else
		{
			int_vars.insert(pair<string, float>(temp_name, result));
		}
		match(Semicolon);
	} while (symbol == Id);
	
	
	do 
	{
		if (symbol == Write)
		{
			match(Write);
			match(Lparen);
			if (symbol == Id)
			{
				// �������ids�У�����ʾ�ñ���δ����
				if (ids.find(token_value) == ids.end())
				{
					cout << "(error)line " << line << ", undefined variable " << token_value << "." << endl;
					exit(3);
				}
				auto iter = ids.find(token_value);
				if (iter->second == Float)
				{
					cout <<token_value<<": "<<float_vars.find(token_value)->second << endl;
					out << token_value << ": " << float_vars.find(token_value)->second << endl;
				}
				else
				{
					cout << token_value << ": " << (int) int_vars.find(token_value)->second << endl;
					out << token_value << ": " << (int)int_vars.find(token_value)->second << endl;
				}
				next();
			}
			match(Rparen);
			next();
		}
	} while (symbol != Period);
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "3 arguments needed." << endl;
		exit(4);
	}
	
	ifstream in(argv[1]);
	char buffer[256];
	if (!in.is_open())
	{
		cout << "file " << argv[1] << " not exist.";
		exit(5);
	}
	while (!in.eof())
	{
		in.getline(buffer, 100);
		source += buffer;
		source += '\n';
	}
	
	source = source.substr(0, source.size() - 2);
	if (source[source.size() - 1] != '.')
	{
		cout << "the end of the source should be \".\"" << endl;
		exit(6);
	}
	
	out = ofstream(argv[2]);
	program();
	cout << "write to " << argv[2] << " success." << endl;
	out.close();
	return 0;
}