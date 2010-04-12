import java.io.*;
import java.net.*;
import java.util.*;

public class  XMLStringParser
{
	
	public static void main(String[] args) 
	{
		String xmlString = new String();
		
	    BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	    System.out.println("Please enter String:");
	    try {
		      xmlString = br.readLine();
		      System.out.println("<message>"+ xmlString + "</message>");
	        }catch(IOException ie){System.out.println("Input error" + ie );}  
	        
	        stringParser(xmlString);
	        
        		
	}
	
	public static void stringParser(String message){
		
		StringTokenizer st = new StringTokenizer(message,"<,>,/");
		int numOfValues = st.countTokens();
		String[] values = new String[numOfValues];
		for (int index = 0; index < numOfValues; index++) {
			String stringValue = st.nextToken();
			values[index] = stringValue;
		}
         
		String tag = values[0];
		System.out.println(tag);
	}
	
}

