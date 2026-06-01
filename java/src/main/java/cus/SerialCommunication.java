package cus;

import com.fazecast.jSerialComm.SerialPort;
import java.io.PrintWriter;
import java.util.Scanner;

    public class SerialCommunication {
        private SerialPort comPort;
        private PrintWriter output;
        private Scanner input;

        public SerialCommunication(String portName, int baudRate) {
            this.comPort = SerialPort.getCommPort(portName);
            this.comPort.setBaudRate(baudRate);
        }

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

        public void sendMsg(String msg) {
            if (output != null) {
                output.println(msg);
                output.flush();
                System.out.println("[Inviato a Arduino]: " + msg);
            } else {
                System.err.println("[Errore]: Canale di output non pronto. Provare a controllare se la connessione è stata iniziata.");
            }

            readResponse();
        }

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

        private void readResponse() {
            if (input != null && input.hasNextLine()) {
                String response = input.nextLine();
                System.out.println("[Ricevuto da Arduino]: " + response);
            }
        }
    }
