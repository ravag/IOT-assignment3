package cus.kernel;

import cus.data.*;

enum Danger{
    IDLE,
    OVER_L1,
    OVER_L2,
    WAIT;
}

public class valveManagement extends Thread{
    
    private boolean stopped = false;
    private SerialCommunication serial;
    private DataPoint data;
    private Danger danger;
    private long time;
    private long now;
    private static final double L1 = 50.0;
    private static final double L2 = 75.0;
    private static final int T1 = 10;

    public valveManagement(SerialCommunication serial, DataPoint data) {
        this.serial = serial;
        this.data = data;
        time = System.currentTimeMillis();
        danger = Danger.IDLE;
    }

    public void run() {
        serial.connect();
        while (!stopped) {
            if (data.getState() != Mode.AUTOMATIC) {
                //mandare all'arduino l'apertura dell'operatore sul sito
                serial.sendMsg("MODE: " + data.getState() + ", "
                    + "OPEN: " + data.getOpeningObjective());

            } else {
                //ragionare su quanto aprire in base ai dati dell'ESP

                if (danger.equals(Danger.IDLE)) {       //se sono in IDLE

                    //controllo se si è superato il livello soglia L1
                    if (data.getWaterLevel() >= L1) {
                        time = System.currentTimeMillis();
                        danger = Danger.OVER_L1;
                    }

                } else if (danger.equals(Danger.OVER_L1)) {     //se sono sopra la soglia L1

                    //controllo se si è direttamente sopra la soglia L2
                    if (data.getWaterLevel() >= L2) {
                        danger = Danger.OVER_L2;

                        //inviare all'arduino di aprire la valvola al 100%
                        data.setOpeningObjective(100);
                        serial.sendMsg("MODE: " + data.getState() + ", "
                            + "OPEN: " + data.getOpeningObjective());
                    }

                    //controllo se sono in questo stato da un tempo maggiore di T1
                    now = System.currentTimeMillis();
                    if ((now - time) >= T1) {
                        danger = Danger.WAIT;

                        //inviare all'arduino di aprire la valvola al 50%
                        data.setOpeningObjective(50);
                        serial.sendMsg("MODE: " + data.getState() + ", "
                            + "OPEN: " + data.getOpeningObjective());
                    }
                
                } else if (danger.equals(Danger.WAIT)) {    //se sono in WAIT

                    //controllo se il livello dell'acqua ha superato la soglia L2
                    if (data.getWaterLevel() >= L2) {
                        danger = Danger.OVER_L2;

                        //inviare all'arduino di aprire la valvola al 100%
                        data.setOpeningObjective(100);
                        serial.sendMsg("MODE: " + data.getState() + ", "
                            + "OPEN: " + data.getOpeningObjective());

                    } else if (data.getWaterLevel() <= L1) {    //caso in cui l'acqua è tornata sotto il livello L1
                        danger = Danger.IDLE;

                        //inviare all'arduino di aprire la valvola al 0%
                        data.setOpeningObjective(0);
                        serial.sendMsg("MODE: " + data.getState() + ", "
                            + "OPEN: " + data.getOpeningObjective());
                    }



                } else {    //se sono sopra la soglia L2

                    //controllo se la soglia dell'acqua è tornata sotto il livello L2
                    if (data.getWaterLevel() <= L2) {
                        danger = Danger.WAIT;

                        //inviare all'arduino di aprire la valvola al 50%
                        data.setOpeningObjective(50);
                        serial.sendMsg("MODE: " + data.getState() + ", "
                            + "OPEN: " + data.getOpeningObjective());
                    }
                }
            }
        }
        serial.disconnect();
    }

    public void terminate() {
        this.stopped = true;
    }
}
