//Moschetta Daniele 1216411
#ifndef STAZIONE_H
#define STAZIONE_H

#include "Treno.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>

/*
STAZIONE
La stazione e' gestita tramite una classe Stazione generica che viene poi specializzata in StazPrincipale e StazSecondaria. La differenza e' 
il numero di binari (vedi consegna), in entrambi i casi sono gestiti con un vector di puntatori a Treni. Per convenzione consideriamo gli indici 
pari come binari di andata e quelli dispari come binari di ritorno. Gli shared_ptr sono stati preferiti ai "raw ptr" in quanto gestiscono in modo
automatico la deallocazione, anche nel caso in cui più shared_ptr puntino allo stesso oggetto (situazione possbile per come è gestito il progetto).
*/

class Stazione{
	public:
		//costruttore
		Stazione(std::string n, int x) : nome{n}, distanza{x} {}

    	//setter e getter
		std::string getNome() const { return nome; }
		int getDistanza() const { return distanza; }
		virtual int getTipo() const = 0;
		
		//funzioni necessarie
        int occupaBinario(std::shared_ptr<Treno>);						//occupa un binario e restituisce il binario occupato, se e' pieno restituisce -1
        int liberaBinario(const std::shared_ptr<Treno>);				//libera il binario occupato dal treno e lo resituisce, se non era presente restituisce -1
        int controllaBinario(const std::shared_ptr<Treno>) const;		//resituisce il binario occupato dal treno, se non era presente restituisce -1
        
        void occupaParcheggio(std::shared_ptr<Treno>);					//inserisce un treno nel parcheggio (spazio per infiniti treni)
        bool liberaParcheggio(const std::shared_ptr<Treno>);			//libera il parcheggio da un treno fornito in input, resituisce false se non era presente
        bool controllaParcheggio(const std::shared_ptr<Treno>) const;	//resituisce true se il treno e' nel parcheggio, false altrimenti
        
        virtual std::string toString() const;							//utilizzato nell'operator<< 
	
	protected:
		static constexpr int NBIN_PRINCIPALE=4;
		static constexpr int NBIN_SECONDARIA=6;
		
		std::string nome;												//nome della stazione
		int distanza; 													//distanza dall'origine
		std::vector<std::shared_ptr<Treno>> binari;
		std::vector<std::shared_ptr<Treno>> parcheggi;					//parcheggi dovrà contenere un numero di treni non noto a priori
};

std::ostream& operator<<(std::ostream&, const Stazione&);				//chiama il metodo toString() in base alla classe (polimorfismo runtime)

class StazPrincipale : public Stazione{
	public:
		//costruttore
		StazPrincipale(std::string, int);

		//setter e getter
		int getTipo() const override { return 0; }	
		
		//funzioni necessarie
		std::string toString() const override;
};

class StazSecondaria : public Stazione{
	public:
		//costruttore
		StazSecondaria(std::string, int);

		//setter e getter
		int getTipo() const override { return 1; }

	    //funzioni necessarie
	    int occupaTransito(std::shared_ptr<Treno>);						//occupa un binario di transito e restituisce il binario occupato, se e' pieno restituisce -1
	    int liberaTransito(const std::shared_ptr<Treno>);				//libera il binario di transito occupato dal treno e lo resituisce, se non era presente restituisce -1
	    int controllaTransito(const std::shared_ptr<Treno>) const;		//resituisce il binario di transito occupato dal treno, se non era presente restituisce -1
	    
		std::string toString() const override;
};

//legge un file txt e carica in un vector le stazioni valide presenti
std::vector<std::shared_ptr<Stazione>> caricaStazioni(std::string fileName);

//controlla i dati precedentemente letti da txt e li corregge nel caso non rispettassero le regole (vedi consegna)
void controllaDati(std::vector<std::shared_ptr<Stazione>>&, std::vector<std::shared_ptr<Treno>>&);
bool comparaStazionePtr(std::shared_ptr<Stazione>,std::shared_ptr<Stazione>); //compara le distanze della stazioni puntate (necessario per ordinarle)
bool isNumber(std::string s);

#endif //STAZIONE_H
