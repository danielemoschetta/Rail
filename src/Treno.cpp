//Favrin Alice 1188686
#include "Treno.h"
#include "Stazione.h"
#include <math.h>

// COSTRUTTORE
Treno::Treno(int n, int ver, std::vector<int> o)
	: numeroTreno{ n }, verso{ ver }, orari{ o }, ritardo{ 0 }, km{ 0 }					// ritardo inizializzato a zero
{}

// RETURN VERSO DEL TRENO
bool Treno::daOrigine() const															// funzione membro che restituisce un boolean true se il treno va da origine a capolinea, false se ha verso opposto
{
	if (verso == 0)
		return true;
	else if (verso == 1)
		return false;
	else
		throw;
}

int Treno::getOra(int n) const {														// restituisce l'orario d'arrivo del treno alla stazione n
	if (orari.size() > n)
		return orari[n];
	return -1;
}

// SET KM
void Treno::setKm(int k)
{
	km = k;																				// funzione chiamata dalla classe Stazione
}

// GESTIONE RITARDO
void Treno::addRitardo(int n)
{
	ritardo = ritardo + n;																// n ritardo provvisorio (passato da LineaFerroviaria)
}

// CONVERTE ORA IN FORMATO hh::mm
std::string convertiOra(int o)
{
	int ore = int(floor(o / 60)), min = (o % 60);										// trovo il numero di ore e di minuti dividendo per 60 e considerandone il resto
	if(ore >= 24)																		// valuto il caso in cui l'orario sia del giorno successivo (nel caso di treni con arrivo dopo la mezzanotte)
		ore = ore % 24;
	std::string s_ore=std::to_string(ore), s_min=std::to_string(min);					// converto in string ore e minuti
	if(s_ore.length()==1) 
		s_ore="0" + s_ore;
	if(s_min.length()==1)
		s_min="0" + s_min;
	return s_ore + ":" + s_min;															// return orario in formato hh:mm
}

// CONTROLLA LA VALIDITA' DEI TRENI
void Treno::controllaTreno(std::vector<std::shared_ptr<Stazione>> staz)
{
	int nPrincipali=0;
	int nFermate = staz.size();
	for(std::shared_ptr<Stazione> s : staz){											// conto le stazioni principali (uniche in cui si fermano treni AV e AVSuper)
		if(s->getTipo()==0)								
			nPrincipali++;
	}
	if (getTipo() == 2 || getTipo() == 3)
		nFermate = nPrincipali;															// nel caso in cui il treno sia AV o AVSuper imposto il numero di fermate a numero di stazioni principali
	
	if(nFermate > orari.size()){														// completa gli orari mancanti inizializzandoli a zero (verranno modificati successivamente)
		int n=nFermate-orari.size();
		for(int i=0;i<n;i++)
			orari.push_back(0);
	}
	if(orari.size() > nFermate){														// elimina gli orari in eccesso
		int n=orari.size() - nFermate;
		for(int i=0;i<n;i++)
			orari.pop_back();
	}

	int dieciKm = 8;																	// min impiegati per percorrere dieci km (5 in partenza e 5 in arrivo) a 80 km/h
	int dist;																			// distanza tra una stazione e la successiva
	int difOrari;																		// differenza tra orario di partenza e arrivo di un treno (in due stazioni successive)
	if (verso == 0)																		// se il treno percorre la tratta da origine a fine
	{
		for(int i=0;i < nFermate - 1;i++){
			dist=staz[i+1]->getDistanza() - staz[i]->getDistanza() - 10;
			difOrari=orari[i+1]-orari[i];
			if((dist/(vmax/60) + dieciKm) > difOrari)									// se nel tempo stabilito non puo' percorrere i km fino alla prossima stazione
				orari[i+1]+=dist/(vmax/60) + dieciKm - difOrari;						// modifico l'orario in modo che sia possibile raggiungere la stazione predefinita in un certo tempo
		}
	}
	else if (verso == 1)																// se il treno percorre la tratta da fine a origine (cambiano le condizioni per la gestione del vettore staz)
	{
		int j=0;																		// indice che gestisce i dati del vettore orari (inverso rispetto al vettore contenente le stazioni)
		for(int i = nFermate - 1;i > 0;i--){											// gestione orari come verso ==0, con la sola differenza che scorriamo il vettore staz dalla fine all'inizio
			dist=staz[i]->getDistanza() - staz[i-1]->getDistanza() - 10;
			difOrari=orari[j+1]-orari[j];
			if((dist/(vmax/60) + dieciKm) > difOrari)
				orari[j+1]+=dist/(vmax/60) + dieciKm - difOrari;
			j++;
		}
	}
}

// METTE IN UNA STRINGA I DATI DEL TRENO
std::string Treno::toString() const
{
	std::string v = "da Capolinea";
	if(verso) v = "da Origine";
	std::string ret = "Treno " + std::to_string(numeroTreno);							// inserisco numero treno
	if(getTipo()==1)																	// inserisco tipo treno
		ret += " (Regionale)";
	if(getTipo()==2)
		ret += " (AltaVelocita')";
	if(getTipo()==3)
		ret += " (AVSuper)";
	ret += "\tDirezione: " + v;															// inserisco verso treno						
	ret = ret + "\tRitardo attuale: " + std::to_string(ritardo) + " minuti";			// inserisco ritardo treno
	ret += "\tOrari fermate: ";
	for (int i = 0; i < orari.size(); i++)
		ret += convertiOra(orari[i]) + "\t";
	return ret;																			// return della stringa contenente i dati dell'oggetto Treno
}

// ELIMINA ORARI
void Treno::eliminaOrario(int pos){
	if(orari.size()>pos)
		orari.erase(orari.begin()+pos);													// elimina gli orari in eccesso (se ci sono)
}

// OVERLOAD OPERATOR<<
std::ostream& operator<<(std::ostream& os, const Treno& s)
{
	return os << s.toString();															//restituisco in output tutti i dati del treno convertiti a stringa
}

// TRENO REGIONALE
Regionale::Regionale(int n, int ver, std::vector<int> o)
	: Treno(n, ver, o)
{
	vmax = 160;																			// set vmax treno Regionale a 160 km/h
}

// TRENO ALTA VELOCITA'
AV::AV(int n, int ver, std::vector<int> o)
	: Treno(n, ver, o)
{
	vmax = 240;																			// set vmax treno AV a 240 km/h
}

//TRENO ALTA VELOCITA' SUPER
AVSuper::AVSuper(int n, int ver, std::vector<int> o)
	: Treno(n, ver, o)
{
	vmax = 300;																			// set vmax treno AVSuper a 300 km/h
}


// CARICA TRENI DA FILE DI TESTO
std::vector<std::shared_ptr<Treno>> caricaTreno(std::string filetxt)
{
	std::vector<std::shared_ptr<Treno>> treni;
	std::ifstream read(filetxt);
	if(!read.good())																	//se il file non esiste lancia un'eccezione
		throw std::invalid_argument("File "+ filetxt +" non esistente!");
	int num, ori, tipo;
	std::vector<int> fermate;
	std::string line;

	getline(read, line);																//leggo la prima riga
	while (!read.fail()) {
		std::istringstream is(line);
		int tok;																					
		is >> num;																		//primo int-> numero treno
		is >> ori;																		//secondo int-> verso treno
		is >> tipo;																		//terzo int-> tipologia treno
		while (is >> tok)
			fermate.push_back(tok);														//inserisco in un vettore tutti gli interi rimanenti (ossia gli orari delle fermate)																			
		if (tipo == 1)																	//costruttore del treno in base al tipo
			treni.push_back(std::make_shared<Regionale>(num, ori, fermate));
		if (tipo == 2)
			treni.push_back(std::make_shared<AV>(num, ori, fermate));
		if (tipo == 3)
			treni.push_back(std::make_shared<AVSuper>(num, ori, fermate));
		fermate.clear();
		getline(read, line);															//leggo la prossima riga
	}
	read.close();
	return treni;
}
