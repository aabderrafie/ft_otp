#ifndef SIMPLE_GUI_HPP
#define SIMPLE_GUI_HPP

#include <string>

class SimpleGUI {
public:
    // Run the GUI interface
    static void run();
    
private:
    SimpleGUI() = delete; // Static class
    
    // Display menu and get user choice
    static int displayMenu();
    
    // Handle key generation
    static void handleKeyGeneration();
    
    // Handle OTP generation
    static void handleOTPGeneration();
    
    // Handle QR code generation
    static void handleQRGeneration();
};

#endif // SIMPLE_GUI_HPP
