package cus.backend;

import java.util.Calendar;
import java.util.Date;
import java.util.Random;

import cus.data.DataPoint;
import cus.data.Pair;
import cus.data.State;
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
	private DataPoint values;
	private Random rnd = new Random();
	
	public DataService(int port) {
		values = new DataPoint(10, 0, State.AUTOMATIC,0);		
		this.port = port;
	}

	@Override
	public void start() {		
		Router router = Router.router(vertx);

		router.post("/api/mode").handler(BodyHandler.create().setHandleFileUploads(false).setBodyLimit(-1)).handler(this::handleModeChange);

		router.route("/api/data").handler(BodyHandler.create());
		router.get("/api/data").handler(this::handleGetData);
		router.post("/api/data").handler(this::handleNewOpening);

		

		router.get().handler(StaticHandler.create("webroot")).handler(this::handlePage);

		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);

		log("Service ready on port: " + port);
	}

	private void handleModeChange(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		JsonObject res = routingContext.body().asJsonObject();
		if (res == null) {
			sendError(400, response);
		} else {
			values.setState(State.valueOf(res.getString("state")));
			log("Received new status: " + res.getString("state"));
			response.setStatusCode(200).end();
		}
	}

	private void handlePage(RoutingContext routingContext) {
		HttpServerRequest request = routingContext.request();
		HttpServerResponse response = routingContext.response();
		if (request.path().equals("/")) {
			response.sendFile("src\\main\\resources\\webroot\\index.html");
		}else{
			response.sendFile( "src\\main\\resources\\webroot" + request.path() );
		}
	}
	
	private void handleNewOpening(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		log("new msg "+routingContext.body().asString());
		JsonObject res = routingContext.body().asJsonObject();
		if (res == null) {
			sendError(400, response);
		} else {
			float value = res.getFloat("value");
			String place = res.getString("place");
			long time = System.currentTimeMillis();
			
			/* values.addLast(new DataPoint(value, time, place));
			if (values.size() > MAX_SIZE) {
				values.removeFirst();
			} */
			
			log("New value: " + value + " from " + place + " on " + new Date(time));
			response.setStatusCode(200).end();
		}
	}
	
	private void handleGetData(RoutingContext routingContext) {
		JsonArray arr = new JsonArray();
		values.addData(new Pair<>(rnd.nextDouble(),Calendar.getInstance()));

		for (Pair<Double,Calendar> p: values.getData()) {
			JsonObject data = new JsonObject();
			data.put("time", p.b().get(Calendar.MINUTE) + ":" + p.b().get(Calendar.SECOND));
			data.put("value", p.a());
			arr.add(data);
		} 

		JsonObject data = new JsonObject();
		values.setCurrentOpening(rnd.nextInt(100));
		data.put("data",arr);
		data.put("opening",values.getCurrentOpening());
		data.put("status", values.getState());

		routingContext.response()
			.putHeader("content-type", "application/json")
            //.sendFile("src\\main\\resources\\*");
			.end(data.encodePrettily());
	}
	
	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[DATA SERVICE] "+msg);
	}

}
