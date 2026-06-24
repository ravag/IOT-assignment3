package cus.backend;

import cus.data.DataPoint;
import io.vertx.core.Vertx;

public class RunBackend extends Thread{
    final static int PORT = 8080;
    private final DataPoint data;
    private final Vertx vertx;
    private boolean running = true; 

    public RunBackend(DataPoint data) {
        this.data = data;
        vertx = Vertx.vertx();
    }

    public void run() {
        DataService service = new DataService(PORT,data);
        vertx.deployVerticle(service);
    }

    public void terminate() {
        vertx.close().onComplete(v -> this.running = false);
    }

    public boolean verticleStatus() {
        return running;
    }
}
