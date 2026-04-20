# Threat Model - Lab 3: DES qua Socket

## 1. Assets (Tài sản cần bảo vệ)

- [cite_start]Plaintext của người dùng[cite: 100].
- [cite_start]Khóa DES[cite: 101].
- [cite_start]IV[cite: 102].
- [cite_start]Log hệ thống[cite: 103].

## 2. Attacker model (Mô hình kẻ tấn công)

- [cite_start]Có khả năng nghe lén trong cùng mạng LAN[cite: 107].
- [cite_start]Có thể sửa dữ liệu trên đường truyền hoặc gửi gói tin lỗi/giả mạo[cite: 111].
- [cite_start]Có thể làm gián đoạn kết nối[cite: 112].

## 3. Threats (Mối đe dọa)

- [cite_start]Lộ khóa DES vì gửi dưới dạng plaintext[cite: 115].
- [cite_start]Sửa đổi ciphertext trên đường truyền gây lỗi giải mã (Padding error)[cite: 116].
- [cite_start]Giả mạo độ dài gói tin ở Header để làm receiver đọc sai kích thước thực tế[cite: 117].

## 4. Mitigations (Biện pháp giảm thiểu)

- [cite_start]Không truyền khóa ở dạng plaintext, sử dụng cơ chế trao đổi khóa an toàn hơn (như Diffie-Hellman hoặc dùng TLS) [cite: 120-121].
- [cite_start]Thêm xác thực toàn vẹn (MAC/HMAC)[cite: 122].
- [cite_start]Ràng buộc kiểm tra độ dài dữ liệu, đặt timeout và xử lý exception rõ ràng khi nhận gói tin [cite: 123-124].

## 5. Residual risks (Rủi ro còn lại)

- [cite_start]Máy người dùng bị compromise (nhiễm malware)[cite: 127].
- [cite_start]Môi trường mạng nội bộ vẫn có thể bị sniffing nếu các biện pháp mã hóa khóa không được triển khai đầy đủ[cite: 129].
