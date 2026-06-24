package cus.data;

public enum State {
    MANUAL("MANUAL"),
    AUTOMATIC("AUTOMATIC"),
    UNCONNECTED("UNCONNECTED");

    private String name;

    private State(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }
}
