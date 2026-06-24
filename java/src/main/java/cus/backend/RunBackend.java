package cus.backend;

import cus.data.DataPoint;
import io.vertx.core.Vertx;

public class RunBackend extends Thread{
    final static int PORT = 8080;
    private final DataPoint data;

    public RunBackend(DataPoint data) {
        this.data = data;
    }

    public void run() {
        Vertx vertx = Vertx.vertx();
        DataService service = new DataService(PORT,data);
        vertx.deployVerticle(service);
    }
}
