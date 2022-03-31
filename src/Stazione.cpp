//Moschetta Daniele 1216411
#include "Stazione.h"

/*				STAZIONE 			*/
int Stazione::occupaBinario(std::shared_ptr<Treno> t){
    int verso = (t->daOrigine()) ? 0 : 1; 		//in base al verso del treno il successivo ciclo for scorrera' binari pari o dispari

    for(int i=verso;i<NBIN_PRINCIPALE;i=i+2){
		if(binari[i]==nullptr){
			binari[i]=t;
			t->setKm(distanza);					//imposto la distanza raggiunta dal treno
			return i;
		}
	}
    occupaParcheggio(t); 						//se non c'e' spazio nei binari lo inserisco direttamente nel parcheggio
    return -1;
}

int Stazione::liberaBinario(const std::shared_ptr<Treno> t){
    int verso = (t->daOrigine()) ? 0 : 1; 		//in base al verso del treno il successivo ciclo for scorrera' binari pari o dispari
    
    for(int i=verso;i<NBIN_PRINCIPALE;i=i+2){
		if(binari[i] && binari[i]->getNum()==t->getNum()){
			binari[i]=nullptr;
			return i;
		}
			
	}
	return -1;
}

int Stazione::controllaBinario(const std::shared_ptr<Treno> t) const{
	int verso = (t->daOrigine()) ? 0 : 1; 		//in base al verso del treno il successivo ciclo for scorrera' binari pari o dispari
	
    for(int i=verso;i<NBIN_PRINCIPALE;i=i+2){
		if(binari[i] && binari[i]->getNum()==t->getNum())
			return i;
	}
	return -1;
}	

void Stazione::occupaParcheggio(std::shared_ptr<Treno> t){
	parcheggi.push_back(t);
	t->setKm(distanza);
}

bool Stazione::liberaParcheggio(const std::shared_ptr<Treno> t){
	for(int i=0;i<parcheggi.size();i++){
		if(parcheggi.at(i)->getNum()==t->getNum()){
			parcheggi.erase(parcheggi.begin()+i);
			return true;
		}	
	}
	return false;
}

bool Stazione::controllaParcheggio(const std::shared_ptr<Treno> t) const{
	for(int i=0;i<parcheggi.size();i++){
		if(parcheggi.at(i)->getNum()==t->getNum())
			return true;
	}
	return false;
}

std::string Stazione::toString() const {
	std::string s="Stazione "+nome;
	if(getTipo()==0)
		s+=" (Principale)\t";
	else
		s+=" (Secondaria)\t";
	s+="Distanza: "+std::to_string(distanza)+" km\n";
	return s;
}

std::ostream& operator<<(std::ostream& os, const Stazione& s) {
	return os << s.toString();
}

/*		STAZIONE PRINCIPALE 		*/
StazPrincipale::StazPrincipale(std::string n, int x) 
				: Stazione(n,x) {
	binari=std::vector<std::shared_ptr<Treno>>(NBIN_PRINCIPALE,nullptr);	//inizializza i binari a nullptr (=binario libero)
}

std::string StazPrincipale::toString() const{
	std::string s=Stazione::toString();										//chiama il toString() della classe base
	for(int i=0;i<NBIN_PRINCIPALE;i++){
		s+="\tBinario "+std::to_string(i+1)+": ";
		if(binari[i])
			s+=binari[i]->toString();
		else
			s+=" -";
		s+="\n";
	}
	for(std::shared_ptr<Treno> t : parcheggi)
		s+="\tParcheggio: "+t->toString()+"\n";
	return s;
}

/*		STAZIONE SECONDARIA 		*/		
StazSecondaria::StazSecondaria(std::string n, int x) 
				: Stazione(n,x) {
	binari=std::vector<std::shared_ptr<Treno>>(NBIN_SECONDARIA,nullptr);	//inizializza i binari a nullptr (=binario libero)
}

int StazSecondaria::occupaTransito(std::shared_ptr<Treno> t){
    int verso = (t->daOrigine()) ? 0 : 1; 		//in base al verso del treno il successivo ciclo for scorrera' binari pari o dispari

    for(int i=NBIN_PRINCIPALE + verso;i<NBIN_SECONDARIA;i=i+2){
        if(binari[i]==nullptr){
            binari[i]=t;
            t->setKm(distanza);
            return i;
        }
    }
    occupaParcheggio(t); 						//se non c'e' spazio nei binari lo inserisco direttamente nel parcheggio
    return -1;
}

int StazSecondaria::liberaTransito(const std::shared_ptr<Treno> t){
    int verso = (t->daOrigine()) ? 0 : 1; 		//in base al verso del treno il successivo ciclo for scorrera' binari pari o dispari

    for(int i=NBIN_PRINCIPALE + verso;i<NBIN_SECONDARIA;i=i+2){
        if(binari[i] && binari[i]->getNum()==t->getNum()){
            binari[i]=nullptr;
            return i;
        }
    }
    return -1;
}

int StazSecondaria::controllaTransito(const std::shared_ptr<Treno> t) const {
    int verso = (t->daOrigine()) ? 0 : 1; 		//in base al verso del treno il successivo ciclo for scorrera' binari pari o dispari

    for(int i=NBIN_PRINCIPALE + verso;i<NBIN_SECONDARIA;i=i+2){
        if(binari[i] && binari[i]->getNum()==t->getNum()){
            return i;
        }
    }
    return -1;
}

std::string StazSecondaria::toString() const{
	std::string s=Stazione::toString();										//chiama il toString() della classe base
	for(int i=0;i<NBIN_SECONDARIA;i++){
		if(i<NBIN_PRINCIPALE)
			s+="\tBinario "+std::to_string(i+1)+": ";
		else
			s+="\tTransito "+std::to_string(i+1)+": ";
		
		if(binari[i])
			s+=binari[i]->toString();
		else
			s+=" -";
		s+="\n";
	}
	for(std::shared_ptr<Treno> t : parcheggi)
		s+="\tParcheggio: "+t->toString()+"\n";
	return s;
}

/*		CONTROLLO DATI INPUT		*/
void controllaDati(std::vector<std::shared_ptr<Stazione>> &stazioni, std::vector<std::shared_ptr<Treno>> &treni){
	std::sort(stazioni.begin(), stazioni.end(), comparaStazionePtr);		//ordina le stazioni in base alla distanza

	//CONTROLLO STAZIONI
	std::vector<int> stazioniPrincipaliEliminate;		//conterra' gli indici stazioni pricipali eliminate
	std::vector<int> stazioniSecondarieEliminate;		//conterra' gli indici stazioni secondarie eliminate
	for(int i=0;i<stazioni.size() - 1;i++){
		if(stazioni.at(i+1)->getDistanza() - stazioni.at(i)->getDistanza() < 20){ 	//controlla se la stazione e' da eliminare
			if(stazioni.at(i+1)->getTipo()==0)										
				stazioniPrincipaliEliminate.push_back(i+1);							//salva l'indice
			else
				stazioniSecondarieEliminate.push_back(i+1);
			stazioni.erase(stazioni.begin() + i+1);									//elimina quella più distante dall'origine
		}
	}
	
	//ELIMINO ORARI DELLE STAZIONI ELIMINATE
	for(int i=0;i<treni.size();i++){
		if(treni.at(i)->getOra(0) >= 1440){
			treni.erase(treni.begin() + i);
			continue;
		}
		if(treni.at(i)->getTipo()==1){								//se il treno e' regionale, elimina sia principali che secondarie
			for(int pos : stazioniPrincipaliEliminate)
				treni.at(i)->eliminaOrario(pos);
			for(int pos : stazioniSecondarieEliminate)
				treni.at(i)->eliminaOrario(pos);
		}
		else{														//se e' AV o AVSuper elimina solo le principali
			for(int pos : stazioniPrincipaliEliminate){				//scorre gli indici delle principali da eliminare
				int k=0;
				for(int i=0;i<pos;i++)								//per ogni principale conta quante secondarie la precedono
					if(stazioni.at(i)->getTipo()== 1)
						k++;
				treni.at(i)->eliminaOrario(pos-k);		//il corretto indice dell'orario e' (stazione principale - quante secondarie la precedono)
			}
		}
		treni.at(i)->controllaTreno(stazioni);					//invoca il metodo di Treno che controlla e corregge gli orari
	}
}

bool comparaStazionePtr(std::shared_ptr<Stazione> a,std::shared_ptr<Stazione> b){
	return (a->getDistanza() < b->getDistanza()); 
}

/*		INPUT STAZIONI DA FILE		*/	
std::vector<std::shared_ptr<Stazione>> caricaStazioni(std::string fileName){
	std::vector<std::shared_ptr<Stazione>> stazioni;
	std::ifstream read(fileName);
	if(!read.good())											//se il file non esiste lancia un'eccezione
		throw std::invalid_argument("File "+ fileName +" non esistente!");
	std::string nome, line;
	int tipo=0, distanza=0;
	
	getline(read,line);											//legge la prima riga
	while(!read.fail()){
		std::istringstream iss(line);
    	std::string word;
    	while(iss >> word){										//legge le singole word della riga
    		if(!isNumber(word))									//se e' una string la considera parte del nome
    			nome += word + " ";
    		else{												//se e' un intero e' la parte finale (tipo e distanza)
	    		tipo=std::stoi(word);
	    		iss>>word;
	    		distanza=std::stoi(word);
			}
    	}
    	if(tipo==0)												//inizalizza la stazione spefica in base al tipo immesso
        	stazioni.push_back(std::make_shared<StazPrincipale>(nome,distanza));
        if(tipo==1)
        	stazioni.push_back(std::make_shared<StazSecondaria>(nome,distanza));
        nome="";
        getline(read,line);										//legge la riga successiva
	}
    read.close();
    return stazioni;
}

bool isNumber(std::string s){
    for (int i = 0; i < s.length(); i++)
        if (std::isdigit(s[i]) == false)						//controlla se la stringa contiene cifre
            return false;
    return true;
}
