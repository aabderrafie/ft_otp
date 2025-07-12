# ft_otp

A simple Time-based One-Time Password (TOTP) generator written in C++.

## What it does

- Stores hexadecimal keys securely
- Generates 6-digit TOTP codes (like Google Authenticator)
- Codes change every 30 seconds

## Requirements

- C++ compiler
- OpenSSL library

Install OpenSSL on macOS:

```bash
brew install openssl@3
```

## Build

```bash
cd mandatory
make
```

## Usage

### 1. Save a key

```bash
echo "3132333435363738393031323334353637383930313233343536373839303132" > key.hex
./ft_otp -g key.hex
```

Output: `Key was successfully saved in ft_otp.key.`

### 2. Generate TOTP code

```bash
./ft_otp -k ft_otp.key
```

Output: `123456` (6-digit code)

## Files

- `ft_otp.cpp` - Main code
- `ft_otp.hpp` - Header file
- `main.cpp` - Command line interface
- `decrypt_otp_key.py` - Python utility to view stored keys
- `Makefile` - Build configuration

## Commands

- `make` - Build the program
- `make clean` - Remove build files
- `make fclean` - Remove all generated files
