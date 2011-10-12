/*
 * Created on Jul 4, 2005
 */
package frodo.interaction.meeting.graph;

/**
 * This viewer/editor is an extension of a free (GNU public licence) 
 * software distributed as an example with JGraph library. For more
 * information @see org.jgraph.example.GraphEd.
 * 
 * This class is a substitute for OPCPViewer design for interaction
 * with and statistics gathering from the pseudotree graph of the
 * meeting scheduling program.
 * 
 * @author ushakov
 * 
 *  Some of the icons used in the GUI for this class are available from
 *	Jonathan Bingham at http://www.codeguru.com/java/articles/656.shtml
 * 
 */

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Rectangle2D;
import java.util.Vector;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPopupMenu;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.border.Border;

import org.jgraph.event.GraphSelectionEvent;
import org.jgraph.event.GraphSelectionListener;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphConstants;
import org.jgraph.layout.JGraphLayoutAlgorithm;
import org.jgraph.layout.TreeLayoutAlgorithm;

import frodo.CSP.BinaryConstraint;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPBasicProblem;
import frodo.OCOP.DPOP.graphics.GraphEd;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.DFSStatistics;
import frodo.interaction.meeting.roundtrip.DPOPRunner;
import frodo.monitoring.interfaces.MonitorListener;
import frodo.monitoring.interfaces.OCOPModelEvent;
import frodo.monitoring.interfaces.OCOPModelListener;
import frodo.monitoring.interfaces.OCOPSelectionEvent;

public class PseudoTreeEditor extends GraphEd implements ActionListener, MonitorListener, OCOPModelListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	DefaultGraphModel model;

	Object[] cells;

	TreeLayoutAlgorithm treeLayoutAlgm;

	Object[] backEdges;

	DefaultGraphCell root;

	JTextArea message;

	int[] flaggedLink;

	OCOPBasicProblem ocop;

	Vector cellsVector;

	Vector pseudoEdges;

	//---------------------------------------------------------------------
	// GUI components.

	JFrame mainFrame;

	JToolBar toolbar;

	JToggleButton jtbHighlight;

	JToggleButton jtbHide;

	JScrollPane scrollPane;

	//---------------------------------------------------------------------

	private OCOPSelectionEvent savedSelectionEvent = null;

	//	private int oldTraversalMethod = Heuristics.CURRENT_TREE_BUILD_METHOD;
	//
	//	private int oldDFSRootId = Heuristics.CURRENT_DFS_ROOT_ID;

	//----------------------------------------------------------------------

	/**
	 * Custom graph selection listener for reflecting the selected node
	 * in the meeting explorer, possibly. 
	 */
	class SelectionChangeListener implements GraphSelectionListener {
		/*
		 * (non-Javadoc)
		 * 
		 * @see org.jgraph.event.GraphSelectionListener#valueChanged(org.jgraph.event.GraphSelectionEvent)
		 */
		public void valueChanged(GraphSelectionEvent event) {
		}
	}

	/**
	 * Overloads the constractor from GraphEd to transform it
	 * into an instance of the JFrame that will be called from
	 * MeetingExplorer.
	 * 
	 * @param oap An instance of the centralized OCOPProblem. 
	 */
	public PseudoTreeEditor(OCOPBasicProblem ocop) {
		super();

		this.ocop = ocop;

		//Get the handle on the contains of the GraphEd applet.
		mainFrame = new JFrame("Pseudotree Viewer, JGraph");

		mainFrame.getContentPane().add(super.getContentPane());

		// Get the handle of the graph model.
		model = (DefaultGraphModel) graph.getModel();

		// Get the handle on the toolbar
		Component[] components = super.getContentPane().getComponents();
		for (int i = 0; i < components.length; i++) {
			if (components[i] instanceof JToolBar) {
				toolbar = (JToolBar) components[i];
			}
		}

		//------------------------------------------------------------------------
		// Initialize GUI.

		toolbar.addSeparator();

		JButton jbLayout = new JButton();
		jbLayout.setIcon(new ImageIcon(getClass().getResource("/frodo/resources/icons/layout.gif")));
		jbLayout.setActionCommand("Layout");
		jbLayout.addActionListener(this);
		toolbar.add(jbLayout);

		jtbHighlight = new JToggleButton();
		jtbHighlight.setIcon(new ImageIcon(getClass().getResource("/frodo/resources/icons/highlight.gif")));
		jtbHighlight.setActionCommand("Highlight");
		jtbHighlight.addActionListener(this);
		jtbHighlight.setSelected(false);
		toolbar.add(jtbHighlight);

		jtbHide = new JToggleButton();
		jtbHide.setIcon(new ImageIcon(getClass().getResource("/frodo/resources/icons/mask.gif")));
		jtbHide.setActionCommand("Hide");
		jtbHide.addActionListener(this);
		jtbHide.setSelected(false);
		toolbar.add(jtbHide);

		JButton jbRandomize = new JButton();
		jbRandomize.setIcon(new ImageIcon(getClass().getResource("/frodo/resources/icons/lightning.gif")));
		jbRandomize.setActionCommand("Randomize");
		jbRandomize.addActionListener(this);
		toolbar.add(jbRandomize);

		JButton runBut = new JButton();
		runBut.setIcon(new ImageIcon(getClass().getResource("/frodo/resources/icons/run.gif")));
		runBut.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 10));
		runBut.setActionCommand("Run");
		runBut.addActionListener(this);
		toolbar.add(runBut);

		message = new JTextArea("System message:");
		message.setFont(new java.awt.Font("Dialog", java.awt.Font.PLAIN, 10));
		message.setLineWrap(true);
		message.setEditable(false);
		scrollPane = new JScrollPane(message);
		scrollPane.setPreferredSize(new Dimension(100, 50));
		mainFrame.getContentPane().add(scrollPane, BorderLayout.SOUTH);

		mainFrame.setSize(700, 400);
		mainFrame.setLocation(500, 0);

		//---------------------------------------------------------------------------
		// Interacting with the graph.

		graph.addGraphSelectionListener(new SelectionChangeListener());

		//---------------------------------------------------------------------------

		//Note the first time the DFS MCN heuristic is used.
		makeGraph();
		//---------------------------------------------------------------------------

		mainFrame.setVisible(true);
	}

	/* (non-Javadoc)
	 * @see org.jgraph.example.GraphEd#createPopupMenu(java.awt.Point, java.lang.Object)
	 */
	public JPopupMenu createPopupMenu(final Point pt, final Object cell) {
		JPopupMenu popup = super.createPopupMenu(pt, cell);
		if (cell instanceof DefaultEdge) {
			popup.addSeparator();
			popup.add(new AbstractAction("Flag") {
				private static final long serialVersionUID = 1L;

				/*
				 * (non-Javadoc)
				 * 
				 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
				 */
				public void actionPerformed(ActionEvent arg0) {
					/*
					 * Mark this edge for investigation of the dimension of the
					 * messages passed through the link represented by this
					 * edge.
					 */
					DefaultGraphCell senderCell = (DefaultGraphCell) ((DefaultPort) ((DefaultEdge) cell).getSource()).getParent();
					DefaultGraphCell receiverCell = (DefaultGraphCell) ((DefaultPort) ((DefaultEdge) cell).getTarget()).getParent();
					flaggedLink = new int[2];
					flaggedLink[0] = getNodeId(senderCell);
					flaggedLink[1] = getNodeId(receiverCell);
					displayMessage("Flagged edge: " + flaggedLink[0] + "<->" + flaggedLink[1]);

				}
			});
		} else if (cell instanceof DefaultGraphCell) {
			popup.addSeparator();
			popup.add(new AbstractAction("Set As Root") {
				private static final long serialVersionUID = 1L;

				/*
				 * (non-Javadoc)
				 * 
				 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
				 */
				public void actionPerformed(ActionEvent arg0) {
					int nodeId = getNodeId((DefaultGraphCell) cell);
					if (Heuristics.CURRENT_TREE_BUILD_METHOD == Heuristics.DFS) {
						if (Heuristics.CURRENT_DFS_ROOT_ID != nodeId) {
							displayMessage("setting this node as a root: " + nodeId);
							Heuristics.CURRENT_DFS_ROOT_ID = nodeId;
							recreate();
						}
					} else {
						displayMessage("please switch to DFS mode first");
					}
				}
			});
			popup.addSeparator();
			popup.add(new AbstractAction("Get HC Size") {
				private static final long serialVersionUID = 1L;

				/* (non-Javadoc)
				 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
				 */
				public void actionPerformed(ActionEvent arg0) {
					int nodeId = getNodeId((DefaultGraphCell) cell);
					//					long hcSize = GeorgesHeuristics.getHypercubeSize(ocop, ocop.getNodeById(nodeId));
					long hcSize = ((DFSNode) ocop.getNodeById(nodeId)).getDFSData().getUTIL_SIZE();
					displayMessage("UTIL hypercube size from " + nodeId + " is " + hcSize);
				}
			});
		}
		return popup;
	}

	public void makeGraph() {

		Heuristics.buildPseudoTree(ocop);
		DFSStatistics statistics = DFSMaster.getStatisticsOfDFS(ocop);
		DPOPRunner.setInfo(statistics);

		displayMessage("------------------------------------");
		displayMessage("Problem parameters:");
		displayMessage("heuristics: " + Heuristics.getCurrentHeuristic());
		displayMessage("DFS Statistics: " + statistics.toString());
		displayMessage("traversal: " + DFSMaster.getCurrentDFSTraversalString(ocop));
		displayMessage("------------------------------------");

		int vars = ocop.getNumberOfVariables();
		int constr = ocop.getNumberOfConstraints();

		cellsVector = new Vector();

		for (int i = 0; i < vars; i++) {
			DFSNode curnode = (DFSNode) ocop.getNodeByIndex(i);

			if (curnode.getDFSData().getPseudochildren().getSize() > 0) {
				String label = ocop.getVariable(i).getId().getLabel() + " (" + ocop.getVariable(i).getId() + ")";
				cellsVector.add(createVertex("" + label, 140, 140, 10 * label.length(), 20, Color.red, false));
			} else {
				String label = ocop.getVariable(i).getId().getLabel() + " (" + ocop.getVariable(i).getId() + ")";
				cellsVector.add(createVertex("" + label, 140, 140, 10 * label.length(), 20, Color.green, false));
			}

			if (curnode.getDFSData().getParent() == null) {
				root = (DefaultGraphCell) cellsVector.elementAt(i);
			}
		}

		pseudoEdges = new Vector();

		for (int i = 0; i < constr; i++) {
			// Create Edge
			DefaultEdge edge = createDefaultEdge();
			BinaryConstraint bc = ocop.getConstraint(i);
			setEdge(edge, bc);
			if (bc.getArity() == 2) {
				cellsVector.add(edge);
			}
		}

		cells = cellsVector.toArray();

		backEdges = pseudoEdges.toArray();

		// Insert the cells via the cache, so they get selected
		graph.getGraphLayoutCache().insert(cells);

		treeLayoutAlgm = new TreeLayoutAlgorithm();

		applyTreeLayout();

	}

	/*
	 * This method determines the type of the edge and connects
	 * it to the approptiate cells in the graph.
	 */
	private void setEdge(DefaultEdge edge, BinaryConstraint bc) {
		GraphConstants.setEndFill(edge.getAttributes(), true);
		int src = bc.getSourceId().getAgentID();
		int dest = bc.getDestinationId().getAgentID();
		DFSNode srcNode = (DFSNode) ocop.getNodeById(src);
		DFSNode destNode = (DFSNode) ocop.getNodeById(dest);
		switch (bc.getType()) {
		case '#':
			GraphConstants.setLineWidth(edge.getAttributes(), 4);
			GraphConstants.setLineEnd(edge.getAttributes(), GraphConstants.ARROW_DIAMOND);
			break;
		case '=':
			GraphConstants.setLineWidth(edge.getAttributes(), 2);
			GraphConstants.setLineEnd(edge.getAttributes(), GraphConstants.ARROW_CLASSIC);
			break;
		default:
			GraphConstants.setLineWidth(edge.getAttributes(), 1);
			GraphConstants.setLineEnd(edge.getAttributes(), GraphConstants.ARROW_CIRCLE);
		}

		if (srcNode.getId() != destNode.getId()) {
			if (srcNode.getDFSData().getParentID() == dest) {
				edge.setSource(((DefaultGraphCell) cellsVector.get(dest)).getChildAt(0));
				edge.setTarget(((DefaultGraphCell) cellsVector.get(src)).getChildAt(0));
				GraphConstants.setLineColor(edge.getAttributes(), Color.BLACK);

			} else if (destNode.getDFSData().getParentID() == src) {
				edge.setSource(((DefaultGraphCell) cellsVector.get(src)).getChildAt(0));
				edge.setTarget(((DefaultGraphCell) cellsVector.get(dest)).getChildAt(0));
				GraphConstants.setLineColor(edge.getAttributes(), Color.BLACK);

			} else if (destNode.getDFSData().getPseudoparents().containsNeighbor(srcNode.getId())) {

				edge.setSource(((DefaultGraphCell) cellsVector.get(src)).getChildAt(0));
				edge.setTarget(((DefaultGraphCell) cellsVector.get(dest)).getChildAt(0));
				GraphConstants.setLineColor(edge.getAttributes(), Color.RED);
				pseudoEdges.add(edge);

			} else if (srcNode.getDFSData().getPseudoparents().containsNeighbor(destNode.getId())) {

				edge.setSource(((DefaultGraphCell) cellsVector.get(dest)).getChildAt(0));
				edge.setTarget(((DefaultGraphCell) cellsVector.get(src)).getChildAt(0));
				GraphConstants.setLineColor(edge.getAttributes(), Color.RED);
				pseudoEdges.add(edge);
			}
		}
	}

	public void closeWindow() {
		System.out.println("closing CSP viewer");
		mainFrame.setVisible(false);
		mainFrame.dispose();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	public void actionPerformed(java.awt.event.ActionEvent actionEvent) {
		String command = actionEvent.getActionCommand();
		if (command.equalsIgnoreCase("Layout")) {
			JFrame jFrame = new JFrame();
			JDialog settingsDiag = JGraphLayoutAlgorithm.createDialog(treeLayoutAlgm.createSettings(), jFrame, "Settings", "Close", "Apply");
			settingsDiag.setSize(200, 200);
			settingsDiag.setVisible(true);
			applyTreeLayout();
		} else if (command.equalsIgnoreCase("Highlight")) {
			if (jtbHighlight.isSelected() == false) {
				clearHighlights();
			} else {
				//Process a pending event.
				if (savedSelectionEvent != null) {
					highlightChanged(savedSelectionEvent);
				}
			}
		} else if (command.equalsIgnoreCase("Hide")) {
			if (jtbHide.isSelected()) {
				showPseudoEdges(false);
			} else {
				showPseudoEdges(true);
			}
		} else if (command.equalsIgnoreCase("Randomize")) {
			DFSMaster.randomizeNodeOrder(ocop);
			recreate();
		} else if (command.equalsIgnoreCase("Run")) {
			displayMessage("running the problem instance...");
			DPOPRunner.runDPOP(ocop, this);
		}
	}

	public int getNodeId(DefaultGraphCell cell) {
		int id = -1;
		if (!(cell instanceof DefaultEdge)) {
			String label = graph.convertValueToString(cell);
			String idString = label.substring(label.indexOf("(") + 1, label.lastIndexOf(")"));
			id = (int) Double.parseDouble(idString);
		}
		return id;
	}

	public void applyTreeLayout() {
		if (ocop.getNumberOfNodes() > 0) {
			System.out.println("[Applying tree layout]");

			Object[] backEdgesCopy = copyEdges(backEdges);

			graph.getGraphLayoutCache().remove(backEdges);

			//	Root should be somewhere on the top and in the middle of the
			// screen.
			Rectangle2D bounds = (Rectangle2D) GraphConstants.getBounds(root.getAttributes()).clone();

			// Modify the position of the root by the scale of 
			// the current view.

			double scale = graph.getScale();
			System.out.println("using scale " + scale);

			bounds.setRect(mainFrame.getBounds().getWidth() / (2 * scale), bounds.getY(), bounds.getWidth(), bounds.getHeight());

			GraphConstants.setBounds(root.getAttributes(), bounds);

			Object[] rootHolder = new Object[1];
			rootHolder[0] = root;

			TreeLayoutAlgorithm.applyLayout(graph, treeLayoutAlgm, rootHolder);

			for (int i = 0; i < backEdges.length; i++) {
				((DefaultEdge) backEdges[i]).setSource(((DefaultEdge) backEdgesCopy[i]).getSource());
				((DefaultEdge) backEdges[i]).setTarget(((DefaultEdge) backEdgesCopy[i]).getTarget());
			}

			graph.getGraphLayoutCache().insert(backEdges);

			graph.clearSelection();
		}
	}

	public void highlightCell(DefaultGraphCell cell, Color color) {
		DefaultGraphCell[] holder = { cell };

		Border newBorder = BorderFactory.createLineBorder(color, 3);

		GraphConstants.setBorder(cell.getAttributes(), newBorder);
		model.cellsChanged(holder);
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	public DefaultGraphCell createVertex(String name, double x, double y, double w, double h, Color bg, boolean raised) {

		// Create vertex with the given name, uses the method from super.
		DefaultGraphCell cell = createDefaultGraphCell();
		// Overide the default label with the specified name.
		GraphConstants.setValue(cell.getAttributes(), name);

		// Set bounds
		GraphConstants.setBounds(cell.getAttributes(), new Rectangle2D.Double(x, y, w, h));

		// Set fill color
		if (bg != null) {
			GraphConstants.setGradientColor(cell.getAttributes(), bg);
			//GraphConstants.setBackground(cell.getAttributes(), bg);
			GraphConstants.setOpaque(cell.getAttributes(), true);
		}

		// Set raised border
		if (raised) {
			GraphConstants.setBorder(cell.getAttributes(), BorderFactory.createRaisedBevelBorder());
		} else {
			// Set black border
			GraphConstants.setBorder(cell.getAttributes(), BorderFactory.createLineBorder(Color.black, 1));
		}

		return cell;
	}

	public int[] getFlaggedLink() {
		return flaggedLink;
	}

	/**
	 * Convenience method for cloning the backedges of the graph.
	 * @param edges array of type DefaultEdge
	 */
	private Object[] copyEdges(Object[] edges) {

		Object[] copyEdges = new Object[edges.length];

		for (int i = 0; i < edges.length; i++) {
			DefaultEdge copyFromEdge = (DefaultEdge) edges[i];
			DefaultEdge copyToEdge = (DefaultEdge) copyFromEdge.clone();
			copyToEdge.setSource(copyFromEdge.getSource());
			copyToEdge.setTarget(copyFromEdge.getTarget());
			copyEdges[i] = copyToEdge;
		}
		return copyEdges;
	}

	public void setUp() {
	}

	public void updateStatistics(Object[] parameterValue) {
	}

	public void displayMessage(String msg) {
		message.append("\n" + msg);
		// Scroll aoutomatically the text area.
		JScrollBar bar = scrollPane.getVerticalScrollBar();
		Rectangle visible = (Rectangle) scrollPane.getVisibleRect().clone();
		scrollPane.getViewport().setViewPosition(new Point((int) visible.getWidth(), bar.getMaximum() * bar.getUnitIncrement()));
	}

	public void recreate() {
		model.remove(graph.getDescendants(cells));
		makeGraph();
		mainFrame.setVisible(true);
	}

	//---------------------------------------------------------------------------------
	public DefaultGraphCell findCell(int id) {
		DefaultGraphCell cell = null;
		//The cells have variable ids in the format (1.0)
		String idStr = "(" + id + ".";
		int i = 0;
		boolean found = false;
		while (i < cells.length && !found) {
			DefaultGraphCell nextCell = (DefaultGraphCell) cells[i];
			String cellLabel = GraphConstants.getValue(nextCell.getAttributes()).toString();
			if (cellLabel.indexOf(idStr) >= 0) {
				cell = nextCell;
				found = true;
			}
			i++;
		}
		return cell;
	}

	//---------------------------------------------------------------------------------
	public void clearHighlights() {
		for (int i = 0; i < cells.length; i++) {
			DefaultGraphCell cell = (DefaultGraphCell) cells[i];
			GraphConstants.setBorder(cell.getAttributes(), BorderFactory.createLineBorder(Color.black, 1));
		}
		model.cellsChanged(cells);
	}

	//---------------------------------------------------------------------------------
	public void showPseudoEdges(boolean visible) {
		for (int i = 0; i < backEdges.length; i++) {
			DefaultGraphCell edge = (DefaultGraphCell) backEdges[i];
			if (visible) {
				// Note, setOpaque() does not seem to work for the edges,
				// so there is a little work around.
				GraphConstants.setLineColor(edge.getAttributes(), Color.red);
			} else {
				Color background = graph.getBackground();
				GraphConstants.setLineColor(edge.getAttributes(), background);
			}
		}
		model.cellsChanged(backEdges);
		if (visible) {
			graph.getGraphLayoutCache().toFront(backEdges);
		} else {
			graph.getGraphLayoutCache().toBack(backEdges);
		}
	}

	// ----------------------------------------------------------------------------

	public void ocopModelChanged(OCOPModelEvent event) {
		ocop = event.getProblem();
		//Remake the graph.
		recreate();
	}

	//---------------------------------------------------------------------------------
	public void highlightChanged(OCOPSelectionEvent event) {
		//Save this event.
		savedSelectionEvent = event;
		if (jtbHighlight.isSelected()) {
			int type = event.getType();
			int[] ids = event.getIds();
			Object[] changed = null;

			if (type == OCOPSelectionEvent.NEW_SELECTION_MEETING || type == OCOPSelectionEvent.NEW_SELECTION_PARTICIPANT) {
				clearHighlights();
				changed = new Object[ids.length];
				for (int i = 0; i < ids.length; i++) {
					DefaultGraphCell cell = findCell(ids[i]);
					//Highlight new nodes.
					if (type == OCOPSelectionEvent.NEW_SELECTION_MEETING) {
						GraphConstants.setBorder(cell.getAttributes(), BorderFactory.createLineBorder(Color.blue, 3));
					} else if (type == OCOPSelectionEvent.NEW_SELECTION_PARTICIPANT) {
						GraphConstants.setBorder(cell.getAttributes(), BorderFactory.createLineBorder(Color.cyan, 3));
					}
					changed[i] = cell;
				}
				model.cellsChanged(changed);
			} else if (type == OCOPSelectionEvent.SELECTION_CLEARED) {
				clearHighlights();
			}
		}
	}
	//---------------------------------------------------------------------------------
}