package cus;

import cus.backend.DataService;
import io.vertx.core.Vertx;

public class RunBackend extends Thread{
    final static int PORT = 8080;

    public void run() {
        Vertx vertx = Vertx.vertx();
        DataService service = new DataService(PORT);
        vertx.deployVerticle(service);
    }
}
