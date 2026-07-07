package cus;

import java.lang.Thread.State;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.WindowConstants;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import cus.backend.RunBackend;
import cus.data.DataPoint;
import cus.data.Mode;
import cus.kernel.Receiver;
import cus.kernel.SerialCommunication;
import cus.kernel.valveManagement;
import cus.mqtt.SimpleSubscriber;

public class Main {
    
    public static void main(String[] args) throws Exception{
        DataPoint data = new DataPoint(10, 0, Mode.AUTOMATIC, 0);
        RunBackend backend = new RunBackend(data);
        SimpleSubscriber subscriber = new SimpleSubscriber(data);
        //valveManagement valve = new valveManagement(new SerialCommunication("COM3", 115200), data);
        Receiver recv = new Receiver(new SerialCommunication("COM5", 115200), data);
        subscriber.start();
        backend.start();
        recv.start();
        //valve.start();

        JFrame frame = new JFrame("Stopper");
        JButton btn = new JButton("Termina cus");
        btn.addActionListener(e -> {
            backend.terminate();
            subscriber.terminate();
            recv.terminate();
            //valve.terminate();
        });
        btn.setSize(400, 200);
        frame.add(btn);
        frame.setVisible(true);
        frame.setSize(400, 200);

        //Listener to close correctly all systems
        frame.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        WindowListener l = new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                backend.terminate();
                subscriber.terminate();
                recv.terminate();
                //valve.terminate();
            }
        };
        frame.addWindowListener(l);

        while (subscriber.getState() != State.TERMINATED || backend.verticleStatus()){ //|| valve.getState() != State.TERMINATED) {
            Thread.sleep(1000);
        }
        System.exit(0);
    }
}
