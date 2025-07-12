#include "qr_generator.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

bool QRGenerator::generateTOTPQR(const std::string& hex_key, 
                                const std::string& account_name,
                                const std::string& issuer,
                                const std::string& output_file) {
    try {
        // Convert hex to Base32
        std::string base32_secret = hexToBase32(hex_key);
        
        // Generate otpauth URL
        std::string url = generateOTPAuthURL(base32_secret, account_name, issuer);
        
        // Use qrencode to generate QR code
        std::string command = "qrencode -o \"" + output_file + "\" \"" + url + "\"";
        
        int result = system(command.c_str());
        if (result == 0) {
            std::cout << "QR code generated: " << output_file << std::endl;
            std::cout << "Scan with your authenticator app!" << std::endl;
            std::cout << "Manual entry secret (Base32): " << base32_secret << std::endl;
            return true;
        } else {
            std::cerr << "Error: qrencode not found. Install with: brew install qrencode" << std::endl;
            std::cout << "Manual entry secret (Base32): " << base32_secret << std::endl;
            std::cout << "Manual entry URL: " << url << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error generating QR code: " << e.what() << std::endl;
        return false;
    }
}

std::string QRGenerator::generateOTPAuthURL(const std::string& base32_secret,
                                          const std::string& account_name,
                                          const std::string& issuer) {
    std::ostringstream url;
    url << "otpauth://totp/";
    url << issuer << ":" << account_name;
    url << "?secret=" << base32_secret;
    url << "&issuer=" << issuer;
    url << "&algorithm=SHA1";
    url << "&digits=6";
    url << "&period=30";
    
    return url.str();
}

std::string QRGenerator::hexToBase32(const std::string& hex_key) {
    const std::string base32_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    
    // Convert hex to binary string
    std::string binary;
    for (size_t i = 0; i < hex_key.length(); i += 2) {
        std::string hex_byte = hex_key.substr(i, 2);
        int byte_value = std::stoi(hex_byte, nullptr, 16);
        
        for (int j = 7; j >= 0; j--) {
            binary += ((byte_value >> j) & 1) ? '1' : '0';
        }
    }
    
    // Convert binary to Base32
    std::string base32;
    for (size_t i = 0; i < binary.length(); i += 5) {
        std::string chunk = binary.substr(i, 5);
        while (chunk.length() < 5) {
            chunk += '0'; // Pad with zeros
        }
        
        int value = 0;
        for (size_t j = 0; j < chunk.length(); j++) {
            value = (value << 1) + (chunk[j] - '0');
        }
        
        base32 += base32_chars[value];
    }
    
    // Add padding if needed
    while (base32.length() % 8 != 0) {
        base32 += '=';
    }
    
    return base32;
}
