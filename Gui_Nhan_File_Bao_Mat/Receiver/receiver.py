import os
import socket
import base64
import json
import time
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP, DES
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA512
from Crypto.Util.Padding import unpad

# Địa chỉ và cổng lắng nghe
HOST = '127.0.0.1'
PORT = 65432

# Bước khởi tạo khóa RSA cho Receiver (chạy một lần)
if not os.path.exists('receiver_private.pem'):
    key = RSA.generate(2048)
    with open('receiver_private.pem', 'wb') as f:
        f.write(key.export_key())
    with open('receiver_public.pem', 'wb') as f:
        f.write(key.publickey().export_key())
    print('Receiver: Đã tạo cặp khóa RSA.')
    time.sleep(2)

# Bắt đầu lắng nghe kết nối từ Sender
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    print(f'Receiver: Đợi kết nối tại {HOST}:{PORT}...')
    time.sleep(2)
    conn, addr = s.accept()
    with conn:
        print('Receiver: Đã kết nối với', addr)
        time.sleep(2)

        # Bước 1: Handshake đơn giản
        print('Receiver: Đang thực hiện BƯỚC 1 - Handshake...')
        data = conn.recv(5)
        if data == b'HELLO':
            print('Receiver: Nhận HELLO từ Sender')
            time.sleep(2)
            conn.sendall(b'READY')
            print('Receiver: Gửi READY cho Sender')
            time.sleep(2)
        else:
            print('Receiver: Handshake không hợp lệ')
            time.sleep(2)
            conn.close()
            exit()

        # Bước 2: Gửi public key RSA của Receiver
        print('Receiver: Đang thực hiện BƯỚC 2 - Gửi PublicKey RSA...')
        pub_R = open('receiver_public.pem', 'rb').read()
        conn.sendall(len(pub_R).to_bytes(4, 'big') + pub_R)
        print('Receiver: Đã gửi PublicKey_R cho Sender')
        time.sleep(2)

        # Nhận gói tin JSON từ Sender
        print('Receiver: Đang nhận dữ liệu từ Sender...')
        size = int.from_bytes(conn.recv(4), 'big')
        packet = conn.recv(size)
        data = json.loads(packet)
        esk = base64.b64decode(data['esk'])  # Khóa phiên đã được mã hóa
        sig = base64.b64decode(data['sig'])  # Chữ ký của metadata
        iv = base64.b64decode(data['iv'])    # Tham số khởi tạo (nếu có)
        cipher = base64.b64decode(data['cipher'])
        hash_orig = data['hash']             # Giá trị băm gốc

        # Load khóa RSA
        priv_R = RSA.import_key(open('receiver_private.pem', 'rb').read())
        pub_S = RSA.import_key(open('sender_public.pem', 'rb').read())

        # Bước 3: Giải mã SessionKey
        print('Receiver: Đang thực hiện BƯỚC 3 - Giải mã SessionKey...')
        session_key = PKCS1_OAEP.new(priv_R).decrypt(esk)
        print('Receiver: Giải mã SessionKey thành công')
        time.sleep(2)

        # Bước 3: Xác thực chữ ký metadata
        print('Receiver: Đang thực hiện BƯỚC 3 - Xác thực chữ ký metadata...')
        metadata = data['metadata'].encode()
        try:
            h_meta = SHA512.new(metadata)
            pkcs1_15.new(pub_S).verify(h_meta, sig)
            print('Receiver: Chữ ký metadata hợp lệ')
            time.sleep(2)
        except (ValueError, TypeError):
            print('Receiver: Chữ ký metadata không hợp lệ!')
            time.sleep(2)
            exit()

        # Bước 3: Kiểm tra tính toàn vẹn
        print('Receiver: Đang thực hiện BƯỚC 3 - Kiểm tra tính toàn vẹn dữ liệu...')
        h_cipher = SHA512.new(cipher).hexdigest()
        if h_cipher != hash_orig:
            print('Receiver: Kiểm tra toàn vẹn thất bại')
            time.sleep(2)
            conn.sendall(b'NACK')
            exit()
        print('Receiver: Kiểm tra toàn vẹn thành công')
        time.sleep(2)

        # Bước 3: Giải mã dữ liệu và lưu file
        print('Receiver: Đang thực hiện BƯỚC 3 - Giải mã và lưu file...')
        plaintext = DES.new(session_key, DES.MODE_CBC, iv).decrypt(cipher)
        plaintext = unpad(plaintext, DES.block_size)
        with open('received_email.txt', 'wb') as f:
            f.write(plaintext)
        print('Receiver: Đã lưu file received_email.txt')
        time.sleep(2)

        # Phản hồi ACK cho Sender
        conn.sendall(b'ACK')
        print('Receiver: Gửi ACK (Nhận thành công)')
        time.sleep(2)
