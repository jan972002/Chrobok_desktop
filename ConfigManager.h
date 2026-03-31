//
// Created by jan97 on 08.01.2026.
//
#pragma once
#include "AppState.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>

class ConfigManager {
private:
    // Hash-map based parser replaces 12 if-else chains with O(1) lookup
    // Better than if-else: scales to new settings without adding conditions
    static void ParseConfigValue(const std::string& line, AppState& s) {
        size_t delimPos = line.find('=');
        if (delimPos == std::string::npos) return;

        std::string key = line.substr(0, delimPos);
        std::string value = line.substr(delimPos + 1);

        static const std::unordered_map<std::string, std::function<void(const std::string&, AppState&)>> parsers = {
            {"Skala", [](const std::string& v, AppState& s) { s.skala_tekstu = std::stof(v); }},
            {"Monitor", [](const std::string& v, AppState& s) { s.selected_monitor = std::stoi(v); }},
            {"Tlo_R", [](const std::string& v, AppState& s) { s.bgColor[0] = std::stof(v); }},
            {"Tlo_G", [](const std::string& v, AppState& s) { s.bgColor[1] = std::stof(v); }},
            {"Tlo_B", [](const std::string& v, AppState& s) { s.bgColor[2] = std::stof(v); }},
            {"Txt_R", [](const std::string& v, AppState& s) { s.textColor[0] = std::stof(v); }},
            {"Txt_G", [](const std::string& v, AppState& s) { s.textColor[1] = std::stof(v); }},
            {"Txt_B", [](const std::string& v, AppState& s) { s.textColor[2] = std::stof(v); }},
            {"Show_Ramie", [](const std::string& v, AppState& s) { s.show_sterowanie_ramienia = std::stoi(v); }},
            {"Show_Param", [](const std::string& v, AppState& s) { s.show_parametry_ramienia = std::stoi(v); }},
            {"Show_Ruch", [](const std::string& v, AppState& s) { s.show_sterowanie_ruchem = std::stoi(v); }},
            {"Show_Detekcja", [](const std::string& v, AppState& s) { s.show_detekcja_obrazu = std::stoi(v); }},
            {"Show_Lidar", [](const std::string& v, AppState& s) { s.show_odczyt_lidar = std::stoi(v); }},
            {"Show_FPS", [](const std::string& v, AppState& s) { s.show_debug_info = std::stoi(v); }}
        };

        auto it = parsers.find(key);
        if (it != parsers.end()) {
            it->second(value, s);
        }
    }

public:
    static void Zapisz(const AppState& s) {
        std::ofstream plik("config.ini");
        if (!plik.is_open()) return;
        plik << "[Interfejs]\nSkala=" << s.skala_tekstu << "\nMonitor=" << s.selected_monitor << "\n";
        plik << "[Kolory]\nTlo_R=" << s.bgColor[0] << "\nTlo_G=" << s.bgColor[1] << "\nTlo_B=" << s.bgColor[2] << "\n";
        plik << "Txt_R=" << s.textColor[0] << "\nTxt_G=" << s.textColor[1] << "\nTxt_B=" << s.textColor[2] << "\n";
        plik << "[Widocznosc]\nShow_Ramie=" << s.show_sterowanie_ramienia << "\nShow_Param=" << s.show_parametry_ramienia << "\n";
        plik << "Show_Ruch=" << s.show_sterowanie_ruchem << "\nShow_Detekcja=" << s.show_detekcja_obrazu << "\n";
        plik << "Show_Lidar=" << s.show_odczyt_lidar << "\nShow_FPS=" << s.show_debug_info << "\n";
        plik.close();
    }

    static void Laduj(AppState& s) {
        std::ifstream plik("config.ini");
        if (!plik.is_open()) return;
        std::string ln;
        while (std::getline(plik, ln)) {
            ParseConfigValue(ln, s);
        }
    }
};

