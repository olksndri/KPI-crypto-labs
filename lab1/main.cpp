#include "include.h"

using namespace std; 

int main(int, char**)
{
	std::setlocale(LC_ALL, ""); 
	
	std::locale::global(std::locale(""));
	std::wcin.imbue(std::locale(""));
	std::wcout.imbue(std::locale(""));
	
	wstring str_a = L"лабораторна робота номер один"; 
	wstring str_b = L"тимошенко о і";
	wstring str_c = L"метою лабораторної роботи номер один є отримання базових знань із \
криптографії та криптоаналізу шляхом розробки та застосування інструментарію для \
шифрування та дешифрування повідомлень лабораторну роботу виконав тимошенко о і \
лабораторна робота виконана мовою с плюс плюс відповідно варіанту \
завдання"; 
	
	task_1(str_a, str_b, str_c); 

	task_2(str_a, str_b, str_c); 
	
	task_3(str_a, str_b, str_c); 

	return 0; 
}
