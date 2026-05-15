#ifndef ISVESTIS_H
#define ISVESTIS_H

#include "studentas.h"
#include "apdorojimas.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>

/**
 * @brief Parodo pagrindinį meniu ir grąžina vartotojo pasirinkimą.
 * @return Pasirinkimo numeris (1-9).
 */
int menu();

/**
 * @brief Vartotojo pasirinkimas, pagal ką rūšiuoti studentus.
 * @return Pasirinkimas (1-6).
 */
int rusiavimoPasirinkimas();

/**
 * @brief Pasirinkimas, kokį konteinerį naudoti testavimui.
 * @return 1 – STL vector, 2 – deque, 3 – list, 4 - nuosavas vector.
 */
int gautiTipoPasirinkima();

/**
 * @brief Kiek kartų kartoti testavimą.
 * @return Kartų skaičius.
 */
int testavimoPasirinkimas();

/**
 * @brief Pasirinkimas, kurią skirstymo strategiją naudoti.
 * @return 0-3.
 */
int strategijosPasirinkimas();

/**
 * @brief Įvedamas norimas studentų skaičius generavimui.
 * @return Studentų skaičius.
 */
int studentuPasirinkimas();

/**
 * @brief Įvedamas namų darbų rezultatų skaičius generavimui.
 * @return nd rezultatų skaičius.
 */
int ndPasirinkimas();

/**
 * @brief Ar skaičiuoti galutinį pažymį naudojant medianą?
 * @return true – medianą, false – vidurkį.
 */
bool medianosUzklausa();

/**
 * @brief Ar išvesti rezultatus į failą?
 * @return true – į failą, false – į terminalą.
 */
bool failoUzklausa();

/**
 * @brief Suskaičiuoja, kiek papildomų baitų užima lietuviškos raidės (UTF-8).
 * @param eilutė - tekstas.
 * @return Papildomų baitų skaičius (eilutės ilgis – simbolių skaičius).
 */
int lietuviskosRaides(const std::string& eilute);

/**
 * @brief Leidžia vartotojui pasirinkti failą iš esamų .txt failų programos direktorijoje.
 * @param rezervas Išvedamas rezervavimo dydis (jei failo pavadinime yra skaičius).
 * @param egzistuoja Ar rastas bent vienas tinkamas failas.
 * @param failoPavadinimas Pasirinkto failo pavadinimas.
 * @param vieta direktorija, kurioje numatyta ieškoti failų (numatyta ".").
 */
void failoPasirinkimas(int &rezervas, bool &egzistuoja, std::string &failoPavadinimas, const std::string& vieta = ".");

#endif