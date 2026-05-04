package cus;

import java.util.Date;
import java.util.LinkedList;
import java.util.Random;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpServerRequest;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.StaticHandler;

public class DataService extends AbstractVerticle {
    
    private int port;
	private static final int MAX_SIZE = 10;
	private LinkedList<DataPoint> values;
	private Random rnd = new Random();
	
	public DataService(int port) {
		values = new LinkedList<>();		
		this.port = port;
	}

	@Override
	public void start() {		
		Router router = Router.router(vertx);

		router.route("/api/data").handler(BodyHandler.create());
		
		router.get("/api/data").handler(this::handleGetData);
		router.post("/api/data").handler(this::handleAddNewData);

		router.route().handler(StaticHandler.create("webroot"));
		router.route().handler(routingContext -> {
			HttpServerRequest request = routingContext.request();
			HttpServerResponse response = routingContext.response();
			if (request.path().equals("/")) {
				response.sendFile("src\\main\\resources\\webroot\\index.html");
			}else{
				response.sendFile( "src\\main\\resources\\webroot" + request.path() );
 			}
		});

		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);

		log("Service ready on port: " + port);
	}
	
	private void handleAddNewData(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		log("new msg "+routingContext.body().asString());
		JsonObject res = routingContext.body().asJsonObject();
		if (res == null) {
			sendError(400, response);
		} else {
			float value = res.getFloat("value");
			String place = res.getString("place");
			long time = System.currentTimeMillis();
			
			values.addLast(new DataPoint(value, time, place));
			if (values.size() > MAX_SIZE) {
				values.removeFirst();
			}
			
			log("New value: " + value + " from " + place + " on " + new Date(time));
			response.setStatusCode(200).end();
		}
	}
	
	private void handleGetData(RoutingContext routingContext) {
		JsonArray arr = new JsonArray();
		values.addLast(new DataPoint(rnd.nextDouble(), System.currentTimeMillis(), "ciao"));
		if (values.size() > MAX_SIZE) {
				values.removeFirst();
			}

		for (DataPoint p: values) {
			JsonObject data = new JsonObject();
			data.put("time", p.getTime());
			data.put("value", p.getValue());
			data.put("place", p.getPlace());
			arr.add(data);
		} 
		routingContext.response()
			.putHeader("content-type", "application/json")
            //.sendFile("src\\main\\resources\\*");
			.end(arr.encodePrettily());
	}
	
	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[DATA SERVICE] "+msg);
	}

}
