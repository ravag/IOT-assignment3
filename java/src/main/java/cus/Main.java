package cus;

import cus.backend.RunBackend;
import cus.data.DataPoint;
import cus.data.State;
import cus.mqtt.SimpleSubscriber;

public class Main {
    
    public static void main(String[] args) throws Exception{
        DataPoint data = new DataPoint(10, 0, State.AUTOMATIC, 0);
        RunBackend backend = new RunBackend(data);
        SimpleSubscriber subscriber = new SimpleSubscriber(data);
        subscriber.start();
        backend.start();
    }
}
