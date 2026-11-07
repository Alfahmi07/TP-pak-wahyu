# LAPORAN TAHAPAN KERJA & LOG PROMPT AI - ANGGOTA 2 (BUSINESS LOGIC LAYER)

**Proyek:** Sistem Informasi Penyewaan Alat Camping Berbasis C++  
**Peran Anggota 2:** Business Logic Layer Specialist  
**Fokus Tugas:** Pembuatan aturan bisnis aplikasi, algoritma pemrosesan data (Sorting & Searching), validasi input, kalkulasi biaya & denda, verifikasi pembayaran, alur pengembalian barang, serta pelaporan keuangan.

---

## 1. Desain Arsitektur & Aturan Bisnis (Business Logic)
Anggota 2 bertanggung jawab mengembangkan modul `business_logic.h` dan `business_logic.cpp` yang mencakup:
1. **`AlgorithmEngine` (Algoritma Wajib Ilmu Komputer):**
   - **MergeSort (Divide & Conquer):** Pengurutan alat camping multi-kriteria (Harga termurah/termahal, Nama A-Z / Z-A, Rating tertinggi, Popularitas, Stok tersedia).
   - **QuickSort (Partitioning):** Pengurutan riwayat transaksi berdasarkan tanggal, nominal total, atau nomor invoice.
   - **BubbleSort:** Pengurutan ulasan pelanggan dari tanggal terbaru.
   - **Binary Search:** Pencarian cepat berbasis kode SKU unik barang pada array terurut O(log N).
   - **Linear Search Multi-Field:** Pencarian alat dengan multi-kriteria (nama, merk, kategori, spesifikasi, batas harga, stok tersedia).
2. **`AuthService`:** Validasi format ketat (email, no HP, username alfanumerik, minimal karakter), enkripsi password hashing FNV-1a dengan salt, proteksi status akun, dan manajemen sesi.
3. **`CatalogService` & `CartService`:** Pemeriksaan ketersediaan stok real-time, kontrol kuantitas sewa, perhitungan subtotal dan estimasi uang jaminan (deposit).
4. **`RentalService` (Checkout Wizard):** Pengurangan stok otomatis, validasi tanggal sewa, engine kupon voucher diskon (`CAMPINGASIK10`, `WEEKENDSERU20`, `MEMBERBARU`), dan penerbitan nomor invoice unik format `INV-YYYYMM-XXXX`.
5. **`PaymentService`:** Simulasi instruksi pembayaran Virtual Account (BCA, Mandiri, BRI), QRIS Instant NMID, E-Wallet (GoPay, OVO, Dana), dan Tunai di Toko (COD).
6. **`ReturnService`:** Otomasi inspeksi kondisi fisik barang (Baik, Kotor Berat, Rusak Ringan, Rusak Berat, Hilang), kalkulasi denda telat per hari (150% sewa harian), pemulihan stok inventaris, serta rekonsiliasi deposit (pengembalian atau tagihan kurang bayar).
7. **`ReportFinanceService`:** Kalkulasi metrik KPI eksekutif, laba kotor, diskon, uang jaminan, denda masuk, laba bersih operasional, top 5 alat terlaris, dan deteksi stok kritis.

---

## 2. Log Riwayat Prompt AI (Kewajiban Individu Anggota 2)

### Prompt Tahap 1: Implementasi Algoritma Sorting & Searching
> **Prompt:**  
> *"Buatkan implementasi algoritma MergeSort untuk mengurutkan CampingGear berdasarkan harga dan nama, QuickSort untuk mengurutkan RentalTransaction berdasarkan tanggal dan total, serta Binary Search untuk mencari barang berdasarkan SKU di C++."*  
> **Hasil:**  
> Dihasilkan class `AlgorithmEngine` dengan implementasi rekursif murni dan efisien tanpa menggunakan helper sorting library luar untuk inti algoritma.

### Prompt Tahap 2: Aturan Bisnis Checkout, Denda, & Rekonsiliasi Deposit
> **Prompt:**  
> *"Buatkan business logic untuk alur checkout sewa alat camping: kalkulasi durasi hari, kupon diskon, kalkulasi deposit jaminan (10% nilai barang), pemotongan stok, serta alur pengembalian dengan denda keterlambatan dan denda kerusakan barang."*  
> **Hasil:**  
> Dihasilkan method `RentalService::processCheckout` dan `ReturnService::processReturnInspection` yang menangani semua skenario transaksi secara akurat.

### Prompt Tahap 3: Pembuatan Driver Unit Test Mandiri
> **Prompt:**  
> *"Buatkan file main_test.cpp untuk Anggota 2 yang memverifikasi setiap service dan algoritma secara terisolasi dengan data hardcoded dan assert C++."*  
> **Hasil:**  
> Dihasilkan `main_test.cpp` dengan 5 modul pengujian mendalam yang menguji algoritma, auth, keranjang, sewa, denda, dan laporan keuangan.

---

## 3. Hasil Eksekusi Unit Test Mandiri (`main_test.cpp`)

```text
=======================================================
 MEMULAI UNIT TEST MANDIRI BUSINESS LOGIC (ANGGOTA 2)  
=======================================================

=======================================================
 [UNIT TEST BUSINESS LOGIC] 1. Pengujian Algoritma Sorting & Searching
=======================================================
[PASS] MergeSort Gear (PRICE_ASC) Berhasil!
[PASS] MergeSort Gear (NAME_ASC) Berhasil!
[PASS] Binary Search SKU Berhasil!
[PASS] Linear Search Multi-Field Berhasil!

=======================================================
 [UNIT TEST BUSINESS LOGIC] 2. Pengujian AuthService (Registrasi & Login)
=======================================================
[PASS] Validasi Registrasi & Login AuthService Berhasil!

=======================================================
 [UNIT TEST BUSINESS LOGIC] 3. Pengujian CartService & RentalService (Checkout)
=======================================================
[PASS] Cart Operasi & Perhitungan Subtotal Berhasil!
-> Invoice Berhasil Diterbitkan: INV-202611-1004
[PASS] Checkout Wizard, Diskon Promo, & Pengurangan Stok Berhasil!

=======================================================
 [UNIT TEST BUSINESS LOGIC] 4. Pengujian ReturnService (Inspeksi Denda & Pengembalian Deposit)
=======================================================
-> Denda Telat: Rp 351000
-> Denda Kerusakan: Rp 237500
-> Total Denda: Rp 588500
[PASS] Return Inspection, Denda Keterlambatan & Kerusakan Berhasil!

=======================================================
 [UNIT TEST BUSINESS LOGIC] 5. Pengujian ReportFinanceService (KPI & Analisis)
=======================================================
-> Top 1 Alat Terlaris: Gas Kaleng Portable Hi-Cook 230g (80 kali disewa)
[PASS] Laporan Keuangan, KPI, dan Top Rented Gears Berhasil!

=======================================================
 SEMUA PENGUJIAN UNIT BUSINESS LOGIC LOLOS DENGAN SUKSES!
=======================================================
```

**Status Modul Business Logic Layer:** SELESAI, TERNORMALISASI, dan SIAP DIINTEGRASIKAN ke UI & Controller Layer (Anggota 3).
