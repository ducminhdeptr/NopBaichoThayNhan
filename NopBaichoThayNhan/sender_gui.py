import socket
import threading
import struct
import os
import tkinter as tk
from tkinter import filedialog, scrolledtext, messagebox
from tkinter import ttk
from Crypto.Cipher import DES
from Crypto.Util.Padding import pad
from Crypto.Random import get_random_bytes

class SenderGUI:
    def __init__(self, master):
        self.master = master
        self.master.title("Sender - Lab 3 DES")
        self.master.geometry("500x450")

        # UI Elements
        tk.Label(master, text="IP Receiver:").place(x=20, y=20)
        self.ip_entry = tk.Entry(master)
        self.ip_entry.insert(0, "127.0.0.1")
        self.ip_entry.place(x=90, y=20, width=100)

        tk.Label(master, text="Port:").place(x=210, y=20)
        self.port_entry = tk.Entry(master)
        self.port_entry.insert(0, "9999")
        self.port_entry.place(x=250, y=20, width=60)

        self.btn_file = tk.Button(master, text="Chọn file để gửi", command=self.choose_file)
        self.btn_file.place(x=20, y=60)
        self.lbl_file = tk.Label(master, text="Chưa chọn file")
        self.lbl_file.place(x=130, y=65)

        self.btn_send = tk.Button(master, text="Connect & Send", command=self.start_sending, bg="lightblue")
        self.btn_send.place(x=350, y=15, height=30)

        self.progress = ttk.Progressbar(master, orient="horizontal", length=460, mode="determinate")
        self.progress.place(x=20, y=110)

        tk.Label(master, text="System Log:").place(x=20, y=140)
        self.log_area = scrolledtext.ScrolledText(master, width=55, height=15, state='disabled')
        self.log_area.place(x=20, y=160)

        self.filepath = ""
        self.sock = None

    def log(self, message):
        self.master.after(0, self._append_log, message)

    def _append_log(self, message):
        self.log_area.config(state='normal')
        self.log_area.insert(tk.END, message + "\n")
        self.log_area.see(tk.END)
        self.log_area.config(state='disabled')

    def update_progress(self, value):
        self.master.after(0, lambda: self.progress.config(value=value))

    def choose_file(self):
        self.filepath = filedialog.askopenfilename()
        if self.filepath:
            filename = os.path.basename(self.filepath)
            self.lbl_file.config(text=filename)

    def start_sending(self):
        if not self.filepath:
            messagebox.showwarning("Cảnh báo", "Vui lòng chọn file trước!")
            return
        
        self.btn_send.config(state=tk.DISABLED)
        threading.Thread(target=self.send_thread, daemon=True).start()

    def send_thread(self):
        ip = self.ip_entry.get()
        port_str = self.port_entry.get()
        self.sock = None
        
        try:
            port = int(port_str)
        except ValueError:
            self.log("[!] Lỗi: Port phải là số!")
            self.master.after(0, lambda: self.btn_send.config(state=tk.NORMAL))
            return
        
        try:
            self.log("\n[*] Đang đọc và mã hóa file...")
            # 1. Sinh Key và IV
            key = get_random_bytes(8)
            iv = get_random_bytes(8)
            self.log("[*] Đã sinh DES Key và IV mới.")

            # 2. Đọc và đệm dữ liệu (PKCS#7)
            with open(self.filepath, "rb") as f:
                plaintext = f.read()
            
            padded_data = pad(plaintext, DES.block_size)
            self.log("[*] Đã padding dữ liệu theo chuẩn PKCS#7.")

            # 3. Mã hóa DES-CBC
            cipher = DES.new(key, DES.MODE_CBC, iv)
            ciphertext = cipher.encrypt(padded_data)
            cipher_len = len(ciphertext)
            self.log(f"[*] Mã hóa thành công. Độ dài Ciphertext: {cipher_len} bytes.")

            # --- KẾT NỐI SOCKET VÀ GỬI ---
            self.log(f"[*] Đang kết nối tới {ip}:{port}...")
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.settimeout(10)
            self.sock.connect((ip, port))
            self.log("[+] Kết nối thành công!")

            # BƯỚC 1 & 2: Gửi Key (8 bytes) + IV (8 bytes)
            self.sock.sendall(key + iv)
            self.log("[*] Đã gửi Key và IV.")

            # BƯỚC 3: Gửi Header (4 bytes)
            header = struct.pack('!I', cipher_len)
            self.sock.sendall(header)
            self.log("[*] Đã gửi Header (4 bytes).")

            # BƯỚC 4: Gửi Ciphertext (chia chunk để UI mượt)
            self.log("[*] Đang gửi Ciphertext...")
            chunk_size = 4096
            sent_length = 0
            self.update_progress(0)

            for i in range(0, cipher_len, chunk_size):
                chunk = ciphertext[i:i+chunk_size]
                self.sock.sendall(chunk)
                sent_length += len(chunk)
                
                percent = (sent_length / cipher_len) * 100
                self.update_progress(percent)

            self.log("[+] Đã gửi file thành công!")

        except ConnectionRefusedError:
            self.log("[!] Lỗi: Không thể kết nối tới Receiver (Bị từ chối).")
        except socket.timeout:
            self.log("[!] Lỗi: Timeout kết nối (Vượt quá 10s).")
        except FileNotFoundError:
            self.log("[!] Lỗi: File không tìm thấy!")
        except Exception as e:
            self.log(f"[!] Lỗi: {str(e)}")
        finally:
            if self.sock:
                try:
                    self.sock.close()
                except:
                    pass
            self.log("[*] Kết thúc phiên gửi.")
            self.master.after(0, lambda: self.btn_send.config(state=tk.NORMAL))

if __name__ == "__main__":
    root = tk.Tk()
    app = SenderGUI(root)
    root.mainloop()