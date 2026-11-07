/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - UI & CONTROLLER LAYER (ANGGOTA 3)
 * File: controller.cpp
 * Deskripsi: Implementasi pengontrol alur aplikasi (Controller), navigasi menu,
 *            interaksi pengguna, validasi alur bisnis, dan integrasi modul.
 * Penulis: Anggota 3 (UI & Controller Specialist / Integrator)
 * ============================================================================
 */

#include "controller.h"
#include <iostream>
#include <fstream>
#include <iomanip>

namespace CampingRental {

    AppController::AppController()
        : dataManager("data"),
          authService(dataManager),
          catalogService(dataManager),
          cartService(dataManager),
          rentalService(dataManager),
          paymentService(dataManager),
          returnService(dataManager),
          adminService(dataManager),
          reportFinanceService(dataManager),
          isRunning(true) {
        dataManager.loadAllData();
    }

    AppController::AppController(const std::string& dataDirectory)
        : dataManager(dataDirectory),
          authService(dataManager),
          catalogService(dataManager),
          cartService(dataManager),
          rentalService(dataManager),
          paymentService(dataManager),
          returnService(dataManager),
          adminService(dataManager),
          reportFinanceService(dataManager),
          isRunning(true) {
        dataManager.loadAllData();
    }

    AppController::~AppController() {
        shutdown();
    }

    void AppController::shutdown() {
        dataManager.saveAllData();
        isRunning = false;
    }

    void AppController::run() {
        while (isRunning) {
            UIViews::printMainMenu();
            int choice = UIViews::readInt("  Pilih Menu [0-4]: ", 0, 4);

            switch (choice) {
                case 1:
                    handleLogin();
                    break;
                case 2:
                    handleRegister();
                    break;
                case 3:
                    handleGuestCatalog();
                    break;
                case 4:
                    UIViews::printRentalRules();
                    UIViews::pauseScreen();
                    break;
                case 0:
                    UIViews::printInfoBox("Terima kasih telah menggunakan Sistem Penyewaan Alat Camping. Sampai jumpa di alam bebas!");
                    isRunning = false;
                    break;
            }
        }
    }

    // ========================================================================
    // ALUR AUTENTIKASI: LOGIN & REGISTER
    // ========================================================================

    void AppController::handleLogin() {
        UIViews::printSectionHeader("MASUK KE AKUN (LOGIN SISTEM)");
        std::string username = UIViews::readString("  Masukkan Username: ");
        std::string password = UIViews::readString("  Masukkan Password: ");

        ServiceResult res = authService.login(username, password);
        if (res.success) {
            UIViews::printSuccessBox(res.message);
            UIViews::pauseScreen();

            if (authService.isAdmin()) {
                handleAdminDashboard();
            } else {
                cartService.initializeForUser(authService.getCurrentSession().userId);
                handleUserDashboard();
            }
        } else {
            UIViews::printErrorBox(res.message);
            UIViews::pauseScreen();
        }
    }

    void AppController::handleRegister() {
        UIViews::printSectionHeader("REGISTRASI AKUN PELANGGAN BARU");
        std::cout << "  Silakan lengkapi formulir pendaftaran di bawah ini:\n\n";

        std::string username = UIViews::readString("  Username (min. 4 karakter alfanumerik) : ");
        std::string password = UIViews::readString("  Password (min. 6 karakter)             : ");
        std::string confirm  = UIViews::readString("  Konfirmasi Ulang Password              : ");
        std::string fullName = UIViews::readString("  Nama Lengkap Sesuai KTP                : ");
        std::string phone    = UIViews::readString("  Nomor WhatsApp / HP Aktif              : ");
        std::string email    = UIViews::readString("  Alamat Email Aktif                     : ");
        std::string idCard   = UIViews::readString("  Nomor Identitas Asli (KTP / KTM / SIM) : ");
        std::string address  = UIViews::readString("  Alamat Domisili Lengkap                : ");

        ServiceResult res = authService.registerUser(username, password, confirm, fullName, phone, email, idCard, address);
        if (res.success) {
            UIViews::printSuccessBox(res.message);
        } else {
            UIViews::printErrorBox(res.message);
        }
        UIViews::pauseScreen();
    }

    void AppController::handleGuestCatalog() {
        GearSortField currentSort = GearSortField::NAME_ASC;
        int categoryFilter = -1;
        std::string searchKeyword = "";
        double maxPrice = -1.0;

        while (true) {
            auto gears = catalogService.getCatalogList(currentSort, categoryFilter, searchKeyword, maxPrice);
            UIViews::printSectionHeader("KATALOG ALAT CAMPING (MODE GUEST)");
            UIViews::displayGearCatalogTable(gears, 1, 10);

            std::cout << "\n  [1] Cari Berdasarkan Kata Kunci / Merk\n";
            std::cout << "  [2] Urutkan Katalog (Termurah, Termahal, Terlaris)\n";
            std::cout << "  [3] Lihat Detail Spesifikasi Alat\n";
            std::cout << "  [4] Reset Filter & Tampilkan Semua\n";
            std::cout << "  [0] Kembali ke Menu Utama\n";

            int act = UIViews::readInt("  Pilih Aksi [0-4]: ", 0, 4);
            if (act == 0) break;

            if (act == 1) {
                searchKeyword = UIViews::readString("  Masukkan kata kunci pencarian: ", true);
            } else if (act == 2) {
                std::cout << "  Pilihan Pengurutan: 1. Nama A-Z | 2. Harga Terendah | 3. Harga Tertinggi | 4. Rating Bintang\n";
                int sChoice = UIViews::readInt("  Pilih [1-4]: ", 1, 4);
                if (sChoice == 1) currentSort = GearSortField::NAME_ASC;
                else if (sChoice == 2) currentSort = GearSortField::PRICE_ASC;
                else if (sChoice == 3) currentSort = GearSortField::PRICE_DESC;
                else if (sChoice == 4) currentSort = GearSortField::RATING_DESC;
            } else if (act == 3) {
                int gid = UIViews::readInt("  Masukkan ID Alat Camping: ", 1, 9999);
                CampingGear* gear = catalogService.getGearDetail(gid);
                if (gear) {
                    auto revs = catalogService.getGearReviews(gid);
                    UIViews::displayGearDetailCard(*gear, revs);
                } else {
                    UIViews::printErrorBox("Alat dengan ID tersebut tidak ditemukan!");
                }
                UIViews::pauseScreen();
            } else if (act == 4) {
                searchKeyword = "";
                categoryFilter = -1;
                maxPrice = -1.0;
                currentSort = GearSortField::NAME_ASC;
                UIViews::printInfoBox("Filter katalog telah direset.");
            }
        }
    }

    // ========================================================================
    // ALUR PELANGGAN: USER DASHBOARD
    // ========================================================================

    void AppController::handleUserDashboard() {
        bool userActive = true;
        while (userActive && authService.isAuthenticated()) {
            UIViews::printUserDashboardMenu(authService.getCurrentSession().fullName, cartService.getTotalItemCount());
            int choice = UIViews::readInt("  Pilih Menu Pelanggan [0-7]: ", 0, 7);

            switch (choice) {
                case 1:
                    handleUserCatalog();
                    break;
                case 2:
                    handleUserCart();
                    break;
                case 3:
                    handleUserCheckout();
                    break;
                case 4:
                    handleUserPayment();
                    break;
                case 5:
                    handleUserRentalHistory();
                    break;
                case 6:
                    handleUserReview();
                    break;
                case 7:
                    handleUserProfile();
                    break;
                case 0:
                    authService.logout();
                    UIViews::printInfoBox("Anda telah berhasil logout dari sesi pelanggan.");
                    UIViews::pauseScreen();
                    userActive = false;
                    break;
            }
        }
    }

    void AppController::handleUserCatalog() {
        GearSortField currentSort = GearSortField::NAME_ASC;
        int categoryFilter = -1;
        std::string searchKeyword = "";
        double maxPrice = -1.0;

        while (true) {
            auto gears = catalogService.getCatalogList(currentSort, categoryFilter, searchKeyword, maxPrice);
            UIViews::printSectionHeader("KATALOG & PEMESANAN ALAT CAMPING");
            UIViews::displayGearCatalogTable(gears, 1, 10);
            UIViews::printCatalogSubMenu();

            int act = UIViews::readInt("  Pilih Menu Katalog [0-5]: ", 0, 5);
            if (act == 0) break;

            if (act == 1) {
                std::cout << "\n  Pilihan Pengurutan:\n";
                std::cout << "  1. Nama A-Z                 2. Nama Z-A\n";
                std::cout << "  3. Harga Termurah ke Termahal 4. Harga Termahal ke Termurah\n";
                std::cout << "  5. Rating Tertinggi         6. Paling Banyak Disewa (Populer)\n";
                std::cout << "  7. Ketersediaan Stok Terbanyak\n";
                int sChoice = UIViews::readInt("  Pilih [1-7]: ", 1, 7);
                if (sChoice == 1) currentSort = GearSortField::NAME_ASC;
                else if (sChoice == 2) currentSort = GearSortField::NAME_DESC;
                else if (sChoice == 3) currentSort = GearSortField::PRICE_ASC;
                else if (sChoice == 4) currentSort = GearSortField::PRICE_DESC;
                else if (sChoice == 5) currentSort = GearSortField::RATING_DESC;
                else if (sChoice == 6) currentSort = GearSortField::POPULARITY_DESC;
                else if (sChoice == 7) currentSort = GearSortField::AVAILABLE_STOCK_DESC;
            } else if (act == 2) {
                std::cout << "\n  Pilih Metode Pencarian / Filter:\n";
                std::cout << "  [1] Berdasarkan Kata Kunci (Nama, Merk, Spesifikasi)\n";
                std::cout << "  [2] Berdasarkan Kategori Alat Camping\n";
                std::cout << "  [3] Batas Maksimal Harga Sewa per Hari\n";
                int fChoice = UIViews::readInt("  Pilih [1-3]: ", 1, 3);
                if (fChoice == 1) {
                    searchKeyword = UIViews::readString("  Masukkan kata kunci pencarian: ");
                } else if (fChoice == 2) {
                    std::cout << "  Pilih Kategori: 0. Tenda | 1. Sleeping Gear | 2. Carrier & Tas | 3. Alat Masak\n";
                    std::cout << "                  4. Penerangan | 5. Navigasi/Safety | 6. Outdoor Comfort\n";
                    categoryFilter = UIViews::readInt("  Pilih Kategori [0-6]: ", 0, 6);
                } else if (fChoice == 3) {
                    maxPrice = UIViews::readDouble("  Batas Maksimal Harga Sewa (Rp): ", 5000, 1000000);
                }
            } else if (act == 3) {
                int gid = UIViews::readInt("  Masukkan ID Alat Camping yang ingin dilihat: ", 1, 9999);
                CampingGear* gear = catalogService.getGearDetail(gid);
                if (gear) {
                    auto revs = catalogService.getGearReviews(gid);
                    UIViews::displayGearDetailCard(*gear, revs);
                } else {
                    UIViews::printErrorBox("Alat dengan ID tersebut tidak ditemukan!");
                }
                UIViews::pauseScreen();
            } else if (act == 4) {
                int gid = UIViews::readInt("  Masukkan ID Alat yang ingin disewa: ", 1, 9999);
                CampingGear* gear = catalogService.getGearDetail(gid);
                if (!gear) {
                    UIViews::printErrorBox("ID Alat tidak ditemukan!");
                    UIViews::pauseScreen();
                    continue;
                }
                int qty = UIViews::readInt("  Jumlah unit yang ingin disewa: ", 1, gear->getStockAvailable());
                int days = UIViews::readInt("  Durasi sewa (hari): ", 1, 30);

                ServiceResult res = cartService.addToCart(gid, qty, days);
                if (res.success) {
                    UIViews::printSuccessBox(res.message);
                } else {
                    UIViews::printErrorBox(res.message);
                }
                UIViews::pauseScreen();
            } else if (act == 5) {
                searchKeyword = "";
                categoryFilter = -1;
                maxPrice = -1.0;
                currentSort = GearSortField::NAME_ASC;
                UIViews::printInfoBox("Semua filter pencarian telah direset.");
            }
        }
    }

    void AppController::handleUserCart() {
        while (true) {
            UIViews::printSectionHeader("KERANJANG PENYEWAAN ALAT");
            UIViews::displayCartTable(cartService.getCart());

            if (cartService.getCart().isEmpty()) {
                UIViews::pauseScreen();
                break;
            }

            std::cout << "\n  PILIHAN AKSI KERANJANG:\n";
            std::cout << "  [1] Lanjutkan ke Checkout & Pembayaran\n";
            std::cout << "  [2] Ubah Kuantitas Barang\n";
            std::cout << "  [3] Hapus Barang Tertentu dari Keranjang\n";
            std::cout << "  [4] Kosongkan Seluruh Keranjang\n";
            std::cout << "  [0] Kembali ke Menu Sebelumnya\n";

            int act = UIViews::readInt("  Pilih Aksi [0-4]: ", 0, 4);
            if (act == 0) break;

            if (act == 1) {
                handleUserCheckout();
                break;
            } else if (act == 2) {
                int gid = UIViews::readInt("  Masukkan ID Alat yang ingin diubah kuantitasnya: ", 1, 9999);
                int newQty = UIViews::readInt("  Masukkan Kuantitas Baru (0 untuk hapus): ", 0, 50);
                ServiceResult r = cartService.updateItemQuantity(gid, newQty);
                if (r.success) UIViews::printSuccessBox(r.message);
                else UIViews::printErrorBox(r.message);
            } else if (act == 3) {
                int gid = UIViews::readInt("  Masukkan ID Alat yang ingin dihapus: ", 1, 9999);
                ServiceResult r = cartService.removeItem(gid);
                if (r.success) UIViews::printSuccessBox(r.message);
                else UIViews::printErrorBox(r.message);
            } else if (act == 4) {
                if (UIViews::confirmAction("  Apakah Anda yakin ingin mengosongkan keranjang?")) {
                    cartService.clearCart();
                    UIViews::printSuccessBox("Keranjang telah dikosongkan.");
                }
            }
        }
    }

    void AppController::handleUserCheckout() {
        if (cartService.getCart().isEmpty()) {
            UIViews::printErrorBox("Keranjang Anda masih kosong! Silakan pilih alat dari katalog.");
            UIViews::pauseScreen();
            return;
        }

        UIViews::printSectionHeader("FORMULIR CHECKOUT & PEMESANAN SEWA");
        std::cout << "  Rincian data pelanggan terdaftar:\n";
        User* currentUser = dataManager.findUserById(authService.getCurrentSession().userId);
        if (!currentUser) return;

        std::cout << "  Nama Penyewa  : " << currentUser->getFullName() << "\n";
        std::cout << "  No. WhatsApp  : " << currentUser->getPhoneNumber() << "\n";
        std::cout << "  No. Identitas : " << currentUser->getIdentityCardNumber() << "\n";
        std::cout << "  Alamat        : " << currentUser->getAddress() << "\n\n";

        if (!UIViews::confirmAction("  Apakah Anda ingin menggunakan data identitas di atas?")) {
            UIViews::printInfoBox("Silakan perbarui profil Anda di menu profil terlebih dahulu.");
            UIViews::pauseScreen();
            return;
        }

        std::string startDate = UIViews::readString("  Tanggal Mulai Sewa / Pengambilan (YYYY-MM-DD, cth: 2026-11-08): ");
        int durationDays = UIViews::readInt("  Durasi Total Penyewaan (hari): ", 1, 30);

        // Voucher Diskon Promo
        std::cout << "\n  VOUCHER PROMO TERSEDIA:\n";
        std::cout << "  * 'CAMPINGASIK10' : Diskon 10% s.d Rp 50.000\n";
        std::cout << "  * 'WEEKENDSERU20' : Diskon 20% s.d Rp 100.000 (Min. Transaksi Rp 200.000)\n";
        std::cout << "  * 'MEMBERBARU'    : Diskon Flat Rp 25.000\n";
        std::string promo = UIViews::readString("  Masukkan Kode Promo (Tekan enter jika tidak ada): ", true);

        // Pemilihan Metode Pembayaran
        std::cout << "\n  PILIH METODE PEMBAYARAN:\n";
        std::cout << "  [1] Transfer Bank BCA (Virtual Account Instant)\n";
        std::cout << "  [2] Transfer Bank Mandiri\n";
        std::cout << "  [3] Transfer Bank BRI\n";
        std::cout << "  [4] QRIS Instant All Payment\n";
        std::cout << "  [5] GoPay\n";
        std::cout << "  [6] OVO\n";
        std::cout << "  [7] DANA\n";
        std::cout << "  [8] Bayar Tunai saat Pengambilan di Outlet (COD)\n";
        int payChoice = UIViews::readInt("  Pilih Metode [1-8]: ", 1, 8);

        PaymentMethod method = PaymentMethod::TRANSFER_BANK_BCA;
        if (payChoice == 1) method = PaymentMethod::TRANSFER_BANK_BCA;
        else if (payChoice == 2) method = PaymentMethod::TRANSFER_BANK_MANDIRI;
        else if (payChoice == 3) method = PaymentMethod::TRANSFER_BANK_BRI;
        else if (payChoice == 4) method = PaymentMethod::EWALLET_QRIS;
        else if (payChoice == 5) method = PaymentMethod::EWALLET_GOPAY;
        else if (payChoice == 6) method = PaymentMethod::EWALLET_OVO;
        else if (payChoice == 7) method = PaymentMethod::EWALLET_DANA;
        else if (payChoice == 8) method = PaymentMethod::CASH_ON_PICKUP;

        std::string orderNotes = UIViews::readString("  Catatan Tambahan untuk Petugas Toko: ", true);

        // Eksekusi Checkout
        ServiceResult coResult = rentalService.processCheckout(
            currentUser->getId(), currentUser->getFullName(), currentUser->getPhoneNumber(),
            currentUser->getIdentityCardNumber(), startDate, durationDays,
            cartService.getCart().getItems(), method, promo, orderNotes
        );

        if (coResult.success) {
            std::string invoice = coResult.message;
            cartService.clearCart();

            UIViews::printSuccessBox("Pemesanan penyewaan berhasil diterbitkan!");
            RentalTransaction* tx = rentalService.getTransactionDetail(invoice);
            if (tx) {
                UIViews::displayOfficialInvoice(*tx);
                std::cout << "\n  INSTRUKSI PEMBAYARAN:\n";
                std::cout << "  -----------------------------------------------------------------\n";
                std::cout << paymentService.generatePaymentInstructions(tx->getPaymentMethod(), tx->getTotalAmount(), tx->getInvoiceNumber()) << "\n";
                std::cout << "  -----------------------------------------------------------------\n";
            }
        } else {
            UIViews::printErrorBox(coResult.message);
        }
        UIViews::pauseScreen();
    }

    void AppController::handleUserPayment() {
        UIViews::printSectionHeader("KONFIRMASI & PEMBAYARAN TAGIHAN");
        auto txs = rentalService.getUserTransactions(authService.getCurrentSession().userId);

        std::vector<RentalTransaction> pendingTxs;
        for (const auto& t : txs) {
            if (t.getPaymentStatus() == PaymentStatus::UNPAID ||
                t.getPaymentStatus() == PaymentStatus::PENDING_VERIFICATION) {
                pendingTxs.push_back(t);
            }
        }

        if (pendingTxs.empty()) {
            UIViews::printInfoBox("Tidak ada tagihan pembayaran sewa yang tertunda untuk akun Anda.");
            UIViews::pauseScreen();
            return;
        }

        UIViews::displayTransactionListTable(pendingTxs);
        std::string invoice = UIViews::readString("  Masukkan Nomor Invoice yang ingin dibayar / dikonfirmasi: ");
        RentalTransaction* tx = rentalService.getTransactionDetail(invoice);

        if (!tx || tx->getUserId() != authService.getCurrentSession().userId) {
            UIViews::printErrorBox("Invoice tidak ditemukan atau bukan milik akun Anda!");
            UIViews::pauseScreen();
            return;
        }

        UIViews::displayOfficialInvoice(*tx);
        std::cout << "\n  INSTRUKSI PEMBAYARAN:\n";
        std::cout << paymentService.generatePaymentInstructions(tx->getPaymentMethod(), tx->getTotalAmount(), tx->getInvoiceNumber()) << "\n\n";

        if (tx->getPaymentStatus() == PaymentStatus::PENDING_VERIFICATION) {
            UIViews::printInfoBox("Pembayaran Anda sedang dalam proses verifikasi oleh tim kasir.");
            UIViews::pauseScreen();
            return;
        }

        std::cout << "  [1] Konfirmasi Pembayaran Selesai (Input Nomor Bukti Transfer / Resi)\n";
        std::cout << "  [0] Kembali\n";
        int opt = UIViews::readInt("  Pilih [0-1]: ", 0, 1);
        if (opt == 1) {
            std::string proof = UIViews::readString("  Masukkan Nomor Referensi Transfer / Resi Pembayaran: ");
            ServiceResult r = paymentService.submitPaymentProof(invoice, proof);
            if (r.success) UIViews::printSuccessBox(r.message);
            else UIViews::printErrorBox(r.message);
        }
        UIViews::pauseScreen();
    }

    void AppController::handleUserRentalHistory() {
        while (true) {
            UIViews::printSectionHeader("RIWAYAT PENYEWAAN SAYA");
            auto txs = rentalService.getUserTransactions(authService.getCurrentSession().userId);
            UIViews::displayTransactionListTable(txs);

            std::cout << "\n  [1] Lihat Nota Rinci & Cetak Invoice Struk Resmi\n";
            std::cout << "  [0] Kembali ke Menu Pelanggan\n";
            int act = UIViews::readInt("  Pilih [0-1]: ", 0, 1);
            if (act == 0) break;

            if (act == 1) {
                std::string inv = UIViews::readString("  Masukkan Nomor Invoice: ");
                RentalTransaction* tx = rentalService.getTransactionDetail(inv);
                if (tx && tx->getUserId() == authService.getCurrentSession().userId) {
                    UIViews::displayOfficialInvoice(*tx);
                } else {
                    UIViews::printErrorBox("Invoice tidak ditemukan!");
                }
                UIViews::pauseScreen();
            }
        }
    }

    void AppController::handleUserReview() {
        UIViews::printSectionHeader("BERIKAN ULASAN & RATING ALAT CAMPING");
        auto txs = rentalService.getUserTransactions(authService.getCurrentSession().userId);

        std::vector<RentalItem> eligibleItems;
        for (const auto& tx : txs) {
            if (tx.getTransactionStatus() == TransactionStatus::RETURNED_COMPLETED ||
                tx.getTransactionStatus() == TransactionStatus::RETURNED_WITH_ISSUE) {
                for (const auto& it : tx.getItems()) {
                    eligibleItems.push_back(it);
                }
            }
        }

        if (eligibleItems.empty()) {
            UIViews::printInfoBox("Anda belum memiliki riwayat sewa yang telah selesai untuk diulas.");
            UIViews::pauseScreen();
            return;
        }

        std::cout << "  Daftar alat camping yang pernah Anda sewa:\n";
        for (size_t i = 0; i < eligibleItems.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] " << eligibleItems[i].getGearSku()
                      << " - " << eligibleItems[i].getGearName() << "\n";
        }

        int choice = UIViews::readInt("  Pilih alat yang ingin Anda ulas [1-" + std::to_string(eligibleItems.size()) + "]: ", 1, static_cast<int>(eligibleItems.size()));
        const auto& selected = eligibleItems[choice - 1];

        int rating = UIViews::readInt("  Beri Rating Bintang [1 s/d 5]: ", 1, 5);
        std::string text = UIViews::readString("  Tulis ulasan/pengalaman Anda menggunakan alat ini: ");

        ServiceResult res = catalogService.submitReview(selected.getGearId(), authService.getCurrentSession().userId,
                                                       authService.getCurrentSession().username, rating, text);
        if (res.success) UIViews::printSuccessBox(res.message);
        else UIViews::printErrorBox(res.message);
        UIViews::pauseScreen();
    }

    void AppController::handleUserProfile() {
        UIViews::printSectionHeader("PROFIL PENGGUNA & KEAMANAN AKUN");
        User* user = dataManager.findUserById(authService.getCurrentSession().userId);
        if (!user) return;

        std::cout << "  Username         : " << user->getUsername() << "\n";
        std::cout << "  Nama Lengkap     : " << user->getFullName() << "\n";
        std::cout << "  Nomor HP / WA    : " << user->getPhoneNumber() << "\n";
        std::cout << "  Email            : " << user->getEmail() << "\n";
        std::cout << "  Nomor Identitas  : " << user->getIdentityCardNumber() << "\n";
        std::cout << "  Alamat Domisili  : " << user->getAddress() << "\n";
        std::cout << "  Total Sewa Sukses: " << user->getTotalRentalsCompleted() << " kali transaksi\n";
        std::cout << "  Skor Reputasi    : " << std::fixed << std::setprecision(1) << user->getTrustScore() << " / 100.0\n";
        std::cout << "  Terdaftar Sejak  : " << user->getRegisteredAt() << "\n";
        std::cout << "  -----------------------------------------------------------------\n";
        std::cout << "  [1] Perbarui Data Profil (Nama, No HP, Email, Alamat)\n";
        std::cout << "  [2] Ganti Password Akun\n";
        std::cout << "  [0] Kembali ke Menu Pelanggan\n";

        int opt = UIViews::readInt("  Pilih Aksi [0-2]: ", 0, 2);
        if (opt == 1) {
            std::string newName = UIViews::readString("  Nama Lengkap Baru: ");
            std::string newPhone = UIViews::readString("  Nomor HP Baru: ");
            std::string newMail = UIViews::readString("  Email Baru: ");
            std::string newAddr = UIViews::readString("  Alamat Domisili Baru: ");
            ServiceResult r = authService.updateProfile(newName, newPhone, newMail, newAddr);
            if (r.success) UIViews::printSuccessBox(r.message);
            else UIViews::printErrorBox(r.message);
            UIViews::pauseScreen();
        } else if (opt == 2) {
            std::string oldPass = UIViews::readString("  Masukkan Password Lama: ");
            std::string newPass = UIViews::readString("  Masukkan Password Baru (min. 6 karakter): ");
            ServiceResult r = authService.changePassword(oldPass, newPass);
            if (r.success) UIViews::printSuccessBox(r.message);
            else UIViews::printErrorBox(r.message);
            UIViews::pauseScreen();
        }
    }

    // ========================================================================
    // ALUR ADMINISTRATOR: ADMIN DASHBOARD
    // ========================================================================

    void AppController::handleAdminDashboard() {
        bool adminActive = true;
        while (adminActive && authService.isAdmin()) {
            UIViews::printAdminDashboardMenu(authService.getCurrentSession().fullName);
            int choice = UIViews::readInt("  Pilih Menu Admin [0-6]: ", 0, 6);

            switch (choice) {
                case 1:
                    handleAdminInventory();
                    break;
                case 2:
                    handleAdminTransactions();
                    break;
                case 3:
                    handleAdminReturnGear();
                    break;
                case 4:
                    handleAdminCustomerManagement();
                    break;
                case 5:
                    handleAdminDashboardAnalytics();
                    break;
                case 6:
                    handleAdminExportReport();
                    break;
                case 0:
                    authService.logout();
                    UIViews::printInfoBox("Sesi administrator ditutup.");
                    UIViews::pauseScreen();
                    adminActive = false;
                    break;
            }
        }
    }

    void AppController::handleAdminInventory() {
        GearSortField currentSort = GearSortField::NAME_ASC;
        while (true) {
            auto gears = catalogService.getCatalogList(currentSort);
            UIViews::printSectionHeader("MANAJEMEN INVENTARIS ALAT CAMPING");
            UIViews::displayAdminGearTable(gears);

            std::cout << "\n  PILIHAN AKSI INVENTARIS:\n";
            std::cout << "  [1] Tambah Alat Camping Baru\n";
            std::cout << "  [2] Edit Rincian & Harga Alat\n";
            std::cout << "  [3] Restock / Tambah Unit Fisik\n";
            std::cout << "  [4] Hapus Alat dari Inventaris\n";
            std::cout << "  [5] Urutkan Inventaris (Harga, Stok, Rating)\n";
            std::cout << "  [0] Kembali ke Menu Administrator\n";

            int act = UIViews::readInt("  Pilih Aksi [0-5]: ", 0, 5);
            if (act == 0) break;

            if (act == 1) handleAdminAddGear();
            else if (act == 2) handleAdminEditGear();
            else if (act == 3) handleAdminRestockGear();
            else if (act == 4) handleAdminDeleteGear();
            else if (act == 5) {
                std::cout << "  Urutkan: 1. Nama A-Z | 2. Harga Termurah | 3. Harga Termahal | 4. Stok Sedia Terbanyak\n";
                int sc = UIViews::readInt("  Pilih [1-4]: ", 1, 4);
                if (sc == 1) currentSort = GearSortField::NAME_ASC;
                else if (sc == 2) currentSort = GearSortField::PRICE_ASC;
                else if (sc == 3) currentSort = GearSortField::PRICE_DESC;
                else if (sc == 4) currentSort = GearSortField::AVAILABLE_STOCK_DESC;
            }
        }
    }

    void AppController::handleAdminAddGear() {
        UIViews::printSubHeader("TAMBAH ALAT CAMPING BARU");
        std::string name = UIViews::readString("  Nama Alat Camping: ");
        std::string brand = UIViews::readString("  Merk / Brand: ");

        std::cout << "  Pilih Kategori: 0. Tenda | 1. Sleeping Gear | 2. Carrier/Tas | 3. Alat Masak\n";
        std::cout << "                  4. Penerangan | 5. Navigasi/Safety | 6. Outdoor Comfort | 7. Lainnya\n";
        int catInt = UIViews::readInt("  Kategori [0-7]: ", 0, 7);
        GearCategory category = static_cast<GearCategory>(catInt);

        double rentalPrice = UIViews::readDouble("  Harga Sewa per Hari (Rp): ", 1000, 10000000);
        double replVal = UIViews::readDouble("  Nilai Penggantian jika Rusak/Hilang (Rp): ", 5000, 50000000);
        int stock = UIViews::readInt("  Jumlah Unit Fisik Awal: ", 1, 500);
        std::string spec = UIViews::readString("  Spesifikasi Teknis Singkat: ");
        std::string notes = UIViews::readString("  Catatan Kondisi Fisik: ");

        ServiceResult res = adminService.addNewGear(name, brand, category, rentalPrice, replVal, stock, spec, notes);
        if (res.success) UIViews::printSuccessBox(res.message);
        else UIViews::printErrorBox(res.message);
        UIViews::pauseScreen();
    }

    void AppController::handleAdminEditGear() {
        UIViews::printSubHeader("EDIT DETAIL ALAT CAMPING");
        int gid = UIViews::readInt("  Masukkan ID Alat yang ingin diedit: ", 1, 9999);
        CampingGear* gear = dataManager.findGearById(gid);
        if (!gear) {
            UIViews::printErrorBox("Alat tidak ditemukan!");
            UIViews::pauseScreen();
            return;
        }

        std::cout << "  Data saat ini: " << gear->getName() << " | Harga: " << UIViews::formatRupiah(gear->getRentalPricePerDay()) << "\n";
        std::string name = UIViews::readString("  Nama Baru (enter untuk tidak ubah): ", true);
        if (name.empty()) name = gear->getName();

        std::string brand = UIViews::readString("  Merk Baru (enter untuk tidak ubah): ", true);
        if (brand.empty()) brand = gear->getBrand();

        double price = UIViews::readDouble("  Harga Sewa per Hari Baru (Rp): ", 1000, 10000000);
        double replVal = UIViews::readDouble("  Nilai Penggantian Baru (Rp): ", 5000, 50000000);
        std::string spec = UIViews::readString("  Spesifikasi Baru: ");

        ServiceResult r = adminService.updateGearDetails(gid, name, brand, price, replVal, spec);
        if (r.success) UIViews::printSuccessBox(r.message);
        else UIViews::printErrorBox(r.message);
        UIViews::pauseScreen();
    }

    void AppController::handleAdminRestockGear() {
        UIViews::printSubHeader("RESTOCK UNIT FISIK ALAT");
        int gid = UIViews::readInt("  Masukkan ID Alat yang ingin direstock: ", 1, 9999);
        int additional = UIViews::readInt("  Jumlah Unit Tambahan yang Baru Masuk: ", 1, 100);

        ServiceResult r = adminService.restockGear(gid, additional);
        if (r.success) UIViews::printSuccessBox(r.message);
        else UIViews::printErrorBox(r.message);
        UIViews::pauseScreen();
    }

    void AppController::handleAdminDeleteGear() {
        UIViews::printSubHeader("HAPUS ALAT DARI INVENTARIS");
        int gid = UIViews::readInt("  Masukkan ID Alat yang ingin dihapus: ", 1, 9999);
        CampingGear* g = dataManager.findGearById(gid);
        if (!g) {
            UIViews::printErrorBox("Alat tidak ditemukan!");
            UIViews::pauseScreen();
            return;
        }

        if (UIViews::confirmAction("  Apakah Anda yakin ingin menghapus alat '" + g->getName() + "'?")) {
            ServiceResult r = adminService.removeGear(gid);
            if (r.success) UIViews::printSuccessBox(r.message);
            else UIViews::printErrorBox(r.message);
        }
        UIViews::pauseScreen();
    }

    void AppController::handleAdminTransactions() {
        while (true) {
            UIViews::printSectionHeader("MANAJEMEN TRANSAKSI PENYEWAAN");
            const auto& allTxs = dataManager.getAllTransactions();
            UIViews::displayTransactionListTable(allTxs);

            std::cout << "\n  PILIHAN AKSI TRANSAKSI:\n";
            std::cout << "  [1] Verifikasi & Setujui Pembayaran Pelanggan\n";
            std::cout << "  [2] Serahkan Unit ke Pelanggan (Ubah Status ke AKTIF SEWA)\n";
            std::cout << "  [3] Batalkan Transaksi Sewa\n";
            std::cout << "  [4] Lihat Struk Nota Resmi Transaksi Lengkap\n";
            std::cout << "  [5] Cari Transaksi Berdasarkan Invoice atau Nama Pelanggan\n";
            std::cout << "  [0] Kembali ke Menu Administrator\n";

            int act = UIViews::readInt("  Pilih Aksi [0-5]: ", 0, 5);
            if (act == 0) break;

            if (act == 1) {
                std::string inv = UIViews::readString("  Masukkan Nomor Invoice yang ingin diverifikasi: ");
                ServiceResult r = paymentService.confirmPaymentByAdmin(inv, authService.getCurrentSession().fullName);
                if (r.success) UIViews::printSuccessBox(r.message);
                else UIViews::printErrorBox(r.message);
                UIViews::pauseScreen();
            } else if (act == 2) {
                std::string inv = UIViews::readString("  Masukkan Nomor Invoice yang unitnya diambil pelanggan: ");
                ServiceResult r = adminService.markTransactionAsPickedUp(inv);
                if (r.success) UIViews::printSuccessBox(r.message);
                else UIViews::printErrorBox(r.message);
                UIViews::pauseScreen();
            } else if (act == 3) {
                std::string inv = UIViews::readString("  Masukkan Nomor Invoice yang ingin dibatalkan: ");
                std::string reason = UIViews::readString("  Alasan pembatalan: ");
                ServiceResult r = adminService.cancelTransaction(inv, reason);
                if (r.success) UIViews::printSuccessBox(r.message);
                else UIViews::printErrorBox(r.message);
                UIViews::pauseScreen();
            } else if (act == 4) {
                std::string inv = UIViews::readString("  Masukkan Nomor Invoice: ");
                RentalTransaction* tx = rentalService.getTransactionDetail(inv);
                if (tx) {
                    UIViews::displayOfficialInvoice(*tx);
                } else {
                    UIViews::printErrorBox("Invoice tidak ditemukan!");
                }
                UIViews::pauseScreen();
            } else if (act == 5) {
                std::string q = UIViews::readString("  Masukkan kata kunci (Invoice / Nama): ");
                auto filtered = AlgorithmEngine::linearSearchTransactions(allTxs, q);
                UIViews::displayTransactionListTable(filtered);
                UIViews::pauseScreen();
            }
        }
    }

    void AppController::handleAdminReturnGear() {
        UIViews::printSectionHeader("PENGEMBALIAN ALAT & PEMERIKSAAN KONDISI (INSPEKSI)");
        std::string inv = UIViews::readString("  Masukkan Nomor Invoice Sewa: ");
        RentalTransaction* tx = rentalService.getTransactionDetail(inv);

        if (!tx) {
            UIViews::printErrorBox("Nomor invoice tidak ditemukan dalam sistem!");
            UIViews::pauseScreen();
            return;
        }

        if (tx->getTransactionStatus() != TransactionStatus::ACTIVE_RENTED &&
            tx->getTransactionStatus() != TransactionStatus::PAID_CONFIRMED) {
            UIViews::printErrorBox("Transaksi ini tidak dalam status aktif sewa! (Status: " + transactionStatusToString(tx->getTransactionStatus()) + ")");
            UIViews::pauseScreen();
            return;
        }

        UIViews::displayOfficialInvoice(*tx);
        std::cout << "\n  MEMULAI PROSES INSPEKSI PENGEMBALIAN:\n";
        std::string actDate = UIViews::readString("  Tanggal Pengembalian Realisasi (YYYY-MM-DD, cth: 2026-11-10): ");

        std::vector<ReturnCondition> conditions;
        std::cout << "\n  KONDISI FISIK TIAP BARANG:\n";
        std::cout << "  [0] Baik Sempurna (Mulus, denda 0%)\n";
        std::cout << "  [1] Kotor Berat (Lumpur/Noda tebal butuh laundry khusus, denda 10%)\n";
        std::cout << "  [2] Rusak Ringan (Sobek kecil/pasak bengkok, denda 25%)\n";
        std::cout << "  [3] Rusak Berat (Frame patah/kain robek besar, denda 60%)\n";
        std::cout << "  [4] Hilang Total (Ganti 100% nilai barang)\n";

        for (const auto& item : tx->getItems()) {
            std::cout << "\n  Periksa: " << item.getGearName() << " (Qty: " << item.getQuantity() << " unit)\n";
            int cInt = UIViews::readInt("  Pilih Kondisi [0-4]: ", 0, 4);
            conditions.push_back(static_cast<ReturnCondition>(cInt));
        }

        std::string notes = UIViews::readString("  Catatan Berita Acara Pemeriksaan: ", true);
        ReturnLog log;
        ServiceResult retRes = returnService.processReturnInspection(
            inv, actDate, conditions, authService.getCurrentSession().fullName, notes, log
        );

        if (retRes.success) {
            UIViews::printSuccessBox(retRes.message);
            UIViews::displayOfficialReturnReceipt(*tx, log);
        } else {
            UIViews::printErrorBox(retRes.message);
        }
        UIViews::pauseScreen();
    }

    void AppController::handleAdminCustomerManagement() {
        while (true) {
            UIViews::printSectionHeader("MANAJEMEN PELANGGAN TOKO");
            const auto& users = dataManager.getAllUsers();
            UIViews::displayAdminCustomerTable(users);

            std::cout << "\n  [1] Blokir Akun Pelanggan (Banned)\n";
            std::cout << "  [2] Buka Blokir Akun (Aktifkan Kembali)\n";
            std::cout << "  [3] Lihat Riwayat Sewa Pelanggan Tertentu\n";
            std::cout << "  [0] Kembali ke Menu Administrator\n";

            int act = UIViews::readInt("  Pilih Aksi [0-3]: ", 0, 3);
            if (act == 0) break;

            if (act == 1) {
                int uid = UIViews::readInt("  Masukkan ID Pelanggan yang ingin diblokir: ", 1, 9999);
                ServiceResult r = adminService.toggleUserStatus(uid, UserStatus::BANNED);
                if (r.success) UIViews::printSuccessBox(r.message);
                else UIViews::printErrorBox(r.message);
                UIViews::pauseScreen();
            } else if (act == 2) {
                int uid = UIViews::readInt("  Masukkan ID Pelanggan yang ingin diaktifkan: ", 1, 9999);
                ServiceResult r = adminService.toggleUserStatus(uid, UserStatus::ACTIVE);
                if (r.success) UIViews::printSuccessBox(r.message);
                else UIViews::printErrorBox(r.message);
                UIViews::pauseScreen();
            } else if (act == 3) {
                int uid = UIViews::readInt("  Masukkan ID Pelanggan: ", 1, 9999);
                auto userTxs = rentalService.getUserTransactions(uid);
                UIViews::displayTransactionListTable(userTxs);
                UIViews::pauseScreen();
            }
        }
    }

    void AppController::handleAdminDashboardAnalytics() {
        UIViews::printSectionHeader("DASHBOARD BISNIS & ANALISIS KEUANGAN");
        FinancialKPI kpi = reportFinanceService.calculateOverallFinancialKPI();
        auto topGears = reportFinanceService.getTop5RentedGears();
        auto lowStock = reportFinanceService.getLowStockAlertGears();

        UIViews::displayFinancialDashboard(kpi, topGears, lowStock);
        UIViews::pauseScreen();
    }

    void AppController::handleAdminExportReport() {
        UIViews::printSectionHeader("EXPORT LAPORAN KEUANGAN KE BERKAS TEKS");
        std::string report = reportFinanceService.exportFinancialReportText();

        std::string filename = "laporan_keuangan_camping.txt";
        std::ofstream file(filename);
        if (file.is_open()) {
            file << report;
            file.close();
            UIViews::printSuccessBox("Laporan keuangan berhasil diexport ke berkas: " + filename);
        } else {
            UIViews::printErrorBox("Gagal membuka berkas untuk export!");
        }
        UIViews::pauseScreen();
    }

} // namespace CampingRental
