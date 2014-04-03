import java.io.*;
import java.net.*;

class Client {
  public static void main(String argv[]) throws Exception {
    try {
      String message;
      Socket server = new Socket(argv[0], Integer.parseInt(argv[1]));
      System.out.println("Connected to Arduino Server.");
      DataOutputStream output = new DataOutputStream(server.getOutputStream());
      InputStreamReader input = new InputStreamReader(server.getInputStream());
      BufferedReader reader = new BufferedReader(input);
      output.writeBytes("Hello Arduino Server.\n");

      while ((message = reader.readLine()) != null) {
        System.out.println("Arduino Server: " + message);
        if (message.equals("Alert!")) {
          server.close();
          execute("truecrypt -t --list");
          execute("truecrypt -d");
          System.out.println("Volumes dismounted successfully.");
          execute("shutdown -h now");
          break;
        }
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  private static void execute(String command) throws IOException {
    try {
      String printline;
      java.lang.Process p = Runtime.getRuntime().exec(command);
      InputStreamReader input = new InputStreamReader(p.getInputStream());
      BufferedReader reader = new BufferedReader(input);
      while ((printline = reader.readLine()) != null) {
        System.out.println(printline);
      }
      reader.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
