/*
 * Created on Jul 4, 2005
 */
package frodo.interaction.meeting.graph;

/**
 * From OCOPViewer
 * 
 * @author ushakov
 * 
 *  Some of the icons used in the GUI for this class are available from
 *	Jonathan Bingham at http://www.codeguru.com/java/articles/656.shtml
 * 
 */

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.geom.Rectangle2D;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;

import org.jgraph.JGraph;
import org.jgraph.event.GraphSelectionEvent;
import org.jgraph.event.GraphSelectionListener;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphModel;
import org.jgraph.layout.JGraphLayoutAlgorithm;
import org.jgraph.layout.TreeLayoutAlgorithm;

import frodo.CSP.BinaryConstraint;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.interaction.meeting.roundtrip.DPOPRunner;
import frodo.monitoring.interfaces.MonitorListener;

public class PseudoTreeViewer extends JFrame implements ActionListener, MonitorListener, KeyListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	JGraph graph;

	GraphModel model;

	Object[] cells;

	TreeLayoutAlgorithm treeLayoutAlgm;

	Object[] backEdges;

	DefaultGraphCell root;

	JTextArea message;

	int[] flaggedLink;

	OCOPAbstractProblem ocop;

	//---------------------------------------------------------------------
	// GUI components.

	JToggleButton toggleNode;

	JToggleButton toggleEdge;

	JToggleButton toggleSelect;

	ButtonGroup groupInsert;

	JButton flagBut = new JButton("Flag");

	class SelectionChangeListener implements GraphSelectionListener {
		/*
		 * (non-Javadoc)
		 * 
		 * @see org.jgraph.event.GraphSelectionListener#valueChanged(org.jgraph.event.GraphSelectionEvent)
		 */
		public void valueChanged(GraphSelectionEvent event) {

			DefaultGraphCell cell = (DefaultGraphCell) event.getCell();

			if (event.isAddedCell(cell)) {
				//A graph cell was added to the selection.
				if (cell instanceof DefaultEdge) {
					// An edge was selected.
					System.out.println("graph edge added");
					flagBut.setEnabled(true);
				} else {
					flagBut.setEnabled(false);
				}
			} else {
				flagBut.setEnabled(false);
			}

		}
	}

	public void keyPressed(KeyEvent arg0) {
		if (arg0.getKeyCode() == KeyEvent.VK_ESCAPE) {
			toggleSelect.setSelected(true);
		}
	}

	public void keyReleased(KeyEvent arg0) {
		// TODO Auto-generated method stub

	}

	public void keyTyped(KeyEvent arg0) {
	}

	public PseudoTreeViewer(OCOPAbstractProblem oap) {
		super();

		ocop = oap;

		this.setTitle("CSP Viewer");
		this.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent evt) {
				closeWindow();
			}
		});

		// Construct Model and Graph
		model = new DefaultGraphModel();
		graph = new JGraph(model);

		// Enable edit without final RETURN keystroke
		graph.setInvokesStopCellEditing(true);

		// When over a cell, jump to its default port (we only have one, anyway)
		graph.setJumpToDefaultPort(true);

		//---------------------------------------------------------------------------
		// Interacting with the graph.

		graph.addGraphSelectionListener(new SelectionChangeListener());
		graph.addKeyListener(this);

		//---------------------------------------------------------------------------

		int vars = oap.getNumberOfVariables();
		int constr = oap.getNumberOfConstraints();

		Vector cellsVector = new Vector();

		for (int i = 0; i < vars; i++) {
			DFSNode curnode = (DFSNode) oap.getNodeByIndex(i);

			if (curnode.getDFSData().getPseudochildren().getSize() > 0) {
				String label = oap.getVariable(i).getId().getLabel() + " (" + oap.getVariable(i).getId() + ")";
				cellsVector.add(createVertex("" + label, 140, 140, 10 * label.length(), 20, Color.RED, true));
			} else {
				String label = oap.getVariable(i).getId().getLabel() + " (" + oap.getVariable(i).getId() + ")";
				cellsVector.add(createVertex("" + label, 140, 140, 10 * label.length(), 20, Color.GREEN, true));
			}

			if (curnode.getDFSData().getParent() == null) {
				root = (DefaultGraphCell) cellsVector.elementAt(i);
			}
		}

		Vector pseudoEdges = new Vector();

		for (int i = 0; i < constr; i++) {
			// Create Edge
			DefaultEdge edge = new DefaultEdge();
			GraphConstants.setEndFill(edge.getAttributes(), true);
			// Fetch the ports from the new vertices, and connect them with the edge
			BinaryConstraint bc = oap.getConstraint(i);
			int src = bc.getSourceId().getAgentID();
			int dest = bc.getDestinationId().getAgentID();
			DFSNode srcNode = (DFSNode) oap.getNodeById(src);
			DFSNode destNode = (DFSNode) oap.getNodeById(dest);
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

				cellsVector.add(edge);
			} else {
				// This is a unary constraint case.
			}
		}

		// Insert all three cells in one call, so we need an array to store them
		cells = cellsVector.toArray();

		backEdges = pseudoEdges.toArray();

		// Insert the cells via the cache, so they get selected
		graph.getGraphLayoutCache().insert(cells);

		treeLayoutAlgm = new TreeLayoutAlgorithm();

		JToolBar toolbar = new JToolBar();
		toolbar.setLayout(new BoxLayout(toolbar, BoxLayout.X_AXIS));

		JButton startBut = new JButton("Layout");
		startBut.setActionCommand("Layout");
		startBut.addActionListener(this);
		startBut.setMnemonic(KeyEvent.VK_L);
		toolbar.add(startBut);

		//flagBut = new JButton("Flag");
		flagBut.setActionCommand("Flag");
		flagBut.addActionListener(this);
		flagBut.setMnemonic(KeyEvent.VK_F);
		flagBut.setEnabled(false);
		toolbar.add(flagBut);

		JButton runBut = new JButton("Run");
		runBut.setActionCommand("Run");
		runBut.addActionListener(this);
		runBut.setMnemonic(KeyEvent.VK_R);
		toolbar.add(runBut);

		toolbar.addSeparator();

		groupInsert = new ButtonGroup();

		toggleSelect = new JToggleButton("Select");
		toggleNode = new JToggleButton("Node");
		toggleEdge = new JToggleButton("Edge");

		groupInsert.add(toggleSelect);
		groupInsert.add(toggleNode);
		groupInsert.add(toggleEdge);

		toolbar.add(toggleSelect);
		toolbar.add(toggleNode);
		toolbar.add(toggleEdge);

		JButton exitb = new JButton("Exit");
		exitb.setActionCommand("Exit");
		exitb.addActionListener(this);
		exitb.setMnemonic(KeyEvent.VK_X);
		toolbar.add(exitb);

		message = new JTextArea("System message");
		message.setLineWrap(true);
		message.setEditable(false);
		message.setPreferredSize(new Dimension(100, 50));
		JScrollPane jsp_msg = new JScrollPane(message);
		jsp_msg.setMaximumSize(new Dimension(1200, 40));

		JPanel upperPanel = new JPanel();
		upperPanel.setLayout(new BoxLayout(upperPanel, BoxLayout.Y_AXIS));
		upperPanel.add(toolbar);
		upperPanel.add(jsp_msg);

		JPanel wholePanel = new JPanel();
		wholePanel.setLayout(new BoxLayout(wholePanel, BoxLayout.Y_AXIS));

		JScrollPane jsp = new JScrollPane(graph);
		jsp.setPreferredSize(new Dimension(1000, 800));
		wholePanel.add(upperPanel);
		wholePanel.add(jsp);
		getContentPane().add(wholePanel);
		//setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		applyTreeLayout();
		pack();
		setVisible(true);
	}

	private void closeWindow() {
		System.out.println("closing CSP viewer");
		this.setVisible(false);
		this.dispose();
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	public void actionPerformed(java.awt.event.ActionEvent actionEvent) {
		String command = actionEvent.getActionCommand();

		if (command.equalsIgnoreCase("Exit")) {
			closeWindow();
		} else if (command.equalsIgnoreCase("Layout")) {
			applyTreeLayout();
		} else if (command.equalsIgnoreCase("Flag")) {
			Object edge = graph.getSelectionCell();
			if (edge != null && edge instanceof DefaultEdge) {
				// Mark this edge for investigation of the width
				DefaultGraphCell senderCell = (DefaultGraphCell) ((DefaultPort) ((DefaultEdge) edge).getSource()).getParent();
				DefaultGraphCell receiverCell = (DefaultGraphCell) ((DefaultPort) ((DefaultEdge) edge).getTarget()).getParent();
				String senderLabel = graph.convertValueToString(senderCell);
				String receiverLabel = graph.convertValueToString(receiverCell);
				// Parse the labels of the nodes to get the agent numbers.
				String senderIdStr = senderLabel.substring(senderLabel.indexOf("(") + 1, senderLabel.lastIndexOf(")"));
				String receiverIdStr = receiverLabel.substring(receiverLabel.indexOf("(") + 1, receiverLabel.lastIndexOf(")"));
				flaggedLink = new int[2];
				flaggedLink[0] = (int) Double.parseDouble(senderIdStr);
				flaggedLink[1] = (int) Double.parseDouble(receiverIdStr);
				message.append("\nFlagged edge: " + flaggedLink[0] + "<->" + flaggedLink[1]);
			}
		} else if (command.equalsIgnoreCase("Run")) {
			DPOPRunner.runDPOP(ocop, this);
		}

	}

	private void applyTreeLayout() {
		System.out.println("[Applying tree layout]");

		treeLayoutAlgm.setNodeDistance(50);
		treeLayoutAlgm.setLevelDistance(30);

		JFrame jFrame = new JFrame();

		JDialog settingsDiag = JGraphLayoutAlgorithm.createDialog(treeLayoutAlgm.createSettings(), jFrame, "Settings", "Close", "Apply");

		settingsDiag.setSize(200, 200);
		settingsDiag.setVisible(true);

		Object[] backEdgesCopy = copyEdges(backEdges);

		graph.getGraphLayoutCache().remove(backEdges);

		Object[] rootHolder = new Object[1];
		rootHolder[0] = root;

		TreeLayoutAlgorithm.applyLayout(graph, treeLayoutAlgm, rootHolder);

		for (int i = 0; i < backEdges.length; i++) {
			((DefaultEdge) backEdges[i]).setSource(((DefaultEdge) backEdgesCopy[i]).getSource());
			((DefaultEdge) backEdges[i]).setTarget(((DefaultEdge) backEdgesCopy[i]).getTarget());
		}

		graph.getGraphLayoutCache().insert(backEdges);

	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	public static DefaultGraphCell createVertex(String name, double x, double y, double w, double h, Color bg, boolean raised) {

		// Create vertex with the given name
		DefaultGraphCell cell = new DefaultGraphCell(name);

		// Set bounds
		GraphConstants.setBounds(cell.getAttributes(), new Rectangle2D.Double(x, y, w, h));

		// Set fill color
		if (bg != null) {
			GraphConstants.setGradientColor(cell.getAttributes(), bg);
			GraphConstants.setOpaque(cell.getAttributes(), true);
		}

		// Set raised border
		if (raised)
			GraphConstants.setBorder(cell.getAttributes(), BorderFactory.createRaisedBevelBorder());
		else
			// Set black border
			GraphConstants.setBorderColor(cell.getAttributes(), Color.black);

		// Add a Port
		DefaultPort port = new DefaultPort();
		cell.add(port);
		port.setParent(cell);

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
		message.append((String) msg);
	}
}