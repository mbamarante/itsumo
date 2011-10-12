/*
 * Created on 17 mai 2005
 * 
 */
package frodo.interaction.meeting.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ItemEvent;
import java.net.URL;
import java.util.Enumeration;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.JToolBar;
import javax.swing.JTree;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableColumnModel;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableColumn;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import frodo.interaction.meeting.TimeGrid;
import frodo.interaction.meeting.graph.Heuristics;
import frodo.interaction.meeting.roundtrip.DPOPMaker;
import frodo.interaction.meeting.roundtrip.DPOPRunner;

/**
 * @author gushakov
 * 
 *  Some of the icons used in the GUI for this class are available from
 *	Jonathan Bingham at http://www.codeguru.com/java/articles/656.shtml
 *  
 */
public class MeetingExplorer extends JFrame {

	/**
	 * 
	 */
	public static final String ICON_PATH = "/frodo/resources/icons";

	private static final long serialVersionUID = 1L;

	private javax.swing.JPanel jContentPane = null;

	private JSplitPane jSplitPane = null;

	private JPanel jPanel = null;

	private JPanel jPanel1 = null;

	private JScrollPane jScrollPane = null;

	private JTree jtOrganizer = null;

	private JToolBar jToolBar = null;

	private JTable jtabPreferences = null;

	private JScrollPane jScrollPane1 = null;

	private JButton jButton = null;

	private JPanel jPanel2 = null;

	private JMenu jMenu = null;

	private JMenuItem jmiSaveAsDPOP = null;

	private JLabel jLabel = null;

	private JTable jtabSolution = null;

	private JScrollPane jScrollPane2 = null;

	private JPanel jPanel3 = null;

	private JLabel jLabel1 = null;

	private JMenu jMenu1 = null;

	private JMenuItem jmiShowSolution = null;

	DefaultTreeModel organizerModel = null;

	TreeSelectionModel organizerSelection = null;

	SchedulerTableModel schedulerModel = null;

	DefaultTableColumnModel schedulerColumnModel = null;

	MeetingsTableModel meetingsModel = null;

	DefaultTableColumnModel meetingsColumnModel = null;

	SolutionTableModel solutionModel = null;

	DefaultTableColumnModel solutionColumnModel = null;

	DPOPMaker maker = null;

	private JMenuItem jmiSaveAsMeeting = null;

	private JMenuItem jmiLoad = null;

	private JMenuItem jmiViewCSPGraph = null;

	private JButton jButton1 = null;

	private JMenuItem jmiShowStatistics = null;

	private JMenuItem jmiLoadRandomTree = null;

	private JTabbedPane jTabbedPane = null;

	private JTable jtabMeetings = null;

	private JScrollPane jScrollPane3 = null;

	private JMenu jMenu2 = null;

	private JCheckBoxMenuItem jCheckBoxMenuItem = null;

	private JCheckBoxMenuItem jCheckBoxMenuItem1 = null;

	private JCheckBoxMenuItem jCheckBoxMenuItem2 = null;

	private JCheckBoxMenuItem jCheckBoxMenuItem3 = null;

	private JCheckBoxMenuItem jCheckBoxMenuItem4 = null;

	private JButton jButton2 = null;

	private JButton jButton3 = null;

	private JButton jButton4 = null;

	private JCheckBoxMenuItem jCheckBoxMenuItem5 = null;

	private JCheckBoxMenuItem jCheckBoxMenuItem6 = null;

	//---------------------------------------------------------------------------------
	/**
	 * Custom cell renderer which displays the cells in different colors
	 * depending on their values or wether or not a cell is edited.
	 */
	class ColorCellRenderer extends DefaultTableCellRenderer {
		private static final long serialVersionUID = 1L;

		/*
		 *  (non-Javadoc)
		 * @see javax.swing.table.TableCellRenderer#getTableCellRendererComponent(javax.swing.JTable, java.lang.Object, boolean, boolean, int, int)
		 */
		public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {

			Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);

			//Participants' labels are in gray.
			if (column == 0) {
				c.setBackground(Color.lightGray);
			} else {
				int v = Integer.parseInt(value.toString());
				if (v >= 0) {
					// Set the background for the negative values.
					c.setBackground(Color.green);
				} else {
					c.setBackground(Color.red);
				}
			}
			return c;
		}
	}

	class MeetingsCellRenderer extends DefaultTableCellRenderer {
		private static final long serialVersionUID = 1L;

		/*
		 *  (non-Javadoc)
		 * @see javax.swing.table.TableCellRenderer#getTableCellRendererComponent(javax.swing.JTable, java.lang.Object, boolean, boolean, int, int)
		 */
		public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
			Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
			if (column == 0 || value.toString().equals("NA")) {
				c.setBackground(Color.lightGray);
			} else {
				int v = Integer.parseInt(value.toString());
				if (v >= 0) {
					// Set the background for the negative values.
					c.setBackground(Color.green);
				} else {
					c.setBackground(Color.red);
				}
			}

			return c;
		}
	}

	class SchedulerTableModel extends DefaultTableModel {
		private static final long serialVersionUID = 1L;

		/*
		 * Overrides to disallow changes to the first column (labels).
		 * 
		 * @see javax.swing.table.TableModel#isCellEditable(int, int)
		 */
		public boolean isCellEditable(int row, int column) {
			if (column > 0)
				return true;
			else
				return false;
		}

		/*
		 * Overrides to format the input values, defaults to the value specified
		 * in ParticipantTreeNode
		 * 
		 * @see javax.swing.table.TableModel#setValueAt(java.lang.Object, int,
		 *      int)
		 */
		public void setValueAt(Object aValue, int row, int column) {
			Object newValue = null;
			try {

				if (aValue.toString().matches("\\s*")) {

					newValue = new Integer(ParticipantTreeNode.DEFAULT_SCHEDULING_PREFERENCE);
				} else {
					newValue = new Integer(Integer.parseInt(aValue.toString()));
				}

			} catch (NumberFormatException e) {
				// Can't parse the cell value as int.
				newValue = new Integer(ParticipantTreeNode.DEFAULT_SCHEDULING_PREFERENCE);
			}

			super.setValueAt(newValue, row, column);
		}
	}

	class MeetingsTableModel extends SchedulerTableModel {
		private static final long serialVersionUID = 1L;

		/*
		 * (non-Javadoc)
		 * @see javax.swing.table.TableModel#isCellEditable(int, int)
		 */
		public boolean isCellEditable(int row, int column) {
			boolean editable = super.isCellEditable(row, column);
			if (editable) {
				int currentTimeSlot = Integer.parseInt(meetingsModel.getColumnName(column));
				//Restrict the editable cells to the ones corresponding
				//to a meeting's time slots.
				String meetingLabel = meetingsModel.getValueAt(row, 0).toString();
				MeetingTreeNode meeting = ((RootTreeNode) organizerModel.getRoot()).getMeetingByLabel(meetingLabel);
				TimeGrid grid = meeting.getGrid();
				int begin = grid.getStartSlot().intValue();
				int end = grid.getEndSlot().intValue();
				if (currentTimeSlot < begin || currentTimeSlot > end) {
					editable = false;
				}
			}
			return editable;
		}

		/* (non-Javadoc)
		 * @see javax.swing.table.DefaultTableModel#setValueAt(java.lang.Object, int, int)
		 */
		public void setValueAt(Object aValue, int row, int column) {
			Object newValue = null;
			if (isCellEditable(row, column)) {
				try {

					if (aValue.toString().matches("\\s*")) {

						newValue = new Integer(ParticipantTreeNode.DEFAULT_SCHEDULING_PREFERENCE);
					} else {
						newValue = new Integer(Integer.parseInt(aValue.toString()));
					}

				} catch (NumberFormatException e) {
					// Can't parse the cell value as int.
					newValue = new Integer(ParticipantTreeNode.DEFAULT_SCHEDULING_PREFERENCE);
				}
			} else {
				newValue = "NA";
			}

			super.setValueAt(newValue, row, column);
		}

	}

	class LeafSelectionModel extends DefaultTreeSelectionModel {
		private static final long serialVersionUID = 1L;
		/*
		 * Overrides to add only a leaf from the parent of already selected
		 * leafs.
		 * 
		 * @see javax.swing.tree.TreeSelectionModel#addSelectionPath(javax.swing.tree.TreePath)
		 */
		public void addSelectionPath(TreePath path) {
			if (canSelectPath(path)) {
				super.addSelectionPath(path);
			}
		}

		public void setSelectionPaths(TreePath[] pPaths) {
			boolean flag = true;
			if (pPaths != null && pPaths.length > 1) {
				int i = 0;
				while (flag && i < pPaths.length) {
					if (!canSelectPath(pPaths[i])) {
						flag = false;
					}
					i++;
				}
			}
			if (flag) {
				super.setSelectionPaths(pPaths);
			}
		}

		private boolean canSelectPath(TreePath path) {
			boolean answer = false;
			if (((TreeNode) path.getLastPathComponent()).isLeaf()) {
				TreePath[] paths = getSelectionPaths();
				if (paths != null && paths.length > 0) {
					TreePath firstParentPath = paths[0].getParentPath();
					if (firstParentPath != null) {
						TreeNode firstParent = (TreeNode) paths[0].getParentPath().getLastPathComponent();
						TreeNode thisParent = (TreeNode) path.getParentPath().getLastPathComponent();
						if (firstParent == thisParent) {
							answer = true;
						}
					}
				}
			}

			return answer;
		}
	}

	class SolutionTableModel extends DefaultTableModel {
		private static final long serialVersionUID = 1L;
		public boolean isCellEditable(int row, int column) {
			return false;
		}
	}

	class SolutionCellRenderer extends DefaultTableCellRenderer {
		private static final long serialVersionUID = 1L;

		public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {

			Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);

			//Meeting's labels are in gray.
			if (column == 0) {
				c.setBackground(Color.lightGray);
			} else {
				if (!value.toString().equals("")) {
					c.setBackground(Color.yellow);
				} else {
					c.setBackground(Color.white);
				}

			}
			return c;
		}
	}

	class OrganizerTreeCellRenderer extends DefaultTreeCellRenderer {
		private static final long serialVersionUID = 1L;

		Object currentCell = null;

		/* (non-Javadoc)
		 * @see javax.swing.tree.DefaultTreeCellRenderer#getTreeCellRendererComponent(javax.swing.JTree, java.lang.Object, boolean, boolean, boolean, int, boolean)
		 */
		public Component getTreeCellRendererComponent(JTree arg0, Object value, boolean arg2, boolean arg3, boolean arg4, int arg5, boolean arg6) {
			currentCell = value;
			return super.getTreeCellRendererComponent(arg0, value, arg2, arg3, arg4, arg5, arg6);
		}

		public Icon getLeafIcon() {
			Icon icon = null;
			if (currentCell instanceof ParticipantTreeNode) {
				icon = new ImageIcon(getClass().getResource(ICON_PATH + "/participant.gif"));
			} else {
				icon = getDefaultLeafIcon();
			}
			return icon;
		}

		public Icon getOpenIcon() {
			Icon icon = null;
			if (currentCell instanceof RootTreeNode) {
				icon = getDefaultOpenIcon();
			} else {
				icon = new ImageIcon(getClass().getResource(ICON_PATH + "/meeting.gif"));
			}
			return icon;
		}
	}

	/**
	 * This is the default constructor
	 */
	public MeetingExplorer() {
		super();
		initialize();

		// Additionally, register the models of the organizer and the
		// schedulers.

		organizerModel = (DefaultTreeModel) jtOrganizer.getModel();
		organizerSelection = jtOrganizer.getSelectionModel();

		schedulerModel = (SchedulerTableModel) jtabPreferences.getModel();

		schedulerColumnModel = (DefaultTableColumnModel) jtabPreferences.getColumnModel();

		meetingsModel = (MeetingsTableModel) jtabMeetings.getModel();

		meetingsColumnModel = (DefaultTableColumnModel) jtabMeetings.getColumnModel();

		solutionModel = (SolutionTableModel) jtabSolution.getModel();

		solutionColumnModel = (DefaultTableColumnModel) jtabSolution.getColumnModel();

		//Set the DPOP maker object.
		RootTreeNode root = (RootTreeNode) organizerModel.getRoot();
		maker = new DPOPMaker(root);
	}

	/**
	 * Creates the headers for the schedule table. Each meeting has it's own
	 * time grid.
	 * 
	 * @see MeetingTreeNode#grid
	 * @see frodo.interaction.meeting.TimeGrid
	 */
	private void makePreferencesModel(MeetingTreeNode meeting) {
		TimeGrid grid = meeting.getGrid();
		schedulerModel.addColumn("Participants");
		for (int i = 0; i < grid.getNumberOfTimeSlots(); i++) {
			schedulerModel.addColumn(grid.getSlotAt(i).toString());
		}
	}

	private void setPreferencesCellRenderer() {
		ColorCellRenderer renderer = new ColorCellRenderer();
		Enumeration columns = schedulerColumnModel.getColumns();
		while (columns.hasMoreElements()) {
			TableColumn column = (TableColumn) columns.nextElement();
			//Provide the columns with a custom cell renderer.
			column.setCellRenderer(renderer);
		}
	}

	private MutableTreeNode findParticipantNode(String label) {
		MutableTreeNode node = null;
		TreePath[] paths = organizerSelection.getSelectionPaths();
		boolean found = false;
		int i = 0;
		while (!found && i < paths.length) {
			node = (MutableTreeNode) paths[i].getLastPathComponent();
			if (node instanceof ParticipantTreeNode && ((LabeledTreeNode) node).getLabel().equals(label)) {
				found = true;
			}
			i++;
		}
		return node;
	}

	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setJMenuBar(getMyJMenuBar());
		this.setSize(500, 400);
		this.setContentPane(getJContentPane());
		this.setTitle("Meeting Explorer");
		this.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent e) {
				//TODO: Check if any info needs to be saved.
				System.out.println("Closing meeting explorer.");
				System.exit(0);
			}
		});
	}

	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getJContentPane() {
		if (jContentPane == null) {
			jContentPane = new javax.swing.JPanel();
			jContentPane.setLayout(new java.awt.BorderLayout());
			jContentPane.setPreferredSize(new java.awt.Dimension(500, 400));
			jContentPane.add(getJSplitPane(), java.awt.BorderLayout.CENTER);
		}
		return jContentPane;
	}

	/**
	 * This method initializes jSplitPane
	 * 
	 * @return javax.swing.JSplitPane
	 */
	private JSplitPane getJSplitPane() {
		if (jSplitPane == null) {
			jSplitPane = new JSplitPane();
			jSplitPane.setDividerSize(5);
			jSplitPane.setLeftComponent(getJPanel());
			jSplitPane.setRightComponent(getJPanel1());
		}
		return jSplitPane;
	}

	/**
	 * This method initializes jPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel() {
		if (jPanel == null) {
			jPanel = new JPanel();
			jPanel.setLayout(new BorderLayout());
			jPanel.add(getJToolBar(), java.awt.BorderLayout.NORTH);
			jPanel.add(getJScrollPane(), java.awt.BorderLayout.CENTER);
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
			jPanel1.add(getJPanel3(), null);
			jPanel1.add(getJTabbedPane(), null);
			jPanel1.add(getJPanel2(), null);
			jPanel1.add(getJScrollPane2(), null);
		}
		return jPanel1;
	}

	/**
	 * This method initializes jScrollPane
	 * 
	 * @return javax.swing.JScrollPane
	 */
	private JScrollPane getJScrollPane() {
		if (jScrollPane == null) {
			jScrollPane = new JScrollPane();
			jScrollPane.setViewportView(getJtOrganizer());
		}
		return jScrollPane;
	}

	/**
	 * This method initializes jtOrganizer
	 * 
	 * @return javax.swing.JTree
	 */
	private JTree getJtOrganizer() {
		if (jtOrganizer == null) {
			//jtOrganizer = new JTree(makeOneMeeting());
			//TODO: change the default file to load.
			jtOrganizer = new JTree(RootTreeNode.fromFile("tests/3.mt"));
			jtOrganizer.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 10));
			jtOrganizer.setSelectionModel(new LeafSelectionModel());
			jtOrganizer.setCellRenderer(new OrganizerTreeCellRenderer());
			jtOrganizer.addTreeSelectionListener(new javax.swing.event.TreeSelectionListener() {
				public void valueChanged(javax.swing.event.TreeSelectionEvent e) {
					if (jTabbedPane.getSelectedIndex() == 0) {
						createSchedulingPreferencesByParticipants();
					} else if (jTabbedPane.getSelectedIndex() == 1) {
						createSchedulingPreferencesByMeetings();
					}
					//Highlight the selected nodes.
					TreePath[] paths = organizerSelection.getSelectionPaths();
					if (paths != null && paths.length == 1) {
						Object last = paths[0].getLastPathComponent();
						if (last instanceof MeetingTreeNode) {
							maker.highlightAllAttendants((MeetingTreeNode) last);
						} else if (last instanceof ParticipantTreeNode) {
							maker.highlightAllParticipations((ParticipantTreeNode) last);
						} else {
							maker.clearAllHighlights();
						}
					}
				}
			});
		}
		return jtOrganizer;
	}

	private void createSchedulingPreferencesByParticipants() {
		TreePath[] paths = organizerSelection.getSelectionPaths();

		//Clear the schedule table.
		schedulerModel.setRowCount(0);
		schedulerModel.setColumnCount(0);

		MeetingTreeNode meeting = null;

		if (paths != null) {

			/*
			 * Show the relations information for the
			 * selected participant.
			 */

			if (paths.length == 1 && ((MutableTreeNode) paths[0].getLastPathComponent()) instanceof MeetingTreeNode) {
				meeting = (MeetingTreeNode) paths[0].getLastPathComponent();
			} else if (paths.length >= 1 && paths[0] != null && !(((MutableTreeNode) paths[0].getLastPathComponent()) instanceof RootTreeNode)) {
				meeting = (MeetingTreeNode) ((MutableTreeNode) paths[0].getLastPathComponent()).getParent();
				makePreferencesModel(meeting);
				setPreferencesCellRenderer();
				/*
				 * Add the selected participants to the
				 * scheduler.
				 */
				for (int i = 0; i < paths.length; i++) {
					TreePath path = paths[i];
					TreeNode lastNode = (TreeNode) path.getLastPathComponent();
					if (lastNode instanceof ParticipantTreeNode) {
						//Display the participants schedule
						// preferences.

						Vector rowData = ((ParticipantTreeNode) lastNode).getLabelSchedule(meeting.getGrid());
						schedulerModel.addRow(rowData);
					}
				}
			}
		}

	}

	private void createSchedulingPreferencesByMeetings() {
		TreePath[] paths = organizerSelection.getSelectionPaths();

		// Clear the meetings table.
		meetingsModel.setRowCount(0);
		meetingsModel.setColumnCount(0);

		if (paths != null && paths.length == 1) {
			MutableTreeNode last = (MutableTreeNode) paths[0].getLastPathComponent();
			if (last instanceof ParticipantTreeNode) {
				//Generate all of the meetings attended by this participant.
				ParticipantTreeNode participant = (ParticipantTreeNode) last;
				Vector attendedMeetings = new Vector();
				Enumeration myenum = ((RootTreeNode) organizerModel.getRoot()).children();

				while (myenum.hasMoreElements()) {
					MeetingTreeNode meeting = (MeetingTreeNode) myenum.nextElement();
					if (meeting.hasParticipant(participant)) {
						attendedMeetings.add(meeting);

					}
				}
				TimeGrid commonGrid = makeMeetingsModel(attendedMeetings);
				setMeetingsCellRenderer();
				for (int i = 0; i < attendedMeetings.size(); i++) {
					//Store the row data for each participation.
					Object[] row = new Object[commonGrid.getNumberOfTimeSlots() + 1];

					MeetingTreeNode meeting = (MeetingTreeNode) attendedMeetings.elementAt(i);
					row[0] = meeting.getLabel();
					TimeGrid grid = meeting.getGrid();
					/*
					 * Detrmine where does the grid for this meeting fit
					 * into the common grid, add one for the first column
					 * with meeting labels.
					 */
					int beginIndex = commonGrid.getIndexForSlot(grid.getStartSlot()) + 1;
					int endIndex = commonGrid.getIndexForSlot(grid.getEndSlot()) + 1;
					for (int j = 1; j < row.length; j++) {
						if (j >= beginIndex && j <= endIndex) {
							Integer timeSlot = new Integer(meetingsColumnModel.getColumn(j).getHeaderValue().toString());
							row[j] = participant.getSchedule().get(timeSlot);
						} else {
							row[j] = "NA";
						}
					}
					//Add row to the table.
					meetingsModel.addRow(row);
				}
			}
		}
	}

	/*
	 * Adds the appropriate number of columns to the
	 * meeting table and returns the common time grid
	 * of the meetings.
	 */
	private TimeGrid makeMeetingsModel(Vector allAttended) {
		// Set up the columns of the meetings table.
		meetingsModel.addColumn("Meetings");
		TimeGrid common = RootTreeNode.getCommonGrid(allAttended);
		Vector timeSlots = common.getTimeSlots();
		for (int i = 0; i < timeSlots.size(); i++) {
			Integer slot = (Integer) timeSlots.elementAt(i);
			meetingsModel.addColumn(slot);
		}
		return common;
	}

	private void setMeetingsCellRenderer() {
		MeetingsCellRenderer renderer = new MeetingsCellRenderer();
		Enumeration columns = meetingsColumnModel.getColumns();
		while (columns.hasMoreElements()) {
			TableColumn column = (TableColumn) columns.nextElement();
			//Provide the columns with a custom cell renderer.
			column.setCellRenderer(renderer);
		}
	}

	/**
	 * This method initializes jToolBar
	 * 
	 * @return javax.swing.JToolBar
	 */
	private JToolBar getJToolBar() {
		if (jToolBar == null) {
			jToolBar = new JToolBar();
			jToolBar.setPreferredSize(new java.awt.Dimension(50, 30));
			jToolBar.add(getJButton());
			jToolBar.add(getJButton1());
			jToolBar.add(getJButton2());
			jToolBar.add(getJButton3());
			jToolBar.add(getJButton4());
		}
		return jToolBar;
	}

	/**
	 * This method initializes jtabPreferences
	 * 
	 * @return javax.swing.JTable
	 */
	private JTable getJtabPreferences() {
		if (jtabPreferences == null) {
			jtabPreferences = new JTable();
			jtabPreferences.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 10));
			jtabPreferences.setRowSelectionAllowed(false);
			jtabPreferences.setModel(new SchedulerTableModel());
			jtabPreferences.getModel().addTableModelListener(new TableModelListener() {
				public void tableChanged(TableModelEvent e) {
					int row = e.getFirstRow();
					int column = e.getColumn();
					if (row >= 0 && column > 0) {
						//Record the changes to the participant's preferences.
						String label = (String) schedulerModel.getValueAt(row, 0);
						ParticipantTreeNode participant = (ParticipantTreeNode) findParticipantNode(label);
						Object timeSlot = schedulerColumnModel.getColumn(column).getHeaderValue();
						Integer key = new Integer(timeSlot.toString());
						participant.setPreferenceForTimeSlot(key, schedulerModel.getValueAt(row, column));

						//Notify maker that the unary constraint has changed.
						maker.changeUnaryConstraint(participant, key);
					}
				}
			});
		}
		return jtabPreferences;
	}

	/**
	 * This method initializes jScrollPane1
	 * 
	 * @return javax.swing.JScrollPane
	 */
	private JScrollPane getJScrollPane1() {
		if (jScrollPane1 == null) {
			jScrollPane1 = new JScrollPane();
			jScrollPane1.setViewportView(getJtabPreferences());
		}
		return jScrollPane1;
	}

	/**
	 * This method initializes jButton
	 * 
	 * @return javax.swing.JButton
	 */
	private JButton getJButton() {
		if (jButton == null) {
			jButton = new JButton();
			jButton.setToolTipText("Create new meeting or a participant.");
			jButton.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/new.gif")));
			jButton.setPreferredSize(new java.awt.Dimension(16, 16));
			jButton.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {

					TreePath path = organizerSelection.getSelectionPath();

					MutableTreeNode node;

					if (organizerModel.getRoot() == null) {
						organizerModel.setRoot(new RootTreeNode());
					} else if (path == null) {
						// If no node selected, add new meeting node.
						addNewMeeting();
					} else {
						node = (MutableTreeNode) path.getLastPathComponent();
						if (node instanceof RootTreeNode) {
							addNewMeeting();
						} else if (node instanceof MeetingTreeNode) {
							addNewParticipant((MeetingTreeNode) node);
						} else {
							//Selected node is a participant node.
							node = (MutableTreeNode) node.getParent();
							addNewParticipant((MeetingTreeNode) node);
						}
					}
				}

			});
		}
		return jButton;
	}

	private void addNewMeeting() {
		RootTreeNode root = (RootTreeNode) organizerModel.getRoot();
		NewMeetingDialog.MeetingOptions options = NewMeetingDialog.showDialod();
		if (options != null) {
			String label = options.getLabel();
			if (root.canAddMeeting(label)) {
				MeetingTreeNode meeting = new MeetingTreeNode(label, new TimeGrid(options.getRangeFrom(), options.getRangeTo()));
				organizerModel.insertNodeInto(meeting, root, root.getChildCount());
				jtOrganizer.expandPath(new TreePath(organizerModel.getRoot()));
			}
		}
	}

	private void addNewParticipant(MeetingTreeNode parent) {
		RootTreeNode root = (RootTreeNode) organizerModel.getRoot();
		String s = JOptionPane.showInputDialog(MeetingExplorer.this, "Enter the name of the new participant:", "New Participant", JOptionPane.QUESTION_MESSAGE);
		if (s != null && parent.canAddParticipant(s)) {
			ParticipantTreeNode participant = new ParticipantTreeNode(s, parent.getGrid());
			ParticipantTreeNode existingNode = root.getParticipantByName(s.trim());
			if (existingNode != null) {
				participant.setSchedule(existingNode.getSchedule());
			}
			organizerModel.insertNodeInto(participant, parent, parent.getChildCount());
			Object[] toExpand = new Object[2];
			toExpand[0] = root;
			toExpand[1] = parent;
			jtOrganizer.expandPath(new TreePath(toExpand));

			//Notify the maker of the change.
			maker.addNewParticipation(participant, parent, root);
		}
	}

	//---------------------------------------------------------------------------------
	//----------------------- TEST MEETINGS -------------------------------------------
/*
	private RootTreeNode makeOneMeeting() {
		RootTreeNode root = new RootTreeNode();
		MeetingTreeNode m1 = new MeetingTreeNode("m_0");
		ParticipantTreeNode p1_m1 = new ParticipantTreeNode("p_0", m1.getGrid());
		ParticipantTreeNode p2_m1 = new ParticipantTreeNode("p_1", m1.getGrid());
		ParticipantTreeNode p3_m1 = new ParticipantTreeNode("p_2", m1.getGrid());
		m1.add(p1_m1);
		m1.add(p2_m1);
		m1.add(p3_m1);
		root.add(m1);
		return root;
	}

	private RootTreeNode makeAnotherMeetings() {
		RootTreeNode root = new RootTreeNode();

		MeetingTreeNode m0 = new MeetingTreeNode("m_0", new TimeGrid(1, 5));
		ParticipantTreeNode p0_m0 = new ParticipantTreeNode("p_0", m0.getGrid());
		ParticipantTreeNode p1_m0 = new ParticipantTreeNode("p_1", m0.getGrid());
		ParticipantTreeNode p2_m0 = new ParticipantTreeNode("p_2", m0.getGrid());

		MeetingTreeNode m1 = new MeetingTreeNode("m_1", new TimeGrid(3, 7));
		ParticipantTreeNode p0_m1 = new ParticipantTreeNode("p_0", m1.getGrid());
		p0_m1.setSchedule(p0_m0.getSchedule());
		ParticipantTreeNode p3_m1 = new ParticipantTreeNode("p_3", m1.getGrid());

		m0.add(p0_m0);
		m0.add(p1_m0);
		m0.add(p2_m0);
		m1.add(p0_m1);
		m1.add(p3_m1);

		root.add(m0);
		root.add(m1);

		return root;
	}
*/
	//  ---------------------------------------------------------------------------------
	//  ---------------------------------------------------------------------------------

	/**
	 * This method initializes jPanel2
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel2() {
		if (jPanel2 == null) {
			jLabel = new JLabel();
			FlowLayout flowLayout1 = new FlowLayout();
			jPanel2 = new JPanel();
			jPanel2.setLayout(flowLayout1);
			jLabel.setText("Solution");
			jLabel.setFont(new java.awt.Font("Dialog", java.awt.Font.BOLD, 10));
			flowLayout1.setAlignment(java.awt.FlowLayout.LEFT);
			jPanel2.add(jLabel, null);
		}
		return jPanel2;
	}

	/**
	 * This method initializes JMenuBar
	 * 
	 * @return javax.swing.JMenuBar
	 */
	private JMenuBar getMyJMenuBar() {
		JMenuBar jmb = new JMenuBar();
		jmb.setPreferredSize(new Dimension(100, 20));
		jmb.add(getJMenu());
		jmb.add(getJMenu1());
		jmb.add(getJMenu2());
		return jmb;
	}

	/**
	 * This method initializes jMenu
	 * 
	 * @return javax.swing.JMenu
	 */
	private JMenu getJMenu() {
		if (jMenu == null) {
			jMenu = new JMenu();
			jMenu.setText("File");
			jMenu.setFont(new java.awt.Font("Dialog", java.awt.Font.BOLD, 12));
			jMenu.add(getJmiLoad());
			jMenu.add(getJmiLoadRandomTree());
			jMenu.addSeparator();
			jMenu.add(getJmiSaveAsDPOP());
			jMenu.add(getJmiSaveAsMeeting());
		}
		return jMenu;
	}

	/**
	 * This method initializes jmiSaveAsDPOP
	 * 
	 * @return javax.swing.JMenuItem
	 */
	private JMenuItem getJmiSaveAsDPOP() {
		if (jmiSaveAsDPOP == null) {
			jmiSaveAsDPOP = new JMenuItem();
			jmiSaveAsDPOP.setText("Save (DPOP)");
			URL myURL = getClass().getResource(ICON_PATH + "/export.gif");
			if (myURL != null) {
				Icon myi = new ImageIcon(myURL);
				jmiSaveAsDPOP.setIcon(myi);
			}
			jmiSaveAsDPOP.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					String fileName = (String) JOptionPane.showInputDialog("Enter the file name (in tests/):");
					if (fileName != null) {
						System.out.println("Writing DPOP file to tests/" + fileName);
						maker.update((RootTreeNode) organizerModel.getRoot());
						maker.writeOCOPToFile("tests/" + fileName);
					}
				}
			});
		}
		return jmiSaveAsDPOP;
	}

	/**
	 * This method initializes jtabSolution
	 * 
	 * @return javax.swing.JTable
	 */
	private JTable getJtabSolution() {
		if (jtabSolution == null) {
			jtabSolution = new JTable();
			jtabSolution.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 10));
			jtabSolution.setRowSelectionAllowed(false);
			jtabSolution.setColumnSelectionAllowed(false);
			jtabSolution.setModel(new SolutionTableModel());
		}
		return jtabSolution;
	}

	/**
	 * This method initializes jScrollPane2
	 * 
	 * @return javax.swing.JScrollPane
	 */
	private JScrollPane getJScrollPane2() {
		if (jScrollPane2 == null) {
			jScrollPane2 = new JScrollPane();
			jScrollPane2.setViewportView(getJtabSolution());
		}
		return jScrollPane2;
	}

	/**
	 * This method initializes jPanel3
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJPanel3() {
		if (jPanel3 == null) {
			jLabel1 = new JLabel();
			FlowLayout flowLayout2 = new FlowLayout();
			jPanel3 = new JPanel();
			jPanel3.setLayout(flowLayout2);
			jLabel1.setText("Scheduling Preferences");
			jLabel1.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 10));
			flowLayout2.setAlignment(java.awt.FlowLayout.LEFT);
			jPanel3.add(jLabel1, null);
		}
		return jPanel3;
	}

	/**
	 * This method initializes jMenu1
	 * 
	 * @return javax.swing.JMenu
	 */
	private JMenu getJMenu1() {
		if (jMenu1 == null) {
			jMenu1 = new JMenu();
			jMenu1.setText("Solve");
			jMenu1.add(getJmiViewCSPGraph());
			jMenu1.add(getJmiShowSolution());
			jMenu1.add(getJmiShowStatistics());
		}
		return jMenu1;
	}

	/**
	 * This method initializes jmiShowSolution
	 * 
	 * @return javax.swing.JMenuItem
	 */
	private JMenuItem getJmiShowSolution() {
		if (jmiShowSolution == null) {
			jmiShowSolution = new JMenuItem();
			jmiShowSolution.setText("Show Solution");
			jmiShowSolution.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/run.gif")));
			jmiShowSolution.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					solveOCOP();
				}
			});
		}
		return jmiShowSolution;
	}

	private void solveOCOP() {
		solutionModel.setRowCount(0);
		solutionModel.setColumnCount(0);

		DPOPRunner.runDPOP(maker.getOCOP(), null);

		while (!DPOPRunner.getMonitor().getEndOfProblem()) {
			// Wait until the monitor signals the end of the problem cycle.
			try {
				Thread.sleep(1000);
			} catch (Exception exp) {
			}
		}
		makeSolutionModel();
	}

	private void makeSolutionModel() {
		RootTreeNode root = (RootTreeNode) organizerModel.getRoot();

		TimeGrid common = root.getCommonGridAll();
		Vector timeSlots = common.getTimeSlots();
		solutionModel.addColumn("Meetings");
		for (int i = 0; i < timeSlots.size(); i++) {
			solutionModel.addColumn(timeSlots.elementAt(i));
		}

		setSolutionCellRenderer();

		Enumeration myenum = root.children();
		while (myenum.hasMoreElements()) {
			MeetingTreeNode meeting = (MeetingTreeNode) myenum.nextElement();
			Object[] row = new Object[timeSlots.size() + 1];
			row[0] = meeting.getLabel();

			Integer value = maker.getMeetingSolution(meeting);

			/*
			 * Solution for each meeting is represented by
			 * displaying the meeting's label in the cell
			 * of the time grid for which the meeting has
			 * been scheduled.
			 */
			int solutionIndex = timeSlots.indexOf(value) + 1;
			row[solutionIndex] = meeting.getLabel();

			//Fill the rest of the solution table with blanks.
			for (int i = 1; i < row.length; i++) {
				if (i != solutionIndex) {
					row[i] = "";
				}
			}

			solutionModel.addRow(row);
		}
	}

	private void setSolutionCellRenderer() {
		SolutionCellRenderer renderer = new SolutionCellRenderer();
		Enumeration columns = solutionColumnModel.getColumns();
		while (columns.hasMoreElements()) {
			TableColumn column = (TableColumn) columns.nextElement();
			//Provide the columns with a custom cell renderer.
			column.setCellRenderer(renderer);
		}
	}

	/**
	 * This method initializes jmiSaveAsMeeting	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getJmiSaveAsMeeting() {
		if (jmiSaveAsMeeting == null) {
			jmiSaveAsMeeting = new JMenuItem();
			jmiSaveAsMeeting.setText("Save (Meeting)");
			jmiSaveAsMeeting.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/save.gif")));
			jmiSaveAsMeeting.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					String fileName = (String) JOptionPane.showInputDialog("Enter the file name (in tests/):");
					if (fileName != null) {
						System.out.println("Writing meetings tree to file tests/" + fileName);
						writeTreeToFile("tests/" + fileName);
					}
				}
			});
		}
		return jmiSaveAsMeeting;
	}

	/**
	 * This method initializes jmiLoad	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getJmiLoad() {
		if (jmiLoad == null) {
			jmiLoad = new JMenuItem();
			jmiLoad.setText("Load");
			jmiLoad.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					String fileName = (String) JOptionPane.showInputDialog("Enter the file name (in tests/):");
					if (fileName != null) {
						System.out.println("Loading from the file tests/" + fileName);
						RootTreeNode newRoot = RootTreeNode.fromFile("tests/" + fileName);
						organizerModel.setRoot(newRoot);
						maker.update(newRoot);
					}
				}
			});
		}
		return jmiLoad;
	}

	private void writeTreeToFile(String fileName) {

		RootTreeNode root = (RootTreeNode) organizerModel.getRoot();
		if (root != null) {
			root.writeToFile(fileName);
		}
	}

	/**
	 * This method initializes jmiViewCSPGraph	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getJmiViewCSPGraph() {
		if (jmiViewCSPGraph == null) {
			jmiViewCSPGraph = new JMenuItem();
			jmiViewCSPGraph.setText("View CSP Graph");
			jmiViewCSPGraph.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/tree.gif")));
			jmiViewCSPGraph.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("generating a view of DPOP");
					maker.update((RootTreeNode) organizerModel.getRoot());
					maker.displayOCOP();
				}
			});
		}
		return jmiViewCSPGraph;
	}

	/**
	 * This method initializes jButton1	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButton1() {
		if (jButton1 == null) {
			jButton1 = new JButton();
			jButton1.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/delete.gif")));
			jButton1.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("deleting a node");
					TreePath[] paths = organizerSelection.getSelectionPaths();
					if (paths != null) {
						for (int i = 0; i < paths.length; i++) {
							MutableTreeNode node = (MutableTreeNode) paths[i].getLastPathComponent();
							if (!(node instanceof RootTreeNode)) {
								organizerModel.removeNodeFromParent(node);
							} else {
								organizerModel.setRoot(null);
							}
						}
						//TODO: for now we just notify the listeners that the problem has changed.
						maker.update((RootTreeNode) organizerModel.getRoot());
					}
				}
			});
		}
		return jButton1;
	}

	/**
	 * This method initializes jmiShowStatistics	
	 * 	
	 * @return javax.swing.JMenuItem	
	 */
	private JMenuItem getJmiShowStatistics() {
		if (jmiShowStatistics == null) {
			jmiShowStatistics = new JMenuItem();
			jmiShowStatistics.setText("Statistics");
			jmiShowStatistics.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/cascade.gif")));
			jmiShowStatistics.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("openning monitor window");
					DPOPRunner.showMonitorWindow();
				}
			});
		}
		return jmiShowStatistics;
	}

	/**
	 * This method initializes jmiLoadRandomTree
	 * 
	 * @return javax.swing.JMenuItem
	 */
	private JMenuItem getJmiLoadRandomTree() {
		if (jmiLoadRandomTree == null) {
			jmiLoadRandomTree = new JMenuItem();
			jmiLoadRandomTree.setText("Load Random Tree");
			jmiLoadRandomTree.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					RootTreeNode tree = RandomMeetingGenerator.getTree();
					if (tree != null) {
						organizerModel.setRoot(tree);
						maker.update(tree);
					}
				}
			});
		}
		return jmiLoadRandomTree;
	}

	/**
	 * This method initializes jTabbedPane	
	 * 	
	 * @return javax.swing.JTabbedPane	
	 */
	private JTabbedPane getJTabbedPane() {
		if (jTabbedPane == null) {
			jTabbedPane = new JTabbedPane();
			jTabbedPane.setFont(new java.awt.Font("Dialog", java.awt.Font.BOLD, 10));
			jTabbedPane.addTab("By participants", null, getJScrollPane1(), null);
			jTabbedPane.addTab("By meetings", null, getJScrollPane3(), null);
			jTabbedPane.addChangeListener(new javax.swing.event.ChangeListener() {
				public void stateChanged(javax.swing.event.ChangeEvent e) {
					notifyTreeSelectionListeners(null);
				}
			});
		}
		return jTabbedPane;
	}

	private void notifyTreeSelectionListeners(TreeSelectionEvent event) {
		TreeSelectionListener[] listeners = jtOrganizer.getTreeSelectionListeners();
		for (int i = 0; i < listeners.length; i++) {
			listeners[i].valueChanged(null);
		}
	}

	/**
	 * This method initializes jtabMeetings	
	 * 	
	 * @return javax.swing.JTable	
	 */
	private JTable getJtabMeetings() {
		if (jtabMeetings == null) {
			jtabMeetings = new JTable();
			jtabMeetings.setRowSelectionAllowed(false);
			jtabMeetings.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 10));
			jtabMeetings.setModel(new MeetingsTableModel());
			jtabMeetings.getModel().addTableModelListener(new TableModelListener() {
				public void tableChanged(TableModelEvent e) {
					int row = e.getFirstRow();
					int column = e.getColumn();
					if (row >= 0 && column > 0) {
						//Record the changes to the participant's preferences.
						String label = (String) meetingsModel.getValueAt(row, 0);
						ParticipantTreeNode participant = (ParticipantTreeNode) findParticipantNode(label);
						Object timeSlot = meetingsColumnModel.getColumn(column).getHeaderValue();
						Integer key = new Integer(timeSlot.toString());
						participant.setPreferenceForTimeSlot(key, meetingsModel.getValueAt(row, column));

						/* Need to redraw the table since all of the values
						 * for this time slot will change for this participant
						 * for all of the meeting he is scheduled to attend.
						 */
						notifyTreeSelectionListeners(null);

						// Notify maker that the unary constraint has changed.
						maker.changeUnaryConstraint(participant, key);
					}
				}
			});
		}
		return jtabMeetings;
	}

	/**
	 * This method initializes jScrollPane3	
	 * 	
	 * @return javax.swing.JScrollPane	
	 */
	private JScrollPane getJScrollPane3() {
		if (jScrollPane3 == null) {
			jScrollPane3 = new JScrollPane();
			jScrollPane3.setViewportView(getJtabMeetings());
		}
		return jScrollPane3;
	}

	/**
	 * This method initializes jMenu2	
	 * 	
	 * @return javax.swing.JMenu	
	 */
	private JMenu getJMenu2() {
		if (jMenu2 == null) {
			jMenu2 = new JMenu();
			jMenu2.setText("Heuristics");
			jMenu2.add(getJCheckBoxMenuItem());
			jMenu2.add(getJCheckBoxMenuItem1());
			jMenu2.addSeparator();
			jMenu2.add(getJCheckBoxMenuItem2());
			jMenu2.add(getJCheckBoxMenuItem3());
			jMenu2.add(getJCheckBoxMenuItem4());
			jMenu2.add(getJCheckBoxMenuItem5());
			jMenu2.add(getJCheckBoxMenuItem6());

			//To assure the exlusive selection.
			ButtonGroup group1 = new ButtonGroup();
			group1.add(jCheckBoxMenuItem);
			group1.add(jCheckBoxMenuItem1);

			ButtonGroup group2 = new ButtonGroup();
			group2.add(jCheckBoxMenuItem2);
			group2.add(jCheckBoxMenuItem3);
			group2.add(jCheckBoxMenuItem4);
			group2.add(jCheckBoxMenuItem5);
			group2.add(jCheckBoxMenuItem6);
		}
		return jMenu2;
	}

	/**
	 * This method initializes jCheckBoxMenuItem	
	 * 	
	 * @return javax.swing.JCheckBoxMenuItem	
	 */
	private JCheckBoxMenuItem getJCheckBoxMenuItem() {
		if (jCheckBoxMenuItem == null) {
			jCheckBoxMenuItem = new JCheckBoxMenuItem();
			jCheckBoxMenuItem.setText("Connect-To-First");
			jCheckBoxMenuItem.addItemListener(new java.awt.event.ItemListener() {
				public void itemStateChanged(java.awt.event.ItemEvent e) {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						System.out.println("switching to Connect-To-First heuristic");
						Heuristics.CURRENT_EQUALITY_CONNECTION = Heuristics.CONNECT_TO_FIRST;
						if (maker != null) {
							maker.update((RootTreeNode) organizerModel.getRoot());
						}
					}
				}
			});
		}
		if (Heuristics.CURRENT_EQUALITY_CONNECTION == Heuristics.CONNECT_TO_FIRST) {
			jCheckBoxMenuItem.setSelected(true);
		}
		return jCheckBoxMenuItem;
	}

	/**
	 * This method initializes jCheckBoxMenuItem1	
	 * 	
	 * @return javax.swing.JCheckBoxMenuItem	
	 */
	private JCheckBoxMenuItem getJCheckBoxMenuItem1() {
		if (jCheckBoxMenuItem1 == null) {
			jCheckBoxMenuItem1 = new JCheckBoxMenuItem();
			jCheckBoxMenuItem1.setText("Chain");
			jCheckBoxMenuItem1.addItemListener(new java.awt.event.ItemListener() {
				public void itemStateChanged(java.awt.event.ItemEvent e) {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						System.out.println("switching to Chain heuristic");
						Heuristics.CURRENT_EQUALITY_CONNECTION = Heuristics.CHAIN;
						if (maker != null) {
							maker.update((RootTreeNode) organizerModel.getRoot());
						}
					}

				}
			});
		}
		if (Heuristics.CURRENT_EQUALITY_CONNECTION == Heuristics.CHAIN) {
			jCheckBoxMenuItem1.setSelected(true);
		}
		return jCheckBoxMenuItem1;
	}

	/**
	 * This method initializes jCheckBoxMenuItem2	
	 * 	
	 * @return javax.swing.JCheckBoxMenuItem	
	 */
	private JCheckBoxMenuItem getJCheckBoxMenuItem2() {
		if (jCheckBoxMenuItem2 == null) {
			jCheckBoxMenuItem2 = new JCheckBoxMenuItem();
			jCheckBoxMenuItem2.setText("DFS (" + Heuristics.CURRENT_DFS_ROOT_ID + ")");
			jCheckBoxMenuItem2.addItemListener(new java.awt.event.ItemListener() {
				public void itemStateChanged(java.awt.event.ItemEvent e) {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						System.out.println("switching to DFS traversal mode");
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS;
						if (maker != null) {
							maker.displayOCOP();
						}
					}
				}
			});
		}
		if (Heuristics.CURRENT_TREE_BUILD_METHOD == Heuristics.DFS) {
			jCheckBoxMenuItem2.setSelected(true);
		}
		return jCheckBoxMenuItem2;
	}

	/**
	 * This method initializes jCheckBoxMenuItem3	
	 * 	
	 * @return javax.swing.JCheckBoxMenuItem	
	 */
	private JCheckBoxMenuItem getJCheckBoxMenuItem3() {
		if (jCheckBoxMenuItem3 == null) {
			jCheckBoxMenuItem3 = new JCheckBoxMenuItem();
			jCheckBoxMenuItem3.setText("DFS MCN");
			jCheckBoxMenuItem3.addItemListener(new java.awt.event.ItemListener() {
				public void itemStateChanged(java.awt.event.ItemEvent e) {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						System.out.println("switching to DFS MCN traversal mode");
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN;
						if (maker != null) {
							maker.displayOCOP();
						}
					}
				}
			});
		}
		if (Heuristics.CURRENT_TREE_BUILD_METHOD == Heuristics.DFS_MCN) {
			jCheckBoxMenuItem3.setSelected(true);
		}
		return jCheckBoxMenuItem3;
	}

	/**
	 * This method initializes jCheckBoxMenuItem4	
	 * 	
	 * @return javax.swing.JCheckBoxMenuItem	
	 */
	private JCheckBoxMenuItem getJCheckBoxMenuItem4() {
		if (jCheckBoxMenuItem4 == null) {
			jCheckBoxMenuItem4 = new JCheckBoxMenuItem();
			jCheckBoxMenuItem4.setText("DFS CLIQUE MCN");
			jCheckBoxMenuItem4.addItemListener(new java.awt.event.ItemListener() {
				public void itemStateChanged(java.awt.event.ItemEvent e) {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						System.out.println("switching to DFS CLIQUE MCN traversal mode");
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_CLIQUE_MCN;
						if (maker != null) {
							maker.displayOCOP();
						}
					}
				}
			});
		}
		if (Heuristics.CURRENT_TREE_BUILD_METHOD == Heuristics.DFS_CLIQUE_MCN) {
			jCheckBoxMenuItem4.setSelected(true);
		}
		return jCheckBoxMenuItem4;
	}

	/**
	 * This method initializes jButton2	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButton2() {
		if (jButton2 == null) {
			jButton2 = new JButton();
			jButton2.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/run.gif")));
			jButton2.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("running the problem");
					solveOCOP();
				}
			});
		}
		return jButton2;
	}

	/**
	 * This method initializes jButton3	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButton3() {
		if (jButton3 == null) {
			jButton3 = new JButton();
			jButton3.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/cascade.gif")));
			jButton3.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("openning the monitor window");
					DPOPRunner.showMonitorWindow();
				}
			});
		}
		return jButton3;
	}

	/**
	 * This method initializes jButton4	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButton4() {
		if (jButton4 == null) {
			jButton4 = new JButton();
			jButton4.setIcon(new ImageIcon(getClass().getResource(ICON_PATH + "/tree.gif")));
			jButton4.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					System.out.println("generating a view of DPOP");
					maker.update((RootTreeNode) organizerModel.getRoot());
					maker.displayOCOP();
				}
			});
		}
		return jButton4;
	}

	/**
	 * This method initializes jCheckBoxMenuItem5	
	 * 	
	 * @return javax.swing.JCheckBoxMenuItem	
	 */
	private JCheckBoxMenuItem getJCheckBoxMenuItem5() {
		if (jCheckBoxMenuItem5 == null) {
			jCheckBoxMenuItem5 = new JCheckBoxMenuItem();
			jCheckBoxMenuItem5.setText("DFS MCN DSTB");
			jCheckBoxMenuItem5.addItemListener(new java.awt.event.ItemListener() {
				public void itemStateChanged(java.awt.event.ItemEvent e) {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						System.out.println("switching to DFS MCN DSTB traversal mode");
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN_DSTB;
						if (maker != null) {
							maker.displayOCOP();
						}
					}
				}
			});
			if (Heuristics.CURRENT_TREE_BUILD_METHOD == Heuristics.DFS_MCN_DSTB) {
				jCheckBoxMenuItem5.setSelected(true);
			}
		}
		return jCheckBoxMenuItem5;
	}

	/**
	 * This method initializes jCheckBoxMenuItem6	
	 * 	
	 * @return javax.swing.JCheckBoxMenuItem	
	 */
	private JCheckBoxMenuItem getJCheckBoxMenuItem6() {
		if (jCheckBoxMenuItem6 == null) {
			jCheckBoxMenuItem6 = new JCheckBoxMenuItem();
			jCheckBoxMenuItem6.setText("DFS MCN BEC");
			jCheckBoxMenuItem6.addItemListener(new java.awt.event.ItemListener() {
				public void itemStateChanged(java.awt.event.ItemEvent e) {
					if (e.getStateChange() == ItemEvent.SELECTED) {
						System.out.println("switching to DFS MCN BEC traversal mode");
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN_BEC;
						if (maker != null) {
							maker.displayOCOP();
						}
					}
				}
			});
			if (Heuristics.CURRENT_TREE_BUILD_METHOD == Heuristics.DFS_MCN_BEC) {
				jCheckBoxMenuItem6.setSelected(true);
			}
		}
		return jCheckBoxMenuItem6;
	}

	/**
	 * Main method for testing
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		MeetingExplorer test = new MeetingExplorer();
		test.pack();
		test.setVisible(true);
	}

} //  @jve:decl-index=0:visual-constraint="10,10"
