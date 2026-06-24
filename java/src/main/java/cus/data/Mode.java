package cus.data;

public enum Mode {
    MANUAL("MANUAL"),
    AUTOMATIC("AUTOMATIC"),
    UNCONNECTED("UNCONNECTED");

    private String name;

    private Mode(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() {
        return this.name;
    }
}
