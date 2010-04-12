import java.io.*;
import java.net.*;
import java.util.*;

public class  Test
{
        String open;
        String data;
        String close;
        Test (String s)
        {
          this.open = new String ("<Test>");
          this.data = s;
          this.close = new String ("</Test>");
        }

	public static void main(String[] args) 
	{
          Test t = new Test ("Sumant");
          StringBuffer buf = new StringBuffer ();
          t.write (buf);
          System.out.println (buf);
        }
        
        public void write (StringBuffer buf)
        {
          buf.append (this.open);
          buf.append (this.data);
          buf.append (this.close);
        }
}
