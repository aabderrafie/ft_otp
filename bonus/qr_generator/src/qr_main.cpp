#include "qr_generator.hpp"
#include "ft_otp.hpp"
#include <iostream>
#include <string>

void printQRUsage() {
    std::cout << "Usage: ./ft_otp_qr [-g] [key_file] [account_name]" << std::endl;
    std::cout << "  -g: Generate QR code from hex key file" << std::endl;
    std::cout << "  key_file: File containing hexadecimal key" << std::endl;
    std::cout << "  account_name: Account name for the QR code (optional, default: user@ft_otp)" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printQRUsage();
        return 1;
    }
    
    std::string option = argv[1];
    std::string key_file = argv[2];
    std::string account_name = (argc > 3) ? argv[3] : "user@ft_otp";
    
    if (option == "-g") {
        try {
            // Read and validate the hex key
            std::string hex_key = FtOtp::readKeyFromFile(key_file);
            
            if (!FtOtp::validateHexKey(hex_key)) {
                std::cerr << "Error: key must be 64 hexadecimal characters." << std::endl;
                return 1;
            }
            
            // Generate QR code
            std::string output_file = "ft_otp_qr.png";
            bool success = QRGenerator::generateTOTPQR(hex_key, account_name, "ft_otp", output_file);
            
            if (success) {
                std::cout << "QR code generated successfully!" << std::endl;
                std::cout << "Use any authenticator app to scan the QR code." << std::endl;
            } else {
                std::cout << "QR code image generation failed, but you can use the manual entry information above." << std::endl;
            }
            
            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    } else {
        printQRUsage();
        return 1;
    }
}
