#include <LiquidCrystal.h>
LiquidCrystal Lcd(0, 1, 3, 4, 5, 6);
int const pin_segmenti[]{13, 12, 11, 10};
#define pin_punto 9
#define pin_unita 7
#define pin_decimi 8
#define pin_puls 2
int const pin_lcd[]{0, 1, 3, 4, 5, 6};
#define pin_pot A5
float numeroDaVisualizzare = 0.0;

void setup()
{
    //setup pin I/O
    for (int i = 0; i < 14; i++)
        pinMode(i, (i == pin_puls) ? INPUT : OUTPUT);
    
    //setup interrupt
    attachInterrupt(digitalPinToInterrupt(pin_puls), incrementa, RISING);
    
    //setup per l'lcd
    Lcd.begin(16, 2);
    Lcd.home();
    Lcd.print("Valore: ");
    Lcd.setCursor(0, 1);
    Lcd.print("Tensione: ");
}

void loop()
{
    //scrittura sul 7 segmenti
    scriviSegmenti(numeroDaVisualizzare);
    //scrittura sull'lcd
    scriviLcd();
}

void incrementa()//funzione di interrupt che aumenta il numero da visualizzare di 0.1
{
    (numeroDaVisualizzare < 1.2) ? numeroDaVisualizzare += 0.1 : numeroDaVisualizzare = 0;
}

void scriviLcd()//funzione per gestire la scrittura del valore del potenziometro sull'lcd
{
    float tens_pot = map(analogRead(pin_pot), 0, 1023, 0, 500);
    tens_pot /= 100;
    Lcd.setCursor(8, 0);
    Lcd.print("    ");
    Lcd.setCursor(8, 0);
    Lcd.print(analogRead(pin_pot));
    Lcd.setCursor(10, 1);
    Lcd.print("    ");
    Lcd.setCursor(10, 1);
    Lcd.print(tens_pot);
}


void scriviSegmenti(float numeroDecimale)//funzione per scrivere il numero sul 7 segmenti
{ 
    int intera = int(numeroDecimale); //si ricava la parte intera del numero da scrivere
    int frazionaria = (numeroDecimale * 10 - intera * 10);//si ricava la parte decimale del numero da scrivere

    bool *pBool = dec_bin(intera);//pointer all'array contenente la cifra convertita in binario

    /* scrivi prima cifra */
    //abilita il display di sinistra 
    digitalWrite(pin_unita, 1);
    digitalWrite(pin_decimi, 0);
    //accende il led del punto decimale
    digitalWrite(pin_punto, 1);
    //alimenta il cd4511 secondo il numero convertito
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(pin_segmenti[i], pBool[i]);
    }
    delay(50);

    pBool = dec_bin(frazionaria); //riassegna il pointer all'array contenente la seconda cifra in binario

    /* scrivi seconda cifra */
    //abilita il display di destra
    digitalWrite(pin_unita, 0);
    digitalWrite(pin_decimi, 1);
    //spegne il led del punto decimale
    digitalWrite(pin_punto, 0);
    //alimenta il cd4511 secondo il numero convertito
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(pin_segmenti[i], pBool[i]);
    }
    delay(50);
}

bool* dec_bin(int numero_decimale)//funzione per trasformre da valore intero a binario (solo una cifra)
{
    static bool binario[4];//dichiarazione array contenente il numero convertito
    //conversione da base 10 a base 2
    for (int i = 0; i < 4; i++)
    {
        binario[i] = numero_decimale % 2;
        numero_decimale /= 2;
    }
    
    return binario;//ritorna l'indirizzo dell'array 
}

void scriviSegmenti(int numeroDecimale)//function overload per numeri senzza virgola
{
    int decine = numeroDecimale/10;
    int unita = numeroDecimale-decine*10;

    bool *pBool = dec_bin(decine);//pointer all'array contenente la cifra convertita in binario

    /* scrivi prima cifra */
    //abilita il display di sinistra 
    digitalWrite(pin_unita, 1);
    digitalWrite(pin_decimi, 0);
    //spegne il led del punto decimale
    digitalWrite(pin_punto, 0);
    //alimenta il cd4511 secondo il numero convertito
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(pin_segmenti[i], pBool[i]);
    }
    delay(50);

    pBool = dec_bin(unita); //riassegna il pointer all'array contenente la seconda cifra in binario

    /* scrivi seconda cifra */
    //abilita il display di destra
    digitalWrite(pin_unita, 0);
    digitalWrite(pin_decimi, 1);
    //spegne il led del punto decimale
    digitalWrite(pin_punto, 0);
    //alimenta il cd4511 secondo il numero convertito
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(pin_segmenti[i], pBool[i]);
    }
    delay(50);
}