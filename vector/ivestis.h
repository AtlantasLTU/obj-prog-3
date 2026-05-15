#ifndef IVESTIS_H
#define IVESTIS_H

#include "studentas.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ivestis.tpp"

/**
 * @brief Paprašo vartotojo patvirtinimo (y/n).
 * @param pranesimas Tekstas, rodomas prieš įvedimą.
 * @return true jei 'y', false jei 'n'.
 */
bool gautiPatvirtinima(const std::string &pranesimas);

/**
 * @brief Nuskaito studentus iš standartinės įvesties iki EOF.
 * @return Vektorius su nuskaitytais studentais.
 */
std::vector<Studentas> ivestiStudentus();

/**
 * @brief Nuskaito vieną studentą iš srauto (naudojama skaitymui iš failo ar konsolės).
 * @param A Studento objektas, kur bus įrašyti duomenys.
 * @return true jei nuskaitymas sėkmingas.
 */
bool skaitymas(Studentas &A);

/**
 * @brief Nuskaito vardą ir pavardę iš eilutės.
 * @param A Studento objektas.
 * @param eilute Eilutė su vardu ir pavarde.
 * @return true jei nuskaityta sėkmingai.
 */
bool studentoVardoPavardesIvestis(Studentas &A, const std::string &eilute);

/**
 * @brief Nuskaito namų darbų rezultatus (terminale).
 * @param A Studento objektas.
 */
void namuDarbuRezultatuIvestis(Studentas &A);

/**
 * @brief Nuskaito egzamino rezultatą (terminale).
 * @param A Studento objektas.
 */
void egzaminoRezultatoIvestis(Studentas &A);

/**
 * @brief Nuskaito sveikąjį skaičių iš konsolės su patikrinimais.
 * @param pranešimas - tekstas, rodomas prieš įvedimą.
 * @param min Mažiausia leistina reikšmė.
 * @param max Didžiausia leistina reikšmė.
 * @param galiButiTuscia Ar galima įvesti tuščią eilutę (grąžina -1).
 * @return Įvestas skaičius.
 */
int gautiSkaiciu(const std::string &pranešimas, int min, int max, bool galiButiTuscia = false);

/**
 * @brief Patikrina, ar eilutę sudaro tik skaitmenys ir tarpai.
 * @param s Tikrinama eilutė.
 * @return true jei taip.
 */
bool arTikSkaicius(const std::string &s);

/**
 * @brief Nuskaito studentus iš failo.
 * @tparam Konteineris (STL vector, deque, list, nuosavas vector).
 * @param failoPavadinimas Failo pavadinimas.
 * @param ndKiekis Išvestinis namų darbų skaičius (nustatomas iš antraštės).
 * @param rezervas Preliminarus konteinerio rezervavimo dydis (jei palaikomas).
 * @return Konteineris su nuskaitytais studentais.
 */
template<class Konteineris>
Konteineris skaitymasIsFailo(const std::string &failoPavadinimas, int &ndKiekis, int rezervas);

/**
 * @brief Apdoroja EOF atvejį cin sraute.
 */
void cinEOFgaudymas();

#endif