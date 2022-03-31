#include "LineaFerroviaria.h"
#include <iostream>
#include <vector>

int main() {
	std::vector<std::shared_ptr<Stazione>> staz = caricaStazioni("line_description.txt");
    std::vector<std::shared_ptr<Treno>> tren = caricaTreno("timetables.txt");
    controllaDati(staz, tren);
    std::cout<<"Dati di input letti e controllati\n";
    LineaFerroviaria rail = LineaFerroviaria(tren, staz);
    time(rail);
    return 0;
}
