//write server and have matching client
//pare down unneccessary stuff
//add needed things
//if odd 3A+1
//if even divide by 2
//send back number of steps to get to 0
//we don't need web logic
//all the logic goes into handleClient
//open input and output streams
//read number from input streams
//use the algorithms
//count the steps
//reply with number of steps

package web;

import java.io.*;
import java.net.*;
import java.util.*;

class Worker extends Thread implements HttpConstants {

   final static int BUF_SIZE = 2048;
   static final byte[] EOL = { (byte) '\r', (byte) '\n' };

   /* buffer to use for requests */
   byte[] buffer;
   /* Socket to client we're handling */
   private Socket socket;
   /* WebServer the worker works for */
   WebServer webServer;

   Worker(WebServer webServer, String name) {
      super(name);
      this.webServer = webServer;
      buffer = new byte[BUF_SIZE];
      this.start();
   }

   synchronized void setSocket(Socket socket) {
      this.socket = socket;
      notify();
   }

   public synchronized void run() {

      webServer.workerHasStarted();

      while (true) {
         /* nothing to do */
         try {
            wait();
         } catch (InterruptedException e) {
            /* should not happen */
            continue;
         }

         try {
            handleClient();
         } catch (Exception e) {
            e.printStackTrace();
         }
         /*
          * go back in wait queue if there's fewer than numHandler connections.
          */

         Vector pool = webServer.threads;
         synchronized (pool) {
            if (pool.size() >= webServer.workers) {
               /* too many threads, exit this one */
               return;
            } else {
               pool.addElement(this);
            }
         }
      }
   }

   void handleClient() throws IOException {
      // getting client in and outs
      InputStream fClient = new BufferedInputStream(socket.getInputStream());
      PrintStream tClient = new PrintStream(socket.getOutputStream());
      DataInputStream fromClient = new DataInputStream(fClient);
      DataOutputStream toClient = new DataOutputStream(tClient);

      int charFromClient = 0;
      System.out.print("Client connected.\n");
      // converting client input to an int
      charFromClient = fromClient.readInt();
      // call algorithm
      int algorithmSteps = ThreeAPlusOne(charFromClient);
      // write it back to client
      toClient.write(algorithmSteps);

   }

   /*
    * Takes an Integer input and applies the 3A+1 algorithm to reduce input to 1 &
    * return the steps it takes
    */
   int ThreeAPlusOne(int InputStream) {
      int inputInt = InputStream;
      int algortihmCounter = 0;

      while (inputInt != 1) {
         if (inputInt % 2 == 0) {
            inputInt = inputInt / 2;
         } else {
            inputInt = 3 * inputInt + 1;
         }
         algortihmCounter++;
      }

      return algortihmCounter ;
   }

}
