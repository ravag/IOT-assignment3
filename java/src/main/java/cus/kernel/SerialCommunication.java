package cus.kernel;

import com.fazecast.jSerialComm.SerialPort;
import java.io.PrintWriter;
import java.util.Scanner;

/**
 * Gestisce la comunicazione seriale bidirezionale e sincrona con una scheda Arduino Uno.
 * <p>
 * La classe si occupa dell'apertura della porta COM, della configurazione dei segnali hardware (DTS/RTS), della gestione del reset della scheda tramite handshake e della chiusura pulita dei flussi per evitare il blocco della risorsa nel sistema operativo.
 * </p>
 * @author Marco
 * @version 1.0
 */
public class SerialCommunication {
    private SerialPort comPort;
    private PrintWriter output;
    private Scanner input;

    /**
     * Costruisce un'istanza di comunicazione seriale assegnando il nome della porta e il baud rate.
     * @param portName Il nome della porta seriale.
     * @param baudRate La velocità di trasmissione della linea seriale.
     */
    public SerialCommunication(String portName, int baudRate) {
        this.comPort = SerialPort.getCommPort(portName);
        this.comPort.setBaudRate(baudRate);
    }

    /**
     * Apre la porta seriale, configura i segnali hardware DTR/RST e inizializza i flussi I/O.
     * Al termine dell'apertura, invoca il metodo di handshake {@link #waitForArduinoReady()} per attendere il riavvio della scheda.
     * @return {@code true} se la porta è stata aperta e inizializzata con successo; 
     * {@code false} in caso di errore di apertura.
     */
    public boolean connect() {
        if (comPort.isOpen()) {
            comPort.closePort();
        }

        if (!comPort.openPort()) {
            System.err.println("Errore. Impossibile aprire la porta " + comPort.getSystemPortName() + ". Ricordati di chiudere l'esecuzione precedente in background.");
            return false;
        }

        comPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0);
        comPort.setDTR();
        comPort.setRTS();

        this.output = new PrintWriter(comPort.getOutputStream(), true);
        this.input = new Scanner(comPort.getInputStream());

        waitForArduinoReady();

        System.out.println("Porta seriale aperta con successo su " + comPort.getSystemPortName());

        return true;
    }

    private void waitForArduinoReady() {
        System.out.println("In attesa del segnale di pronto da Arduino Uno...");

        if (input == null) {
            System.err.println("Scanner non inizializzato.");
            return;
        }

        while (input.hasNextLine()) {
            String line = input.nextLine();
            System.out.println("[DEBUG]: java ha letto nel boot -> " + line);
            if (line.contains("READY")) {
                System.out.println("Arduino è sveglio e pronto!");
                break;
            }
        }
    }

    /**
     * Invia un messaggio di testo ad Arduino sul canale seriale.
     * <p>
     * Al termine dell'invio, esegue il flush del buffer e invoca immediatamente il metodo privato {@link #readResponse()} per attendere in modo sincrono la risposta di Arduino, garantendo l'odrine sequenziale richiesta-risposta.
     * </p>
     * @param msg Il messaggio di testo da trasmettere (verrà appeso automaticamente il carattere '\n').
     */
    public void sendMsg(String msg) {
        if (output != null) {
            output.println(msg);
            output.flush();
            System.out.println("[Inviato a Arduino]: " + msg);
        } else {
            System.err.println("[Errore]: Canale di output non pronto. Provare a controllare se la connessione è stata iniziata.");
        }
    }

    /**
     * Rilascia in modo sicuro le risorse hardware e i flussi software di I/O.
     * <p>
     * <b>Nota di design critica:</b> La porta seriale fisica viene chiusa <i>PRIMA</i> dei flussi logici. 
     * Questo invia un segnale di End-Of-File (EOF) allo Scanner che potrebbe essere bloccato in lettura, sbloccandolo instantaneamente ed evitando situazioni di deadlock.
     * </p>
     */
    public void disconnect() {
        try {
            System.out.println("Disconnessione dalla porta seriale in corso...");
            if (comPort != null && comPort.isOpen()) {
                comPort.closePort();
                System.out.println("Porta seriale chiusa correttamente.");
            }

            if (input != null) {
                input.close();
            }
            if (output != null) {
                output.close();
            }
            System.out.println("Tutti i flussi I/O sono stati chiusi correttamente.");

        } catch (Exception e) {
            System.err.println("Errore durante la chiusura della porta seriale: " + e.getMessage());
        }
    }

    public String readResponse() {
        if (input != null && input.hasNextLine()) {
            String response = input.nextLine();
            System.out.println("[Ricevuto da Arduino]: " + response);
            return response;
        }
        return "";
    }
}
