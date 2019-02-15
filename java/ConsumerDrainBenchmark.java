import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

import java.util.concurrent.atomic.AtomicLong;
import org.junit.jupiter.api.Test;

public class ConsumerDrainBenchmark {

  static AtomicLong work = new AtomicLong(0);

  interface Consumer {
    void populate();
    List<Push> drain(int maxRecords);
  }

  static class Push {
    static Random generator = new Random();
    public Push () {
      //generator.nextBytes(key);
      //generator.nextBytes(value);
    }
    private byte[] key = new byte[16];
    private byte[] value = new byte[128];
  }

  static int toUnsignedInt(int x) {
    return x & 0x7fffffff;
  }

  static class MultiQueueConsumer implements Consumer {
    public MultiQueueConsumer(int streams, int iters) {
      this.numStreams = streams;
      this.numIterations = iters;
      queues = new ConcurrentHashMap<>(numStreams);
      streamsArray = new ArrayList<UUID>(numStreams);

      for(int i = 0;i < numStreams; ++i) {
        UUID uuid = UUID.randomUUID();
        streamsArray.add(uuid);
        queues.put(uuid, new ConcurrentLinkedQueue<Push>());
      }
    }

    @Override
    public void populate() {
      for(int i = 0; i < numIterations; ++i) {
        int index = toUnsignedInt(generator.nextInt()) % numStreams;
        UUID stream = streamsArray.get(index);
        queues.get(stream).add(new Push());
      }
    }

    @Override
    public List<Push> drain(int maxRecords) {
      List<Push> records = new ArrayList<>();
      int numQueueChecked = 0;
      while (maxRecords > 0) {
        UUID stream = streamsArray.get(streamIndex);
        ConcurrentLinkedQueue<Push> queue = queues.get(stream);
        if (queue.isEmpty()) {
          ++numQueueChecked;
          ++streamIndex;
          if (streamIndex == numStreams)
            streamIndex = 0;
        }
        else {
          while (maxRecords > 0 && !queue.isEmpty()) {
            records.add(queue.remove());
            --maxRecords;
          }
        }
        if (numQueueChecked == numStreams)
          return records;
      }

      return records;
    }

    private int numStreams;
    private int numIterations;
    private ConcurrentHashMap<UUID, ConcurrentLinkedQueue<Push>> queues;
    private ArrayList<UUID> streamsArray;
    private Random generator = new Random();
    private int streamIndex = 0; // wraps around at numStreams
  }

  static class SingleQueueConsumer implements Consumer {
    public SingleQueueConsumer(int streams, int iters) {
      this.numIterations = iters;
      queue = new ConcurrentLinkedQueue<Push>();
    }

    @Override
    public void populate() {
      for(int i = 0; i < numIterations; ++i) {
        queue.add(new Push());
      }
    }

    @Override
    public List<Push> drain(int maxRecords) {
      List<Push> records = new ArrayList<>();
      while (maxRecords > 0 && !queue.isEmpty()) {
        records.add(queue.remove());
        --maxRecords;
      }

      return records;
    }

    private int numIterations;
    private ConcurrentLinkedQueue<Push> queue;
  }

  public void processRecords(List<Push> records) {
    for(int i = 0; i < records.size() * 10000; ++i) {
      work.incrementAndGet();
    }
  }

  public void runTest(Consumer testConsumer, int numStreams, int numIterations, int maxRecords) {
    try {
      final AtomicInteger totalRecordsDrained = new AtomicInteger(0);

      long startTime = System.nanoTime();
      Thread producerThread = new Thread(testConsumer::populate);
      Thread readerThread = new Thread(() -> {
        List<Push> records = null;
        do {
          records = testConsumer.drain(maxRecords);
          totalRecordsDrained.addAndGet(records.size());
          processRecords(records);
        } while(!records.isEmpty());
      });

      producerThread.run();
      readerThread.run();

      producerThread.join();
      readerThread.join();
      if (totalRecordsDrained.get() !=  numIterations)
        throw new java.lang.RuntimeException("numRecords did not match");

      long endTime = System.nanoTime();
      System.out.println(String.format("%s: numIterations = %9d, numStream = %7d, maxRecords = %7d in %6d millis",
          testConsumer.getClass().getSimpleName(), numIterations, numStreams, maxRecords, (endTime - startTime) / 1000 / 1000));
    }
    catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  @Test
  void Warmup() {
    try {
      Thread t = new Thread(UUID::randomUUID);
      t.run();
      t.join();
    }
    catch(Exception ex) {

    }
  }

  @Test
  public void testMultiQueueConsumer() {
    long startTime = System.nanoTime();
    for (int numIterations: numIterationsList) {
      for (int numStreams: numStreamsList) {
        for (int maxRecords: maxRecordsList) {
          MultiQueueConsumer consumer = new MultiQueueConsumer(numStreams, numIterations);
          runTest(consumer, numStreams, numIterations, maxRecords);
        }
      }
    }
    long endTime = System.nanoTime();
    System.out.println(String.format("testMultiQueueConsumer time = %d millis", (endTime-startTime)/1000/1000));
  }

  @Test
  public void testSingleQueueConsumer() {
    long startTime = System.nanoTime();
    for (int numIterations: numIterationsList) {
      for (int numStreams: numStreamsList) {
        for (int maxRecords: maxRecordsList) {
          SingleQueueConsumer consumer = new SingleQueueConsumer(numStreams, numIterations);
          runTest(consumer, numStreams, numIterations, maxRecords);
        }
      }
    }
    long endTime = System.nanoTime();
    System.out.println(String.format("testSingleQueueConsumer time = %d millis", (endTime-startTime)/1000/1000));
  }

  static int[] numStreamsList = {1000, 5000, 10000, 25000, 50000, 100000};
  static int[] numIterationsList = { 1000*1000, 1000*1000*10 };
  static int[] maxRecordsList = { 1, 2, 5, 10, 25, 50, 100, 500, 1000 };
}
