#pragma once
// Moduł logiki — funkcje pomocnicze sterowania i logowania, wszelkie cudeńka uwu
// Created by jan97 on 08.01.2026. (zaktualizowano komentarze na polskie, owo)
#include "AppState.h"

class Logic{
public:
    static void ZerowanieRamienia(AppState&);
    static void SendCommand(AppState& state, const std::string& cmd);

    static void ParseCommand(AppState& state);

    static void AddLog(AppState& state, const std::string& cmd);
};