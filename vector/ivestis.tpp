/**
 * @brief Nuskaito studentus iš failo.
 * @tparam Konteineris (STL vector, deque, list, nuosavas vector).
 * @param failoPavadinimas Failo pavadinimas.
 * @param ndKiekis Išvestinis namų darbų skaičius (nustatomas iš antraštės).
 * @param rezervas Preliminarus konteinerio rezervavimo dydis (jei palaikomas).
 * @return Konteineris su nuskaitytais studentais.
 */
template<class Konteineris>
Konteineris skaitymasIsFailo(const std::string &failoPavadinimas, int &ndKiekis, int rezervas){
    Konteineris studentai;
    if constexpr(requires(Konteineris konteineris){konteineris.reserve(0);})
        studentai.reserve(rezervas);
    std::string eil, t;

    std::ifstream open_f(failoPavadinimas);
    if (!open_f.is_open()) throw std::runtime_error("Klaida: failas \"" + failoPavadinimas + "\" nerastas.");

    if(!std::getline(open_f, eil)) throw std::runtime_error("Klaida: failas tuščias arba netinkantis");
    std::istringstream antraste(eil);
    antraste >> t >> t;
    while(antraste >> t){
        if(t == "Egz." || t == "Egzaminas") break;
        ndKiekis++;
    }

    std::string vardas, pavarde;
    int paz;
    
    while (true) {
        Studentas studentas;
        if(!(studentas.readStudentStream(open_f, ndKiekis))) break;
        studentai.push_back(std::move(studentas));
    }

    open_f.close();
    return studentai;
}