#include "EncryptionUtils.h"

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
