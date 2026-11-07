/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - UI & CONTROLLER LAYER (ANGGOTA 3)
 * File: ui_views.cpp
 * Deskripsi: Implementasi antarmuka visual konsol (CLI), tata letak tabel,
 *            cetak invoice struk resmi, format mata uang Rupiah, dan input sanitization.
 * Penulis: Anggota 3 (UI & Controller Specialist / Integrator)
 * ============================================================================
 */

#include "ui_views.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cmath>

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

    void UIViews::enableVirtualTerminal() {
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

    void UIViews::clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
        enableVirtualTerminal();
#else
        std::system("clear");
#endif
    }

    void UIViews::pauseScreen() {
        std::cout << "\nTekan [ENTER] untuk melanjutkan...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string UIViews::formatRupiah(double amount) {
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

    std::string UIViews::centerText(const std::string& text, int width) {
        int len = static_cast<int>(text.length());
        if (len >= width) return text;
        int leftPadding = (width - len) / 2;
        int rightPadding = width - len - leftPadding;
        return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
    }

    std::string UIViews::padRight(const std::string& text, size_t width) {
        if (text.length() >= width) {
            return text.substr(0, width);
        }
        return text + std::string(width - text.length(), ' ');
    }

    std::string UIViews::padLeft(const std::string& text, size_t width) {
        if (text.length() >= width) {
            return text.substr(0, width);
        }
        return std::string(width - text.length(), ' ') + text;
    }

    void UIViews::printDivider(int length, char ch) {
        std::cout << std::string(length, ch) << "\n";
    }

    void UIViews::printMainBanner() {
        enableVirtualTerminal();
        std::cout << ConsoleColor::BLUE;
        std::cout << "=========================================================================================\n";
        std::cout << "    /\\_ /\\      CAMPING GEAR RENTAL HUB - SISTEM PENYEWAAN ALAT CAMPING                  \n";
        std::cout << "   (  . .)     Penyewaan Alat Outdoor Terlengkap: Tenda, Carrier, Kompor & Sleeping Bag \n";
        std::cout << "    >  ^ <      Bersih, Terawat, Berkualitas Tinggi, & Siap Menemani Ekspedisi Anda!     \n";
        std::cout << "=========================================================================================\n";
        std::cout << ConsoleColor::RESET;
    }

    void UIViews::printSectionHeader(const std::string& title) {
        std::cout << ConsoleColor::BLUE;
        std::cout << "\n+---------------------------------------------------------------------------------------+\n";
        std::cout << "| " << padRight(">> " + title, 85) << " |\n";
        std::cout << "+---------------------------------------------------------------------------------------+\n";
        std::cout << ConsoleColor::RESET;
    }

    void UIViews::printSubHeader(const std::string& title) {
        std::cout << "\n--- [ " << title << " ] ---\n";
    }

    void UIViews::printSuccessBox(const std::string& message) {
        std::cout << "\n" << ConsoleColor::GREEN << "[BERHASIL] " << message << ConsoleColor::RESET << "\n";
    }

    void UIViews::printErrorBox(const std::string& message) {
        std::cout << "\n" << ConsoleColor::RED << "[PERINGATAN/GAGAL] " << message << ConsoleColor::RESET << "\n";
    }

    void UIViews::printInfoBox(const std::string& message) {
        std::cout << "\n[INFO] " << message << "\n";
    }

    int UIViews::readInt(const std::string& prompt, int minVal, int maxVal) {
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

    double UIViews::readDouble(const std::string& prompt, double minVal, double maxVal) {
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

    std::string UIViews::readString(const std::string& prompt, bool allowEmpty) {
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

    bool UIViews::confirmAction(const std::string& question) {
        std::string ans;
        std::cout << question << " (y/n): ";
        std::getline(std::cin, ans);
        return (!ans.empty() && (ans[0] == 'y' || ans[0] == 'Y'));
    }

    void UIViews::displayGearCatalogTable(const std::vector<CampingGear>& gears, int currentPage, int itemsPerPage) {
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

    void UIViews::displayGearDetailCard(const CampingGear& gear, const std::vector<ReviewRating>& reviews) {
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

    void UIViews::displayCartTable(const Cart& cart) {
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

    void UIViews::displayTransactionListTable(const std::vector<RentalTransaction>& transactions) {
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

    void UIViews::displayOfficialInvoice(const RentalTransaction& tx) {
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
        std::cout << "+=======================================================================================+\n";
    }

    void UIViews::displayOfficialReturnReceipt(const RentalTransaction& tx, const ReturnLog& log) {
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
        std::cout << "+=======================================================================================+\n";
    }

    void UIViews::displayAdminGearTable(const std::vector<CampingGear>& gears) {
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

    void UIViews::displayAdminCustomerTable(const std::vector<User>& users) {
        std::cout << "\n+-----+-----------------+---------------------------+-----------------+--------------+----------+--------+\n";
        std::cout << "| ID  | Username        | Nama Lengkap Pelanggan    | Nomor Telepon   | Status Akun  | Total Sew| Reputasi|\n";
        std::cout << "+-----+-----------------+---------------------------+-----------------+--------------+----------+--------+\n";
        for (const auto& u : users) {
            if (u.isAdmin()) continue; // Hanya tampilkan pelanggan
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

    void UIViews::displayFinancialDashboard(const FinancialKPI& kpi,
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

    void UIViews::printMainMenu() {
        printMainBanner();
        std::cout << ConsoleColor::BLUE
                  << "  SELAMAT DATANG DI APLIKASI PENYEWAAN ALAT CAMPING\n"
                  << "  -------------------------------------------------\n"
                  << ConsoleColor::RESET;
        std::cout << "  [1] Masuk ke Akun (Login)\n";
        std::cout << "  [2] Registrasi Akun Pelanggan Baru\n";
        std::cout << "  [3] Lihat Katalog Alat Camping (Mode Tamu/Guest)\n";
        std::cout << "  [4] Syarat, Ketentuan, & Tata Cara Penyewaan\n";
        std::cout << "  [0] Keluar dari Aplikasi\n";
        std::cout << "  -------------------------------------------------\n";
    }

    void UIViews::printUserDashboardMenu(const std::string& customerName, int cartItemCount) {
        printMainBanner();
        std::cout << ConsoleColor::BLUE
                  << "  PANEL PELANGGAN | Halo, " << customerName << "!\n"
                  << "  ===================================================================\n"
                  << ConsoleColor::RESET;
        std::cout << "  [1] Katalog Alat Camping (Cari, Filter, Urutkan, & Sewa)\n";
        std::cout << "  [2] Keranjang Penyewaan [" << cartItemCount << " Item]\n";
        std::cout << "  [3] Transaksi & Checkout (Pilih Tanggal, Kupon Promo & Pembayaran)\n";
        std::cout << "  [4] Konfirmasi & Instruksi Pembayaran\n";
        std::cout << "  [5] Riwayat Penyewaan & Cetak Nota Sewa\n";
        std::cout << "  [6] Berikan Ulasan & Rating Alat yang Pernah Disewa\n";
        std::cout << "  [7] Profil Pengguna & Ganti Password\n";
        std::cout << "  [0] Keluar / Logout Sesi\n";
        std::cout << "  ===================================================================\n";
    }

    void UIViews::printAdminDashboardMenu(const std::string& adminName) {
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

    void UIViews::printCatalogSubMenu() {
        std::cout << "\n  PILIHAN NAVIGASI KATALOG:\n";
        std::cout << "  [1] Urutkan Katalog (Sorting: Harga, Nama, Rating, Stok)\n";
        std::cout << "  [2] Cari Alat Camping (Searching Kata Kunci / Filter Kategori)\n";
        std::cout << "  [3] Lihat Rincian Spesifikasi & Ulasan Barang (Detail Card)\n";
        std::cout << "  [4] Masukkan Barang ke Keranjang Sewa\n";
        std::cout << "  [5] Reset Filter & Tampilkan Semua\n";
        std::cout << "  [0] Kembali ke Menu Sebelumnya\n";
    }

    void UIViews::printRentalRules() {
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

} // namespace CampingRental
