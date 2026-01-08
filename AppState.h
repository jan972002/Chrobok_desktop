//
// Created by jan97 on 08.01.2026.
//
#pragma once
#include<string>
#include<vector>
#include<mutex>

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
    bool show_aboutApp_info = false;
    bool show_aboutUs_info = false;
    bool show_aboutProject_info = false;
    bool show_console = true;

    bool is_running = false;

    // Stany ruchu
    bool ruch_przod = false;
    bool ruch_tyl = false;
    bool ruch_lewo = false;
    bool ruch_prawo = false;
    bool skret_lewo = false;
    bool skret_prawo = false;
    bool zatrzymanie = false;
    bool zerowanie_ramienia = false;

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

    // Dodatkowe
    int dummy = 127;
    std::string nazwaAplikacji = "Chrobok Desktop";
    std::string dataWydania = "09.01.2025";
    std::string wersjaAplikacji = "1.0.1-beta";
    std::string oNas = "agjnoaijfaifgj aifaijf afijfij  aifjfa0iwafiajfa0 aifjai0jfafa  afuaiffaij aijfaifja0f aif0ja0ijfa";
    std::string oProjekcie = "agjnoaijfaifgj aifaijf afijfij  aifjfa0iwafiajfa0 aifjai0jfafa  afuaiffaij aijfaifja0f aif0ja0ijfa ";

    std::vector<std::string> historia_komend;
    int max_logow = 5;
};

