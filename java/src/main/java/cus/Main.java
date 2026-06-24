package cus;

import cus.backend.RunBackend;
import cus.data.DataPoint;
import cus.data.Mode;
import cus.kernel.SerialCommunication;
import cus.kernel.valveManagement;
import cus.mqtt.SimpleSubscriber;

public class Main {
    
    public static void main(String[] args) throws Exception{
        DataPoint data = new DataPoint(10, 0, Mode.AUTOMATIC, 0);
        RunBackend backend = new RunBackend(data);
        SimpleSubscriber subscriber = new SimpleSubscriber(data);
        valveManagement valve = new valveManagement(new SerialCommunication("COM3", 115200), data);
        subscriber.start();
        backend.start();
        valve.start();
    }
}
