/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - BUSINESS LOGIC LAYER (ANGGOTA 2)
 * File: business_logic.h
 * Deskripsi: Definisi seluruh service, aturan bisnis (business rules),
 *            algoritma pengurutan (Sorting) & pencarian (Searching),
 *            kalkulasi biaya sewa, denda, diskon, dan pelaporan keuangan.
 * Penulis: Anggota 2 (Business Logic Specialist)
 * ============================================================================
 */

#ifndef BUSINESS_LOGIC_H
#define BUSINESS_LOGIC_H

#include "../Anggota_1_Data_Layer/models.h"
#include "../Anggota_1_Data_Layer/data_manager.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace CampingRental {

    // ========================================================================
    // STRUKTUR SESI & HASIL OPERASI (SERVICE RESULT)
    // ========================================================================
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

    // ========================================================================
    // ENUM KRITERIA PENGURUTAN (SORTING CRITERIA)
    // ========================================================================
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

    // ========================================================================
    // CLASS ALGORITMA SORTING & SEARCHING (MANDATORY CS ALGORITHMS)
    // ========================================================================
    class AlgorithmEngine {
    public:
        // Algoritma MergeSort untuk pengurutan Alat Camping
        static void mergeSortGears(std::vector<CampingGear>& arr, int left, int right, GearSortField sortField);
        static void merge(std::vector<CampingGear>& arr, int left, int mid, int right, GearSortField sortField);

        // Algoritma QuickSort untuk pengurutan Transaksi
        static void quickSortTransactions(std::vector<RentalTransaction>& arr, int low, int high, TransactionSortField sortField);
        static int partition(std::vector<RentalTransaction>& arr, int low, int high, TransactionSortField sortField);

        // Algoritma BubbleSort untuk pengurutan ulasan / list kecil
        static void bubbleSortReviews(std::vector<ReviewRating>& arr, bool newestFirst = true);

        // Algoritma Binary Search untuk pencarian SKU alat (setelah terurut)
        static int binarySearchGearBySku(const std::vector<CampingGear>& sortedGears, const std::string& targetSku);

        // Algoritma Linear Search Multi-Field untuk alat camping
        static std::vector<CampingGear> linearSearchGears(const std::vector<CampingGear>& gears,
                                                         const std::string& keyword,
                                                         int categoryFilter = -1,
                                                         double maxPrice = -1.0,
                                                         bool onlyAvailable = false);

        // Linear Search Transaksi
        static std::vector<RentalTransaction> linearSearchTransactions(const std::vector<RentalTransaction>& txs,
                                                                      const std::string& query);
    };

    // ========================================================================
    // SERVICE: AUTENTIKASI & MANAJEMEN PENGGUNA (AUTH SERVICE)
    // ========================================================================
    class AuthService {
    private:
        DataManager& dataManager;
        UserSession currentSession;

    public:
        explicit AuthService(DataManager& dm);

        const UserSession& getCurrentSession() const { return currentSession; }
        bool isAuthenticated() const { return currentSession.isLoggedIn; }
        bool isAdmin() const { return currentSession.isLoggedIn && currentSession.role == UserRole::ADMIN; }

        ServiceResult registerUser(const std::string& username, const std::string& password,
                                  const std::string& confirmPassword, const std::string& fullName,
                                  const std::string& phone, const std::string& email,
                                  const std::string& idCard, const std::string& address);

        ServiceResult login(const std::string& username, const std::string& password);
        void logout();

        ServiceResult changePassword(const std::string& oldPassword, const std::string& newPassword);
        ServiceResult updateProfile(const std::string& fullName, const std::string& phone,
                                   const std::string& email, const std::string& address);

        static std::string hashPassword(const std::string& rawPassword);
        static bool validateEmailFormat(const std::string& email);
        static bool validatePhoneFormat(const std::string& phone);
        static bool validateUsernameFormat(const std::string& username);
    };

    // ========================================================================
    // SERVICE: KATALOG ALAT CAMPING (CATALOG SERVICE)
    // ========================================================================
    class CatalogService {
    private:
        DataManager& dataManager;

    public:
        explicit CatalogService(DataManager& dm);

        std::vector<CampingGear> getCatalogList(GearSortField sortField = GearSortField::NAME_ASC,
                                               int categoryFilter = -1,
                                               const std::string& searchKeyword = "",
                                               double maxPrice = -1.0,
                                               bool onlyAvailable = false);

        CampingGear* getGearDetail(int gearId);
        CampingGear* getGearBySku(const std::string& sku);
        std::vector<ReviewRating> getGearReviews(int gearId);
        ServiceResult submitReview(int gearId, int userId, const std::string& username,
                                  int ratingValue, const std::string& reviewText);
    };

    // ========================================================================
    // SERVICE: KERANJANG PENYEWAAN (CART SERVICE)
    // ========================================================================
    class CartService {
    private:
        DataManager& dataManager;
        Cart userCart;

    public:
        explicit CartService(DataManager& dm);

        void initializeForUser(int userId);
        const Cart& getCart() const { return userCart; }
        Cart& getCartRef() { return userCart; }

        ServiceResult addToCart(int gearId, int quantity, int rentalDays);
        ServiceResult updateItemQuantity(int gearId, int newQuantity);
        ServiceResult removeItem(int gearId);
        void clearCart();

        int getTotalItemCount() const;
        double calculateCartSubtotal() const;
        double calculateCartEstimatedDeposit() const;
    };

    // ========================================================================
    // SERVICE: TRANSAKSI & PROMO (RENTAL & CHECKOUT SERVICE)
    // ========================================================================
    class RentalService {
    private:
        DataManager& dataManager;
        std::vector<PromoCode> availablePromos;

        void initializePromos();

    public:
        explicit RentalService(DataManager& dm);

        const std::vector<PromoCode>& getAvailablePromos() const { return availablePromos; }
        PromoCode* findPromoCode(const std::string& code);
        double calculatePromoDiscount(const std::string& code, double subtotal);

        // Kalkulasi Durasi Hari dari Format YYYY-MM-DD
        static int calculateDaysBetween(const std::string& startDate, const std::string& endDate);
        static std::string getCurrentDateString();
        static std::string addDaysToDate(const std::string& dateStr, int days);

        // Checkout Wizard
        ServiceResult processCheckout(int userId,
                                      const std::string& customerName,
                                      const std::string& customerPhone,
                                      const std::string& customerIdCard,
                                      const std::string& startDate,
                                      int rentalDurationDays,
                                      const std::vector<RentalItem>& items,
                                      PaymentMethod paymentMethod,
                                      const std::string& promoCode = "",
                                      const std::string& orderNotes = "");

        std::vector<RentalTransaction> getUserTransactions(int userId);
        RentalTransaction* getTransactionDetail(const std::string& invoiceNumber);
    };

    // ========================================================================
    // SERVICE: PEMBAYARAN (PAYMENT SERVICE)
    // ========================================================================
    class PaymentService {
    private:
        DataManager& dataManager;

    public:
        explicit PaymentService(DataManager& dm);

        std::string generatePaymentInstructions(PaymentMethod method, double totalAmount,
                                               const std::string& invoiceNumber);

        ServiceResult submitPaymentProof(const std::string& invoiceNumber, const std::string& proofRef);
        ServiceResult confirmPaymentByAdmin(const std::string& invoiceNumber, const std::string& adminName);
    };

    // ========================================================================
    // SERVICE: PENGEMBALIAN ALAT & DENDA (RETURN SERVICE)
    // ========================================================================
    class ReturnService {
    private:
        DataManager& dataManager;

    public:
        explicit ReturnService(DataManager& dm);

        // Kalkulasi Denda Keterlambatan
        static double calculateLateFeePerDay(double itemPricePerDay);
        static double calculateConditionPenalty(double replacementValue, ReturnCondition condition);

        ServiceResult processReturnInspection(const std::string& invoiceNumber,
                                             const std::string& actualReturnDate,
                                             const std::vector<ReturnCondition>& itemConditions,
                                             const std::string& adminInspectorName,
                                             const std::string& notes,
                                             ReturnLog& outReturnLog);
    };

    // ========================================================================
    // SERVICE: MANAJEMEN ADMIN (INVENTARIS, PELANGGAN, TRANSAKSI)
    // ========================================================================
    class AdminService {
    private:
        DataManager& dataManager;

    public:
        explicit AdminService(DataManager& dm);

        // Manajemen Inventaris
        ServiceResult addNewGear(const std::string& name, const std::string& brand,
                                GearCategory category, double rentalPrice, double replacementValue,
                                int stock, const std::string& spec, const std::string& notes);
        ServiceResult updateGearDetails(int gearId, const std::string& name, const std::string& brand,
                                       double rentalPrice, double replacementValue, const std::string& spec);
        ServiceResult restockGear(int gearId, int additionalStock);
        ServiceResult removeGear(int gearId);

        // Manajemen Pelanggan
        ServiceResult toggleUserStatus(int userId, UserStatus newStatus);
        ServiceResult updateUserTrustScore(int userId, double scoreDelta);

        // Manajemen Transaksi Admin
        ServiceResult updateTransactionStatus(const std::string& invoiceNumber, TransactionStatus newStatus);
        ServiceResult markTransactionAsPickedUp(const std::string& invoiceNumber);
        ServiceResult cancelTransaction(const std::string& invoiceNumber, const std::string& reason);
    };

    // ========================================================================
    // SERVICE: DASHBOARD & LAPORAN KEUANGAN (REPORT SERVICE)
    // ========================================================================
    class ReportFinanceService {
    private:
        DataManager& dataManager;

    public:
        explicit ReportFinanceService(DataManager& dm);

        FinancialKPI calculateOverallFinancialKPI();
        std::vector<std::pair<CampingGear, int>> getTop5RentedGears();
        std::vector<CampingGear> getLowStockAlertGears(int threshold = 3);
        std::vector<RentalTransaction> getOverdueTransactions(const std::string& currentDate);
        std::string exportFinancialReportText();
    };

} // namespace CampingRental

#endif // BUSINESS_LOGIC_H
