/**
 * ====================================================================================================
 * PROYEK AKHIR PEMROGRAMAN C++: SISTEM PENYEWAAN ALAT CAMPING
 * ====================================================================================================
 * 
 * Judul Proyek    : SISTEM PENYEWAAN ALAT CAMPING (CAMPING GEAR RENTAL HUB)
 * Pendekatan      : Modular 3-Layer Architecture (Data Layer, Business Logic Layer, UI & Controller)
 * Versi Berkas    : Berkas Terintegrasi Lengkap (Single Unified Codebase)
 * Standar Bahasa  : C++17 (Kompatibel dengan GCC / Clang / MSVC di Windows & Linux)
 * 
 * ----------------------------------------------------------------------------------------------------
 * PEMBAGIAN TUGAS MODULAR KELOMPOK (TIM 3 ORANG) SESUAI PANDUAN DOSEN:
 * 
 * [1] ANGGOTA 1 (DATA LAYER SPECIALIST):
 *     - Bertanggung jawab atas class entitas model data: User, CampingGear, RentalItem, Cart,
 *       RentalTransaction, ReturnLog, ReviewRating, PromoCode, dan FinancialKPI.
 *     - Bertanggung jawab atas serialisasi & deserialisasi data string berbasis delimiter '|' dan '#'.
 *     - Bertanggung jawab atas penyimpanan berkas persisten flat file (.txt) pada direktori data/:
 *       (users.txt, gears.txt, transactions.txt, returns.txt, reviews.txt).
 *     - Bertanggung jawab atas operasi CRUD in-memory repositori dan auto-seeding data awal.
 * 
 * [2] ANGGOTA 2 (BUSINESS LOGIC LAYER SPECIALIST):
 *     - Bertanggung jawab atas perancangan algoritma Sorting wajib:
 *       * MergeSort (Divide & Conquer) untuk pengurutan katalog alat camping multi-kriteria.
 *       * QuickSort (Partitioning) untuk pengurutan riwayat transaksi sewa.
 *       * BubbleSort untuk pengurutan ulasan pelanggan dari tanggal terbaru.
 *     - Bertanggung jawab atas perancangan algoritma Searching:
 *       * Binary Search berbasis kode SKU barang pada array terurut O(log N).
 *       * Linear Search multi-kriteria (kata kunci, kategori, rentang harga, ketersediaan).
 *     - Bertanggung jawab atas aturan bisnis validasi input (email, telepon, username, password hashing).
 *     - Bertanggung jawab atas kalkulasi durasi tanggal, diskon voucher promo, kalkulasi deposit jaminan,
 *       denda keterlambatan per hari, dan denda kondisi kerusakan barang (Baik, Rusak, Hilang).
 *     - Bertanggung jawab atas layanan pelaporan keuangan, metrik KPI, dan pemulihan stok otomatis.
 * 
 * [3] ANGGOTA 3 (UI & CONTROLLER LAYER SPECIALIST / INTEGRATOR AKHIR):
 *     - Bertanggung jawab atas perancangan visual antarmuka konsol (CLI) dengan border ASCII box drawing.
 *     - Bertanggung jawab atas formatter mata uang Rupiah otomatis dengan pemisah ribuan titik.
 *     - Bertanggung jawab atas tata letak tabel berpaginasi untuk katalog alat, keranjang, dan inventaris.
 *     - Bertanggung jawab atas format struk resmi cetak nota sewa dan berita acara pengembalian.
 *     - Bertanggung jawab atas navigasi menu interaktif Guest Mode, User Dashboard, dan Admin Panel.
 *     - Bertanggung jawab sebagai integrator akhir yang menyatukan seluruh layer menjadi 1 aplikasi utuh.
 * 
 * ----------------------------------------------------------------------------------------------------
 * PANDUAN KOMPILASI DAN EKSEKUSI MANDIRI (SINGLE-FILE RUN):
 * 1. Kompilasi Program Menggunakan G++ (Standar C++17):
 *    g++ -std=c++17 -Wall main_gabungan.cpp -o rental_camping.exe
 * 
 * 2. Menjalankan Mode Otomatis Diagnostik / Unit Testing:
 *    ./rental_camping.exe --test
 * 
 * 3. Menjalankan Aplikasi Interaktif Utama:
 *    ./rental_camping.exe
 * 
 * KREDENSIAL AKUN DEMO BAWAAN SISTEM:
 * - Administrator : Username: admin | Password: admin123
 * - Pelanggan 1   : Username: budi  | Password: budi123  (Member Silver)
 * - Pelanggan 2   : Username: siti  | Password: siti123  (Member Gold)
 * - Pelanggan 3   : Username: andi  | Password: andi123  (Member Bronze)
 * 
 * KODE KUPON PROMO:
 * - 'CAMPINGASIK10' : Diskon 10% s.d Rp 50.000
 * - 'WEEKENDSERU20' : Diskon 20% s.d Rp 100.000 (Min. Transaksi Rp 200.000)
 * - 'MEMBERBARU'    : Diskon Flat Rp 25.000
 * ====================================================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <ctime>
#include <cmath>
#include <cctype>
#include <limits>
#include <memory>
#include <filesystem>

#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace CampingRental {

    // Kode Warna ANSI untuk Antarmuka Konsol (CLI)
    namespace ConsoleColor {
        constexpr const char* RESET       = "\033[0m";
        constexpr const char* BOLD        = "\033[1m";
        constexpr const char* RED         = "\033[1;31m";
        constexpr const char* GREEN       = "\033[1;32m";
        constexpr const char* YELLOW      = "\033[1;33m";
        constexpr const char* BLUE        = "\033[1;34m";
        constexpr const char* BRIGHT_BLUE = "\033[94m";
        constexpr const char* CYAN        = "\033[1;36m";
    }

    // ================================================================================================
    // [BAGIAN 1: ENUMERASI & STRUKTUR DASAR SISTEM]
    // ================================================================================================

    enum class UserRole {
        ADMIN,
        USER,
        GUEST
    };

    enum class UserStatus {
        ACTIVE,
        SUSPENDED,
        BANNED
    };

    enum class GearCategory {
        TENDA,              // Tenda Dome, Tenda Kapasitas Besar, Flysheet Waterproof
        SLEEPING_GEAR,      // Sleeping Bag Polar, Matras Alumunium, Bantal Tiup
        CARRIER_BAG,        // Tas Carrier 45L, 60L, 80L, Daypack Gunung
        COOKING_EQUIPMENT,  // Kompor Gas Portable Ultralight, Nesting Cookware, Gas Can
        LIGHTING,           // Headlamp LED, Lentera Tenda Rechargeable Solar
        NAVIGATION_SAFETY,  // Kompas Bidik, P3K Outdoor Kit, Survival Gear, HT Baofeng
        OUTDOOR_COMFORT,    // Meja Lipat Aluminium, Kursi Lipat Duralumin, Hammock Double
        OTHER               // Aksesoris pelengkap lainnya
    };

    enum class TransactionStatus {
        PENDING_PAYMENT,     // Menunggu pembayaran dari penyewa
        WAITING_CONFIRMATION,// Menunggu verifikasi konfirmasi pembayaran oleh kasir/admin
        PAID_CONFIRMED,      // Pembayaran lunas, alat siap diambil di outlet
        ACTIVE_RENTED,       // Alat sedang dibawa dan dipakai penyewa di alam bebas
        RETURNED_COMPLETED,  // Pengembalian selesai normal tanpa kendala / denda
        RETURNED_WITH_ISSUE, // Pengembalian selesai dengan denda keterlambatan / kerusakan
        CANCELLED            // Pesanan sewa dibatalkan
    };

    enum class PaymentMethod {
        TRANSFER_BANK_BCA,
        TRANSFER_BANK_MANDIRI,
        TRANSFER_BANK_BRI,
        EWALLET_QRIS,
        EWALLET_GOPAY,
        EWALLET_OVO,
        EWALLET_DANA,
        CASH_ON_PICKUP
    };

    enum class PaymentStatus {
        UNPAID,
        PENDING_VERIFICATION,
        VERIFIED_PAID,
        FAILED_EXPIRED,
        REFUNDED
    };

    enum class ReturnCondition {
        GOOD_CONDITION,     // 100% Bersih & Mulus (Denda 0%)
        DIRTY_HEAVY,        // Kotor berat berlumpur butuh laundry khusus (Denda 10% nilai barang)
        MINOR_DAMAGE,       // Rusak ringan: sobek kecil, pasak bengkok (Denda 25% nilai barang)
        MAJOR_DAMAGE,       // Rusak berat: frame patah, robek besar (Denda 60% nilai barang)
        LOST                // Hilang total (Wajib mengganti 100% nilai barang)
    };

    enum class GearSortField {
        NAME_ASC,
        NAME_DESC,
        PRICE_ASC,
        PRICE_DESC,
        RATING_DESC,
        POPULARITY_DESC,
        AVAILABLE_STOCK_DESC
    };

    enum class TransactionSortField {
        INVOICE_ASC,
        INVOICE_DESC,
        DATE_ASC,
        DATE_DESC,
        TOTAL_ASC,
        TOTAL_DESC
    };

    // ================================================================================================
    // [FUNGSI BANTUAN KONVERSI ENUM KE STRING & SEBALIKNYA]
    // ================================================================================================

    inline std::string userRoleToString(UserRole role) {
        switch (role) {
            case UserRole::ADMIN: return "ADMIN";
            case UserRole::USER:  return "USER";
            case UserRole::GUEST: return "GUEST";
            default:              return "UNKNOWN";
        }
    }

    inline UserRole stringToUserRole(const std::string& str) {
        if (str == "ADMIN") return UserRole::ADMIN;
        if (str == "USER")  return UserRole::USER;
        return UserRole::GUEST;
    }

    inline std::string userStatusToString(UserStatus status) {
        switch (status) {
            case UserStatus::ACTIVE:    return "AKTIF";
            case UserStatus::SUSPENDED: return "DITANGGUHKAN";
            case UserStatus::BANNED:    return "DIBLOKIR";
            default:                    return "TIDAK DIKETAHUI";
        }
    }

    inline UserStatus stringToUserStatus(const std::string& str) {
        if (str == "AKTIF" || str == "ACTIVE") return UserStatus::ACTIVE;
        if (str == "DITANGGUHKAN" || str == "SUSPENDED") return UserStatus::SUSPENDED;
        if (str == "DIBLOKIR" || str == "BANNED") return UserStatus::BANNED;
        return UserStatus::ACTIVE;
    }

    inline std::string categoryToString(GearCategory cat) {
        switch (cat) {
            case GearCategory::TENDA:             return "Tenda & Shelter";
            case GearCategory::SLEEPING_GEAR:     return "Sleeping Gear";
            case GearCategory::CARRIER_BAG:       return "Carrier & Tas";
            case GearCategory::COOKING_EQUIPMENT: return "Alat Masak & Kompor";
            case GearCategory::LIGHTING:          return "Penerangan & Lentera";
            case GearCategory::NAVIGATION_SAFETY: return "Navigasi & Safety";
            case GearCategory::OUTDOOR_COMFORT:   return "Outdoor Comfort";
            case GearCategory::OTHER:             return "Aksesoris Lainnya";
            default:                              return "Umum";
        }
    }

    inline GearCategory stringToCategory(const std::string& str) {
        if (str == "Tenda & Shelter" || str == "TENDA") return GearCategory::TENDA;
        if (str == "Sleeping Gear" || str == "SLEEPING_GEAR") return GearCategory::SLEEPING_GEAR;
        if (str == "Carrier & Tas" || str == "CARRIER_BAG") return GearCategory::CARRIER_BAG;
        if (str == "Alat Masak & Kompor" || str == "COOKING_EQUIPMENT") return GearCategory::COOKING_EQUIPMENT;
        if (str == "Penerangan & Lentera" || str == "LIGHTING") return GearCategory::LIGHTING;
        if (str == "Navigasi & Safety" || str == "NAVIGATION_SAFETY") return GearCategory::NAVIGATION_SAFETY;
        if (str == "Outdoor Comfort" || str == "OUTDOOR_COMFORT") return GearCategory::OUTDOOR_COMFORT;
        return GearCategory::OTHER;
    }

    inline std::string transactionStatusToString(TransactionStatus status) {
        switch (status) {
            case TransactionStatus::PENDING_PAYMENT:      return "MENUNGGU PEMBAYARAN";
            case TransactionStatus::WAITING_CONFIRMATION:  return "MENUNGGU KONFIRMASI";
            case TransactionStatus::PAID_CONFIRMED:       return "DIBAYAR (SIAP DIAMBIL)";
            case TransactionStatus::ACTIVE_RENTED:        return "SEDANG DISEWA";
            case TransactionStatus::RETURNED_COMPLETED:   return "SELESAI (KEMBALI)";
            case TransactionStatus::RETURNED_WITH_ISSUE:  return "SELESAI (ADA DENDA)";
            case TransactionStatus::CANCELLED:            return "DIBATALKAN";
            default:                                      return "STATUS TIDAK DIKETAHUI";
        }
    }

    inline TransactionStatus stringToTransactionStatus(const std::string& str) {
        if (str == "MENUNGGU PEMBAYARAN" || str == "PENDING_PAYMENT") return TransactionStatus::PENDING_PAYMENT;
        if (str == "MENUNGGU KONFIRMASI" || str == "WAITING_CONFIRMATION") return TransactionStatus::WAITING_CONFIRMATION;
        if (str == "DIBAYAR (SIAP DIAMBIL)" || str == "PAID_CONFIRMED") return TransactionStatus::PAID_CONFIRMED;
        if (str == "SEDANG DISEWA" || str == "ACTIVE_RENTED") return TransactionStatus::ACTIVE_RENTED;
        if (str == "SELESAI (KEMBALI)" || str == "RETURNED_COMPLETED") return TransactionStatus::RETURNED_COMPLETED;
        if (str == "SELESAI (ADA DENDA)" || str == "RETURNED_WITH_ISSUE") return TransactionStatus::RETURNED_WITH_ISSUE;
        if (str == "DIBATALKAN" || str == "CANCELLED") return TransactionStatus::CANCELLED;
        return TransactionStatus::PENDING_PAYMENT;
    }

    inline std::string paymentMethodToString(PaymentMethod method) {
        switch (method) {
            case PaymentMethod::TRANSFER_BANK_BCA:     return "Transfer BCA";
            case PaymentMethod::TRANSFER_BANK_MANDIRI: return "Transfer Mandiri";
            case PaymentMethod::TRANSFER_BANK_BRI:     return "Transfer BRI";
            case PaymentMethod::EWALLET_QRIS:          return "QRIS Instant";
            case PaymentMethod::EWALLET_GOPAY:         return "GoPay";
            case PaymentMethod::EWALLET_OVO:           return "OVO";
            case PaymentMethod::EWALLET_DANA:          return "DANA";
            case PaymentMethod::CASH_ON_PICKUP:        return "Tunai saat Pengambilan (COD)";
            default:                                   return "Metode Lain";
        }
    }

    inline PaymentMethod stringToPaymentMethod(const std::string& str) {
        if (str == "Transfer BCA" || str == "TRANSFER_BANK_BCA") return PaymentMethod::TRANSFER_BANK_BCA;
        if (str == "Transfer Mandiri" || str == "TRANSFER_BANK_MANDIRI") return PaymentMethod::TRANSFER_BANK_MANDIRI;
        if (str == "Transfer BRI" || str == "TRANSFER_BANK_BRI") return PaymentMethod::TRANSFER_BANK_BRI;
        if (str == "QRIS Instant" || str == "EWALLET_QRIS") return PaymentMethod::EWALLET_QRIS;
        if (str == "GoPay" || str == "EWALLET_GOPAY") return PaymentMethod::EWALLET_GOPAY;
        if (str == "OVO" || str == "EWALLET_OVO") return PaymentMethod::EWALLET_OVO;
        if (str == "DANA" || str == "EWALLET_DANA") return PaymentMethod::EWALLET_DANA;
        return PaymentMethod::CASH_ON_PICKUP;
    }

    inline std::string paymentStatusToString(PaymentStatus status) {
        switch (status) {
            case PaymentStatus::UNPAID:               return "BELUM BAYAR";
            case PaymentStatus::PENDING_VERIFICATION: return "VERIFIKASI";
            case PaymentStatus::VERIFIED_PAID:        return "LUNAS";
            case PaymentStatus::FAILED_EXPIRED:       return "KADALUWARSA";
            case PaymentStatus::REFUNDED:             return "PENGEMBALIAN DANA";
            default:                                  return "TIDAK DIKETAHUI";
        }
    }

    inline PaymentStatus stringToPaymentStatus(const std::string& str) {
        if (str == "BELUM BAYAR" || str == "UNPAID") return PaymentStatus::UNPAID;
        if (str == "VERIFIKASI" || str == "PENDING_VERIFICATION") return PaymentStatus::PENDING_VERIFICATION;
        if (str == "LUNAS" || str == "VERIFIED_PAID") return PaymentStatus::VERIFIED_PAID;
        if (str == "KADALUWARSA" || str == "FAILED_EXPIRED") return PaymentStatus::FAILED_EXPIRED;
        if (str == "PENGEMBALIAN DANA" || str == "REFUNDED") return PaymentStatus::REFUNDED;
        return PaymentStatus::UNPAID;
    }

    inline std::string returnConditionToString(ReturnCondition cond) {
        switch (cond) {
            case ReturnCondition::GOOD_CONDITION: return "Baik Sempurna";
            case ReturnCondition::DIRTY_HEAVY:    return "Kotor Berat";
            case ReturnCondition::MINOR_DAMAGE:   return "Rusak Ringan";
            case ReturnCondition::MAJOR_DAMAGE:   return "Rusak Berat";
            case ReturnCondition::LOST:           return "Hilang";
            default:                              return "Tidak Diketahui";
        }
    }

    inline ReturnCondition stringToReturnCondition(const std::string& str) {
        if (str == "Baik Sempurna" || str == "GOOD_CONDITION") return ReturnCondition::GOOD_CONDITION;
        if (str == "Kotor Berat" || str == "DIRTY_HEAVY") return ReturnCondition::DIRTY_HEAVY;
        if (str == "Rusak Ringan" || str == "MINOR_DAMAGE") return ReturnCondition::MINOR_DAMAGE;
        if (str == "Rusak Berat" || str == "MAJOR_DAMAGE") return ReturnCondition::MAJOR_DAMAGE;
        if (str == "Hilang" || str == "LOST") return ReturnCondition::LOST;
        return ReturnCondition::GOOD_CONDITION;
    }

    // ================================================================================================
    // [MODEL TAMBAHAN: PAKET HEMAT CAMPING (CAMPING BUNDLE PACKAGE)]
    // ================================================================================================
    struct BundleItem {
        int gearId;
        std::string gearName;
        int quantity;
    };

    class CampingBundle {
    private:
        int id;
        std::string code;
        std::string name;
        std::string targetDescription; // Cth: "Paket Lengkap 4 Orang Siap Naik Gunung"
        std::vector<BundleItem> bundleItems;
        double normalDailyPrice;
        double packageDailyPrice;      // Harga diskon paket hemat
        double packageDeposit;

    public:
        CampingBundle()
            : id(0), code(""), name(""), targetDescription(""),
              normalDailyPrice(0.0), packageDailyPrice(0.0), packageDeposit(0.0) {}

        CampingBundle(int bId, const std::string& bCode, const std::string& bName,
                      const std::string& desc, double normalPrice, double packPrice, double dep)
            : id(bId), code(bCode), name(bName), targetDescription(desc),
              normalDailyPrice(normalPrice), packageDailyPrice(packPrice), packageDeposit(dep) {}

        int getId() const { return id; }
        std::string getCode() const { return code; }
        std::string getName() const { return name; }
        std::string getTargetDescription() const { return targetDescription; }
        const std::vector<BundleItem>& getBundleItems() const { return bundleItems; }
        double getNormalDailyPrice() const { return normalDailyPrice; }
        double getPackageDailyPrice() const { return packageDailyPrice; }
        double getPackageDeposit() const { return packageDeposit; }

        double calculateSavingsPerDay() const {
            return std::max(0.0, normalDailyPrice - packageDailyPrice);
        }

        double calculateSavingsPercentage() const {
            if (normalDailyPrice <= 0.0) return 0.0;
            return ((normalDailyPrice - packageDailyPrice) / normalDailyPrice) * 100.0;
        }

        void addBundleItem(int gId, const std::string& gName, int qty) {
            bundleItems.push_back({gId, gName, qty});
        }
    };

    // ================================================================================================
    // [STRUKTUR TIER MEMBER & LOYALITAS PELANGGAN]
    // ================================================================================================
    enum class MembershipTier {
        BRONZE_EXPLORER,    // < 3 sewa
        SILVER_ADVENTURER,  // 3 - 5 sewa (Diskon 5%)
        GOLD_MOUNTAINEER,   // 6 - 10 sewa (Diskon 10%)
        PLATINUM_EXPEDITION // > 10 sewa (Diskon 15%)
    };

    inline std::string membershipTierToString(MembershipTier tier) {
        switch (tier) {
            case MembershipTier::BRONZE_EXPLORER:    return "Bronze Explorer";
            case MembershipTier::SILVER_ADVENTURER:  return "Silver Adventurer (Diskon 5%)";
            case MembershipTier::GOLD_MOUNTAINEER:   return "Gold Mountaineer (Diskon 10%)";
            case MembershipTier::PLATINUM_EXPEDITION: return "Platinum Expedition (Diskon 15%)";
            default:                                 return "Member Reguler";
        }
    }

    inline double getTierDiscountRate(MembershipTier tier) {
        switch (tier) {
            case MembershipTier::BRONZE_EXPLORER:    return 0.00;
            case MembershipTier::SILVER_ADVENTURER:  return 0.05;
            case MembershipTier::GOLD_MOUNTAINEER:   return 0.10;
            case MembershipTier::PLATINUM_EXPEDITION: return 0.15;
            default:                                 return 0.00;
        }
    }

    inline MembershipTier calculateUserTier(int rentalsCompleted, double trustScore) {
        if (trustScore < 70.0) return MembershipTier::BRONZE_EXPLORER;
        if (rentalsCompleted >= 10 && trustScore >= 90.0) return MembershipTier::PLATINUM_EXPEDITION;
        if (rentalsCompleted >= 6 && trustScore >= 85.0) return MembershipTier::GOLD_MOUNTAINEER;
        if (rentalsCompleted >= 3 && trustScore >= 80.0) return MembershipTier::SILVER_ADVENTURER;
        return MembershipTier::BRONZE_EXPLORER;
    }

    // ================================================================================================
    // [STRUKTUR AUDIT LOG TRANSAKSI & PERUBAHAN SISTEM]
    // ================================================================================================
    struct SystemAuditEntry {
        std::string timestamp;
        std::string actor;
        std::string actionCategory;
        std::string detailMessage;
    };

    struct ServiceResult {
        bool success;
        std::string message;
        std::string errorCode;

        ServiceResult(bool s = false, const std::string& msg = "", const std::string& code = "")
            : success(s), message(msg), errorCode(code) {}

        static ServiceResult ok(const std::string& msg = "Operasi Berhasil") {
            return ServiceResult(true, msg, "OK");
        }

        static ServiceResult fail(const std::string& msg, const std::string& code = "ERR") {
            return ServiceResult(false, msg, code);
        }
    };

    struct UserSession {
        int userId;
        std::string username;
        std::string fullName;
        UserRole role;
        bool isLoggedIn;

        UserSession()
            : userId(0), username(""), fullName(""), role(UserRole::GUEST), isLoggedIn(false) {}

        void clear() {
            userId = 0;
            username = "";
            fullName = "";
            role = UserRole::GUEST;
            isLoggedIn = false;
        }
    };

    // ================================================================================================
    // [BAGIAN 2: CLASS ENTITAS MODEL DATA (ANGGOTA 1 - DATA LAYER)]
    // ================================================================================================

    class User {
    private:
        int id;
        std::string username;
        std::string passwordHash;
        std::string fullName;
        std::string phoneNumber;
        std::string email;
        std::string identityCardNumber;
        std::string address;
        UserRole role;
        UserStatus status;
        int totalRentalsCompleted;
        double trustScore;
        std::string registeredAt;

    public:
        User()
            : id(0), username(""), passwordHash(""), fullName(""),
              phoneNumber(""), email(""), identityCardNumber(""),
              address(""), role(UserRole::USER), status(UserStatus::ACTIVE),
              totalRentalsCompleted(0), trustScore(100.0), registeredAt("2026-11-07") {}

        User(int uId, const std::string& uName, const std::string& pwd,
             const std::string& name, const std::string& phone,
             const std::string& mail, const std::string& idCard,
             const std::string& addr, UserRole uRole, UserStatus uStat,
             int rentals = 0, double trust = 100.0, const std::string& regDate = "2026-11-07")
            : id(uId), username(uName), passwordHash(pwd), fullName(name),
              phoneNumber(phone), email(mail), identityCardNumber(idCard),
              address(addr), role(uRole), status(uStat),
              totalRentalsCompleted(rentals), trustScore(trust),
              registeredAt(regDate.empty() ? "2026-11-07" : regDate) {}

        // Getters
        int getId() const { return id; }
        std::string getUsername() const { return username; }
        std::string getPasswordHash() const { return passwordHash; }
        std::string getFullName() const { return fullName; }
        std::string getPhoneNumber() const { return phoneNumber; }
        std::string getEmail() const { return email; }
        std::string getIdentityCardNumber() const { return identityCardNumber; }
        std::string getAddress() const { return address; }
        UserRole getRole() const { return role; }
        UserStatus getStatus() const { return status; }
        int getTotalRentalsCompleted() const { return totalRentalsCompleted; }
        double getTrustScore() const { return trustScore; }
        std::string getRegisteredAt() const { return registeredAt; }

        // Setters
        void setId(int newId) { id = newId; }
        void setUsername(const std::string& u) { username = u; }
        void setPasswordHash(const std::string& p) { passwordHash = p; }
        void setFullName(const std::string& fn) { fullName = fn; }
        void setPhoneNumber(const std::string& ph) { phoneNumber = ph; }
        void setEmail(const std::string& em) { email = em; }
        void setIdentityCardNumber(const std::string& idc) { identityCardNumber = idc; }
        void setAddress(const std::string& addr) { address = addr; }
        void setRole(UserRole r) { role = r; }
        void setStatus(UserStatus s) { status = s; }
        void setTotalRentalsCompleted(int count) { totalRentalsCompleted = count; }
        void incrementRentalCount() { totalRentalsCompleted++; }
        void setTrustScore(double score) { trustScore = std::max(0.0, std::min(100.0, score)); }
        void setRegisteredAt(const std::string& reg) { registeredAt = reg; }

        bool isAdmin() const { return role == UserRole::ADMIN; }
        bool isActive() const { return status == UserStatus::ACTIVE; }

        // Serialisasi & Deserialisasi
        std::string serialize() const {
            std::string reg = registeredAt.empty() ? "2026-11-07" : registeredAt;
            std::ostringstream oss;
            oss << id << "|"
                << username << "|"
                << passwordHash << "|"
                << fullName << "|"
                << phoneNumber << "|"
                << email << "|"
                << identityCardNumber << "|"
                << address << "|"
                << userRoleToString(role) << "|"
                << userStatusToString(status) << "|"
                << totalRentalsCompleted << "|"
                << std::fixed << std::setprecision(1) << trustScore << "|"
                << reg;
            return oss.str();
        }

        static User deserialize(const std::string& line) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 12) {
                return User();
            }
            User u;
            u.id = std::stoi(tokens[0]);
            u.username = tokens[1];
            u.passwordHash = tokens[2];
            u.fullName = tokens[3];
            u.phoneNumber = tokens[4];
            u.email = tokens[5];
            u.identityCardNumber = tokens[6];
            u.address = tokens[7];
            u.role = stringToUserRole(tokens[8]);
            u.status = stringToUserStatus(tokens[9]);
            u.totalRentalsCompleted = std::stoi(tokens[10]);
            u.trustScore = std::stod(tokens[11]);
            u.registeredAt = (tokens.size() >= 13 && tokens[12] != "-") ? tokens[12] : "2026-11-07";
            return u;
        }
    };

    class CampingGear {
    private:
        int id;
        std::string sku;
        std::string name;
        std::string brand;
        GearCategory category;
        double rentalPricePerDay;
        double replacementValue;
        int stockTotal;
        int stockAvailable;
        int timesRented;
        double rating;
        int reviewCount;
        std::string specification;
        std::string conditionNotes;

    public:
        CampingGear()
            : id(0), sku(""), name(""), brand(""), category(GearCategory::OTHER),
              rentalPricePerDay(0.0), replacementValue(0.0), stockTotal(0),
              stockAvailable(0), timesRented(0), rating(5.0), reviewCount(0),
              specification(""), conditionNotes("Bagus") {}

        CampingGear(int gId, const std::string& gSku, const std::string& gName,
                    const std::string& gBrand, GearCategory cat, double price,
                    double replValue, int total, int avail, int rented = 0,
                    double rate = 5.0, int revCount = 0,
                    const std::string& spec = "", const std::string& notes = "Bagus")
            : id(gId), sku(gSku), name(gName), brand(gBrand), category(cat),
              rentalPricePerDay(price), replacementValue(replValue),
              stockTotal(total), stockAvailable(avail), timesRented(rented),
              rating(rate), reviewCount(revCount), specification(spec),
              conditionNotes(notes) {}

        int getId() const { return id; }
        std::string getSku() const { return sku; }
        std::string getName() const { return name; }
        std::string getBrand() const { return brand; }
        GearCategory getCategory() const { return category; }
        double getRentalPricePerDay() const { return rentalPricePerDay; }
        double getReplacementValue() const { return replacementValue; }
        int getStockTotal() const { return stockTotal; }
        int getStockAvailable() const { return stockAvailable; }
        int getTimesRented() const { return timesRented; }
        double getRating() const { return rating; }
        int getReviewCount() const { return reviewCount; }
        std::string getSpecification() const { return specification; }
        std::string getConditionNotes() const { return conditionNotes; }

        void setId(int newId) { id = newId; }
        void setSku(const std::string& s) { sku = s; }
        void setName(const std::string& n) { name = n; }
        void setBrand(const std::string& b) { brand = b; }
        void setCategory(GearCategory c) { category = c; }
        void setRentalPricePerDay(double p) { rentalPricePerDay = p; }
        void setReplacementValue(double r) { replacementValue = r; }
        void setStockTotal(int t) { stockTotal = t; }
        void setStockAvailable(int a) { stockAvailable = a; }
        void setTimesRented(int r) { timesRented = r; }
        void setRating(double r) { rating = r; }
        void setReviewCount(int rc) { reviewCount = rc; }
        void setSpecification(const std::string& s) { specification = s; }
        void setConditionNotes(const std::string& c) { conditionNotes = c; }

        bool canRent(int qty) const {
            return (stockAvailable >= qty && qty > 0);
        }

        bool rentUnits(int qty) {
            if (canRent(qty)) {
                stockAvailable -= qty;
                timesRented += qty;
                return true;
            }
            return false;
        }

        void returnUnits(int qty) {
            stockAvailable = std::min(stockTotal, stockAvailable + qty);
        }

        void addReview(double newRating) {
            double totalPoints = (rating * reviewCount) + newRating;
            reviewCount++;
            rating = totalPoints / reviewCount;
        }

        std::string serialize() const {
            std::ostringstream oss;
            oss << id << "|"
                << sku << "|"
                << name << "|"
                << brand << "|"
                << categoryToString(category) << "|"
                << std::fixed << std::setprecision(0) << rentalPricePerDay << "|"
                << std::fixed << std::setprecision(0) << replacementValue << "|"
                << stockTotal << "|"
                << stockAvailable << "|"
                << timesRented << "|"
                << std::fixed << std::setprecision(1) << rating << "|"
                << reviewCount << "|"
                << specification << "|"
                << conditionNotes;
            return oss.str();
        }

        static CampingGear deserialize(const std::string& line) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 14) {
                return CampingGear();
            }
            CampingGear g;
            g.id = std::stoi(tokens[0]);
            g.sku = tokens[1];
            g.name = tokens[2];
            g.brand = tokens[3];
            g.category = stringToCategory(tokens[4]);
            g.rentalPricePerDay = std::stod(tokens[5]);
            g.replacementValue = std::stod(tokens[6]);
            g.stockTotal = std::stoi(tokens[7]);
            g.stockAvailable = std::stoi(tokens[8]);
            g.timesRented = std::stoi(tokens[9]);
            g.rating = std::stod(tokens[10]);
            g.reviewCount = std::stoi(tokens[11]);
            g.specification = tokens[12];
            g.conditionNotes = tokens[13];
            return g;
        }
    };

    class RentalItem {
    private:
        int gearId;
        std::string gearSku;
        std::string gearName;
        double pricePerDay;
        double replacementValue;
        int quantity;
        int rentalDays;
        ReturnCondition returnCondition;
        double damageFee;

    public:
        RentalItem()
            : gearId(0), gearSku(""), gearName(""), pricePerDay(0.0),
              replacementValue(0.0), quantity(0), rentalDays(1),
              returnCondition(ReturnCondition::GOOD_CONDITION), damageFee(0.0) {}

        RentalItem(int gId, const std::string& sku, const std::string& name,
                   double price, double replVal, int qty, int days = 1)
            : gearId(gId), gearSku(sku), gearName(name), pricePerDay(price),
              replacementValue(replVal), quantity(qty), rentalDays(days),
              returnCondition(ReturnCondition::GOOD_CONDITION), damageFee(0.0) {}

        int getGearId() const { return gearId; }
        std::string getGearSku() const { return gearSku; }
        std::string getGearName() const { return gearName; }
        double getPricePerDay() const { return pricePerDay; }
        double getReplacementValue() const { return replacementValue; }
        int getQuantity() const { return quantity; }
        int getRentalDays() const { return rentalDays; }
        ReturnCondition getReturnCondition() const { return returnCondition; }
        double getDamageFee() const { return damageFee; }

        void setQuantity(int q) { quantity = q; }
        void setRentalDays(int d) { rentalDays = d; }
        void setReturnCondition(ReturnCondition cond) { returnCondition = cond; }
        void setDamageFee(double fee) { damageFee = fee; }

        double calculateSubtotal() const {
            return pricePerDay * quantity * rentalDays;
        }

        std::string serializeItem() const {
            std::ostringstream oss;
            oss << gearId << "#"
                << gearSku << "#"
                << gearName << "#"
                << std::fixed << std::setprecision(0) << pricePerDay << "#"
                << std::fixed << std::setprecision(0) << replacementValue << "#"
                << quantity << "#"
                << rentalDays << "#"
                << returnConditionToString(returnCondition) << "#"
                << std::fixed << std::setprecision(0) << damageFee;
            return oss.str();
        }

        static RentalItem deserializeItem(const std::string& str) {
            std::stringstream ss(str);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, '#')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 9) return RentalItem();
            RentalItem item;
            item.gearId = std::stoi(tokens[0]);
            item.gearSku = tokens[1];
            item.gearName = tokens[2];
            item.pricePerDay = std::stod(tokens[3]);
            item.replacementValue = std::stod(tokens[4]);
            item.quantity = std::stoi(tokens[5]);
            item.rentalDays = std::stoi(tokens[6]);
            item.returnCondition = stringToReturnCondition(tokens[7]);
            item.damageFee = std::stod(tokens[8]);
            return item;
        }
    };

    class Cart {
    private:
        int userId;
        std::vector<RentalItem> items;
        int defaultRentalDays;

    public:
        Cart() : userId(0), defaultRentalDays(1) {}
        explicit Cart(int uId) : userId(uId), defaultRentalDays(1) {}

        int getUserId() const { return userId; }
        void setUserId(int uId) { userId = uId; }
        int getDefaultRentalDays() const { return defaultRentalDays; }
        void setDefaultRentalDays(int days) {
            defaultRentalDays = std::max(1, days);
            for (auto& it : items) {
                it.setRentalDays(defaultRentalDays);
            }
        }

        const std::vector<RentalItem>& getItems() const { return items; }
        std::vector<RentalItem>& getItemsRef() { return items; }

        void addItem(const CampingGear& gear, int quantity, int days) {
            for (auto& item : items) {
                if (item.getGearId() == gear.getId()) {
                    item.setQuantity(item.getQuantity() + quantity);
                    item.setRentalDays(days);
                    return;
                }
            }
            items.push_back(RentalItem(gear.getId(), gear.getSku(), gear.getName(),
                                       gear.getRentalPricePerDay(), gear.getReplacementValue(),
                                       quantity, days));
        }

        bool updateQuantity(int gearId, int newQuantity) {
            for (auto it = items.begin(); it != items.end(); ++it) {
                if (it->getGearId() == gearId) {
                    if (newQuantity <= 0) {
                        items.erase(it);
                    } else {
                        it->setQuantity(newQuantity);
                    }
                    return true;
                }
            }
            return false;
        }

        bool removeItem(int gearId) {
            for (auto it = items.begin(); it != items.end(); ++it) {
                if (it->getGearId() == gearId) {
                    items.erase(it);
                    return true;
                }
            }
            return false;
        }

        void clear() {
            items.clear();
        }

        bool isEmpty() const {
            return items.empty();
        }

        int getTotalItemCount() const {
            int count = 0;
            for (const auto& it : items) {
                count += it.getQuantity();
            }
            return count;
        }

        double calculateSubtotal() const {
            double total = 0.0;
            for (const auto& it : items) {
                total += it.calculateSubtotal();
            }
            return total;
        }
    };

    class PromoCode {
    private:
        std::string code;
        double discountPercentage;
        double maxDiscountAmount;
        double minTransactionAmount;
        bool isActive;
        std::string description;

    public:
        PromoCode()
            : code(""), discountPercentage(0.0), maxDiscountAmount(0.0),
              minTransactionAmount(0.0), isActive(false), description("") {}

        PromoCode(const std::string& c, double pct, double maxDisc, double minTrx,
                  bool active, const std::string& desc)
            : code(c), discountPercentage(pct), maxDiscountAmount(maxDisc),
              minTransactionAmount(minTrx), isActive(active), description(desc) {}

        std::string getCode() const { return code; }
        double getDiscountPercentage() const { return discountPercentage; }
        double getMaxDiscountAmount() const { return maxDiscountAmount; }
        double getMinTransactionAmount() const { return minTransactionAmount; }
        bool getIsActive() const { return isActive; }
        std::string getDescription() const { return description; }

        double calculateDiscount(double subtotal) const {
            if (!isActive || subtotal < minTransactionAmount) return 0.0;
            double disc = subtotal * (discountPercentage / 100.0);
            if (maxDiscountAmount > 0.0 && disc > maxDiscountAmount) {
                disc = maxDiscountAmount;
            }
            return disc;
        }
    };

    class RentalTransaction {
    private:
        std::string invoiceNumber;
        int userId;
        std::string customerName;
        std::string customerPhone;
        std::string customerIdCard;
        std::string bookingDate;
        std::string startDate;
        std::string expectedReturnDate;
        std::string actualReturnDate;
        std::vector<RentalItem> items;
        double subtotal;
        double discountAmount;
        std::string promoCodeUsed;
        double depositAmount;
        double totalAmount;
        PaymentMethod paymentMethod;
        PaymentStatus paymentStatus;
        TransactionStatus transactionStatus;
        std::string paymentProofRef;
        double lateFee;
        double damagePenaltyFee;
        double depositRefunded;
        std::string notes;

    public:
        RentalTransaction()
            : invoiceNumber(""), userId(0), customerName(""), customerPhone(""),
              customerIdCard(""), bookingDate(""), startDate(""), expectedReturnDate(""),
              actualReturnDate("-"), subtotal(0.0), discountAmount(0.0), promoCodeUsed("-"),
              depositAmount(0.0), totalAmount(0.0), paymentMethod(PaymentMethod::TRANSFER_BANK_BCA),
              paymentStatus(PaymentStatus::UNPAID), transactionStatus(TransactionStatus::PENDING_PAYMENT),
              paymentProofRef(""), lateFee(0.0), damagePenaltyFee(0.0), depositRefunded(0.0),
              notes("") {}

        std::string getInvoiceNumber() const { return invoiceNumber; }
        int getUserId() const { return userId; }
        std::string getCustomerName() const { return customerName; }
        std::string getCustomerPhone() const { return customerPhone; }
        std::string getCustomerIdCard() const { return customerIdCard; }
        std::string getBookingDate() const { return bookingDate; }
        std::string getStartDate() const { return startDate; }
        std::string getExpectedReturnDate() const { return expectedReturnDate; }
        std::string getActualReturnDate() const { return actualReturnDate; }
        const std::vector<RentalItem>& getItems() const { return items; }
        std::vector<RentalItem>& getItemsRef() { return items; }
        double getSubtotal() const { return subtotal; }
        double getDiscountAmount() const { return discountAmount; }
        std::string getPromoCodeUsed() const { return promoCodeUsed; }
        double getDepositAmount() const { return depositAmount; }
        double getTotalAmount() const { return totalAmount; }
        PaymentMethod getPaymentMethod() const { return paymentMethod; }
        PaymentStatus getPaymentStatus() const { return paymentStatus; }
        TransactionStatus getTransactionStatus() const { return transactionStatus; }
        std::string getPaymentProofRef() const { return paymentProofRef; }
        double getLateFee() const { return lateFee; }
        double getDamagePenaltyFee() const { return damagePenaltyFee; }
        double getDepositRefunded() const { return depositRefunded; }
        std::string getNotes() const { return notes; }

        void setInvoiceNumber(const std::string& inv) { invoiceNumber = inv; }
        void setUserId(int uid) { userId = uid; }
        void setCustomerName(const std::string& name) { customerName = name; }
        void setCustomerPhone(const std::string& phone) { customerPhone = phone; }
        void setCustomerIdCard(const std::string& idc) { customerIdCard = idc; }
        void setBookingDate(const std::string& dt) { bookingDate = dt; }
        void setStartDate(const std::string& dt) { startDate = dt; }
        void setExpectedReturnDate(const std::string& dt) { expectedReturnDate = dt; }
        void setActualReturnDate(const std::string& dt) { actualReturnDate = dt; }
        void setItems(const std::vector<RentalItem>& it) { items = it; }
        void addItem(const RentalItem& item) { items.push_back(item); }
        void setSubtotal(double sub) { subtotal = sub; }
        void setDiscountAmount(double disc) { discountAmount = disc; }
        void setPromoCodeUsed(const std::string& code) { promoCodeUsed = code; }
        void setDepositAmount(double dep) { depositAmount = dep; }
        void setTotalAmount(double tot) { totalAmount = tot; }
        void setPaymentMethod(PaymentMethod method) { paymentMethod = method; }
        void setPaymentStatus(PaymentStatus stat) { paymentStatus = stat; }
        void setTransactionStatus(TransactionStatus stat) { transactionStatus = stat; }
        void setPaymentProofRef(const std::string& ref) { paymentProofRef = ref; }
        void setLateFee(double fee) { lateFee = fee; }
        void setDamagePenaltyFee(double fee) { damagePenaltyFee = fee; }
        void setDepositRefunded(double ref) { depositRefunded = ref; }
        void setNotes(const std::string& n) { notes = n; }

        std::string serialize() const {
            std::ostringstream oss;
            oss << invoiceNumber << "|"
                << userId << "|"
                << customerName << "|"
                << customerPhone << "|"
                << customerIdCard << "|"
                << bookingDate << "|"
                << startDate << "|"
                << expectedReturnDate << "|"
                << actualReturnDate << "|"
                << std::fixed << std::setprecision(0) << subtotal << "|"
                << std::fixed << std::setprecision(0) << discountAmount << "|"
                << promoCodeUsed << "|"
                << std::fixed << std::setprecision(0) << depositAmount << "|"
                << std::fixed << std::setprecision(0) << totalAmount << "|"
                << paymentMethodToString(paymentMethod) << "|"
                << paymentStatusToString(paymentStatus) << "|"
                << transactionStatusToString(transactionStatus) << "|"
                << paymentProofRef << "|"
                << std::fixed << std::setprecision(0) << lateFee << "|"
                << std::fixed << std::setprecision(0) << damagePenaltyFee << "|"
                << std::fixed << std::setprecision(0) << depositRefunded << "|"
                << notes << "|";

            for (size_t i = 0; i < items.size(); ++i) {
                oss << items[i].serializeItem();
                if (i + 1 < items.size()) {
                    oss << ";";
                }
            }
            return oss.str();
        }

        static RentalTransaction deserialize(const std::string& line) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 22) {
                return RentalTransaction();
            }
            RentalTransaction tx;
            tx.invoiceNumber = tokens[0];
            tx.userId = std::stoi(tokens[1]);
            tx.customerName = tokens[2];
            tx.customerPhone = tokens[3];
            tx.customerIdCard = tokens[4];
            tx.bookingDate = tokens[5];
            tx.startDate = tokens[6];
            tx.expectedReturnDate = tokens[7];
            tx.actualReturnDate = tokens[8];
            tx.subtotal = std::stod(tokens[9]);
            tx.discountAmount = std::stod(tokens[10]);
            tx.promoCodeUsed = tokens[11];
            tx.depositAmount = std::stod(tokens[12]);
            tx.totalAmount = std::stod(tokens[13]);
            tx.paymentMethod = stringToPaymentMethod(tokens[14]);
            tx.paymentStatus = stringToPaymentStatus(tokens[15]);
            tx.transactionStatus = stringToTransactionStatus(tokens[16]);
            tx.paymentProofRef = tokens[17];
            tx.lateFee = std::stod(tokens[18]);
            tx.damagePenaltyFee = std::stod(tokens[19]);
            tx.depositRefunded = std::stod(tokens[20]);
            tx.notes = tokens[21];

            if (tokens.size() > 22 && !tokens[22].empty()) {
                std::stringstream itemStream(tokens[22]);
                std::string itemChunk;
                while (std::getline(itemStream, itemChunk, ';')) {
                    if (!itemChunk.empty()) {
                        tx.items.push_back(RentalItem::deserializeItem(itemChunk));
                    }
                }
            }
            return tx;
        }
    };

    class ReturnLog {
    private:
        int id;
        std::string invoiceNumber;
        std::string returnDate;
        int overdueDays;
        double lateFeeCalculated;
        double damageFeeCalculated;
        double totalPenalty;
        double depositHeld;
        double depositRefund;
        double extraPaymentDue;
        std::string inspectorAdminName;
        std::string resolutionNotes;

    public:
        ReturnLog()
            : id(0), invoiceNumber(""), returnDate(""), overdueDays(0),
              lateFeeCalculated(0.0), damageFeeCalculated(0.0), totalPenalty(0.0),
              depositHeld(0.0), depositRefund(0.0), extraPaymentDue(0.0),
              inspectorAdminName("Admin"), resolutionNotes("") {}

        ReturnLog(int lId, const std::string& inv, const std::string& rDate,
                  int lateDays, double lateFee, double damageFee,
                  double deposit, const std::string& admin, const std::string& notes)
            : id(lId), invoiceNumber(inv), returnDate(rDate), overdueDays(lateDays),
              lateFeeCalculated(lateFee), damageFeeCalculated(damageFee),
              depositHeld(deposit), inspectorAdminName(admin), resolutionNotes(notes) {
            totalPenalty = lateFeeCalculated + damageFeeCalculated;
            if (depositHeld >= totalPenalty) {
                depositRefund = depositHeld - totalPenalty;
                extraPaymentDue = 0.0;
            } else {
                depositRefund = 0.0;
                extraPaymentDue = totalPenalty - depositHeld;
            }
        }

        int getId() const { return id; }
        std::string getInvoiceNumber() const { return invoiceNumber; }
        std::string getReturnDate() const { return returnDate; }
        int getOverdueDays() const { return overdueDays; }
        double getLateFeeCalculated() const { return lateFeeCalculated; }
        double getDamageFeeCalculated() const { return damageFeeCalculated; }
        double getTotalPenalty() const { return totalPenalty; }
        double getDepositHeld() const { return depositHeld; }
        double getDepositRefund() const { return depositRefund; }
        double getExtraPaymentDue() const { return extraPaymentDue; }
        std::string getInspectorAdminName() const { return inspectorAdminName; }
        std::string getResolutionNotes() const { return resolutionNotes; }

        void setId(int nid) { id = nid; }
        void setInvoiceNumber(const std::string& inv) { invoiceNumber = inv; }
        void setReturnDate(const std::string& dt) { returnDate = dt; }
        void setOverdueDays(int d) { overdueDays = d; }
        void setLateFeeCalculated(double fee) { lateFeeCalculated = fee; }
        void setDamageFeeCalculated(double fee) { damageFeeCalculated = fee; }
        void setTotalPenalty(double tot) { totalPenalty = tot; }
        void setDepositHeld(double dep) { depositHeld = dep; }
        void setDepositRefund(double ref) { depositRefund = ref; }
        void setExtraPaymentDue(double due) { extraPaymentDue = due; }
        void setInspectorAdminName(const std::string& adm) { inspectorAdminName = adm; }
        void setResolutionNotes(const std::string& n) { resolutionNotes = n; }

        std::string serialize() const {
            std::ostringstream oss;
            oss << id << "|"
                << invoiceNumber << "|"
                << returnDate << "|"
                << overdueDays << "|"
                << std::fixed << std::setprecision(0) << lateFeeCalculated << "|"
                << std::fixed << std::setprecision(0) << damageFeeCalculated << "|"
                << std::fixed << std::setprecision(0) << totalPenalty << "|"
                << std::fixed << std::setprecision(0) << depositHeld << "|"
                << std::fixed << std::setprecision(0) << depositRefund << "|"
                << std::fixed << std::setprecision(0) << extraPaymentDue << "|"
                << inspectorAdminName << "|"
                << resolutionNotes;
            return oss.str();
        }

        static ReturnLog deserialize(const std::string& line) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 12) return ReturnLog();
            ReturnLog l;
            l.id = std::stoi(tokens[0]);
            l.invoiceNumber = tokens[1];
            l.returnDate = tokens[2];
            l.overdueDays = std::stoi(tokens[3]);
            l.lateFeeCalculated = std::stod(tokens[4]);
            l.damageFeeCalculated = std::stod(tokens[5]);
            l.totalPenalty = std::stod(tokens[6]);
            l.depositHeld = std::stod(tokens[7]);
            l.depositRefund = std::stod(tokens[8]);
            l.extraPaymentDue = std::stod(tokens[9]);
            l.inspectorAdminName = tokens[10];
            l.resolutionNotes = tokens[11];
            return l;
        }
    };

    class ReviewRating {
    private:
        int id;
        int gearId;
        int userId;
        std::string username;
        int ratingValue;
        std::string reviewText;
        std::string reviewDate;

    public:
        ReviewRating()
            : id(0), gearId(0), userId(0), username(""), ratingValue(5),
              reviewText(""), reviewDate("2026-11-07") {}

        ReviewRating(int rId, int gId, int uId, const std::string& uName,
                     int rVal, const std::string& text, const std::string& date)
            : id(rId), gearId(gId), userId(uId), username(uName),
              ratingValue(rVal), reviewText(text), reviewDate(date) {}

        int getId() const { return id; }
        int getGearId() const { return gearId; }
        int getUserId() const { return userId; }
        std::string getUsername() const { return username; }
        int getRatingValue() const { return ratingValue; }
        std::string getReviewText() const { return reviewText; }
        std::string getReviewDate() const { return reviewDate; }

        void setId(int nid) { id = nid; }
        void setGearId(int gid) { gearId = gid; }
        void setUserId(int uid) { userId = uid; }
        void setUsername(const std::string& name) { username = name; }
        void setRatingValue(int r) { ratingValue = r; }
        void setReviewText(const std::string& t) { reviewText = t; }
        void setReviewDate(const std::string& d) { reviewDate = d; }

        std::string serialize() const {
            std::ostringstream oss;
            oss << id << "|"
                << gearId << "|"
                << userId << "|"
                << username << "|"
                << ratingValue << "|"
                << reviewText << "|"
                << reviewDate;
            return oss.str();
        }

        static ReviewRating deserialize(const std::string& line) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 7) return ReviewRating();
            ReviewRating r;
            r.id = std::stoi(tokens[0]);
            r.gearId = std::stoi(tokens[1]);
            r.userId = std::stoi(tokens[2]);
            r.username = tokens[3];
            r.ratingValue = std::stoi(tokens[4]);
            r.reviewText = tokens[5];
            r.reviewDate = tokens[6];
            return r;
        }
    };

    struct FinancialKPI {
        double totalGrossRevenue;
        double totalRentalIncome;
        double totalLatePenaltyIncome;
        double totalDamagePenaltyIncome;
        double totalDiscountGiven;
        double totalDepositHeld;
        double totalDepositRefunded;
        double totalNetProfit;
        int totalTransactionsCount;
        int completedRentalsCount;
        int activeRentalsCount;
        int overdueRentalsCount;
        int totalCustomersCount;
        int totalGearsRegistered;
        int totalUnitsInStock;
        int totalUnitsRentedOut;
    };

    // ================================================================================================
    // [BAGIAN 3: PERSISTENSI FILE & REPOSITORI DATA (DATA MANAGER)]
    // ================================================================================================

    class DataManager {
    private:
        std::string usersFilePath;
        std::string gearsFilePath;
        std::string transactionsFilePath;
        std::string returnsFilePath;
        std::string reviewsFilePath;

        std::vector<User> users;
        std::vector<CampingGear> gears;
        std::vector<RentalTransaction> transactions;
        std::vector<ReturnLog> returnLogs;
        std::vector<ReviewRating> reviews;

        void seedInitialUsers();
        void seedInitialGears();
        void seedInitialTransactions();
        void seedInitialReviews();

    public:
        DataManager();
        explicit DataManager(const std::string& dataDirectory);
        ~DataManager();

        void setDataDirectory(const std::string& dataDirectory);

        bool loadAllData();
        bool saveAllData();

        bool loadUsersFromFile();
        bool saveUsersToFile();

        bool loadGearsFromFile();
        bool saveGearsToFile();

        bool loadTransactionsFromFile();
        bool saveTransactionsToFile();

        bool loadReturnLogsFromFile();
        bool saveReturnLogsToFile();

        bool loadReviewsFromFile();
        bool saveReviewsToFile();

        // CRUD USER
        const std::vector<User>& getAllUsers() const { return users; }
        std::vector<User>& getAllUsersRef() { return users; }
        User* findUserById(int id);
        User* findUserByUsername(const std::string& username);
        User* findUserByEmail(const std::string& email);
        bool addUser(const User& user);
        bool updateUser(const User& user);
        bool deleteUser(int id);
        int getNextUserId() const;

        // CRUD CAMPING GEAR
        const std::vector<CampingGear>& getAllGears() const { return gears; }
        std::vector<CampingGear>& getAllGearsRef() { return gears; }
        CampingGear* findGearById(int id);
        CampingGear* findGearBySku(const std::string& sku);
        bool addGear(const CampingGear& gear);
        bool updateGear(const CampingGear& gear);
        bool deleteGear(int id);
        int getNextGearId() const;
        std::string generateNextGearSku(GearCategory category) const;

        // CRUD TRANSAKSI
        const std::vector<RentalTransaction>& getAllTransactions() const { return transactions; }
        std::vector<RentalTransaction>& getAllTransactionsRef() { return transactions; }
        RentalTransaction* findTransactionByInvoice(const std::string& invoice);
        std::vector<RentalTransaction> findTransactionsByUserId(int userId) const;
        std::vector<RentalTransaction> findTransactionsByStatus(TransactionStatus status) const;
        bool addTransaction(const RentalTransaction& trx);
        bool updateTransaction(const RentalTransaction& trx);
        bool deleteTransaction(const std::string& invoice);
        std::string generateNextInvoiceNumber() const;

        // CRUD RETURN LOG
        const std::vector<ReturnLog>& getAllReturnLogs() const { return returnLogs; }
        std::vector<ReturnLog>& getAllReturnLogsRef() { return returnLogs; }
        ReturnLog* findReturnLogByInvoice(const std::string& invoice);
        bool addReturnLog(const ReturnLog& log);
        int getNextReturnLogId() const;

        // CRUD REVIEW
        const std::vector<ReviewRating>& getAllReviews() const { return reviews; }
        std::vector<ReviewRating> getReviewsByGearId(int gearId) const;
        bool addReview(const ReviewRating& review);
        int getNextReviewId() const;

        void resetToDefaultSeed();
    };

    DataManager::DataManager() {
        setDataDirectory("data");
    }

    DataManager::DataManager(const std::string& dataDirectory) {
        setDataDirectory(dataDirectory);
    }

    DataManager::~DataManager() {
        saveAllData();
    }

    void DataManager::setDataDirectory(const std::string& dataDirectory) {
        std::string base = dataDirectory;
        if (!base.empty()) {
            std::error_code ec;
            std::filesystem::create_directories(base, ec);
            if (base.back() != '/' && base.back() != '\\') {
                base += "/";
            }
        }
        usersFilePath = base + "users.txt";
        gearsFilePath = base + "gears.txt";
        transactionsFilePath = base + "transactions.txt";
        returnsFilePath = base + "returns.txt";
        reviewsFilePath = base + "reviews.txt";
    }

    bool DataManager::loadAllData() {
        loadUsersFromFile();
        loadGearsFromFile();
        loadTransactionsFromFile();
        loadReturnLogsFromFile();
        loadReviewsFromFile();

        if (users.empty()) {
            seedInitialUsers();
            saveUsersToFile();
        }
        if (gears.empty()) {
            seedInitialGears();
            saveGearsToFile();
        }
        if (transactions.empty()) {
            seedInitialTransactions();
            saveTransactionsToFile();
        }
        if (reviews.empty()) {
            seedInitialReviews();
            saveReviewsToFile();
        }

        return true;
    }

    bool DataManager::saveAllData() {
        bool u = saveUsersToFile();
        bool g = saveGearsToFile();
        bool t = saveTransactionsToFile();
        bool r = saveReturnLogsToFile();
        bool rev = saveReviewsToFile();
        return (u && g && t && r && rev);
    }

    bool DataManager::loadUsersFromFile() {
        users.clear();
        std::ifstream file(usersFilePath);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            User u = User::deserialize(line);
            if (u.getId() > 0) users.push_back(u);
        }
        file.close();
        return true;
    }

    bool DataManager::saveUsersToFile() {
        std::ofstream file(usersFilePath);
        if (!file.is_open()) return false;
        file << "# Database Pengguna Sistem Penyewaan Alat Camping\n";
        file << "# Format: ID|Username|PasswordHash|FullName|Phone|Email|IDCard|Address|Role|Status|RentalsCount|TrustScore|RegDate\n";
        for (const auto& u : users) {
            file << u.serialize() << "\n";
        }
        file.close();
        return true;
    }

    bool DataManager::loadGearsFromFile() {
        gears.clear();
        std::ifstream file(gearsFilePath);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            CampingGear g = CampingGear::deserialize(line);
            if (g.getId() > 0) gears.push_back(g);
        }
        file.close();
        return true;
    }

    bool DataManager::saveGearsToFile() {
        std::ofstream file(gearsFilePath);
        if (!file.is_open()) return false;
        file << "# Database Inventaris Alat Camping\n";
        file << "# Format: ID|SKU|Name|Brand|Category|PricePerDay|ReplacementValue|StockTotal|StockAvail|TimesRented|Rating|ReviewCount|Spec|Notes\n";
        for (const auto& g : gears) {
            file << g.serialize() << "\n";
        }
        file.close();
        return true;
    }

    bool DataManager::loadTransactionsFromFile() {
        transactions.clear();
        std::ifstream file(transactionsFilePath);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            RentalTransaction tx = RentalTransaction::deserialize(line);
            if (!tx.getInvoiceNumber().empty()) transactions.push_back(tx);
        }
        file.close();
        return true;
    }

    bool DataManager::saveTransactionsToFile() {
        std::ofstream file(transactionsFilePath);
        if (!file.is_open()) return false;
        file << "# Database Transaksi Penyewaan Alat Camping\n";
        file << "# Format: Invoice|UserId|CustName|CustPhone|IdCard|BookDate|StartDate|ExpReturnDate|ActReturnDate|Subtotal|Disc|Promo|Deposit|Total|PayMethod|PayStat|TrxStat|ProofRef|LateFee|DamageFee|DepositRefund|Notes|ItemsList\n";
        for (const auto& tx : transactions) {
            file << tx.serialize() << "\n";
        }
        file.close();
        return true;
    }

    bool DataManager::loadReturnLogsFromFile() {
        returnLogs.clear();
        std::ifstream file(returnsFilePath);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            ReturnLog rl = ReturnLog::deserialize(line);
            if (rl.getId() > 0) returnLogs.push_back(rl);
        }
        file.close();
        return true;
    }

    bool DataManager::saveReturnLogsToFile() {
        std::ofstream file(returnsFilePath);
        if (!file.is_open()) return false;
        file << "# Database Log Pengembalian dan Denda\n";
        file << "# Format: ID|Invoice|ReturnDate|OverdueDays|LateFee|DamageFee|TotalPenalty|DepositHeld|DepositRefund|ExtraPaymentDue|Admin|Notes\n";
        for (const auto& rl : returnLogs) {
            file << rl.serialize() << "\n";
        }
        file.close();
        return true;
    }

    bool DataManager::loadReviewsFromFile() {
        reviews.clear();
        std::ifstream file(reviewsFilePath);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            ReviewRating r = ReviewRating::deserialize(line);
            if (r.getId() > 0) reviews.push_back(r);
        }
        file.close();
        return true;
    }

    bool DataManager::saveReviewsToFile() {
        std::ofstream file(reviewsFilePath);
        if (!file.is_open()) return false;
        file << "# Database Ulasan dan Rating Produk\n";
        file << "# Format: ID|GearId|UserId|Username|Rating|ReviewText|ReviewDate\n";
        for (const auto& r : reviews) {
            file << r.serialize() << "\n";
        }
        file.close();
        return true;
    }

    User* DataManager::findUserById(int id) {
        for (auto& u : users) {
            if (u.getId() == id) return &u;
        }
        return nullptr;
    }

    User* DataManager::findUserByUsername(const std::string& username) {
        for (auto& u : users) {
            if (u.getUsername() == username) return &u;
        }
        return nullptr;
    }

    User* DataManager::findUserByEmail(const std::string& email) {
        for (auto& u : users) {
            if (u.getEmail() == email) return &u;
        }
        return nullptr;
    }

    bool DataManager::addUser(const User& user) {
        if (findUserByUsername(user.getUsername()) != nullptr) return false;
        users.push_back(user);
        saveUsersToFile();
        return true;
    }

    bool DataManager::updateUser(const User& user) {
        for (auto& u : users) {
            if (u.getId() == user.getId()) {
                u = user;
                saveUsersToFile();
                return true;
            }
        }
        return false;
    }

    bool DataManager::deleteUser(int id) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->getId() == id) {
                users.erase(it);
                saveUsersToFile();
                return true;
            }
        }
        return false;
    }

    int DataManager::getNextUserId() const {
        int maxId = 0;
        for (const auto& u : users) {
            if (u.getId() > maxId) maxId = u.getId();
        }
        return maxId + 1;
    }

    CampingGear* DataManager::findGearById(int id) {
        for (auto& g : gears) {
            if (g.getId() == id) return &g;
        }
        return nullptr;
    }

    CampingGear* DataManager::findGearBySku(const std::string& sku) {
        for (auto& g : gears) {
            if (g.getSku() == sku) return &g;
        }
        return nullptr;
    }

    bool DataManager::addGear(const CampingGear& gear) {
        if (findGearBySku(gear.getSku()) != nullptr) return false;
        gears.push_back(gear);
        saveGearsToFile();
        return true;
    }

    bool DataManager::updateGear(const CampingGear& gear) {
        for (auto& g : gears) {
            if (g.getId() == gear.getId()) {
                g = gear;
                saveGearsToFile();
                return true;
            }
        }
        return false;
    }

    bool DataManager::deleteGear(int id) {
        for (auto it = gears.begin(); it != gears.end(); ++it) {
            if (it->getId() == id) {
                gears.erase(it);
                saveGearsToFile();
                return true;
            }
        }
        return false;
    }

    int DataManager::getNextGearId() const {
        int maxId = 0;
        for (const auto& g : gears) {
            if (g.getId() > maxId) maxId = g.getId();
        }
        return maxId + 1;
    }

    std::string DataManager::generateNextGearSku(GearCategory category) const {
        std::string prefix;
        switch (category) {
            case GearCategory::TENDA:             prefix = "TND"; break;
            case GearCategory::SLEEPING_GEAR:     prefix = "SLP"; break;
            case GearCategory::CARRIER_BAG:       prefix = "BAG"; break;
            case GearCategory::COOKING_EQUIPMENT: prefix = "CKG"; break;
            case GearCategory::LIGHTING:          prefix = "LGT"; break;
            case GearCategory::NAVIGATION_SAFETY: prefix = "SFT"; break;
            case GearCategory::OUTDOOR_COMFORT:   prefix = "CFT"; break;
            default:                              prefix = "OTH"; break;
        }
        int count = 1;
        for (const auto& g : gears) {
            if (g.getCategory() == category) count++;
        }
        std::ostringstream oss;
        oss << prefix << "-" << std::setw(3) << std::setfill('0') << count;
        return oss.str();
    }

    RentalTransaction* DataManager::findTransactionByInvoice(const std::string& invoice) {
        for (auto& tx : transactions) {
            if (tx.getInvoiceNumber() == invoice) return &tx;
        }
        return nullptr;
    }

    std::vector<RentalTransaction> DataManager::findTransactionsByUserId(int userId) const {
        std::vector<RentalTransaction> result;
        for (const auto& tx : transactions) {
            if (tx.getUserId() == userId) result.push_back(tx);
        }
        return result;
    }

    std::vector<RentalTransaction> DataManager::findTransactionsByStatus(TransactionStatus status) const {
        std::vector<RentalTransaction> result;
        for (const auto& tx : transactions) {
            if (tx.getTransactionStatus() == status) result.push_back(tx);
        }
        return result;
    }

    bool DataManager::addTransaction(const RentalTransaction& tx) {
        if (findTransactionByInvoice(tx.getInvoiceNumber()) != nullptr) return false;
        transactions.push_back(tx);
        saveTransactionsToFile();
        return true;
    }

    bool DataManager::updateTransaction(const RentalTransaction& tx) {
        for (auto& t : transactions) {
            if (t.getInvoiceNumber() == tx.getInvoiceNumber()) {
                t = tx;
                saveTransactionsToFile();
                return true;
            }
        }
        return false;
    }

    bool DataManager::deleteTransaction(const std::string& invoice) {
        for (auto it = transactions.begin(); it != transactions.end(); ++it) {
            if (it->getInvoiceNumber() == invoice) {
                transactions.erase(it);
                saveTransactionsToFile();
                return true;
            }
        }
        return false;
    }

    std::string DataManager::generateNextInvoiceNumber() const {
        int nextNum = static_cast<int>(transactions.size()) + 1001;
        std::ostringstream oss;
        oss << "INV-202611-" << nextNum;
        return oss.str();
    }

    ReturnLog* DataManager::findReturnLogByInvoice(const std::string& invoice) {
        for (auto& rl : returnLogs) {
            if (rl.getInvoiceNumber() == invoice) return &rl;
        }
        return nullptr;
    }

    bool DataManager::addReturnLog(const ReturnLog& log) {
        returnLogs.push_back(log);
        saveReturnLogsToFile();
        return true;
    }

    int DataManager::getNextReturnLogId() const {
        int maxId = 0;
        for (const auto& rl : returnLogs) {
            if (rl.getId() > maxId) maxId = rl.getId();
        }
        return maxId + 1;
    }

    std::vector<ReviewRating> DataManager::getReviewsByGearId(int gearId) const {
        std::vector<ReviewRating> result;
        for (const auto& r : reviews) {
            if (r.getGearId() == gearId) result.push_back(r);
        }
        return result;
    }

    bool DataManager::addReview(const ReviewRating& review) {
        reviews.push_back(review);
        saveReviewsToFile();
        return true;
    }

    int DataManager::getNextReviewId() const {
        int maxId = 0;
        for (const auto& r : reviews) {
            if (r.getId() > maxId) maxId = r.getId();
        }
        return maxId + 1;
    }

    void DataManager::seedInitialUsers() {
        users.clear();
        users.push_back(User(1, "admin", "admin123", "Administrator Utama", "081122334455",
                             "admin@campingrental.id", "3271010101900001", "Jl. Rinjani No. 10, Bandung",
                             UserRole::ADMIN, UserStatus::ACTIVE, 0, 100.0, "2026-01-01"));

        users.push_back(User(2, "budi", "budi123", "Budi Santoso", "081234567890",
                             "budi@gmail.com", "3271011505950002", "Jl. Dago Asri No. 45, Bandung",
                             UserRole::USER, UserStatus::ACTIVE, 3, 98.5, "2026-02-15"));

        users.push_back(User(3, "siti", "siti123", "Siti Aminah", "081398765432",
                             "siti.outdoor@yahoo.com", "3271014208980003", "Jl. Gejayan No. 12, Sleman",
                             UserRole::USER, UserStatus::ACTIVE, 5, 100.0, "2026-03-10"));

        users.push_back(User(4, "andi", "andi123", "Andi Wijaya", "081765432109",
                             "andi.wijaya@outlook.com", "3271012304990004", "Jl. Dipati Ukur No. 88, Bandung",
                             UserRole::USER, UserStatus::ACTIVE, 1, 95.0, "2026-05-20"));
    }

    void DataManager::seedInitialGears() {
        gears.clear();
        // Tenda & Shelter
        gears.push_back(CampingGear(1, "TND-001", "Tenda Eiger Guardian 4P", "Eiger",
            GearCategory::TENDA, 50000, 1250000, 8, 6, 24, 4.9, 18,
            "Kapasitas 4 orang, double layer waterproof 3000mm, alloy pole", "Kondisi sangat mulus, pasak lengkap"));

        gears.push_back(CampingGear(2, "TND-002", "Tenda Consina Magnum 4", "Consina",
            GearCategory::TENDA, 45000, 950000, 10, 8, 30, 4.7, 22,
            "Kapasitas 4-5 orang, teras depan luas, bahan polyester PU 2500mm", "Bersih, jahitan rapi"));

        gears.push_back(CampingGear(3, "TND-003", "Tenda Naturehike Cloud Up 2 Ultralight", "Naturehike",
            GearCategory::TENDA, 40000, 1100000, 6, 5, 15, 4.8, 12,
            "Kapasitas 2 orang ultralight 1.7kg, 20D silicone coated 4000mm", "Sangat prima, frame utuh"));

        gears.push_back(CampingGear(4, "TND-004", "Quechua Arpenaz Family 4.1 Fresh & Black", "Quechua",
            GearCategory::TENDA, 75000, 2100000, 4, 3, 10, 5.0, 9,
            "Kapasitas 4 orang 1 kamar + ruang santai, teknologi adem gelap", "Terawat, minus sablon pudar sedikit"));

        // Sleeping Gear
        gears.push_back(CampingGear(5, "SLP-001", "Sleeping Bag Arei Everest Polar", "Arei",
            GearCategory::SLEEPING_GEAR, 15000, 220000, 15, 12, 45, 4.6, 28,
            "Lapisan dalam polar bulu hangat, limit suhu 5 derajat Celcius", "Harum laundry, ritsleting lancar"));

        gears.push_back(CampingGear(6, "SLP-002", "Sleeping Bag Deuter Orbit +5 Mummy", "Deuter",
            GearCategory::SLEEPING_GEAR, 25000, 750000, 8, 7, 20, 4.9, 14,
            "Model kepompong insulasi thermal sintetis high grade, berat 1kg", "Kondisi prima seperti baru"));

        gears.push_back(CampingGear(7, "SLP-003", "Matras Alumunium Foil Double Side", "Wildshell",
            GearCategory::SLEEPING_GEAR, 7500, 65000, 20, 18, 55, 4.5, 30,
            "Reflektor panas tubuh, busa empuk 4mm, waterproof", "Bagus, lipatan rapi"));

        // Carrier & Tas
        gears.push_back(CampingGear(8, "BAG-001", "Carrier Osprey Atmos AG 65L", "Osprey",
            GearCategory::CARRIER_BAG, 60000, 3400000, 5, 4, 18, 5.0, 16,
            "Anti-Gravity suspension backsystem, include raincover asli", "Sangat mulus, busa punggung tebal"));

        gears.push_back(CampingGear(9, "BAG-002", "Carrier Consina Tarebbi 60L", "Consina",
            GearCategory::CARRIER_BAG, 30000, 650000, 12, 9, 40, 4.6, 25,
            "Bahan cordura tahan banting, kompartemen utama luas", "Kuat, siap jelajah hutan"));

        gears.push_back(CampingGear(10, "BAG-003", "Carrier Eiger Rhinos 60L", "Eiger",
            GearCategory::CARRIER_BAG, 35000, 950000, 10, 8, 35, 4.8, 20,
            "Sistem sirkulasi udara tropic flow, buckle acetal tangguh", "Terawat baik, warna cerah"));

        // Alat Masak & Kompor
        gears.push_back(CampingGear(11, "CKG-001", "Kompor Ultralight Windproof Kovar", "Kovar",
            GearCategory::COOKING_EQUIPMENT, 12000, 130000, 15, 13, 50, 4.7, 32,
            "Pelindung angin model bunga mekar, pemantik piezo elektrik", "Api biru stabil, bersih"));

        gears.push_back(CampingGear(12, "CKG-002", "Nesting Cooking Set DS-308 (4-5 Org)", "Alocs",
            GearCategory::COOKING_EQUIPMENT, 20000, 260000, 10, 8, 38, 4.8, 26,
            "3 panci aluminium anodized + 1 teko 0.8L + mangkok & spons", "Lengkap dengan tas jaring, non-stick"));

        gears.push_back(CampingGear(13, "CKG-003", "Gas Kaleng Portable Hi-Cook 230g", "Hi-Cook",
            GearCategory::COOKING_EQUIPMENT, 15000, 25000, 30, 26, 80, 4.9, 45,
            "Gas butana murni bersegel pabrik untuk kompor portable", "Segel baru 100%"));

        // Penerangan & Lentera
        gears.push_back(CampingGear(14, "LGT-001", "Headlamp Petzl Tikka 350 Lumens", "Petzl",
            GearCategory::LIGHTING, 20000, 450000, 10, 9, 28, 4.9, 19,
            "3 level cahaya putih + mode merah malam, waterproof IPX4", "Baterai awet, strap elastis"));

        gears.push_back(CampingGear(15, "LGT-002", "Lentera Tenda LED Rechargeable Solar", "Naturehike",
            GearCategory::LIGHTING, 15000, 180000, 12, 10, 33, 4.7, 21,
            "Bisa dicharge USB Type-C & matahari, fungsi powerbank darurat", "Terang benderang, baterai sehat"));

        // Navigasi & Safety
        gears.push_back(CampingGear(16, "SFT-001", "Trekking Pole Duralumin Naturehike", "Naturehike",
            GearCategory::NAVIGATION_SAFETY, 10000, 150000, 16, 13, 42, 4.8, 24,
            "Material duralumin 7075 kuat & ringan, pengunci quick lock", "Peredam getar normal, ujung karbida"));

        gears.push_back(CampingGear(17, "SFT-002", "HT Walkie Talkie Baofeng UV-5R Dual Band", "Baofeng",
            GearCategory::NAVIGATION_SAFETY, 25000, 350000, 8, 6, 22, 4.8, 15,
            "Jangkauan 3-5 km di alam terbuka, include headset & charger", "Frekuensi jernih, baterai tahan lama"));

        gears.push_back(CampingGear(18, "SFT-003", "Kotak P3K Outdoor First Aid Kit Lengkap", "OutdoorMedic",
            GearCategory::NAVIGATION_SAFETY, 10000, 120000, 12, 11, 25, 4.9, 17,
            "Perban elastis, plester, povidone, alkohol pads, gunting, mitela", "Steril, isi obat kadaluarsa 2028"));

        // Outdoor Comfort
        gears.push_back(CampingGear(19, "CFT-001", "Hammock Double Layer Ticket to the Moon", "TTTM",
            GearCategory::OUTDOOR_COMFORT, 20000, 320000, 10, 8, 36, 4.9, 27,
            "Kain sutra parasut elastis tahan beban 200kg, include webbing", "Bahan lembut dan bersih"));

        gears.push_back(CampingGear(20, "CFT-002", "Kursi Lipat Duralumin Ultralight", "Dhaulagiri",
            GearCategory::OUTDOOR_COMFORT, 15000, 195000, 14, 11, 40, 4.7, 28,
            "Kapasitas beban 120kg, rangka aluminium alloy lipat ringkas", "Kokoh, tidak goyang"));

        gears.push_back(CampingGear(21, "CFT-003", "Meja Lipat Aluminium Portable Camping", "Speed",
            GearCategory::OUTDOOR_COMFORT, 20000, 240000, 8, 6, 26, 4.8, 18,
            "Meja gulung aluminium ukuran 56x40 cm, anti karat & panas", "Stabil, engsel berfungsi sempurna"));

        gears.push_back(CampingGear(22, "TND-005", "Tenda Glamping Piramida Indian Teepee 6P", "Naturehike",
            GearCategory::TENDA, 95000, 3200000, 4, 3, 8, 5.0, 7,
            "Kapasitas 6 orang estetik, tiang tengah alloy kokoh, tahan angin kencang", "Sangat mulus & bersih"));

        gears.push_back(CampingGear(23, "SLP-004", "Matras Angin Inflatable Ultralight Naturehike", "Naturehike",
            GearCategory::SLEEPING_GEAR, 15000, 240000, 12, 10, 31, 4.7, 19,
            "Busa seluler tebal 6.5cm anti rembes embun es, berat 550 gram", "Klep angin kedap, tidak bocor"));

        gears.push_back(CampingGear(24, "BAG-004", "Daypack Summit Ultralight 25L Deuter", "Deuter",
            GearCategory::CARRIER_BAG, 20000, 420000, 10, 9, 27, 4.8, 15,
            "Ransel serbu puncak anti air, kompartemen water bladder terintegrasi", "Kondisi prima, tali kokoh"));

        gears.push_back(CampingGear(25, "CKG-004", "Teko Kopi Camping Stainless Steel 1.2L", "Fire-Maple",
            GearCategory::COOKING_EQUIPMENT, 10000, 140000, 12, 11, 44, 4.9, 29,
            "Ceret pemanas air praktis dengan handle silikon anti panas", "Bersih mengkilap, food grade"));

        gears.push_back(CampingGear(26, "LGT-003", "Senter Sorot LED Camping 1000 Lumens", "Nitecore",
            GearCategory::LIGHTING, 25000, 380000, 8, 7, 19, 4.9, 13,
            "Jarak sorot 200 meter, USB charging, tahan jatuh 1.5 meter", "Kaca lensa bening, baterai awet"));

        gears.push_back(CampingGear(27, "SFT-004", "Kompas Bidik Logam Akurat Suunto KB-14", "Suunto",
            GearCategory::NAVIGATION_SAFETY, 12000, 260000, 10, 10, 22, 5.0, 14,
            "Akurasi militer redaman cairan, skala prisma pembaca azimuth", "Kalibrasi akurat, jarum stabil"));

        gears.push_back(CampingGear(28, "SFT-005", "Solar Panel Charger Portable 28W Outdoor", "BigBlue",
            GearCategory::NAVIGATION_SAFETY, 30000, 480000, 6, 5, 16, 4.8, 11,
            "3 Port USB auto-detect output, tahan percikan air IPX4", "Panel sel bersih, konversi daya prima"));

        gears.push_back(CampingGear(29, "CFT-004", "Flysheet Tarp Ultralight Waterproof 4x6m", "Arei",
            GearCategory::OUTDOOR_COMFORT, 18000, 220000, 12, 11, 35, 4.7, 20,
            "19 Loop webbing pengikat kuat, lapisan silver coating anti UV", "Jahitan tertutup seam tape rapi"));

        gears.push_back(CampingGear(30, "CFT-005", "Lampu Hias Tenda String Warm White Solar", "Naturehike",
            GearCategory::OUTDOOR_COMFORT, 10000, 95000, 15, 13, 28, 4.8, 18,
            "Panjang 10 meter 80 LED, estetika tenda malam hari, baterai solar", "Kabel lentur dan semua LED menyala"));
    }

    void DataManager::seedInitialTransactions() {
        transactions.clear();

        RentalTransaction tx1;
        tx1.setInvoiceNumber("INV-202611-1001");
        tx1.setUserId(2);
        tx1.setCustomerName("Budi Santoso");
        tx1.setCustomerPhone("081234567890");
        tx1.setCustomerIdCard("3271011505950002");
        tx1.setBookingDate("2026-11-01");
        tx1.setStartDate("2026-11-02");
        tx1.setExpectedReturnDate("2026-11-04");
        tx1.setActualReturnDate("2026-11-04");
        tx1.addItem(RentalItem(1, "TND-001", "Tenda Eiger Guardian 4P", 50000, 1250000, 1, 2));
        tx1.addItem(RentalItem(5, "SLP-001", "Sleeping Bag Arei Everest Polar", 15000, 220000, 2, 2));
        tx1.setSubtotal(160000);
        tx1.setDiscountAmount(16000);
        tx1.setPromoCodeUsed("CAMPINGASIK10");
        tx1.setDepositAmount(50000);
        tx1.setTotalAmount(194000);
        tx1.setPaymentMethod(PaymentMethod::TRANSFER_BANK_BCA);
        tx1.setPaymentStatus(PaymentStatus::VERIFIED_PAID);
        tx1.setTransactionStatus(TransactionStatus::RETURNED_COMPLETED);
        tx1.setPaymentProofRef("TRX-BCA-98124412");
        tx1.setLateFee(0);
        tx1.setDamagePenaltyFee(0);
        tx1.setDepositRefunded(50000);
        tx1.setNotes("Penyewaan gunung Papandayan lancar, barang kembali bersih.");
        transactions.push_back(tx1);

        RentalTransaction tx2;
        tx2.setInvoiceNumber("INV-202611-1002");
        tx2.setUserId(3);
        tx2.setCustomerName("Siti Aminah");
        tx2.setCustomerPhone("081398765432");
        tx2.setCustomerIdCard("3271014208980003");
        tx2.setBookingDate("2026-11-05");
        tx2.setStartDate("2026-11-06");
        tx2.setExpectedReturnDate("2026-11-09");
        tx2.setActualReturnDate("-");
        tx2.addItem(RentalItem(2, "TND-002", "Tenda Consina Magnum 4", 45000, 950000, 1, 3));
        tx2.addItem(RentalItem(8, "BAG-001", "Carrier Osprey Atmos AG 65L", 60000, 3400000, 1, 3));
        tx2.addItem(RentalItem(11, "CKG-001", "Kompor Ultralight Windproof Kovar", 12000, 130000, 1, 3));
        tx2.setSubtotal(351000);
        tx2.setDiscountAmount(50000);
        tx2.setPromoCodeUsed("WEEKENDSERU20");
        tx2.setDepositAmount(100000);
        tx2.setTotalAmount(401000);
        tx2.setPaymentMethod(PaymentMethod::EWALLET_QRIS);
        tx2.setPaymentStatus(PaymentStatus::VERIFIED_PAID);
        tx2.setTransactionStatus(TransactionStatus::ACTIVE_RENTED);
        tx2.setPaymentProofRef("QRIS-NMD-87623199");
        tx2.setNotes("Penyewaan Gunung Prau 3 hari.");
        transactions.push_back(tx2);

        RentalTransaction tx3;
        tx3.setInvoiceNumber("INV-202611-1003");
        tx3.setUserId(4);
        tx3.setCustomerName("Andi Wijaya");
        tx3.setCustomerPhone("081765432109");
        tx3.setCustomerIdCard("3271012304990004");
        tx3.setBookingDate("2026-11-07");
        tx3.setStartDate("2026-11-08");
        tx3.setExpectedReturnDate("2026-11-10");
        tx3.setActualReturnDate("-");
        tx3.addItem(RentalItem(3, "TND-003", "Tenda Naturehike Cloud Up 2 Ultralight", 40000, 1100000, 1, 2));
        tx3.addItem(RentalItem(16, "SFT-001", "Trekking Pole Duralumin Naturehike", 10000, 150000, 2, 2));
        tx3.setSubtotal(120000);
        tx3.setDiscountAmount(0);
        tx3.setPromoCodeUsed("-");
        tx3.setDepositAmount(50000);
        tx3.setTotalAmount(170000);
        tx3.setPaymentMethod(PaymentMethod::TRANSFER_BANK_MANDIRI);
        tx3.setPaymentStatus(PaymentStatus::UNPAID);
        tx3.setTransactionStatus(TransactionStatus::PENDING_PAYMENT);
        tx3.setPaymentProofRef("");
        tx3.setNotes("Trip bukit Pangalengan.");
        transactions.push_back(tx3);

        // Transaksi 4: Selesai dengan Denda Kerusakan (Budi Santoso)
        RentalTransaction tx4;
        tx4.setInvoiceNumber("INV-202611-1004");
        tx4.setUserId(2);
        tx4.setCustomerName("Budi Santoso");
        tx4.setCustomerPhone("081234567890");
        tx4.setCustomerIdCard("3271011505950002");
        tx4.setBookingDate("2026-10-20");
        tx4.setStartDate("2026-10-21");
        tx4.setExpectedReturnDate("2026-10-23");
        tx4.setActualReturnDate("2026-10-24");
        tx4.addItem(RentalItem(9, "BAG-002", "Carrier Consina Tarebbi 60L", 30000, 650000, 1, 2));
        tx4.addItem(RentalItem(14, "LGT-001", "Headlamp Petzl Tikka 350 Lumens", 20000, 450000, 1, 2));
        tx4.setSubtotal(100000);
        tx4.setDiscountAmount(10000);
        tx4.setPromoCodeUsed("CAMPINGASIK10");
        tx4.setDepositAmount(50000);
        tx4.setTotalAmount(140000);
        tx4.setPaymentMethod(PaymentMethod::EWALLET_GOPAY);
        tx4.setPaymentStatus(PaymentStatus::VERIFIED_PAID);
        tx4.setTransactionStatus(TransactionStatus::RETURNED_WITH_ISSUE);
        tx4.setPaymentProofRef("GOPAY-99218821");
        tx4.setLateFee(45000); // 1 hari telat
        tx4.setDamagePenaltyFee(0);
        tx4.setDepositRefunded(5000); // Sisa deposit 50k - 45k denda = 5k
        tx4.setNotes("Telat 1 hari karena macet jalur puncak.");
        transactions.push_back(tx4);

        // Transaksi 5: Dibatalkan (Andi Wijaya)
        RentalTransaction tx5;
        tx5.setInvoiceNumber("INV-202611-1005");
        tx5.setUserId(4);
        tx5.setCustomerName("Andi Wijaya");
        tx5.setCustomerPhone("081765432109");
        tx5.setCustomerIdCard("3271012304990004");
        tx5.setBookingDate("2026-10-15");
        tx5.setStartDate("2026-10-16");
        tx5.setExpectedReturnDate("2026-10-18");
        tx5.setActualReturnDate("-");
        tx5.addItem(RentalItem(20, "CFT-002", "Kursi Lipat Duralumin Ultralight", 15000, 195000, 2, 2));
        tx5.setSubtotal(60000);
        tx5.setDiscountAmount(0);
        tx5.setPromoCodeUsed("-");
        tx5.setDepositAmount(50000);
        tx5.setTotalAmount(110000);
        tx5.setPaymentMethod(PaymentMethod::CASH_ON_PICKUP);
        tx5.setPaymentStatus(PaymentStatus::FAILED_EXPIRED);
        tx5.setTransactionStatus(TransactionStatus::CANCELLED);
        tx5.setPaymentProofRef("");
        tx5.setNotes("Dibatalkan: Cuaca buruk badai hujan lebat.");
        transactions.push_back(tx5);
    }

    void DataManager::seedInitialReviews() {
        reviews.clear();
        reviews.push_back(ReviewRating(1, 1, 2, "budi", 5,
            "Tenda Eiger Guardian mantap banget, waktu badai di gunung gak bocor sama sekali!", "2026-11-04"));
        reviews.push_back(ReviewRating(2, 5, 2, "budi", 5,
            "Sleeping bag bersih dan wangi banget. Bikin tidur nyenyak di suhu dingin.", "2026-11-04"));
        reviews.push_back(ReviewRating(3, 8, 3, "siti", 5,
            "Osprey Atmos benar-benar anti gravitasi, beban 15kg serasa enteng di punggung.", "2026-10-25"));
    }

    void DataManager::resetToDefaultSeed() {
        seedInitialUsers();
        seedInitialGears();
        seedInitialTransactions();
        seedInitialReviews();
        returnLogs.clear();
        saveAllData();
    }

    // ================================================================================================
    // [BAGIAN 4: ALGORITMA SORTING & SEARCHING (ANGGOTA 2 - BUSINESS LOGIC)]
    // ================================================================================================
    /**
     * Class AlgorithmEngine memuat implementasi murni algoritma-algoritma fundamental
     * ilmu komputer yang digunakan untuk memproses data inventaris dan transaksi:
     * 
     * 1. MERGESORT (Divide and Conquer):
     *    - Kompleksitas Waktu: O(N log N) pada Best, Average, dan Worst Case.
     *    - Kompleksitas Ruang: O(N) untuk array pembantu sementara.
     *    - Stabilitas       : Stabil (mempertahankan urutan relatif elemen ekuivalen).
     *    - Penggunaan       : Pengurutan katalog alat camping multi-kriteria (Harga, Nama, Rating, Stok).
     * 
     * 2. QUICKSORT (Partition Exchange):
     *    - Kompleksitas Waktu: O(N log N) average case, O(N^2) worst case.
     *    - Kompleksitas Ruang: O(log N) stack rekursif.
     *    - Penggunaan       : Pengurutan data transaksi penyewaan (Tanggal sewa, Nilai omzet, No Invoice).
     * 
     * 3. BUBBLESORT (Comparison Sort):
     *    - Kompleksitas Waktu: O(N^2) worst case, O(N) best case saat sudah terurut.
     *    - Kompleksitas Ruang: O(1) in-place.
     *    - Penggunaan       : Pengurutan ulasan pelanggan dari tanggal terbaru.
     * 
     * 4. BINARY SEARCH (Dichotomic Search):
     *    - Kompleksitas Waktu: O(log N) pada koleksi terurut.
     *    - Kompleksitas Ruang: O(1) iteratif.
     *    - Penggunaan       : Pencarian presisi instan berdasarkan kode SKU unik barang.
     * 
     * 5. LINEAR SEARCH (Sequential Search):
     *    - Kompleksitas Waktu: O(N) pada koleksi tak terstruktur.
     *    - Penggunaan       : Pencarian multi-kategori, rentang harga, dan substring nama/spesifikasi.
     */
    class AlgorithmEngine {
    public:
        // Algoritma Insertion Sort untuk item-item kecil di dalam keranjang belanja
        static void insertionSortRentalItems(std::vector<RentalItem>& items) {
            int n = static_cast<int>(items.size());
            for (int i = 1; i < n; ++i) {
                RentalItem key = items[i];
                int j = i - 1;
                while (j >= 0 && items[j].calculateSubtotal() < key.calculateSubtotal()) {
                    items[j + 1] = items[j];
                    j = j - 1;
                }
                items[j + 1] = key;
            }
        }

        static void merge(std::vector<CampingGear>& arr, int left, int mid, int right, GearSortField sortField) {
            int n1 = mid - left + 1;
            int n2 = right - mid;

            std::vector<CampingGear> L(n1);
            std::vector<CampingGear> R(n2);

            for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
            for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

            auto comparator = [sortField](const CampingGear& a, const CampingGear& b) -> bool {
                switch (sortField) {
                    case GearSortField::NAME_ASC:
                        return a.getName() <= b.getName();
                    case GearSortField::NAME_DESC:
                        return a.getName() >= b.getName();
                    case GearSortField::PRICE_ASC:
                        return a.getRentalPricePerDay() <= b.getRentalPricePerDay();
                    case GearSortField::PRICE_DESC:
                        return a.getRentalPricePerDay() >= b.getRentalPricePerDay();
                    case GearSortField::RATING_DESC:
                        return a.getRating() >= b.getRating();
                    case GearSortField::POPULARITY_DESC:
                        return a.getTimesRented() >= b.getTimesRented();
                    case GearSortField::AVAILABLE_STOCK_DESC:
                        return a.getStockAvailable() >= b.getStockAvailable();
                    default:
                        return a.getName() <= b.getName();
                }
            };

            int i = 0, j = 0, k = left;
            while (i < n1 && j < n2) {
                if (comparator(L[i], R[j])) {
                    arr[k] = L[i];
                    i++;
                } else {
                    arr[k] = R[j];
                    j++;
                }
                k++;
            }

            while (i < n1) {
                arr[k] = L[i];
                i++;
                k++;
            }
            while (j < n2) {
                arr[k] = R[j];
                j++;
                k++;
            }
        }

        static void mergeSortGears(std::vector<CampingGear>& arr, int left, int right, GearSortField sortField) {
            if (left < right) {
                int mid = left + (right - left) / 2;
                mergeSortGears(arr, left, mid, sortField);
                mergeSortGears(arr, mid + 1, right, sortField);
                merge(arr, left, mid, right, sortField);
            }
        }

        static int partition(std::vector<RentalTransaction>& arr, int low, int high, TransactionSortField sortField) {
            RentalTransaction pivot = arr[high];

            auto comparator = [sortField](const RentalTransaction& a, const RentalTransaction& b) -> bool {
                switch (sortField) {
                    case TransactionSortField::INVOICE_ASC:
                        return a.getInvoiceNumber() <= b.getInvoiceNumber();
                    case TransactionSortField::INVOICE_DESC:
                        return a.getInvoiceNumber() >= b.getInvoiceNumber();
                    case TransactionSortField::DATE_ASC:
                        return a.getBookingDate() <= b.getBookingDate();
                    case TransactionSortField::DATE_DESC:
                        return a.getBookingDate() >= b.getBookingDate();
                    case TransactionSortField::TOTAL_ASC:
                        return a.getTotalAmount() <= b.getTotalAmount();
                    case TransactionSortField::TOTAL_DESC:
                        return a.getTotalAmount() >= b.getTotalAmount();
                    default:
                        return a.getBookingDate() >= b.getBookingDate();
                }
            };

            int i = low - 1;
            for (int j = low; j < high; ++j) {
                if (comparator(arr[j], pivot)) {
                    i++;
                    std::swap(arr[i], arr[j]);
                }
            }
            std::swap(arr[i + 1], arr[high]);
            return i + 1;
        }

        static void quickSortTransactions(std::vector<RentalTransaction>& arr, int low, int high, TransactionSortField sortField) {
            if (low < high) {
                int pi = partition(arr, low, high, sortField);
                quickSortTransactions(arr, low, pi - 1, sortField);
                quickSortTransactions(arr, pi + 1, high, sortField);
            }
        }

        static void bubbleSortReviews(std::vector<ReviewRating>& arr, bool newestFirst = true) {
            size_t n = arr.size();
            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j + 1 < n - i; ++j) {
                    bool shouldSwap = false;
                    if (newestFirst) {
                        if (arr[j].getReviewDate() < arr[j + 1].getReviewDate()) shouldSwap = true;
                    } else {
                        if (arr[j].getReviewDate() > arr[j + 1].getReviewDate()) shouldSwap = true;
                    }
                    if (shouldSwap) {
                        std::swap(arr[j], arr[j + 1]);
                    }
                }
            }
        }

        static int binarySearchGearBySku(const std::vector<CampingGear>& sortedGears, const std::string& targetSku) {
            int low = 0;
            int high = static_cast<int>(sortedGears.size()) - 1;
            while (low <= high) {
                int mid = low + (high - low) / 2;
                if (sortedGears[mid].getSku() == targetSku) return mid;
                if (sortedGears[mid].getSku() < targetSku) low = mid + 1;
                else high = mid - 1;
            }
            return -1;
        }

        static std::vector<CampingGear> linearSearchGears(const std::vector<CampingGear>& gears,
                                                         const std::string& keyword,
                                                         int categoryFilter = -1,
                                                         double maxPrice = -1.0,
                                                         bool onlyAvailable = false) {
            std::vector<CampingGear> result;

            auto toLower = [](const std::string& str) -> std::string {
                std::string lower = str;
                std::transform(lower.begin(), lower.end(), lower.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                return lower;
            };

            std::string kwLower = toLower(keyword);

            for (const auto& g : gears) {
                if (categoryFilter >= 0 && static_cast<int>(g.getCategory()) != categoryFilter) {
                    continue;
                }
                if (maxPrice > 0.0 && g.getRentalPricePerDay() > maxPrice) {
                    continue;
                }
                if (onlyAvailable && g.getStockAvailable() <= 0) {
                    continue;
                }
                if (!kwLower.empty()) {
                    std::string nameLower = toLower(g.getName());
                    std::string skuLower = toLower(g.getSku());
                    std::string brandLower = toLower(g.getBrand());
                    std::string specLower = toLower(g.getSpecification());

                    if (nameLower.find(kwLower) == std::string::npos &&
                        skuLower.find(kwLower) == std::string::npos &&
                        brandLower.find(kwLower) == std::string::npos &&
                        specLower.find(kwLower) == std::string::npos) {
                        continue;
                    }
                }
                result.push_back(g);
            }
            return result;
        }

        static std::vector<RentalTransaction> linearSearchTransactions(const std::vector<RentalTransaction>& txs,
                                                                      const std::string& query) {
            std::vector<RentalTransaction> result;
            auto toLower = [](const std::string& str) -> std::string {
                std::string lower = str;
                std::transform(lower.begin(), lower.end(), lower.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                return lower;
            };
            std::string qLower = toLower(query);

            for (const auto& tx : txs) {
                if (qLower.empty()) {
                    result.push_back(tx);
                    continue;
                }
                std::string inv = toLower(tx.getInvoiceNumber());
                std::string name = toLower(tx.getCustomerName());
                std::string phone = toLower(tx.getCustomerPhone());
                std::string idc = toLower(tx.getCustomerIdCard());

                if (inv.find(qLower) != std::string::npos ||
                    name.find(qLower) != std::string::npos ||
                    phone.find(qLower) != std::string::npos ||
                    idc.find(qLower) != std::string::npos) {
                    result.push_back(tx);
                }
            }
            return result;
        }
    };

    // ================================================================================================
    // [BAGIAN 5: LAYANAN BISNIS (SERVICES LAYER - ANGGOTA 2)]
    // ================================================================================================

    class AuthService {
    private:
        DataManager& dataManager;
        UserSession currentSession;

    public:
        explicit AuthService(DataManager& dm) : dataManager(dm) {}

        const UserSession& getCurrentSession() const { return currentSession; }
        bool isAuthenticated() const { return currentSession.isLoggedIn; }
        bool isAdmin() const { return currentSession.isLoggedIn && currentSession.role == UserRole::ADMIN; }

        static std::string hashPassword(const std::string& rawPassword) {
            const uint64_t FNV_offset_basis = 0xcbf29ce484222325ULL;
            const uint64_t FNV_prime = 0x100000001b3ULL;
            uint64_t hash = FNV_offset_basis;

            std::string salted = "CAMPING_SALT_2026_" + rawPassword;
            for (char c : salted) {
                hash ^= static_cast<uint64_t>(c);
                hash *= FNV_prime;
            }

            std::ostringstream oss;
            oss << std::hex << std::setw(16) << std::setfill('0') << hash;
            return oss.str();
        }

        static bool validateEmailFormat(const std::string& email) {
            size_t atPos = email.find('@');
            size_t dotPos = email.rfind('.');
            return (atPos != std::string::npos && dotPos != std::string::npos && atPos < dotPos && atPos > 0);
        }

        static bool validatePhoneFormat(const std::string& phone) {
            if (phone.length() < 10 || phone.length() > 15) return false;
            for (char c : phone) {
                if (!std::isdigit(c)) return false;
            }
            return true;
        }

        static bool validateUsernameFormat(const std::string& username) {
            if (username.length() < 4 || username.length() > 20) return false;
            for (char c : username) {
                if (!std::isalnum(c) && c != '_') return false;
            }
            return true;
        }

        ServiceResult registerUser(const std::string& username, const std::string& password,
                                  const std::string& confirmPassword, const std::string& fullName,
                                  const std::string& phone, const std::string& email,
                                  const std::string& idCard, const std::string& address) {
            if (!validateUsernameFormat(username)) {
                return ServiceResult::fail("Username minimal 4-20 karakter alfanumerik!");
            }
            if (password.length() < 6) {
                return ServiceResult::fail("Password minimal 6 karakter!");
            }
            if (password != confirmPassword) {
                return ServiceResult::fail("Konfirmasi password tidak cocok!");
            }
            if (fullName.empty() || fullName.length() < 3) {
                return ServiceResult::fail("Nama lengkap wajib diisi minimal 3 karakter!");
            }
            if (!validatePhoneFormat(phone)) {
                return ServiceResult::fail("Format nomor HP tidak valid (harus 10-15 digit angka)!");
            }
            if (!validateEmailFormat(email)) {
                return ServiceResult::fail("Format email tidak valid!");
            }
            if (idCard.length() < 8) {
                return ServiceResult::fail("Nomor identitas (KTP/KTM/Paspor) wajib diisi valid!");
            }

            if (dataManager.findUserByUsername(username) != nullptr) {
                return ServiceResult::fail("Username sudah terdaftar! Silakan gunakan username lain.");
            }
            if (dataManager.findUserByEmail(email) != nullptr) {
                return ServiceResult::fail("Email sudah terdaftar dalam sistem!");
            }

            int newId = dataManager.getNextUserId();
            User newUser(newId, username, password, fullName, phone, email, idCard, address,
                         UserRole::USER, UserStatus::ACTIVE, 0, 100.0, "2026-11-07");

            if (dataManager.addUser(newUser)) {
                return ServiceResult::ok("Registrasi akun berhasil! Silakan login.");
            }
            return ServiceResult::fail("Gagal menyimpan data pengguna ke berkas!");
        }

        ServiceResult login(const std::string& username, const std::string& password) {
            User* user = dataManager.findUserByUsername(username);
            if (user == nullptr) {
                return ServiceResult::fail("Username tidak ditemukan!");
            }

            if (user->getPasswordHash() != password &&
                user->getPasswordHash() != hashPassword(password)) {
                return ServiceResult::fail("Password salah!");
            }

            if (!user->isActive()) {
                return ServiceResult::fail("Akun Anda telah dinonaktifkan atau diblokir oleh admin!");
            }

            currentSession.userId = user->getId();
            currentSession.username = user->getUsername();
            currentSession.fullName = user->getFullName();
            currentSession.role = user->getRole();
            currentSession.isLoggedIn = true;

            return ServiceResult::ok("Login berhasil! Selamat datang, " + user->getFullName());
        }

        void logout() {
            currentSession.clear();
        }

        ServiceResult changePassword(const std::string& oldPassword, const std::string& newPassword) {
            if (!isAuthenticated()) return ServiceResult::fail("Anda belum login!");
            User* user = dataManager.findUserById(currentSession.userId);
            if (!user) return ServiceResult::fail("User tidak ditemukan!");

            if (user->getPasswordHash() != oldPassword &&
                user->getPasswordHash() != hashPassword(oldPassword)) {
                return ServiceResult::fail("Password lama salah!");
            }

            if (newPassword.length() < 6) return ServiceResult::fail("Password baru minimal 6 karakter!");

            user->setPasswordHash(newPassword);
            dataManager.saveUsersToFile();
            return ServiceResult::ok("Password berhasil diperbarui!");
        }

        ServiceResult updateProfile(const std::string& fullName, const std::string& phone,
                                   const std::string& email, const std::string& address) {
            if (!isAuthenticated()) return ServiceResult::fail("Anda belum login!");
            User* user = dataManager.findUserById(currentSession.userId);
            if (!user) return ServiceResult::fail("User tidak ditemukan!");

            if (!validatePhoneFormat(phone)) return ServiceResult::fail("Nomor telepon tidak valid!");
            if (!validateEmailFormat(email)) return ServiceResult::fail("Email tidak valid!");

            user->setFullName(fullName);
            user->setPhoneNumber(phone);
            user->setEmail(email);
            user->setAddress(address);

            currentSession.fullName = fullName;
            dataManager.saveUsersToFile();
            return ServiceResult::ok("Profil Anda berhasil diperbarui!");
        }
    };

    class CatalogService {
    private:
        DataManager& dataManager;

    public:
        explicit CatalogService(DataManager& dm) : dataManager(dm) {}

        std::vector<CampingGear> getCatalogList(GearSortField sortField = GearSortField::NAME_ASC,
                                               int categoryFilter = -1,
                                               const std::string& searchKeyword = "",
                                               double maxPrice = -1.0,
                                               bool onlyAvailable = false) {
            const std::vector<CampingGear>& allGears = dataManager.getAllGears();

            std::vector<CampingGear> filtered = AlgorithmEngine::linearSearchGears(
                allGears, searchKeyword, categoryFilter, maxPrice, onlyAvailable
            );

            if (!filtered.empty()) {
                AlgorithmEngine::mergeSortGears(filtered, 0, static_cast<int>(filtered.size()) - 1, sortField);
            }

            return filtered;
        }

        CampingGear* getGearDetail(int gearId) {
            return dataManager.findGearById(gearId);
        }

        CampingGear* getGearBySku(const std::string& sku) {
            return dataManager.findGearBySku(sku);
        }

        std::vector<ReviewRating> getGearReviews(int gearId) {
            std::vector<ReviewRating> revs = dataManager.getReviewsByGearId(gearId);
            AlgorithmEngine::bubbleSortReviews(revs, true);
            return revs;
        }

        ServiceResult submitReview(int gearId, int userId, const std::string& username,
                                  int ratingValue, const std::string& reviewText) {
            CampingGear* gear = dataManager.findGearById(gearId);
            if (!gear) return ServiceResult::fail("Alat camping tidak ditemukan!");

            if (ratingValue < 1 || ratingValue > 5) {
                return ServiceResult::fail("Rating harus antara 1 sampai 5 bintang!");
            }

            int revId = dataManager.getNextReviewId();
            ReviewRating rev(revId, gearId, userId, username, ratingValue, reviewText, "2026-11-07");
            dataManager.addReview(rev);

            gear->addReview(static_cast<double>(ratingValue));
            dataManager.saveGearsToFile();

            return ServiceResult::ok("Ulasan Anda berhasil dikirim! Terima kasih.");
        }
    };

    class CartService {
    private:
        DataManager& dataManager;
        Cart userCart;

    public:
        explicit CartService(DataManager& dm) : dataManager(dm) {}

        void initializeForUser(int userId) {
            userCart.setUserId(userId);
            userCart.clear();
        }

        const Cart& getCart() const { return userCart; }
        Cart& getCartRef() { return userCart; }

        ServiceResult addToCart(int gearId, int quantity, int rentalDays) {
            if (quantity <= 0) return ServiceResult::fail("Jumlah unit harus lebih dari 0!");
            if (rentalDays <= 0) return ServiceResult::fail("Durasi sewa minimal 1 hari!");

            CampingGear* gear = dataManager.findGearById(gearId);
            if (!gear) return ServiceResult::fail("Barang tidak ditemukan dalam inventaris!");

            int currentInCart = 0;
            for (const auto& it : userCart.getItems()) {
                if (it.getGearId() == gearId) {
                    currentInCart += it.getQuantity();
                }
            }

            if (currentInCart + quantity > gear->getStockAvailable()) {
                std::ostringstream oss;
                oss << "Stok tidak mencukupi! Tersedia: " << gear->getStockAvailable()
                    << " unit (Sudah di keranjang: " << currentInCart << ").";
                return ServiceResult::fail(oss.str());
            }

            userCart.addItem(*gear, quantity, rentalDays);
            return ServiceResult::ok("Barang berhasil dimasukkan ke dalam keranjang!");
        }

        ServiceResult updateItemQuantity(int gearId, int newQuantity) {
            if (newQuantity <= 0) {
                userCart.removeItem(gearId);
                return ServiceResult::ok("Barang dihapus dari keranjang.");
            }

            CampingGear* gear = dataManager.findGearById(gearId);
            if (!gear) return ServiceResult::fail("Barang tidak ditemukan!");

            if (newQuantity > gear->getStockAvailable()) {
                return ServiceResult::fail("Kuantitas melebihi stok yang tersedia!");
            }

            userCart.updateQuantity(gearId, newQuantity);
            return ServiceResult::ok("Kuantitas barang berhasil diperbarui.");
        }

        ServiceResult removeItem(int gearId) {
            if (userCart.removeItem(gearId)) {
                return ServiceResult::ok("Item berhasil dihapus dari keranjang.");
            }
            return ServiceResult::fail("Item tidak ditemukan di keranjang.");
        }

        void clearCart() {
            userCart.clear();
        }

        int getTotalItemCount() const {
            return userCart.getTotalItemCount();
        }

        double calculateCartSubtotal() const {
            return userCart.calculateSubtotal();
        }

        double calculateCartEstimatedDeposit() const {
            double totalReplVal = 0.0;
            for (const auto& it : userCart.getItems()) {
                totalReplVal += (it.getReplacementValue() * it.getQuantity());
            }
            double est = totalReplVal * 0.10;
            return std::max(50000.0, std::round(est / 5000.0) * 5000.0);
        }
    };

    class RentalService {
    private:
        DataManager& dataManager;
        std::vector<PromoCode> availablePromos;

        void initializePromos() {
            availablePromos.clear();
            availablePromos.push_back(PromoCode("CAMPINGASIK10", 10.0, 50000.0, 100000.0, true,
                "Diskon 10% hingga Rp 50.000 untuk sewa di atas Rp 100.000"));
            availablePromos.push_back(PromoCode("WEEKENDSERU20", 20.0, 100000.0, 200000.0, true,
                "Diskon 20% hingga Rp 100.000 untuk petualangan seru minimal Rp 200.000"));
            availablePromos.push_back(PromoCode("MEMBERBARU", 15.0, 25000.0, 50000.0, true,
                "Diskon spesial 15% maksimal Rp 25.000 untuk semua pelanggan baru"));
        }

    public:
        explicit RentalService(DataManager& dm) : dataManager(dm) {
            initializePromos();
        }

        const std::vector<PromoCode>& getAvailablePromos() const { return availablePromos; }

        PromoCode* findPromoCode(const std::string& code) {
            for (auto& p : availablePromos) {
                if (p.getCode() == code && p.getIsActive()) return &p;
            }
            return nullptr;
        }

        double calculatePromoDiscount(const std::string& code, double subtotal) {
            PromoCode* promo = findPromoCode(code);
            if (promo) return promo->calculateDiscount(subtotal);
            return 0.0;
        }

        static std::string getCurrentDateString() {
            return "2026-11-07";
        }

        static std::string addDaysToDate(const std::string& dateStr, int days) {
            int year, month, day;
            char dash1, dash2;
            std::stringstream ss(dateStr);
            if (!(ss >> year >> dash1 >> month >> dash2 >> day)) {
                return dateStr;
            }

            std::tm tm_date = {};
            tm_date.tm_year = year - 1900;
            tm_date.tm_mon = month - 1;
            tm_date.tm_mday = day + days;
            tm_date.tm_isdst = -1;

            std::mktime(&tm_date);

            std::ostringstream oss;
            oss << (tm_date.tm_year + 1900) << "-"
                << std::setw(2) << std::setfill('0') << (tm_date.tm_mon + 1) << "-"
                << std::setw(2) << std::setfill('0') << tm_date.tm_mday;
            return oss.str();
        }

        static int calculateDaysBetween(const std::string& startDate, const std::string& endDate) {
            int y1, m1, d1, y2, m2, d2;
            char c;
            std::stringstream s1(startDate), s2(endDate);
            s1 >> y1 >> c >> m1 >> c >> d1;
            s2 >> y2 >> c >> m2 >> c >> d2;

            std::tm tm1 = {}, tm2 = {};
            tm1.tm_year = y1 - 1900; tm1.tm_mon = m1 - 1; tm1.tm_mday = d1;
            tm2.tm_year = y2 - 1900; tm2.tm_mon = m2 - 1; tm2.tm_mday = d2;

            std::time_t t1 = std::mktime(&tm1);
            std::time_t t2 = std::mktime(&tm2);

            double seconds = std::difftime(t2, t1);
            return static_cast<int>(seconds / (60 * 60 * 24));
        }

        ServiceResult processCheckout(int userId,
                                      const std::string& customerName,
                                      const std::string& customerPhone,
                                      const std::string& customerIdCard,
                                      const std::string& startDate,
                                      int rentalDurationDays,
                                      const std::vector<RentalItem>& items,
                                      PaymentMethod paymentMethod,
                                      const std::string& promoCode = "",
                                      const std::string& orderNotes = "") {
            if (items.empty()) return ServiceResult::fail("Keranjang sewa masih kosong!");
            if (rentalDurationDays <= 0) return ServiceResult::fail("Durasi sewa minimal 1 hari!");
            if (customerName.empty() || customerPhone.empty() || customerIdCard.empty()) {
                return ServiceResult::fail("Data pelanggan dan identitas (KTP) wajib lengkap!");
            }

            for (const auto& item : items) {
                CampingGear* gear = dataManager.findGearById(item.getGearId());
                if (!gear) {
                    return ServiceResult::fail("Alat dengan ID " + std::to_string(item.getGearId()) + " tidak ditemukan!");
                }
                if (gear->getStockAvailable() < item.getQuantity()) {
                    return ServiceResult::fail("Stok barang '" + gear->getName() + "' tidak mencukupi saat ini!");
                }
            }

            double subtotal = 0.0;
            double totalReplVal = 0.0;
            for (const auto& item : items) {
                subtotal += (item.getPricePerDay() * item.getQuantity() * rentalDurationDays);
                totalReplVal += (item.getReplacementValue() * item.getQuantity());
            }

            double discount = 0.0;
            std::string usedCode = "-";
            if (!promoCode.empty()) {
                discount = calculatePromoDiscount(promoCode, subtotal);
                if (discount > 0.0) usedCode = promoCode;
            }

            double deposit = std::max(50000.0, std::round((totalReplVal * 0.10) / 5000.0) * 5000.0);
            double totalAmount = (subtotal - discount) + deposit;

            for (const auto& item : items) {
                CampingGear* gear = dataManager.findGearById(item.getGearId());
                if (gear) gear->rentUnits(item.getQuantity());
            }
            dataManager.saveGearsToFile();

            std::string invoice = dataManager.generateNextInvoiceNumber();
            std::string bookingDate = getCurrentDateString();
            std::string expectedReturn = addDaysToDate(startDate, rentalDurationDays);

            RentalTransaction tx;
            tx.setInvoiceNumber(invoice);
            tx.setUserId(userId);
            tx.setCustomerName(customerName);
            tx.setCustomerPhone(customerPhone);
            tx.setCustomerIdCard(customerIdCard);
            tx.setBookingDate(bookingDate);
            tx.setStartDate(startDate);
            tx.setExpectedReturnDate(expectedReturn);
            tx.setActualReturnDate("-");
            tx.setItems(items);
            tx.setSubtotal(subtotal);
            tx.setDiscountAmount(discount);
            tx.setPromoCodeUsed(usedCode);
            tx.setDepositAmount(deposit);
            tx.setTotalAmount(totalAmount);
            tx.setPaymentMethod(paymentMethod);
            tx.setPaymentStatus(paymentMethod == PaymentMethod::CASH_ON_PICKUP ? PaymentStatus::PENDING_VERIFICATION : PaymentStatus::UNPAID);
            tx.setTransactionStatus(TransactionStatus::PENDING_PAYMENT);
            tx.setNotes(orderNotes);

            if (dataManager.addTransaction(tx)) {
                User* user = dataManager.findUserById(userId);
                if (user) {
                    user->incrementRentalCount();
                    dataManager.saveUsersToFile();
                }
                return ServiceResult::ok(invoice);
            }

            return ServiceResult::fail("Gagal menyimpan transaksi ke berkas!");
        }

        std::vector<RentalTransaction> getUserTransactions(int userId) {
            std::vector<RentalTransaction> list = dataManager.findTransactionsByUserId(userId);
            if (!list.empty()) {
                AlgorithmEngine::quickSortTransactions(list, 0, static_cast<int>(list.size()) - 1, TransactionSortField::DATE_DESC);
            }
            return list;
        }

        RentalTransaction* getTransactionDetail(const std::string& invoiceNumber) {
            return dataManager.findTransactionByInvoice(invoiceNumber);
        }
    };

    class PaymentService {
    private:
        DataManager& dataManager;

    public:
        explicit PaymentService(DataManager& dm) : dataManager(dm) {}

        std::string generatePaymentInstructions(PaymentMethod method, double totalAmount,
                                               const std::string& invoiceNumber) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(0);
            switch (method) {
                case PaymentMethod::TRANSFER_BANK_BCA:
                    oss << "Transfer Bank BCA Virtual Account\n"
                        << "Nomor VA     : 827708" << invoiceNumber.substr(invoiceNumber.length() - 6) << "\n"
                        << "Atas Nama    : Camping Rental Official\n"
                        << "Total Bayar  : Rp " << totalAmount << "\n"
                        << "Batas Waktu  : 1x24 Jam";
                    break;
                case PaymentMethod::TRANSFER_BANK_MANDIRI:
                    oss << "Transfer Bank Mandiri Virtual Account\n"
                        << "Nomor Mandiri: 887008" << invoiceNumber.substr(invoiceNumber.length() - 6) << "\n"
                        << "Atas Nama    : Camping Rental Mandiri\n"
                        << "Total Bayar  : Rp " << totalAmount;
                    break;
                case PaymentMethod::TRANSFER_BANK_BRI:
                    oss << "Transfer Bank BRI BRIVA\n"
                        << "Nomor BRIVA  : 104508" << invoiceNumber.substr(invoiceNumber.length() - 6) << "\n"
                        << "Atas Nama    : Camping Rental BRI\n"
                        << "Total Bayar  : Rp " << totalAmount;
                    break;
                case PaymentMethod::EWALLET_QRIS:
                    oss << "QRIS Instant Payment (NMID: ID1020304050607)\n"
                        << "Scan QR code di kasir/aplikasi BCA/GoPay/OVO/Dana/ShopeePay\n"
                        << "Total Bayar  : Rp " << totalAmount << "\n"
                        << "Status       : Pembayaran otomatis terverifikasi.";
                    break;
                case PaymentMethod::EWALLET_GOPAY:
                case PaymentMethod::EWALLET_OVO:
                case PaymentMethod::EWALLET_DANA:
                    oss << "E-Wallet Transfer (" << paymentMethodToString(method) << ")\n"
                        << "Nomor Tujuan : 0812-3456-7890 (a.n Camping Hub)\n"
                        << "Total Bayar  : Rp " << totalAmount;
                    break;
                case PaymentMethod::CASH_ON_PICKUP:
                    oss << "Pembayaran Tunai (Cash on Pickup / COD)\n"
                        << "Silakan lakukan pembayaran tunai saat pengambilan unit di outlet.\n"
                        << "Total Bayar  : Rp " << totalAmount;
                    break;
            }
            return oss.str();
        }

        ServiceResult submitPaymentProof(const std::string& invoiceNumber, const std::string& proofRef) {
            RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
            if (!tx) return ServiceResult::fail("Invoice tidak ditemukan!");

            tx->setPaymentProofRef(proofRef);
            tx->setPaymentStatus(PaymentStatus::PENDING_VERIFICATION);
            tx->setTransactionStatus(TransactionStatus::WAITING_CONFIRMATION);
            dataManager.saveTransactionsToFile();

            return ServiceResult::ok("Bukti pembayaran berhasil dikirim! Menunggu konfirmasi admin.");
        }

        ServiceResult confirmPaymentByAdmin(const std::string& invoiceNumber, const std::string& adminName) {
            RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
            if (!tx) return ServiceResult::fail("Invoice tidak ditemukan!");

            tx->setPaymentStatus(PaymentStatus::VERIFIED_PAID);
            tx->setTransactionStatus(TransactionStatus::PAID_CONFIRMED);
            tx->setNotes(tx->getNotes() + " | Diverifikasi oleh " + adminName);
            dataManager.saveTransactionsToFile();

            return ServiceResult::ok("Pembayaran invoice " + invoiceNumber + " berhasil diverifikasi dan disetujui!");
        }
    };

    class ReturnService {
    private:
        DataManager& dataManager;

    public:
        explicit ReturnService(DataManager& dm) : dataManager(dm) {}

        static double calculateLateFeePerDay(double itemPricePerDay) {
            return itemPricePerDay * 1.5;
        }

        static double calculateConditionPenalty(double replacementValue, ReturnCondition condition) {
            switch (condition) {
                case ReturnCondition::GOOD_CONDITION:
                    return 0.0;
                case ReturnCondition::DIRTY_HEAVY:
                    return replacementValue * 0.10;
                case ReturnCondition::MINOR_DAMAGE:
                    return replacementValue * 0.25;
                case ReturnCondition::MAJOR_DAMAGE:
                    return replacementValue * 0.60;
                case ReturnCondition::LOST:
                    return replacementValue * 1.00;
                default:
                    return 0.0;
            }
        }

        ServiceResult processReturnInspection(const std::string& invoiceNumber,
                                             const std::string& actualReturnDate,
                                             const std::vector<ReturnCondition>& itemConditions,
                                             const std::string& adminInspectorName,
                                             const std::string& notes,
                                             ReturnLog& outReturnLog) {
            RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
            if (!tx) return ServiceResult::fail("Transaksi tidak ditemukan!");

            if (tx->getTransactionStatus() == TransactionStatus::RETURNED_COMPLETED ||
                tx->getTransactionStatus() == TransactionStatus::RETURNED_WITH_ISSUE) {
                return ServiceResult::fail("Transaksi ini sudah pernah diproses pengembaliannya!");
            }

            int overdueDays = RentalService::calculateDaysBetween(tx->getExpectedReturnDate(), actualReturnDate);
            if (overdueDays < 0) overdueDays = 0;

            double totalLateFee = 0.0;
            double totalDamageFee = 0.0;

            std::vector<RentalItem>& items = tx->getItemsRef();
            for (size_t i = 0; i < items.size(); ++i) {
                ReturnCondition cond = (i < itemConditions.size()) ? itemConditions[i] : ReturnCondition::GOOD_CONDITION;
                items[i].setReturnCondition(cond);

                if (overdueDays > 0) {
                    totalLateFee += (calculateLateFeePerDay(items[i].getPricePerDay()) * items[i].getQuantity() * overdueDays);
                }

                double dmg = calculateConditionPenalty(items[i].getReplacementValue(), cond) * items[i].getQuantity();
                items[i].setDamageFee(dmg);
                totalDamageFee += dmg;

                CampingGear* gear = dataManager.findGearById(items[i].getGearId());
                if (gear) {
                    if (cond != ReturnCondition::LOST) {
                        gear->returnUnits(items[i].getQuantity());
                    } else {
                        gear->setStockTotal(std::max(0, gear->getStockTotal() - items[i].getQuantity()));
                    }
                }
            }
            dataManager.saveGearsToFile();

            double depositHeld = tx->getDepositAmount();
            double totalPenalty = totalLateFee + totalDamageFee;
            double depositRefund = 0.0;
            double extraPaymentDue = 0.0;

            if (depositHeld >= totalPenalty) {
                depositRefund = depositHeld - totalPenalty;
                extraPaymentDue = 0.0;
            } else {
                depositRefund = 0.0;
                extraPaymentDue = totalPenalty - depositHeld;
            }

            tx->setActualReturnDate(actualReturnDate);
            tx->setLateFee(totalLateFee);
            tx->setDamagePenaltyFee(totalDamageFee);
            tx->setDepositRefunded(depositRefund);
            tx->setTransactionStatus(totalPenalty > 0 ? TransactionStatus::RETURNED_WITH_ISSUE : TransactionStatus::RETURNED_COMPLETED);
            dataManager.saveTransactionsToFile();

            int logId = dataManager.getNextReturnLogId();
            outReturnLog = ReturnLog(logId, invoiceNumber, actualReturnDate, overdueDays,
                                     totalLateFee, totalDamageFee, depositHeld,
                                     adminInspectorName, notes);
            outReturnLog.setDepositRefund(depositRefund);
            outReturnLog.setExtraPaymentDue(extraPaymentDue);
            dataManager.addReturnLog(outReturnLog);

            std::ostringstream msg;
            msg << "Pengembalian berhasil diproses! ";
            if (totalPenalty > 0) {
                msg << "Total Denda: Rp " << totalPenalty << ". ";
                if (depositRefund > 0) {
                    msg << "Sisa deposit yang dikembalikan ke pelanggan: Rp " << depositRefund;
                } else if (extraPaymentDue > 0) {
                    msg << "Kurang bayar denda yang harus dilunasi pelanggan: Rp " << extraPaymentDue;
                }
            } else {
                msg << "Alat lengkap dan mulus. Uang jaminan penuh Rp " << depositRefund << " dikembalikan.";
            }

            return ServiceResult::ok(msg.str());
        }
    };

    class AdminService {
    private:
        DataManager& dataManager;

    public:
        explicit AdminService(DataManager& dm) : dataManager(dm) {}

        ServiceResult addNewGear(const std::string& name, const std::string& brand,
                                GearCategory category, double rentalPrice, double replacementValue,
                                int stock, const std::string& spec, const std::string& notes) {
            if (name.empty() || brand.empty()) return ServiceResult::fail("Nama dan merk alat tidak boleh kosong!");
            if (rentalPrice <= 0) return ServiceResult::fail("Harga sewa harian harus lebih besar dari 0!");
            if (replacementValue <= 0) return ServiceResult::fail("Nilai penggantian barang harus valid!");
            if (stock <= 0) return ServiceResult::fail("Stok unit awal harus minimal 1!");

            int newId = dataManager.getNextGearId();
            std::string sku = dataManager.generateNextGearSku(category);

            CampingGear gear(newId, sku, name, brand, category, rentalPrice, replacementValue,
                             stock, stock, 0, 5.0, 0, spec, notes);

            if (dataManager.addGear(gear)) {
                return ServiceResult::ok("Alat camping baru berhasil ditambahkan! SKU: " + sku);
            }
            return ServiceResult::fail("Gagal menyimpan alat ke inventaris!");
        }

        ServiceResult updateGearDetails(int gearId, const std::string& name, const std::string& brand,
                                       double rentalPrice, double replacementValue, const std::string& spec) {
            CampingGear* gear = dataManager.findGearById(gearId);
            if (!gear) return ServiceResult::fail("Alat tidak ditemukan!");

            gear->setName(name);
            gear->setBrand(brand);
            gear->setRentalPricePerDay(rentalPrice);
            gear->setReplacementValue(replacementValue);
            gear->setSpecification(spec);

            dataManager.saveGearsToFile();
            return ServiceResult::ok("Data alat camping berhasil diperbarui!");
        }

        ServiceResult restockGear(int gearId, int additionalStock) {
            if (additionalStock <= 0) return ServiceResult::fail("Jumlah restock harus lebih dari 0!");
            CampingGear* gear = dataManager.findGearById(gearId);
            if (!gear) return ServiceResult::fail("Alat tidak ditemukan!");

            gear->setStockTotal(gear->getStockTotal() + additionalStock);
            gear->setStockAvailable(gear->getStockAvailable() + additionalStock);
            dataManager.saveGearsToFile();

            return ServiceResult::ok("Restock berhasil! Stok total sekarang: " + std::to_string(gear->getStockTotal()));
        }

        ServiceResult removeGear(int gearId) {
            CampingGear* gear = dataManager.findGearById(gearId);
            if (!gear) return ServiceResult::fail("Alat tidak ditemukan!");

            if (gear->getStockAvailable() < gear->getStockTotal()) {
                return ServiceResult::fail("Tidak dapat menghapus alat yang sedang dalam masa sewa aktif!");
            }

            if (dataManager.deleteGear(gearId)) {
                return ServiceResult::ok("Alat camping berhasil dihapus dari inventaris.");
            }
            return ServiceResult::fail("Gagal menghapus alat!");
        }

        ServiceResult toggleUserStatus(int userId, UserStatus newStatus) {
            User* user = dataManager.findUserById(userId);
            if (!user) return ServiceResult::fail("Pengguna tidak ditemukan!");
            if (user->isAdmin()) return ServiceResult::fail("Tidak dapat mengubah status akun Administrator!");

            user->setStatus(newStatus);
            dataManager.saveUsersToFile();
            return ServiceResult::ok("Status pengguna berhasil diubah menjadi: " + userStatusToString(newStatus));
        }

        ServiceResult markTransactionAsPickedUp(const std::string& invoiceNumber) {
            RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
            if (!tx) return ServiceResult::fail("Transaksi tidak ditemukan!");

            if (tx->getPaymentStatus() != PaymentStatus::VERIFIED_PAID &&
                tx->getPaymentMethod() != PaymentMethod::CASH_ON_PICKUP) {
                return ServiceResult::fail("Barang tidak boleh diserahkan sebelum pembayaran terverifikasi!");
            }

            tx->setTransactionStatus(TransactionStatus::ACTIVE_RENTED);
            dataManager.saveTransactionsToFile();
            return ServiceResult::ok("Unit berhasil diserahkan kepada pelanggan! Status sewa AKTIF.");
        }

        ServiceResult cancelTransaction(const std::string& invoiceNumber, const std::string& reason) {
            RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
            if (!tx) return ServiceResult::fail("Transaksi tidak ditemukan!");

            if (tx->getTransactionStatus() == TransactionStatus::ACTIVE_RENTED ||
                tx->getTransactionStatus() == TransactionStatus::RETURNED_COMPLETED) {
                return ServiceResult::fail("Tidak dapat membatalkan transaksi yang sedang berjalan atau sudah selesai!");
            }

            for (const auto& item : tx->getItems()) {
                CampingGear* gear = dataManager.findGearById(item.getGearId());
                if (gear) {
                    gear->returnUnits(item.getQuantity());
                }
            }
            dataManager.saveGearsToFile();

            tx->setTransactionStatus(TransactionStatus::CANCELLED);
            tx->setNotes(tx->getNotes() + " | Dibatalkan: " + reason);
            dataManager.saveTransactionsToFile();

            return ServiceResult::ok("Transaksi " + invoiceNumber + " berhasil dibatalkan dan stok dikembalikan.");
        }
    };

    class ReportFinanceService {
    private:
        DataManager& dataManager;

    public:
        explicit ReportFinanceService(DataManager& dm) : dataManager(dm) {}

        FinancialKPI calculateOverallFinancialKPI() {
            FinancialKPI kpi = {};
            const std::vector<RentalTransaction>& txs = dataManager.getAllTransactions();
            const std::vector<User>& users = dataManager.getAllUsers();
            const std::vector<CampingGear>& gears = dataManager.getAllGears();

            kpi.totalTransactionsCount = static_cast<int>(txs.size());

            for (const auto& u : users) {
                if (!u.isAdmin()) kpi.totalCustomersCount++;
            }

            kpi.totalGearsRegistered = static_cast<int>(gears.size());
            for (const auto& g : gears) {
                kpi.totalUnitsInStock += g.getStockTotal();
                kpi.totalUnitsRentedOut += (g.getStockTotal() - g.getStockAvailable());
            }

            for (const auto& tx : txs) {
                if (tx.getTransactionStatus() == TransactionStatus::CANCELLED) continue;

                if (tx.getTransactionStatus() == TransactionStatus::ACTIVE_RENTED) {
                    kpi.activeRentalsCount++;
                } else if (tx.getTransactionStatus() == TransactionStatus::RETURNED_COMPLETED ||
                           tx.getTransactionStatus() == TransactionStatus::RETURNED_WITH_ISSUE) {
                    kpi.completedRentalsCount++;
                }

                kpi.totalGrossRevenue += tx.getTotalAmount();
                kpi.totalRentalIncome += (tx.getSubtotal() - tx.getDiscountAmount());
                kpi.totalDiscountGiven += tx.getDiscountAmount();
                kpi.totalDepositHeld += tx.getDepositAmount();
                kpi.totalDepositRefunded += tx.getDepositRefunded();
                kpi.totalLatePenaltyIncome += tx.getLateFee();
                kpi.totalDamagePenaltyIncome += tx.getDamagePenaltyFee();
            }

            kpi.totalNetProfit = kpi.totalRentalIncome + kpi.totalLatePenaltyIncome + kpi.totalDamagePenaltyIncome;
            return kpi;
        }

        std::vector<std::pair<CampingGear, int>> getTop5RentedGears() {
            std::vector<CampingGear> copy = dataManager.getAllGears();
            std::sort(copy.begin(), copy.end(), [](const CampingGear& a, const CampingGear& b) {
                return a.getTimesRented() > b.getTimesRented();
            });

            std::vector<std::pair<CampingGear, int>> result;
            size_t count = std::min(size_t(5), copy.size());
            for (size_t i = 0; i < count; ++i) {
                result.push_back({copy[i], copy[i].getTimesRented()});
            }
            return result;
        }

        std::vector<CampingGear> getLowStockAlertGears(int threshold = 3) {
            std::vector<CampingGear> result;
            for (const auto& g : dataManager.getAllGears()) {
                if (g.getStockAvailable() <= threshold) {
                    result.push_back(g);
                }
            }
            return result;
        }

        std::string exportFinancialReportText() {
            FinancialKPI kpi = calculateOverallFinancialKPI();
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(0);
            oss << "========================================================================\n"
                << "              LAPORAN KEUANGAN & EKSEKUTIF PENYEWAAN CAMPING            \n"
                << "                      CAMPING GEAR RENTAL HUB 2026                      \n"
                << "========================================================================\n"
                << "Tanggal Laporan            : " << RentalService::getCurrentDateString() << "\n"
                << "Total Pelanggan Terdaftar  : " << kpi.totalCustomersCount << " orang\n"
                << "Total Jenis Alat Camping   : " << kpi.totalGearsRegistered << " tipe\n"
                << "Total Unit Dimiliki Toko   : " << kpi.totalUnitsInStock << " unit\n"
                << "Unit Sedang Disewa Luar    : " << kpi.totalUnitsRentedOut << " unit\n"
                << "------------------------------------------------------------------------\n"
                << "METRIK TRANSAKSI:\n"
                << "  Total Transaksi Masuk    : " << kpi.totalTransactionsCount << "\n"
                << "  Transaksi Selesai        : " << kpi.completedRentalsCount << "\n"
                << "  Transaksi Sedang Berjalan: " << kpi.activeRentalsCount << "\n"
                << "------------------------------------------------------------------------\n"
                << "RINCIAN KEUANGAN (ARUS KAS & PENDAPATAN):\n"
                << "  Pendapatan Kotor Masuk   : Rp " << kpi.totalGrossRevenue << "\n"
                << "  Pendapatan Murni Sewa    : Rp " << kpi.totalRentalIncome << "\n"
                << "  Potongan Diskon Promo    : Rp " << kpi.totalDiscountGiven << "\n"
                << "  Total Deposit Diterima   : Rp " << kpi.totalDepositHeld << "\n"
                << "  Total Deposit Dikembalikan: Rp " << kpi.totalDepositRefunded << "\n"
                << "  Pendapatan Denda Telat   : Rp " << kpi.totalLatePenaltyIncome << "\n"
                << "  Pendapatan Denda Kerusakan: Rp " << kpi.totalDamagePenaltyIncome << "\n"
                << "------------------------------------------------------------------------\n"
                << "TOTAL LABA BERSIH OPERASIONAL: Rp " << kpi.totalNetProfit << "\n"
                << "========================================================================\n";
            return oss.str();
        }
    };

    // ================================================================================================
    // [LAYANAN BISNIS TAMBAHAN: BUNDLE SERVICE (PAKET HEMAT CAMPING)]
    // ================================================================================================
    class BundleService {
    private:
        DataManager& dataManager;
        std::vector<CampingBundle> availableBundles;

        void initializeDefaultBundles() {
            availableBundles.clear();

            // Paket 1: Paket Solo Hikers / Pendaki Tunggal
            CampingBundle b1(1, "BND-01", "Paket Solo Hikers (1 Orang)",
                "Perlengkapan ultralight mandiri untuk pendaki solo: Tenda 2P, SB Polar, Kompor, Headlamp",
                87000.0, 70000.0, 50000.0);
            b1.addBundleItem(3, "Tenda Naturehike Cloud Up 2", 1);
            b1.addBundleItem(5, "Sleeping Bag Arei Everest Polar", 1);
            b1.addBundleItem(11, "Kompor Ultralight Windproof Kovar", 1);
            b1.addBundleItem(14, "Headlamp Petzl Tikka 350 Lumens", 1);
            availableBundles.push_back(b1);

            // Paket 2: Paket Duet Romantis / Bestie (2 Orang)
            CampingBundle b2(2, "BND-02", "Paket Duo Adventure (2 Orang)",
                "Paket favorit sepasang sahabat: Tenda 4P, 2 SB, Matras Foil, Kompor, Cookset, 2 Headlamp",
                142000.0, 115000.0, 75000.0);
            b2.addBundleItem(1, "Tenda Eiger Guardian 4P", 1);
            b2.addBundleItem(5, "Sleeping Bag Arei Everest Polar", 2);
            b2.addBundleItem(7, "Matras Alumunium Foil Double Side", 1);
            b2.addBundleItem(11, "Kompor Ultralight Windproof Kovar", 1);
            b2.addBundleItem(12, "Nesting Cooking Set DS-308", 1);
            b2.addBundleItem(14, "Headlamp Petzl Tikka 350 Lumens", 2);
            availableBundles.push_back(b2);

            // Paket 3: Paket Regu Puncak (4 Orang Siap Summit)
            CampingBundle b3(3, "BND-03", "Paket Squad Mountaineer (4 Orang)",
                "Paket lengkap pendakian tim 4 orang: Tenda 4P, 4 SB, 2 Matras, Kompor, Cookset, Lentera, HT",
                210000.0, 165000.0, 100000.0);
            b3.addBundleItem(2, "Tenda Consina Magnum 4", 1);
            b3.addBundleItem(5, "Sleeping Bag Arei Everest Polar", 4);
            b3.addBundleItem(7, "Matras Alumunium Foil Double Side", 2);
            b3.addBundleItem(11, "Kompor Ultralight Windproof Kovar", 1);
            b3.addBundleItem(12, "Nesting Cooking Set DS-308", 1);
            b3.addBundleItem(15, "Lentera Tenda LED Solar", 1);
            b3.addBundleItem(17, "HT Walkie Talkie Baofeng UV-5R", 1);
            availableBundles.push_back(b3);

            // Paket 4: Paket Glamping & Santai Keluarga (4-5 Orang)
            CampingBundle b4(4, "BND-04", "Paket Glamping & Family Leisure",
                "Kenyamanan maksimal berkemah keluarga: Tenda Besar Quechua, 2 Kursi Lipat, Meja, Hammock, Lentera",
                160000.0, 128000.0, 100000.0);
            b4.addBundleItem(4, "Quechua Arpenaz Family 4.1", 1);
            b4.addBundleItem(19, "Hammock Double Layer TTTM", 1);
            b4.addBundleItem(20, "Kursi Lipat Duralumin", 2);
            b4.addBundleItem(21, "Meja Lipat Aluminium Portable", 1);
            b4.addBundleItem(15, "Lentera Tenda LED Solar", 1);
            availableBundles.push_back(b4);
        }

    public:
        explicit BundleService(DataManager& dm) : dataManager(dm) {
            initializeDefaultBundles();
        }

        const std::vector<CampingBundle>& getAllBundles() const {
            return availableBundles;
        }

        CampingBundle* findBundleById(int bundleId) {
            for (auto& b : availableBundles) {
                if (b.getId() == bundleId) return &b;
            }
            return nullptr;
        }

        CampingBundle* findBundleByCode(const std::string& code) {
            for (auto& b : availableBundles) {
                if (b.getCode() == code) return &b;
            }
            return nullptr;
        }

        ServiceResult addBundleToCart(CartService& cartService, int bundleId, int rentalDays) {
            CampingBundle* bundle = findBundleById(bundleId);
            if (!bundle) return ServiceResult::fail("Paket hemat tidak ditemukan!");

            for (const auto& bi : bundle->getBundleItems()) {
                CampingGear* gear = dataManager.findGearById(bi.gearId);
                if (!gear || gear->getStockAvailable() < bi.quantity) {
                    return ServiceResult::fail("Stok barang '" + (gear ? gear->getName() : "Item") + "' tidak mencukupi untuk paket ini!");
                }
            }

            for (const auto& bi : bundle->getBundleItems()) {
                cartService.addToCart(bi.gearId, bi.quantity, rentalDays);
            }

            std::ostringstream msg;
            msg << "Paket '" << bundle->getName() << "' (" << bundle->getBundleItems().size()
                << " jenis barang) berhasil dimasukkan ke keranjang sewa! Anda hemat Rp "
                << static_cast<long long>(bundle->calculateSavingsPerDay() * rentalDays) << "!";
            return ServiceResult::ok(msg.str());
        }
    };

    // ================================================================================================
    // [BAGIAN 6: ANTARMUKA VISUAL & TATA LETAK CLI (ANGGOTA 3 - UI LAYER)]
    // ================================================================================================

    class UIViews {
    public:
        static void enableVirtualTerminal() {
#if defined(_WIN32) || defined(_WIN64)
            static bool initialized = false;
            if (!initialized) {
                HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
                if (hOut != INVALID_HANDLE_VALUE) {
                    DWORD dwMode = 0;
                    if (GetConsoleMode(hOut, &dwMode)) {
                        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                        SetConsoleMode(hOut, dwMode);
                    }
                }
                initialized = true;
            }
#endif
        }

        static void clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
            std::system("cls");
            enableVirtualTerminal();
#else
            std::system("clear");
#endif
        }

        static void pauseScreen() {
            std::cout << "\nTekan [ENTER] untuk melanjutkan...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        static std::string formatRupiah(double amount) {
            long long val = static_cast<long long>(std::round(amount));
            bool isNegative = val < 0;
            if (isNegative) val = -val;

            std::string s = std::to_string(val);
            int n = static_cast<int>(s.length()) - 3;
            while (n > 0) {
                s.insert(n, ".");
                n -= 3;
            }
            return (isNegative ? "-Rp " : "Rp ") + s;
        }

        static std::string centerText(const std::string& text, int width) {
            int len = static_cast<int>(text.length());
            if (len >= width) return text;
            int leftPadding = (width - len) / 2;
            int rightPadding = width - len - leftPadding;
            return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
        }

        static std::string padRight(const std::string& text, size_t width) {
            if (text.length() >= width) return text.substr(0, width);
            return text + std::string(width - text.length(), ' ');
        }

        static std::string padLeft(const std::string& text, size_t width) {
            if (text.length() >= width) return text.substr(0, width);
            return std::string(width - text.length(), ' ') + text;
        }

        static void printMainBanner() {
            enableVirtualTerminal();
            std::cout << ConsoleColor::BLUE;
            std::cout << "=========================================================================================\n";
            std::cout << "    /\\_ /\\      CAMPING GEAR RENTAL HUB - SISTEM PENYEWAAN ALAT CAMPING                  \n";
            std::cout << "   (  . .)     Penyewaan Alat Outdoor Terlengkap: Tenda, Carrier, Kompor & Sleeping Bag \n";
            std::cout << "    >  ^ <      Bersih, Terawat, Berkualitas Tinggi, & Siap Menemani Ekspedisi Anda!     \n";
            std::cout << "=========================================================================================\n";
            std::cout << ConsoleColor::RESET;
        }

        static void printSectionHeader(const std::string& title) {
            std::cout << ConsoleColor::BLUE;
            std::cout << "\n+---------------------------------------------------------------------------------------+\n";
            std::cout << "| " << padRight(">> " + title, 85) << " |\n";
            std::cout << "+---------------------------------------------------------------------------------------+\n";
            std::cout << ConsoleColor::RESET;
        }

        static void printSubHeader(const std::string& title) {
            std::cout << "\n--- [ " << title << " ] ---\n";
        }

        static void printSuccessBox(const std::string& message) {
            std::cout << "\n" << ConsoleColor::GREEN << "[BERHASIL] " << message << ConsoleColor::RESET << "\n";
        }

        static void printErrorBox(const std::string& message) {
            std::cout << "\n" << ConsoleColor::RED << "[PERINGATAN/GAGAL] " << message << ConsoleColor::RESET << "\n";
        }

        static void printInfoBox(const std::string& message) {
            std::cout << "\n[INFO] " << message << "\n";
        }

        static int readInt(const std::string& prompt, int minVal = 0, int maxVal = 999999) {
            int val;
            while (true) {
                std::cout << prompt;
                if (std::cin >> val) {
                    if (val >= minVal && val <= maxVal) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        return val;
                    }
                    std::cout << ConsoleColor::RED << "Input harus antara " << minVal << " sampai " << maxVal << "!" << ConsoleColor::RESET << "\n";
                } else {
                    std::cout << ConsoleColor::RED << "Input tidak valid! Harap masukkan angka bulat." << ConsoleColor::RESET << "\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }

        static double readDouble(const std::string& prompt, double minVal = 0.0, double maxVal = 1e9) {
            double val;
            while (true) {
                std::cout << prompt;
                if (std::cin >> val) {
                    if (val >= minVal && val <= maxVal) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        return val;
                    }
                    std::cout << ConsoleColor::RED << "Input harus antara " << formatRupiah(minVal) << " sampai " << formatRupiah(maxVal) << "!" << ConsoleColor::RESET << "\n";
                } else {
                    std::cout << ConsoleColor::RED << "Input tidak valid! Harap masukkan angka numerik." << ConsoleColor::RESET << "\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }

        static std::string readString(const std::string& prompt, bool allowEmpty = false) {
            std::string input;
            while (true) {
                std::cout << prompt;
                std::getline(std::cin, input);
                if (allowEmpty || !input.empty()) {
                    return input;
                }
                std::cout << ConsoleColor::RED << "Input tidak boleh kosong!" << ConsoleColor::RESET << "\n";
            }
        }

        static bool confirmAction(const std::string& question) {
            std::string ans;
            std::cout << question << " (y/n): ";
            std::getline(std::cin, ans);
            return (!ans.empty() && (ans[0] == 'y' || ans[0] == 'Y'));
        }

        static void displayGearCatalogTable(const std::vector<CampingGear>& gears, int currentPage = 1, int itemsPerPage = 8) {
            if (gears.empty()) {
                std::cout << "\n+---------------------------------------------------------------------------------------+\n";
                std::cout << "|                     TIDAK ADA ALAT CAMPING YANG MEMENUHI KRITERIA                     |\n";
                std::cout << "+---------------------------------------------------------------------------------------+\n";
                return;
            }

            int totalItems = static_cast<int>(gears.size());
            int totalPages = (totalItems + itemsPerPage - 1) / itemsPerPage;
            if (currentPage < 1) currentPage = 1;
            if (currentPage > totalPages) currentPage = totalPages;

            int startIndex = (currentPage - 1) * itemsPerPage;
            int endIndex = std::min(startIndex + itemsPerPage, totalItems);

            std::cout << "\n+-----+---------+-----------------------------------+--------------------+--------------+------+--------+\n";
            std::cout << "| ID  | SKU     | Nama Alat Camping                 | Kategori           |  Harga/Hari  | Stok | Rating |\n";
            std::cout << "+-----+---------+-----------------------------------+--------------------+--------------+------+--------+\n";

            for (int i = startIndex; i < endIndex; ++i) {
                const auto& g = gears[i];
                std::ostringstream rateOss;
                rateOss << std::fixed << std::setprecision(1) << g.getRating() << "*";

                std::cout << "| " << padRight(std::to_string(g.getId()), 3)
                          << " | " << padRight(g.getSku(), 7)
                          << " | " << padRight(g.getName(), 33)
                          << " | " << padRight(categoryToString(g.getCategory()), 18)
                          << " | " << padLeft(formatRupiah(g.getRentalPricePerDay()), 12)
                          << " | " << padLeft(std::to_string(g.getStockAvailable()), 4)
                          << " | " << padRight(rateOss.str(), 6)
                          << " |\n";
            }
            std::cout << "+-----+---------+-----------------------------------+--------------------+--------------+------+--------+\n";
            std::cout << "  Halaman " << currentPage << " dari " << totalPages
                      << " (Menampilkan " << (endIndex - startIndex) << " dari total " << totalItems << " alat)\n";
        }

        static void displayGearDetailCard(const CampingGear& gear, const std::vector<ReviewRating>& reviews) {
            std::cout << "\n+=======================================================================================+\n";
            std::cout << "|                             DETAIL ALAT CAMPING LENGKAP                               |\n";
            std::cout << "+=======================================================================================+\n";
            std::cout << "  Kode SKU        : " << gear.getSku() << " (ID Sistem: " << gear.getId() << ")\n";
            std::cout << "  Nama Produk     : " << gear.getName() << "\n";
            std::cout << "  Merk / Brand    : " << gear.getBrand() << "\n";
            std::cout << "  Kategori        : " << categoryToString(gear.getCategory()) << "\n";
            std::cout << "  Biaya Sewa      : " << formatRupiah(gear.getRentalPricePerDay()) << " / hari\n";
            std::cout << "  Nilai Ganti Rugi: " << formatRupiah(gear.getReplacementValue()) << " (Jika Rusak/Hilang)\n";
            std::cout << "  Ketersediaan    : " << gear.getStockAvailable() << " unit siap sewa (Total Milik: " << gear.getStockTotal() << ")\n";
            std::cout << "  Rating Bintang  : " << std::fixed << std::setprecision(1) << gear.getRating()
                      << " / 5.0 (Berdasarkan " << gear.getReviewCount() << " ulasan, " << gear.getTimesRented() << " kali disewa)\n";
            std::cout << "  Spesifikasi     : " << gear.getSpecification() << "\n";
            std::cout << "  Kondisi Fisik   : " << gear.getConditionNotes() << "\n";
            std::cout << "+---------------------------------------------------------------------------------------+\n";
            std::cout << "  ULASAN PELANGGAN TERBARU:\n";
            if (reviews.empty()) {
                std::cout << "  (Belum ada ulasan untuk barang ini. Jadilah yang pertama memberikan ulasan!)\n";
            } else {
                for (size_t i = 0; i < std::min(size_t(3), reviews.size()); ++i) {
                    std::cout << "  [" << reviews[i].getReviewDate() << "] " << reviews[i].getUsername()
                              << " (Rating: " << reviews[i].getRatingValue() << "*): \""
                              << reviews[i].getReviewText() << "\"\n";
                }
            }
            std::cout << "+=======================================================================================+\n";
        }

        static void displayCartTable(const Cart& cart) {
            const auto& items = cart.getItems();
            if (items.empty()) {
                std::cout << "\n+---------------------------------------------------------------------------------------+\n";
                std::cout << "|                          KERANJANG PENYEWAAN ANDA MASIH KOSONG                        |\n";
                std::cout << "+---------------------------------------------------------------------------------------+\n";
                return;
            }

            std::cout << "\n+----+---------+-----------------------------------+--------------+--------+-----+--------------+\n";
            std::cout << "| No | SKU     | Nama Alat Camping                 |  Harga/Hari  | Durasi | Qty |   Subtotal   |\n";
            std::cout << "+----+---------+-----------------------------------+--------------+--------+-----+--------------+\n";

            int no = 1;
            for (const auto& it : items) {
                std::cout << "| " << padRight(std::to_string(no++), 2)
                          << " | " << padRight(it.getGearSku(), 7)
                          << " | " << padRight(it.getGearName(), 33)
                          << " | " << padLeft(formatRupiah(it.getPricePerDay()), 12)
                          << " | " << padRight(std::to_string(it.getRentalDays()) + " hr", 6)
                          << " | " << padRight(std::to_string(it.getQuantity()) + " bh", 3)
                          << " | " << padLeft(formatRupiah(it.calculateSubtotal()), 12)
                          << " |\n";
            }
            std::cout << "+----+---------+-----------------------------------+--------------+--------+-----+--------------+\n";
            std::cout << "| " << padLeft("TOTAL BIAYA SEWA : " + formatRupiah(cart.calculateSubtotal()), 85) << " |\n";
            std::cout << "+----+---------+-----------------------------------+--------------+--------+-----+--------------+\n";
        }

        static void displayTransactionListTable(const std::vector<RentalTransaction>& transactions) {
            if (transactions.empty()) {
                std::cout << "\n+---------------------------------------------------------------------------------------+\n";
                std::cout << "|                              BELUM ADA RIWAYAT TRANSAKSI                              |\n";
                std::cout << "+---------------------------------------------------------------------------------------+\n";
                return;
            }

            std::cout << "\n+------------------+------------+----------------------+--------------+-----------------------+\n";
            std::cout << "| Nomor Invoice    | Tgl Booking| Total Tagihan        | Status Bayar | Status Transaksi      |\n";
            std::cout << "+------------------+------------+----------------------+--------------+-----------------------+\n";

            for (const auto& tx : transactions) {
                std::cout << "| " << padRight(tx.getInvoiceNumber(), 16)
                          << " | " << padRight(tx.getBookingDate(), 10)
                          << " | " << padLeft(formatRupiah(tx.getTotalAmount()), 20)
                          << " | " << padRight(paymentStatusToString(tx.getPaymentStatus()), 12)
                          << " | " << padRight(transactionStatusToString(tx.getTransactionStatus()), 21)
                          << " |\n";
            }
            std::cout << "+------------------+------------+----------------------+--------------+-----------------------+\n";
        }

        static void displayOfficialInvoice(const RentalTransaction& tx) {
            std::cout << "\n+=======================================================================================+\n";
            std::cout << "|                       NOTA RESMI PENYEWAAN ALAT CAMPING                               |\n";
            std::cout << "|                          CAMPING GEAR RENTAL HUB 2026                                 |\n";
            std::cout << "+=======================================================================================+\n";
            std::cout << "  No. Invoice       : " << tx.getInvoiceNumber() << "\n";
            std::cout << "  Tanggal Booking   : " << tx.getBookingDate() << "\n";
            std::cout << "  Periode Sewa      : " << tx.getStartDate() << "  s/d  " << tx.getExpectedReturnDate() << "\n";
            std::cout << "  Nama Penyewa      : " << tx.getCustomerName() << "\n";
            std::cout << "  Nomor HP / WA     : " << tx.getCustomerPhone() << "\n";
            std::cout << "  Nomor KTP/KTM/SIM : " << tx.getCustomerIdCard() << "\n";
            std::cout << "  Metode Pembayaran : " << paymentMethodToString(tx.getPaymentMethod()) << "\n";
            std::cout << "  Status Pembayaran : " << paymentStatusToString(tx.getPaymentStatus()) << "\n";
            std::cout << "  Status Transaksi  : " << transactionStatusToString(tx.getTransactionStatus()) << "\n";
            std::cout << "+---------------------------------------------------------------------------------------+\n";
            std::cout << "  RINCIAN PERLENGKAPAN YANG DISEWA:\n";
            std::cout << "  -------------------------------------------------------------------------------------\n";
            std::cout << "  " << padRight("Nama Barang", 36) << padRight("Harga/Hr", 14) << padRight("Hari", 8)
                      << padRight("Qty", 6) << padLeft("Total", 16) << "\n";
            std::cout << "  -------------------------------------------------------------------------------------\n";

            for (const auto& it : tx.getItems()) {
                std::cout << "  " << padRight(it.getGearName(), 36)
                          << padRight(formatRupiah(it.getPricePerDay()), 14)
                          << padRight(std::to_string(it.getRentalDays()) + " hr", 8)
                          << padRight(std::to_string(it.getQuantity()) + " bh", 6)
                          << padLeft(formatRupiah(it.calculateSubtotal()), 16) << "\n";
            }
            std::cout << "  -------------------------------------------------------------------------------------\n";
            std::cout << "  " << padRight("Subtotal Biaya Sewa", 64) << padLeft(formatRupiah(tx.getSubtotal()), 20) << "\n";
            if (tx.getDiscountAmount() > 0.0) {
                std::cout << "  " << padRight("Potongan Diskon Promo (" + tx.getPromoCodeUsed() + ")", 64)
                          << padLeft("-" + formatRupiah(tx.getDiscountAmount()), 20) << "\n";
            }
            std::cout << "  " << padRight("Uang Jaminan / Deposit (Akan Dikembalikan)", 64)
                      << padLeft(formatRupiah(tx.getDepositAmount()), 20) << "\n";
            std::cout << "  =====================================================================================\n";
            std::cout << "  " << padRight("TOTAL PEMBAYARAN DI AWAL", 64)
                      << padLeft(formatRupiah(tx.getTotalAmount()), 20) << "\n";
            std::cout << "  =====================================================================================\n";

            if (tx.getLateFee() > 0.0 || tx.getDamagePenaltyFee() > 0.0) {
                std::cout << "  REKONSILIASI PENGEMBALIAN & DENDA:\n";
                std::cout << "    Tanggal Dikembalikan : " << tx.getActualReturnDate() << "\n";
                std::cout << "    Denda Keterlambatan  : " << formatRupiah(tx.getLateFee()) << "\n";
                std::cout << "    Denda Kerusakan      : " << formatRupiah(tx.getDamagePenaltyFee()) << "\n";
                std::cout << "    Deposit Dikembalikan : " << formatRupiah(tx.getDepositRefunded()) << "\n";
            }

            std::cout << "\n  Catatan Penyewa   : " << (tx.getNotes().empty() ? "-" : tx.getNotes()) << "\n";
            std::cout << "+---------------------------------------------------------------------------------------+\n";
            std::cout << "  PERNYATAAN & KESEPAKATAN SEWA:\n";
            std::cout << "  1. Penyewa telah memeriksa dan menerima seluruh peralatan dalam kondisi baik dan lengkap.\n";
            std::cout << "  2. Penyewa bersedia mengembalikan peralatan tepat waktu sesuai tanggal rencana kembali.\n";
            std::cout << "  3. Uang jaminan (deposit) akan dicairkan 100% setelah peralatan diverifikasi mulus oleh kasir.\n";
            std::cout << "  4. Apabila terjadi keterlambatan atau kerusakan, biaya akan dipotong langsung dari deposit.\n";
            std::cout << "  -------------------------------------------------------------------------------------\n";
            std::cout << "      Tanda Tangan Penyewa,                              Petugas Kasir Toko,           \n\n\n";
            std::cout << "   ( _______________________ )                        ( _______________________ )      \n";
            std::cout << "     " << padRight(tx.getCustomerName(), 25) << "                          Camping Hub Bandung         \n";
            std::cout << "+=======================================================================================+\n";
        }

        static void displayOfficialReturnReceipt(const RentalTransaction& tx, const ReturnLog& log) {
            std::cout << "\n+=======================================================================================+\n";
            std::cout << "|                       BERITA ACARA PENGEMBALIAN ALAT CAMPING                          |\n";
            std::cout << "+=======================================================================================+\n";
            std::cout << "  Nomor Dokumen Log : LOG-RET-" << log.getId() << "\n";
            std::cout << "  Nomor Invoice     : " << log.getInvoiceNumber() << "\n";
            std::cout << "  Nama Pelanggan    : " << tx.getCustomerName() << "\n";
            std::cout << "  Tenggat Kembali   : " << tx.getExpectedReturnDate() << "\n";
            std::cout << "  Realisasi Kembali : " << log.getReturnDate() << "\n";
            std::cout << "  Keterlambatan     : " << log.getOverdueDays() << " hari\n";
            std::cout << "  Admin Pemeriksa   : " << log.getInspectorAdminName() << "\n";
            std::cout << "+---------------------------------------------------------------------------------------+\n";
            std::cout << "  RINCIAN HASIL INSPEKSI KONDISI FISIK BARANG:\n";
            for (const auto& it : tx.getItems()) {
                std::cout << "  - " << padRight(it.getGearName(), 35)
                          << " Kondisi: " << padRight(returnConditionToString(it.getReturnCondition()), 18)
                          << " Denda: " << formatRupiah(it.getDamageFee()) << "\n";
            }
            std::cout << "+---------------------------------------------------------------------------------------+\n";
            std::cout << "  KALKULASI KEUANGAN AKHIR:\n";
            std::cout << "    Denda Telat (" << log.getOverdueDays() << " hari) : " << formatRupiah(log.getLateFeeCalculated()) << "\n";
            std::cout << "    Denda Kerusakan / Hilang: " << formatRupiah(log.getDamageFeeCalculated()) << "\n";
            std::cout << "    Total Denda             : " << formatRupiah(log.getTotalPenalty()) << "\n";
            std::cout << "    Uang Jaminan (Deposit)  : " << formatRupiah(log.getDepositHeld()) << "\n";
            std::cout << "    ---------------------------------------------------------------------------------\n";
            if (log.getDepositRefund() > 0) {
                std::cout << "    STATUS: UANG DEPOSIT DIKEMBALIKAN KE PELANGGAN SEBESAR : "
                          << formatRupiah(log.getDepositRefund()) << "\n";
            } else if (log.getExtraPaymentDue() > 0) {
                std::cout << "    STATUS: DEPOSIT TIDAK CUKUP. PELANGGAN WAJIB BAYAR LAGI: "
                          << formatRupiah(log.getExtraPaymentDue()) << "\n";
            } else {
                std::cout << "    STATUS: DEPOSIT DAN DENDA BERNILAI PAS. TIDAK ADA PENGEMBALIAN DANA.\n";
            }
            std::cout << "  Catatan Khusus    : " << log.getResolutionNotes() << "\n";
            std::cout << "+---------------------------------------------------------------------------------------+\n";
            std::cout << "  DOKUMEN INI ADALAH BUKTI SERAH TERIMA PENGEMBALIAN BARANG YANG SAH & BERKEKUATAN HUKUM.\n";
            std::cout << "  -------------------------------------------------------------------------------------\n";
            std::cout << "      Petugas Inspeksi Toko,                             Pelanggan / Penyewa,          \n\n\n";
            std::cout << "   ( _______________________ )                        ( _______________________ )      \n";
            std::cout << "     " << padRight(log.getInspectorAdminName(), 25) << "                          " << padRight(tx.getCustomerName(), 25) << "\n";
            std::cout << "+=======================================================================================+\n";
        }

        static void displayAdminGearTable(const std::vector<CampingGear>& gears) {
            std::cout << "\n+-----+---------+-----------------------------------+--------------------+--------------+------------+--------+--------+\n";
            std::cout << "| ID  | SKU     | Nama Alat Camping                 | Kategori           |  Harga/Hari  | Nilai Ganti| Stok S | Stok T |\n";
            std::cout << "+-----+---------+-----------------------------------+--------------------+--------------+------------+--------+--------+\n";
            for (const auto& g : gears) {
                std::cout << "| " << padRight(std::to_string(g.getId()), 3)
                          << " | " << padRight(g.getSku(), 7)
                          << " | " << padRight(g.getName(), 33)
                          << " | " << padRight(categoryToString(g.getCategory()), 18)
                          << " | " << padLeft(formatRupiah(g.getRentalPricePerDay()), 12)
                          << " | " << padLeft(formatRupiah(g.getReplacementValue()), 10)
                          << " | " << padLeft(std::to_string(g.getStockAvailable()), 6)
                          << " | " << padLeft(std::to_string(g.getStockTotal()), 6)
                          << " |\n";
            }
            std::cout << "+-----+---------+-----------------------------------+--------------------+--------------+------------+--------+--------+\n";
            std::cout << "  *Stok S = Stok Siap Sewa, Stok T = Stok Total Keseluruhan\n";
        }

        static void displayAdminCustomerTable(const std::vector<User>& users) {
            std::cout << "\n+-----+-----------------+---------------------------+-----------------+--------------+----------+--------+\n";
            std::cout << "| ID  | Username        | Nama Lengkap Pelanggan    | Nomor Telepon   | Status Akun  | Total Sew| Reputasi|\n";
            std::cout << "+-----+-----------------+---------------------------+-----------------+--------------+----------+--------+\n";
            for (const auto& u : users) {
                if (u.isAdmin()) continue;
                std::ostringstream rep;
                rep << std::fixed << std::setprecision(1) << u.getTrustScore();

                std::cout << "| " << padRight(std::to_string(u.getId()), 3)
                          << " | " << padRight(u.getUsername(), 15)
                          << " | " << padRight(u.getFullName(), 25)
                          << " | " << padRight(u.getPhoneNumber(), 15)
                          << " | " << padRight(userStatusToString(u.getStatus()), 12)
                          << " | " << padLeft(std::to_string(u.getTotalRentalsCompleted()), 8)
                          << " | " << padLeft(rep.str(), 6)
                          << " |\n";
            }
            std::cout << "+-----+-----------------+---------------------------+-----------------+--------------+----------+--------+\n";
        }

        static void displayFinancialDashboard(const FinancialKPI& kpi,
                                             const std::vector<std::pair<CampingGear, int>>& topGears,
                                             const std::vector<CampingGear>& lowStockGears) {
            std::cout << "\n+=======================================================================================+\n";
            std::cout << "|                 DASHBOARD EKSEKUTIF & LAPORAN KEUANGAN CAMPING HUB                    |\n";
            std::cout << "+=======================================================================================+\n";
            std::cout << "  +-----------------------------+-----------------------------+-----------------------+\n";
            std::cout << "  | TOTAL OMZET PENYEWAAN       | LABA BERSIH OPERASIONAL     | SEWA AKTIF BERJALAN   |\n";
            std::cout << "  | " << padRight(formatRupiah(kpi.totalGrossRevenue), 27)
                      << " | " << padRight(formatRupiah(kpi.totalNetProfit), 27)
                      << " | " << padRight(std::to_string(kpi.activeRentalsCount) + " Transaksi", 21) << " |\n";
            std::cout << "  +-----------------------------+-----------------------------+-----------------------+\n";
            std::cout << "  | TOTAL ALAT DIMILIKI         | UNIT SEDANG DISEWA          | TOTAL PELANGGAN       |\n";
            std::cout << "  | " << padRight(std::to_string(kpi.totalUnitsInStock) + " Unit Fisik", 27)
                      << " | " << padRight(std::to_string(kpi.totalUnitsRentedOut) + " Unit di Luar", 27)
                      << " | " << padRight(std::to_string(kpi.totalCustomersCount) + " Member Aktif", 21) << " |\n";
            std::cout << "  +-----------------------------+-----------------------------+-----------------------+\n";

            std::cout << "\n  RINCIAN KEUANGAN:\n";
            std::cout << "  - Pendapatan Sewa Bersih : " << formatRupiah(kpi.totalRentalIncome) << "\n";
            std::cout << "  - Total Diskon Diberikan : " << formatRupiah(kpi.totalDiscountGiven) << "\n";
            std::cout << "  - Uang Jaminan Ditahan   : " << formatRupiah(kpi.totalDepositHeld) << "\n";
            std::cout << "  - Deposit Dikembalikan   : " << formatRupiah(kpi.totalDepositRefunded) << "\n";
            std::cout << "  - Pendapatan Denda Telat : " << formatRupiah(kpi.totalLatePenaltyIncome) << "\n";
            std::cout << "  - Pendapatan Denda Rusak : " << formatRupiah(kpi.totalDamagePenaltyIncome) << "\n";

            std::cout << "\n  TOP 5 ALAT CAMPING PALING BANYAK DISEWA:\n";
            for (size_t i = 0; i < topGears.size(); ++i) {
                std::cout << "   " << (i + 1) << ". " << padRight(topGears[i].first.getName(), 38)
                          << " Disewa: " << topGears[i].second << " kali (Rating: "
                          << std::fixed << std::setprecision(1) << topGears[i].first.getRating() << "*)\n";
            }

            std::cout << "\n  PERINGATAN STOK ALAT MENIPIS (<= 3 Unit):\n";
            if (lowStockGears.empty()) {
                std::cout << "   [AMAN] Semua stok perlengkapan camping berada dalam batas aman.\n";
            } else {
                for (const auto& g : lowStockGears) {
                    std::cout << "   [PERINGATAN] SKU: " << g.getSku() << " | " << padRight(g.getName(), 30)
                              << " | Sisa: " << g.getStockAvailable() << " unit!\n";
                }
            }
            std::cout << "+=======================================================================================+\n";
        }

        static void displayBundleTable(const std::vector<CampingBundle>& bundles) {
            std::cout << "\n+=======================================================================================+\n";
            std::cout << "|                          DAFTAR PAKET HEMAT CAMPING SPESIAL                           |\n";
            std::cout << "+=======================================================================================+\n";
            for (const auto& b : bundles) {
                std::cout << "  [" << b.getId() << "] " << b.getCode() << " - " << b.getName() << "\n";
                std::cout << "      Deskripsi      : " << b.getTargetDescription() << "\n";
                std::cout << "      Harga Normal   : " << formatRupiah(b.getNormalDailyPrice()) << " / hari\n";
                std::cout << "      Harga Paket    : " << formatRupiah(b.getPackageDailyPrice()) << " / hari (Hemat "
                          << std::fixed << std::setprecision(0) << b.calculateSavingsPercentage() << "%)\n";
                std::cout << "      Uang Jaminan   : " << formatRupiah(b.getPackageDeposit()) << " (Deposit Jaminan)\n";
                std::cout << "      Item Termasuk  :\n";
                for (const auto& it : b.getBundleItems()) {
                    std::cout << "        * " << it.gearName << " (" << it.quantity << " unit)\n";
                }
                std::cout << "  -------------------------------------------------------------------------------------\n";
            }
            std::cout << "+=======================================================================================+\n";
        }

        static void printMainMenu() {
            printMainBanner();
            std::cout << ConsoleColor::BLUE
                      << "  SELAMAT DATANG DI APLIKASI PENYEWAAN ALAT CAMPING\n"
                      << "  -------------------------------------------------\n"
                      << ConsoleColor::RESET;
            std::cout << "  [1] Masuk ke Akun (Login)\n";
            std::cout << "  [2] Registrasi Akun Pelanggan Baru\n";
            std::cout << "  [3] Lihat Katalog Alat Camping (Mode Tamu/Guest)\n";
            std::cout << "  [4] Lihat Paket Hemat Camping Spesial (Bundling)\n";
            std::cout << "  [5] Syarat, Ketentuan, & Tata Cara Penyewaan\n";
            std::cout << "  [6] Panduan Fitur Lengkap & Bantuan Teknis Sistem\n";
            std::cout << "  [0] Keluar dari Aplikasi\n";
            std::cout << "  -------------------------------------------------\n";
        }

        static void printUserDashboardMenu(const std::string& customerName, int cartItemCount) {
            printMainBanner();
            std::cout << ConsoleColor::BLUE
                      << "  PANEL PELANGGAN | Halo, " << customerName << "!\n"
                      << "  ===================================================================\n"
                      << ConsoleColor::RESET;
            std::cout << "  [1] Katalog Alat Camping (Cari, Filter, Urutkan, & Sewa)\n";
            std::cout << "  [2] Paket Hemat Camping Spesial (Bundling Siap Pakai)\n";
            std::cout << "  [3] Keranjang Penyewaan [" << cartItemCount << " Item]\n";
            std::cout << "  [4] Transaksi & Checkout (Pilih Tanggal, Kupon Promo & Pembayaran)\n";
            std::cout << "  [5] Konfirmasi & Instruksi Pembayaran\n";
            std::cout << "  [6] Riwayat Penyewaan & Cetak Nota Sewa\n";
            std::cout << "  [7] Berikan Ulasan & Rating Alat yang Pernah Disewa\n";
            std::cout << "  [8] Profil Pengguna, Status Loyalitas & Ganti Password\n";
            std::cout << "  [0] Keluar / Logout Sesi\n";
            std::cout << "  ===================================================================\n";
        }

        static void printAdminDashboardMenu(const std::string& adminName) {
            printMainBanner();
            std::cout << ConsoleColor::BLUE
                      << "  PANEL ADMINISTRATOR TOKO | Administrator: " << adminName << "\n"
                      << "  ===================================================================\n"
                      << ConsoleColor::RESET;
            std::cout << "  [1] Manajemen Inventaris Alat (Tambah, Edit, Restock, Hapus, Cari)\n";
            std::cout << "  [2] Manajemen Transaksi Sewa (Verifikasi Bayar, Serah Unit, Filter)\n";
            std::cout << "  [3] Pengembalian Alat & Kalkulasi Denda (Inspeksi Fisik & Deposit)\n";
            std::cout << "  [4] Manajemen Pelanggan (Lihat Member, Reputasi, Blokir/Buka Blokir)\n";
            std::cout << "  [5] Dashboard Bisnis & Laporan Keuangan Lengkap\n";
            std::cout << "  [6] Export Laporan Keuangan ke Berkas Teks (.txt)\n";
            std::cout << "  [0] Keluar / Logout Administrator\n";
            std::cout << "  ===================================================================\n";
        }

        static void printCatalogSubMenu() {
            std::cout << "\n  PILIHAN NAVIGASI KATALOG:\n";
            std::cout << "  [1] Urutkan Katalog (Sorting: Harga, Nama, Rating, Stok)\n";
            std::cout << "  [2] Cari Alat Camping (Searching Kata Kunci / Filter Kategori)\n";
            std::cout << "  [3] Lihat Rincian Spesifikasi & Ulasan Barang (Detail Card)\n";
            std::cout << "  [4] Masukkan Barang ke Keranjang Sewa\n";
            std::cout << "  [5] Reset Filter & Tampilkan Semua\n";
            std::cout << "  [0] Kembali ke Menu Sebelumnya\n";
        }

        static void printRentalRules() {
            std::cout << "\n+=======================================================================================+\n";
            std::cout << "|                 SYARAT & KETENTUAN PENYEWAAN ALAT CAMPING                             |\n";
            std::cout << "+=======================================================================================+\n";
            std::cout << "  1. Identitas Wajib:\n";
            std::cout << "     Penyewa wajib menitipkan/menyerahkan identitas asli yang masih berlaku (KTP/KTM/SIM).\n";
            std::cout << "  2. Uang Jaminan (Deposit):\n";
            std::cout << "     Deposit dihitung 10% dari total nilai penggantian barang (minimal Rp 50.000).\n";
            std::cout << "     Uang jaminan akan DIKEMBALIKAN PENUH 100% apabila barang kembali tepat waktu dan mulus.\n";
            std::cout << "  3. Durasi Sewa & Keterlambatan:\n";
            std::cout << "     Durasi dihitung per hari (24 jam). Keterlambatan pengembalian tanpa konfirmasi dikenakan\n";
            std::cout << "     denda sebesar 150% dari tarif sewa harian per hari keterlambatan.\n";
            std::cout << "  4. Kondisi Perlengkapan:\n";
            std::cout << "     - Kotor berat (lumpur pekat): Denda 10% nilai barang (biaya laundry khusus).\n";
            std::cout << "     - Rusak ringan (sobek kecil/pasak bengkok): Denda 25% nilai barang.\n";
            std::cout << "     - Rusak berat (frame patah/robek besar): Denda 60% nilai barang.\n";
            std::cout << "     - Hilang: Wajib mengganti 100% nilai penggantian barang.\n";
            std::cout << "  5. Kupon Promo:\n";
            std::cout << "     - 'CAMPINGASIK10' : Diskon 10% hingga Rp 50.000\n";
            std::cout << "     - 'WEEKENDSERU20' : Diskon 20% hingga Rp 100.000\n";
            std::cout << "     - 'MEMBERBARU'    : Diskon flat Rp 25.000\n";
            std::cout << "+=======================================================================================+\n";
        }

        static void displayHelpGuide() {
            std::cout << "\n+=======================================================================================+\n";
            std::cout << "|                 PANDUAN LENGKAP PENGGUNAAN FITUR SISTEM PENYEWAAN CAMPING             |\n";
            std::cout << "+=======================================================================================+\n";
            std::cout << "  1. FITUR PENGGUNA (USER):\n";
            std::cout << "     * Registrasi & Login  : Daftarkan akun baru dengan username dan NIK valid. Login untuk sesi.\n";
            std::cout << "     * Katalog Alat Outdoor: Cari barang berdasarkan kata kunci, filter kategori, atau urutkan\n";
            std::cout << "                             berdasarkan harga, nama A-Z, rating bintang, dan ketersediaan stok.\n";
            std::cout << "     * Keranjang Sewa      : Masukkan alat camping ke keranjang, atur kuantitas dan durasi sewa.\n";
            std::cout << "     * Paket Bundling      : Dapatkan harga hemat dengan paket komplit (Solo, Duo, Squad, Glamping).\n";
            std::cout << "     * Transaksi & Checkout: Pilih tanggal mulai sewa, gunakan kode voucher promo (cth: 'CAMPINGASIK10'),\n";
            std::cout << "                             dan pilih metode bayar (BCA, Mandiri, BRI, QRIS, E-Wallet, Cash COD).\n";
            std::cout << "     * Konfirmasi Bayar    : Masukkan nomor resi / referensi transfer untuk diverifikasi kasir.\n";
            std::cout << "     * Riwayat & Struk     : Lihat status transaksi (Menunggu, Dibayar, Disewa, Kembali) dan cetak nota.\n";
            std::cout << "     * Ulasan & Rating     : Berikan testimoni bintang 1-5 untuk alat yang telah selesai disewa.\n";
            std::cout << "     * Loyalitas Member    : Dapatkan kenaikan tingkat (Bronze, Silver, Gold, Platinum) dan diskon sewa.\n\n";
            std::cout << "  2. FITUR ADMINISTRATOR (ADMIN):\n";
            std::cout << "     * Manajemen Inventaris: CRUD inventaris alat camping, penambahan unit baru, edit harga sewa,\n";
            std::cout << "                             restock unit fisik, serta penghapusan alat yang tidak aktif.\n";
            std::cout << "     * Manajemen Transaksi : Verifikasi pembayaran pelanggan, serahkan unit fisik ke penyewa saat\n";
            std::cout << "                             pengambilan di outlet, batalkan pesanan sewa, dan cari transaksi.\n";
            std::cout << "     * Pengembalian & Denda: Proses inspeksi fisik alat saat kembali (Baik, Kotor, Rusak Ringan,\n";
            std::cout << "                             Rusak Berat, Hilang). Sistem otomatis menghitung denda keterlambatan\n";
            std::cout << "                             dan denda kerusakan, serta mencairkan sisa uang jaminan (deposit).\n";
            std::cout << "     * Manajemen Pelanggan : Pantau seluruh member terdaftar, reputasi trust score, dan kontrol blokir.\n";
            std::cout << "     * Dashboard Bisnis    : Pantau metrik KPI omzet kotor, laba bersih, sewa aktif, top 5 alat terlaris,\n";
            std::cout << "                             peringatan stok kritis, dan export laporan keuangan ke berkas teks.\n";
            std::cout << "+=======================================================================================+\n";
        }
    };

    // ================================================================================================
    // [BAGIAN 7: CONTROLLER & ORKESTRASI UTAMA (APP CONTROLLER)]
    // ================================================================================================

    class AppController {
    private:
        DataManager dataManager;
        AuthService authService;
        CatalogService catalogService;
        CartService cartService;
        RentalService rentalService;
        PaymentService paymentService;
        ReturnService returnService;
        AdminService adminService;
        ReportFinanceService reportFinanceService;
        BundleService bundleService;

        bool isRunning;

        void handleLogin();
        void handleRegister();
        void handleGuestCatalog();
        void handleUserDashboard();
        void handleUserCatalog();
        void handleUserBundles();
        void handleUserCart();
        void handleUserCheckout();
        void handleUserPayment();
        void handleUserRentalHistory();
        void handleUserReview();
        void handleUserProfile();

        void handleAdminDashboard();
        void handleAdminInventory();
        void handleAdminAddGear();
        void handleAdminEditGear();
        void handleAdminRestockGear();
        void handleAdminDeleteGear();
        void handleAdminTransactions();
        void handleAdminReturnGear();
        void handleAdminCustomerManagement();
        void handleAdminDashboardAnalytics();
        void handleAdminExportReport();

    public:
        AppController();
        explicit AppController(const std::string& dataDirectory);
        ~AppController();

        void run();
        void shutdown();
        void runSelfDiagnostics();
    };

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
          bundleService(dataManager),
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
          bundleService(dataManager),
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
            int choice = UIViews::readInt("  Pilih Menu [0-6]: ", 0, 6);

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
                case 4: {
                    UIViews::displayBundleTable(bundleService.getAllBundles());
                    UIViews::pauseScreen();
                    break;
                }
                case 5:
                    UIViews::printRentalRules();
                    UIViews::pauseScreen();
                    break;
                case 6:
                    UIViews::displayHelpGuide();
                    UIViews::pauseScreen();
                    break;
                case 0:
                    UIViews::printInfoBox("Terima kasih telah menggunakan Sistem Penyewaan Alat Camping. Sampai jumpa di alam bebas!");
                    isRunning = false;
                    break;
            }
        }
    }

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

    void AppController::handleUserDashboard() {
        bool userActive = true;
        while (userActive && authService.isAuthenticated()) {
            UIViews::printUserDashboardMenu(authService.getCurrentSession().fullName, cartService.getTotalItemCount());
            int choice = UIViews::readInt("  Pilih Menu Pelanggan [0-8]: ", 0, 8);

            switch (choice) {
                case 1:
                    handleUserCatalog();
                    break;
                case 2:
                    handleUserBundles();
                    break;
                case 3:
                    handleUserCart();
                    break;
                case 4:
                    handleUserCheckout();
                    break;
                case 5:
                    handleUserPayment();
                    break;
                case 6:
                    handleUserRentalHistory();
                    break;
                case 7:
                    handleUserReview();
                    break;
                case 8:
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

    void AppController::handleUserBundles() {
        while (true) {
            UIViews::printSectionHeader("PAKET HEMAT CAMPING SPESIAL (BUNDLING SIAP PAKAI)");
            UIViews::displayBundleTable(bundleService.getAllBundles());

            std::cout << "\n  PILIHAN AKSI PAKET BUNDLE:\n";
            std::cout << "  [1] Masukkan Paket Hemat ke Keranjang Sewa\n";
            std::cout << "  [0] Kembali ke Menu Pelanggan\n";

            int act = UIViews::readInt("  Pilih Aksi [0-1]: ", 0, 1);
            if (act == 0) break;

            if (act == 1) {
                int bId = UIViews::readInt("  Masukkan Nomor ID Paket Bundle [1-4]: ", 1, 4);
                int days = UIViews::readInt("  Durasi sewa paket (hari): ", 1, 30);
                ServiceResult res = bundleService.addBundleToCart(cartService, bId, days);
                if (res.success) {
                    UIViews::printSuccessBox(res.message);
                } else {
                    UIViews::printErrorBox(res.message);
                }
                UIViews::pauseScreen();
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

        std::cout << "\n  VOUCHER PROMO TERSEDIA:\n";
        std::cout << "  * 'CAMPINGASIK10' : Diskon 10% s.d Rp 50.000\n";
        std::cout << "  * 'WEEKENDSERU20' : Diskon 20% s.d Rp 100.000 (Min. Transaksi Rp 200.000)\n";
        std::cout << "  * 'MEMBERBARU'    : Diskon Flat Rp 25.000\n";
        std::string promo = UIViews::readString("  Masukkan Kode Promo (Tekan enter jika tidak ada): ", true);

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
            UIViews::printInfoBox("Pembayaran Anda sedang dalam proses verifikasi oleh kasir.");
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

    // ================================================================================================
    // [BAGIAN 8: SELF-DIAGNOSTICS / AUTOMATED UNIT TESTING RUNNER]
    // ================================================================================================

    void AppController::runSelfDiagnostics() {
        std::cout << "\n=========================================================================================\n";
        std::cout << "     MENJALANKAN AUTOMATED UNIT TESTING & INTEGRATION DIAGNOSTICS (ALL LAYERS)           \n";
        std::cout << "=========================================================================================\n";

        // 1. Uji Data Layer
        std::cout << "[TEST DATA LAYER] Memeriksa inisialisasi entitas User & Gear...\n";
        assert(dataManager.getAllUsers().size() >= 4);
        assert(dataManager.getAllGears().size() >= 20);
        std::cout << "-> [PASS] Data Layer OK.\n";

        // 2. Uji Algoritma Sorting & Searching
        std::cout << "[TEST ALGORITHMS] Memeriksa MergeSort, QuickSort, BubbleSort, & Binary Search...\n";
        auto gears = dataManager.getAllGears();
        AlgorithmEngine::mergeSortGears(gears, 0, static_cast<int>(gears.size()) - 1, GearSortField::PRICE_ASC);
        assert(gears.front().getRentalPricePerDay() <= gears.back().getRentalPricePerDay());

        // Pengurutan SKU sebelum Binary Search
        std::sort(gears.begin(), gears.end(), [](const CampingGear& a, const CampingGear& b) {
            return a.getSku() < b.getSku();
        });
        int idx = AlgorithmEngine::binarySearchGearBySku(gears, gears[0].getSku());
        assert(idx != -1);
        assert(gears[idx].getSku() == gears[0].getSku());

        // QuickSort Transaksi
        auto txs = dataManager.getAllTransactions();
        if (!txs.empty()) {
            AlgorithmEngine::quickSortTransactions(txs, 0, static_cast<int>(txs.size()) - 1, TransactionSortField::TOTAL_ASC);
            assert(txs.front().getTotalAmount() <= txs.back().getTotalAmount());
        }

        // Linear Search
        auto foundTenda = AlgorithmEngine::linearSearchGears(dataManager.getAllGears(), "Eiger");
        assert(!foundTenda.empty());
        std::cout << "-> [PASS] Algoritma Sorting & Searching OK.\n";

        // 3. Uji Business Logic Kalkulasi
        std::cout << "[TEST BUSINESS LOGIC] Memeriksa kalkulasi sewa, denda, & promo...\n";
        double disc = rentalService.calculatePromoDiscount("CAMPINGASIK10", 200000);
        assert(disc == 20000);

        double lateFee = ReturnService::calculateLateFeePerDay(50000);
        assert(lateFee == 75000);

        double dirtyFee = ReturnService::calculateConditionPenalty(1000000, ReturnCondition::DIRTY_HEAVY);
        assert(dirtyFee == 100000);
        double minorFee = ReturnService::calculateConditionPenalty(1000000, ReturnCondition::MINOR_DAMAGE);
        assert(minorFee == 250000);
        double majorFee = ReturnService::calculateConditionPenalty(1000000, ReturnCondition::MAJOR_DAMAGE);
        assert(majorFee == 600000);
        double lostFee = ReturnService::calculateConditionPenalty(1000000, ReturnCondition::LOST);
        assert(lostFee == 1000000);
        std::cout << "-> [PASS] Aturan Bisnis & Kalkulasi Denda OK.\n";

        // 4. Uji Paket Bundle & Loyalitas Member
        std::cout << "[TEST BUNDLE & LOYALTY] Memeriksa Paket Bundle & Tingkat Member...\n";
        assert(bundleService.getAllBundles().size() >= 4);
        CampingBundle* bnd1 = bundleService.findBundleById(1);
        assert(bnd1 != nullptr);
        assert(bnd1->calculateSavingsPerDay() > 0);

        MembershipTier t1 = calculateUserTier(2, 95.0);
        assert(t1 == MembershipTier::BRONZE_EXPLORER);
        MembershipTier t2 = calculateUserTier(5, 90.0);
        assert(t2 == MembershipTier::SILVER_ADVENTURER);
        MembershipTier t3 = calculateUserTier(8, 92.0);
        assert(t3 == MembershipTier::GOLD_MOUNTAINEER);
        MembershipTier t4 = calculateUserTier(12, 95.0);
        assert(t4 == MembershipTier::PLATINUM_EXPEDITION);
        std::cout << "-> [PASS] Paket Bundle & Loyalty Tier OK.\n";

        // 5. Uji UI Formatter
        std::cout << "[TEST UI FORMATTER] Memeriksa Rupiah Formatter...\n";
        assert(UIViews::formatRupiah(150000) == "Rp 150.000");
        assert(UIViews::formatRupiah(1250000) == "Rp 1.250.000");
        assert(UIViews::formatRupiah(-25000) == "-Rp 25.000");
        std::cout << "-> [PASS] UI Formatter OK.\n";

        std::cout << "\n=========================================================================================\n";
        std::cout << "     SELURUH SUBSISTEM TERINTEGRASI 100% STABIL DAN LOLOS DIAGNOSTIK!                    \n";
        std::cout << "=========================================================================================\n\n";
    }

} // namespace CampingRental

// ====================================================================================================
// [BAGIAN 9: MAIN ENTRY POINT]
// ====================================================================================================

int main(int argc, char* argv[]) {
    // Mode testing jika dijalankan dengan argumen --test
    if (argc > 1 && std::string(argv[1]) == "--test") {
        CampingRental::AppController tester("data");
        tester.runSelfDiagnostics();
        return 0;
    }

    // Eksekusi aplikasi normal
    CampingRental::AppController app("data");
    app.run();
    return 0;
}
