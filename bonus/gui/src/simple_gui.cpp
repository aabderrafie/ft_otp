#include "simple_gui.hpp"
#include "ft_otp.hpp"
#include "qr_generator.hpp"
#include <iostream>
#include <string>
#include <limits>

void SimpleGUI::run() {
    std::cout << "=== ft_otp Graphic Interface ===" << std::endl;
    std::cout << "Welcome to the TOTP Key Manager!" << std::endl;
    std::cout << std::endl;
    
    while (true) {
        int choice = displayMenu();
        
        switch (choice) {
            case 1:
                handleKeyGeneration();
                break;
            case 2:
                handleOTPGeneration();
                break;
            case 3:
                handleQRGeneration();
                break;
            case 4:
                std::cout << "Goodbye!" << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
        
        std::cout << std::endl;
    }
}

int SimpleGUI::displayMenu() {
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Generate and save key from hex file" << std::endl;
    std::cout << "2. Generate OTP from saved key" << std::endl;
    std::cout << "3. Generate QR code from hex key" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice (1-4): ";
    
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void SimpleGUI::handleKeyGeneration() {
    std::cout << std::endl << "=== Key Generation ===" << std::endl;
    std::cout << "Enter the path to your hex key file: ";
    
    std::string filename;
    std::getline(std::cin, filename);
    
    try {
        std::string hex_key = FtOtp::readKeyFromFile(filename);
        
        if (!FtOtp::validateHexKey(hex_key)) {
            std::cout << "Error: key must be 64 hexadecimal characters." << std::endl;
            return;
        }
        
        if (FtOtp::saveKey("ft_otp.key", hex_key)) {
            std::cout << "✓ Key was successfully saved in ft_otp.key." << std::endl;
        } else {
            std::cout << "✗ Failed to save key." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "✗ Error reading key file: " << e.what() << std::endl;
    }
}

void SimpleGUI::handleOTPGeneration() {
    std::cout << std::endl << "=== OTP Generation ===" << std::endl;
    
    try {
        std::string otp = FtOtp::generateTOTP("ft_otp.key");
        if (!otp.empty()) {
            std::cout << "Current OTP: " << otp << std::endl;
            std::cout << "✓ OTP generated successfully!" << std::endl;
        } else {
            std::cout << "✗ Failed to generate OTP. Make sure ft_otp.key exists." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "✗ Error generating OTP: " << e.what() << std::endl;
    }
}

void SimpleGUI::handleQRGeneration() {
    std::cout << std::endl << "=== QR Code Generation ===" << std::endl;
    std::cout << "Enter the path to your hex key file: ";
    
    std::string filename;
    std::getline(std::cin, filename);
    
    std::cout << "Enter account name (default: user@ft_otp): ";
    std::string account_name;
    std::getline(std::cin, account_name);
    if (account_name.empty()) {
        account_name = "user@ft_otp";
    }
    
    try {
        std::string hex_key = FtOtp::readKeyFromFile(filename);
        
        if (!FtOtp::validateHexKey(hex_key)) {
            std::cout << "Error: key must be 64 hexadecimal characters." << std::endl;
            return;
        }
        
        bool success = QRGenerator::generateTOTPQR(hex_key, account_name, "ft_otp", "ft_otp_qr.png");
        
        if (success) {
            std::cout << "✓ QR code generated successfully!" << std::endl;
        } else {
            std::cout << "⚠ QR code image generation failed, but manual entry info provided." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "✗ Error: " << e.what() << std::endl;
    }
}
