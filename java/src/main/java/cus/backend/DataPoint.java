package cus.backend;

import java.util.Calendar;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Stream;

import cus.Pair;

class DataPoint {
	private final List<Pair<Double,Calendar>> data = new LinkedList<>();
	private final int size;
	private int opening;
	private String state; /*Sostituisci con un enum */

	public DataPoint(int size, int opening, String state) {
		this.size = size;
		this.opening = opening;
		this.state = state;
		data.addAll(Stream.iterate(0,i -> i+1).limit(size).map(i -> new Pair<>(0.0, Calendar.getInstance())).toList());
	}

	public int getSize() {
		return size;
	}

	public int getOpening() {
		return this.opening;
	}

	public void setOpening(int opening) {
		this.opening = opening;
	}

	public String getState() {
		return state;
	}

	public void setState(String state) {
		this.state = state;
	}

	public void addData(Pair<Double,Calendar> newData) {
		data.addLast(newData);
		data.removeFirst();
	}
	
	public List<Pair<Double,Calendar>> getData() {
		return List.copyOf(data);
	}
}