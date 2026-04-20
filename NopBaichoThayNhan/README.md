# Báo cáo Lab 3: Hệ thống gửi và nhận dữ liệu mã hóa bởi DES qua Socket

## Giới thiệu

Hệ thống gồm 2 thành phần Sender và Receiver, thực hiện truyền file qua mạng LAN bằng giao thức TCP socket, mã hóa dữ liệu bằng DES-CBC với padding PKCS#7.

## Hướng dẫn chạy

1. Cài đặt thư viện: `pip install pycryptodome`
2. Chạy Server: `python receiver.py`. Chọn thư mục lưu và bấm Start Server.
3. Chạy Client: `python sender.py`. Nhập IP của Server (vd: 172.16.5.211), chọn file và bấm Connect & Send.

## Ethics & Safe use

- [cite_start]Bài thực hành này chỉ thử nghiệm trên localhost hoặc mạng học tập được phép[cite: 134].
- [cite_start]Chỉ dùng dữ liệu giả lập để truyền tải[cite: 135].
- [cite_start]Không quét cổng hay gửi dữ liệu tới máy lạ ngoài phạm vi bài học[cite: 136].
- [cite_start]Không sử dụng các đoạn mã trong bài này để nghe lén, sửa gói hay tấn công hệ thống thật[cite: 137].
