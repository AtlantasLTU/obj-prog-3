#include "vector_compare.h"

void vector_compare(){
    std::vector<unsigned int> sz{10000, 100000, 1000000, 10000000, 100000000};
    for(auto x : sz){
        int perskirstymai = 0;
        Timer t;
        std::vector<int> v1;
        
        for(int i = 1; i <= x; ++i)
        {   
            v1.push_back(i);
            if(v1.size() == v1.capacity()) perskirstymai++;
        }
        std::cout << "Vektoriaus v1 dydis - " << x << " Užpildymo laikas - " << t.elapsed() << "s." << " Perskirstymų kiekis - " << perskirstymai << "\n";
        perskirstymai = 0;
        t.reset();
        Vector<int> v2;
        for(int i = 1; i <= x; ++i)
        {
            v2.push_back(i);
            if(v2.size() == v2.capacity()) perskirstymai++;
        }

        std::cout << "Vektoriaus v2 dydis - " << x << " Užpildymo laikas - " << t.elapsed() << "s." << " Perskirstymų kiekis - " << perskirstymai << "\n";
    }
}