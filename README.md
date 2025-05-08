# WildVision: Deteksi Gerakan dengan ESP32-CAM 🚶‍♂️📸
![RANGKAIAN](https://github.com/user-attachments/assets/c5d26dc3-250e-4bcf-89cc-b6228d6ea0f0)



## Prototype Hardware untuk kedepannya!
![HARDWARE WILDVISION](https://github.com/user-attachments/assets/f6956ed9-dfae-4120-970b-5679f3911d36)

Selamat datang di **WildVision**! 🌟 Proyek ini menggunakan **ESP32-CAM** untuk menangkap foto ketika ada gerakan terdeteksi oleh sensor PIR. Foto yang diambil akan dikirimkan ke server melalui WiFi. Mari kita lihat bagaimana cara kerjanya! 🔥

## Apa yang Dibutuhkan? 🛠️

- **ESP32-CAM**: Kamera mini dengan WiFi yang sangat keren.
- **Sensor PIR**: Untuk mendeteksi gerakan, seperti ninja yang diam-diam lewat! 🕵️‍♂️
- **LED Flash**: Untuk menambahkan efek dramatis saat foto diambil! 💡
- **WiFi**: Untuk mengirim foto ke server—pastikan koneksi internetmu stabil ya! 🌐

## Fitur Utama 🚀

- **Deteksi Gerakan**: Sensor PIR akan mendeteksi gerakan dan memicu pengambilan foto otomatis. 🕺
- **Kamera ESP32-CAM**: Menggunakan kamera berkualitas untuk mengambil gambar dalam format JPEG 📸.
- **Kirim Foto ke Server**: Foto akan dikirim ke server yang telah ditentukan. 📤
- **LED Indikator**: LED menyala saat foto dikirim untuk memberi tahu bahwa foto sedang dikirim! 💡
- **WiFi**: ESP32-CAM akan terhubung ke WiFi untuk mengirim foto. 📶

## Bagaimana Cara Kerja? 🤔

1. **Mulai Kamera**: ESP32-CAM diinisialisasi dengan konfigurasi pin yang telah ditentukan.
2. **Deteksi Gerakan**: Ketika sensor PIR mendeteksi gerakan, kamera akan segera mengambil foto.
3. **Kirim Foto**: Foto dikirim ke server melalui WiFi menggunakan HTTP POST.
4. **Indikator LED**: LED indikator menyala saat foto dikirim dan mati setelah selesai.

## Langkah-langkah Pemasangan 💻

1. **Pasang Kode ke ESP32-CAM**:
   - Upload kode **`wildvision.ino`** ke board ESP32-CAM menggunakan Arduino IDE.
   
2. **Konfigurasi WiFi**:
   - Ganti `ssid` dan `password` dengan nama WiFi dan password jaringan Anda di kode.

3. **Server**:
   - Tentukan URL server untuk menerima foto pada variabel `serverUrl`. Gantilah `server:port/upload` dengan alamat server Anda.

4. **Sensor PIR**:
   - Sambungkan sensor PIR ke pin GPIO 15 pada ESP32-CAM.

5. **Flash dan LED**:
   - Sambungkan LED indikator ke pin GPIO 12 untuk menandakan pengiriman foto.

6. **Jalankan**:
   - Setelah semua terpasang, nyalakan ESP32-CAM dan pastikan WiFi terhubung.

## Penjelasan Kode 📚

### 1. Inisialisasi Kamera 📸
Di fungsi `startCamera()`, kami mengonfigurasi pin-pin kamera ESP32-CAM dan menginisialisasi kamera dengan resolusi yang sesuai berdasarkan apakah **PSRAM** tersedia.

### 2. Pengambilan Foto 📷
Setelah mendeteksi gerakan dengan sensor PIR, fungsi `takePhoto()` akan dipanggil untuk mengambil foto dan mengirimkannya ke server.

### 3. Pengiriman Foto 🌐
Fungsi `sendPhoto()` akan mengirimkan foto ke server menggunakan metode POST dan menampilkan hasil status pengiriman melalui serial monitor.

### 4. WiFi & LED Indikator 🌟
Saat ESP32-CAM terhubung ke WiFi, LED indikator akan menyala untuk memberi tahu bahwa foto sedang diproses dan dikirim.

## Troubleshooting ⚠️

- **Tidak Ada Gambar yang Dikirim**: Pastikan koneksi WiFi Anda stabil dan server URL sudah benar.
- **Sensor PIR Tidak Berfungsi**: Cek sambungan pin PIR pada board ESP32-CAM dan pastikan sensor PIR berfungsi dengan baik.

## Lisensi 📜

Proyek ini bersifat **Open-Source** dan dapat digunakan sesuai dengan **MIT License**. 🎉
