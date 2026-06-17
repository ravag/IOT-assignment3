#include "Arduino.h"
#include "devices/PotImpl.h"

// Creiamo un puntatore all'interfaccia Pot
Pot* mioPotenziometro;

void setup() {
    // Inizializza la comunicazione seriale a 9600 baud
    Serial.begin(9600);
    while (!Serial) {
        ; // Aspetta che la porta seriale si connetta (necessario per alcune schede come Leonardo/Micro)
    }

    // Istanziamo il potenziometro sul pin analogico A0
    mioPotenziometro = new PotImpl(A0);
    
    Serial.println("--- Test Potenziometro Avviato ---");
    Serial.println("Gira il potenziometro per vedere i cambiamenti...");
}

void loop() {
    // Controlliamo SE il valore è cambiato oltre la soglia di tolleranza
    if (mioPotenziometro->hasChanged()) {
        
        // Leggiamo sia il valore grezzo che la percentuale
        int valoreGrezzo = mioPotenziometro->getValue();
        int percentuale = mioPotenziometro->getPercentage();
        
        // Stampiamo i dati sul Serial Monitor
        Serial.print("Cambiamento rilevato! -> Valore ADC: ");
        Serial.print(valoreGrezzo);
        Serial.print(" | Percentuale: ");
        Serial.print(percentuale);
        Serial.println("%");
    }

    // Un piccolissimo delay per non sovraccaricare la CPU di Arduino
    delay(50);
}