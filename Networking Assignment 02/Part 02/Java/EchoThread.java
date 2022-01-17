package echo_thread_package;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

class EchoThread
  {
    public static void main(String[] args)
      {
        // hard coded port number on an unused port
        int portNumber = 23657;

        // try opening a ServerSocket in a try-with-resources
        try (ServerSocket server = new ServerSocket(portNumber);)
          {
            // keep looping to accept new clients constantly
            while (true)
              {
                // accept a client socket
                Socket client = server.accept();
                System.out.println("New Connection");

                // create an EchoThread to start handling the client socket
                EchoThreaded handler = new EchoThreaded(client);

                // start running the EchoThread to deal with the Socket so the
                // loop can keep accepting
                new Thread(handler).start();
              }
          } catch (IOException err)
          {
            err.printStackTrace();
          }
      }

  }

class EchoThreaded implements Runnable
{
  // class variables
  private Socket client;
  
  /**
   * EchoThread initializes thread object with given socket connection.
   * 
   * @param client - socket connection to be handled by thread.
   */
  
  public EchoThreaded(Socket client)
    {
      this.client = client;
    }
  
  /**
   * Run function opens up data IO steams with client then listens. Reads
   * characters from client one at a time. Processes each character as
   * follows: - ignores characters that are not letters. - looks for the
   * sequence "quit", terminates connection if found. - all characters
   * that are letters are echoed back to the client.
   */
  
  @Override
  public synchronized void run()
    {
      DataInputStream fromClient = null;
      DataOutputStream toClient = null;
      
      int charFromClient = 0;
      boolean keepGoing = true;
      
      // show that we are connected to client
      System.out.println("Client connected ...");
      
      // first get the streams
      try
        {
          fromClient = new DataInputStream(client.getInputStream());
          toClient = new DataOutputStream(client.getOutputStream());
        }
      
      catch (IOException e)
        {
          System.err.println("Error opening network streams");
          return;
        }
      
      // now talk to the client

      int demo = 0;

      while (keepGoing)
        {
          try
            {
              charFromClient = fromClient.readByte();
              
              System.out.print((char)charFromClient);

              demo += (char)charFromClient;
              // System.out.println((char)demo);
              System.out.println("..." + demo);

              
            }

          
          catch (IOException e)
            {
              System.err.println("Error reading character from client");
              return;
            }
          
          try
            {
              toClient.writeByte(charFromClient);
            }
          
          catch (IOException e)
            {
              System.err.println("Error writing character to client");
              return;
            }
          
          if (charFromClient == 'q')
            {
              System.out.println("\nBailing out!");
              keepGoing = false;
            }
        }
        
    
        
      try
        {
          client.close();
        }
      
      catch (IOException e)
        {
          System.err.println("Error closing socket to client");
        }
    }
              
}