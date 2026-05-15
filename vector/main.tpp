#include "main.h"
#include "apdorojimas.tpp"
#include "isvestis.tpp"

/**
 * @brief Pasirenka skirstymo strategiją.
 * @tparam Konteineris (list, STL vector, deque, nuosavas vector).
 * @param sPasirinkimas Strategijos numeris (0-3).
 * @param studentai Studentų konteineris.
 * @param galvociai Konteineris studentams, kurių galutinis >=5.
 * @param vargsiukai Konteineris studentams, kurių galutinis <5.
 * @param medianos Ar naudoti medianą galutiniam pažymiui.
 * @param ndKiekis Namų darbų skaičius.
 */
template <class Konteineris>
void skirstymoPasirinkimas(int sPasirinkimas, Konteineris &studentai, Konteineris &galvociai, Konteineris &vargsiukai);

/**
 * @brief Atlieka failo testavimą su duotu konteinerio tipu.
 * @tparam Konteineris (STL vector, deque, list, nuosavas vector).
 * @param failoPavadinimas Failo pavadinimas.
 * @param rezervas Rezervavimo dydis.
 * @param tPasirinkimas Kiek kartų testuoti.
 * @param sPasirinkimas Skirstymo strategija.
 * @param ndKiekis Namų darbų skaičius.
 * @param medianos Ar naudoti medianą galutiniam pažymiui.
 */
template <class Konteineris>
void failoTestavimas(const std::string &failoPavadinimas, int rezervas, int tPasirinkimas, int sPasirinkimas, int &ndKiekis, bool medianos)
{
    double skaitymoTrukme = 0;
    double skaiciavimoTrukme = 0;
    double rusiavimoTrukme = 0;
    double skirstymoTrukme = 0;
    double isvedimoTrukme1 = 0;
    double isvedimoTrukme2 = 0;
    ndKiekis = 0;
    Konteineris studentai = skaitymasIsFailo<Konteineris>(failoPavadinimas, ndKiekis, rezervas); // nuskaitymas
/*     skaiciavimas<Konteineris>(studentai, medianos, ndKiekis);  */// rezultatu apsiskaiciavimas pries rikiavima
    rusiavimasSkirstymas(studentai, 5, medianos, ndKiekis); // rusiavimas didejanciai
    Konteineris vargsiukai;
    Konteineris galvociai;
    if constexpr(requires(Konteineris konteineris){konteineris.reserve(0);}){
        galvociai.reserve(studentai.size());
        vargsiukai.reserve(studentai.size());
    }
    skirstymoPasirinkimas(sPasirinkimas, studentai, galvociai, vargsiukai, medianos, ndKiekis);
    /*     isvestis(galvociai, medianos, true, "galvociai.txt");
        isvestis(vargsiukai, medianos, true, "vargsiukai.txt"); */
    for(int i = 0; i < tPasirinkimas; i++)
    {   
        ndKiekis = 0;
        Timer t;
        Konteineris studentai = skaitymasIsFailo<Konteineris>(failoPavadinimas, ndKiekis, rezervas); // nuskaitymas
        skaitymoTrukme += t.elapsed(); // Skirtumas (s)
/*         t.reset();
        skaiciavimas<Konteineris>(studentai, medianos, ndKiekis); // rezultatu apsiskaiciavimas pries rikiavima
        skaiciavimoTrukme += t.elapsed(); */
        t.reset();
        rusiavimasSkirstymas(studentai, 5, medianos, ndKiekis); // rusiavimas didejanciai
        rusiavimoTrukme += t.elapsed();
        t.reset();
        Konteineris vargsiukai;
        Konteineris galvociai;
        if constexpr(requires(Konteineris konteineris){konteineris.reserve(0);}){
            galvociai.reserve(studentai.size());
            vargsiukai.reserve(studentai.size());
        }
        skirstymoPasirinkimas(sPasirinkimas, studentai, galvociai, vargsiukai, medianos, ndKiekis);
        skirstymoTrukme += t.elapsed();
        t.reset();
       isvestis(galvociai, medianos, true, "galvociai.txt");
        isvedimoTrukme1 += t.elapsed(); // Skirtumas (s)
        t.reset();
        isvestis(vargsiukai, medianos, true, "vargsiukai.txt");
        isvedimoTrukme2 += t.elapsed(); // Skirtumas (s)
    }
    std::cout << "Failo nuskaitymas į studentai konteinerį vidutiniškai užtruko: " << skaitymoTrukme/tPasirinkimas << " s\n";
/*     std::cout << "Rezultatų skaičiavimas vidutiniškai užtruko: " << skaiciavimoTrukme/tPasirinkimas << " s\n"; */
    std::cout << "Duomenų rūšiavimas didėjančiai vidutiniškai užtruko: " << rusiavimoTrukme/tPasirinkimas << " s\n";
    std::cout << "Studentų skirstymas pagal pažymius vidutiniškai užtruko: " << skirstymoTrukme/tPasirinkimas << " s\n";
    std::cout << "Studentų išvedimas į galvociai.txt vidutiniškai užtruko: " << isvedimoTrukme1/tPasirinkimas << " s\n";
    std::cout << "Studentų išvedimas į vargsiukai.txt vidutiniškai užtruko: " << isvedimoTrukme2/tPasirinkimas << " s\n";
    std::cout << "Bendra trukmė: " << skaitymoTrukme + skaiciavimoTrukme + rusiavimoTrukme + skirstymoTrukme + isvedimoTrukme1 + isvedimoTrukme2 << " s\n";
}

/**
 * @brief Įgyvendina skirstymo strategijos pasirinkimą.
 * @tparam Konteineris (STL vector, deque, list, nuosavas vector).
 */
template <class Konteineris>
void skirstymoPasirinkimas(int sPasirinkimas, Konteineris &studentai, Konteineris &galvociai, Konteineris &vargsiukai, bool medianos, int ndKiekis = 0)
{
    switch (sPasirinkimas)
    {
        case 0:
        {
            skirstymas(studentai, galvociai, vargsiukai, medianos, ndKiekis);
            break;
        }
        case 1:
        {
            skirstymasStrat1(studentai, galvociai, vargsiukai, medianos, ndKiekis);
            break;
        }
        case 2:
        {
            skirstymasStrat2(studentai, vargsiukai, medianos, ndKiekis);
            break;
        }
        case 3:
        {
            skirstymasStrat3(studentai, vargsiukai, medianos, ndKiekis);
            break;
        }
        default:
        {
            break;
        }
    }
}
