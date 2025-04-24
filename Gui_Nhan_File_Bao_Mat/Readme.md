##Ví dụ mẫu cơ bản: Mô phỏng Gửi và Nhận Dữ liệu Bảo Mật

1. Mô tả

Dự án gồm hai script Python chạy trên hai máy riêng biệt:

sender.py (chạy trên máy Sender)

receiver.py (chạy trên máy Receiver)

Quy trình bảo mật:

Handshake đơn giản: Sender gửi “HELLO”, Receiver trả lời “READY!”.

Xác thực & Trao đổi khóa: RSA để ký metadata và mã hóa SessionKey.

Truyền dữ liệu & Kiểm tra tính toàn vẹn: DES mã hóa file, gửi kèm hash SHA-512; Receiver kiểm tra và giải mã.

Mục tiêu: Minh họa ba khái niệm cơ bản trong bảo mật: Authentication, Confidentiality, Integrity.

2. Yêu cầu

Python 3.6+

Thư viện PyCryptodome

Kết nối TCP qua cổng 65432 từ Sender → Receiver

3. Cài đặt môi trường

# (Tuỳ chọn) Tạo virtual environment
python3 -m venv venv
source venv/bin/activate    # Linux/macOS
venv\Scripts\activate      # Windows

# Cài thư viện cần thiết
pip install pycryptodome

4. Cấu trúc thư mục trên mỗi máy

# Máy Sender:
sender.py
sender_private.pem    # sinh lần đầu khi chạy sender.py
sender_public.pem     # sinh lần đầu khi chạy sender.py
email.txt             # file cần gửi

# Máy Receiver:
receiver.py
receiver_private.pem  # sinh lần đầu khi chạy receiver.py
receiver_public.pem   # sinh lần đầu khi chạy receiver.py

5. Cách chạy

Máy Receiver:

python3 receiver.py

Tạo RSA keys (nếu chưa có) và lắng nghe cổng 65432.

Máy Sender (sau khi Receiver sẵn sàng):

python3 sender.py <IP_Receiver>

Thay <IP_Receiver> bằng địa chỉ IP của máy Receiver.

Quan sát log in ra trên mỗi máy, có delay 2s giữa các bước.
