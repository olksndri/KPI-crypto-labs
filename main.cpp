#include <iostream>
#include <string>
#include <clocale>

using namespace std; 

#define VARIANT		(6)

#define TXT_RED 		"\033[0;31m"
#define TXT_GREEN 		"\033[0;32m"
#define TXT_YELLOW 		"\033[0;33m"
#define TXT_BLUE 		"\033[0;34m"
#define TXT_END_COLOR 	"\033[0m"

wstring ukr_alphabet = L"АаБбВвГгҐґДдЕеЄєЖжЗзИиІіЇїЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщьЮюЯя .,;_-+=)(*&^%$#@!~'\"\\/)1234567890"; 

wstring encrypt_caesar(wstring msg, int key) 
{   
	int msg_len = msg.length(); 
	int alph_len = ukr_alphabet.length(); 

	wstring encr_msg(msg_len, '*');

	for(int i = 0; i < msg_len; i++)
	{ 
		int alph_idx = ukr_alphabet.find(msg[i]); 
		int shft_idx = alph_idx + key;
		if (shft_idx>(alph_len-1)) shft_idx -= alph_len;  
		encr_msg[i] = ukr_alphabet[shft_idx]; 
	}

	return encr_msg; 
}

wstring decrypt_caesar(wstring msg, int key)
{ 
	int msg_len = msg.length(); 
	int alph_len = ukr_alphabet.length(); 

	wstring decr_msg(msg_len, '*'); 

	for(int i = 0; i < msg_len; i++)
	{ 
		int alph_idx = ukr_alphabet.find(msg[i]); 
		int shft_idx = alph_idx - key;
		if (shft_idx<0) shft_idx += alph_len;  
		decr_msg[i] = ukr_alphabet[shft_idx]; 
	}

	return decr_msg; 
}


void print_task_1_2(wstring str, int key) 
{ 
	wstring encr_message; 
	wstring decr_message; 

	encr_message = encrypt_caesar(str, key);  
	decr_message = decrypt_caesar(encr_message, key); 
	wcout << TXT_YELLOW << L"message: " << TXT_END_COLOR << str << "\n"; 
	wcout << TXT_RED << L"encrypted message: " << TXT_END_COLOR <<  encr_message << "\n"; 
	wcout << TXT_GREEN << L"decrypted message: " << TXT_END_COLOR <<  decr_message << "\n"; 
	wcout << "\n"; 
}

void task_1_2() 
{
	wstring str_a = L"лабораторна робота номер один"; 
	wstring str_b = L"Тимошенко О. І.";
	wstring str_c = L"метою лабораторної роботи номер один є отримання базових знань із \
криптографії та криптоаналізу шляхом розробки та застосування інструментарію для \
шифрування та дешифрування повідомлень. лабораторну роботу виконав Тимошенко О. І. \
лабораторна робота виконана мовою С++ відповідно варіанту \
завдання"; 

	int key = VARIANT; // Порядковий номер у списку студентів
	
	wcout << "\n"; 

	print_task_1_2(str_a, key);  
	print_task_1_2(str_b, key);  
	print_task_1_2(str_c, key);  
}

int main(int, char**)
{
	std::setlocale(LC_ALL, ""); 
	
	std::locale::global(std::locale(""));
	std::wcin.imbue(std::locale(""));
	std::wcout.imbue(std::locale(""));
	
	task_1_2(); 

	return 0; 
}
