#include "PasswordStorage.h"
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include "EncryptionUtils.h"

const std::string PASSWORD_FILE_NAME = "password.txt";

bool SavePassword(const std::string& encryptedPassword, const std::string& key)
{
    try
    {
        // Open the password file for writing
        std::ofstream passwordFile(PASSWORD_FILE_NAME);
        if (!passwordFile)
        {
            // Failed to open the file
            std::cerr << "Error: Failed to open password file for writing." << std::endl;
            return false;
        }

        // Write the encrypted password to the file
        passwordFile << encryptedPassword << std::endl;

        // Close the file
        passwordFile.close();

        // Password saved successfully
        return true;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception occurred while saving password: " << ex.what() << std::endl;
        return false;
    }
}

std::pair<std::string, std::string> RetrievePasswordAndSaltFromFile()
{
    try
    {
        std::ifstream passwordFile(PASSWORD_FILE_NAME);
        if (!passwordFile)
        {
            // Failed to open the file
            throw std::runtime_error("Error: Failed to open password file for reading.");
        }

        std::string encryptedPassword;
        std::string salt;

        // Read the encrypted password and salt from the file
        if (!(passwordFile >> encryptedPassword >> salt))
        {
            // Error reading from file
            throw std::runtime_error("Error: Failed to read password and salt from file.");
        }

        // Close the file
        passwordFile.close();

        return std::make_pair(encryptedPassword, salt);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception occurred while retrieving password and salt from file: " << ex.what() << std::endl;
        return std::make_pair("", "");
    }
}

std::string EncryptAES(const std::string& plaintext, const std::string& key) {
    // Initialize the cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Error creating cipher context");
    }

    // Initialize the cipher context for AES-256 CBC (Cipher Block Chaining) mode
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error initializing cipher context");
    }

    // Add the plaintext to the cipher context for encryption
    std::string ciphertext;
    int len = 0;
    int ciphertextLength = 0;
    unsigned char outbuf[1024];

    if (EVP_EncryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error updating cipher context");
    }
    ciphertextLength = len;
    ciphertext.append(reinterpret_cast<char*>(outbuf), len);

    // Finalize the encryption
    if (EVP_EncryptFinal_ex(ctx, outbuf, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error finalizing cipher context");
    }
    ciphertextLength += len;
    ciphertext.append(reinterpret_cast<char*>(outbuf), len);

    // Free the cipher context
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}

std::string DecryptAES(const std::string& encryptedText, const std::string& key)
{
    // Initialize the cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Error creating cipher context");
    }

    // Initialize the cipher context for AES-256 CBC (Cipher Block Chaining) mode
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error initializing cipher context");
    }

    // Add the encrypted text to the cipher context for decryption
    std::string decryptedText;
    int len = 0;
    int decryptedLength = 0;
    unsigned char outbuf[1024];

    if (EVP_DecryptUpdate(ctx, outbuf, &len, reinterpret_cast<const unsigned char*>(encryptedText.c_str()), encryptedText.length()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error updating cipher context");
    }
    decryptedLength = len;
    decryptedText.append(reinterpret_cast<char*>(outbuf), len);

    // Finalize the decryption
    if (EVP_DecryptFinal_ex(ctx, outbuf, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Error finalizing cipher context");
    }
    decryptedLength += len;
    decryptedText.append(reinterpret_cast<char*>(outbuf), len);

    // Free the cipher context
    EVP_CIPHER_CTX_free(ctx);

    return decryptedText;
}
