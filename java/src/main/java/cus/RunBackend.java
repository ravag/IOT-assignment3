package cus;

import io.vertx.core.Vertx;

public class RunBackend {
    final static int PORT = 8080;

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        DataService service = new DataService(PORT);
        vertx.deployVerticle(service);

        SerialCommunication arduino = new SerialCommunication("COM3", 115200);
        if (arduino.connect()) {
            //try{Thread.sleep(5000);} catch (InterruptedException e) {}
            arduino.sendMsg("test invio");
            //try{Thread.sleep(5000);} catch (InterruptedException e) {}
            arduino.disconnect();

            
        }
    }
}
