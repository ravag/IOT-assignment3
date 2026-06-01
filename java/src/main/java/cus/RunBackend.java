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
            arduino.waitForArduinoReady();
            arduino.sendMsg("Mi senti?");
            arduino.readResponse();
            arduino.disconnect();
        }
    }
}
