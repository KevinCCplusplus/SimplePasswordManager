#pragma once

#include <string>
#include <openssl/rand.h>
#include <stdexcept>

// Function to retrieve the stored password
std::string RetrievePassword(const std::string& key);

// Declare the SavePassword function
bool SavePassword(const std::string& encryptedPassword, const std::string& key);

// Function to encrypt a plaintext using AES with a given key
std::string EncryptAES(const std::string& plaintext, const std::string& key);

// Function to decrypt an encrypted text using AES with a given key
std::string DecryptAES(const std::string& encryptedText, const std::string& key);

