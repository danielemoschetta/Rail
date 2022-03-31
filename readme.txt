	Corso di Laboratorio di Porgrammazione
	   SISTEMA DI SEGNALAZIOE DEI TRENI
		    2° assegnamento

SUDDIVISIONE DEL PROGETTO:
- Enrico Bolzonello
	classe LineaFerroviaria
- Daniele Moschetta
	classe Stazione
	input "line_description.txt"
	controllo stazioni in input
- Alice Favrin
	classe Treno
	input "timetables.txt"
	controllo orari treni in input

NOTE:
- Nei txt di input alcuni dati sono volutamente incorretti per testare le funzioni che li correggono.
- Tramite un istruzione cmake i file "line_description.txt" e "timetables.txt" vengono automaticamente copiati nella directory di build.
- CMakeLists.txt e' stato testato con CodeLite - MinGW Makefiles.