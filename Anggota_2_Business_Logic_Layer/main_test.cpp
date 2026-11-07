/**
 * ============================================================================
 * UNIT TEST MANDIRI - BUSINESS LOGIC LAYER (ANGGOTA 2)
 * File: main_test.cpp
 * Deskripsi: Driver pengujian mandiri menggunakan hardcoded data untuk menguji
 *            seluruh method pada business logic: Algoritma Sorting (MergeSort, QuickSort),
 *            Searching (Binary Search, Linear Search), AuthService, CatalogService,
 *            CartService, RentalService (Checkout, Diskon, Deposit),
 *            ReturnService (Denda Keterlambatan & Kerusakan), dan ReportFinanceService.
 * Standar: Sesuai Panduan Proyek C++ (Tahap Pengujian Mandiri Anggota 2)
 * ============================================================================
 */

#include "business_logic.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

using namespace CampingRental;

void printHeader(const std::string& title) {
    std::cout << "\n=======================================================\n";
    std::cout << " [UNIT TEST BUSINESS LOGIC] " << title << "\n";
    std::cout << "=======================================================\n";
}

void testAlgorithmEngine() {
    printHeader("1. Pengujian Algoritma Sorting & Searching");

    std::vector<CampingGear> testGears;
    testGears.push_back(CampingGear(1, "TND-003", "Tenda C", "BrandX", GearCategory::TENDA, 70000, 1000000, 5, 5, 10, 4.2));
    testGears.push_back(CampingGear(2, "TND-001", "Tenda A", "BrandX", GearCategory::TENDA, 30000, 500000, 8, 8, 25, 4.9));
    testGears.push_back(CampingGear(3, "TND-002", "Tenda B", "BrandX", GearCategory::TENDA, 50000, 800000, 4, 4, 15, 4.7));

    // Uji MergeSort berdasarkan Harga Termurah (PRICE_ASC)
    AlgorithmEngine::mergeSortGears(testGears, 0, static_cast<int>(testGears.size()) - 1, GearSortField::PRICE_ASC);
    assert(testGears[0].getRentalPricePerDay() == 30000);
    assert(testGears[1].getRentalPricePerDay() == 50000);
    assert(testGears[2].getRentalPricePerDay() == 70000);
    std::cout << "[PASS] MergeSort Gear (PRICE_ASC) Berhasil!\n";

    // Uji MergeSort berdasarkan Nama (NAME_ASC)
    AlgorithmEngine::mergeSortGears(testGears, 0, static_cast<int>(testGears.size()) - 1, GearSortField::NAME_ASC);
    assert(testGears[0].getName() == "Tenda A");
    assert(testGears[1].getName() == "Tenda B");
    assert(testGears[2].getName() == "Tenda C");
    std::cout << "[PASS] MergeSort Gear (NAME_ASC) Berhasil!\n";

    // Uji Binary Search by SKU
    // Pastikan terurut berdasarkan SKU terlebih dahulu
    std::vector<CampingGear> skuSorted = testGears; // TND-001, TND-002, TND-003
    int foundIdx = AlgorithmEngine::binarySearchGearBySku(skuSorted, "TND-002");
    assert(foundIdx != -1);
    assert(skuSorted[foundIdx].getSku() == "TND-002");

    int notFoundIdx = AlgorithmEngine::binarySearchGearBySku(skuSorted, "TND-999");
    assert(notFoundIdx == -1);
    std::cout << "[PASS] Binary Search SKU Berhasil!\n";

    // Uji Linear Search Multi-field
    auto searchRes = AlgorithmEngine::linearSearchGears(testGears, "Tenda B");
    assert(searchRes.size() == 1);
    assert(searchRes[0].getName() == "Tenda B");
    std::cout << "[PASS] Linear Search Multi-Field Berhasil!\n";
}

void testAuthService() {
    printHeader("2. Pengujian AuthService (Registrasi & Login)");

    DataManager dm("test_bll_data");
    dm.resetToDefaultSeed();
    AuthService auth(dm);

    // Test Registrasi dengan format username salah
    ServiceResult r1 = auth.registerUser("ab", "pass123", "pass123", "Nama User", "081234567890", "a@b.com", "12345678", "Bandung");
    assert(!r1.success);

    // Test Registrasi dengan password tidak cocok
    ServiceResult r2 = auth.registerUser("userbaru", "pass123", "passbeda", "Nama User", "081234567890", "baru@test.com", "12345678", "Bandung");
    assert(!r2.success);

    // Test Registrasi Sukses
    ServiceResult r3 = auth.registerUser("userbagus", "pass123", "pass123", "Bagus Setiawan", "081234567890", "bagus@test.com", "3201123456780001", "Bandung");
    assert(r3.success);

    // Test Login Gagal
    ServiceResult l1 = auth.login("userbagus", "passsalah");
    assert(!l1.success);

    // Test Login Berhasil
    ServiceResult l2 = auth.login("userbagus", "pass123");
    assert(l2.success);
    assert(auth.isAuthenticated());
    assert(auth.getCurrentSession().username == "userbagus");

    // Test Admin Login
    auth.logout();
    ServiceResult lAdmin = auth.login("admin", "admin123");
    assert(lAdmin.success);
    assert(auth.isAdmin());

    std::cout << "[PASS] Validasi Registrasi & Login AuthService Berhasil!\n";
}

void testCartAndRentalCheckout() {
    printHeader("3. Pengujian CartService & RentalService (Checkout)");

    DataManager dm("test_bll_data");
    dm.resetToDefaultSeed();

    CartService cart(dm);
    cart.initializeForUser(2);

    // Tambah tenda ID 1 ke keranjang (stok tersedia = 6)
    ServiceResult c1 = cart.addToCart(1, 2, 3); // 2 unit selama 3 hari
    assert(c1.success);
    assert(cart.getTotalItemCount() == 2);

    // Tambah melebihi stok yang ada
    ServiceResult c2 = cart.addToCart(1, 10, 3);
    assert(!c2.success);

    // Subtotal: 2 unit * 50.000 * 3 hari = 300.000
    assert(cart.calculateCartSubtotal() == 300000.0);
    std::cout << "[PASS] Cart Operasi & Perhitungan Subtotal Berhasil!\n";

    // Uji Checkout
    RentalService rental(dm);
    ServiceResult co = rental.processCheckout(2, "Budi Santoso", "081234567890", "3271011505950002",
                                             "2026-11-08", 3, cart.getCart().getItems(),
                                             PaymentMethod::TRANSFER_BANK_BCA, "CAMPINGASIK10");
    assert(co.success);
    std::string invoice = co.message;
    std::cout << "-> Invoice Berhasil Diterbitkan: " << invoice << "\n";

    RentalTransaction* createdTx = rental.getTransactionDetail(invoice);
    assert(createdTx != nullptr);
    assert(createdTx->getSubtotal() == 300000.0);
    // Diskon 10% dari 300.000 = 30.000
    assert(createdTx->getDiscountAmount() == 30000.0);
    // Deposit minimum 50.000 atau 10%
    assert(createdTx->getDepositAmount() >= 50000.0);

    // Pastikan stok berkurang
    CampingGear* gear1 = dm.findGearById(1);
    assert(gear1->getStockAvailable() == 4); // Awal 6 - 2 = 4

    std::cout << "[PASS] Checkout Wizard, Diskon Promo, & Pengurangan Stok Berhasil!\n";
}

void testReturnAndPenaltyService() {
    printHeader("4. Pengujian ReturnService (Inspeksi Denda & Pengembalian Deposit)");

    DataManager dm("test_bll_data");
    dm.resetToDefaultSeed();

    ReturnService returnService(dm);

    // Buat skenario pengembalian dengan keterlambatan 2 hari dan 1 unit rusak ringan
    // Kita gunakan transaksi yang sedang aktif (INV-202611-1002, expected return 2026-11-09)
    RentalTransaction* tx = dm.findTransactionByInvoice("INV-202611-1002");
    assert(tx != nullptr);

    // Return tanggal 2026-11-11 (telat 2 hari)
    std::vector<ReturnCondition> conditions;
    conditions.push_back(ReturnCondition::MINOR_DAMAGE); // Tenda rusak ringan (25% nilai barang)
    conditions.push_back(ReturnCondition::GOOD_CONDITION);
    conditions.push_back(ReturnCondition::GOOD_CONDITION);

    ReturnLog retLog;
    ServiceResult retRes = returnService.processReturnInspection("INV-202611-1002", "2026-11-11",
                                                                conditions, "Admin Tester",
                                                                "Tenda robek kecil di teras", retLog);
    assert(retRes.success);
    assert(retLog.getOverdueDays() == 2);
    assert(retLog.getLateFeeCalculated() > 0);
    assert(retLog.getDamageFeeCalculated() > 0);
    std::cout << "-> Denda Telat: Rp " << retLog.getLateFeeCalculated() << "\n";
    std::cout << "-> Denda Kerusakan: Rp " << retLog.getDamageFeeCalculated() << "\n";
    std::cout << "-> Total Denda: Rp " << retLog.getTotalPenalty() << "\n";

    // Pastikan status transaksi terupdate menjadi RETURNED_WITH_ISSUE
    assert(tx->getTransactionStatus() == TransactionStatus::RETURNED_WITH_ISSUE);

    std::cout << "[PASS] Return Inspection, Denda Keterlambatan & Kerusakan Berhasil!\n";
}

void testReportFinanceService() {
    printHeader("5. Pengujian ReportFinanceService (KPI & Analisis)");

    DataManager dm("test_bll_data");
    dm.resetToDefaultSeed();

    ReportFinanceService report(dm);
    FinancialKPI kpi = report.calculateOverallFinancialKPI();

    assert(kpi.totalTransactionsCount >= 3);
    assert(kpi.totalCustomersCount >= 3);
    assert(kpi.totalGearsRegistered >= 20);
    assert(kpi.totalGrossRevenue > 0);
    assert(kpi.totalNetProfit > 0);

    auto topGears = report.getTop5RentedGears();
    assert(topGears.size() == 5);
    std::cout << "-> Top 1 Alat Terlaris: " << topGears[0].first.getName()
              << " (" << topGears[0].second << " kali disewa)\n";

    std::string reportText = report.exportFinancialReportText();
    assert(!reportText.empty());

    std::cout << "[PASS] Laporan Keuangan, KPI, dan Top Rented Gears Berhasil!\n";
}

int main() {
    std::cout << "\n=======================================================\n";
    std::cout << " MEMULAI UNIT TEST MANDIRI BUSINESS LOGIC (ANGGOTA 2)  \n";
    std::cout << "=======================================================\n";

    testAlgorithmEngine();
    testAuthService();
    testCartAndRentalCheckout();
    testReturnAndPenaltyService();
    testReportFinanceService();

    std::cout << "\n=======================================================\n";
    std::cout << " SEMUA PENGUJIAN UNIT BUSINESS LOGIC LOLOS DENGAN SUKSES!\n";
    std::cout << "=======================================================\n\n";
    return 0;
}
