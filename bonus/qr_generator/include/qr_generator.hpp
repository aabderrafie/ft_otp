#ifndef QR_GENERATOR_HPP
#define QR_GENERATOR_HPP

#include <string>

class QRGenerator {
public:
    // Generate QR code for TOTP setup
    static bool generateTOTPQR(const std::string& hex_key, 
                              const std::string& account_name = "user@ft_otp",
                              const std::string& issuer = "ft_otp",
                              const std::string& output_file = "ft_otp_qr.png");
    
    // Generate otpauth URL
    static std::string generateOTPAuthURL(const std::string& base32_secret,
                                        const std::string& account_name,
                                        const std::string& issuer);
    
    // Convert hex key to Base32 for QR code
    static std::string hexToBase32(const std::string& hex_key);
    
private:
    QRGenerator() = delete; // Static class
};

#endif // QR_GENERATOR_HPP
