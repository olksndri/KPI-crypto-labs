import binascii

from Crypto.Cipher import PKCS1_OAEP
from Crypto.PublicKey import RSA


def run_rsa_lab():
    print("--- Лабораторна робота №3: RSA ---")

    # 1.1 Генерація ключів
    key = RSA.generate(2048)
    private_key = key.export_key()
    public_key = key.publickey().export_key()

    print("[INFO] Ключі успішно згенеровано (2048 bit)")

    # Створюємо об'єкти шифрування
    encryptor = PKCS1_OAEP.new(RSA.import_key(public_key))
    decryptor = PKCS1_OAEP.new(RSA.import_key(private_key))

    # 1.2 Зашифрування тексту
    original_text = "виконав Тимошенко О. І."
    print(f"\n1.2. Вихідний текст: {original_text}")

    ciphertext = encryptor.encrypt(original_text.encode("utf-8"))
    print(f"Зашифрований текст (hex): {binascii.hexlify(ciphertext).decode()[:100]}...")

    # 1.3 Розшифрування тексту
    decrypted_text = decryptor.decrypt(ciphertext).decode("utf-8")
    print(f"1.3. Результат розшифрування: {decrypted_text}")

    # 1.4 Розшифрування НЕзашифрованого тексту
    raw_data = "Це просто звичайний текст, не шифр".encode("utf-8")
    print(f"\n1.4. Спроба розшифрувати незашифровані дані: {raw_data}")

    try:
        decryptor.decrypt(raw_data)
    except (ValueError, TypeError) as e:
        print(f"Результат: Помилка дешифрування! ({e})")
        print(
            "Пояснення: Алгоритм OAEP виявив некоректне доповнення (padding) і відхилив запит."
        )

    # 1.5 Зашифрування тексту з п.1.4
    print(f"\n1.5. Зашифрування тексту з п.1.4 публічним ключем...")
    cipher_14 = encryptor.encrypt(raw_data)
    print(f"Результат (hex): {binascii.hexlify(cipher_14).decode()[:100]}...")

    # 1.6 Порівняння результатів
    print("\n--- 1.6. Порівняння результатів ---")
    print(f"Довжина шифротексту (байт): {len(ciphertext)}")
    print(f"Відповідність після дешифрування: {original_text == decrypted_text}")


if __name__ == "__main__":
    run_rsa_lab()
