#include "ft_otp.hpp"
#include <iostream>

void printUsage() {
    std::cerr << "Usage: ./ft_otp [-g|-k] [file]" << std::endl;
}

void printError(const std::string& message) {
    std::cerr << "./ft_otp: error: " << message << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }
    
    std::string option = argv[1];
    std::string argument = argv[2];
    
    if (option == "-g") {
        try {
            std::string hex_key = FtOtp::readKeyFromFile(argument);
            
            if (!FtOtp::validateHexKey(hex_key)) {
                printError("key must be 64 hexadecimal characters.");
                return 1;
            }
            
            if (FtOtp::saveKey("ft_otp.key", hex_key)) {
                std::cout << "Key was successfully saved in ft_otp.key." << std::endl;
                return 0;
            } else {
                printError("failed to save key.");
                return 1;
            }
        } catch (const std::exception& e) {
            printError("failed to read key file.");
            return 1;
        }
    }
    else if (option == "-k") {
        // Generate OTP
        try {
            std::string otp = FtOtp::generateTOTP(argument);
            if (otp.empty()) {
                printError("failed to generate OTP.");
                return 1;
            }
            std::cout << otp << std::endl;
            return 0;
        } catch (const std::exception& e) {
            printError("failed to generate OTP.");
            return 1;
        }
    }
    else {
        printUsage();
        return 1;
    }
}
