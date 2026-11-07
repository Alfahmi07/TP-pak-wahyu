/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - DATA LAYER (ANGGOTA 1)
 * File: data_manager.cpp
 * Deskripsi: Implementasi class DataManager untuk persistensi berkas (.txt),
 *            penyimpanan array/vector, CRUD operasi, dan initial seed data.
 * Penulis: Anggota 1 (Data Layer Specialist)
 * ============================================================================
 */

#include "data_manager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <filesystem>

namespace CampingRental {

    DataManager::DataManager() {
        setDataDirectory("data");
    }

    DataManager::DataManager(const std::string& dataDirectory) {
        setDataDirectory(dataDirectory);
    }

    DataManager::~DataManager() {
        // Auto-save data saat objek dimusnahkan
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

        // Jika data masih kosong sama sekali (pertama kali dijalankan), lakukan seeding otomatis
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

    // -------------------------------------------------------------
    // LOAD & SAVE USERS
    // -------------------------------------------------------------
    bool DataManager::loadUsersFromFile() {
        users.clear();
        std::ifstream file(usersFilePath);
        if (!file.is_open()) {
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            User u = User::deserialize(line);
            if (u.getId() > 0) {
                users.push_back(u);
            }
        }
        file.close();
        return true;
    }

    bool DataManager::saveUsersToFile() {
        std::ofstream file(usersFilePath);
        if (!file.is_open()) {
            return false;
        }
        file << "# Database Pengguna Sistem Penyewaan Alat Camping\n";
        file << "# Format: ID|Username|PasswordHash|FullName|Phone|Email|IDCard|Address|Role|Status|RentalsCount|TrustScore|RegDate\n";
        for (const auto& u : users) {
            file << u.serialize() << "\n";
        }
        file.close();
        return true;
    }

    // -------------------------------------------------------------
    // LOAD & SAVE GEARS
    // -------------------------------------------------------------
    bool DataManager::loadGearsFromFile() {
        gears.clear();
        std::ifstream file(gearsFilePath);
        if (!file.is_open()) {
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            CampingGear g = CampingGear::deserialize(line);
            if (g.getId() > 0) {
                gears.push_back(g);
            }
        }
        file.close();
        return true;
    }

    bool DataManager::saveGearsToFile() {
        std::ofstream file(gearsFilePath);
        if (!file.is_open()) {
            return false;
        }
        file << "# Database Inventaris Alat Camping\n";
        file << "# Format: ID|SKU|Name|Brand|Category|PricePerDay|ReplacementValue|StockTotal|StockAvail|TimesRented|Rating|ReviewCount|Spec|Notes\n";
        for (const auto& g : gears) {
            file << g.serialize() << "\n";
        }
        file.close();
        return true;
    }

    // -------------------------------------------------------------
    // LOAD & SAVE TRANSACTIONS
    // -------------------------------------------------------------
    bool DataManager::loadTransactionsFromFile() {
        transactions.clear();
        std::ifstream file(transactionsFilePath);
        if (!file.is_open()) {
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            RentalTransaction tx = RentalTransaction::deserialize(line);
            if (!tx.getInvoiceNumber().empty()) {
                transactions.push_back(tx);
            }
        }
        file.close();
        return true;
    }

    bool DataManager::saveTransactionsToFile() {
        std::ofstream file(transactionsFilePath);
        if (!file.is_open()) {
            return false;
        }
        file << "# Database Transaksi Penyewaan Alat Camping\n";
        file << "# Format: Invoice|UserId|CustName|CustPhone|IdCard|BookDate|StartDate|ExpReturnDate|ActReturnDate|Subtotal|Disc|Promo|Deposit|Total|PayMethod|PayStat|TrxStat|ProofRef|LateFee|DamageFee|DepositRefund|Notes|ItemsList\n";
        for (const auto& tx : transactions) {
            file << tx.serialize() << "\n";
        }
        file.close();
        return true;
    }

    // -------------------------------------------------------------
    // LOAD & SAVE RETURN LOGS
    // -------------------------------------------------------------
    bool DataManager::loadReturnLogsFromFile() {
        returnLogs.clear();
        std::ifstream file(returnsFilePath);
        if (!file.is_open()) {
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            ReturnLog rl = ReturnLog::deserialize(line);
            if (rl.getId() > 0) {
                returnLogs.push_back(rl);
            }
        }
        file.close();
        return true;
    }

    bool DataManager::saveReturnLogsToFile() {
        std::ofstream file(returnsFilePath);
        if (!file.is_open()) {
            return false;
        }
        file << "# Database Log Pengembalian dan Denda\n";
        file << "# Format: ID|Invoice|ReturnDate|OverdueDays|LateFee|DamageFee|TotalPenalty|DepositHeld|DepositRefund|ExtraPaymentDue|Admin|Notes\n";
        for (const auto& rl : returnLogs) {
            file << rl.serialize() << "\n";
        }
        file.close();
        return true;
    }

    // -------------------------------------------------------------
    // LOAD & SAVE REVIEWS
    // -------------------------------------------------------------
    bool DataManager::loadReviewsFromFile() {
        reviews.clear();
        std::ifstream file(reviewsFilePath);
        if (!file.is_open()) {
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            ReviewRating r = ReviewRating::deserialize(line);
            if (r.getId() > 0) {
                reviews.push_back(r);
            }
        }
        file.close();
        return true;
    }

    bool DataManager::saveReviewsToFile() {
        std::ofstream file(reviewsFilePath);
        if (!file.is_open()) {
            return false;
        }
        file << "# Database Ulasan dan Rating Produk\n";
        file << "# Format: ID|GearId|UserId|Username|Rating|ReviewText|ReviewDate\n";
        for (const auto& r : reviews) {
            file << r.serialize() << "\n";
        }
        file.close();
        return true;
    }

    // -------------------------------------------------------------
    // CRUD IMPLEMENTASI: USER
    // -------------------------------------------------------------
    const std::vector<User>& DataManager::getAllUsers() const {
        return users;
    }

    std::vector<User>& DataManager::getAllUsersRef() {
        return users;
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
        if (findUserByUsername(user.getUsername()) != nullptr) {
            return false;
        }
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

    // -------------------------------------------------------------
    // CRUD IMPLEMENTASI: CAMPING GEAR
    // -------------------------------------------------------------
    const std::vector<CampingGear>& DataManager::getAllGears() const {
        return gears;
    }

    std::vector<CampingGear>& DataManager::getAllGearsRef() {
        return gears;
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
        if (findGearBySku(gear.getSku()) != nullptr) {
            return false;
        }
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

    // -------------------------------------------------------------
    // CRUD IMPLEMENTASI: TRANSAKSI
    // -------------------------------------------------------------
    const std::vector<RentalTransaction>& DataManager::getAllTransactions() const {
        return transactions;
    }

    std::vector<RentalTransaction>& DataManager::getAllTransactionsRef() {
        return transactions;
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
            if (tx.getUserId() == userId) {
                result.push_back(tx);
            }
        }
        return result;
    }

    std::vector<RentalTransaction> DataManager::findTransactionsByStatus(TransactionStatus status) const {
        std::vector<RentalTransaction> result;
        for (const auto& tx : transactions) {
            if (tx.getTransactionStatus() == status) {
                result.push_back(tx);
            }
        }
        return result;
    }

    bool DataManager::addTransaction(const RentalTransaction& tx) {
        if (findTransactionByInvoice(tx.getInvoiceNumber()) != nullptr) {
            return false;
        }
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

    // -------------------------------------------------------------
    // CRUD IMPLEMENTASI: RETURN LOG
    // -------------------------------------------------------------
    const std::vector<ReturnLog>& DataManager::getAllReturnLogs() const {
        return returnLogs;
    }

    std::vector<ReturnLog>& DataManager::getAllReturnLogsRef() {
        return returnLogs;
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

    // -------------------------------------------------------------
    // CRUD IMPLEMENTASI: REVIEWS
    // -------------------------------------------------------------
    const std::vector<ReviewRating>& DataManager::getAllReviews() const {
        return reviews;
    }

    std::vector<ReviewRating> DataManager::getReviewsByGearId(int gearId) const {
        std::vector<ReviewRating> result;
        for (const auto& r : reviews) {
            if (r.getGearId() == gearId) {
                result.push_back(r);
            }
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

    // -------------------------------------------------------------
    // SEED INITIAL DATA (DATA AWAL LENGKAP & REALISTIS)
    // -------------------------------------------------------------
    void DataManager::seedInitialUsers() {
        users.clear();
        // Admin default (password: admin123)
        users.push_back(User(1, "admin", "admin123", "Administrator Utama", "081122334455",
                             "admin@campingrental.id", "3271010101900001", "Jl. Rinjani No. 10, Bandung",
                             UserRole::ADMIN, UserStatus::ACTIVE, 0, 100.0, "2026-01-01"));

        // User Demo 1: Budi Santoso (password: budi123)
        users.push_back(User(2, "budi", "budi123", "Budi Santoso", "081234567890",
                             "budi@gmail.com", "3271011505950002", "Jl. Dago Asri No. 45, Bandung",
                             UserRole::USER, UserStatus::ACTIVE, 3, 98.5, "2026-02-15"));

        // User Demo 2: Siti Aminah (password: siti123)
        users.push_back(User(3, "siti", "siti123", "Siti Aminah", "081398765432",
                             "siti.outdoor@yahoo.com", "3271014208980003", "Jl. Gejayan No. 12, Sleman",
                             UserRole::USER, UserStatus::ACTIVE, 5, 100.0, "2026-03-10"));

        // User Demo 3: Andi Wijaya (password: andi123)
        users.push_back(User(4, "andi", "andi123", "Andi Wijaya", "081765432109",
                             "andi.wijaya@outlook.com", "3271012304990004", "Jl. Dipati Ukur No. 88, Bandung",
                             UserRole::USER, UserStatus::ACTIVE, 1, 95.0, "2026-05-20"));
    }

    void DataManager::seedInitialGears() {
        gears.clear();
        // Kategori Tenda & Shelter
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
            "Kapasitas 4 orang 1 kamar + 1 ruang santai, teknologi adem gelap", "Terawat, minus sablon pudar sedikit"));

        // Kategori Sleeping Gear
        gears.push_back(CampingGear(5, "SLP-001", "Sleeping Bag Arei Everest Polar", "Arei",
            GearCategory::SLEEPING_GEAR, 15000, 220000, 15, 12, 45, 4.6, 28,
            "Lapisan dalam polar bulu hangat, limit suhu 5 derajat Celcius", "Harum laundry, ritsleting lancar"));

        gears.push_back(CampingGear(6, "SLP-002", "Sleeping Bag Deuter Orbit +5 Mummy", "Deuter",
            GearCategory::SLEEPING_GEAR, 25000, 750000, 8, 7, 20, 4.9, 14,
            "Model kepompong insulasi thermal sintetis high grade, berat 1kg", "Kondisi prima seperti baru"));

        gears.push_back(CampingGear(7, "SLP-003", "Matras Alumunium Foil Double Side", "Wildshell",
            GearCategory::SLEEPING_GEAR, 7500, 65000, 20, 18, 55, 4.5, 30,
            "Reflektor panas tubuh, busa empuk 4mm, waterproof", "Bagus, lipatan rapi"));

        // Kategori Carrier & Tas
        gears.push_back(CampingGear(8, "BAG-001", "Carrier Osprey Atmos AG 65L", "Osprey",
            GearCategory::CARRIER_BAG, 60000, 3400000, 5, 4, 18, 5.0, 16,
            "Anti-Gravity suspension backsystem, include raincover asli", "Sangat mulus, busa punggung tebal"));

        gears.push_back(CampingGear(9, "BAG-002", "Carrier Consina Tarebbi 60L", "Consina",
            GearCategory::CARRIER_BAG, 30000, 650000, 12, 9, 40, 4.6, 25,
            "Bahan cordura tahan banting, kompartemen utama luas", "Kuat, siap jelajah hutan"));

        gears.push_back(CampingGear(10, "BAG-003", "Carrier Eiger Rhinos 60L", "Eiger",
            GearCategory::CARRIER_BAG, 35000, 950000, 10, 8, 35, 4.8, 20,
            "Sistem sirkulasi udara tropic flow, buckle acetal tangguh", "Terawat baik, warna cerah"));

        // Kategori Alat Masak & Kompor
        gears.push_back(CampingGear(11, "CKG-001", "Kompor Ultralight Windproof Kovar", "Kovar",
            GearCategory::COOKING_EQUIPMENT, 12000, 130000, 15, 13, 50, 4.7, 32,
            "Pelindung angin model bunga mekar, pemantik piezo elektrik", "Api biru stabil, bersih"));

        gears.push_back(CampingGear(12, "CKG-002", "Nesting Cooking Set DS-308 (4-5 Org)", "Alocs",
            GearCategory::COOKING_EQUIPMENT, 20000, 260000, 10, 8, 38, 4.8, 26,
            "3 panci aluminium anodized + 1 teko 0.8L + mangkok & spons", "Lengkap dengan tas jaring, non-stick"));

        gears.push_back(CampingGear(13, "CKG-003", "Gas Kaleng Portable Hi-Cook 230g", "Hi-Cook",
            GearCategory::COOKING_EQUIPMENT, 15000, 25000, 30, 26, 80, 4.9, 45,
            "Gas butana murni bersegel pabrik untuk kompor portable", "Segel baru 100%"));

        // Kategori Penerangan & Lentera
        gears.push_back(CampingGear(14, "LGT-001", "Headlamp Petzl Tikka 350 Lumens", "Petzl",
            GearCategory::LIGHTING, 20000, 450000, 10, 9, 28, 4.9, 19,
            "3 level cahaya putih + mode merah malam, waterproof IPX4", "Baterai awet, strap elastis"));

        gears.push_back(CampingGear(15, "LGT-002", "Lentera Tenda LED Rechargeable Solar", "Naturehike",
            GearCategory::LIGHTING, 15000, 180000, 12, 10, 33, 4.7, 21,
            "Bisa dicharge USB Type-C & matahari, fungsi powerbank darurat", "Terang benderang, baterai sehat"));

        // Kategori Navigasi & Safety
        gears.push_back(CampingGear(16, "SFT-001", "Trekking Pole Duralumin Naturehike", "Naturehike",
            GearCategory::NAVIGATION_SAFETY, 10000, 150000, 16, 13, 42, 4.8, 24,
            "Material duralumin 7075 kuat & ringan, pengunci quick lock", "Peredam getar normal, ujung karbida"));

        gears.push_back(CampingGear(17, "SFT-002", "HT Walkie Talkie Baofeng UV-5R Dual Band", "Baofeng",
            GearCategory::NAVIGATION_SAFETY, 25000, 350000, 8, 6, 22, 4.8, 15,
            "Jangkauan 3-5 km di alam terbuka, include headset & charger", "Frekuensi jernih, baterai tahan lama"));

        gears.push_back(CampingGear(18, "SFT-003", "Kotak P3K Outdoor First Aid Kit Lengkap", "OutdoorMedic",
            GearCategory::NAVIGATION_SAFETY, 10000, 120000, 12, 11, 25, 4.9, 17,
            "Perban elastis, plester, povidone, alkohol pads, gunting, mitela", "Steril, isi obat kadaluarsa 2028"));

        // Kategori Outdoor Comfort
        gears.push_back(CampingGear(19, "CFT-001", "Hammock Double Layer Ticket to the Moon", "TTTM",
            GearCategory::OUTDOOR_COMFORT, 20000, 320000, 10, 8, 36, 4.9, 27,
            "Kain sutra parasut elastis tahan beban 200kg, include webbing", "Bahan lembut dan bersih"));

        gears.push_back(CampingGear(20, "CFT-002", "Kursi Lipat Duralumin Ultralight", "Dhaulagiri",
            GearCategory::OUTDOOR_COMFORT, 15000, 195000, 14, 11, 40, 4.7, 28,
            "Kapasitas beban 120kg, rangka aluminium alloy lipat ringkas", "Kokoh, tidak goyang"));

        gears.push_back(CampingGear(21, "CFT-003", "Meja Lipat Aluminium Portable Camping", "Speed",
            GearCategory::OUTDOOR_COMFORT, 20000, 240000, 8, 6, 26, 4.8, 18,
            "Meja gulung aluminium ukuran 56x40 cm, anti karat & panas", "Stabil, engsel berfungsi sempurna"));
    }

    void DataManager::seedInitialTransactions() {
        transactions.clear();

        // Transaksi 1: Transaksi Selesai (Budi Santoso)
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
        tx1.setTotalAmount(194000); // 160000 - 16000 + 50000
        tx1.setPaymentMethod(PaymentMethod::TRANSFER_BANK_BCA);
        tx1.setPaymentStatus(PaymentStatus::VERIFIED_PAID);
        tx1.setTransactionStatus(TransactionStatus::RETURNED_COMPLETED);
        tx1.setPaymentProofRef("TRX-BCA-98124412");
        tx1.setLateFee(0);
        tx1.setDamagePenaltyFee(0);
        tx1.setDepositRefunded(50000);
        tx1.setNotes("Penyewaan gunung Papandayan lancar, barang kembali bersih.");
        transactions.push_back(tx1);

        // Transaksi 2: Sedang Disewa (Siti Aminah)
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
        tx2.setSubtotal(351000); // (45k*3) + (60k*3) + (12k*3) = 135k + 180k + 36k
        tx2.setDiscountAmount(50000);
        tx2.setPromoCodeUsed("WEEKENDSERU20");
        tx2.setDepositAmount(100000);
        tx2.setTotalAmount(401000); // 351000 - 50000 + 100000
        tx2.setPaymentMethod(PaymentMethod::EWALLET_QRIS);
        tx2.setPaymentStatus(PaymentStatus::VERIFIED_PAID);
        tx2.setTransactionStatus(TransactionStatus::ACTIVE_RENTED);
        tx2.setPaymentProofRef("QRIS-NMD-87623199");
        tx2.setNotes("Penyewaan Gunung Prau 3 hari.");
        transactions.push_back(tx2);

        // Transaksi 3: Menunggu Pembayaran (Andi Wijaya)
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

} // namespace CampingRental
