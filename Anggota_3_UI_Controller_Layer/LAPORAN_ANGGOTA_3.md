# LAPORAN TAHAPAN KERJA & LOG PROMPT AI - ANGGOTA 3 (UI & CONTROLLER LAYER)

**Proyek:** Sistem Informasi Penyewaan Alat Camping Berbasis C++  
**Peran Anggota 3:** UI & Controller Specialist / Integrator Akhir  
**Fokus Tugas:** Perancangan antarmuka visual konsol (CLI), tata letak menu utama, navigasi alur pengguna (User & Admin), pencetakan struk/invoice resmi, validasi input pengguna, serta pengintegrasian menyeluruh seluruh modul dari Anggota 1 dan Anggota 2.

---

## 1. Desain Visual & Pengendali Alur (UI & Controller)
Anggota 3 mengembangkan berkas:
1. **`ui_views.h` & `ui_views.cpp`:**
   - Desain tata letak berbasis ASCII box-drawing border.
   - Formatter mata uang Rupiah otomatis dengan pemisah ribuan titik (`formatRupiah(1250000)` -> `Rp 1.250.000`).
   - Tabel responsif berpaginasi untuk katalog alat, keranjang belanja, dan inventaris admin.
   - Format struk resmi cetak nota sewa dan berita acara pengembalian lengkap dengan kalkulasi denda dan deposit.
   - Input sanitization helper (`readInt`, `readDouble`, `readString`, `confirmAction`).
2. **`controller.h` & `controller.cpp`:**
   - Menghubungkan seluruh fungsi backend (`AuthService`, `CatalogService`, `CartService`, `RentalService`, `PaymentService`, `ReturnService`, `AdminService`, `ReportFinanceService`) dengan antarmuka CLI.
   - State routing sesi dinamis antara Guest Mode, User Dashboard, dan Admin Panel.
3. **`main_menu_mock.cpp`:**
   - File driver mandiri untuk memverifikasi tata letak, banner, dan alur menu sebelum integrasi akhir.

---

## 2. Log Riwayat Prompt AI (Kewajiban Individu Anggota 3)

### Prompt Tahap 1: Perancangan Tata Letak CLI & Box Drawing
> **Prompt:**  
> *"Buatkan modul UI C++ berbasis CLI dengan fungsi formatRupiah, banner judul camping, box notifikasi berhasil/gagal, dan tabel bergaris rapi untuk menampilkan katalog alat camping dan keranjang belanja."*  
> **Hasil:**  
> Dihasilkan file `ui_views.h` dan `ui_views.cpp` dengan formatting string rapi dan sanitasi cin yang aman dari loop tak terbatas.

### Prompt Tahap 2: Perancangan Pengendali Alur (Controller)
> **Prompt:**  
> *"Buatkan class AppController yang mengintegrasikan DataManager (Anggota 1) dan Business Logic (Anggota 2) ke dalam alur navigasi menu konsol untuk Guest, User, dan Admin."*  
> **Hasil:**  
> Dihasilkan file `controller.h` dan `controller.cpp` dengan alur modular yang saling terhubung.

### Prompt Tahap 3: Pembuatan Driver Uji Mandiri Kerangka Menu
> **Prompt:**  
> *"Buatkan file main_menu_mock.cpp untuk Anggota 3 yang memvalidasi visual tampilan menu, banner, struk, dan formatter tanpa perlu menjalankan seluruh database."*  
> **Hasil:**  
> Dihasilkan `main_menu_mock.cpp` yang memverifikasi seluruh komponen visual.

---

## 3. Hasil Eksekusi Uji Mandiri (`main_menu_mock.cpp`)

```text
=======================================================
 [PENGUJIAN VISUAL & KERANGKA MENU UTAMA - ANGGOTA 3]  
=======================================================

=========================================================================================
    /\_ /\      CAMPING GEAR RENTAL HUB - SISTEM PENYEWAAN ALAT CAMPING                  
   (  . .)     Penyewaan Alat Outdoor Terlengkap: Tenda, Carrier, Kompor & Sleeping Bag 
    >  ^ <      Bersih, Terawat, Berkualitas Tinggi, & Siap Menemani Ekspedisi Anda!     
=========================================================================================

[1] Pengujian Format Mata Uang Rupiah:
  50000       -> Rp 50.000
  1250000     -> Rp 1.250.000
  -75000      -> -Rp 75.000

+---------------------------------------------------------------------------------------+
| >> PENGUJIAN MODUL UI BOX                                                             |
+---------------------------------------------------------------------------------------+

[BERHASIL] Operasi berhasil dijalankan dengan lancar!

[INFO] Silakan periksa kembali daftar perlengkapan Anda.

[PERINGATAN/GAGAL] Format input tidak sesuai kriteria!

[2] Pengujian Kerangka Menu Utama: Lolos
[3] Pengujian Kerangka Menu Pelanggan: Lolos
[4] Pengujian Kerangka Menu Administrator: Lolos
[5] Pengujian Tata Letak Tabel Alat Camping Mock: Lolos

=======================================================
 KERANGKA MENU UTAMA & UI ANGGOTA 3 LOLOS PENGUJIAN!   
=======================================================
```

**Status Modul UI & Controller:** SELESAI, TANGGUH, dan SIAP SEBAGAI INTEGRATOR AKHIR SELURUH KODE KELOMPOK.
