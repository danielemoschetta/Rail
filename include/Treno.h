//Favrin Alice 1188686
#ifndef TRENO_H
#define TRENO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>

// CLASSE TRENO
/*
* La classe Treno crea l'oggetto Treno a partire dai suoi tre attributi principali ossia il numero del treno, il verso della tratta e gli orari delle fermate.
* I treni sono pero' categorizzati in Regionali, AltaVelocit� e AltaVelocit�Super, che si differenziano tra loro in base alla velocit� massima raggiungibile. Questa � un attributo
* dell'oggetto Treno il cui set viene effettuato nel costruttore della classe corrispondente (e non nel costruttore della classe Treno).
* La gestione degli orari delle fermate avviene tramite un vettore gestito come una coda: ogni volta che il treno ha effettuato una fermata, l'orario corrispondente a tale fermata viene rimosso
* e il primo orario che si legge all'interno del vettore sar� appunto l'orario della prossima fermata.
*/

class Stazione;	//forward declaration per evitare "include circolari"

class Treno {
	public:
		// COSTRUTTORE
		Treno(int, int, std::vector<int>);
		
		// METODI GET
		int getNum() const{ return numeroTreno;}						// OK
		int getOra(int) const;										// OK
		std::vector<int> getOrari() const { return orari; }				// OK 
		int getVel() const{ return vmax;}								// OK
		bool daOrigine() const;
		int getRitardo() const { return ritardo; }						// OK
		int getKm() const { return km; }								// OK

		// METODI SET
		void setKm(int);


		// ALTRE FUNZIONI
		void addRitardo(int);											// RITARDO TRENO - OK
		std::string toString() const;									// DATI TRENO IN UNA STRINGA - OK
		void eliminaOrario(int);										// ELIMINA ORARI IN ECCESSO - OK
		virtual int getTipo() const = 0;								// RESTITUISCE TIPO TRENO (REGIONALE, AV, AVSUPER) - OK
		void controllaTreno(std::vector<std::shared_ptr<Stazione>>);					// CONTROLLA SE I TRENI SONO "VALIDI" E RESTITUISCE UN BOOLEAN
		
	protected:
		int numeroTreno;												//numero treno
		int verso;														//se parte da origine =0, se parte da fine =1
		std::vector<int> orari;											//vector orari contenente orario di partenza e arrivi nelle stazioni
		int vmax;														//velocita' massima treno; set nella classe del tipo treno
		int ritardo;													//ritardo treno
		int km;														//chilometro ultima stazione passata
};

std::ostream& operator<<(std::ostream & os, const Treno & s);			//funzione overload operator<<


class Regionale : public Treno {
	public:
		// COSTRUTTORE
		Regionale(int, int, std::vector<int>);

		//OVERRIDE GetTipoTreno()
		int getTipo() const override {	return 1;}
};

class AV : public Treno {
	public:
		// COSTRUTTORE
		AV(int, int, std::vector<int>);

		//OVERRIDE GetTipoTreno()
		int getTipo() const override { return 2; }
};

class AVSuper : public Treno {
	public:
		// COSTRUTTORE
		AVSuper(int, int, std::vector<int>);

		//OVERRIDE GetTipoTreno()
		int getTipo() const override { return 3; }
};


// FUNZIONE CARICAMENTO FILE timetables.txt;
std::vector<std::shared_ptr<Treno>> caricaTreno(std::string);

std::string convertiOra(int);								// CONVERTE ORA IN FORMATO HH::MM


#endif //TRENO_H
