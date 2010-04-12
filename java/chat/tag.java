import java.io.*;
import java.net.*;
import java.util.*;

public class  tag
{
         String open;
         String data;
         String close;

         public tag (String s , String op)
         {
           this.open = "<" + op + ">";
           this.data = s;
           this.close = "</" + op + ">";
         }

         public tag(String op)
         {
        	 this.open = "<" + op + ">";
        	 this.close = "</" + op + ">";
         }   
/*
         public static void main(String[] args)
         {
           tag t1 = new tag ("Hi" ,"message");
           tag t5 = new tag ("Archana" ,"sender");
           tag t6 = new tag ("Sumant" ,"receiver");
           StringBuffer buf = new StringBuffer ();
           t1.write(buf);
           t5.write(buf);
           t6.write(buf);
           System.out.println (buf);
           String str = buf.toString ();
           tag t2 = new tag ("message");
           tag t3 = new tag ("sender");
           tag t4 = new tag ("receiver");
           int i = t2.read (buf.toString(), 0);
           i = t3.read (buf.toString(), i);
           i = t4.read (buf.toString(), i);
           System.out.println (t2.getData());
           System.out.println (t3.getData());
           System.out.println (t4.getData());
         }
*/
         public String getData ()
         {
           return this.data;
         }

         public void write (StringBuffer buf)
         {
           buf.append (this.open);
           buf.append (this.data);
           buf.append (this.close);
         }

         public int read(String s ,int beginIndex)
         {
              	 int i = s.indexOf(this.open , beginIndex);
        	 i += this.open.length();
        	 int j = s.indexOf(this.close, i);
        	 this.data = s.subSequence(i, j).toString();
                 int retval = j + this.close.length();
        	 return retval;        	
         }
}
