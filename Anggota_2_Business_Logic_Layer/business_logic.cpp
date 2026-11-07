/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - BUSINESS LOGIC LAYER (ANGGOTA 2)
 * File: business_logic.cpp
 * Deskripsi: Implementasi algoritma Sorting (MergeSort, QuickSort, BubbleSort),
 *            Searching (Binary Search, Linear Search), validasi input,
 *            aturan sewa, kalkulasi biaya & denda, dan layanan bisnis.
 * Penulis: Anggota 2 (Business Logic Specialist)
 * ============================================================================
 */

#include "business_logic.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <algorithm>
#include <cmath>

namespace CampingRental {

    // ========================================================================
    // IMPLEMENTASI ALGORITMA SORTING & SEARCHING
    // ========================================================================

    void AlgorithmEngine::merge(std::vector<CampingGear>& arr, int left, int mid, int right, GearSortField sortField) {
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

    void AlgorithmEngine::mergeSortGears(std::vector<CampingGear>& arr, int left, int right, GearSortField sortField) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortGears(arr, left, mid, sortField);
            mergeSortGears(arr, mid + 1, right, sortField);
            merge(arr, left, mid, right, sortField);
        }
    }

    int AlgorithmEngine::partition(std::vector<RentalTransaction>& arr, int low, int high, TransactionSortField sortField) {
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

    void AlgorithmEngine::quickSortTransactions(std::vector<RentalTransaction>& arr, int low, int high, TransactionSortField sortField) {
        if (low < high) {
            int pi = partition(arr, low, high, sortField);
            quickSortTransactions(arr, low, pi - 1, sortField);
            quickSortTransactions(arr, pi + 1, high, sortField);
        }
    }

    void AlgorithmEngine::bubbleSortReviews(std::vector<ReviewRating>& arr, bool newestFirst) {
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

    int AlgorithmEngine::binarySearchGearBySku(const std::vector<CampingGear>& sortedGears, const std::string& targetSku) {
        int low = 0;
        int high = static_cast<int>(sortedGears.size()) - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (sortedGears[mid].getSku() == targetSku) {
                return mid;
            }
            if (sortedGears[mid].getSku() < targetSku) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return -1;
    }

    std::vector<CampingGear> AlgorithmEngine::linearSearchGears(const std::vector<CampingGear>& gears,
                                                               const std::string& keyword,
                                                               int categoryFilter,
                                                               double maxPrice,
                                                               bool onlyAvailable) {
        std::vector<CampingGear> result;

        // Helper lowercase
        auto toLower = [](const std::string& str) -> std::string {
            std::string lower = str;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            return lower;
        };

        std::string kwLower = toLower(keyword);

        for (const auto& g : gears) {
            // Filter Kategori
            if (categoryFilter >= 0 && static_cast<int>(g.getCategory()) != categoryFilter) {
                continue;
            }

            // Filter Harga Maksimum
            if (maxPrice > 0.0 && g.getRentalPricePerDay() > maxPrice) {
                continue;
            }

            // Filter Ketersediaan Stok
            if (onlyAvailable && g.getStockAvailable() <= 0) {
                continue;
            }

            // Pencarian Kata Kunci (Nama, SKU, Brand, Spesifikasi)
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

    std::vector<RentalTransaction> AlgorithmEngine::linearSearchTransactions(const std::vector<RentalTransaction>& txs,
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

    // ========================================================================
    // SERVICE: AUTHENTICATION SERVICE IMPLEMENTATION
    // ========================================================================

    AuthService::AuthService(DataManager& dm) : dataManager(dm) {}

    std::string AuthService::hashPassword(const std::string& rawPassword) {
        // Algoritma hashing FNV-1a dengan salt konsisten untuk keamanan
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

    bool AuthService::validateEmailFormat(const std::string& email) {
        size_t atPos = email.find('@');
        size_t dotPos = email.rfind('.');
        return (atPos != std::string::npos && dotPos != std::string::npos && atPos < dotPos && atPos > 0);
    }

    bool AuthService::validatePhoneFormat(const std::string& phone) {
        if (phone.length() < 10 || phone.length() > 15) return false;
        for (char c : phone) {
            if (!std::isdigit(c)) return false;
        }
        return true;
    }

    bool AuthService::validateUsernameFormat(const std::string& username) {
        if (username.length() < 4 || username.length() > 20) return false;
        for (char c : username) {
            if (!std::isalnum(c) && c != '_') return false;
        }
        return true;
    }

    ServiceResult AuthService::registerUser(const std::string& username, const std::string& password,
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

        // Cek duplikasi
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

    ServiceResult AuthService::login(const std::string& username, const std::string& password) {
        User* user = dataManager.findUserByUsername(username);
        if (user == nullptr) {
            return ServiceResult::fail("Username tidak ditemukan!");
        }

        // Verifikasi password (plain atau hash)
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

    void AuthService::logout() {
        currentSession.clear();
    }

    ServiceResult AuthService::changePassword(const std::string& oldPassword, const std::string& newPassword) {
        if (!isAuthenticated()) {
            return ServiceResult::fail("Anda belum login!");
        }
        User* user = dataManager.findUserById(currentSession.userId);
        if (!user) return ServiceResult::fail("User tidak ditemukan!");

        if (user->getPasswordHash() != oldPassword &&
            user->getPasswordHash() != hashPassword(oldPassword)) {
            return ServiceResult::fail("Password lama salah!");
        }

        if (newPassword.length() < 6) {
            return ServiceResult::fail("Password baru minimal 6 karakter!");
        }

        user->setPasswordHash(newPassword);
        dataManager.saveUsersToFile();
        return ServiceResult::ok("Password berhasil diperbarui!");
    }

    ServiceResult AuthService::updateProfile(const std::string& fullName, const std::string& phone,
                                            const std::string& email, const std::string& address) {
        if (!isAuthenticated()) {
            return ServiceResult::fail("Anda belum login!");
        }
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

    // ========================================================================
    // SERVICE: CATALOG SERVICE IMPLEMENTATION
    // ========================================================================

    CatalogService::CatalogService(DataManager& dm) : dataManager(dm) {}

    std::vector<CampingGear> CatalogService::getCatalogList(GearSortField sortField,
                                                           int categoryFilter,
                                                           const std::string& searchKeyword,
                                                           double maxPrice,
                                                           bool onlyAvailable) {
        const std::vector<CampingGear>& allGears = dataManager.getAllGears();

        // 1. Searching & Filtering dengan Algoritma Linear Search
        std::vector<CampingGear> filtered = AlgorithmEngine::linearSearchGears(
            allGears, searchKeyword, categoryFilter, maxPrice, onlyAvailable
        );

        // 2. Sorting dengan Algoritma MergeSort
        if (!filtered.empty()) {
            AlgorithmEngine::mergeSortGears(filtered, 0, static_cast<int>(filtered.size()) - 1, sortField);
        }

        return filtered;
    }

    CampingGear* CatalogService::getGearDetail(int gearId) {
        return dataManager.findGearById(gearId);
    }

    CampingGear* CatalogService::getGearBySku(const std::string& sku) {
        return dataManager.findGearBySku(sku);
    }

    std::vector<ReviewRating> CatalogService::getGearReviews(int gearId) {
        std::vector<ReviewRating> revs = dataManager.getReviewsByGearId(gearId);
        AlgorithmEngine::bubbleSortReviews(revs, true); // Terurut dari terbaru
        return revs;
    }

    ServiceResult CatalogService::submitReview(int gearId, int userId, const std::string& username,
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

        return ServiceResult::ok("Ulasan Anda berhasil dikirim! Terima kasih atas feedback-nya.");
    }

    // ========================================================================
    // SERVICE: CART SERVICE IMPLEMENTATION
    // ========================================================================

    CartService::CartService(DataManager& dm) : dataManager(dm) {}

    void CartService::initializeForUser(int userId) {
        userCart.setUserId(userId);
        userCart.clear();
    }

    ServiceResult CartService::addToCart(int gearId, int quantity, int rentalDays) {
        if (quantity <= 0) return ServiceResult::fail("Jumlah unit harus lebih dari 0!");
        if (rentalDays <= 0) return ServiceResult::fail("Durasi sewa minimal 1 hari!");

        CampingGear* gear = dataManager.findGearById(gearId);
        if (!gear) return ServiceResult::fail("Barang tidak ditemukan dalam inventaris!");

        // Hitung total quantity yang sudah ada di keranjang untuk barang ini
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

    ServiceResult CartService::updateItemQuantity(int gearId, int newQuantity) {
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

    ServiceResult CartService::removeItem(int gearId) {
        if (userCart.removeItem(gearId)) {
            return ServiceResult::ok("Item berhasil dihapus dari keranjang.");
        }
        return ServiceResult::fail("Item tidak ditemukan di keranjang.");
    }

    void CartService::clearCart() {
        userCart.clear();
    }

    int CartService::getTotalItemCount() const {
        return userCart.getTotalItemCount();
    }

    double CartService::calculateCartSubtotal() const {
        return userCart.calculateSubtotal();
    }

    double CartService::calculateCartEstimatedDeposit() const {
        // Kalkulasi deposit: 10% dari total nilai penggantian barang, minimal Rp 50.000
        double totalReplVal = 0.0;
        for (const auto& it : userCart.getItems()) {
            totalReplVal += (it.getReplacementValue() * it.getQuantity());
        }
        double est = totalReplVal * 0.10;
        return std::max(50000.0, std::round(est / 5000.0) * 5000.0);
    }

    // ========================================================================
    // SERVICE: RENTAL & CHECKOUT SERVICE IMPLEMENTATION
    // ========================================================================

    RentalService::RentalService(DataManager& dm) : dataManager(dm) {
        initializePromos();
    }

    void RentalService::initializePromos() {
        availablePromos.clear();
        availablePromos.push_back(PromoCode("CAMPINGASIK10", 10.0, 50000.0, 100000.0, true,
            "Diskon 10% hingga Rp 50.000 untuk sewa di atas Rp 100.000"));
        availablePromos.push_back(PromoCode("WEEKENDSERU20", 20.0, 100000.0, 200000.0, true,
            "Diskon 20% hingga Rp 100.000 untuk petualangan seru minimal Rp 200.000"));
        availablePromos.push_back(PromoCode("MEMBERBARU", 15.0, 25000.0, 50000.0, true,
            "Diskon spesial 15% maksimal Rp 25.000 untuk semua pelanggan baru"));
    }

    PromoCode* RentalService::findPromoCode(const std::string& code) {
        for (auto& p : availablePromos) {
            if (p.getCode() == code && p.getIsActive()) return &p;
        }
        return nullptr;
    }

    double RentalService::calculatePromoDiscount(const std::string& code, double subtotal) {
        PromoCode* promo = findPromoCode(code);
        if (promo) {
            return promo->calculateDiscount(subtotal);
        }
        return 0.0;
    }

    std::string RentalService::getCurrentDateString() {
        return "2026-11-07";
    }

    std::string RentalService::addDaysToDate(const std::string& dateStr, int days) {
        // Asumsi format YYYY-MM-DD
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

    int RentalService::calculateDaysBetween(const std::string& startDate, const std::string& endDate) {
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

    ServiceResult RentalService::processCheckout(int userId,
                                                const std::string& customerName,
                                                const std::string& customerPhone,
                                                const std::string& customerIdCard,
                                                const std::string& startDate,
                                                int rentalDurationDays,
                                                const std::vector<RentalItem>& items,
                                                PaymentMethod paymentMethod,
                                                const std::string& promoCode,
                                                const std::string& orderNotes) {
        if (items.empty()) {
            return ServiceResult::fail("Keranjang sewa masih kosong!");
        }
        if (rentalDurationDays <= 0) {
            return ServiceResult::fail("Durasi sewa minimal 1 hari!");
        }
        if (customerName.empty() || customerPhone.empty() || customerIdCard.empty()) {
            return ServiceResult::fail("Data pelanggan dan identitas (KTP) wajib lengkap!");
        }

        // 1. Validasi stok semua item sebelum dieksekusi
        for (const auto& item : items) {
            CampingGear* gear = dataManager.findGearById(item.getGearId());
            if (!gear) {
                return ServiceResult::fail("Alat dengan ID " + std::to_string(item.getGearId()) + " tidak ditemukan!");
            }
            if (gear->getStockAvailable() < item.getQuantity()) {
                return ServiceResult::fail("Stok barang '" + gear->getName() + "' tidak mencukupi saat ini!");
            }
        }

        // 2. Kalkulasi biaya
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
            if (discount > 0.0) {
                usedCode = promoCode;
            }
        }

        double deposit = std::max(50000.0, std::round((totalReplVal * 0.10) / 5000.0) * 5000.0);
        double totalAmount = (subtotal - discount) + deposit;

        // 3. Potong stok alat
        for (const auto& item : items) {
            CampingGear* gear = dataManager.findGearById(item.getGearId());
            if (gear) {
                gear->rentUnits(item.getQuantity());
            }
        }
        dataManager.saveGearsToFile();

        // 4. Buat objek transaksi
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
            // Update stats user jika ada
            User* user = dataManager.findUserById(userId);
            if (user) {
                user->incrementRentalCount();
                dataManager.saveUsersToFile();
            }
            return ServiceResult::ok(invoice);
        }

        return ServiceResult::fail("Gagal menyimpan transaksi ke berkas!");
    }

    std::vector<RentalTransaction> RentalService::getUserTransactions(int userId) {
        std::vector<RentalTransaction> list = dataManager.findTransactionsByUserId(userId);
        AlgorithmEngine::quickSortTransactions(list, 0, static_cast<int>(list.size()) - 1, TransactionSortField::DATE_DESC);
        return list;
    }

    RentalTransaction* RentalService::getTransactionDetail(const std::string& invoiceNumber) {
        return dataManager.findTransactionByInvoice(invoiceNumber);
    }

    // ========================================================================
    // SERVICE: PAYMENT SERVICE IMPLEMENTATION
    // ========================================================================

    PaymentService::PaymentService(DataManager& dm) : dataManager(dm) {}

    std::string PaymentService::generatePaymentInstructions(PaymentMethod method, double totalAmount,
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

    ServiceResult PaymentService::submitPaymentProof(const std::string& invoiceNumber, const std::string& proofRef) {
        RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
        if (!tx) return ServiceResult::fail("Invoice tidak ditemukan!");

        tx->setPaymentProofRef(proofRef);
        tx->setPaymentStatus(PaymentStatus::PENDING_VERIFICATION);
        tx->setTransactionStatus(TransactionStatus::WAITING_CONFIRMATION);
        dataManager.saveTransactionsToFile();

        return ServiceResult::ok("Bukti pembayaran berhasil dikirim! Menunggu konfirmasi admin.");
    }

    ServiceResult PaymentService::confirmPaymentByAdmin(const std::string& invoiceNumber, const std::string& adminName) {
        RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
        if (!tx) return ServiceResult::fail("Invoice tidak ditemukan!");

        tx->setPaymentStatus(PaymentStatus::VERIFIED_PAID);
        tx->setTransactionStatus(TransactionStatus::PAID_CONFIRMED);
        tx->setNotes(tx->getNotes() + " | Diverifikasi oleh " + adminName);
        dataManager.saveTransactionsToFile();

        return ServiceResult::ok("Pembayaran invoice " + invoiceNumber + " berhasil diverifikasi dan disetujui!");
    }

    // ========================================================================
    // SERVICE: RETURN SERVICE IMPLEMENTATION
    // ========================================================================

    ReturnService::ReturnService(DataManager& dm) : dataManager(dm) {}

    double ReturnService::calculateLateFeePerDay(double itemPricePerDay) {
        // Denda keterlambatan adalah 1.5x dari harga sewa harian
        return itemPricePerDay * 1.5;
    }

    double ReturnService::calculateConditionPenalty(double replacementValue, ReturnCondition condition) {
        switch (condition) {
            case ReturnCondition::GOOD_CONDITION:
                return 0.0;
            case ReturnCondition::DIRTY_HEAVY:
                // Laundry khusus / pembersihan mendalam
                return replacementValue * 0.10;
            case ReturnCondition::MINOR_DAMAGE:
                // Sobek kecil, resleting macet, pasak bengkok
                return replacementValue * 0.25;
            case ReturnCondition::MAJOR_DAMAGE:
                // Frame patah, kain robek besar, bocor parah
                return replacementValue * 0.60;
            case ReturnCondition::LOST:
                // Ganti 100% nilai barang
                return replacementValue * 1.00;
            default:
                return 0.0;
        }
    }

    ServiceResult ReturnService::processReturnInspection(const std::string& invoiceNumber,
                                                        const std::string& actualReturnDate,
                                                        const std::vector<ReturnCondition>& itemConditions,
                                                        const std::string& adminInspectorName,
                                                        const std::string& notes,
                                                        ReturnLog& outReturnLog) {
        RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
        if (!tx) return ServiceResult::fail("Transaksi tidak ditemukan!");

        if (tx->getTransactionStatus() == TransactionStatus::RETURNED_COMPLETED ||
            tx->getTransactionStatus() == TransactionStatus::RETURNED_WITH_ISSUE) {
            return ServiceResult::fail("Transaksi ini sudah pernah diproses pengembaliannya sebelumnya!");
        }

        // 1. Hitung hari keterlambatan
        int overdueDays = RentalService::calculateDaysBetween(tx->getExpectedReturnDate(), actualReturnDate);
        if (overdueDays < 0) overdueDays = 0; // Tepat waktu / lebih awal

        double totalLateFee = 0.0;
        double totalDamageFee = 0.0;

        std::vector<RentalItem>& items = tx->getItemsRef();
        for (size_t i = 0; i < items.size(); ++i) {
            ReturnCondition cond = (i < itemConditions.size()) ? itemConditions[i] : ReturnCondition::GOOD_CONDITION;
            items[i].setReturnCondition(cond);

            // Denda keterlambatan per item
            if (overdueDays > 0) {
                totalLateFee += (calculateLateFeePerDay(items[i].getPricePerDay()) * items[i].getQuantity() * overdueDays);
            }

            // Denda kondisi barang
            double dmg = calculateConditionPenalty(items[i].getReplacementValue(), cond) * items[i].getQuantity();
            items[i].setDamageFee(dmg);
            totalDamageFee += dmg;

            // 2. Pulihkan stok barang jika tidak hilang total
            CampingGear* gear = dataManager.findGearById(items[i].getGearId());
            if (gear) {
                if (cond != ReturnCondition::LOST) {
                    gear->returnUnits(items[i].getQuantity());
                } else {
                    // Jika hilang, kurangi stockTotal unit toko
                    gear->setStockTotal(std::max(0, gear->getStockTotal() - items[i].getQuantity()));
                }
            }
        }
        dataManager.saveGearsToFile();

        // 3. Rekonsiliasi Deposit
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

        // 4. Update transaksi
        tx->setActualReturnDate(actualReturnDate);
        tx->setLateFee(totalLateFee);
        tx->setDamagePenaltyFee(totalDamageFee);
        tx->setDepositRefunded(depositRefund);
        tx->setTransactionStatus(totalPenalty > 0 ? TransactionStatus::RETURNED_WITH_ISSUE : TransactionStatus::RETURNED_COMPLETED);
        dataManager.saveTransactionsToFile();

        // 5. Catat Log Pengembalian Resmi
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

    // ========================================================================
    // SERVICE: ADMIN SERVICE IMPLEMENTATION
    // ========================================================================

    AdminService::AdminService(DataManager& dm) : dataManager(dm) {}

    ServiceResult AdminService::addNewGear(const std::string& name, const std::string& brand,
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

    ServiceResult AdminService::updateGearDetails(int gearId, const std::string& name, const std::string& brand,
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

    ServiceResult AdminService::restockGear(int gearId, int additionalStock) {
        if (additionalStock <= 0) return ServiceResult::fail("Jumlah restock harus lebih dari 0!");
        CampingGear* gear = dataManager.findGearById(gearId);
        if (!gear) return ServiceResult::fail("Alat tidak ditemukan!");

        gear->setStockTotal(gear->getStockTotal() + additionalStock);
        gear->setStockAvailable(gear->getStockAvailable() + additionalStock);
        dataManager.saveGearsToFile();

        return ServiceResult::ok("Restock berhasil! Stok total sekarang: " + std::to_string(gear->getStockTotal()));
    }

    ServiceResult AdminService::removeGear(int gearId) {
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

    ServiceResult AdminService::toggleUserStatus(int userId, UserStatus newStatus) {
        User* user = dataManager.findUserById(userId);
        if (!user) return ServiceResult::fail("Pengguna tidak ditemukan!");
        if (user->isAdmin()) return ServiceResult::fail("Tidak dapat mengubah status akun Administrator!");

        user->setStatus(newStatus);
        dataManager.saveUsersToFile();
        return ServiceResult::ok("Status pengguna berhasil diubah menjadi: " + userStatusToString(newStatus));
    }

    ServiceResult AdminService::updateUserTrustScore(int userId, double scoreDelta) {
        User* user = dataManager.findUserById(userId);
        if (!user) return ServiceResult::fail("Pengguna tidak ditemukan!");

        user->setTrustScore(user->getTrustScore() + scoreDelta);
        dataManager.saveUsersToFile();
        return ServiceResult::ok("Skor reputasi pelanggan diperbarui.");
    }

    ServiceResult AdminService::updateTransactionStatus(const std::string& invoiceNumber, TransactionStatus newStatus) {
        RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
        if (!tx) return ServiceResult::fail("Transaksi tidak ditemukan!");

        tx->setTransactionStatus(newStatus);
        dataManager.saveTransactionsToFile();
        return ServiceResult::ok("Status transaksi diperbarui menjadi: " + transactionStatusToString(newStatus));
    }

    ServiceResult AdminService::markTransactionAsPickedUp(const std::string& invoiceNumber) {
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

    ServiceResult AdminService::cancelTransaction(const std::string& invoiceNumber, const std::string& reason) {
        RentalTransaction* tx = dataManager.findTransactionByInvoice(invoiceNumber);
        if (!tx) return ServiceResult::fail("Transaksi tidak ditemukan!");

        if (tx->getTransactionStatus() == TransactionStatus::ACTIVE_RENTED ||
            tx->getTransactionStatus() == TransactionStatus::RETURNED_COMPLETED) {
            return ServiceResult::fail("Tidak dapat membatalkan transaksi yang sedang berjalan atau sudah selesai!");
        }

        // Kembalikan stok barang yang sebelumnya dipesan
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

    // ========================================================================
    // SERVICE: REPORT & FINANCE SERVICE IMPLEMENTATION
    // ========================================================================

    ReportFinanceService::ReportFinanceService(DataManager& dm) : dataManager(dm) {}

    FinancialKPI ReportFinanceService::calculateOverallFinancialKPI() {
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
            if (tx.getTransactionStatus() == TransactionStatus::CANCELLED) {
                continue;
            }

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

        // Keuntungan bersih = pendapatan sewa + pendapatan denda
        kpi.totalNetProfit = kpi.totalRentalIncome + kpi.totalLatePenaltyIncome + kpi.totalDamagePenaltyIncome;

        return kpi;
    }

    std::vector<std::pair<CampingGear, int>> ReportFinanceService::getTop5RentedGears() {
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

    std::vector<CampingGear> ReportFinanceService::getLowStockAlertGears(int threshold) {
        std::vector<CampingGear> result;
        for (const auto& g : dataManager.getAllGears()) {
            if (g.getStockAvailable() <= threshold) {
                result.push_back(g);
            }
        }
        return result;
    }

    std::vector<RentalTransaction> ReportFinanceService::getOverdueTransactions(const std::string& currentDate) {
        std::vector<RentalTransaction> result;
        for (const auto& tx : dataManager.getAllTransactions()) {
            if (tx.getTransactionStatus() == TransactionStatus::ACTIVE_RENTED) {
                if (RentalService::calculateDaysBetween(tx.getExpectedReturnDate(), currentDate) > 0) {
                    result.push_back(tx);
                }
            }
        }
        return result;
    }

    std::string ReportFinanceService::exportFinancialReportText() {
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

} // namespace CampingRental
