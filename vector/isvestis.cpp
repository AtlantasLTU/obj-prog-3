#include "isvestis.h"
#include "ivestis.h"

int menu()
{
    return gautiSkaiciu("Pasirinkite programos eigą:\n1 - ranką,\n2 - generuoti tik pažymius,\n3 - generuoti studentų vardus, pavardės ir pažymius,\n4 - skaityti studentus iš failo,\n5 - testavimas su failais,\n6 - generuoti failą,\n7 - testuoti klasę Studentas,\n8 - nuosavo vektoriaus ir STL vektoriaus palyginimas,\n9 - baigti darbą: ", 1, 9);
}

int rusiavimoPasirinkimas()
{
    return gautiSkaiciu("Pasirinkite pagal ką rūšiuoti: \n1 - vardą (A->Ž),\n2 - vardą (Ž->A),\n3 - pavardę (A->Ž),\n4 - pavardę (Ž->A),\n5 - galutinį pažymį didėjančiai (1->10),\n6 - galutinį pažymį mažėjančiai (10->1)\n", 1, 6);
}

int gautiTipoPasirinkima()
{
    return gautiSkaiciu("Kokį konteinerio tipą norite testuoti?\n1 - STL vektorių,\n2 - deque,\n3 - list,\n4 - nuosavas vektorius: ", 1, 4);
}

int testavimoPasirinkimas()
{
    return gautiSkaiciu("Kiek kartų testuoti failą? (1-100):", 1, 100);
}

int strategijosPasirinkimas()
{
    return gautiSkaiciu("Pagal kokią strategiją testuoti?\n0 - pradiniame relize naudota strategija,\n1 - pirma strategija,\n2 - antra strategija,\n3 - trečia strategija:", 0, 3);
}

int studentuPasirinkimas()
{
    return gautiSkaiciu("Įveskite norimą generuoti studentų kiekį (1-10000000):", 1, 10000000);
}

int ndPasirinkimas()
{
    return gautiSkaiciu("Įveskite norimą generuoti namų darbų rezultatų kiekį (0-10000000): ", 0, 10000000);
}

bool medianosUzklausa()
{
    return gautiPatvirtinima("Skaičiuoti tik medianas? Jei ne, tai galutinis rezultatas bus skaičiuojamas su vidurkiu");
}

bool failoUzklausa()
{
    return gautiPatvirtinima("Ar išvesti į terminalą? Jei ne, tai bus išvedama į rezultatai.txt failą");
}

// apskaiciuoti kiek string su lietuviskomis raidemis sudaro baitu, kadangi viena lietuviska raide - 2 baitai, o ne 1 baitas. Kitaip sakant vardas Ąžuolas turi 7 raides, o jį sudaro 8 baitai, o setw mato baitus, tai jei setw(20), tai jis pridės 12 tusciu tarpu, o ne 13.
int lietuviskosRaides(const std::string& eilute)
{
    int simboliuKiekis = 0;
    for (char c : eilute)
    {
        // jei baitas neprasideda su 10xxxxxx, tai naujas simbolis
        if ((c & 0xC0) != 0x80)
        { // paprastas ASCII simbolis prasideda su 0, keliu baitu pvz lietuviskos raides prasideda su 11 arba 111 arba 1111, priklausomai nuo kodavimo | 0xC0 = 11000000, 0x80 = 10000000. & (AND) bit'u operacija atranda ar c prasideda su 0 ar 1. antras, trecias ar ketvirtas baitas UTF-8 kodavime visad prasides su 10xxxxxx
            simboliuKiekis++;
        }
    }
    // grazinam trukstama isvesties ploti.
    return eilute.length() - simboliuKiekis;
}

void failoPasirinkimas(int &rezervas, bool &egzistuoja, std::string &failoPavadinimas, const std::string& vieta)
{
    std::vector<std::filesystem::directory_entry> failai;
    std::vector<int> rezervai;

    for(auto &failas : std::filesystem::directory_iterator(vieta))
    {
        if(!failas.is_regular_file())
            continue;

        if(failas.path().extension() != ".txt")
            continue;
        
        std::string vardas = failas.path().filename().string();

        if(vardas == "rezultatai.txt" || vardas == "galvociai.txt" || vardas == "vargsiukai.txt")
            continue;

        if(vardas.find("studentai", 0) == 0)
        {
            std::string skaicius = vardas.substr(9, vardas.size() - 9 - 4);

            if(skaicius.empty() || !std::all_of(skaicius.begin(), skaicius.end(), [](unsigned char simbolis)
            {
                return std::isdigit(simbolis);
            }))
            {
                rezervai.push_back(0);
            } 
            else
            {
                rezervai.push_back(std::stoi(skaicius));
            }
        } else {
            rezervai.push_back(0);
        }

        failai.push_back(failas);
    }

    if(failai.empty())
    {
        std::cout << "Nerasta tekstinių failų vietoje." << vieta << "\n";
        egzistuoja = false;
    } else {

    std::cout << "Pasirinkite failą:\n";
    for(int i = 0; i < failai.size(); i++)
    {
        std::cout << (i + 1) << ": " << failai.at(i).path().filename().string() << "\n";
    }

    int pasirinkimas = gautiSkaiciu("Įveskite failo numerį: ", 1, failai.size(), false);
    failoPavadinimas = failai.at(pasirinkimas-1).path().filename().string();
    rezervas = rezervai.at(pasirinkimas-1);
    }
}