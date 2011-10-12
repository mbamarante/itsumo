/*
 * Created on 17 mai 2005
 *
 */
package frodo.interaction.meeting.gui;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.tree.DefaultMutableTreeNode;

import frodo.interaction.meeting.TimeGrid;
import frodo.utils.MeetingUtils;


/**
 * @author gushakov
 *	Implements a jtree node for a participants of the meeting.
 *	This is intended to be a leaf node.
 */
public class ParticipantTreeNode extends DefaultMutableTreeNode implements LabeledTreeNode {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private String label = null;

	private HashMap schedule = null;

	public static int DEFAULT_SCHEDULING_PREFERENCE = 0;

	public ParticipantTreeNode(String label, TimeGrid grid) {
		super();
		this.label = label;
		if (schedule == null) {
			schedule = new HashMap();
			initializePreferenceValues(grid);
		} else {
			combineTimeGrids(grid);
		}
	}

	/*
	 * Initializes the sheduling preferences for this participant.
	 * Note, the time grid should correspond to the one declared for
	 * the parent meeting. 
	 */
	private void initializePreferenceValues(TimeGrid grid) {

		for (int i = 0; i < grid.getNumberOfTimeSlots(); i++) {
			schedule.put(grid.getSlotAt(i), new Integer(DEFAULT_SCHEDULING_PREFERENCE));
		}
	}

	public static ParticipantTreeNode fromString(String line, TimeGrid timeGrid) {
		String[] parsedLine = line.trim().split("\\s+|:");
		String label = parsedLine[0].trim();
		ParticipantTreeNode participant = new ParticipantTreeNode(label, timeGrid);
		for (int i = 1; i < parsedLine.length - 1; i += 2) {
			Integer timeSlot = new Integer(parsedLine[i]);
			Integer preference = new Integer(parsedLine[i + 1]);
			participant.setPreferenceForTimeSlot(timeSlot, preference);
		}
		return participant;
	}

	/**
	 * Combines the two time grids into one for this
	 * particiapant. Each meeting this participant
	 * is scheduled to attend can have it's own time grid.
	 * @see MeetingTreeNode
	 */
	private void combineTimeGrids(TimeGrid tg) {
		for (int i = 0; i < tg.getNumberOfTimeSlots(); i++) {
			Comparable key = tg.getSlotAt(i);
			if (!schedule.containsKey(key)) {
				schedule.put(key, new Integer(DEFAULT_SCHEDULING_PREFERENCE));
			}
		}
	}

	/**
	 * If this participant has previous scheduled meetings,
	 * set corresponding scheduling preferences for the 
	 * corresponding time slots.
	 * @param anotherSchedule
	 * 				A scheduling preference of a previously defined meeting
	 * 				with this participant.
	 * @see MeetingExplore -- addition of exsisting participant.
	 */
	public void setSchedule(HashMap anotherSchedule) {
		Iterator keys = schedule.keySet().iterator();
		while (keys.hasNext()) {
			Object key = keys.next();
			anotherSchedule.put(key, schedule.get(key));
		}
		schedule = anotherSchedule;
	}

	public boolean getAllowsChildren() {
		return false;
	}

	public void setAllowsChildren(boolean allows) {
		// skip
	}

	public boolean isLeaf() {
		return true;
	}

	public String toString() {
		return label;
	}

	public String getLabel() {
		return label;
	}

	public void setLabel(String label) {
		this.label = label;
	}

	public HashMap getSchedule() {
		return schedule;
	}

	/**
	 * Returns a row of participant's schedule conforming
	 * to a specified time grid of a metting.
	 * @param tg time greed of a meeting
	 */
	public Vector getLabelSchedule(TimeGrid tg) {
		Vector v = new Vector();
		v.add(label);
		for (int i = 0; i < tg.getNumberOfTimeSlots(); i++) {
			v.add(schedule.get(tg.getSlotAt(i)));
		}
		return v;
	}

	public void setPreferenceForTimeSlot(Object key, Object value) {
		if (schedule.containsKey(key)) {
			schedule.remove(key);
		}
		schedule.put(key, value);
	}

	public Object getPreferenceForTimeSlot(Object timeSlot) {
		return schedule.get(timeSlot);
	}

	public Vector getScheduleTimeSlots() {
		Vector slots = new Vector();
		Iterator iter = schedule.keySet().iterator();
		while (iter.hasNext()) {
			Integer key = (Integer) iter.next();
			slots.add(key);
		}
		MeetingUtils.sort(slots);
		return slots;
	}

	public String toStringMeetingView(TimeGrid timeGrid) {
		String out = "\t";
		Vector v = getLabelSchedule(timeGrid);
		out += v.elementAt(0) + "\t";
		for (int i = 1; i < v.size(); i++) {
			Object timeSlot = timeGrid.getSlotAt(i - 1);
			Object value = getSchedule().get(timeSlot);
			out += timeSlot + ":" + value + (i < v.size() - 1 ? "\t" : "");
		}
		return out;
	}
}
