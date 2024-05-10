#include "EncryptionUtils.h"
#include "PasswordStorage.h"
#include <fstream>

std::string GenerateAESKey() {
    // Definieer de lengte van de AES-sleutel in bytes (bijv. 256 bits = 32 bytes)
    const int AES_KEY_LENGTH = 32;

    // Buffer om de AES-sleutel op te slaan
    unsigned char aesKey[AES_KEY_LENGTH];

    // Genereer een willekeurige AES-sleutel
    if (RAND_bytes(aesKey, AES_KEY_LENGTH) != 1) {
        // Fout bij het genereren van de AES-sleutel
        throw std::runtime_error("Error generating AES key");
    }

    // Converteer de sleutel naar een hexadecimale string voor gemakkelijke weergave en opslag
    std::string aesKeyString(reinterpret_cast<const char*>(aesKey), AES_KEY_LENGTH);
    return aesKeyString;
}

// Function to generate salt
std::string GenerateSalt(int length)
{
    constexpr int max_salt_length = 256; // Max length for the salt
    if (length <= 0 || length > max_salt_length)
    {
        throw std::invalid_argument("Invalid salt length");
    }

    // Buffer to store the salt
    unsigned char salt[max_salt_length];

    // Generate random bytes as salt
    if (RAND_bytes(salt, length) != 1)
    {
        // Error generating salt
        throw std::runtime_error("Error generating salt");
    }

    // Convert the salt to a string representation
    std::string saltString(reinterpret_cast<const char*>(salt), length);

    return saltString;
}

std::pair<std::string, std::string> RetrievePasswordAndSalt() {
    std::ifstream passwordFile(PASSWORD_FILE_NAME);
    std::string salt, encryptedPassword;
    if (passwordFile.is_open()) {
        // Read the salt and encrypted password from the file
        passwordFile >> salt >> encryptedPassword;
        passwordFile.close();
    }
    else {
        // Handle file not found or other errors
        throw std::runtime_error("Error: Failed to open password file for reading.");
    }
    return std::make_pair(salt, encryptedPassword);
}