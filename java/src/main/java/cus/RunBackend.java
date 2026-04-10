package cus;

import io.vertx.core.Vertx;

public class RunBackend {
    final static int PORT = 8080;

    public static void main(String[] args) {
        Vertx vertx = Vertx.vertx();
        DataService service = new DataService(PORT);
        vertx.deployVerticle(service);
    }
}
