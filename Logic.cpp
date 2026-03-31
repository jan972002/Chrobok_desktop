//
// Created by jan97 on 08.01.2026.
//
#include "Logic.h"
#include "AppState.h"
#include <iostream>
#include <thread>

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
    // Use struct arrays instead of 15 individual bool/int variables
    // Better than individual vars: easier to scale, loop-based processing, less memory fragmentation
    struct BoolCmd {
        bool* current;
        bool* last;
        const char* onCmd;
    };

    struct IntCmd {
        int* current;
        int* last;
        const char* prefix;
    };

    BoolCmd boolCmds[] = {
        { &state.ruch_przod, new bool(false), "Przod" },
        { &state.ruch_tyl, new bool(false), "Tyl" },
        { &state.ruch_lewo, new bool(false), "Lewo" },
        { &state.ruch_prawo, new bool(false), "Prawo" },
        { &state.skret_lewo, new bool(false), "WMiejscuWLewo" },
        { &state.skret_prawo, new bool(false), "WMiejscuWPrawo" }
    };

    IntCmd intCmds[] = {
        { &state.M_1, new int(-1), "M1:" },
        { &state.M_2, new int(-1), "M2:" },
        { &state.M_3, new int(-1), "M3:" },
        { &state.M_4, new int(-1), "M4:" },
        { &state.M_5, new int(-1), "M5:" },
        { &state.PWM, new int(-1), "PWM:" },
        { &state.silaSkretu, new int(-1), "silaSkretu:" },
        { &state.opoznienieChwytaka, new int(-1), "opoznienieChwytaka:" },
        { &state.opoznienieServa, new int(-1), "opoznienieRuchuServa:" }
    };

    while (state.is_running) {
        // Process all boolean movement commands in a loop
        // Better than if-else chain: O(n) vs repeated comparisons, easier to add new movements
        for (auto& cmd : boolCmds) {
            if (*cmd.current != *cmd.last) {
                *cmd.last = *cmd.current;
                SendCommand(state, *cmd.current ? cmd.onCmd : "STOP");
            }
        }

        // One-off stop commands
        if (state.zatrzymanie) {
            SendCommand(state, "STOP");
            state.zatrzymanie = false;
        }
        if (state.zerowanie_ramienia) {
            SendCommand(state, "ZerowaPozycja");
            state.zerowanie_ramienia = false;
        }

        // Process all integer motor commands in a loop
        // Better than if-else chain: eliminates 9 repeated condition checks
        for (auto& cmd : intCmds) {
            if (*cmd.current != *cmd.last) {
                *cmd.last = *cmd.current;
                SendCommand(state, std::string(cmd.prefix) + std::to_string(*cmd.last));
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // Cleanup
    for (auto& cmd : boolCmds) delete cmd.last;
    for (auto& cmd : intCmds) delete cmd.last;
}