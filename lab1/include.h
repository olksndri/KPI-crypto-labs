#include <iostream>
#include <string>
#include <clocale>
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <map>
#include <cmath>
#include <random>

#define VARIANT						(6)
#define SURNAME						(L"тим")

#define TXT_RED 		"\033[0;31m"
#define TXT_GREEN 		"\033[0;32m"
#define TXT_YELLOW 		"\033[0;33m"
#define TXT_BLUE 		"\033[0;34m"
#define TXT_END_COLOR 	"\033[0m"

extern std::wstring analysis_alphabet; 

extern std::wstring alphabet;  

extern wchar_t ukr_common[33]; 

extern std::map<wchar_t, double> ukr_freqs; 

void task_1(std::wstring str_a, std::wstring str_b, std::wstring str_c);

void task_2(std::wstring str_a, std::wstring str_b, std::wstring str_c); 

void task_3(std::wstring str_a, std::wstring str_b, std::wstring str_c); 
