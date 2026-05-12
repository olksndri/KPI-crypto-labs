#include "../include.h"

using namespace std; 

wstring encrypt_caesar(wstring msg, int key) 
{   
	int msg_len = msg.length(); 
	int alph_len = alphabet.length(); 

	wstring encr_msg(msg_len, '*');

	for(int i = 0; i < msg_len; i++)
	{ 
		if (msg[i] == L' ') {
            encr_msg[i] = L' ';
            continue;
        }
		int alph_idx = alphabet.find(msg[i]); 
		int shft_idx = alph_idx + key;
		if (shft_idx>(alph_len-1)) shft_idx -= alph_len;  
		encr_msg[i] = alphabet[shft_idx]; 
	}

	return encr_msg; 
}

wstring decrypt_caesar(wstring msg, int key)
{ 
	int msg_len = msg.length(); 
	int alph_len = alphabet.length(); 

	wstring decr_msg(msg_len, '*'); 

	for(int i = 0; i < msg_len; i++)
	{ 
		if (msg[i] == L' ') {
            decr_msg[i] = L' ';
            continue;
        }
		int alph_idx = alphabet.find(msg[i]); 
		int shft_idx = alph_idx - key;
		if (shft_idx<0) shft_idx += alph_len;  
		decr_msg[i] = alphabet[shft_idx]; 
	}

	return decr_msg; 
}

wstring brutforce_caesar(wstring msg, wstring control_word, int *match_key)
{ 
	int alph_len = alphabet.length(); 

	for(int key = 0; key < alph_len; key++) 
	{
		wstring attempt = decrypt_caesar(msg, key); 
		if(attempt.find(control_word) != wstring::npos) 
		{ 
			*match_key = key; 
			return attempt; 
		}
	}

	*match_key = -1; 
	return L"not decrypted";
}	

wstring freq_analysis_caesar(wstring msg, wstring control_word, int *match_key, int common_letters_count)
{ 
	int msg_len = msg.length(); 
	int alph_len = alphabet.length(); 

	vector<int> freq_counters(alph_len, 0);

	for(wchar_t c : msg) {
        size_t idx = alphabet.find(c);
        if (idx != wstring::npos) freq_counters[idx]++;
    }

	// Створюємо пари {частота, індекс}, щоб легко відсортувати
    vector<pair<int, int>> sorted_freqs;
    for(int i = 0; i < alph_len; i++) {
        if (freq_counters[i] > 0)
            sorted_freqs.push_back({freq_counters[i], i});
    }
    
    // Сортуємо за спаданням частоти
    sort(sorted_freqs.rbegin(), sorted_freqs.rend());

    // Беремо топ-5 (або менше, якщо літер в тексті мало)
    int top_n = min((int)sorted_freqs.size(), 5);

    for(int i = 0; i < common_letters_count; i++) {
        for(int f = 0; f < top_n; f++) {
            int encrypt_idx = sorted_freqs[f].second;
            int orig_idx = alphabet.find(ukr_common[i]);
            
            if (orig_idx == wstring::npos) continue;

            int key = (encrypt_idx - orig_idx + alph_len) % alph_len; 
        
            wstring attempt = decrypt_caesar(msg, key); 
            if(attempt.find(control_word) != wstring::npos) 
            { 
                *match_key = key; 
                return attempt; 
            } 
        }
    }

	*match_key = -1; 
	return L"not decrypted";
}	


void print_task_1(wstring str, wstring control_word, int key) 
{
	int match_key_brut; 
	int match_key_freq; 

	int common_letters_count = 33; 

	wstring encr_message = encrypt_caesar(str, key);  
	wstring decr_message = decrypt_caesar(encr_message, key); 
	wstring decr_brutforce_message = brutforce_caesar(encr_message, control_word, &match_key_brut); 
	wstring decr_freqanalysis_message = freq_analysis_caesar(encr_message, control_word, &match_key_freq, common_letters_count);

	wcout << TXT_YELLOW << L"message: " << TXT_END_COLOR << str << "\n"; 
	wcout << TXT_RED << L"encrypted caesar message: " << TXT_END_COLOR <<  encr_message << "\n"; 
	wcout << TXT_GREEN << L"decrypted caesar message: " << TXT_END_COLOR <<  decr_message << "\n"; 
	wcout << TXT_GREEN << L"Brutforce attack, matched key: " << TXT_END_COLOR << match_key_brut << "\n"; 
	wcout << TXT_GREEN << L"Frequency analysis attack, matched key: " << TXT_END_COLOR << match_key_freq << "\n"; 
	wcout << "\n"; 
}

void task_1(wstring str_a, wstring str_b, wstring str_c) 
{
	wstring str_a_controlword = L"номер один"; 
	wstring str_b_controlword = L"тимошенко"; 
	wstring str_c_controlword = L"номер один"; 

	int key = VARIANT; // Порядковий номер у списку студентів
	
	wcout << "\n"; 

	print_task_1(str_a, str_a_controlword, key);  
	print_task_1(str_b, str_b_controlword, key);  
	print_task_1(str_c, str_c_controlword, key);  
}
