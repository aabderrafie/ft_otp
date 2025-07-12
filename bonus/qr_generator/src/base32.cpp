#include "base32.hpp"
#include <algorithm>
#include <stdexcept>

const std::string Base32::alphabet_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
const char Base32::padding_ = '=';

std::string Base32::encode(const std::vector<uint8_t>& data) {
    if (data.empty()) {
        return "";
    }
    
    std::string result;
    result.reserve(((data.size() + 4) / 5) * 8);
    
    size_t i = 0;
    while (i < data.size()) {
        // Take up to 5 bytes
        uint64_t buffer = 0;
        size_t bytes_taken = 0;
        
        for (size_t j = 0; j < 5 && i + j < data.size(); ++j) {
            buffer = (buffer << 8) | data[i + j];
            bytes_taken++;
        }
        
        // Pad buffer to 40 bits
        buffer <<= (5 - bytes_taken) * 8;
        
        // Extract 8 groups of 5 bits
        size_t chars_to_output = (bytes_taken * 8 + 4) / 5;
        for (size_t j = 0; j < chars_to_output; ++j) {
            result += encodeChar((buffer >> (35 - j * 5)) & 0x1F);
        }
        
        // Add padding
        while (result.length() % 8 != 0) {
            result += padding_;
        }
        
        i += bytes_taken;
    }
    
    return result;
}

std::string Base32::encode(const std::string& data) {
    std::vector<uint8_t> bytes(data.begin(), data.end());
    return encode(bytes);
}

std::vector<uint8_t> Base32::decode(const std::string& encoded) {
    if (encoded.empty()) {
        return {};
    }
    
    std::string clean_input = removeSpacesAndToUpper(encoded);
    
    if (!isValid(clean_input)) {
        throw std::invalid_argument("Invalid Base32 string");
    }
    
    // Remove padding
    while (!clean_input.empty() && clean_input.back() == padding_) {
        clean_input.pop_back();
    }
    
    std::vector<uint8_t> result;
    result.reserve((clean_input.length() * 5) / 8);
    
    size_t i = 0;
    while (i < clean_input.length()) {
        // Take up to 8 characters
        uint64_t buffer = 0;
        size_t chars_taken = 0;
        
        for (size_t j = 0; j < 8 && i + j < clean_input.length(); ++j) {
            buffer = (buffer << 5) | decodeChar(clean_input[i + j]);
            chars_taken++;
        }
        
        // Extract bytes
        size_t bytes_to_output = (chars_taken * 5) / 8;
        buffer >>= (8 - chars_taken) * 5;
        
        for (size_t j = 0; j < bytes_to_output; ++j) {
            result.push_back((buffer >> ((bytes_to_output - 1 - j) * 8)) & 0xFF);
        }
        
        i += chars_taken;
    }
    
    return result;
}

std::string Base32::decodeToString(const std::string& encoded) {
    std::vector<uint8_t> bytes = decode(encoded);
    return std::string(bytes.begin(), bytes.end());
}

bool Base32::isValid(const std::string& encoded) {
    std::string clean_input = removeSpacesAndToUpper(encoded);
    
    // Check length (must be multiple of 8 when padded)
    if (clean_input.length() % 8 != 0) {
        return false;
    }
    
    // Check characters
    for (char c : clean_input) {
        if (c != padding_ && alphabet_.find(c) == std::string::npos) {
            return false;
        }
    }
    
    // Check padding (only at the end)
    bool padding_started = false;
    for (char c : clean_input) {
        if (c == padding_) {
            padding_started = true;
        } else if (padding_started) {
            return false; // Non-padding character after padding
        }
    }
    
    return true;
}

char Base32::encodeChar(uint8_t value) {
    if (value >= 32) {
        throw std::invalid_argument("Value out of range for Base32");
    }
    return alphabet_[value];
}

uint8_t Base32::decodeChar(char c) {
    size_t pos = alphabet_.find(c);
    if (pos == std::string::npos) {
        throw std::invalid_argument("Invalid Base32 character");
    }
    return static_cast<uint8_t>(pos);
}

std::string Base32::removeSpacesAndToUpper(const std::string& input) {
    std::string result;
    result.reserve(input.length());
    
    for (char c : input) {
        if (!std::isspace(c)) {
            result += std::toupper(c);
        }
    }
    
    return result;
}
