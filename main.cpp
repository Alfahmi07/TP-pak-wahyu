/**
 * ============================================================================
 * SISTEM INFORMASI PENYEWAAN ALAT CAMPING (MODULAR INTEGRATED ENTRY POINT)
 * Proyek Akhir Pemrograman C++ (Tim 3 Orang)
 *
 * Pembagian Kerja:
 * - Anggota 1: Data Layer (models.h, data_manager.h, data_manager.cpp)
 * - Anggota 2: Business Logic Layer (business_logic.h, business_logic.cpp)
 * - Anggota 3: UI & Controller Layer (ui_views.h, ui_views.cpp, controller.h, controller.cpp)
 * ============================================================================
 */

#include "Anggota_3_UI_Controller_Layer/controller.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // Inisialisasi controller utama aplikasi
    CampingRental::AppController app("data");
    app.run();
    return 0;
}
