package cus.kernel;

import cus.data.DataPoint;
import cus.data.Mode;

public class Receiver extends Thread {
    private SerialCommunication serial;
    private boolean stopped = false;
    private String msg;
    private DataPoint data;

    public Receiver(SerialCommunication serial, DataPoint data) {
        this.serial = serial;
        this.data = data;
        this.msg = "";
    }

    public void run() {
        while (!stopped) {
            msg = serial.readResponse();
            if (msg != "" || !msg.contains("[DEBUG]")) {
                System.out.println(msg);
                String[] spaced = msg.split(" ");
                System.out.println(spaced);
                data.setCurrentOpening(Integer.parseInt(spaced[1]));
                if (!data.getState().equals(Mode.UNCONNECTED)) {
                    data.setState(Mode.valueOf(spaced[3]));
                }
            }
            
            try {
                Thread.sleep(250);
            } catch (Exception e) {
                System.err.println("WompWomp");
            }
        }
    }

    public void terminate() {
        this.stopped = true;
    }
}
