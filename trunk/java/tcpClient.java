// tcpClient.java by fpont 3/2000

// usage : java tcpClient <server> <port>
// default port is 1500

import java.net.*;
import java.io.*;

public class tcpClient {
    
    
    
    public static void main(String[] args) {

	int port = 1500;
	String server = "localhost";
	Socket socket = null;
	String lineToBeSent;
	BufferedReader input;
	PrintWriter output;
	int ERROR = 1;
	
	// read arguments 
	if(args.length == 2) {
	    server = args[0];
	    try { 
		port = Integer.parseInt(args[1]);
	    }
	    catch (Exception e) {
		System.out.println("server port = 1500 (default)");
		port = 1500;
	    }
	} 
	
	
	
	// connect to server
	try {
	    socket = new Socket(server, port);
	    System.out.println("Connected with server " +
				   socket.getInetAddress() +
				   ":" + socket.getPort());
	}
	catch (UnknownHostException e) {
	    System.out.println(e);
	    System.exit(ERROR);
	}
	catch (IOException e) {
	    System.out.println(e);
	    System.exit(ERROR);
	}


	
	try {
	    input = new BufferedReader(new InputStreamReader(System.in)); 
	    output = new PrintWriter(socket.getOutputStream(),true);
	    
	    // get user input and transmit it to server
	    while(true) {
		lineToBeSent = input.readLine();
		// stop if input line is "."
		if(lineToBeSent.equals(".")) break;
		output.println(lineToBeSent);
	    }
	}
	catch (IOException e) {
	    System.out.println(e);
	}

	try {
	    socket.close();
	}
	catch (IOException e) {
	    System.out.println(e);
	}
    }    
}

