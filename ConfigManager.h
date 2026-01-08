//
// Created by jan97 on 08.01.2026.
//
#pragma once
#include "AppState.h"
#include <fstream>
#include <string>

class ConfigManager {
public:
    static void Zapisz(const AppState& s) {
        std::ofstream plik("config.ini");
        if (!plik.is_open()) return;
        plik << "[Interfejs]\nSkala=" << s.skala_tekstu << "\nMonitor=" << s.selected_monitor << "\n";
        plik << "[Kolory]\n" << "Tlo_R=" << s.bgColor[0] << "\nTlo_G=" << s.bgColor[1] << "\nTlo_B=" << s.bgColor[2] << "\n";
        plik << "Txt_R=" << s.textColor[0] << "\nTxt_G=" << s.textColor[1] << "\nTxt_B=" << s.textColor[2] << "\n";
        plik << "[Widocznosc]\n" << "Show_Ramie=" << s.show_sterowanie_ramienia << "\nShow_Param=" << s.show_parametry_ramienia << "\n";
        plik << "Show_Ruch=" << s.show_sterowanie_ruchem << "\nShow_Detekcja=" << s.show_detekcja_obrazu << "\n";
        plik << "Show_Lidar=" << s.show_odczyt_lidar << "\nShow_FPS=" << s.show_debug_info << "\n";
        plik.close();
    }

    static void Laduj(AppState& s) {
        std::ifstream plik("config.ini");
        if (!plik.is_open()) return;
        std::string ln;
        while (std::getline(plik, ln)) {
            if (ln.find("Skala=") == 0) s.skala_tekstu = std::stof(ln.substr(6));
            else if (ln.find("Monitor=") == 0) s.selected_monitor = std::stoi(ln.substr(8));
            else if (ln.find("Tlo_R=") == 0) s.bgColor[0] = std::stof(ln.substr(6));
            else if (ln.find("Tlo_G=") == 0) s.bgColor[1] = std::stof(ln.substr(6));
            else if (ln.find("Tlo_B=") == 0) s.bgColor[2] = std::stof(ln.substr(6));
            else if (ln.find("Txt_R=") == 0) s.textColor[0] = std::stof(ln.substr(6));
            else if (ln.find("Txt_G=") == 0) s.textColor[1] = std::stof(ln.substr(6));
            else if (ln.find("Txt_B=") == 0) s.textColor[2] = std::stof(ln.substr(6));
            else if (ln.find("Show_Ramie=") == 0) s.show_sterowanie_ramienia = std::stoi(ln.substr(11));
            else if (ln.find("Show_Param=") == 0) s.show_parametry_ramienia = std::stoi(ln.substr(11));
            else if (ln.find("Show_Ruch=") == 0) s.show_sterowanie_ruchem = std::stoi(ln.substr(10));
            else if (ln.find("Show_Detekcja=") == 0) s.show_detekcja_obrazu = std::stoi(ln.substr(14));
            else if (ln.find("Show_Lidar=") == 0) s.show_odczyt_lidar = std::stoi(ln.substr(11));
            else if (ln.find("Show_FPS=") == 0) s.show_debug_info = std::stoi(ln.substr(9));
        }
    }
};

