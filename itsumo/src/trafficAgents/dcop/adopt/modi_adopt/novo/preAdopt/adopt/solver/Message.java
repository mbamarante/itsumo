/**
  * Super class of all message types that we might want to send
  */

package adopt.solver;

public class Message {

  public int destAgent = -1;
  public int sourceAgent = -1;
  public String message = null;

  public Message (int destAgent, int sourceAgent, String msg) {
    this.destAgent = destAgent;
    this.sourceAgent = sourceAgent;
    this.message = msg;
  }

  public String rawMessage() { return message; }

}
