# SISTEM PENYEWAAN ALAT CAMPING (CAMPING GEAR RENTAL HUB)
**Proyek Akhir Pemrograman C++ (Tim 3 Orang) — Pendekatan Modular & Berkas Terintegrasi**

---

## 🏕️ Ringkasan Proyek
**Sistem Penyewaan Alat Camping** adalah aplikasi berbasis konsol (CLI) berperforma tinggi yang dibangun menggunakan standar modern **C++17**. Aplikasi ini dirancang untuk mendigitalisasi operasional bisnis rental perlengkapan outdoor mulai dari katalog alat, keranjang sewa, sistem reservasi, kalkulasi diskon voucher promo, kalkulasi uang jaminan (deposit), verifikasi multi-metode pembayaran, alur pengembalian dengan inspeksi denda keterlambatan/kerusakan, hingga dashboard eksekutif dan pelaporan keuangan laba rugi.

Sesuai dengan instruksi panduan pengerjaan kelompok:
1. **Pendekatan Modular (3 Bagian / Layer):** Kode dibagi menjadi 3 folder anggota tim yang masing-masing memiliki tanggung jawab, file `.h`, file `.cpp`, file unit testing mandiri `main_test.cpp`, dan log laporan prompt AI.
2. **Berkas Terintegrasi Tunggal (`main_gabungan.cpp`):** Menggabungkan seluruh fungsionalitas 3 modul ke dalam 1 file mandiri dengan ukuran **5.001 baris kode (clean lines)** yang dapat langsung dikompilasi dan dijalankan secara lancar tanpa *dependency* eksternal.

---

## 👥 Pembagian Tugas Anggota Tim (Pendekatan Modular)

| Anggota | Modul / Layer | Tanggung Jawab Utama | Berkas Kerja |
| :--- | :--- | :--- | :--- |
| **Anggota 1** | **Data Layer** | Perancangan class entitas data, persistensi file `.txt`, serialisasi/deserialisasi objek, inisialisasi repositori CRUD dan auto-seeding data awal. | `models.h`, `data_manager.h`, `data_manager.cpp`, `main_test.cpp`, `LAPORAN_ANGGOTA_1.md` |
| **Anggota 2** | **Business Logic Layer** | Algoritma Sorting (*MergeSort*, *QuickSort*, *BubbleSort*), Searching (*Binary Search*, *Linear Search*), validasi input ketat, aturan checkout, kalkulasi denda dan deposit, serta reporting. | `business_logic.h`, `business_logic.cpp`, `main_test.cpp`, `LAPORAN_ANGGOTA_2.md` |
| **Anggota 3** | **UI & Controller Layer** | Desain visual antarmuka konsol (CLI) dengan border ASCII box drawing, formatter Rupiah, tabel paginasi, invoice resmi, alur controller navigasi, dan integrator akhir. | `ui_views.h`, `ui_views.cpp`, `controller.h`, `controller.cpp`, `main_menu_mock.cpp`, `LAPORAN_ANGGOTA_3.md` |

---

## 🚀 Fitur Lengkap Aplikasi

### 👤 Fitur Pengguna (Pelanggan / User)
1. **Registrasi dan Login:**
   - Validasi ketat format username, password, email, nomor HP, dan NIK/KTP asli.
   - Enkripsi password hashing FNV-1a dengan salt unik.
   - Manajemen sesi login pelanggan aktif dan profil pengguna.
2. **Katalog Alat Camping (Sorting & Searching):**
   - **Algoritma Sorting (MergeSort):** Urutkan berdasarkan Nama (A-Z / Z-A), Harga Sewa (Terendah / Tertinggi), Rating Bintang, Popularitas sewa, dan Stok terbanyak.
   - **Algoritma Searching:** Pencarian multi-kata kunci (nama, brand, spesifikasi), filter kategori (Tenda, Sleeping Bag, Carrier, Alat Masak, Lampu, Navigasi/Safety, Outdoor Comfort), dan batas maksimal harga sewa.
   - Rincian kartu spesifikasi alat dan ulasan rating bintang dari penyewa sebelumnya.
   - **Paket Hemat Camping (Bundling):** Paket Solo, Duo Adventure, Squad Mountaineer, dan Family Glamping.
3. **Keranjang Penyewaan:**
   - Tambah alat dengan kuantitas dan durasi hari sewa fleksibel.
   - Pengecekan ketersediaan stok real-time (mencegah *overbooking*).
   - Ubah kuantitas, hapus barang individual, atau kosongkan keranjang.
4. **Transaksi Penyewaan (Checkout Wizard):**
   - Pemilihan tanggal mulai sewa dan durasi sewa.
   - Validasi identitas penyewa asli (KTP / KTM / SIM).
   - Engine kupon diskon promo (`CAMPINGASIK10`, `WEEKENDSERU20`, `MEMBERBARU`).
   - Kalkulasi otomatis deposit jaminan (10% nilai barang, min. Rp 50.000).
   - Penerbitan nomor invoice unik format `INV-YYYYMM-XXXX`.
5. **Pembayaran:**
   - Multi-channel: Virtual Account (BCA, Mandiri, BRI), QRIS Instant All Payment, E-Wallet (GoPay, OVO, Dana), dan Tunai di Outlet (COD).
   - Pengunggahan nomor referensi / resi bukti pembayaran.
   - Cetak struk/nota invoice resmi dengan rincian biaya, deposit, dan kolom tanda tangan.
6. **Riwayat Penyewaan & Ulasan:**
   - Riwayat transaksi (Pending, Terverifikasi, Aktif Sewa, Selesai, Dibatalkan).
   - Pemberian rating bintang (1-5) dan testimoni ulasan untuk perlengkapan yang disewa.
   - Sistem loyalitas pelanggan (Tingkat Explorer, Adventurer, Mountaineer, Expedition).

---

### 🛠️ Fitur Administrator (Admin)
1. **Manajemen Inventaris (CRUD, Sorting & Searching):**
   - Tambah alat baru dengan SKU unik otomatis berdasarkan kategori.
   - Edit rincian harga sewa, spesifikasi, dan nilai ganti rugi barang.
   - Restock unit fisik ke dalam toko.
   - Hapus alat dari inventaris (dengan proteksi jika barang sedang disewa).
   - Pencarian cepat dan pengurutan inventaris.
2. **Manajemen Transaksi:**
   - Pantau seluruh transaksi masuk.
   - Verifikasi dan persetujuan pembayaran pelanggan.
   - Serah terima unit (mengubah status menjadi AKTIF SEWA).
   - Pembatalan transaksi (otomatis mengembalikan stok inventaris).
   - Pencarian transaksi berdasarkan nomor invoice atau nama pelanggan.
3. **Pengembalian Alat & Kalkulasi Denda:**
   - Inspeksi kondisi fisik barang: **Baik Sempurna (0%)**, **Kotor Berat (10%)**, **Rusak Ringan (25%)**, **Rusak Berat (60%)**, **Hilang (100%)**.
   - Kalkulasi otomatis denda keterlambatan per hari (150% sewa harian).
   - Rekonsiliasi uang jaminan (deposit): pengembalian sisa deposit atau tagihan kurang bayar jika denda melebihi deposit.
   - Pemulihan stok inventaris otomatis (unit hilang otomatis mengurangi aset total).
   - Penerbitan Berita Acara Pengembalian Resmi.
4. **Manajemen Pelanggan:**
   - Daftar seluruh member terdaftar, kontak, alamat, dan total sewa.
   - Skor reputasi (*Trust Score*) pelanggan.
   - Fitur blokir (*banned*) dan buka blokir akun pelanggan bermasalah.
   - Pemantauan riwayat sewa per pelanggan.
5. **Dashboard & Laporan Keuangan:**
   - Kartu Metrik KPI: Total Omzet, Laba Bersih, Sewa Aktif Berjalan, Total Unit Toko.
   - Rincian Arus Kas: Pendapatan Sewa, Potongan Diskon, Uang Jaminan, Pendapatan Denda Keterlambatan, Pendapatan Denda Kerusakan.
   - Peringkat Top 5 Alat Paling Banyak Disewa.
   - Peringatan Dini Stok Kritis (stok <= 3 unit).
   - Export laporan keuangan lengkap ke berkas teks (`laporan_keuangan_camping.txt`).

---

## 🧠 Algoritma Pemrosesan Data (Computer Science Core)
Sistem ini mengimplementasikan algoritma murni pada class `AlgorithmEngine`:
1. **MergeSort ($O(N \log N)$):** Pengurutan rekursif stabil *divide-and-conquer* untuk katalog alat berdasarkan harga, nama, rating, atau ketersediaan stok.
2. **QuickSort ($O(N \log N)$ average):** Pengurutan data transaksi sewa berdasarkan tanggal pemesanan, nominal omzet, dan nomor invoice menggunakan partisi *Lomuto*.
3. **BubbleSort ($O(N^2)$):** Pengurutan ulasan pelanggan berdasarkan tanggal terbaru.
4. **InsertionSort ($O(N^2)$):** Pengurutan item barang di dalam keranjang belanja berdasarkan subtotal sewa.
5. **Binary Search ($O(\log N)$):** Pencarian biner berbasis kode SKU unik barang pada array terurut.
6. **Linear Search ($O(N)$):** Pencarian multi-kriteria untuk teks substring, filter kategori enum, batas harga, dan ketersediaan unit.

---

## 📁 Struktur Direktori Berkas Proyek

```text
TP/
├── Anggota_1_Data_Layer/
│   ├── models.h                 <- Class entitas data & enum model
│   ├── data_manager.h           <- Deklarasi repositori & persistensi file
│   ├── data_manager.cpp         <- Implementasi CRUD & auto-seeding
│   ├── main_test.cpp            <- Unit testing mandiri Anggota 1 (Hardcoded data)
│   └── LAPORAN_ANGGOTA_1.md     <- Log prompt AI & hasil uji coba unit test
├── Anggota_2_Business_Logic_Layer/
│   ├── business_logic.h         <- Deklarasi algoritma & seluruh business service
│   ├── business_logic.cpp       <- Implementasi sorting, searching, denda, checkout
│   ├── main_test.cpp            <- Unit testing mandiri Anggota 2 (Hardcoded data)
│   └── LAPORAN_ANGGOTA_2.md     <- Log prompt AI & hasil uji coba unit test
├── Anggota_3_UI_Controller_Layer/
│   ├── ui_views.h               <- Tata letak CLI, tabel, box drawing ASCII, invoice
│   ├── ui_views.cpp             <- Implementasi visual konsol & input sanitizer
│   ├── controller.h             <- Deklarasi pengendali alur aplikasi (Controller)
│   ├── controller.cpp           <- Implementasi menu & integrasi layer
│   ├── main_menu_mock.cpp       <- Driver uji coba visual menu Anggota 3
│   └── LAPORAN_ANGGOTA_3.md     <- Log prompt AI & hasil uji coba visual UI
├── data/
│   ├── users.txt                <- Berkas data pengguna persisten
│   ├── gears.txt                <- Berkas katalog inventaris alat camping
│   ├── transactions.txt         <- Berkas transaksi penyewaan
│   ├── returns.txt              <- Berkas log pengembalian & denda
│   └── reviews.txt              <- Berkas ulasan & rating pelanggan
├── main.cpp                     <- Titik masuk aplikasi versi modular terintegrasi
├── main_gabungan.cpp            <- 1 Berkas Terintegrasi Tunggal (5.001 Baris Kode Mandiri)
├── compile_and_run.bat          <- Script build otomatis & menu runner untuk Windows
└── README.md                    <- Dokumentasi lengkap proyek
```

---

## 💻 Panduan Kompilasi & Menjalankan Aplikasi

Pastikan compiler C++ (seperti **GCC / G++ 11+** atau **MinGW**) telah terpasang.

### Opsi A: Menjalankan File Gabungan Standalone (`main_gabungan.cpp` - 5.001 Baris)
File ini berisi seluruh 3 layer dan dapat langsung dikompilasi dalam 1 baris perintah:

```powershell
# 1. Kompilasi file gabungan
g++ -std=c++17 -Wall main_gabungan.cpp -o rental_camping.exe

# 2. Jalankan mode self-diagnostics / unit test otomatis
.\rental_camping.exe --test

# 3. Jalankan aplikasi utama interaktif
.\rental_camping.exe
```

### Opsi B: Menjalankan Versi Modular Terintegrasi (3 Layer)
```powershell
# Kompilasi seluruh berkas modular sekaligus
g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_2_Business_Logic_Layer/business_logic.cpp Anggota_3_UI_Controller_Layer/ui_views.cpp Anggota_3_UI_Controller_Layer/controller.cpp main.cpp -o rental_modular.exe

# Jalankan aplikasi modular
.\rental_modular.exe
```

### Opsi C: Menjalankan Unit Test Mandiri Tiap Anggota (Sesuai Panduan Dosen)
```powershell
# Unit Test Mandiri Anggota 1 (Data Layer)
g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_1_Data_Layer/main_test.cpp -o test_data.exe
.\test_data.exe

# Unit Test Mandiri Anggota 2 (Business Logic Layer)
g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_2_Business_Logic_Layer/business_logic.cpp Anggota_2_Business_Logic_Layer/main_test.cpp -o test_bll.exe
.\test_bll.exe

# Uji Visual Kerangka Menu Anggota 3 (UI Layer)
g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_2_Business_Logic_Layer/business_logic.cpp Anggota_3_UI_Controller_Layer/ui_views.cpp Anggota_3_UI_Controller_Layer/main_menu_mock.cpp -o test_ui.exe
.\test_ui.exe
```

### Opsi D: Menggunakan Batch Script Satu-Klik (`compile_and_run.bat`)
Cukup jalankan berkas batch script:
```powershell
.\compile_and_run.bat
```
Pilih opsi angka `[1-6]` sesuai kebutuhan pengujian.

---

## 🔑 Kredensial Akun Bawaan Sistem (Demo)

| Role | Username | Password | Nama Lengkap | Keterangan |
| :--- | :--- | :--- | :--- | :--- |
| **Admin** | `admin` | `admin123` | Administrator Utama | Akses penuh dashboard, inventaris, & pengembalian |
| **User** | `budi` | `budi123` | Budi Santoso | Pelanggan Member Silver (3x riwayat sewa) |
| **User** | `siti` | `siti123` | Siti Aminah | Pelanggan Member Gold (5x riwayat sewa) |
| **User** | `andi` | `andi123` | Andi Wijaya | Pelanggan Member Bronze (1x riwayat sewa) |

**Voucher Diskon Tersedia:**
- `CAMPINGASIK10` : Diskon 10% s.d Rp 50.000
- `WEEKENDSERU20` : Diskon 20% s.d Rp 100.000 (Min. Transaksi Rp 200.000)
- `MEMBERBARU`    : Diskon Flat Rp 25.000

---
*Dikembangkan dengan dedikasi untuk memenuhi standar tugas besar proyek C++ berorientasi objek.*
