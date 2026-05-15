#include "main.h"

int main()
{
    try
    {
        #ifdef _WIN32 // Jei kompiliuojama Windows operacinei sistemai nustatyti konsoles įvestį ir išvestį UTF-8 užkodavimui.
        SetConsoleOutputCP(CP_UTF8); // pakeičiame išvesties code page į UTF-8
        SetConsoleCP(CP_UTF8); // pakeičiame įvesties code page į UTF-8
        #endif
        bool veikimas = true;
        while(veikimas)
        {
            //pasirinkima galima tobulint su enumeratorium del type safety ir jei butu norima valdyti atminti.
            int pasirinkimas = menu();
            bool failas = false;
            bool medianos = false;
            if(pasirinkimas<5)
            {
                failas = !failoUzklausa();
                medianos = medianosUzklausa();
            } else if(pasirinkimas == 5){
                medianos = medianosUzklausa();
            }

            switch(pasirinkimas){
                case 1: // rankinis ivedimas
                {
                    std::vector<Studentas> studentai = ivestiStudentus();
/*                     skaiciavimas(studentai, medianos); */
                    isvestis(studentai, medianos, failas);
                    break;
                }
                case 2: // tik pazymiu generavimas.
                {
                    std::vector<Studentas> studentai = ivestiStudentusRandom(pasirinkimas);
/*                     skaiciavimas(studentai, medianos); */
                    isvestis(studentai, medianos, failas);
                    break;
                }
                case 3: // studentu ir pazymiu generavimas;
                {
                    std::vector<Studentas> studentai = ivestiStudentusRandom(pasirinkimas);
/*                     skaiciavimas(studentai, medianos); */
                    isvestis(studentai, medianos, failas);
                    break;
                }
                case 4: // skaitymas is failo
                {
                    int ndKiekis = 0;
                    int rezervas = 0;
                    std::string fPasirinkimas;
                    bool egzistuojaFailai = true;
                    failoPasirinkimas(rezervas, egzistuojaFailai, fPasirinkimas);
                    if(egzistuojaFailai == false) break;
                    int rPasirinkimas = rusiavimoPasirinkimas();
                    failoApdorojimas(fPasirinkimas, rezervas, ndKiekis, medianos, rPasirinkimas, failas);
                    break;
                }
                case 5: // testavimas su failais // kiekviename test case'e uzkomentuota koda arba jo dalis galima atkomentuoti bei keisti parametrus, kad pakeisti kas yra testuojama, kadangi tiksliai neapibrezta pagal ka testuoti.
                {
                    int ndKiekis = 0;
                    int rezervas = 0;
                    std::string fPasirinkimas;
                    bool egzistuojaFailai = true;
                    failoPasirinkimas(rezervas, egzistuojaFailai, fPasirinkimas);
                    if(egzistuojaFailai == false) break;
                    int tipoPasirinkimas = gautiTipoPasirinkima();
                    int tPasirinkimas = testavimoPasirinkimas();
                    int sPasirinkimas = strategijosPasirinkimas();
                    if(tPasirinkimas <= 0) throw std::invalid_argument("Testavimo skaičius turi būti daugiau už 0!");
                    switch(tipoPasirinkimas){
                        case 1:
                        {
                            failoTestavimas<std::vector<Studentas>>(fPasirinkimas, rezervas, tPasirinkimas, sPasirinkimas, ndKiekis, medianos);
                            break;
                        }
                        case 2:
                        {
                            failoTestavimas<std::deque<Studentas>>(fPasirinkimas, rezervas, tPasirinkimas, sPasirinkimas, ndKiekis, medianos);
                            break;
                        }
                        case 3:
                        {
                            failoTestavimas<std::list<Studentas>>(fPasirinkimas, rezervas, tPasirinkimas, sPasirinkimas, ndKiekis, medianos);
                            break;
                        }
                        case 4:
                        {
                            failoTestavimas<Vector<Studentas>>(fPasirinkimas, rezervas, tPasirinkimas, sPasirinkimas, ndKiekis, medianos);
                            break;
                        }
                        default:
                        {

                        }
                    }
                    break;
                }
                case 6: // failo generavimas
                {   
                    int studentuKiekis = studentuPasirinkimas();
                    int ndKiekis = ndPasirinkimas();
                    int tPasirinkimas = testavimoPasirinkimas();
                    if(tPasirinkimas <= 0) throw std::invalid_argument("Testavimo skaičius turi būti daugiau už 0!");
                    double trukme = 0;
                    for(int i = 0; i < tPasirinkimas+1; i++)
                    {
                        if(i!=0)
                        {
                            Timer t;
                            failoGeneravimas(studentuKiekis, ndKiekis);
                            trukme += t.elapsed();
                        }
                        else
                        {
                            failoGeneravimas(studentuKiekis, ndKiekis);
                        }
                    }
                    std::cout << "Failų(-o) generavimas vidutiniškai užtruko: " << trukme/tPasirinkimas << " s\n";
                    break;
                }
                case 7: // studento klasės testas
                {
                    testas();
                    break;
                }
                case 8: // nuosavo vektoriaus ir STL vektoriaus testas
                {
                    vector_compare();
                    break;
                }
                case 9: // darbo baigtis
                {
                    std::cout << "Darbas su programa baigtas.";
                    veikimas = false;
                    return 0;
                }
                default:
                {
                    std::cout << "How did we get here?" << std::endl; // https://minecraft.wiki/w/Tutorial:Advancement_guide/How_Did_We_Get_Here%3F
                    return 0;
                }
            }
        }
    }
    catch(const std::exception &klaida)
    {
        std::cerr << klaida.what() << "\n";
        return 1;
    }
    return 0;
}

void failoApdorojimas(const std::string &failoPavadinimas, int rezervas, int &ndKiekis, bool medianos, int rPasirinkimas, bool failas)
{
    Timer t;
    std::vector<Studentas> studentai = skaitymasIsFailo<std::vector<Studentas>>(failoPavadinimas, ndKiekis, rezervas);
    double skaitymoTrukme = t.elapsed(); // Skirtumas (s)
/*     t.reset();
    skaiciavimas(studentai, medianos, ndKiekis);
    double skaiciavimoTrukme = t.elapsed(); */
    t.reset();
    rusiavimasSkirstymas(studentai, rPasirinkimas, medianos);
    double rusiavimoTrukme = t.elapsed();
    t.reset();
    isvestis(studentai, medianos, failas);
    double isvedimoTrukme = t.elapsed(); // Skirtumas (s)
    std::cout << "Failo nuskaitymas į studentai vektorių užtruko: " << skaitymoTrukme << " s\n";
/*     std::cout << "Rezultatų skaičiavimas užtruko: " << skaiciavimoTrukme << " s\n"; */
    std::cout << "Duomenų rūšiavimas pagal pasirinktą parametrą užtruko: " << rusiavimoTrukme << " s\n";
    std::cout << "Studentų išvedimas užtruko: " << isvedimoTrukme << " s\n";
    std::cout << "Bendra trukmė: " << skaitymoTrukme + /* skaiciavimoTrukme + */ rusiavimoTrukme + isvedimoTrukme << " s\n";
}