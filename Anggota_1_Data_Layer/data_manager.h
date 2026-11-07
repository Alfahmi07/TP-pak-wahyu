/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - DATA LAYER (ANGGOTA 1)
 * File: data_manager.h
 * Deskripsi: Deklarasi class DataManager untuk mengelola persistensi file
 *            (.txt), repositori data memori, operasi CRUD, dan inisialisasi seed data.
 * Penulis: Anggota 1 (Data Layer Specialist)
 * ============================================================================
 */

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "models.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace CampingRental {

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

        // Helper File I/O Internal
        bool ensureDirectoryExists(const std::string& path);
        void seedInitialUsers();
        void seedInitialGears();
        void seedInitialTransactions();
        void seedInitialReviews();

    public:
        DataManager();
        explicit DataManager(const std::string& dataDirectory);
        ~DataManager();

        // Path Configuration
        void setDataDirectory(const std::string& dataDirectory);

        // Operasi File Load & Save
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

        // -------------------------------------------------------------
        // CRUD USER
        // -------------------------------------------------------------
        const std::vector<User>& getAllUsers() const;
        std::vector<User>& getAllUsersRef();
        User* findUserById(int id);
        User* findUserByUsername(const std::string& username);
        User* findUserByEmail(const std::string& email);
        bool addUser(const User& user);
        bool updateUser(const User& user);
        bool deleteUser(int id);
        int getNextUserId() const;

        // -------------------------------------------------------------
        // CRUD CAMPING GEAR
        // -------------------------------------------------------------
        const std::vector<CampingGear>& getAllGears() const;
        std::vector<CampingGear>& getAllGearsRef();
        CampingGear* findGearById(int id);
        CampingGear* findGearBySku(const std::string& sku);
        bool addGear(const CampingGear& gear);
        bool updateGear(const CampingGear& gear);
        bool deleteGear(int id);
        int getNextGearId() const;
        std::string generateNextGearSku(GearCategory category) const;

        // -------------------------------------------------------------
        // CRUD TRANSAKSI
        // -------------------------------------------------------------
        const std::vector<RentalTransaction>& getAllTransactions() const;
        std::vector<RentalTransaction>& getAllTransactionsRef();
        RentalTransaction* findTransactionByInvoice(const std::string& invoice);
        std::vector<RentalTransaction> findTransactionsByUserId(int userId) const;
        std::vector<RentalTransaction> findTransactionsByStatus(TransactionStatus status) const;
        bool addTransaction(const RentalTransaction& trx);
        bool updateTransaction(const RentalTransaction& trx);
        bool deleteTransaction(const std::string& invoice);
        std::string generateNextInvoiceNumber() const;

        // -------------------------------------------------------------
        // CRUD RETURN LOG
        // -------------------------------------------------------------
        const std::vector<ReturnLog>& getAllReturnLogs() const;
        std::vector<ReturnLog>& getAllReturnLogsRef();
        ReturnLog* findReturnLogByInvoice(const std::string& invoice);
        bool addReturnLog(const ReturnLog& log);
        int getNextReturnLogId() const;

        // -------------------------------------------------------------
        // CRUD REVIEW
        // -------------------------------------------------------------
        const std::vector<ReviewRating>& getAllReviews() const;
        std::vector<ReviewRating> getReviewsByGearId(int gearId) const;
        bool addReview(const ReviewRating& review);
        int getNextReviewId() const;

        // Reset / Re-seed Data (Untuk keperluan testing / restore default)
        void resetToDefaultSeed();
    };

} // namespace CampingRental

#endif // DATA_MANAGER_H
