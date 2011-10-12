/*
 * Created on Mar 6, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.MeetingScheduling;

import frodo.utils.HelperUtilities;

/**
 * @author apetcu
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Meeting {
	int participants[];

	int valuations[];

	int time;

	public int getTime() {
		return time;
	}

	public void setTime(int time) {
		this.time = time;
	}

	/**
	 * @param participants
	 * @param valuations
	 */
	public Meeting(int[] participants, int[] valuations) {
		super();
		this.participants = participants;
		this.valuations = valuations;
	}

	public int[] getParticipants() {
		return participants;
	}

	public void setParticipants(int[] participants) {
		this.participants = participants;
	}

	public int[] getValuations() {
		return valuations;
	}

	public void setValuations(int[] valuations) {
		this.valuations = valuations;
	}

	public boolean hasParticipant(int id) {
		if (HelperUtilities.findInArray(participants, id) >= 0)
			return true;
		else
			return false;
	}

	public int getUtility(int id) {
		int index = HelperUtilities.findInArray(participants, id);
		if (index >= 0)
			return valuations[index];
		else
			return -1;
	}

	public String toString() {
		//		return "[" +HelperUtilities.arrayToString(participants) + "]";
		return HelperUtilities.arrayToString(participants);
	}
}