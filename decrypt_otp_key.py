
import base64

key = b"ft_otp_simple_key_for_encryption_2025"

with open("ft_otp.key", "rb") as f:
    encrypted = f.read()

decrypted = bytes([b ^ key[i % len(key)] for i, b in enumerate(encrypted)])


print(base64.b32encode(decrypted).decode().strip('='))
