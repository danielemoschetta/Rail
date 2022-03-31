//Bolzonello Enrico 1216351
#include "Stazione.h"
#include "Treno.h"
#include <cmath>
#include <memory>

#ifndef RAILWAY_LINEAFERROVIARIA_H
#define RAILWAY_LINEAFERROVIARIA_H
class LineaFerroviaria{
public:
    const int TempoStazione = 4; //tempo parcheggio-stazione

    LineaFerroviaria(std::vector<std::shared_ptr<Treno>> t, std::vector<std::shared_ptr<Stazione>> s); //constructor

    //metodo per controllare se la stazione ha un binario disponibile, se si, mette il treno nel binario, altrimenti lo mette nel parcheggio della relativa stazione
    void checkStation(int IndexStazionePartenza, int IndexTreno);
    //metodo che calcola il ritardo
    static void calcolaRitardo(int t, const std::shared_ptr<Treno>& tr);
    //metodo che la distanza fra due treni sarà minore di 10 km, se sì si fa aspettare il treno dietro alla stazione
    bool isTooClose(int IndexTrenoDietro, int IndexStazionePartenza, int t);

    bool isApproachingStation(int t, const std::shared_ptr<Treno>& tr, int n);

    int contaSorpassi(int IndexTreno, std::shared_ptr<Stazione>& s) const;

    //get
    std::shared_ptr<Treno> getTreno(int n) const;
    std::shared_ptr<Stazione> getStazione(int n) const;
    std::shared_ptr<Treno> getTrenoSuccessivo(int IndexTrenoPrecedente) const;
    int getNTreni() const;
    int getNStazioni() const;
    int getIndexStazione(int);
    int getIndexStazioneSuccessiva(int n, bool verso) const;

private:
    std::vector<std::shared_ptr<Treno>> treni; //ordine degli elementi del vettore è l'ordine dei treni effettivi. Se un treno sorpassa un'altro si swappano gli elementi
    std::vector<std::shared_ptr<Stazione>> stazioni; //ordine degli elementi è l'ordine della stazione (origine -> fine)
};

//funzione per far scorrere il tempo
void time(LineaFerroviaria&);

//commento
#endif //RAILWAY_LINEAFERROVIARIA_H
