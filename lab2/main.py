import binascii
import time

from Crypto.Cipher import AES, ARC4
from Crypto.Random import get_random_bytes


# --- 1. Власна реалізація RC4 ---
def manual_rc4(key, data):
    # KSA - Key Scheduling Algorithm
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]

    # PRGA - Pseudo-Random Generation Algorithm
    i = j = 0
    res = []
    for char in data:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        k = S[(S[i] + S[j]) % 256]
        res.append(char ^ k)
    return bytes(res)


# --- 2. Функції для роботи з API (Library) ---
def library_rc4(key, data):
    cipher = ARC4.new(key)
    return cipher.encrypt(data)


def library_aes_encrypt(key, data):
    # Використовуємо режим EAX для безпеки
    cipher = AES.new(key, AES.MODE_EAX)
    nonce = cipher.nonce
    ciphertext, tag = cipher.encrypt_and_digest(data)
    return nonce + ciphertext  # Повертаємо разом з nonce для дешифрування


def library_aes_decrypt(key, combined_data):
    nonce = combined_data[:16]
    ciphertext = combined_data[16:]
    cipher = AES.new(key, AES.MODE_EAX, nonce=nonce)
    return cipher.decrypt(ciphertext)


# --- 3. Тестовий стенд ---
def run_tests():
    key_rc4 = b"SecretKey123"
    key_aes = get_random_bytes(16)  # 128-bit key

    test_phrases = [
        "лабораторна робота номер два",
        "Тимошенко О. І.",
        "метою лабораторної роботи номер два є отримання базових знань із сучасних симетричних криптографічних систем шляхом використання криптографічних АРІ та розробки та застосування інструментарію для шифрування та дешифрування повідомлень. Лабораторну роботу виконав Тимошенко Олександр Ігорович",
    ]

    print(
        f"{'№':<3} | {'Алгоритм':<15} | {'Тип':<10} | {'Час (мс)':<10} | {'Результат (hex)'}"
    )
    print("-" * 80)

    for idx, phrase in enumerate(test_phrases, 1):
        data = phrase.encode("utf-8")

        # Тест Manual RC4
        start = time.perf_counter()
        m_rc4 = manual_rc4(key_rc4, data)
        end = time.perf_counter()
        print(
            f"{idx}a  | RC4 (Manual)    | Custom     | {(end - start) * 1000:.4f}   | {binascii.hexlify(m_rc4[:20]).decode()}..."
        )

        # Тест Library RC4
        start = time.perf_counter()
        l_rc4 = library_rc4(key_rc4, data)
        end = time.perf_counter()
        print(
            f"{idx}b  | RC4 (API)       | Library    | {(end - start) * 1000:.4f}   | {binascii.hexlify(l_rc4[:20]).decode()}..."
        )

        # Тест AES
        start = time.perf_counter()
        l_aes = library_aes_encrypt(key_aes, data)
        end = time.perf_counter()
        print(
            f"{idx}c  | AES-128 (API)   | Library    | {(end - start) * 1000:.4f}   | {binascii.hexlify(l_aes[:20]).decode()}..."
        )
        print("-" * 80)


if __name__ == "__main__":
    run_tests()
