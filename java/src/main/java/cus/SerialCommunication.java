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

            System.out.println("Porta seriale aperta con successo su " + comPort.getSystemPortName());

            this.output = new PrintWriter(comPort.getOutputStream(), true);
            this.input = new Scanner(comPort.getInputStream());

            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

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
                output.flush();
                System.out.println("[Inviato a Arduino]: " + msg);
            } else {
                System.err.println("[Errore]: Canale di output non pronto. Provare a controllare se la connessione è stata iniziata.");
            }
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
    }
