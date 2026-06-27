package cus.kernel;

import cus.data.Mode;

public class TestRecv extends Thread {
    SerialCommunication serial;
    private boolean stopped = false;
    private Mode mode;
    private String msg;

    public TestRecv(SerialCommunication serial) {
        this.serial = serial;
        mode = Mode.MANUAL;
    }

    public void run() {
        while (!stopped) {
            msg = serial.readResponse();
            if (msg != "") {
                System.out.println(msg);
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
