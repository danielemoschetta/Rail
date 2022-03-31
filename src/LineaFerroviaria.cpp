//Bolzonello Enrico 1216351
#include "LineaFerroviaria.h"

//Costruttore
//@param std::vector<std::shared_ptr<Treno>> t, vettore di shared pointer a oggetti di tipo Treno
//@param std::vector<std::shared_ptr<Stazione>> s, vettore di shared pointer a oggetti di tipo Stazione

LineaFerroviaria::LineaFerroviaria(std::vector<std::shared_ptr<Treno>> t, std::vector<std::shared_ptr<Stazione>> s)
        : treni{std::move(t)}, stazioni{std::move(s)}
{
    for(int i = 0; i < treni.size(); i++)
    {
        if(!getTreno(i) -> daOrigine())
        {
            getTreno(i) ->setKm(getStazione(getNStazioni() - 1) ->getDistanza());
        }
    }
}

//Fornito una Stazione e un Treno controlla se nella stazione c'è un binario libero
//se si occupa il binario, altrimenti occupa il parcheggio
void LineaFerroviaria::checkStation(int IndexStazionePartenza, int IndexTreno)
{
    std::shared_ptr<Treno> t = getTreno(IndexTreno);
    int n = getIndexStazioneSuccessiva(IndexStazionePartenza, t ->daOrigine());

    if(t->getTipo() == 1 || (t->getTipo() != 1 && stazioni[n]->getTipo() == 0))
    {
        //occupa il binario se libero altrimenti occupa il parcheggio
        int binario = stazioni[n] -> occupaBinario(t);

        if(binario == -1){
            std::cout << "Ho occupato il parcheggio" << std::endl;
        }else{
            std::cout << "Il treno " << t->getNum() <<" e' arrivato al binario " << binario << " della stazione di " << getStazione(n) ->getNome() << std::endl;
        }
    }else if(t->getTipo() != 1 && stazioni[n]->getTipo() == 1) //se il treno non è un regionale e la stazione è una stazione secondaria
    {
        std::dynamic_pointer_cast<StazSecondaria> (stazioni[n]) -> occupaTransito(t);
        std::cout << "Il treno " << t->getNum() << " sta transitando nella stazione di " << stazioni[n] ->getNome() << std::endl;
        //devo contare quanti treni sorpassa
        std::cout << "Il treno " << t->getNum() << " ha superato " << contaSorpassi(IndexTreno, stazioni[n]) << " treni" << std::endl;
        std::dynamic_pointer_cast<StazSecondaria> (stazioni[n]) -> liberaTransito(t);
    }
}

//dato tempo t e un puntatore a treno calcola quanto tempo in più il treno deve fermarsi alla stazione (viene aggiunto alla variabile membro ritardo del treno)
void LineaFerroviaria::calcolaRitardo(int t, const std::shared_ptr<Treno>& tr)
{
    tr->addRitardo(t);
    int ritardo2 = tr->getRitardo();
    std::cout << "Il ritardo totale e'' di " << ritardo2 << ". Aumentato di " << t << " rispetto alla stazione precedente" << std::endl;
}

//dato t tempo, tr puntatore a treno e n indice della stazione da cui è partito
//restituisce true se è vicino alla stazione (intesa prima del parcheggio)
bool LineaFerroviaria::isApproachingStation(int t, const std::shared_ptr<Treno>& tr, int IndexStazionePartenza)
{
    int n = getIndexStazioneSuccessiva(IndexStazionePartenza, tr ->daOrigine());
    if(n > stazioni.size() - 1 || n < 0) throw; //se si tenta di accedere ad una stazione che non esiste lancia un'eccezzione

    //indice per richiedere l'orario, dipende dal verso (gli orari nel vettore sono in ordine crescente)
    int IndexOra = n;
    if(!tr->daOrigine()){
        IndexOra = getNStazioni() - 1 - n;
    }

    //controllo se il treno non è già nel binario o nel parcheggio
    if(getStazione(n) -> controllaBinario(tr) == -1 && !getStazione(n)->controllaParcheggio(tr))
    {
        if ( t == ( tr->getOra(IndexOra) + tr->getRitardo() ) ) //controllo che sia il suo orario
        {
            std::cout << "Il treno " << tr->getNum() << " si fermera'/passera' alla stazione " << getStazione(n)->getNome()
                      << " alle ore " << convertiOra(t) << " con un ritardo di " << tr->getRitardo()
                      << " rispetto alla tabella di marcia" << std::endl;
            return true;
        }
    }
    return false;
}

//se i treni sono troppo vicini (x2 - x1 < 10 km) ritorna true
//inoltre aggiunge il tempo minimo che il treno dietro deve aspettare per non incontrare il treno davanti
bool LineaFerroviaria::isTooClose(int IndexTrenoDietro, int IndexStazionePartenza, int t)
{
    std::shared_ptr<Treno> TrenoDietro = treni[IndexTrenoDietro] ;
    std::shared_ptr<Treno> TrenoDavanti = getTrenoSuccessivo(IndexTrenoDietro);

    if(TrenoDavanti == nullptr) return false; //non ha treni davanti
    if(TrenoDietro ->getKm() != TrenoDavanti ->getKm()) return false; //il treno successivo è già nella stazione successiva

    std::shared_ptr<Stazione> StazioneSuccessiva = getStazione(getIndexStazioneSuccessiva(IndexStazionePartenza, TrenoDietro -> daOrigine()));

    int v1 = TrenoDietro -> getVel(); //velocità treno dietro
    int v2 = TrenoDavanti -> getVel(); //velocità treno davanti

    if(v2 < v1 && ( !(StazioneSuccessiva -> controllaParcheggio(TrenoDavanti)) && StazioneSuccessiva -> controllaBinario(TrenoDavanti) == -1 ) )
    {
        //i indice della stazione, indexora relativo agli orari
        int i = getIndexStazioneSuccessiva(TrenoDavanti->getKm(), TrenoDavanti ->daOrigine());
        int IndexOra = IndexStazionePartenza;
        if(!TrenoDavanti ->daOrigine())
        {
            i = getNStazioni() - 1 - i;
            IndexOra = getNStazioni() - 1 - IndexOra;
        }

        int tArrivoTrenoDavanti = TrenoDavanti -> getOra(i) + TrenoDavanti -> getRitardo() - TempoStazione; //tempo (in minuti) di quando il treno arriva AL PARCHEGGIO
        int x0 = std::round((v2 * (t - TrenoDavanti->getOra(IndexOra) ) / 60)); //distanza del treno davanti dalla stazione (tolti i 5 km) nell'istante t
        if (x0 + std::round((v2 - v1)*(tArrivoTrenoDavanti - t - TempoStazione) / 60) < 10) //controllo se la distanza fra i due treni sarà minore di 10 in prossimità del parcheggio
        {
            std::cout << "Il treno deve aspettare perche' altrimenti si avvicinera' troppo a quello davanti" << std::endl;
            calcolaRitardo(std::round((v2 * (tArrivoTrenoDavanti - t) + x0 + 10) / v1)  , TrenoDietro); //aggiungo il tempo minimo che deve aspettare per rispettare i 10 km di distanza
            return true;
        }
    }
    return false;
}

int LineaFerroviaria::contaSorpassi(int IndexTreno, std::shared_ptr<Stazione>& s) const
{
    //in quel momento devo controllare quanti treni (contando il verso) ci sono nei binari della stazione e nel parcheggio
    int counter = 0;
    for(int i = IndexTreno - 1; i >= 0; i--)
    {
        if(s->controllaParcheggio(getTreno(IndexTreno)) || s->controllaBinario(getTreno(IndexTreno)) != -1)
            if(getTreno(IndexTreno) ->daOrigine() == getTreno(i) -> daOrigine())
                counter++;
    }

    return counter;
}

/*  GETTER */

//dati i km dal'origine trova la posizione della stazione nel vettore
int LineaFerroviaria::getIndexStazione(int n)
{
    for(int i = 0; i < stazioni.size(); i++)
    {
        if(stazioni[i] -> getDistanza() == n)
        {
            return i;
        }
    }

    return -1;
}

//dato un indice restituisce il puntatore relativo
std::shared_ptr<Treno> LineaFerroviaria::getTreno(int n) const
{
    if(n > treni.size()) throw; //se non c'è nel vettore il programma termina
    return treni[n];
}

//dato un indice restituisce il puntatore relativo
std::shared_ptr<Stazione> LineaFerroviaria::getStazione(int n) const
{
    if(n > stazioni.size()) throw; //se non c'è nel vettore il programma termina
    return stazioni[n];
}

int LineaFerroviaria::getNTreni() const {
    return treni.size();
}

int LineaFerroviaria::getNStazioni() const{
    return stazioni.size();
}

//dato l'indice del treno precedente, ritorna il treno successivo
//considera il verso del treno
std::shared_ptr<Treno> LineaFerroviaria::getTrenoSuccessivo(int IndexTrenoPrecedente) const
{
    //treno davanti vuol dire che è partito PRIMA, quindi nel vettore è PRIMA del treno dato in input
    if(IndexTrenoPrecedente == 0) return nullptr;

    std::shared_ptr<Treno> TrenoDietro = getTreno(IndexTrenoPrecedente);
    for(int i = IndexTrenoPrecedente - 1; i >= 0; i--)
    {
        std::shared_ptr<Treno> TrenoDavanti = getTreno(i);

        if(TrenoDietro->daOrigine() == TrenoDavanti->daOrigine())
        {
            return TrenoDavanti;
        }
    }
	return nullptr;
}

int LineaFerroviaria::getIndexStazioneSuccessiva(int n, bool verso) const
{
    //controllo l'indice della prossima stazione in base al verso
    if(verso){
        if(n == getNStazioni() - 1) return n;
        else return n + 1;
    }else{
        if(n == 0) return n;
        else return n - 1;
    }
}

/* FINE METODI CLASSE LINEAFERROVIARIA */

void time(LineaFerroviaria& l) {
    for (int t = 0; t < 1440; t++) //1440 = 24 * 60
    {
        for (int i = 0; i < l.getNTreni(); i++) {
            std::shared_ptr<Treno> tr = l.getTreno(i);

            int IndexStazione = l.getIndexStazione(tr->getKm());
            std::shared_ptr<Stazione> stazione = l.getStazione(IndexStazione);

            //partenza treno
            //DONE
            if ( t == tr->getOra(0) + tr->getRitardo() )
            {
                if(!(l.isTooClose(i, IndexStazione, t))) {
                    std::cout << "Il treno " << tr->getNum() << " sta partendo alle ore " << convertiOra(t)
                              << std::endl;
                }
            }

            //controllo se il treno si sta avvicinando alla stazione (se è nei 5 km prima)
            //if true, controlla se la stazione può essere occupata e la mette nel parcheggio o nella stazione
            //DONE
            if (l.isApproachingStation(t, tr, IndexStazione)) {
                l.checkStation(IndexStazione, i);
            }

            //controllo se un treno è nel binario
            //DONE
            if ( stazione->controllaBinario(tr) != -1 && ( IndexStazione != (l.getNStazioni() - 1) ) )
            {
                //se è tempo di partire e il treno non si avvicina troppo al successivo, libera il binario e da un output
                if (t == (tr->getOra(IndexStazione) + tr->getRitardo() + 5)) {
                    if(!(l.isTooClose(i, IndexStazione, t))) {
                        std::cout << "Il treno " << tr->getNum() << " sta partendo dalla stazione "
                                  << stazione->getNome()
                                  << "alle ore " << convertiOra(t) << std::endl;
                        stazione->liberaBinario(tr);
                    }
                }
            }

            //controllo se c'è il treno nel parcheggio, se c'è un posto libero nella stazione lo toglie dal parcheggio e lo mette nella stazione
            if (stazione->controllaParcheggio(tr) && stazione->occupaBinario(tr) != -1)
            {
                stazione->liberaParcheggio(tr);
                std::cout << "Il treno ha lasciato il parcheggio e sta arrivando alla stazione" << std::endl;
            }

            //controllo se il treno è nella stazione finale e ha aspettato il tempo per far scendere i passeggeri
            //se ha finito, lo tolgo dal binario
            if(tr->daOrigine())
            {
                if (  IndexStazione == (l.getNStazioni() - 1) &&  t == tr->getOra(l.getNStazioni() - 1) + tr->getRitardo() + 5 )
                {
                    std::cout << "Arrivato alla stazione finale. Ha finito alle ore: " << convertiOra(t) << std::endl;
                    stazione->liberaBinario(tr);
                }
            }else{
                if(IndexStazione == 0 && t == tr->getOra(l.getNStazioni() - 1) + tr->getRitardo() + 5)
                {
                    std::cout << "Arrivato alla stazione finale. Ha finito alle ore: " << convertiOra(t) << std::endl;
                    stazione->liberaBinario(tr);
                }
            }
        }
    }
}
