#ifndef BASE32_HPP
#define BASE32_HPP

#include <string>
#include <vector>

class Base32 {
public:
    // Encode binary data to Base32 string
    static std::string encode(const std::vector<uint8_t>& data);
    static std::string encode(const std::string& data);
    
    // Decode Base32 string to binary data
    static std::vector<uint8_t> decode(const std::string& encoded);
    static std::string decodeToString(const std::string& encoded);
    
    // Validate Base32 string
    static bool isValid(const std::string& encoded);
    
private:
    static const std::string alphabet_;
    static const char padding_;
    
    // Helper functions
    static char encodeChar(uint8_t value);
    static uint8_t decodeChar(char c);
    static std::string removeSpacesAndToUpper(const std::string& input);
};

#endif // BASE32_HPP
