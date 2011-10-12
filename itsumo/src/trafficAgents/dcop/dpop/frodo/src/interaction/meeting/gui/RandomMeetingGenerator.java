/*
 * Created on 27 juin 2005
 *
 */
package frodo.interaction.meeting.gui;

import java.awt.FlowLayout;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Random;
import java.util.Stack;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import frodo.interaction.meeting.TimeGrid;


/**
 * @author George
 *  
 */
public class RandomMeetingGenerator extends JDialog {

	private javax.swing.JPanel jContentPane = null;

	private JPanel jPanel = null;

	private JPanel jPanel1 = null;

	private JPanel jPanel2 = null;

	private JPanel jPanel3 = null;

	private JPanel jPanel4 = null;

	private JPanel jPanel5 = null;

	private JPanel jPanel6 = null;

	private JPanel jPanel7 = null;

	private JPanel jPanel8 = null;

	private JPanel jPanel9 = null;

	private JLabel jLabel = null;

	private JTextField jtxNumOfMeetings = null;

	private JPanel jPanel10 = null;

	private JPanel jPanel11 = null;

	private JLabel jLabel1 = null;

	private JTextField jtxMinRange = null;

	private JTextField jtxMaxRange = null;

	private JCheckBox jchbSameForAll = null;

	private JLabel jLabel3 = null;

	private JTextField jtxNumOfParticipants = null;

	private JLabel jLabel5 = null;

	private JLabel jLabel2 = null;

	private JPanel jPanel12 = null;

	private JPanel jPanel13 = null;

	private JLabel jLabel4 = null;

	private JLabel jLabel6 = null;

	private JTextField jtxMinParticipations = null;

	private JLabel jLabel7 = null;

	private JTextField jtxMaxParticipations = null;

	private JCheckBox jchbFixedForAll = null;

	private JLabel jLabel8 = null;

	private JTextField jtxSaveFileName = null;

	private JButton jButton = null;

	private JButton jButton1 = null;

	// ----------------------------------------------------
	private int numOfMeetings;

	private int minRange;

	private int maxRange;

	private boolean sameForAll;

	private int numOfParticipants;

	private int minParticipations;

	private int maxParticipations;

	private boolean fixedForAll;

	private HashMap[] meetings = null;

	private Random rnd = new Random(System.currentTimeMillis());

	private int TOTAL = 0;

	static final int TRIES = 100;

	private RootTreeNode treeRoot = null;

	private JDialog frame = null;

	/**
	 * This is the default constructor
	 */
	public RandomMeetingGenerator() {
		super();
		initialize();
		frame = this;
		this.setModal(true);
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(423, 287);
		this.setContentPane(getJContentPane());
		this.setTitle("Random Meeting Generation.");
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new javax.swing.JPanel();
			jContentPane.setLayout(new BoxLayout(jContentPane, BoxLayout.Y_AXIS));
			jContentPane.add(getJPanel(), null);
			jContentPane.add(getJPanel3(), null);
		}
		return jContentPane;
	}

	/**
	 * This method initializes jPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel() {
		if (jPanel == null) {
			jPanel = new JPanel();
			jPanel.setLayout(new BoxLayout(jPanel, BoxLayout.X_AXIS));
			jPanel.setPreferredSize(new java.awt.Dimension(10, 10));
			jPanel.add(getJPanel2(), null);
			jPanel.add(getJPanel1(), null);
		}
		return jPanel;
	}

	/**
	 * This method initializes jPanel1
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel1() {
		if (jPanel1 == null) {
			jPanel1 = new JPanel();
			jPanel1.setLayout(new BoxLayout(jPanel1, BoxLayout.Y_AXIS));
			jPanel1.setPreferredSize(new java.awt.Dimension(10, 50));
			jPanel1.add(getJPanel7(), null);
			jPanel1.add(getJPanel8(), null);
			jPanel1.add(getJPanel9(), null);
		}
		return jPanel1;
	}

	/**
	 * This method initializes jPanel2
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel2() {
		if (jPanel2 == null) {
			jPanel2 = new JPanel();
			jPanel2.setLayout(new BoxLayout(jPanel2, BoxLayout.Y_AXIS));
			jPanel2.setPreferredSize(new java.awt.Dimension(50, 50));
			jPanel2.add(getJPanel4(), null);
			jPanel2.add(getJPanel5(), null);
			jPanel2.add(getJPanel6(), null);
		}
		return jPanel2;
	}

	/**
	 * This method initializes jPanel3
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel3() {
		if (jPanel3 == null) {
			jLabel8 = new JLabel();
			jPanel3 = new JPanel();
			jPanel3.setLayout(new FlowLayout());
			jLabel8.setText("save to file tests/:");
			jPanel3.add(jLabel8, null);
			jPanel3.add(getJtxSaveFileName(), null);
			jPanel3.add(getJButton(), null);
			jPanel3.add(getJButton1(), null);
		}
		return jPanel3;
	}

	/**
	 * This method initializes jPanel4
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel4() {
		if (jPanel4 == null) {
			jLabel = new JLabel();
			jPanel4 = new JPanel();
			jLabel.setText("number of meetings:");
			jPanel4.add(jLabel, null);
			jPanel4.add(getJtxNumOfMeetings(), null);
		}
		return jPanel4;
	}

	/**
	 * This method initializes jPanel5
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel5() {
		if (jPanel5 == null) {
			jPanel5 = new JPanel();
			jPanel5.setLayout(new BoxLayout(jPanel5, BoxLayout.Y_AXIS));
			jPanel5.add(getJPanel10(), null);
			jPanel5.add(getJPanel11(), null);
		}
		return jPanel5;
	}

	/**
	 * This method initializes jPanel6
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel6() {
		if (jPanel6 == null) {
			jPanel6 = new JPanel();
			jPanel6.add(getJchbSameForAll(), null);
		}
		return jPanel6;
	}

	/**
	 * This method initializes jPanel7
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel7() {
		if (jPanel7 == null) {
			jLabel3 = new JLabel();
			jPanel7 = new JPanel();
			jLabel3.setText("number of participants:");
			jPanel7.add(jLabel3, null);
			jPanel7.add(getJtxNumOfParticipants(), null);
		}
		return jPanel7;
	}

	/**
	 * This method initializes jPanel8
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel8() {
		if (jPanel8 == null) {
			jPanel8 = new JPanel();
			jPanel8.setLayout(new BoxLayout(jPanel8, BoxLayout.Y_AXIS));
			jPanel8.add(getJPanel12(), null);
			jPanel8.add(getJPanel13(), null);
		}
		return jPanel8;
	}

	/**
	 * This method initializes jPanel9
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel9() {
		if (jPanel9 == null) {
			jPanel9 = new JPanel();
			jPanel9.add(getJchbFixedForAll(), null);
		}
		return jPanel9;
	}

	/**
	 * This method initializes jtxNumOfMeetings
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJtxNumOfMeetings() {
		if (jtxNumOfMeetings == null) {
			jtxNumOfMeetings = new JTextField();
			jtxNumOfMeetings.setPreferredSize(new java.awt.Dimension(30, 20));
			jtxNumOfMeetings.setText("2");
		}
		return jtxNumOfMeetings;
	}

	/**
	 * This method initializes jPanel10
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel10() {
		if (jPanel10 == null) {
			jLabel1 = new JLabel();
			jPanel10 = new JPanel();
			jLabel1.setText("schedule range of a meeting:");
			jPanel10.add(jLabel1, null);
		}
		return jPanel10;
	}

	/**
	 * This method initializes jPanel11
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel11() {
		if (jPanel11 == null) {
			jLabel2 = new JLabel();
			jLabel5 = new JLabel();
			jPanel11 = new JPanel();
			jLabel5.setText("min:");
			jLabel2.setText("max:");
			jPanel11.add(jLabel5, null);
			jPanel11.add(getJtxMinRange(), null);
			jPanel11.add(jLabel2, null);
			jPanel11.add(getJtxMaxRange(), null);
		}
		return jPanel11;
	}

	/**
	 * This method initializes jtxMinRange
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJtxMinRange() {
		if (jtxMinRange == null) {
			jtxMinRange = new JTextField();
			jtxMinRange.setPreferredSize(new java.awt.Dimension(30, 20));
			jtxMinRange.setText("1");
		}
		return jtxMinRange;
	}

	/**
	 * This method initializes jtxMaxRange
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJtxMaxRange() {
		if (jtxMaxRange == null) {
			jtxMaxRange = new JTextField();
			jtxMaxRange.setPreferredSize(new java.awt.Dimension(30, 20));
			jtxMaxRange.setText("8");
		}
		return jtxMaxRange;
	}

	/**
	 * This method initializes jchbSameForAll
	 * 
	 * @return javax.swing.JCheckBox
	 */
	private JCheckBox getJchbSameForAll() {
		if (jchbSameForAll == null) {
			jchbSameForAll = new JCheckBox();
			jchbSameForAll.setText("same for all");
			jchbSameForAll.setSelected(true);
		}
		return jchbSameForAll;
	}

	/**
	 * This method initializes jtxNumOfParticipants
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJtxNumOfParticipants() {
		if (jtxNumOfParticipants == null) {
			jtxNumOfParticipants = new JTextField();
			jtxNumOfParticipants.setPreferredSize(new java.awt.Dimension(30, 20));
			jtxNumOfParticipants.setText("3");
		}
		return jtxNumOfParticipants;
	}

	/**
	 * This method initializes jPanel12
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel12() {
		if (jPanel12 == null) {
			jLabel4 = new JLabel();
			jPanel12 = new JPanel();
			jLabel4.setText("number of participations:");
			jPanel12.add(jLabel4, null);
		}
		return jPanel12;
	}

	/**
	 * This method initializes jPanel13
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel13() {
		if (jPanel13 == null) {
			jLabel7 = new JLabel();
			jLabel6 = new JLabel();
			jPanel13 = new JPanel();
			jLabel6.setText("min:");
			jLabel7.setText("max:");
			jPanel13.add(jLabel6, null);
			jPanel13.add(getJtxMinparticipations(), null);
			jPanel13.add(jLabel7, null);
			jPanel13.add(getJtxMaxParticipations(), null);
		}
		return jPanel13;
	}

	/**
	 * This method initializes jtxMinParticipations
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJtxMinparticipations() {
		if (jtxMinParticipations == null) {
			jtxMinParticipations = new JTextField();
			jtxMinParticipations.setPreferredSize(new java.awt.Dimension(30, 20));
			jtxMinParticipations.setText("1");
		}
		return jtxMinParticipations;
	}

	/**
	 * This method initializes jtxMaxParticipations
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJtxMaxParticipations() {
		if (jtxMaxParticipations == null) {
			jtxMaxParticipations = new JTextField();
			jtxMaxParticipations.setPreferredSize(new java.awt.Dimension(30, 20));
			jtxMaxParticipations.setText("2");
		}
		return jtxMaxParticipations;
	}

	/**
	 * This method initializes jchbFixedForAll
	 * 
	 * @return javax.swing.JCheckBox
	 */
	private JCheckBox getJchbFixedForAll() {
		if (jchbFixedForAll == null) {
			jchbFixedForAll = new JCheckBox();
			jchbFixedForAll.setText("fixed for all");
		}
		return jchbFixedForAll;
	}

	/**
	 * This method initializes jtxSaveFileName
	 * 
	 * @return javax.swing.JTextField
	 */
	private JTextField getJtxSaveFileName() {
		if (jtxSaveFileName == null) {
			jtxSaveFileName = new JTextField();
			jtxSaveFileName.setPreferredSize(new java.awt.Dimension(100, 20));
			jtxSaveFileName.setText("random.mt");
		}
		return jtxSaveFileName;
	}

	/**
	 * This method initializes jButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJButton() {
		if (jButton == null) {
			jButton = new JButton();
			jButton.setText("Ok");
			jButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("attempting to generate a  random meeting tree...");
					int t = 0;
					boolean isTree = false;
					do {
						t++;
						getUserInput();
						fillMeetings();
						isTree = checkDFSTree();
					} while (t < TRIES && !isTree);
					if (isTree) {
						System.out.println("generated the random tree...");
						treeRoot = makeMeetingTree();
						writeTreeToFile();
					} else {
						System.out.println("encountered some problems while generating a random " + "\n tree with given parameters, tree written to file for review, \n" + "please, try again...");
						//treeRoot = makeMeetingTree();
						writeTreeToFile();
					}
					frame.setVisible(false);
					frame.dispose();
				}
			});
		}
		return jButton;
	}

	/**
	 * This method initializes jButton1
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJButton1() {
		if (jButton1 == null) {
			jButton1 = new JButton();
			jButton1.setText("Cancel");
			jButton1.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					frame.setVisible(false);
					frame.dispose();
				}
			});
		}
		return jButton1;
	}

	//---------------------------------------------------------------------------

	private void getUserInput() {
		try {
			numOfMeetings = Integer.parseInt(jtxNumOfMeetings.getText());
			minRange = Integer.parseInt(jtxMinRange.getText());
			maxRange = Integer.parseInt(jtxMaxRange.getText());
			sameForAll = jchbSameForAll.isSelected();
			numOfParticipants = Integer.parseInt(jtxNumOfParticipants.getText());
			minParticipations = Integer.parseInt(jtxMinParticipations.getText());
			maxParticipations = Integer.parseInt(jtxMaxParticipations.getText());
			fixedForAll = jchbFixedForAll.isSelected();

			meetings = new HashMap[numOfMeetings];

		} catch (Exception e) {
			e.printStackTrace();
			// TODO: check for number format exception.
		}
	}

	private void fillMeetings() {
		for (int i = 0; i < meetings.length; i++) {
			meetings[i] = new HashMap();
		}

		for (int i = 0; i < numOfParticipants; i++) {

			// All participants have the same, minimum number of
			// participations.
			for (int j = 0; j < minParticipations; j++) {
				placeParticipant(i);
			}
		}

		if (!fixedForAll && minParticipations < maxParticipations) {
			// In addition each participant might have random number more
			// participations.

			for (int i = 0; i < numOfParticipants; i++) {
				int howMany = rnd.nextInt(maxParticipations - minParticipations + 1);
				//System.out.println("how many random participations: " + howMany);
				for (int k = 0; k < howMany; k++) {
					placeParticipant(i);
				}
			}
		}

	}

	private void placeParticipant(int p) {
		boolean placed = false;
		while (!placed) {
			int whichMeeting = rnd.nextInt(numOfMeetings);
			if (!meetings[whichMeeting].containsKey(new Integer(p))) {
				// No participation of participant in this meeting.
				meetings[whichMeeting].put(new Integer(p), new Node(whichMeeting, p));
				placed = true;
			}
		}
	}

	class Node {
		int index;

		int meeting;

		int participant;

		char label;

		Vector neighbors;

		public Node(int m, int p) {
			TOTAL++;
			index = TOTAL;
			meeting = m;
			participant = p;
			label = 'N';
			neighbors = new Vector();
		}

		public boolean equals(Object obj) {
			Node node = (Node) obj;
			return index == node.index;
		}

		public String toString() {

			return "n:" + index + label;
		}
	}

	private void makeNeighbors(Node node) {
		// A node is connected to all of the nodes
		// with the same meeting.
		Iterator iter = meetings[node.meeting].values().iterator();
		while (iter.hasNext()) {
			Node n = (Node) iter.next();
			if (!node.equals(n)) {
				node.neighbors.add(n);
			}
		}

		// A node is also connected to all the nodes
		// which denote the participation of the same participant
		// in another meeting.
		for (int i = 0; i < meetings.length; i++) {
			if (node.meeting != i) {
				iter = meetings[i].values().iterator();
				boolean found = false;
				while (iter.hasNext() && !found) {
					Node n = (Node) iter.next();
					if (node.participant == n.participant) {
						node.neighbors.add(n);
						found = true;
					}
				}
			}
		}
	}

	private boolean checkDFSTree() {

		//All meetings should have at least two participations.
		boolean okFlag = true;
		int j = 0;
		while (j < meetings.length && okFlag) {
			if (meetings[j].size() < 2) {
				okFlag = false;
			}
			j++;
		}

		if (okFlag) {

			int numOfVisited = 0;

			Stack stack = new Stack();
			// Arbitrarily chose the first node for the root of the DFS tree.
			Node root = (Node) meetings[0].values().iterator().next();
			stack.push(root);
			while (!stack.isEmpty()) {

				//System.out.println(stack.toString());

				Node node = (Node) stack.pop();

				if (node.label == 'N') {
					//Mark the node as visited.
					node.label = 'V';
					numOfVisited++;
					//System.out.println("num of visited: " + numOfVisited);
				}
				makeNeighbors(node);
				for (int i = 0; i < node.neighbors.size(); i++) {
					Node neighbor = (Node) node.neighbors.elementAt(i);
					if (neighbor.label == 'N') {
						stack.push(neighbor);
					}
				}
			}
			//If we have a tree (and not a forest) the number of visited nodes
			//should be equal to the total number of nodes.
			okFlag = numOfVisited == TOTAL;
		}

		return okFlag;
	}

	private RootTreeNode makeMeetingTree() {
		RootTreeNode root = new RootTreeNode();

		for (int i = 0; i < meetings.length; i++) {
			//Make the time grid for the meeting.
			TimeGrid grid = makeTimeGrid();
			//Meeting label: m_#meeting.
			String meetingLabel = "m_" + i;
			MeetingTreeNode meetingNode = new MeetingTreeNode(meetingLabel, grid);
			//Add the meeting node to the tree.
			if (root.canAddMeeting(meetingLabel)) {
				root.add(meetingNode);
			} else {
				System.out.println("problem adding a random meeting: " + meetingLabel);
			}
			// Add participations of this meeting.
			Iterator iter = meetings[i].values().iterator();
			while (iter.hasNext()) {
				int participantNumber = ((Node) iter.next()).participant;
				String participantLabel = "p_" + participantNumber;
				if (meetingNode.canAddParticipant(participantLabel)) {
					ParticipantTreeNode participantNode = new ParticipantTreeNode(participantLabel, meetingNode.getGrid());
					ParticipantTreeNode existingNode = root.getParticipantByName(participantLabel);
					if (existingNode != null) {
						participantNode.setSchedule(existingNode.getSchedule());
					}
					meetingNode.add(participantNode);
				}
			}
		}

		return root;
	}

	private TimeGrid makeTimeGrid() {
		TimeGrid grid = null;
		if (sameForAll) {
			grid = new TimeGrid(minRange, maxRange);
		} else {
			int start = minRange + rnd.nextInt(maxRange - minRange + 1);
			int end = start + rnd.nextInt(maxRange - start + 1);
			grid = new TimeGrid(start, end);
		}
		return grid;
	}

	private void writeTreeToFile() {
		String fileName = jtxSaveFileName.getText();
		if (fileName != "" && treeRoot != null) {
			treeRoot.writeToFile("tests/" + fileName);
		}
	}

	public RootTreeNode getTreeRoot() {
		return treeRoot;
	}

	public static RootTreeNode getTree() {
		RandomMeetingGenerator generator = new RandomMeetingGenerator();
		generator.setVisible(true);
		return generator.getTreeRoot();
	}

	public RootTreeNode getTestTree(int _numOfMeetings, int _minRange, int _maxRange, boolean _sameForAll, int _numOfParticipants, int _minParticipations, int _maxParticipations, boolean _fixedForAll) {
		RootTreeNode aTreeRoot = null;
		int t = 0;
		boolean isTree = false;
		do {
			t++;

			numOfMeetings = _numOfMeetings;
			minRange = _minRange;
			maxRange = _maxRange;
			sameForAll = _sameForAll;
			numOfParticipants = _numOfParticipants;
			minParticipations = _minParticipations;
			maxParticipations = _maxParticipations;
			fixedForAll = _fixedForAll;

			meetings = new HashMap[numOfMeetings];
			fillMeetings();
			isTree = checkDFSTree();
		} while (t < TRIES && !isTree);
		if (isTree) {
			System.out.println("generated the random tree...");
			aTreeRoot = makeMeetingTree();
		} else {
			System.out.println("problems generating random tree... ");
		}
		return aTreeRoot;
	}

}//  @jve:decl-index=0:visual-constraint="78,29"
