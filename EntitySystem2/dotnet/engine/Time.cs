namespace engine;

public class Time
{
    private long time;
    public double asSeconds() {
        return time / 1000000.0;
    }

    public Time(long time)
    {
        this.time = time;
    }
}