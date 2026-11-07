/**
 * ============================================================================
 * UNIT TEST MANDIRI - DATA LAYER (ANGGOTA 1)
 * File: main_test.cpp
 * Deskripsi: Driver pengujian mandiri menggunakan hardcoded data untuk menguji
 *            seluruh method pada class entitas (User, CampingGear, RentalItem,
 *            RentalTransaction, ReturnLog, ReviewRating) serta persistensi DataManager.
 * Standar: Sesuai Panduan Proyek C++ (Tahap Pengujian Mandiri Anggota 1)
 * ============================================================================
 */

#include "models.h"
#include "data_manager.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

using namespace CampingRental;

void printHeader(const std::string& title) {
    std::cout << "\n=======================================================\n";
    std::cout << " [UNIT TEST DATA LAYER] " << title << "\n";
    std::cout << "=======================================================\n";
}

void testUserEntity() {
    printHeader("1. Pengujian Class User & Serialisasi");

    User u(101, "johndoe", "passHash123", "John Doe", "081233445566",
           "john@example.com", "3201998877660001", "Jl. Cemara No. 5",
           UserRole::USER, UserStatus::ACTIVE, 2, 95.5, "2026-11-01");

    assert(u.getId() == 101);
    assert(u.getUsername() == "johndoe");
    assert(u.getFullName() == "John Doe");
    assert(u.getRole() == UserRole::USER);
    assert(u.isActive() == true);
    assert(!u.isAdmin());

    // Uji Serialisasi
    std::string serialized = u.serialize();
    std::cout << "-> Serialized User: " << serialized << "\n";

    // Uji Deserialisasi
    User deserialized = User::deserialize(serialized);
    assert(deserialized.getId() == 101);
    assert(deserialized.getUsername() == "johndoe");
    assert(deserialized.getFullName() == "John Doe");
    assert(deserialized.getTrustScore() == 95.5);
    std::cout << "[PASS] Class User dan Serialisasi/Deserialisasi Berhasil!\n";
}

void testCampingGearEntity() {
    printHeader("2. Pengujian Class CampingGear & Manipulasi Stok");

    CampingGear gear(50, "TND-999", "Tenda Dome Expedition 6P", "TheNorthFace",
                     GearCategory::TENDA, 100000.0, 3500000.0, 5, 5, 0,
                     5.0, 0, "6 Orang Waterproof Alloy", "Kondisi Baru");

    assert(gear.getId() == 50);
    assert(gear.getSku() == "TND-999");
    assert(gear.getStockAvailable() == 5);
    assert(gear.canRent(3) == true);
    assert(gear.canRent(6) == false);

    // Uji Pengurangan Stok Sewa
    bool rentSuccess = gear.rentUnits(3);
    assert(rentSuccess == true);
    assert(gear.getStockAvailable() == 2);
    assert(gear.getTimesRented() == 3);

    // Uji Pengembalian Unit
    gear.returnUnits(2);
    assert(gear.getStockAvailable() == 4);

    // Uji Tambah Review
    gear.addReview(4.0);
    assert(gear.getReviewCount() == 1);
    assert(gear.getRating() == 4.0);

    gear.addReview(5.0);
    assert(gear.getReviewCount() == 2);
    assert(gear.getRating() == 4.5);

    // Uji Serialisasi & Deserialisasi
    std::string ser = gear.serialize();
    std::cout << "-> Serialized Gear: " << ser << "\n";
    CampingGear deGear = CampingGear::deserialize(ser);
    assert(deGear.getId() == 50);
    assert(deGear.getSku() == "TND-999");
    assert(deGear.getRating() == 4.5);
    std::cout << "[PASS] Class CampingGear dan Operasi Stok Berhasil!\n";
}

void testRentalTransactionEntity() {
    printHeader("3. Pengujian RentalTransaction & Items Sub-serialisasi");

    RentalTransaction tx;
    tx.setInvoiceNumber("INV-TEST-001");
    tx.setUserId(101);
    tx.setCustomerName("John Doe");
    tx.setCustomerPhone("081233445566");
    tx.setCustomerIdCard("3201998877660001");
    tx.setBookingDate("2026-11-07");
    tx.setStartDate("2026-11-08");
    tx.setExpectedReturnDate("2026-11-10");
    tx.setDepositAmount(50000.0);

    RentalItem it1(1, "TND-001", "Tenda Eiger Guardian 4P", 50000, 1250000, 1, 2);
    RentalItem it2(5, "SLP-001", "Sleeping Bag Polar", 15000, 220000, 2, 2);

    assert(it1.calculateSubtotal() == 100000.0);
    assert(it2.calculateSubtotal() == 60000.0);

    tx.addItem(it1);
    tx.addItem(it2);
    tx.setSubtotal(160000.0);
    tx.setTotalAmount(210000.0); // 160000 + 50000 deposit

    std::string ser = tx.serialize();
    std::cout << "-> Serialized Transaction with Items: \n   " << ser << "\n";

    RentalTransaction deTx = RentalTransaction::deserialize(ser);
    assert(deTx.getInvoiceNumber() == "INV-TEST-001");
    assert(deTx.getItems().size() == 2);
    assert(deTx.getItems()[0].getGearSku() == "TND-001");
    assert(deTx.getItems()[1].getQuantity() == 2);
    std::cout << "[PASS] RentalTransaction dan RentalItem Berhasil!\n";
}

void testDataManagerOperations() {
    printHeader("4. Pengujian DataManager (CRUD & File I/O)");

    DataManager dm("test_data");
    dm.resetToDefaultSeed();

    // Verifikasi data pengguna default
    assert(dm.getAllUsers().size() >= 4);
    User* adminUser = dm.findUserByUsername("admin");
    assert(adminUser != nullptr);
    assert(adminUser->isAdmin());

    // Verifikasi data alat camping default
    assert(dm.getAllGears().size() >= 20);
    CampingGear* tenda = dm.findGearBySku("TND-001");
    assert(tenda != nullptr);
    assert(tenda->getCategory() == GearCategory::TENDA);

    // Pengujian Tambah User Baru
    User newUser(dm.getNextUserId(), "testuser", "testpass", "Tester User",
                 "0899999999", "test@test.com", "1234567890", "Bandung",
                 UserRole::USER, UserStatus::ACTIVE);
    bool added = dm.addUser(newUser);
    assert(added == true);
    assert(dm.findUserByUsername("testuser") != nullptr);

    // Pengujian Tambah Gear Baru
    CampingGear newGear(dm.getNextGearId(), "CFT-099", "Kursi Santai Premium", "Naturehike",
                        GearCategory::OUTDOOR_COMFORT, 25000, 300000, 5, 5);
    bool gAdded = dm.addGear(newGear);
    assert(gAdded == true);
    assert(dm.findGearBySku("CFT-099") != nullptr);

    // Pengujian Simpan dan Baca Ulang File
    assert(dm.saveAllData() == true);
    DataManager dm2("test_data");
    assert(dm2.loadAllData() == true);
    assert(dm2.findUserByUsername("testuser") != nullptr);
    assert(dm2.findGearBySku("CFT-099") != nullptr);

    std::cout << "[PASS] Seluruh Pengujian CRUD & File I/O DataManager Berhasil!\n";
}

int main() {
    std::cout << "\n=======================================================\n";
    std::cout << "  MEMULAI UNIT TEST MANDIRI DATA LAYER (ANGGOTA 1)    \n";
    std::cout << "=======================================================\n";

    testUserEntity();
    testCampingGearEntity();
    testRentalTransactionEntity();
    testDataManagerOperations();

    std::cout << "\n=======================================================\n";
    std::cout << "  SEMUA PENGUJIAN UNIT DATA LAYER LOLOS DENGAN SUKSES! \n";
    std::cout << "=======================================================\n\n";
    return 0;
}
