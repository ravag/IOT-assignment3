package cus.mqtt;

import org.eclipse.paho.client.mqttv3.*;
import cus.data.*;

class StatoAcqua {
    float valore;

    public StatoAcqua() {
        valore = -100;
    }
}

public class SimpleSubscriber extends Thread{

    private final String broker = "tcp://broker.mqtt-dashboard.com";
    private final String clientId = "ESP-Project-" + System.currentTimeMillis();
    private final String topic = "ESP-Project";
    private MqttClient client;
    private DataPoint data;
    private boolean stopped;

    public SimpleSubscriber(DataPoint data) throws Exception{
        this.client = new MqttClient(broker, clientId);
        this.data = data; 
    }
    
    public void run() {
        StatoAcqua statoAttuale = new StatoAcqua();

        //Callback to handle messages and connection events
        client.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {
                System.out.println("Connection lost! " + cause.getMessage());
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) {
                System.out.println("Received message on topic " + topic + ": " + new String(message.getPayload()));
                String payload = new String(message.getPayload());

                //Estraggo solo il valore dell'acqua
                statoAttuale.valore = Float.parseFloat(payload.replace("Valore acqua: ", "").trim());

                if (statoAttuale.valore != -100) {
                    data.addData(statoAttuale.valore);
                    System.out.println("Valore: " + statoAttuale.valore);
                }
                
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });

        try {
            client.connect();

            /*
            * Subscribing - second param is the QoS (from broker to receiver)
            * 
            * - 0: at most once (minimum)
            * - 1: at least once
            * - 2: exactly once (maximum)
            */
            client.subscribe(topic, 1);

            System.out.println("Subscribed to topic: " + topic);


            // Keep the program running to listen for messages
            while (!stopped) {
                Thread.sleep(500);
            }
            client.disconnect();
            client.close();
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    public void terminate() {
        this.stopped = true;
    }
}
