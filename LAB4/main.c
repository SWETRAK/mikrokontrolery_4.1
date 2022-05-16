/*
 * LAB4.c
 *
 * Created: 12.05.2022 12:26:05
 * Author : Kamil Pietrak
 */ 

#include <avr/io.h>
#include <util/delay.h>

void klawiatura(); // inicjalizacja nagłówka funkcji

int main(void)
{
    DDRA = 0xF0; // konfiguracja PORT A 4 starsze bity jako wejscie, a 4 młodsze jako wyjscie
	PORTA = 0x0F; // Podpięcie wewnętrznego rezystora podciągającego do VCC
	
	DDRB = 0xFF; // Konfiguracja PORT B jako wyjscie dla diod (uzywane jako wyświetlacz)
	PORTB = 0x00; // Zgaszenie wszytskich diod 
	
	
    while (1) {
		
		getKey(); // Wywołanie w głównej części programu funkcji odpowiadającej za czytanie klawiatury
		
    }
}

void getKey() {
	// volatile uint8_t zmienna; // zmiena uzywana do przechowywanania wartośći zmiennej (NIE URZYWANA W TEJ WERSJI FUNKCJI)
	uint8_t problem = 0; // zmienna której wartość jest większa niz 1 w przypadku wciśniętych dwóch przycisków
	for (uint8_t i = 4; i<8; i++) { // pętla iterująca przez kolumny w klawiaturze 
		PORTA = ~(1<<i); // odpalenie odpowiedniej kolumny w klawiaturze 
		uint8_t kolumna = i-4; // zmienna pomocnicza do obliczania wartości końcowej
		uint8_t k = PINA; // Zczyatnie stanu PORT A 
		if(k == 0xFF ) { // Jeśli na kalwiaturze nacisniete jest wiecej niz jeden przycisk w wierszu to wyswietl 255 i zakoncz dzialanie 
			PORTB = 255; // wyswietlanie na diodach 255 
			break; // konczenie wykonywania denego cyklu pętli
		}
		uint8_t x = PINA&0x0F; // zczytanie stanu PORT A i zmaskowanie go tak aby pozostały tylko 4 młodsze bity
		// Instrukcja switch decydujaca jaka wartość bedzie zwrócona w przypadku nacisniecia danego przycisku 
		switch(x) {		
			case 0b00001111: // zaden przycisk nie jest wcisniety 
				PORTB = 0; // gasi wszytskie diody
			break;
			case 0b00001110: // wcisniety przycisk w wierszu 1 
				problem ++; // zwieksz flage bledów o jeden
				PORTB = 1 + kolumna; // zaświeć 1 dla pierwszej kolumny, 2 dla drugiej 3 dla trzeciej i 4 dla czwartej
			break;
			case 0b00001101: // wcisniety przycisk w wierszu 2
				problem ++; // zwieksz flage bledów o jeden
				PORTB = 5 + kolumna; // zaświeć 5 dla pierwszej kolumny, 6 dla drugiej 7 dla trzeciej i 8 dla czwartej
			break;
			case 0b00001011:
				problem ++; // zwieksz flage bledów o jeden
				PORTB = 9 + kolumna; // zaświeć 9 dla pierwszej kolumny, 10 dla drugiej 11 dla trzeciej i 12 dla czwartej
			break;
			case 0b00000111:
				problem ++; // zwieksz flage bledów o jeden
				PORTB = 13 + kolumna; // zaświeć 13 dla pierwszej kolumny, 14 dla drugiej 15 dla trzeciej i 16 dla czwartej
			break;
			default:
				PORTB = 255; // Zaswiec 255 w przypadku bledu (dwa klawisze w tej samej kolumnie)
			break;
		}
	}
	if(problem > 1) { // jesli problem jest wiekszy niz jeden to znaczy ze wystąpił błąd i trzeba wyświetlic 255
		PORTB = 255; // Dwa klawisze po przekątnych (nie w kolumnie i nie w rzędzie)
	}

}
