/*
 * Created on Feb 10, 2006 by apetcu
 * Project Public
 * Package frodo.communication
 */
package frodo.communication;

import java.io.FileWriter;
import java.util.Vector;

import frodo.utils.HelperUtilities;

public class CommunicationStatistics {
	public static final int MAX_MESSAGE_TYPES = 6;

	private int[][] stats;

	private int numberOfAgents;

	private int[] messageOfKind;

	private long longestTimeGap;

	private long baseTimeLine;

	private long timeReceived;//when was the last message received

	private Message slowestMessage;//this is the ID of the message that was sent after the longest delay

	private long messageCounter;//how many messages

	private Vector messages;//in case I have to log messages, I will save them here.

	private boolean logsMessages = false;//by default, I will not save messages

	public CommunicationStatistics(int agents) {
		numberOfAgents = agents;
		stats = new int[agents][MAX_MESSAGE_TYPES];
		messageOfKind = new int[MAX_MESSAGE_TYPES];
		messages = new Vector();
	}

	/*

	 public int getNumberOfAgents() {
	 return numberOfAgents;
	 }

	 public void setNumberOfAgents(int numberOfAgents) {
	 this.numberOfAgents = numberOfAgents;
	 }
	 public int[][] getStats() {
	 return stats;
	 }

	 public void setStats(int[][] stats) {
	 this.stats = stats;
	 }
	 */
	public int[] getStatistics(int sender) {
		return stats[sender];
	}

	//------------------------------------------------------------------------------------------------------------------//	
	public int getStatistics(int sender, int type) {
		return stats[sender][type];
	}

	//------------------------------------------------------------------------------------------------------------------//
	public synchronized boolean addMessageToStatistics(Message msg) {
		msg.setMessageId(messageCounter);
		long curTime = 0;
		if (messageCounter == 0) {
			baseTimeLine = System.currentTimeMillis();
			timeReceived = 0;
			curTime = 0;
		} else {
			curTime = System.currentTimeMillis() - baseTimeLine;
		}
		long timeGap = curTime - timeReceived;
		if (longestTimeGap < timeGap) {
			longestTimeGap = timeGap;
			slowestMessage = (Message) msg.clone();
			slowestMessage.setTimestamp(curTime);
		}
		timeReceived = curTime;
		if (logsMessages) {
			Message cloneMsg = (Message) msg.clone();
			cloneMsg.setTimestamp(curTime);
			msg.setTimestamp(curTime);
			messages.add(cloneMsg);
		}
		stats[msg.getSenderId()][msg.getType()]++;
		stats[msg.getSenderId()][0]++;
		messageOfKind[msg.getType()]++;
		messageCounter++;
		if (messageOfKind[msg.getType()] == 1)
			return true;
		else
			return false;
	}

	//------------------------------------------------------------------------------------------------------------------//
	public static void writeToFile(CommunicationStatistics stats, String file, long time) {
		try {
			FileWriter fw = new FileWriter(file);
			fw.write(stats.toString(time));
			fw.close();
		} catch (Exception e) {
			System.out.println("Exception while saving result to file:");
			e.printStackTrace();
			System.exit(1);
		}
	}

	//------------------------------------------------------------------------------------------------------------------//

	public long getMessageCounter() {
		return messageCounter;
	}

	/*
	 public Vector getMessages() {
	 return messages;
	 }
	 */
	public void setLogsMessages(boolean logsMessages) {
		this.logsMessages = logsMessages;
	}

	//------------------------------------------------------------------------------------------------------------------//
	public String toString(long time) {
		StringBuffer sb = new StringBuffer("");
		if (logsMessages) {
			sb.append(messages.size() + " messages:\n");
			for (int j = 0; j < messages.size(); j++) {
				Message curm = (Message) messages.elementAt(j);
				long gap = 0;
				if (j > 0)
					gap = curm.getTimestamp() - ((Message) messages.elementAt(j - 1)).getTimestamp();
				sb.append(j + "\t" + gap + "\t" + curm.toStringComplete() + "\n");
			}
		}
		sb.append("Agent\tTime\t");
		for (int j = 0; j < stats[0].length; j++)
			sb.append(j + "\t");
		sb.append("\n-------------------------------------------------------------------\n");
		int[] sum = new int[stats[0].length];
		for (int i = 0; i < stats.length; i++) {
			for (int j = 0; j < stats[0].length; j++) {
				sum[j] += stats[i][j];
			}
			sb.append("Agent" + i + "\t\t" + HelperUtilities.arrayToString(stats[i]) + "\n");
		}
		sb.append("\n-------------------------------------------------------------------\n");
		sb.append(numberOfAgents + "\t" + time + "\t" + HelperUtilities.arrayToString(sum) + "\n");
		sb.append("Longest time gap " + longestTimeGap + "ms before message " + slowestMessage.toStringComplete() + "\n");
		return sb.toString();
	}
	//------------------------------------------------------------------------------------------------------------------//
}
