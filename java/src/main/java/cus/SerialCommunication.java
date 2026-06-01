package cus;

import com.fazecast.jSerialComm.SerialPort;
import java.io.PrintWriter;
import java.util.Scanner;

public class SerialCommunication {
    private SerialPort comPort;
    private PrintWriter output;
    private Scanner input;
    private Thread listeningThread;

    public SerialCommunication(String portName, int baudRate) {
        this.comPort = SerialPort.getCommPort(portName);
        this.comPort.setBaudRate(baudRate);
    }

    public boolean connect() {
        if (!comPort.openPort()) {
            System.err.println("Errore. Impossibile aprire la porta " + comPort.getSystemPortName());
            return false;
        }

        System.out.println("Porta seriale aperta con successo su " + comPort.getSystemPortName());

        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        this.output = new PrintWriter(comPort.getOutputStream(), true);
        this.input = new Scanner(comPort.getInputStream());

        startListening();

        return true;
    }

    private void startListening() {
        listeningThread = new Thread(() -> {
            try {
                while (input != null && input.hasNextLine()) {
                    String recievedLine = input.nextLine();
                    System.out.println("[Ricevuto da Arduino]: " + recievedLine);
                }
            } catch (Exception e) {
                System.out.println("Lettura seriale interrotta.");
            }
        });

        listeningThread.setDaemon(true);
        listeningThread.start();
    }

    public void sendMsg(String msg) {
        if (output != null) {
            output.println(msg);
            System.out.println("[Inviato a Arduino]: " + msg);
        } else {
            System.err.println("[Errore]: Canale di output non pronto. Provare a controllare se la connessione è stata iniziata.");
        }
    }

    public void disconnect() {
        try {
            if (input != null) {
                input.close();
            }
            if (output != null) {
                output.close();
            }
            if (comPort != null && comPort.isOpen()) {
                System.out.println("Porta seriale chiusa correttamente.");
            }
        } catch (Exception e) {
            System.err.println("Errore durante la chiusura della porta seriale: " + e.getMessage());
        }
    }
}
