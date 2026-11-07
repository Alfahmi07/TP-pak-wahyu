@echo off
title SISTEM PENYEWAAN ALAT CAMPING - BUILD & RUNNER
color 0A

echo =========================================================================
echo       BUILD & RUNNER SISTEM PENYEWAAN ALAT CAMPING (C++17)
echo =========================================================================
echo.
echo Pilih opsi kompilasi:
echo [1] Kompilasi & Jalankan File Gabungan Standalone (main_gabungan.cpp - 5000 lines)
echo [2] Jalankan Tes Diagnostik Otomatis File Gabungan (main_gabungan.cpp --test)
echo [3] Kompilasi & Jalankan Versi Modular Terintegrasi (3 Layer)
echo [4] Jalankan Unit Test Mandiri Anggota 1 (Data Layer)
echo [5] Jalankan Unit Test Mandiri Anggota 2 (Business Logic Layer)
echo [6] Jalankan Pengujian Visual Mock Anggota 3 (UI Layer)
echo [0] Keluar
echo.
set /p choice="Masukkan pilihan [0-6]: "

if "%choice%"=="1" (
    echo.
    echo Mengompilasi main_gabungan.cpp...
    g++ -std=c++17 -Wall main_gabungan.cpp -o rental_camping.exe
    if %errorlevel% neq 0 (
        echo [ERROR] Kompilasi gagal!
        pause
        exit /b %errorlevel%
    )
    echo [SUKSES] Kompilasi berhasil! Menjalankan aplikasi...
    echo.
    .\rental_camping.exe
    pause
    exit /b 0
)

if "%choice%"=="2" (
    echo.
    echo Mengompilasi & Menjalankan Tes Diagnostik Mandiri...
    g++ -std=c++17 -Wall main_gabungan.cpp -o rental_camping.exe
    if %errorlevel% neq 0 (
        echo [ERROR] Kompilasi gagal!
        pause
        exit /b %errorlevel%
    )
    .\rental_camping.exe --test
    pause
    exit /b 0
)

if "%choice%"=="3" (
    echo.
    echo Mengompilasi versi modular 3 layer...
    g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_2_Business_Logic_Layer/business_logic.cpp Anggota_3_UI_Controller_Layer/ui_views.cpp Anggota_3_UI_Controller_Layer/controller.cpp main.cpp -o rental_modular.exe
    if %errorlevel% neq 0 (
        echo [ERROR] Kompilasi modular gagal!
        pause
        exit /b %errorlevel%
    )
    echo [SUKSES] Kompilasi modular berhasil! Menjalankan aplikasi...
    echo.
    .\rental_modular.exe
    pause
    exit /b 0
)

if "%choice%"=="4" (
    echo.
    echo Mengompilasi & Menjalankan Unit Test Anggota 1 (Data Layer)...
    g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_1_Data_Layer/main_test.cpp -o test_data_layer.exe
    .\test_data_layer.exe
    pause
    exit /b 0
)

if "%choice%"=="5" (
    echo.
    echo Mengompilasi & Menjalankan Unit Test Anggota 2 (Business Logic)...
    g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_2_Business_Logic_Layer/business_logic.cpp Anggota_2_Business_Logic_Layer/main_test.cpp -o test_bll.exe
    .\test_bll.exe
    pause
    exit /b 0
)

if "%choice%"=="6" (
    echo.
    echo Mengompilasi & Menjalankan UI Mock Anggota 3 (UI Views)...
    g++ -std=c++17 -Wall Anggota_1_Data_Layer/data_manager.cpp Anggota_2_Business_Logic_Layer/business_logic.cpp Anggota_3_UI_Controller_Layer/ui_views.cpp Anggota_3_UI_Controller_Layer/main_menu_mock.cpp -o test_ui.exe
    .\test_ui.exe
    pause
    exit /b 0
)

echo Selesai.
