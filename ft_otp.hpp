#pragma once

#include <string>
#include <cstdint>

class FtOtp {
public:
  
    static bool saveKey(const std::string& filename, const std::string& hex_key);
  
    static std::string generateTOTP(const std::string& filename);
    
    static bool validateHexKey(const std::string& hex_key);
    
    static std::string readKeyFromFile(const std::string& filename);
    
    static std::string hexToBinary(const std::string& hex);
    
    static std::string encrypt(const std::string& data);
    static std::string decrypt(const std::string& data);

    static std::string generateHOTP(const std::string& key, uint64_t counter);
    
    static uint64_t getCurrentTimeStep();

    static std::string hmacSHA1(const std::string& key, uint64_t counter);
};


