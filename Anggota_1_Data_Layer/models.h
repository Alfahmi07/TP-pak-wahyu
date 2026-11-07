/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - DATA LAYER (ANGGOTA 1)
 * File: models.h
 * Deskripsi: Definisi seluruh entitas data, enum, dan struktur objek model
 *            yang merepresentasikan entitas bisnis dalam sistem penyewaan alat camping.
 * Penulis: Anggota 1 (Data Layer Specialist)
 * ============================================================================
 */

#ifndef MODELS_H
#define MODELS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>

namespace CampingRental {

    // ========================================================================
    // ENUMERASI SISTEM
    // ========================================================================

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
        TENDA,              // Tenda Dome, Tenda Kapasitas Besar, Flysheet
        SLEEPING_GEAR,      // Sleeping bag, Matras spons, Matras angin, Bantal tiup
        CARRIER_BAG,        // Tas Carrier 45L, 60L, 80L, Daypack
        COOKING_EQUIPMENT,  // Kompor ultralight, Nesting cookset, Gas, Spatula
        LIGHTING,           // Headlamp, Lentera tenda, Senter sorot
        NAVIGATION_SAFETY,  // Kompas, P3K, Survival kit, HT Baofeng, Trekking pole
        OUTDOOR_COMFORT,    // Meja lipat, Kursi lipat, Hammock double, Tarp
        OTHER               // Aksesoris pelengkap lainnya
    };

    enum class TransactionStatus {
        PENDING_PAYMENT,    // Menunggu pembayaran
        WAITING_CONFIRMATION,// Menunggu konfirmasi admin
        PAID_CONFIRMED,     // Pembayaran lunas, siap diambil
        ACTIVE_RENTED,      // Alat sedang dibawa penyewa
        RETURNED_COMPLETED, // Pengembalian selesai normal
        RETURNED_WITH_ISSUE,// Pengembalian dengan denda/kerusakan/keterlambatan
        CANCELLED           // Dibatalkan
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
        GOOD_CONDITION,     // 100% Bagus (Denda 0%)
        DIRTY_HEAVY,        // Kotor berat butuh laundry khusus (Denda 10%)
        MINOR_DAMAGE,       // Rusak ringan: sobek kecil, pasak bengkok (Denda 25%)
        MAJOR_DAMAGE,       // Rusak berat: frame patah, sobek besar (Denda 60%)
        LOST                // Hilang total (Ganti 100% nilai barang)
    };

    // ========================================================================
    // FUNGSI BANTUAN ENUM KE STRING
    // ========================================================================

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
            case TransactionStatus::PENDING_PAYMENT:     return "MENUNGGU PEMBAYARAN";
            case TransactionStatus::WAITING_CONFIRMATION: return "MENUNGGU KONFIRMASI";
            case TransactionStatus::PAID_CONFIRMED:      return "DIBAYAR (SIAP DIAMBIL)";
            case TransactionStatus::ACTIVE_RENTED:       return "SEDANG DISEWA";
            case TransactionStatus::RETURNED_COMPLETED:  return "SELESAI (KEMBALI)";
            case TransactionStatus::RETURNED_WITH_ISSUE: return "SELESAI (ADA DENDA)";
            case TransactionStatus::CANCELLED:           return "DIBATALKAN";
            default:                                     return "STATUS TIDAK DIKETAHUI";
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

    // ========================================================================
    // MODEL ENTITAS: USER
    // ========================================================================
    class User {
    private:
        int id;
        std::string username;
        std::string passwordHash;
        std::string fullName;
        std::string phoneNumber;
        std::string email;
        std::string identityCardNumber; // KTP / KTM / Paspor
        std::string address;
        UserRole role;
        UserStatus status;
        int totalRentalsCompleted;
        double trustScore; // 0.0 - 100.0
        std::string registeredAt;

    public:
        User()
            : id(0), username(""), passwordHash(""), fullName(""),
              phoneNumber(""), email(""), identityCardNumber(""),
              address(""), role(UserRole::USER), status(UserStatus::ACTIVE),
              totalRentalsCompleted(0), trustScore(100.0), registeredAt("") {}

        User(int uId, const std::string& uName, const std::string& pwd,
             const std::string& name, const std::string& phone,
             const std::string& mail, const std::string& idCard,
             const std::string& addr, UserRole uRole, UserStatus uStat,
             int rentals = 0, double trust = 100.0, const std::string& regDate = "")
            : id(uId), username(uName), passwordHash(pwd), fullName(name),
              phoneNumber(phone), email(mail), identityCardNumber(idCard),
              address(addr), role(uRole), status(uStat),
              totalRentalsCompleted(rentals), trustScore(trust), registeredAt(regDate) {}

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

        // Serialisasi & Deserialisasi File (.txt)
        std::string serialize() const {
            std::string reg = registeredAt.empty() ? "-" : registeredAt;
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

    // ========================================================================
    // MODEL ENTITAS: CAMPING GEAR (ALAT CAMPING)
    // ========================================================================
    class CampingGear {
    private:
        int id;
        std::string sku;            // Kode unik barang, cth: "TND-001"
        std::string name;           // Cth: "Tenda Eiger Guardian 4P"
        std::string brand;          // Cth: "Eiger", "Consina", "Naturehike"
        GearCategory category;
        double rentalPricePerDay;   // Cth: 45000
        double replacementValue;    // Cth: 950000 (untuk denda hilang/rusak)
        int stockTotal;             // Total unit yang dimiliki toko
        int stockAvailable;         // Unit yang siap disewa saat ini
        int timesRented;            // Statistik berapa kali disewa
        double rating;              // Rata-rata bintang (1.0 - 5.0)
        int reviewCount;            // Jumlah ulasan
        std::string specification;  // Spesifikasi teknis ringkas
        std::string conditionNotes; // Catatan kondisi fisik barang

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

        // Getters
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

        // Setters
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

        // Operasi Stok
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

        // Serialisasi & Deserialisasi
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

    // ========================================================================
    // MODEL ITEM DALAM TRANSAKSI & KERANJANG
    // ========================================================================
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

        // Serialisasi ringkas untuk disimpan dalam transaksi
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

    // ========================================================================
    // MODEL KERANJANG PENYEWAAN (USER CART)
    // ========================================================================
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

    // ========================================================================
    // MODEL KODE PROMO / VOUCHER
    // ========================================================================
    class PromoCode {
    private:
        std::string code;
        double discountPercentage; // Cth: 10.0 (%)
        double maxDiscountAmount;   // Cth: 50000
        double minTransactionAmount;// Cth: 100000
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

    // ========================================================================
    // MODEL ENTITAS: TRANSAKSI PENYEWAAN
    // ========================================================================
    class RentalTransaction {
    private:
        std::string invoiceNumber;     // Cth: "INV-20261107-1001"
        int userId;
        std::string customerName;
        std::string customerPhone;
        std::string customerIdCard;
        std::string bookingDate;       // Tanggal pemesanan dibuat
        std::string startDate;         // Tanggal awal sewa (ambil)
        std::string expectedReturnDate;// Tanggal rencana kembali
        std::string actualReturnDate;  // Tanggal barang dikembalikan
        std::vector<RentalItem> items;
        double subtotal;
        double discountAmount;
        std::string promoCodeUsed;
        double depositAmount;          // Uang jaminan sewa
        double totalAmount;            // Total yang harus dibayar saat booking
        PaymentMethod paymentMethod;
        PaymentStatus paymentStatus;
        TransactionStatus transactionStatus;
        std::string paymentProofRef;   // Ref transfer / no resi
        double lateFee;                // Denda telat jika ada
        double damagePenaltyFee;       // Denda kerusakan jika ada
        double depositRefunded;        // Sisa deposit yang dikembalikan
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

        // Getters
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

        // Setters
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

        // Serialisasi & Deserialisasi
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

            // Serialisasi sub-items yang digabung dengan tanda semikolon (;)
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

            // Deserialisasi items jika ada
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

    // ========================================================================
    // MODEL LOG PENGEMBALIAN & DENDA (RETURN LOG)
    // ========================================================================
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
        double extraPaymentDue; // Jika denda > deposit
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

        // Getters
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

        // Setters
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

    // ========================================================================
    // MODEL ULASAN & RATING (REVIEW)
    // ========================================================================
    class ReviewRating {
    private:
        int id;
        int gearId;
        int userId;
        std::string username;
        int ratingValue; // 1 s/d 5
        std::string reviewText;
        std::string reviewDate;

    public:
        ReviewRating()
            : id(0), gearId(0), userId(0), username(""), ratingValue(5),
              reviewText(""), reviewDate("") {}

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

    // ========================================================================
    // MODEL STRUKTUR LAPORAN KEUANGAN & KPI DASHBOARD
    // ========================================================================
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

} // namespace CampingRental

#endif // MODELS_H
