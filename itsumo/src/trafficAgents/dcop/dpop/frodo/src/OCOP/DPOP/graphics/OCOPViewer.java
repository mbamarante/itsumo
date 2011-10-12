/*
 * Created on Feb 22, 2005
 */
package frodo.OCOP.DPOP.graphics;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.geom.Rectangle2D;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import org.jgraph.JGraph;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphModel;
import org.jgraph.layout.AnnealingLayoutAlgorithm;
import org.jgraph.layout.GEMLayoutAlgorithm;
import org.jgraph.layout.JGraphLayoutAlgorithm;
import org.jgraph.layout.MoenLayoutAlgorithm;
import org.jgraph.layout.SpringEmbeddedLayoutAlgorithm;
import org.jgraph.layout.TreeLayoutAlgorithm;

import frodo.COP.OCOPProblemFactory;
import frodo.CSP.BinaryConstraint;
import frodo.OCOP.DFSNode;
import frodo.OCOP.OCOPAbstractProblem;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.DFSStatistics;

//import frodo.OCOP.DPOP.OCOPNode;
//import frodo.OCOP.DPOP.OCOPProblem;

public class OCOPViewer extends JFrame implements ActionListener {

	//	JFrame frame;

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	JGraph graph;

	GraphModel model;

	DefaultGraphCell[] cells;

	SpringEmbeddedLayoutAlgorithm sela;

	MoenLayoutAlgorithm mla;

	GEMLayoutAlgorithm tla;

	/* Code modified by gushakov */
	TreeLayoutAlgorithm treeLayoutAlgm;

	Object[] backEdges;

	DefaultGraphCell root;

	JMenuBar menubar;

	/*----------------------------------------*/

	JTextArea message;

	final int numButtons = 4;

	JRadioButton[] radioButtons = new JRadioButton[numButtons];

	int layoutMethod = 3;

	public OCOPViewer(OCOPAbstractProblem oap) {
		// Construct Model and Graph
		model = new DefaultGraphModel();
		graph = new JGraph(model);

		// Enable edit without final RETURN keystroke
		graph.setInvokesStopCellEditing(true);

		// When over a cell, jump to its default port (we only have one, anyway)
		graph.setJumpToDefaultPort(true);

		int vars = oap.getNumberOfVariables();
		int constr = oap.getNumberOfConstraints();
		int cellCount = vars + constr;

		// Insert all three cells in one call, so we need an array to store them
		cells = new DefaultGraphCell[cellCount];

		for (int i = 0; i < vars; i++) {
			DFSNode curnode = (DFSNode) oap.getNodeByIndex(i);
			//			System.out.println("Adding X" + oap.getVariable(i).getID());

			if (curnode.getDFSData().getPseudochildren().getSize() > 0) {
				//				cells[i] = createVertex("X" + oap.getVariable(i).getID(), 140, 140, 40, 20, Color.RED, true);
				/* Code modified by gushakov */
				if (oap.getVariable(i).getId().getLabel() == null) {
					cells[i] = createVertex("" + oap.getVariable(i).getId(), 140, 140, 40, 20, Color.RED, true);
				} else {
					String label = oap.getVariable(i).getId().getLabel() + " (" + oap.getVariable(i).getId() + ")";
					cells[i] = createVertex("" + label, 140, 140, 10 * label.length(), 20, Color.RED, true);
				}
				/*----------------------------------------*/
			} else {
				//				cells[i] = createVertex("X" + oap.getVariable(i).getID(), 140, 140, 40, 20, Color.GREEN, true);
				/* Code modified by gushakov */

				/*----------------------------------------*/
				if (oap.getVariable(i).getId().getLabel() == null) {
					cells[i] = createVertex("" + oap.getVariable(i).getId(), 140, 140, 40, 20, Color.GREEN, true);
				} else {
					String label = oap.getVariable(i).getId().getLabel() + " (" + oap.getVariable(i).getId() + ")";
					cells[i] = createVertex("" + label, 140, 140, 10 * label.length(), 20, Color.GREEN, true);
				}
				/*----------------------------------------*/
			}

			/* Code modified by gushakov */
			if (curnode.getDFSData().getParent() == null) {
				root = cells[i];
			}
			/*----------------------------------------*/

		}

		/* Code modified by gushakov */
		Vector pseudoEdges = new Vector();
		//-------------------------------------------------

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
			System.out.println("Got a " + bc.getType() + " " + srcNode.getId() + "-" + destNode.getId());
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
			//			GraphConstants.setLineWidth(edge.getAttributes(), 3);
			//			System.out.println("Adding R(" + src + "," + dest + ")");
			if (srcNode.getDFSData().getParentID() == dest) {
				edge.setSource(cells[dest].getChildAt(0));
				edge.setTarget(cells[src].getChildAt(0));
				System.out.println(dest + " parent of " + src + ")");
				//				GraphConstants.setLineStyle(edge.getAttributes(), 0);
				GraphConstants.setLineColor(edge.getAttributes(), Color.BLACK);

			} else if (destNode.getDFSData().getParentID() == src) {
				edge.setSource(cells[src].getChildAt(0));
				edge.setTarget(cells[dest].getChildAt(0));
				System.out.println(dest + " child of " + src + ")");
				//				GraphConstants.setLineStyle(edge.getAttributes(), 1);
				GraphConstants.setLineColor(edge.getAttributes(), Color.BLACK);

			} else if (destNode.getDFSData().getPseudoparents().containsNeighbor(srcNode.getId())) {

				edge.setSource(cells[src].getChildAt(0));
				edge.setTarget(cells[dest].getChildAt(0));

				System.out.println(dest + " pseudochild of " + src + ")");
				//				GraphConstants.setLineStyle(edge.getAttributes(), 0);
				GraphConstants.setLineColor(edge.getAttributes(), Color.RED);

				/* Code modified by gushakov */
				pseudoEdges.add(edge);
				/*----------------------------------------*/

			} else if (srcNode.getDFSData().getPseudoparents().containsNeighbor(destNode.getId())) {

				edge.setSource(cells[dest].getChildAt(0));
				edge.setTarget(cells[src].getChildAt(0));

				//				edge.setAttributes();
				//				GraphConstants.setLineStyle(edge.getAttributes(), 3);

				System.out.println(dest + " pseudoparent of " + src + ")");
				GraphConstants.setLineColor(edge.getAttributes(), Color.RED);

				/* Code modified by gushakov */
				pseudoEdges.add(edge);
				/*----------------------------------------*/

			} else {
				if (srcNode.getId() == destNode.getId())
					System.out.println("Unary constraint on X" + src);
				else
					System.out.println(src + "  problem " + dest + ")");
			}
			cells[vars + i] = edge;

			// Set Arrow Style for edge
			//			int arrow = GraphConstants.ARROW_NONE;
		}

		/* Code modified by gushakov */
		backEdges = pseudoEdges.toArray();

		/*----------------------------------------*/

		// Insert the cells via the cache, so they get selected
		graph.getGraphLayoutCache().insert(cells);

		sela = new SpringEmbeddedLayoutAlgorithm();
		sela.setFrame(new Rectangle(800, 600));
		mla = new MoenLayoutAlgorithm();
		tla = new GEMLayoutAlgorithm(new AnnealingLayoutAlgorithm());
		//		sela.run(graph, cells, null);

		/*
		 * Code modified by George Ushakov 25 avr. 2005
		 */
		treeLayoutAlgm = new TreeLayoutAlgorithm();

		//----------------------------------------------

		// Show in Frame
		//		frame = new JFrame();
		addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent evt) {
				//exitForm(evt);
				//				frame.dispose();
				System.exit(0);
			}
		});

		JPanel butt_panel = new JPanel();
		butt_panel.setLayout(new BoxLayout(butt_panel, BoxLayout.X_AXIS));

		JButton startBut = new JButton("Start");
		startBut.setActionCommand("Start");
		startBut.addActionListener(this);
		startBut.setMnemonic(KeyEvent.VK_T);
		butt_panel.add(startBut);
		JButton exitb = new JButton("Exit");
		exitb.setActionCommand("Exit");
		exitb.addActionListener(this);
		exitb.setMnemonic(KeyEvent.VK_X);
		butt_panel.add(exitb);

		JPanel radioPanel = new JPanel();
		JLabel scaleLabel = new JLabel("Layout:");
		scaleLabel.setForeground(null);
		scaleLabel.setBackground(null);
		radioPanel.add(scaleLabel);
		radioPanel.setBackground(null);
		radioPanel.setForeground(null);
		ButtonGroup magnGroup = new ButtonGroup();
		for (int i = 0; i < numButtons; i++) {
			System.out.println("adding radio button " + i);
			radioButtons[i] = new JRadioButton(String.valueOf(i));
			radioButtons[i].setActionCommand(String.valueOf(i));
			radioButtons[i].addActionListener(this);
			radioButtons[i].setBackground(null);
			radioButtons[i].setForeground(null);
			magnGroup.add(radioButtons[i]);
			radioPanel.add(radioButtons[i]);
		}
		radioButtons[3].setSelected(true);

		butt_panel.add(radioPanel);

		message = new JTextArea("System message");
		message.setLineWrap(true);
		message.setEditable(false);
		JScrollPane jsp_msg = new JScrollPane(message);
		jsp_msg.setMaximumSize(new Dimension(1200, 40));
		//		jsp_msg.add(message);
		//		message.setPreferredSize(new Dimension(800,0));

		JPanel upperPanel = new JPanel();
		upperPanel.setLayout(new BoxLayout(upperPanel, BoxLayout.Y_AXIS));
		upperPanel.add(butt_panel);
		upperPanel.add(jsp_msg);

		JPanel wholePanel = new JPanel();
		wholePanel.setLayout(new BoxLayout(wholePanel, BoxLayout.Y_AXIS));

		JScrollPane jsp = new JScrollPane(graph);
		jsp.setPreferredSize(new Dimension(1000, 800));
		wholePanel.add(upperPanel);
		wholePanel.add(jsp);

		getContentPane().add(wholePanel);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		pack();
		setVisible(true);
	}

	//	-----------------------------------------------------------------------------------------------------------------------//
	//	---------------------------------------------------------------------------------//
	public void actionPerformed(java.awt.event.ActionEvent actionEvent) {
		String command = actionEvent.getActionCommand();

		if (command.equalsIgnoreCase("Exit")) {
			System.exit(0);
		} else if (command.equalsIgnoreCase("start")) {
			//			mySolver.solve(myProblem, 0);
			//			sela.setAllowedToRun(!sela.isAllowedToRun());

			sela.setFrame(new Rectangle(800, 600));
			switch (layoutMethod) {
			case 0:
				SpringEmbeddedLayoutAlgorithm.applyLayout(this.graph, sela, cells);
				break;
			case 1:
				MoenLayoutAlgorithm.applyLayout(this.graph, mla, cells);
				break;
			case 2:
				JGraphLayoutAlgorithm.applyLayout(this.graph, tla, cells);
				break;
			/*
			 *   Code modified by George Ushakov 25 avr. 2005
			 */
			case 3:
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
				break;
			/*-----------------------------------------------------*/
			}
		} else {
			layoutMethod = Integer.parseInt(command);
			System.out.println("Layout " + layoutMethod);
			//			System.exit(1);
		}

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

	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("Usage: OCOPViewer problem_file.txt");
			return;
		}
		//		OCOPAbstractProblem problem = new OCOPAbstractProblem();
		OCOPAbstractProblem problem = OCOPProblemFactory.loadFromTextFile(args[0], new DFSNode(null, 0, null));
		OCOPAbstractProblem problem1 = null;
		System.out.println("Loaded problem from " + args[0]);
		if (problem.hasMultipleVarsPerNode()) {
			System.out.println("This problem has more variables per node: ");
			problem1 = OCOPProblemFactory.toSingleVariablePerNode(problem);
			System.out.println(problem1.toStringSerialize());
		} else
			problem1 = problem;
		//		problem.buildDFS((OCOPNode) problem.getNodeByIndex(2), null);
		DFSMaster.getMinimalWidthDFS_MCN(problem1);
		DFSStatistics statistix = DFSMaster.getStatisticsOfDFS(problem1);
		OCOPViewer jgv = new OCOPViewer(problem1);
		jgv.setTitle(args[0]);
		jgv.message.setText("Variables=" + problem1.getNumberOfVariables() + "\tConstraints=" + problem1.getNumberOfConstraints() + "\nRoot=X" + statistix.getRoot() + " ->width="
				+ statistix.getWidth() + "\nDFS traversal:\n" + DFSMaster.getCurrentDFSTraversalString(problem1) + ":");
	}

	/**
	 * gushakov Convenience method for cloning the backedges of the graph.
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

}