/**
 * ============================================================================
 * SISTEM PENYEWAAN ALAT CAMPING - UI & CONTROLLER LAYER (ANGGOTA 3)
 * File: controller.h
 * Deskripsi: Definisi class AppController sebagai orkestrator dan integrator
 *            utama dari seluruh modul Data Layer dan Business Logic Layer.
 * Penulis: Anggota 3 (UI & Controller Specialist / Integrator)
 * ============================================================================
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Anggota_1_Data_Layer/models.h"
#include "../Anggota_1_Data_Layer/data_manager.h"
#include "../Anggota_2_Business_Logic_Layer/business_logic.h"
#include "ui_views.h"
#include <string>
#include <vector>
#include <memory>

namespace CampingRental {

    class AppController {
    private:
        DataManager dataManager;
        AuthService authService;
        CatalogService catalogService;
        CartService cartService;
        RentalService rentalService;
        PaymentService paymentService;
        ReturnService returnService;
        AdminService adminService;
        ReportFinanceService reportFinanceService;

        bool isRunning;

        // -------------------------------------------------------------
        // SUBMENU CONTROLLERS: USER FLOW
        // -------------------------------------------------------------
        void handleLogin();
        void handleRegister();
        void handleGuestCatalog();
        void handleUserDashboard();
        void handleUserCatalog();
        void handleUserCart();
        void handleUserCheckout();
        void handleUserPayment();
        void handleUserRentalHistory();
        void handleUserReview();
        void handleUserProfile();

        // -------------------------------------------------------------
        // SUBMENU CONTROLLERS: ADMIN FLOW
        // -------------------------------------------------------------
        void handleAdminDashboard();
        void handleAdminInventory();
        void handleAdminAddGear();
        void handleAdminEditGear();
        void handleAdminRestockGear();
        void handleAdminDeleteGear();
        void handleAdminTransactions();
        void handleAdminReturnGear();
        void handleAdminCustomerManagement();
        void handleAdminDashboardAnalytics();
        void handleAdminExportReport();

    public:
        AppController();
        explicit AppController(const std::string& dataDirectory);
        ~AppController();

        void run();
        void shutdown();
    };

} // namespace CampingRental

#endif // CONTROLLER_H
