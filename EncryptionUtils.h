#pragma once

#include <string>
#include <openssl/rand.h>
#include <stdexcept>

// Function to retrieve the stored password
std::wstring RetrievePassword();

// Function to save the encrypted password to a file
bool SavePassword(const std::wstring& encryptedPassword);

// Function to retrieve the encrypted password from a file
std::wstring RetrievePassword();

// Function to encrypt a plaintext using AES with a given key
std::string EncryptAES(const std::string& plaintext, const std::string& key);

// Function to decrypt an encrypted text using AES with a given key
std::string DecryptAES(const std::string& encryptedText, const std::string& key);

// Declaration of GenerateAESKey function
std::string GenerateAESKey();
#pragma once
