namespace engine;

public class Time
{
    private long time;
    public float asSeconds() {
        return (float)(time / 1000000.0);
    }

    public Time(long time)
    {
        this.time = time;
    }
}