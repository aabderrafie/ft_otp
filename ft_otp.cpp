#include "ft_otp.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sys/stat.h>

bool FtOtp::saveKey(const std::string& filename, const std::string& hex_key) {
    try {
        std::string binary_key = hexToBinary(hex_key);
        std::string encrypted_key = encrypt(binary_key);
        
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open())
            return false;
        file.write(encrypted_key.c_str(), encrypted_key.size());
        file.close();
        chmod(filename.c_str(), S_IRUSR | S_IWUSR);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool FtOtp::validateHexKey(const std::string& hex_key) {
    std::string clean_key;
    for (char c : hex_key) {
        if (!std::isspace(c)) 
        clean_key += std::toupper(c);
    }
    
    
    if (clean_key.length() < 64) 
    return false;
    

    for (char c : clean_key) {
        if (!std::isxdigit(c))
         return false;
    }
    
    return true;
}

std::string FtOtp::readKeyFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) 
        throw std::runtime_error("Cannot open file");
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    std::string content = buffer.str();

    content.erase(content.find_last_not_of(" \t\n\r\f\v") + 1);
    
    return content;
}


std::string FtOtp::generateTOTP(const std::string& filename) {
    try {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return "";
        }
        

        std::string encrypted_data;
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        encrypted_data.resize(file_size);
        file.read(&encrypted_data[0], file_size);
        file.close();
        
        
        std::string decrypted_key = decrypt(encrypted_data);
        
      
        uint64_t time_step = getCurrentTimeStep();
        return generateHOTP(decrypted_key, time_step);
    } catch (const std::exception&) {
        return "";
    }
}

std::string FtOtp::hexToBinary(const std::string& hex) {
    std::string clean_hex;
    for (char c : hex) {
        if (!std::isspace(c))
            clean_hex += std::toupper(c);
    }
    
    std::string binary;
    for (size_t i = 0; i < clean_hex.length(); i += 2) {
        std::string byte_str = clean_hex.substr(i, 2);
        char byte = static_cast<char>(std::stoul(byte_str, nullptr, 16));
        binary += byte;
    }
    
    return binary;
}

std::string FtOtp::encrypt(const std::string& data) {
    std::string key = "ft_otp_simple_key_for_encryption_2025";
    std::string encrypted = data;
    
    for (size_t i = 0; i < encrypted.size(); ++i) 
        encrypted[i] ^= key[i % key.size()];
    return encrypted;
}

std::string FtOtp::decrypt(const std::string& data) {
    return encrypt(data);
}

uint64_t FtOtp::getCurrentTimeStep() {
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    return seconds.count() / 30; 
}

std::string FtOtp::generateHOTP(const std::string& key, uint64_t counter) {

    std::string hmac = hmacSHA1(key, counter);


    const unsigned char* hmac_bytes = reinterpret_cast<const unsigned char*>(hmac.c_str());
    
    // Dynamic truncation
    int offset = hmac_bytes[hmac.length() - 1] & 0x0F;
    
    uint32_t code = ((hmac_bytes[offset] & 0x7F) << 24) |
                    ((hmac_bytes[offset + 1] & 0xFF) << 16) |
                    ((hmac_bytes[offset + 2] & 0xFF) << 8) |
                    (hmac_bytes[offset + 3] & 0xFF);
    
    uint32_t otp = code % 1000000;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(6) << otp;
    return oss.str();
}


std::string FtOtp::hmacSHA1(const std::string& key, uint64_t counter) {
    unsigned char counter_bytes[8];
    for (int i = 7; i >= 0; --i) {
        counter_bytes[i] = counter & 0xFF;
        counter >>= 8;
    }
    
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int result_len;
    
    HMAC(EVP_sha1(), key.c_str(), key.length(),counter_bytes, 8,result, &result_len);
    
    return std::string(reinterpret_cast<char*>(result), result_len);
}
