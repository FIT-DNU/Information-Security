# 🚀 Ví dụ mẫu cơ bản: Mô phỏng Gửi và Nhận Dữ liệu Bảo Mật

![Python](https://img.shields.io/badge/python-3.6%2B-blue) ![PyCryptodome](https://img.shields.io/badge/pycryptodome-required-orange)

## 🎯 Mục Tiêu
- **Minh họa** quy trình bảo mật file qua mạng:  
  1. Handshake  
  2. Xác thực & Trao đổi khóa  
  3. Truyền dữ liệu & Kiểm tra tính toàn vẹn
- **Thực hành** các khái niệm: Authentication, Confidentiality, Integrity.

## 🔍 Tổng Quan
Hai script Python chạy trên 2 máy riêng biệt:
- **sender.py**: Máy Gửi chịu trách nhiệm khởi tạo kết nối, xác thực và mã hóa dữ liệu.  
- **receiver.py**: Máy Nhận lắng nghe kết nối, giải mã và kiểm tra tính toàn vẹn.

## 📝 Yêu Cầu
- Python >= 3.6
- Thư viện **PyCryptodome**
- Kết nối socket TCP (cổng mặc định: 65432)

## ⚙️ Cài Đặt
```bash
# 1. Tạo virtual environment (tuỳ chọn)
python3 -m venv venv
source venv/bin/activate    # Linux/macOS
venv\Scripts\activate      # Windows

# 2. Cài đặt thư viện
pip install pycryptodome
```

## 📂 Cấu Trúc Thư Mục
```plaintext
# Máy Sender:
├─ sender.py
├─ sender_private.pem    # sinh lần đầu khi chạy sender.py
├─ sender_public.pem     # sinh lần đầu khi chạy sender.py
└─ email.txt             # file cần gửi

# Máy Receiver:
├─ receiver.py
├─ receiver_private.pem  # sinh lần đầu khi chạy receiver.py
├─ receiver_public.pem   # sinh lần đầu khi chạy receiver.py
└─ received_email.txt
``` 

## 🚀 Cách Chạy
1. **Máy Receiver** (lắng nghe kết nối):
   ```bash
   python3 receiver.py
   ```
2. **Máy Sender** (gửi dữ liệu, sau khi Receiver sẵn sàng):
   ```bash
   python3 sender.py <IP_Receiver>
   ```
3. _Theo dõi log in ra_ trên mỗi máy (delay 2s giữa các bước).

## 📋 Luồng Hoạt Động
```mermaid
sequenceDiagram
  participant S as Sender
  participant R as Receiver

  Note over S,R: Bước 1 – Handshake
  S->>R: HELLO
  R-->>S: READY!

  Note over S,R: Bước 2 – Xác thực & Trao đổi khóa
  R-->>S: PublicKey_R
  S->>R: E_RSA(SessionKey) + Signature(metadata)
  Note over S,R: ✔ SessionKey đã chia sẻ

  Note over S,R: Bước 3 – Truyền dữ liệu & Kiểm tra
  S->>R: Ciphertext + Hash
  alt Hash hợp lệ
    R-->>S: ACK
  else
    R-->>S: NACK
  end
``` 

## 🔧 Tuỳ Chỉnh & Mở Rộng
Handshake: có thể đơn giản hóa bằng CLI hoặc GUI cơ bản, ví dụ:
- CLI: người dùng nhấn Enter để gửi "HELLO", bên nhận nhấn Enter để gửi "READY!"
- GUI: hai nút "Connect" (gửi HELLO) và "Accept" (gửi READY!)

---
*Created by Nguyen Van Nhan*
[![AIoTLab](https://img.shields.io/badge/AIoTLab-green?style=for-the-badge)](https://www.facebook.com/DNUAIoTLab)
[![Faculty of Information Technology](https://img.shields.io/badge/Faculty%20of%20Information%20Technology-blue?style=for-the-badge)](https://dainam.edu.vn/vi/khoa-cong-nghe-thong-tin)
[![DaiNam University](https://img.shields.io/badge/DaiNam%20University-orange?style=for-the-badge)](https://dainam.edu.vn)

