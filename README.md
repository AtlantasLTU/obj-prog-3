# Programos naudojimas:
  ## Reikalavimai:
  Paprastiems vartotojams:
    Windows OS

  Patyrusiems vartotojams, kurie nori redaguoti kodą arba kompiliuotis savarankiškai:
  
    C++ kompiliatorius su C++20 palaikymu.
    Unix OS (Linux arba MacOS) arba WSL (Windows Subsystem for Linux)
    git

  ## Programos diegimas (paprastiems vartotojams):

    Atsisiųsti setup.exe failą esanti https://github.com/AtlantasLTU/obj-prog-3 puslapyje.
    Paleisti setup.exe failą ir suteikti jam administratoriaus privilegijas (paprašo paleidimo metu).
    Sekti setup.exe nurodymus.
    Pasibaigus setup'ui, turėtų būti programos paleisties nuorodos Atlantas-Alisauskas darbalaukyje bei starto meniu.

  ## Programos diegimas (patyrusiems vartotojams iš source):
    git clone https://github.com/AtlantasLTU/obj-prog-3.git
    cd ./obj-prog-3
  
  ### main (paprastas programos veikimas):
    make main
    ./main

  ### test (studento klasės testas):
    make test

    Jei jau sukompiliuotas tests paleidimo failas, tai tiesiog:
    ./tests

  ### testVector (nuosavo vektoriaus konteinerio testas):
    make testVector
    Jei jau sukompiliuotas testv paleidimo failas, tai tiesiog:
    ./testv

  ### clean (ištrina main bei tests sukompiliuotas programas):
    make clean
    
  ## Programos naudojimas:
  ### main arba studenturezultatai.exe (Atlantas-Alisauskas.lnk):
    Funkcijos:
      Pasirinktina išvestis į failą arba į terminalą.
      Pasirinktinas galutinio rezultato skaičiavimas, remiantis vidurkiu arba mediana.
      Direktorijoje esančių .txt failų pasirinkimas.
      Rūšiavimas pasirinktu būdu.
      
    1 parinktis - rankinis duomenų įvedimas, studento vardo, pavardės, namų darbų rezultatų, egzamino rezultato, jų apdorojimas ir išvedimas.
    2 parinktis - pusiau rankinis duomenų įvedimas, studento vardo, pavardės, rezultatų generavimas, duomenų apdorojimas ir išvedimas.
    3 parinktis - automatinis studentų vardų, pavardžių, rezultatų generavimas, jų apdorojimas ir išvedimas.
    4 parinktis - skaitymas iš pasirinktino failo, rūšiavimas pasirinktinu būdu, duomenų apdorojimas ir išvedimas.
    5 parinktis - testavimas su failais, pasirenkamas failas, konteinerio tipas, strategija, testų skaičius, failai apdorojami (nuskaitymas, skaičiavimas, rūšiavimas, skirstymas) ir išvedami testo rezultatai į terminalą.
    6 parinktis - studentų failų generavimas, studentų, namų darbų kiekio pasirinkimas ir išvedimas į studentai*.txt failą.
    7 parinktis - studento klasės testavimas.
    8 parinktis - nuosavo vektoriaus ir STL vektoriaus palyginimas.
    9 parinktis - programos nutraukimas.

  ### test (patyrusiems vartotojams):
      `make test` paleidžia testus, kurie ištestuoja rule of five, pasitelkiant gtest bibliotekos įrankiais.

  ### testVector (patyrusiems vartotojams):
      'make testV' paleidžia testus, kurie ištestuoja mano realizuotą vektoriaus konteinerį, pasitelkiant gtest biblioteka.

# v3.0:
  ## Realizuotas nuosavas vektorius:
  ### 5 funkcijų aprašymai:
  void push_back(*kintamasis*)

      Prie vektoriaus pabaigos prideda norimą kintamąjį, jei nėra pakankamai talpos, paskiria daugiau atminties vektoriui ir perkelia jį ten.
      Pavyzdžiui:

      Vector<int> v;
      v.push_back(42);

  void reserve(size_type)

      Iš anksto rezervuoja atmintį, kad išvengti pakartotinių perskirstymų.
      Pavyzdžiui:

      v.reserve(10000);

  operator[]

      Elemento prieiga be ribų tikrinimo (O(1))
      Pavyzdžiui:

      int x = v[0];

  emplace_back(Args&&...)

      Sukuria elementą vietoje, perduodant argumentus konstruktoriui.
      Pavyzdžiui:

      v.emplace_back(1);

  swap(Vector&)

      Efektyviai sukeičia dviejų vektorių turinį (O(1)).
      Pavyzdžiui:

      Vector<int> v2
      v.swap(v2);

  ## Nuosavo vektoriaus unit test:
  <img width="631" height="1440" alt="image" src="https://github.com/user-attachments/assets/d1755bdd-6f2c-4abe-98cd-45c19e888f09" />
  <img width="629" height="1440" alt="image" src="https://github.com/user-attachments/assets/29ed1a10-6854-407b-88df-5025df83452a" />

  ## v3.0 testavimas:
  - Kompiuterio, su kuriuo testuota parametrai:
<img width="1006" height="625" alt="image" src="https://github.com/user-attachments/assets/92e41e62-f418-441b-9928-5543b3e09995" />

  - Programa kompiliuota su -Ofast vėliavėlė bei naudojamas *-10 nice* value ant Linux, t.y. ekvivalentu high-priority ant Windows
  - Vidurkis skaičiuotas 10-ties bandymų.
  - Atlikta efektyvumo/spartos analizę: originalusis std::vector vs mano Vector. Palyginu kiek vidutiniškai laiko užtrunka std::vector tipo ir mano Vector tipo tuščius vektorius užpildyti: 10000, 100000, 1000000, 10000000 ir 100000000 int elementų naudojant push_back() funkciją:

|Konteineris|*int* elementų kiekis|Vidurkis (s)|Perskirstymų kiekis|
|---------|---------------------------|------------------------------|-------------------------------|
|std::vector<int>|10000|0.00004433 s|14|
|Vector<int>|10000|0.00009393 s|14|
|std::vector<int>|100000|0.0006383 s|17|
|Vector<int>|100000|0.0002412 s|17|
|std::vector<int>|1000000|0.002686 s|20|
|Vector<int>|1000000|0.002162 s|20|
|std::vector<int>|10000000|0.02199 s|24|
|Vector<int>|10000000|0.02555 s|24|
|std::vector<int>|100000000|0.17368 s|27|
|Vector<int>|100000000|	0.19285 s|27|

# v2.0:
  ## Pridėta doxygen sukurta dokumentacija:
      Aprašyti metodai bei klasės.
      Sukurtos nuorodos tarp metodų.

  ## Pridėtas unit testing:
      Naudojamas gtest
      Automatinis studento klasės rule of five testavimas

<img width="709" height="797" alt="image" src="https://github.com/user-attachments/assets/aa687d6d-d466-4190-a34f-a915b069e7c7" />


# v1.5:
  ## Pridėta bazinė klasė Zmogus:
    Klasė studentas dabar išvestinė.
    Studento klasė paveldi iš žmogaus klasės vardą bei pavardę, metodus susijusiais su šiais kintamaisiais
    Klasė abstrakti dėl virtualaus destruktoriaus.

  - Studento klasės rule of five atnaujintas, kad veiktų su bazinė klase.
  - Rule of five taip pat realizuotas Zmogaus klasėje.
  - Išlaikytas v1.2 testas bei funkcionalumas.

  Bandant sukurti Zmogaus tipo objektą, išmeta klaidą, kad klasė abstrakti:
  <img width="693" height="157" alt="image" src="https://github.com/user-attachments/assets/cd726a52-8453-4cb9-9fb5-59956c8ef023" />


# v1.2:
  ## Realizuota "Rule of Five":
    Destruktorius
    Copy konstruktorius
    Copy assignment operatorius
    Move konstruktorius
    Move assignment operatorius
  - Realizuotas įvesties ir išvesties operatorių perdengimas.
  - Sukurtas studento klasės metodų testas.

### Įvesties operatoriaus naudojimas (veikia su visais įvedimo srautais):
Kai naudojamas `operator>>` su failo srautu, pažymių kiekis
nustatomas automatiškai: visi skaičiai po vardo ir pavardės
laikomi namų darbų pažymiais, o pats paskutinis – egzamino balu.

    Studentas A;
    std::istringstream iss("Jonas Jonaitis 1 2 10");
    iss >> A; // įvestis iš įvedimo srauto.
    std::cin >> A; // įvestis iš konsolės.
### Išvesties operatoriaus naudojimas (veikia su visais išvesties srautais):
    Studentas A;
    std::cout << A; // išvestis į ekraną
    std::ostringstream out;
    out << A; // išvesties srauto kūrimas
    std::ofstream fout("studentai.txt");
    fout << A; // išveda į failą

## Studento klasės metodų testas.
<img width="488" height="654" alt="image" src="https://github.com/user-attachments/assets/9ad64abe-d4fd-44e7-9725-a9ab8de91735" />

# v1.1 testavimas:

- Kompiuterio, su kuriuo testuota parametrai:
<img width="1000" height="606" alt="image" src="https://github.com/user-attachments/assets/76e93716-e1de-4997-9bfb-c176b49e5e1e" />

#### Skirstymo strategijos:
- 0 - pradiniame relize naudota strategija. Dviejų konteinerių "vargšiukų" ir "kietiakų" sukūrimas, duomenys perkeliami iš "studentai" konteinerio su std::move.
- 1 strategija: Bendro studentai konteinerio (vector, list ir deque tipų) skaidymas (rūšiavimas) į du naujus to paties tipo konteinerius: "vargšiukų" ir "kietiakų". Dviejų konteinerių "vargšiukų" ir "kietiakų" sukūrimas, duomenys kopijuojami iš "studentai" konteinerio.
- 2 strategija: Bendro studentų konteinerio (vector, list ir deque) skaidymas (rūšiavimas) panaudojant tik vieną naują konteinerį: "vargšiukai". Studentai konteineris išrūšiuotas, randamas iteratorius rodantis į pirmąjį galvočių, viskas iki iteratoriaus perkeliama į vargšiukų konteinerį ir ištrinama iš studentai konteinerio. Iteratoriui rasti naudojamas lower_bound metodas
- 3 strategija: Bendro studentų konteinerio (vector, list ir deque) skaidymas (rūšiavimas) panaudojant greičiausiai veikianti 1 arba 2 strategiją  įtraukiant į ją "efektyvius" darbo su konteineriais metodus. Šioje strategijoje naudojamas partition metodas.

#### Atlikta programos veikimo greičio (spartos) analizė:
- lyginamas v1.0 struct tipas su v1.1 realizuota klasė.
- lyginamas vienas konteinerius - vektorius, su pačia greičiausia dalijimo strategija - 2.
- 100000 ir 1000000 studentų failai.
- lyginamos kompiliatoriaus optimizavimo vėliavėlės (-Ofast, -O1, -O2, -O3).

Testuota tik naudojant terminalą, visos kitos pašalinės programos testavimo metu buvo išjungtos bei įrenginys "performance" režime.
Visi testavimo atvejai testuoti 10 kartų, su medianų skaičiavimu.

## Spartos palyginimas:
#### 100000 studentų:
|Tipas|Optimizavimo vėliavėlė|Bendras veikimo laikas (s)|Failo dydis (KB)|
|---------|---------------------------|------------------------------|-------------------------------|
|struct|-Ofast|1.65366 s|349.3 KB|
|class|-Ofast|4.66815 s|332.7 KB|
|struct|-O1|2.43243 s|300.6 KB|
|class|-O1|7.11753 s|293.2 KB|
|struct|-O2|2.24629 s|314.2 KB|
|class|-O2|6.3197 s|308.2 KB|
|struct|-O3|2.1662 s|349.5 KB|
|class|-O3|5.94076 s|332.7 KB|
#### 1000000 studentų:
|Tipas|Optimizavimo vėliavėlė|Bendras veikimo laikas (s)|Failo dydis (KB)|
|---------|---------------------------|------------------------------|-------------------------------|
|struct|-Ofast|18.4434 s|349.3 KB|
|class|-Ofast|66.1051 s|332.7 KB|
|struct|-O1|20.5588 s|300.6 KB|
|class|-O1|79.2437 s|293.2 KB|
|struct|-O2|18.8914 s|314.2 KB|
|class|-O2|72.3371 s|308.2 KB|
|struct|-O3|18.0112 s|349.5 KB|
|class|-O3|55.4038 s|332.7 KB|

### Pastebėjimai
  Ta pati programa perdaryta su class tipu lėtesnė ~3x negu su struct, priklausomai nuo optimzacijos vėliavėlės. Taip yra todėl, kad class tipo programoje, galutinis balas skaičiuojamas, kai jo prireikia ir nėra saugojamas atminty, tai sutaupo atminties, tačiau padidina skaičiavimų laiką. Pagrinde -Ofast vėliavėlė greičiausia, tačiau esant milijonui studentų -O3 aplenkia -Ofast vėliavėlė, galbūt -O3 geriau suoptimizuoja failų skaitymą. Mažiausią failą sukuria -O1 vėliavėlė, o didžiausią -O3. Taip yra todėl, kad -O1 skirtas balansui tarp greičio ir dydžio, o -O3 skirtas našesniam programos veikimui.

  Nors struct tipas greitesnis, tačiau class tipas saugesnis ir plečiant programos apimtį, enkapsuliacija, paveldamumas ir kitos class tipo ypatybės padaro jį naudingesniu.

# Relizų aprašas
v1.5

    Sukurta bazinė klasė Zmogus.
    Klasė Studentas paveldi vardą bei pavardę iš klasės Zmogus.
    Kodas pritaikytas darbui su bazine klase.
v1.2

    Realizuota rule of five.
    Realizuotas įvesties išvesties operatorių perkrovimas.
    Sukurtas studento klasės metodų testas.
v1.1

    Studento struktūra paversta į klasę
    Implementuoti get'eriai, set'eriai, konstruktoriai bei destruktorius.
    Kintamieji privatūs, pasiekiami tik per studento klasės metodus.
    Refaktorintas ir patobulintas random vardų, pavardžių generatorius.
    Nebesaugojamas kintamasis galutinio rezultato, jis skaičiuojamas tik, kai jo prireikia.

v1.0

    Padidintas template naudojimas, siekiant ištestuoti vector, deque ir list konteinerius.
    Pridėtos trys studentų skirstymo į „vargšiukus“ (vidurkis < 5.0) ir „kietiakus“ strategijos:
        1 strategija: Bendro konteinerio skaidymas į du naujus, kopijuojant į vargsiukai ir galvočiai konteinerius.
        2 strategija: „Vargšiukų“ perkėlimas į naują konteinerį, juos ištrinant iš studentų konteinerio.
        3 strategija: Optimizuotas skirstymas naudojant efektyvius algoritmus (std::partition).

    Paruoštas pilnas README.md su tyrimo rezultatais, lentelėmis ir naudojimo instrukcija.
    Pridėtas Makefile lengvam programos kompiliavimui.

v0.4
    
    Patobulinta išvestis.
    Pridėtas skirstymas į "galvočius" ir "vargšiukus".
    Pridėtas failų generavimas.
    Pridėtas failų pasirinkimas.

v0.3

    Kodas išskirstytas į daugiau dalių (.h ir .cpp failus).
    Įterptas try-catch blokas failų egzistavimo tikrinimui, kitų klaidų gaudymui.
    Panaudotas template rūšiavimo funkcijoje, kodo skaitomumui pagerinti.
    Refaktoriaus metu apšvarintas main.cpp failas.

v0.2

    Pridėtas duomenų nuskaitymas iš išorinių failų.
    Pridėti testavimo atvejai.

v0.1

    Pridėtas automatinis pažymių generavimas.
    Įdiegtas „sąžiningas“ vidurkio skaičiavimas (atsižvelgiant į trūkstamus namų darbus).
    Patobulintas įvesties valdymas (apsauga nuo neteisingos įvesties).

V.pradinė

    Sukurta studento struktūra.
    Realizuotas vidurkio ir medianos skaičiavimas.
    Pradinė įvesties apsauga.
    Hardcoded reikšmės testavimui.
