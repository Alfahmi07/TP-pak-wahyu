/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - UI & CONTROLLER LAYER (ANGGOTA 3)
 * File: ui_views.h
 * Deskripsi: Definisi tata letak visual antarmuka konsol (CLI), tabel data,
 *            cetak nota struk resmi, format mata uang rupiah, dan input helper.
 * Penulis: Anggota 3 (UI & Controller Specialist / Integrator)
 * ============================================================================
 */

#ifndef UI_VIEWS_H
#define UI_VIEWS_H

#include "../Anggota_1_Data_Layer/models.h"
#include "../Anggota_2_Business_Logic_Layer/business_logic.h"
#include <string>
#include <vector>
#include <iostream>

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

    class UIViews {
    public:
        // Utilitas Tampilan Konsol
        static void enableVirtualTerminal();
        static void clearScreen();
        static void pauseScreen();
        static std::string formatRupiah(double amount);
        static std::string centerText(const std::string& text, int width);
        static std::string padRight(const std::string& text, size_t width);
        static std::string padLeft(const std::string& text, size_t width);

        // Header & Banner Antarmuka
        static void printMainBanner();
        static void printSectionHeader(const std::string& title);
        static void printSubHeader(const std::string& title);
        static void printSuccessBox(const std::string& message);
        static void printErrorBox(const std::string& message);
        static void printInfoBox(const std::string& message);
        static void printDivider(int length = 85, char ch = '=');

        // Input Helper dengan Validasi
        static int readInt(const std::string& prompt, int minVal = 0, int maxVal = 999999);
        static double readDouble(const std::string& prompt, double minVal = 0.0, double maxVal = 1e9);
        static std::string readString(const std::string& prompt, bool allowEmpty = false);
        static bool confirmAction(const std::string& question);

        // Tampilan Tabel & Grid
        static void displayGearCatalogTable(const std::vector<CampingGear>& gears, int currentPage = 1, int itemsPerPage = 6);
        static void displayGearDetailCard(const CampingGear& gear, const std::vector<ReviewRating>& reviews);
        static void displayCartTable(const Cart& cart);
        static void displayTransactionListTable(const std::vector<RentalTransaction>& transactions);
        static void displayOfficialInvoice(const RentalTransaction& tx);
        static void displayOfficialReturnReceipt(const RentalTransaction& tx, const ReturnLog& log);

        // Tampilan Admin
        static void displayAdminGearTable(const std::vector<CampingGear>& gears);
        static void displayAdminCustomerTable(const std::vector<User>& users);
        static void displayFinancialDashboard(const FinancialKPI& kpi,
                                             const std::vector<std::pair<CampingGear, int>>& topGears,
                                             const std::vector<CampingGear>& lowStockGears);

        // Menu Navigasi
        static void printMainMenu();
        static void printUserDashboardMenu(const std::string& customerName, int cartItemCount);
        static void printAdminDashboardMenu(const std::string& adminName);
        static void printCatalogSubMenu();
        static void printRentalRules();
    };

} // namespace CampingRental

#endif // UI_VIEWS_H
