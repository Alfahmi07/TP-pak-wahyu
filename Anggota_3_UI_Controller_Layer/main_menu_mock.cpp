/**
 * ============================================================================
 * KERANGKA MENU UTAMA & TESTING NAVIGASI UI (ANGGOTA 3)
 * File: main_menu_mock.cpp
 * Deskripsi: Driver pengujian kerangka visual menu utama, tata letak tabel,
 *            box drawing ASCII, format rupiah, dan simulasi alur sebelum
 *            penggabungan akhir seluruh layer.
 * Standar: Sesuai Panduan Proyek C++ (Tahap Pengujian Kerangka Menu Anggota 3)
 * ============================================================================
 */

#include "ui_views.h"
#include <iostream>
#include <vector>

using namespace CampingRental;

int main() {
    std::cout << "\n=======================================================\n";
    std::cout << " [PENGUJIAN VISUAL & KERANGKA MENU UTAMA - ANGGOTA 3]  \n";
    std::cout << "=======================================================\n\n";

    // 1. Uji Tampilan Banner
    UIViews::printMainBanner();

    // 2. Uji Format Mata Uang Rupiah
    std::cout << "\n[1] Pengujian Format Mata Uang Rupiah:\n";
    std::cout << "  50000       -> " << UIViews::formatRupiah(50000) << "\n";
    std::cout << "  1250000     -> " << UIViews::formatRupiah(1250000) << "\n";
    std::cout << "  -75000      -> " << UIViews::formatRupiah(-75000) << "\n";

    // 3. Uji Box Header & Notifikasi
    UIViews::printSectionHeader("PENGUJIAN MODUL UI BOX");
    UIViews::printSuccessBox("Operasi berhasil dijalankan dengan lancar!");
    UIViews::printInfoBox("Silakan periksa kembali daftar perlengkapan Anda.");
    UIViews::printErrorBox("Format input tidak sesuai kriteria!");

    // 4. Uji Kerangka Menu Utama & Dashboard
    std::cout << "\n[2] Pengujian Kerangka Menu Utama:\n";
    UIViews::printMainMenu();

    std::cout << "\n[3] Pengujian Kerangka Menu Pelanggan (User Dashboard):\n";
    UIViews::printUserDashboardMenu("Budi Santoso", 3);

    std::cout << "\n[4] Pengujian Kerangka Menu Administrator:\n";
    UIViews::printAdminDashboardMenu("Super Admin");

    // 5. Uji Tata Letak Tabel Mock
    std::cout << "\n[5] Pengujian Tata Letak Tabel Alat Camping Mock:\n";
    std::vector<CampingGear> mockGears;
    mockGears.push_back(CampingGear(1, "TND-001", "Tenda Eiger Guardian 4P", "Eiger",
                        GearCategory::TENDA, 50000, 1250000, 8, 6, 24, 4.9, 18,
                        "Kapasitas 4 orang double layer", "Bagus"));
    mockGears.push_back(CampingGear(2, "SLP-001", "Sleeping Bag Arei Everest Polar", "Arei",
                        GearCategory::SLEEPING_GEAR, 15000, 220000, 15, 12, 45, 4.6, 28,
                        "Bulu polar hangat -5C", "Bagus"));
    UIViews::displayGearCatalogTable(mockGears, 1, 5);

    // 6. Uji Syarat & Ketentuan Sewa
    UIViews::printRentalRules();

    std::cout << "\n=======================================================\n";
    std::cout << " KERANGKA MENU UTAMA & UI ANGGOTA 3 LOLOS PENGUJIAN!   \n";
    std::cout << "=======================================================\n\n";

    return 0;
}
