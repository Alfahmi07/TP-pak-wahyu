# LAPORAN TAHAPAN KERJA & LOG PROMPT AI - ANGGOTA 1 (DATA LAYER)

**Proyek:** Sistem Informasi Penyewaan Alat Camping Berbasis C++  
**Peran Anggota 1:** Data Layer Specialist  
**Fokus Tugas:** Pembuatan class entitas data, struktur model, pengelolaan relasi data, format serialisasi/deserialisasi berkas teks (.txt), dan sistem repositori penyimpanan (CRUD).

---

## 1. Desain Struktur Data & Entitas (Models)
Anggota 1 merancang modul `models.h` yang memuat class entitas utama:
1. **`User`**: Menyimpan identitas pengguna (ID, username, passwordHash, nama lengkap, kontak, NIK/KTP, alamat, role `ADMIN`/`USER`, status, trust score).
2. **`CampingGear`**: Menyimpan katalog alat camping (ID, SKU, nama, merk/brand, kategori enum, harga sewa per hari, nilai ganti rugi barang, total unit, stok tersedia, rating, review count, spesifikasi).
3. **`RentalItem`**: Representasi satuan barang sewa di dalam keranjang belanja maupun transaksi, mencakup durasi sewa, kuantitas, dan kondisi pengembalian.
4. **`Cart`**: Penampung keranjang belanja user dengan kalkulasi subtotal dinamis dan kontrol kuantitas.
5. **`RentalTransaction`**: Menyimpan informasi transaksi sewa (No Invoice unik `INV-YYYYMM-XXXX`, user ID, tanggal booking, tanggal sewa, tenggat kembali, tanggal realisasi pengembalian, rincian barang, diskon voucher, deposit jaminan, metode pembayaran, status transaksi).
6. **`ReturnLog`**: Mencatat berita acara pengembalian alat, denda keterlambatan per hari, denda kondisi barang rusak/hilang, serta kalkulasi pengembalian sisa deposit.
7. **`ReviewRating`**: Menyimpan testimoni ulasan dan rating bintang 1-5 dari pelanggan.
8. **`PromoCode`**: Validasi kupon promo potongan sewa.

---

## 2. Pengelolaan Persistensi Data (`DataManager`)
Modul `data_manager.h` dan `data_manager.cpp` mengimplementasikan:
- Penyimpanan persisten ke berkas flat file:
  - `data/users.txt`
  - `data/gears.txt`
  - `data/transactions.txt`
  - `data/returns.txt`
  - `data/reviews.txt`
- Operasi lengkap CRUD (Create, Read, Update, Delete) berbasis `std::vector` in-memory.
- Inisialisasi otomatis *Seed Data* jika file belum ada (21 item alat camping realistis, akun admin & user demo, serta transaksi contoh).

---

## 3. Log Riwayat Prompt AI (Kewajiban Individu Anggota 1)

### Prompt Tahap 1: Perancangan Class Entitas
> **Prompt:**  
> *"Buatkan arsitektur class entitas C++ untuk sistem penyewaan alat camping: User, CampingGear, RentalItem, Cart, RentalTransaction, ReturnLog, dan ReviewRating. Sertakan method getter/setter, method operasi stok, serta serialisasi dan deserialisasi berbasis delimiter pipe (|) agar bisa disimpan ke file .txt."*  
> **Hasil:**  
> Dihasilkan file `models.h` dengan enkapsulasi data yang kuat, enum class untuk kategori dan status, serta method `serialize()` dan `deserialize()`.

### Prompt Tahap 2: Pembuatan Repositori Data & File I/O
> **Prompt:**  
> *"Buatkan class DataManager di C++ untuk menangani CRUD dan pembacaan/penulisan berkas .txt dari entitas models.h. Tambahkan auto-seeding data awal alat camping (merk Eiger, Consina, Naturehike, Osprey, dll) jika data belum ada."*  
> **Hasil:**  
> Dihasilkan file `data_manager.h` dan `data_manager.cpp` dengan penanganan direktori otomatis, parsing file aman, dan penyimpanan sinkron.

### Prompt Tahap 3: Pembuatan Driver Unit Test Mandiri
> **Prompt:**  
> *"Buatkan file main_test.cpp untuk Anggota 1 yang menguji seluruh class entitas dan operasi DataManager secara mandiri menggunakan hardcoded data dan assertion C++ tanpa interaksi UI."*  
> **Hasil:**  
> Dihasilkan `main_test.cpp` yang mencakup 4 blok pengujian mendalam.

---

## 4. Hasil Eksekusi Unit Test Mandiri (`main_test.cpp`)

```text
=======================================================
  MEMULAI UNIT TEST MANDIRI DATA LAYER (ANGGOTA 1)    
=======================================================

=======================================================
 [UNIT TEST DATA LAYER] 1. Pengujian Class User & Serialisasi
=======================================================
-> Serialized User: 101|johndoe|passHash123|John Doe|081233445566|john@example.com|3201998877660001|Jl. Cemara No. 5|USER|AKTIF|2|95.5|2026-11-01
[PASS] Class User dan Serialisasi/Deserialisasi Berhasil!

=======================================================
 [UNIT TEST DATA LAYER] 2. Pengujian Class CampingGear & Manipulasi Stok
=======================================================
-> Serialized Gear: 50|TND-999|Tenda Dome Expedition 6P|TheNorthFace|Tenda & Shelter|100000|3500000|5|4|3|4.5|2|6 Orang Waterproof Alloy|Kondisi Baru
[PASS] Class CampingGear dan Operasi Stok Berhasil!

=======================================================
 [UNIT TEST DATA LAYER] 3. Pengujian RentalTransaction & Items Sub-serialisasi
=======================================================
-> Serialized Transaction with Items: 
   INV-TEST-001|101|John Doe|081233445566|3201998877660001|2026-11-07|2026-11-08|2026-11-10|-|160000|0|-|50000|210000|Transfer BCA|BELUM BAYAR|MENUNGGU PEMBAYARAN||0|0|0||1#TND-001#Tenda Eiger Guardian 4P#50000#1250000#1#2#Baik Sempurna#0;5#SLP-001#Sleeping Bag Polar#15000#220000#2#2#Baik Sempurna#0
[PASS] RentalTransaction dan RentalItem Berhasil!

=======================================================
 [UNIT TEST DATA LAYER] 4. Pengujian DataManager (CRUD & File I/O)
=======================================================
[PASS] Seluruh Pengujian CRUD & File I/O DataManager Berhasil!

=======================================================
  SEMUA PENGUJIAN UNIT DATA LAYER LOLOS DENGAN SUKSES! 
=======================================================
```

**Status Modul Data Layer:** SELESAI, STABIL, dan SIAP DIINTEGRASIKAN ke Business Logic Layer (Anggota 2).
