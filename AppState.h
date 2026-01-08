//
// Created by jan97 on 08.01.2026.
//
#pragma once

struct AppState {
    // Interfejs
    float skala_tekstu = 2.0f;
    float bgColor[3] = { 0.1f, 0.1f, 0.1f };
    float textColor[3] = { 1.0f, 1.0f, 1.0f };
    int selected_monitor = 0;

    // Widoczność okien
    bool show_sterowanie_ramienia = false;
    bool show_parametry_ramienia = false;
    bool show_sterowanie_ruchem = false;
    bool show_detekcja_obrazu = false;
    bool show_odczyt_lidar = false;
    bool show_ustawienia_aplikacji = false;
    bool show_debug_info = false;

    // Zmienne Logiczne
    int opoznienieChwytaka = 3; // 0-5
    int opoznienieServa = 10; // 0-20
    int silaSkretu = 50; //0-100
    int PWM = 128; // 0-255

    // Zmienne Serv
    int M_1 = 500; // 500-2380
    int M_2 = 75; // 0-150
    int M_3 = 30; // 0-180
    int M_4 = 180; // 0-180
    int M_5 = 90; // 0-180

    // Dummy
    int dummy = 127;
};

