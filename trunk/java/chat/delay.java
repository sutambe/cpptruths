import java.io.*;
import java.util.*;

public class delay {

  public static void main (String [] args) {
    try {
      long startTime = System.currentTimeMillis();
      Thread.sleep (Integer.parseInt(args[0]));
      long currentTime = System.currentTimeMillis();

      System.out.println("Difference = " + (currentTime - startTime));
    } catch (InterruptedException ex) {
        ex.printStackTrace();
    }
  }

};
