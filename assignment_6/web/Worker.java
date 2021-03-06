
package web;
import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.*;

class Worker extends Thread implements HttpConstants {
    public enum Operators{ADD, SUBTRACT, MULTIPLY, DIVIDE, RAISE, SQRT, INCORRECT_OP};

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
      DataInputStream fromClient = new DataInputStream(socket.getInputStream());
      DataOutputStream toClient = new DataOutputStream(socket.getOutputStream());

      int firstNumber = 0;
      int secondNumber = 0;
      int operatorFlag = 0;
      int counter = 0;
      System.out.print("Client connected.\n");
      // converting client input to an correct values
      firstNumber = fromClient.readInt();
      secondNumber = fromClient.readInt();
      operatorFlag = fromClient.readInt();

      System.out.printf("firstNum: %d, secondNum: %d, operatorFlag: %d\n", firstNumber, secondNumber, operatorFlag);

      // call algorithm
      double result = doMath(firstNumber, operatorFlag, secondNumber);

      System.out.printf("Result: %2.3f\n", result);

      // write it back to client
      toClient.writeDouble(result);
      System.out.printf("Completed client\n");
   }

   double doMath(int firstNumber, int operatorFlag, int secondNumber)
   {
      double result;
      if (operatorFlag == 0)
      {
         result = firstNumber + secondNumber;
      }
      else if(operatorFlag == 1)
      {
         result = firstNumber - secondNumber;
      }
      else if(operatorFlag == 2 )
      {
         result = firstNumber * secondNumber;
      }
      else if(operatorFlag == 3)
      {
         result = firstNumber / (double) secondNumber;
      }
      else if(operatorFlag == 4)
      {
         result = Math.pow(firstNumber, secondNumber);
      }
      else
      {
         result = Math.sqrt(firstNumber);
      }

      return result;
   }

}
