//
// Created by jan97 on 08.01.2026.
//
#include "Logic.h"
#include "AppState.h"
#include <iostream>
#include<thread>

void Logic::ZerowanieRamienia(AppState& state) {
    state.M_1 = 500;
    state.M_2 = 75;
    state.M_3 = 30;
    state.M_4 = 180;
    state.M_5 = 90;
    state.zerowanie_ramienia = true;
}

void Logic::AddLog(AppState& state, std::string cmd) {
    state.historia_komend.push_back(cmd);
    if (state.historia_komend.size() > state.max_logow) {
        state.historia_komend.erase(state.historia_komend.begin());
    }
}

void Logic::SendCommand(AppState& state, std::string cmd) {
    std::string packet = cmd + "\n";
    AddLog(state, cmd);
    std::cout << "[WYSLANO]: " << packet;
}

void Logic::ParseCommand(AppState& state) {

    bool last_przod = false;
    bool last_tyl = false;
    bool last_lewo = false;
    bool last_prawo = false;
    bool last_skret_lewo = false;
    bool last_skret_prawo = false;

    int last_M1 = -1;
    int last_M2 = -1;
    int last_M3 = -1;
    int last_M4 = -1;
    int last_M5 = -1;
    int last_pwm = -1;
    int last_sila_skretu = -1;
    int last_opoznienie_chwytaka = -1;
    int last_oposnienie_serwa = -1;

    while (state.is_running) {
        if (state.ruch_przod != last_przod) {
            last_przod = state.ruch_przod;
            last_przod ? SendCommand(state,"Przod") : SendCommand(state,"STOP");
        }
        if (state.ruch_tyl != last_tyl) {
            last_tyl = state.ruch_tyl;
            last_tyl ? SendCommand(state,"Tyl") : SendCommand(state,"STOP");
        }
        if (state.ruch_lewo != last_lewo) {
            last_lewo = state.ruch_lewo;
            last_lewo ? SendCommand(state,"Lewo") : SendCommand(state,"STOP");
        }
        if (state.ruch_prawo != last_prawo) {
            last_prawo = state.ruch_prawo;
            last_prawo ? SendCommand(state,"Prawo") : SendCommand(state,"STOP");
        }
        if (state.skret_lewo != last_skret_lewo) {
            last_skret_lewo = state.skret_lewo;
            last_skret_lewo ? SendCommand(state,"WMiejscuWLewo") : SendCommand(state,"STOP");
        }
        if (state.skret_prawo != last_skret_prawo) {
            last_skret_prawo = state.skret_prawo;
            last_skret_prawo ? SendCommand(state,"WMiejscuWPrawo") : SendCommand(state,"STOP");
        }
        if (state.zatrzymanie) {
            SendCommand(state,"STOP");
            state.zatrzymanie = false;
        }
        if (state.zerowanie_ramienia) {
            SendCommand(state,"ZerowaPozycja");
            state.zerowanie_ramienia = false;
        }

        if (state.M_1 != last_M1) { last_M1 = state.M_1; SendCommand(state,"M1:" + std::to_string(last_M1)); }
        if (state.M_2 != last_M2) { last_M2 = state.M_2; SendCommand(state,"M2:" + std::to_string(last_M2)); }
        if (state.M_3 != last_M3) { last_M3 = state.M_3; SendCommand(state,"M3:" + std::to_string(last_M3)); }
        if (state.M_4 != last_M4) { last_M4 = state.M_4; SendCommand(state,"M4:" + std::to_string(last_M4)); }
        if (state.M_5 != last_M5) { last_M5 = state.M_5; SendCommand(state,"M5:" + std::to_string(last_M5)); }

        if (state.PWM != last_pwm) { last_pwm = state.PWM; SendCommand(state,"PWM:" + std::to_string(last_pwm)); }
        if (state.silaSkretu != last_sila_skretu) { last_sila_skretu = state.silaSkretu; SendCommand(state,"silaSkretu:" + std::to_string(last_sila_skretu)); }
        if (state.opoznienieChwytaka != last_opoznienie_chwytaka) { last_opoznienie_chwytaka = state.opoznienieChwytaka; SendCommand(state,"opoznienieChwytaka:" + std::to_string(last_opoznienie_chwytaka)); }
        if (state.opoznienieServa != last_oposnienie_serwa) { last_oposnienie_serwa = state.opoznienieServa; SendCommand(state,"opoznienieRuchuServa:" + std::to_string(last_oposnienie_serwa)); }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}