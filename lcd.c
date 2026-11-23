#define __AVR_ATmega16A__
#define F_CPU 1000000UL // Częstotliwość taktowania (domyślnie 1 MHz dla fabrycznej ATmega16A)
#include <avr/io.h>
#include <util/delay.h>

// Definicje pinów (Port A)
#define LCD_DIR DDRA
#define LCD_PORT PORTA
#define RS PA0
#define E  PA1

// Impuls na linii Enable
void lcd_strobe(void) {
    LCD_PORT |= (1 << E);
    _delay_us(1);
    LCD_PORT &= ~(1 << E);
    _delay_us(100);
}

// Wysyłanie bajtu w trybie 4-bitowym
void lcd_write_byte(unsigned char data, unsigned char is_data) {
    if (is_data)
        LCD_PORT |= (1 << RS);  // Tryb danych
    else
        LCD_PORT &= ~(1 << RS); // Tryb instrukcji

    // Wysyłanie starszego półbajtu (nibble)
    LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);
    lcd_strobe();

    // Wysyłanie młodszego półbajtu
    LCD_PORT = (LCD_PORT & 0x0F) | ((data << 4) & 0xF0);
    lcd_strobe();

    _delay_ms(2);
}

// Inicjalizacja wyświetlacza HD44780
void lcd_init(void) {
    LCD_DIR |= 0xF3; // PA0, PA1 oraz PA4-PA7 jako wyjścia
    _delay_ms(45);

    // Sekwencja startowa dla magistrali 4-bitowej
    LCD_PORT = (LCD_PORT & 0x0F) | 0x30;
    lcd_strobe();
    _delay_ms(5);
    
    LCD_PORT = (LCD_PORT & 0x0F) | 0x30;
    lcd_strobe();
    _delay_us(150);
    
    LCD_PORT = (LCD_PORT & 0x0F) | 0x30;
    lcd_strobe();
    
    // Przełączenie w tryb 4-bitowy
    LCD_PORT = (LCD_PORT & 0x0F) | 0x20;
    lcd_strobe();

    // Konfiguracja: 2 wiersze, czcionka 5x8
    lcd_write_byte(0x28, 0);
    // Włączenie ekranu, wyłączenie kursora
    lcd_write_byte(0x0C, 0);
    // Czyszczenie ekranu
    lcd_write_byte(0x01, 0);
    // Automatyczny przesuw kursora w prawo
    lcd_write_byte(0x06, 0);
}

// Drukowanie tekstu
void lcd_print(const char *str) {
    while (*str) {
        lcd_write_byte(*str++, 1);
    }
}

int main(void) {
    lcd_init();
    lcd_print("Witaj, AVR!");

    while (1) {
        // Główna pętla programu
    }
}