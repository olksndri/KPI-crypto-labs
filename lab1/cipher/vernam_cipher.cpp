#include "../include.h"

using namespace std; 


wstring encrypt_vernam(wstring msg, wstring key) {
    int alph_len = alphabet.length();
    int msg_len = msg.length();
    int key_len = key.length();
    int k_word_cnt = 0;
    wstring encr_msg(msg_len, L'*');

    for (int i = 0; i < msg_len; i++) {
        int msg_idx = alphabet.find(msg[i]);
        int key_idx = alphabet.find(key[k_word_cnt]);
        
        if (msg_idx == (int)wstring::npos || key_idx == (int)wstring::npos) {
            encr_msg[i] = msg[i];
            continue;
        }

        encr_msg[i] = alphabet[(msg_idx + key_idx) % alph_len];
        k_word_cnt = (k_word_cnt + 1) % key_len;
    }
    return encr_msg;
}

wstring decrypt_vernam(wstring msg, wstring key) {
    int alph_len = alphabet.length();
    int msg_len = msg.length();
    int key_len = key.length();
    int k_word_cnt = 0;
    wstring decr_msg(msg_len, L'*');

    for (int i = 0; i < msg_len; i++) {
        if (msg[i] == L' ') {
            decr_msg[i] = L' ';
            continue;
        }

        int msg_idx = alphabet.find(msg[i]);
        int key_idx = alphabet.find(key[k_word_cnt]);
        
        if (msg_idx == (int)wstring::npos || key_idx == (int)wstring::npos) {
            decr_msg[i] = msg[i];
            continue;
        }
        
        decr_msg[i] = alphabet[(msg_idx - key_idx + alph_len) % alph_len];
        k_word_cnt = (k_word_cnt + 1) % key_len;
    }
    return decr_msg;
}

static void brutforce_vernam_recurse(wstring &keyword, int key_idx, 
    wstring &decr_msg, const wstring &msg, const wstring &control_word, 
    wstring &match_key, int &matched) 
{ 
    if (matched) return; 

    if (key_idx == (int)keyword.length()) { 
        wstring attempt = decrypt_vernam(msg, keyword); 
        if (attempt.find(control_word) != wstring::npos) { 
            decr_msg = attempt; 
            match_key = keyword; 
            matched = 1; 
        }
        return;
    }

    // Для Вернама тут буде мільярд років очікування, якщо повідомлення довге
    for (int k = 0; k < (int)alphabet.length(); k++) {
        keyword[key_idx] = alphabet[k]; 
        brutforce_vernam_recurse(keyword, key_idx + 1, decr_msg, msg, control_word, match_key, matched); 
        if (matched) return;
    }
}

wstring brutforce_vernam(wstring msg, wstring control_word, wstring &match_key) {
    // Рахуємо кількість символів, які реально шифруються (без пробілів)
    int clean_len = 0;
    for(wchar_t c : msg) if(c != L' ') clean_len++;

    wstring keyword(clean_len, L'*'); 
    wstring decr_msg(msg.length(), L'*'); 
    int matched = 0; 

    // Обмеження: запускаємо тільки якщо clean_len дуже малий (наприклад, <= 4)
    if (clean_len > 4) {
        match_key = L"impossible (key too long)";
        return L"decryption aborted";
    }

    brutforce_vernam_recurse(keyword, 0, decr_msg, msg, control_word, match_key, matched); 
    return matched ? decr_msg : L"not found";
}
wchar_t crack_caesar_shift(const wstring& column) {
    int alph_len = alphabet.length();
    double min_chi_square = -1;
    int best_shift = 0;

    for (int shift = 0; shift < alph_len; shift++) {
        map<wchar_t, int> counts;
        int total = 0;
        
        // Робимо зворотний зсув (дешифруємо колонку як Цезар)
        for (wchar_t c : column) {
            size_t idx = alphabet.find(c);
            if (idx != wstring::npos) {
                int original_idx = (idx - shift + alph_len) % alph_len;
                wchar_t decr_char = towlower(alphabet[original_idx]);
                if (ukr_freqs.count(decr_char)) {
                    counts[decr_char]++;
                    total++;
                }
            }
        }

        // Рахуємо статистичну відповідність (Chi-Squared test)
        double chi_sq = 0;
        for (auto const& [letter, freq] : ukr_freqs) {
            double expected = total * freq;
            double observed = counts[letter];
            chi_sq += pow(observed - expected, 2) / (expected + 0.0001);
        }

        if (min_chi_square < 0 || chi_sq < min_chi_square) {
            min_chi_square = chi_sq;
            best_shift = shift;
        }
    }
    return alphabet[best_shift];
}

wstring vernam_frequency_attack(const wstring& msg, const wstring& control_word, wstring &match_key) {
    wstring clean_msg = L"";
    for (wchar_t c : msg) if (c != L' ') clean_msg += c;
    
    int key_len = clean_msg.length();
    wstring recovered_key = L"";

    // Для кожної позиції ми просто беремо найімовірнішу літеру (на основі 'о' або 'а')
    // Але оскільки ключ випадковий, це видасть повну нісенітницю
    for (int i = 0; i < key_len; i++) {
        wstring column(1, clean_msg[i]);
        recovered_key += crack_caesar_shift(column); 
    }

    match_key = recovered_key;
    wstring attempt = decrypt_vernam(msg, recovered_key);
    
    if (attempt.find(control_word) != wstring::npos) return attempt;
    return L"Frequency analysis failed (as expected for Vernam)";
}


void print_task_3(wstring str, wstring control_word, wstring key) 
{

	wstring match_key_brut; 
	wstring match_key_freq; 

	int common_letters_count = 33; 
	
	wstring encr_message = encrypt_vernam(str, key);  
	wstring decr_message = decrypt_vernam(encr_message, key); 
	brutforce_vernam(encr_message, control_word, match_key_brut); 
	vernam_frequency_attack(encr_message, control_word, match_key_freq);

	wcout << TXT_YELLOW << L"message: " << TXT_END_COLOR << str << "\n"; 
	wcout << TXT_RED    << L"encrypted vernam message: " << TXT_END_COLOR <<  encr_message << "\n"; 
	wcout << TXT_GREEN  << L"decrypted vernam message: " << TXT_END_COLOR <<  decr_message << "\n"; 
	wcout << TXT_GREEN  << L"Brutforce attack, matched key: " << TXT_END_COLOR << match_key_brut  << "\n"; 
	wcout << TXT_GREEN  << L"Frequency analysis attack, matched key: " << TXT_END_COLOR << match_key_freq << "\n"; 
	wcout << "\n"; 
}

void task_3(wstring str_a, wstring str_b, wstring str_c) 
{
    int alph_len = alphabet.length(); 

    // Генератор випадкових чисел
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    
    std::uniform_int_distribution<> distrib(0, alph_len); 
 
    wstring key_a(str_a.length(), '*'); 
    wstring key_b(str_b.length(), '*'); 
    wstring key_c(str_c.length(), '*'); 

    for(int i = 0; i < str_a.length(); i++)
        key_a[i] = alphabet[distrib(gen)]; 

    for(int i = 0; i < str_b.length(); i++)
        key_b[i] = alphabet[distrib(gen)]; 
    
    for(int i = 0; i < str_c.length(); i++)
        key_c[i] = alphabet[distrib(gen)]; 

	wstring str_a_controlword = L"номер один"; 
	wstring str_b_controlword = L"тимошенко"; 
	wstring str_c_controlword = L"номер один"; 

	print_task_3(str_a, str_a_controlword, key_a);  
	print_task_3(str_b, str_b_controlword, key_b);  
	print_task_3(str_c, str_c_controlword, key_c);  
}
