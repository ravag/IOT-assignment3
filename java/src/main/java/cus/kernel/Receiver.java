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
            if (msg != "" && !msg.contains("[DEBUG]")) {
                String[] values = msg.replace("MODE: ", "").replace("OPEN: ", "").replace("CHANGE: ", "").split(",");

                data.setCurrentOpening(Integer.parseInt(values[0]));
                if (!data.getState().equals(Mode.UNCONNECTED) && values[2].equals("true")) {
                    data.setState(Mode.valueOf(values[1]));
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
