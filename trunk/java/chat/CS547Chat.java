import java.io.*;
import java.net.*;
import java.util.*;

public class  CS547Chat
{
         String open;
         tag sender;
         tag receiver;
         tag time;
         tag message;
         String close;

         public CS547Chat (String sender ,String receiver ,
                           String time, String message, 
                           String op)
         {
           this.open = "<" + op + ">";
           this.sender = new tag (sender ,"sender");
           this.receiver = new tag (receiver ,"receiver");
           this.time = new tag (time ,"time");
           this.message = new tag (message ,"message");
           this.close = "</" + op + ">";
         }

         public CS547Chat(String op)
         {
        	 this.open = "<" + op + ">";
                 this.sender = new tag ("sender");
                 this.receiver = new tag ("receiver");
                 this.time = new tag ("time");
                 this.message = new tag ("message");
        	 this.close = "</" + op + ">";
         }   

         public static void main(String[] args)
         {
           CS547Chat chat = new CS547Chat ("Archana", "Sumant", "10:10",
                                           "Hi! I love you", 
                                           "CS-547-Chat");
           StringBuffer buf = new StringBuffer ();
           chat.write(buf);
           System.out.println (buf);
           String str = buf.toString ();
           CS547Chat read_chat = new CS547Chat ("CS-547-Chat");
           int i = read_chat.read (buf.toString(), 0);
           System.out.println (read_chat.getSender().getData());
           System.out.println (read_chat.getReceiver().getData());
           System.out.println (read_chat.getTime().getData());
           System.out.println (read_chat.getMessage().getData());
         }

         public void write (StringBuffer buf)
         {
           buf.append (this.open);
           sender.write (buf);
           receiver.write (buf);
           time.write (buf);
           message.write (buf);
           buf.append (this.close);
         }

         public int read(String s ,int beginIndex)
         {
              	 int i = s.indexOf(this.open , beginIndex);
        	 i += this.open.length();
        	 i = this.sender.read (s, i);
        	 i = this.receiver.read (s, i);
        	 i = this.time.read (s, i);
        	 i = this.message.read (s, i);
        	 int j = s.indexOf(this.close, i);
        	 return j + this.close.length();
         }
         
         public tag getSender ()
         {
           return this.sender;
         }
         
         public tag getReceiver ()
         {
           return this.receiver;
         }

         public tag getTime ()
         {
           return this.time;
         }
         
         public tag getMessage ()
         {
           return this.message;
         }

}
