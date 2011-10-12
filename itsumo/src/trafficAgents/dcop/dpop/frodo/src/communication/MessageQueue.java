/*
 * MessageQueue.java
 *
 * Created on November 4, 2002, 11:00 AM
 */

package frodo.communication;

import java.util.LinkedList;


public class MessageQueue {
	private volatile int fCounter = 0;
	private String fName = "default_name";
	private LinkedList fQueue = new LinkedList();

	public MessageQueue() {
	}

	public MessageQueue(String name) {
		fName = name;
	}

	public int getMessageCounter() {
		return fCounter;
	}

	public String getName() {
		return fName;
	}

	synchronized public void addMessage(Message m) {
		fQueue.offer(m);
		fCounter++;
		notify();
	}

	synchronized public Message getMessageBlocking() {
		while (fQueue.isEmpty()) {
			try { wait();} catch (InterruptedException e) {	e.printStackTrace(); }
		}
		return (Message) fQueue.poll();
	}

	synchronized public String toString() {
		return fQueue.toString();
	}
}
