#include "ft_otp.hpp"
#include <iostream>

void printUsage() {
    std::cerr << "Usage: ./ft_otp [-g|-k] [file]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -g <file>    Generate and save key from hex file" << std::endl;
    std::cerr << "  -k <file>    Generate OTP from saved key file" << std::endl;
}


void printError(const std::string& message) {
    std::cerr << "./ft_otp: error: " << message << std::endl;
}


int handleGenerateKey(const std::string& filename) {
    try {
        // Read and validate the hex key
        std::string hex_key = FtOtp::readKeyFromFile(filename);
        
        if (!FtOtp::validateHexKey(hex_key)) {
            printError("key must be 64 hexadecimal characters.");
            return 1;
        }
        
        // Save the key
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


int handleGenerateOTP(const std::string& filename) {
    try {
        std::string otp = FtOtp::generateTOTP(filename);
        
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


int main(int argc, char* argv[]) {
    if (argc != 3)
        return  printUsage() , 1;
    
    const std::string option = argv[1];
    const std::string argument = argv[2];
    
    if (option == "-g") 
        return handleGenerateKey(argument);
    else if (option == "-k") 
        return handleGenerateOTP(argument);
    else 
        return printError("invalid option: " + option),printUsage(), 1;
    
}
