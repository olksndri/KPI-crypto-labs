#include "../include.h"

using namespace std; 

wstring encrypt_vigenere(wstring msg, wstring key) {
    int msg_len = msg.length();
    int alph_len = alphabet.length();
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

wstring decrypt_vigenere(wstring msg, wstring key) {
    int msg_len = msg.length();
    int alph_len = alphabet.length();
    int key_len = key.length();
    int k_word_cnt = 0;
    wstring decr_msg(msg_len, L'*');

    for (int i = 0; i < msg_len; i++) {
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

// Рекурсивна функція підбору
static void brutforce_vigenere_recurse(
    wstring &keyword, 
    int key_idx, 
    wstring &decr_msg, 
    const wstring &msg, 
    const wstring &control_word, 
    wstring &match_key, 
    int &matched, 
    int key_len, 
    int alph_len) 
{ 
    if (matched) return; 

    // Базовий випадок: ключ сформовано
    if (key_idx == key_len) { 
        wstring attempt = decrypt_vigenere(msg, keyword); 
        if (attempt.find(control_word) != wstring::npos) { 
            decr_msg = attempt; 
            match_key = keyword; 
            matched = 1; 
        }
        return;
    }

    // Перебір літер для поточної позиції
    for (int k = 0; k < alph_len; k++) {
        if (matched) return; 
        keyword[key_idx] = alphabet[k]; 
        
        // Передаємо key_len та alph_len як значення (int), а не посилання на R-value
        brutforce_vigenere_recurse(keyword, key_idx + 1, decr_msg, msg, control_word, match_key, matched, key_len, alph_len); 
    }
}

wstring brutforce_vigenere(wstring msg, wstring control_word, wstring &match_key) { 
    int msg_len = msg.length(); 
    int alph_len = alphabet.length(); 
 
    for (int key_len = 1; key_len < alph_len; key_len++) {
        wstring keyword(key_len, L'*'); 
        wstring decr_msg(msg_len, L'*'); 
        int matched = 0; 

        brutforce_vigenere_recurse(keyword, 0, decr_msg, msg, control_word, match_key, matched, key_len, alph_len); 

        if (matched) return decr_msg; 
    }

    match_key = L"not found (limit reached)"; 
    return L"not decrypted";
}

struct ShiftScore {
    int shift;
    double score;
};

bool compareScores(const ShiftScore& a, const ShiftScore& b) {
    return a.score < b.score;
}

// Функція для отримання Топ-N зміщень
vector<wchar_t> get_top_n_shifts(const wstring& column, int n = 3) {
    int alph_len = analysis_alphabet.length();
    vector<ShiftScore> scores;

    for (int shift = 0; shift < alph_len; shift++) {
        map<wchar_t, int> counts;
        int total = 0;
        for (wchar_t c : column) {
            size_t idx = analysis_alphabet.find(c);
            if (idx != wstring::npos) {
                int original_idx = (idx - shift + alph_len) % alph_len;
                counts[analysis_alphabet[original_idx]]++;
                total++;
            }
        }
        double chi_sq = 0;
        for (auto const& [letter, freq] : ukr_freqs) {
            double expected = total * freq;
            chi_sq += pow(counts[letter] - expected, 2) / (expected + 0.0001);
        }
        scores.push_back({shift, chi_sq});
    }
    sort(scores.begin(), scores.end(), compareScores);

    vector<wchar_t> candidates;
    for (int i = 0; i < n && i < (int)scores.size(); i++) {
        candidates.push_back(analysis_alphabet[scores[i].shift]);
    }
    return candidates;
}

// Рекурсивний перебір Топ-кандидатів 
static void check_top_candidates_recurse(
    const vector<vector<wchar_t>>& all_candidates,
    int key_idx, wstring& current_key,
    const wstring& msg, const wstring& control_word,
    wstring& match_key, int& matched) 
{
    if (matched) return;
    if (key_idx == (int)all_candidates.size()) {
        wstring attempt = decrypt_vigenere(msg, current_key);
        if (attempt.find(control_word) != wstring::npos) {
            match_key = current_key;
            matched = 1;
        }
        return;
    }
    for (wchar_t candidate : all_candidates[key_idx]) {
        current_key[key_idx] = candidate;
        check_top_candidates_recurse(all_candidates, key_idx + 1, current_key, msg, control_word, match_key, matched);
        if (matched) return;
    }
}

// Головна функція атаки 

wstring vigenere_frequency_attack(const wstring& msg, const wstring& control_word, wstring &match_key) {
    // Очищуємо текст від пробілів для аналізу (ключ на них не зсувався)
    wstring clean_msg = L"";
    for (wchar_t c : msg) {
        if (analysis_alphabet.find(c) != wstring::npos) clean_msg += c;
    }
    if (clean_msg.empty()) return L"not decrypted (empty)";

    // Гібридний перебір довжин від 1 до 10 (більше для частотки зазвичай не треба)
    for (int len = 1; len <= 10; len++) {
        vector<vector<wchar_t>> all_candidates;
        for (int i = 0; i < len; i++) {
            wstring column = L"";
            for (int j = i; j < (int)clean_msg.length(); j += len) {
                column += clean_msg[j];
            }
            // Беремо топ-3 кандидати для кожної літери ключа
            all_candidates.push_back(get_top_n_shifts(column, 3)); 
        }

        int matched = 0;
        wstring current_key(len, L'*');
        check_top_candidates_recurse(all_candidates, 0, current_key, msg, control_word, match_key, matched);

        if (matched) return decrypt_vigenere(msg, match_key);
    }
    match_key = L"not found";
    return L"not decrypted";
}

void print_task_2(wstring str, wstring control_word, wstring key) 
{
	wstring match_key_brut; 
	wstring match_key_freq; 

	int common_letters_count = 33; 
	
	wstring encr_message = encrypt_vigenere(str, key);  
	wstring decr_message = decrypt_vigenere(encr_message, key); 
	brutforce_vigenere(encr_message, control_word, match_key_brut); 
	vigenere_frequency_attack(encr_message, control_word, match_key_freq);

	wcout << TXT_YELLOW << L"message: " << TXT_END_COLOR << str << "\n"; 
	wcout << TXT_RED    << L"encrypted vigenere message: " << TXT_END_COLOR <<  encr_message << "\n"; 
	wcout << TXT_GREEN  << L"decrypted vigenere message: " << TXT_END_COLOR <<  decr_message << "\n"; 
	wcout << TXT_GREEN  << L"Brutforce attack, matched key: " << TXT_END_COLOR << match_key_brut  << "\n"; 
	wcout << TXT_GREEN  << L"Frequency analysis attack, matched key: " << TXT_END_COLOR << match_key_freq << "\n"; 
	wcout << "\n"; 
}

void task_2(wstring str_a, wstring str_b, wstring str_c) 
{
	wstring str_a_controlword = L"номер один"; 
	wstring str_b_controlword = L"тимошенко"; 
	wstring str_c_controlword = L"номер один"; 

	wstring key = SURNAME;
	
	print_task_2(str_a, str_a_controlword, key);  
	print_task_2(str_b, str_b_controlword, key);  
	print_task_2(str_c, str_c_controlword, key);  
}
