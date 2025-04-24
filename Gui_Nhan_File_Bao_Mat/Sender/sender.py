import os
import socket
import base64
import json
import time
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP, DES
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA512
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad

# Địa chỉ và cổng kết nối đến Receiver
HOST = '127.0.0.1'
PORT = 65432

# Bước khởi tạo khóa RSA cho Sender (chạy một lần)
if not os.path.exists('sender_private.pem'):
    key = RSA.generate(2048)
    with open('sender_private.pem', 'wb') as f:
        f.write(key.export_key())
    with open('sender_public.pem', 'wb') as f:
        f.write(key.publickey().export_key())
    print('Sender: Đã tạo cặp khóa RSA.')
    time.sleep(2)

# Kết nối đến Receiver
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print(f'Sender: Đã kết nối tới {HOST}:{PORT}')
    time.sleep(2)

    # Bước 1: Handshake đơn giản
    print('Sender: Đang thực hiện BƯỚC 1 - Handshake...')
    s.sendall(b'HELLO')
    print('Sender: Gửi HELLO')
    time.sleep(2)
    data = s.recv(5)
    if data == b'READY':
        print('Sender: Nhận READY từ Receiver')
        time.sleep(2)
    else:
        print('Sender: Handshake thất bại')
        time.sleep(2)
        exit()

    # Nhận PublicKey của Receiver
    print('Sender: Đang thực hiện BƯỚC 2 - Nhận PublicKey từ Receiver...')
    size = int.from_bytes(s.recv(4), 'big')
    pub_R_data = s.recv(size)
    with open('receiver_public.pem', 'wb') as f:
        f.write(pub_R_data)
    print('Sender: Đã nhận PublicKey_R')
    time.sleep(2)

    # Load khóa
    pub_R = RSA.import_key(pub_R_data)
    priv_S = RSA.import_key(open('sender_private.pem', 'rb').read())

    # Bước 2: Chuẩn bị metadata và SessionKey
    print('Sender: Đang thực hiện BƯỚC 2 - Tạo SessionKey và chữ ký metadata...')
    timestamp = int(time.time())
    metadata = f'email.txt|{timestamp}'
    # Tạo chữ ký metadata bằng RSA/SHA-512
    h_meta = SHA512.new(metadata.encode())
    sig = pkcs1_15.new(priv_S).sign(h_meta)

    # Tạo SessionKey (DES)
    session_key = get_random_bytes(8)
    # Mã hóa SessionKey bằng RSA
    esk = PKCS1_OAEP.new(pub_R).encrypt(session_key)
    print('Sender: SessionKey được tạo và mã hóa')
    time.sleep(2)

    # Bước 3: Mã hóa file bằng DES
    print('Sender: Đang thực hiện BƯỚC 3 - Mã hóa dữ liệu file...')
    data_plain = open('email.txt', 'rb').read()
    iv = get_random_bytes(8)  # Tham số khởi tạo cho DES
    cipher = DES.new(session_key, DES.MODE_CBC, iv).encrypt(pad(data_plain, DES.block_size))
    print('Sender: File được mã hóa bằng DES')
    time.sleep(2)

    # Tính hash SHA-512 của ciphertext
    h_cipher = SHA512.new(cipher).hexdigest()
    print('Sender: Tính toán hash của ciphertext')
    time.sleep(2)

    # Đóng gói gửi JSON
    packet = json.dumps({
        'esk': base64.b64encode(esk).decode(),
        'sig': base64.b64encode(sig).decode(),
        'metadata': metadata,
        'iv': base64.b64encode(iv).decode(),
        'cipher': base64.b64encode(cipher).decode(),
        'hash': h_cipher
    }).encode()
    s.sendall(len(packet).to_bytes(4, 'big') + packet)
    print('Sender: Gửi session key, chữ ký và ciphertext')
    time.sleep(2)

    # Đợi phản hồi ACK/NACK từ Receiver
    resp = s.recv(4)
    print(f'Sender: Nhận phản hồi {resp.decode()} từ Receiver')
    time.sleep(2)
