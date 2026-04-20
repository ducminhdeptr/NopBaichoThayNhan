import socket
import threading
import struct
import os
import tkinter as tk
from tkinter import filedialog, scrolledtext, messagebox
from tkinter import ttk
from Crypto.Cipher import DES
from Crypto.Util.Padding import unpad

class ReceiverGUI:
    def __init__(self, master):
        self.master = master
        self.master.title("Receiver - Lab 3")
        self.master.geometry("550x500")

        tk.Label(master, text="IP Bind:").place(x=20, y=20)
        self.ip_entry = tk.Entry(master)
        self.ip_entry.insert(0, "0.0.0.0")
        self.ip_entry.place(x=80, y=20, width=100)

        tk.Label(master, text="Port:").place(x=200, y=20)
        self.port_entry = tk.Entry(master)
        self.port_entry.insert(0, "9999")
        self.port_entry.place(x=240, y=20, width=60)

        self.btn_folder = tk.Button(master, text="Chọn thư mục lưu", command=self.choose_folder)
        self.btn_folder.place(x=20, y=60)
        self.lbl_folder = tk.Label(master, text="Chưa chọn thư mục")
        self.lbl_folder.place(x=150, y=65)

        self.btn_listen = tk.Button(master, text="Start Server", command=self.start_listening, bg="lightgreen")
        self.btn_listen.place(x=350, y=15, height=30)

        tk.Label(master, text="System Log:").place(x=20, y=100)
        self.log_area = scrolledtext.ScrolledText(master, width=60, height=20, state='disabled')
        self.log_area.place(x=20, y=120)

        self.save_folder = ""
        self.is_listening = False

    def log(self, message):
        self.master.after(0, self._append_log, message)

    def _append_log(self, message):
        self.log_area.config(state='normal')
        self.log_area.insert(tk.END, message + "\n")
        self.log_area.see(tk.END)
        self.log_area.config(state='disabled')

    def choose_folder(self):
        self.save_folder = filedialog.askdirectory()
        if self.save_folder:
            self.lbl_folder.config(text=self.save_folder)

    def start_listening(self):
        if not self.save_folder:
            messagebox.showwarning("Cảnh báo", "Vui lòng chọn thư mục lưu!")
            return
        if not self.is_listening:
            self.is_listening = True
            self.btn_listen.config(state=tk.DISABLED)
            threading.Thread(target=self.server_thread, daemon=True).start()

    def server_thread(self):
        ip = self.ip_entry.get()
        port_str = self.port_entry.get()
        
        try:
            port = int(port_str)
        except ValueError:
            self.log("[!] Lỗi: Port phải là số!")
            self.is_listening = False
            self.master.after(0, lambda: self.btn_listen.config(state=tk.NORMAL))
            return
        
        server_sock = None
        try:
            server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server_sock.bind((ip, port))
            server_sock.listen(5)
            self.log(f"[*] Đang lắng nghe trên {ip}:{port}...")
            
            while self.is_listening:
                try:
                    server_sock.settimeout(1)
                    conn, addr = server_sock.accept()
                    self.log(f"\n[+] Kết nối từ: {addr}")
                    threading.Thread(target=self.handle_client, args=(conn, addr), daemon=True).start()
                except socket.timeout:
                    continue
                except Exception as e:
                    if self.is_listening:
                        self.log(f"[!] Lỗi khi accept: {str(e)}")
                    break
                    
        except Exception as e:
            self.log(f"[!] Lỗi Server: {str(e)}")
        finally:
            if server_sock:
                try:
                    server_sock.close()
                except:
                    pass
            self.is_listening = False
            self.master.after(0, lambda: self.btn_listen.config(state=tk.NORMAL))
            self.log("[*] Server dừng.")

    def handle_client(self, conn, addr):
        try:
            # 1. Nhận Key và IV (16 bytes)
            key_iv = conn.recv(16)
            if len(key_iv) < 16:
                self.log(f"[{addr}] [!] Lỗi: Không nhận đủ Key và IV (nhận {len(key_iv)} bytes).")
                return
            
            key = key_iv[:8]
            iv = key_iv[8:16]
            self.log(f"[{addr}] [+] Đã nhận DES Key và IV.")

            # 2. Nhận Header (4 bytes)
            header = conn.recv(4)
            if len(header) < 4:
                self.log(f"[{addr}] [!] Lỗi: Không nhận đủ Header 4 byte.")
                return
            
            expected_len = struct.unpack('!I', header)[0]
            self.log(f"[{addr}] [+] Kích thước dữ liệu sẽ nhận: {expected_len} bytes.")

            # 3. Nhận Ciphertext
            ciphertext = b""
            while len(ciphertext) < expected_len:
                chunk = conn.recv(min(4096, expected_len - len(ciphertext)))
                if not chunk:
                    break
                ciphertext += chunk

            if len(ciphertext) < expected_len:
                self.log(f"[{addr}] [!] Lỗi: Ciphertext bị cắt cụt (nhận {len(ciphertext)}/{expected_len}).")
                return

            self.log(f"[{addr}] [+] Đã nhận đủ {len(ciphertext)} bytes.")

            # 4. Giải mã
            cipher = DES.new(key, DES.MODE_CBC, iv)
            decrypted = cipher.decrypt(ciphertext)
            
            # 5. Unpad
            try:
                plaintext = unpad(decrypted, DES.block_size)
                self.log(f"[{addr}] [+] Giải mã thành công. Dữ liệu gốc: {len(plaintext)} bytes.")
            except ValueError as ve:
                self.log(f"[{addr}] [!] Lỗi Unpad: {str(ve)}")
                return

            # 6. Lưu file
            filename = f"received_{addr[0]}_{addr[1]}.dat"
            filepath = os.path.join(self.save_folder, filename)
            with open(filepath, "wb") as f:
                f.write(plaintext)
            
            self.log(f"[{addr}] [+] Lưu file thành công: {filename}")

        except ConnectionResetError:
            self.log(f"[{addr}] [!] Lỗi: Kết nối bị đóng đột ngột.")
        except Exception as e:
            self.log(f"[{addr}] [!] Lỗi: {str(e)}")
        finally:
            try:
                conn.close()
            except:
                pass

if __name__ == "__main__":
    root = tk.Tk()
    app = ReceiverGUI(root)
    root.mainloop()