package cus.data;

import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Stream;

public class DataPoint {
	private final List<Pair<Double,Calendar>> data = new LinkedList<>();
	private final int size;
	private int currentOpening;
	private int openingObjective;
	private State state; /*Sostituisci con un enum */

	public DataPoint(int size, int opening, State state, int openingObjective) {
		this.size = size;
		this.currentOpening = opening;
		this.state = state;
		this.openingObjective = openingObjective;
		data.addAll(Stream.iterate(0,i -> i+1).limit(size).map(i -> new Pair<>(0.0, Calendar.getInstance())).toList());
	}

	public synchronized int getSize() {
		return size;
	}

	public synchronized int getCurrentOpening() {
		return this.currentOpening;
	}

	public synchronized void setCurrentOpening(int opening) {
		this.currentOpening = opening;
	}

	public synchronized int getOpeningObjective() {
		return openingObjective;
	}

	public synchronized void setOpeningObjective(int openingObjective) {
		this.openingObjective = openingObjective;
	}

	public synchronized State getState() {
		return state;
	}

	public synchronized void setState(State state) {
		this.state = state;
	}

	public synchronized void addData(Float newData) {
		Double value = (double)newData;
		data.addLast(new Pair<Double, Calendar>(value, Calendar.getInstance()));
		data.removeFirst();
	}
	
	public synchronized List<Pair<Double,Calendar>> getData() {
		return List.copyOf(data);
	}

	
}