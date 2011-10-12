/*
 * Created on 25 mai 2005
 *
 */
package frodo.interaction.meeting.gui;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Enumeration;
import java.util.Vector;

import javax.swing.tree.DefaultMutableTreeNode;

import frodo.interaction.meeting.TimeGrid;

/**
 * @author gushakov
 *This is the root of the meeting tree; it holds all the participations
 */
public class RootTreeNode extends DefaultMutableTreeNode implements LabeledTreeNode {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public static int DEFAULT_NEGATIVE_UTILITY = -10000;

	public static int DEFAULT_POSITIVE_UTILITY = 0;

	private String label = "Meetings (Root)";

	public RootTreeNode() {
		super();
	}

	public String getLabel() {
		return label;
	}

	public String toString() {
		return label;
	}

	public boolean isRoot() {
		return true;
	}

	/**
	 * Constructs the common time grid of all the children meeting nodes.
	 * @return TimeGrid spanning from the starting time of the
	 * 			earliest scheduled meeting to the finish time of the
	 * 			latest scheduled meeting.
	 * @see MeetingExplorer#makeSolutionModel()
	 */
	public TimeGrid getCommonGridAll() {
		TimeGrid common = null;
		Enumeration myEnum = children();
		if (myEnum.hasMoreElements()) {
			//Get the starting time grid, arbitrarily it can be the grid of the
			// first child meeting.
			TimeGrid start = ((MeetingTreeNode) myEnum.nextElement()).getGrid();
			common = start;
			while (myEnum.hasMoreElements()) {
				MeetingTreeNode meeting = (MeetingTreeNode) myEnum.nextElement();
				common = TimeGrid.getCombinedGrid(common, meeting.getGrid());
			}
		}
		return common;
	}

	/**
	 * Returns the common grid of selected meetings.
	 * @param selected Vector of MeetingTreeNodes
	 * @return common time grid
	 */
	public static TimeGrid getCommonGrid(Vector selected) {
		TimeGrid common = null;
		TimeGrid start = ((MeetingTreeNode) selected.firstElement()).getGrid();
		common = start;
		for (int i = 0; i < selected.size(); i++) {
			MeetingTreeNode meeting = (MeetingTreeNode) selected.elementAt(i);
			common = TimeGrid.getCombinedGrid(common, meeting.getGrid());
		}
		return common;
	}

	public boolean canAddMeeting(String meetingLabel) {
		boolean flag = true;
		Enumeration myEnum = children();
		while (flag && myEnum.hasMoreElements()) {
			if (((MeetingTreeNode) myEnum.nextElement()).getLabel().equals(meetingLabel)) {
				flag = false;
			}
		}
		return flag;
	}

	/**
	 * Returns an existing participant from the leaves
	 * of the tree rooted at this root tree node.
	 * 
	 * @param name
	 *            of the participant
	 * @return tree node of the participant or null if no participant was found
	 */
	public ParticipantTreeNode getParticipantByName(String name) {
		Enumeration nodes = breadthFirstEnumeration();

		ParticipantTreeNode p = null;
		boolean found = false;
		while (nodes.hasMoreElements() && !found) {
			DefaultMutableTreeNode nextNode = (DefaultMutableTreeNode) nodes.nextElement();
			if (nextNode instanceof ParticipantTreeNode && ((ParticipantTreeNode) nextNode).getLabel().equals(name)) {
				p = (ParticipantTreeNode) nextNode;
				found = true;
			}
		}
		return p;
	}

	/**
	 * Returns a meeting with a specified label.
	 * @param label
	 * @return
	 */
	public MeetingTreeNode getMeetingByLabel(String label) {
		MeetingTreeNode meeting = null;
		Enumeration myEnum = children();
		boolean found = false;
		while (myEnum.hasMoreElements() && !found) {
			MeetingTreeNode nextMeeting = (MeetingTreeNode) myEnum.nextElement();
			if (nextMeeting.getLabel().equalsIgnoreCase(label)) {
				meeting = nextMeeting;
				found = true;
			}
		}
		return meeting;
	}

	/**
	 * Makes a root tree node from a file.
	 */
	public static RootTreeNode fromFile(String fileName) {
		RootTreeNode root = new RootTreeNode();
		root.label = "File: " + fileName;
		try {
			BufferedReader reader = new BufferedReader(new FileReader(fileName));
			String line = reader.readLine();
			int numOfMeetings = Integer.parseInt(line.trim());
			for (int i = 0; i < numOfMeetings; i++) {
				if (!line.startsWith("#")) {
					MeetingTreeNode meeting = MeetingTreeNode.fromFile(reader);
					if (root.canAddMeeting(meeting.getLabel())) {
						root.add(meeting);
						// Set the schedules of the participants of the meeting
						Enumeration myEnum = meeting.children();
						while (myEnum.hasMoreElements()) {
							ParticipantTreeNode participant = (ParticipantTreeNode) myEnum.nextElement();
							ParticipantTreeNode existingParticipant = root.getParticipantByName(participant.getLabel());
							if (existingParticipant != null) {
								participant.setSchedule(existingParticipant.getSchedule());
							}
						}
					} else {
						System.err.println("error: can't add meeting " + meeting.getLabel());
					}
				}
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
		return root;
	}

	public String toStringSerialized() {
		String out = "";
		int numOfMeetings = getChildCount();
		out += numOfMeetings + "\n";
		for (int i = 0; i < numOfMeetings; i++) {
			MeetingTreeNode meeting = (MeetingTreeNode) getChildAt(i);
			out += meeting.toStringSerialized() + (i < numOfMeetings - 1 ? "\n" : "");
		}
		return out;
	}

	public void writeToFile(String fileName) {
		try {
			FileWriter fw = new FileWriter(new File(fileName));
			fw.write(toStringSerialized());
			fw.flush();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public int getParticipationsCount() {
		Enumeration myEnum = breadthFirstEnumeration();
		int count = 0;
		while (myEnum.hasMoreElements()) {
			if (myEnum.nextElement() instanceof ParticipantTreeNode) {
				count++;
			}
		}
		return count;
	}

}
