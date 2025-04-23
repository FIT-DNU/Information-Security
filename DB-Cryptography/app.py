from flask import Flask, render_template, request, redirect, url_for, session, flash
import pyodbc

app = Flask(__name__)
app.secret_key = 'your_secret_key'  # bắt buộc cho session

# Cấu hình kết nối SQL Server
conn_str = (
    'DRIVER={ODBC Driver 17 for SQL Server};'
    'SERVER=localhost;'
    'DATABASE=DB_Cryptography;'
    'UID=username;'
    'PWD=password'
)

@app.route('/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']

        # Kết nối đến SQL Server
        conn = pyodbc.connect(conn_str)
        cursor = conn.cursor()

        query = "SELECT * FROM Account WHERE username = ? AND password = ?"
        cursor.execute(query, (username, password))
        user = cursor.fetchone()

        if user:
            session['username'] = username
            return redirect(url_for('dashboard'))
        else:
            flash('Sai tên đăng nhập hoặc mật khẩu')
        conn.close()

    return render_template('login.html')

@app.route('/logout')
def logout():
    session.pop('username', None)
    return redirect(url_for('login'))


@app.route('/dashboard')
def dashboard():
    if 'username' not in session:
        return redirect(url_for('login'))

    conn = pyodbc.connect(conn_str)
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM NhanVien")
    rows = cursor.fetchall()

    # Biến đổi dữ liệu thành dict
    nhanviens = [{
        'id': row.ID,
        'hoTen': row.Ho_Ten,
        'ngaySinh': row.Ngay_Sinh.strftime('%Y-%m-%d'),
        'queQuan': row.Que_Quan,
        'noBHXH': row.No_BHXH
    } for row in rows]

    conn.close()
    return render_template('dashboard.html', nhanviens=nhanviens)

@app.route('/add', methods=['GET', 'POST'])
def add_employee():
    if request.method == 'POST':
        hoTen = request.form['hoTen']
        ngaySinh = request.form['ngaySinh']
        queQuan = request.form['queQuan']
        noBHXH = request.form['noBHXH']

        conn = pyodbc.connect(conn_str)
        cursor = conn.cursor()
        cursor.execute(
            "INSERT INTO NhanVien (Ho_Ten, Ngay_Sinh, Que_Quan, No_BHXH) VALUES (?, ?, ?, ?)",
            (hoTen, ngaySinh, queQuan, noBHXH)
        )
        conn.commit()
        conn.close()
        return redirect(url_for('dashboard'))

    return render_template('form.html', action='Thêm', nv=None)

@app.route('/edit/<int:id>', methods=['GET', 'POST'])
def edit_employee(id):
    conn = pyodbc.connect(conn_str)
    cursor = conn.cursor()

    if request.method == 'POST':
        hoTen = request.form['hoTen']
        ngaySinh = request.form['ngaySinh']
        queQuan = request.form['queQuan']
        noBHXH = request.form['soSoBHXH']

        cursor.execute(
            "UPDATE NhanVien SET Ho_Ten=?, Ngay_Sinh=?, Que_Quan=?, No_BHXH=? WHERE id=?",
            (hoTen, ngaySinh, queQuan, noBHXH, id)
        )
        conn.commit()
        conn.close()
        return redirect(url_for('dashboard'))

    cursor.execute("SELECT * FROM NhanVien WHERE id=?", (id,))
    row = cursor.fetchone()

    nv = {
        'id': row.id,
        'hoTen': row.hoTen,
        'ngaySinh': row.ngaySinh.strftime('%Y-%m-%d'),
        'queQuan': row.queQuan,
        'soSoBHXH': row.soSoBHXH
    }
    conn.close()
    return render_template('form.html', action='Sửa', nv=nv)

@app.route('/delete/<int:id>')
def delete_employee(id):
    conn = pyodbc.connect(conn_str)
    cursor = conn.cursor()
    cursor.execute("DELETE FROM NhanVien WHERE id=?", (id,))
    conn.commit()
    conn.close()
    return redirect(url_for('dashboard'))


if __name__ == '__main__':
    app.run(debug=True)