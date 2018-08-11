/**
 * @author XiongHui Guo
 * @date 2018/08/07 13:27:29
 */
package net.i77studio.hashmap.test;

/**
 * @author XiongHui Guo
 * @date 2018/08/07 13:27:29
 *
 */
public class StopWatch {
    private long startTime;
    private long endTime;

    StopWatch() {
        this.startTime = 0;
        this.endTime = 0;
    }

    void reset() {
        this.startTime = 0;
        this.endTime = 0;
    }

    void restart() {
        this.startTime = System.currentTimeMillis();
        this.endTime = 0;
    }

    void start() {
        this.startTime = System.currentTimeMillis();
    }

    void stop() {
        this.endTime = System.currentTimeMillis();
    }

    double getElapsedMillsecs() {
        return (double) (this.endTime - this.startTime);
    }
}
