/*
 * @(#)GraphEd.java 3.3 23-APR-04
 * 
 * Copyright (c) 2001-2004, Gaudenz Alder All rights reserved.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 */

/*	Modified by Ushakov.
 *	Some of the icons used in the GUI for this class are available from
 *	Jonathan Bingham at http://www.codeguru.com/java/articles/656.shtml
 */
package frodo.OCOP.DPOP.graphics;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.net.URL;
import java.util.Hashtable;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JApplet;
import javax.swing.JFrame;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.event.UndoableEditEvent;

import org.jgraph.JGraph;
import org.jgraph.event.GraphSelectionEvent;
import org.jgraph.event.GraphSelectionListener;
import org.jgraph.graph.BasicMarqueeHandler;
import org.jgraph.graph.CellHandle;
import org.jgraph.graph.CellView;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.Edge;
import org.jgraph.graph.EdgeView;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphContext;
import org.jgraph.graph.GraphModel;
import org.jgraph.graph.GraphUndoManager;
import org.jgraph.graph.Port;
import org.jgraph.graph.PortView;

import frodo.interaction.meeting.gui.MeetingExplorer;

/**
 * @author apetcu
 * This class provides various graph editing functionality (add/delete/change nodes)
 */
public class GraphEd extends JApplet implements GraphSelectionListener, KeyListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	// JGraph instance
	protected JGraph graph;

	// Undo Manager
	protected GraphUndoManager undoManager;

	// Actions which Change State
	protected Action undo, redo, remove, group, ungroup, tofront, toback, cut, copy, paste;

	// cell count that gets put in cell label
	protected int cellCount = 0;

	//
	// Editor Panel
	//

	// Construct an Editor Panel
	public GraphEd() {
		// Use Border Layout
		getContentPane().setLayout(new BorderLayout());
		// Construct the Graph
		graph = createGraph();
		// Use a Custom Marquee Handler
		graph.setMarqueeHandler(new MyMarqueeHandler());

		// Construct Command History
		//
		// Create a GraphUndoManager which also Updates the ToolBar
		undoManager = new GraphUndoManager() {
			/**
			 * 
			 */
			private static final long serialVersionUID = -7613893565730650414L;

			// Override Superclass
			public void undoableEditHappened(UndoableEditEvent e) {
				// First Invoke Superclass
				super.undoableEditHappened(e);
				// Then Update Undo/Redo Buttons
				updateHistoryButtons();
			}
		};

		// Add Listeners to Graph
		//
		// Register UndoManager with the Model
		graph.getModel().addUndoableEditListener(undoManager);
		// Update ToolBar based on Selection Changes
		graph.getSelectionModel().addGraphSelectionListener(this);
		// Listen for Delete Keystroke when the Graph has Focus
		graph.addKeyListener(this);

		// Construct Panel
		//
		// Add a ToolBar
		getContentPane().add(createToolBar(), BorderLayout.NORTH);
		// Add the Graph as Center Component
		getContentPane().add(new JScrollPane(graph), BorderLayout.CENTER);
	}

	// Hook for subclassers
	protected JGraph createGraph() {
		return new MyGraph(new MyModel());
	}

	// Insert a new Vertex at point
	public void insert(Point2D point) {
		// Construct Vertex with no Label
		DefaultGraphCell vertex = createDefaultGraphCell();
		// Create a Map that holds the attributes for the Vertex
		vertex.getAttributes().applyMap(createCellAttributes(point));
		// Insert the Vertex (including child port and attributes)
		graph.getGraphLayoutCache().insert(vertex);
	}

	// Hook for subclassers
	public Map createCellAttributes(Point2D point) {
		Map map = new Hashtable();
		// Snap the Point to the Grid
		point = graph.snap((Point2D) point.clone());
		// Add a Bounds Attribute to the Map
		GraphConstants.setBounds(map, new Rectangle2D.Double(point.getX(), point.getY(), 0, 0));
		// Make sure the cell is resized on insert
		GraphConstants.setResize(map, true);
		// Add a nice looking gradient background
		GraphConstants.setGradientColor(map, Color.blue);
		// Add a Border Color Attribute to the Map
		GraphConstants.setBorderColor(map, Color.black);
		// Add a White Background
		GraphConstants.setBackground(map, Color.white);
		// Make Vertex Opaque
		GraphConstants.setOpaque(map, true);
		return map;
	}

	// Hook for subclassers
	protected DefaultGraphCell createDefaultGraphCell() {
		DefaultGraphCell cell = new DefaultGraphCell("Cell " + new Integer(cellCount++));
		// Add one Floating Port
		cell.add(new DefaultPort());
		return cell;
	}

	// Insert a new Edge between source and target
	public void connect(Port source, Port target) {
		// Construct Edge with no label
		DefaultEdge edge = createDefaultEdge();
		if (graph.getModel().acceptsSource(edge, source) && graph.getModel().acceptsTarget(edge, target)) {
			// Create a Map thath holds the attributes for the edge
			edge.getAttributes().applyMap(createEdgeAttributes());
			// Insert the Edge and its Attributes
			graph.getGraphLayoutCache().insertEdge(edge, source, target);
		}
	}

	// Hook for subclassers
	protected DefaultEdge createDefaultEdge() {
		return new DefaultEdge();
	}

	// Hook for subclassers
	public Map createEdgeAttributes() {
		Map map = new Hashtable();
		// Add a Line End Attribute
		GraphConstants.setLineEnd(map, GraphConstants.ARROW_SIMPLE);
		// Add a label along edge attribute
		GraphConstants.setLabelAlongEdge(map, true);
		return map;
	}

	// Create a Group that Contains the Cells
	public void group(Object[] cells) {
		// Order Cells by Model Layering
		cells = graph.order(cells);
		// If Any Cells in View
		if (cells != null && cells.length > 0) {
			DefaultGraphCell group = createGroupCell();
			// Insert into model
			graph.getGraphLayoutCache().insertGroup(group, cells);
		}
	}

	// Hook for subclassers
	protected DefaultGraphCell createGroupCell() {
		return new DefaultGraphCell();
	}

	// Returns the total number of cells in a graph
	protected int getCellCount(JGraph graph) {
		Object[] cells = graph.getDescendants(graph.getRoots());
		return cells.length;
	}

	// Ungroup the Groups in Cells and Select the Children
	public void ungroup(Object[] cells) {
		graph.getGraphLayoutCache().ungroup(cells);
	}

	// Determines if a Cell is a Group
	public boolean isGroup(Object cell) {
		// Map the Cell to its View
		CellView view = graph.getGraphLayoutCache().getMapping(cell, false);
		if (view != null)
			return !view.isLeaf();
		return false;
	}

	// Brings the Specified Cells to Front
	public void toFront(Object[] c) {
		graph.getGraphLayoutCache().toFront(c);
	}

	// Sends the Specified Cells to Back
	public void toBack(Object[] c) {
		graph.getGraphLayoutCache().toBack(c);
	}

	// Undo the last Change to the Model or the View
	public void undo() {
		try {
			undoManager.undo(graph.getGraphLayoutCache());
		} catch (Exception ex) {
			System.err.println(ex);
		} finally {
			updateHistoryButtons();
		}
	}

	// Redo the last Change to the Model or the View
	public void redo() {
		try {
			undoManager.redo(graph.getGraphLayoutCache());
		} catch (Exception ex) {
			System.err.println(ex);
		} finally {
			updateHistoryButtons();
		}
	}

	// Update Undo/Redo Button State based on Undo Manager
	protected void updateHistoryButtons() {
		// The View Argument Defines the Context
		undo.setEnabled(undoManager.canUndo(graph.getGraphLayoutCache()));
		redo.setEnabled(undoManager.canRedo(graph.getGraphLayoutCache()));
	}

	//
	// Listeners
	//

	// From GraphSelectionListener Interface
	public void valueChanged(GraphSelectionEvent e) {
		// Group Button only Enabled if more than One Cell Selected
		group.setEnabled(graph.getSelectionCount() > 1);
		// Update Button States based on Current Selection
		boolean enabled = !graph.isSelectionEmpty();
		remove.setEnabled(enabled);
		ungroup.setEnabled(enabled);
		tofront.setEnabled(enabled);
		toback.setEnabled(enabled);
		copy.setEnabled(enabled);
		cut.setEnabled(enabled);
	}

	//
	// KeyListener for Delete KeyStroke
	//
	public void keyReleased(KeyEvent e) {
	}

	public void keyTyped(KeyEvent e) {
	}

	public void keyPressed(KeyEvent e) {
		// Listen for Delete Key Press
		if (e.getKeyCode() == KeyEvent.VK_DELETE)
			// Execute Remove Action on Delete Key Press
			remove.actionPerformed(null);
	}

	//
	// Custom Graph
	//

	// Defines a Graph that uses the Shift-Button (Instead of the Right
	// Mouse Button, which is Default) to add/remove point to/from an edge.
	public static class MyGraph extends JGraph {
		private static final long serialVersionUID = 1L;

		// Construct the Graph using the Model as its Data Source
		public MyGraph(GraphModel model) {
			super(model);
			// Make Ports Visible by Default
			setPortsVisible(true);
			// Use the Grid (but don't make it Visible)
			setGridEnabled(true);
			// Set the Grid Size to 10 Pixel
			setGridSize(6);
			// Set the Tolerance to 2 Pixel
			setTolerance(2);
			// Accept edits if click on background
			setInvokesStopCellEditing(true);
			// Allows control-drag
			setCloneable(true);
			// Jump to default port on connect
			setJumpToDefaultPort(true);
		}

		// Override Superclass Method to Return Custom EdgeView
		protected EdgeView createEdgeView(Object cell) {
			// Return Custom EdgeView
			return new EdgeView(cell) {
				private static final long serialVersionUID = 1L;

				/**
				 * Returns a cell handle for the view.
				 */
				public CellHandle getHandle(GraphContext context) {
					return new MyEdgeHandle(this, context);
				}

			};
		}
	}

	//
	// Custom Edge Handle
	//

	// Defines a EdgeHandle that uses the Shift-Button (Instead of the Right
	// Mouse Button, which is Default) to add/remove point to/from an edge.
	public static class MyEdgeHandle extends EdgeView.EdgeHandle {
		private static final long serialVersionUID = 1L;

		/**
		 * @param edge
		 * @param ctx
		 */
		public MyEdgeHandle(EdgeView edge, GraphContext ctx) {
			super(edge, ctx);
		}

		// Override Superclass Method
		public boolean isAddPointEvent(MouseEvent event) {
			// Points are Added using Shift-Click
			return event.isShiftDown();
		}

		// Override Superclass Method
		public boolean isRemovePointEvent(MouseEvent event) {
			// Points are Removed using Shift-Click
			return event.isShiftDown();
		}

	}

	//
	// Custom Model
	//

	// A Custom Model that does not allow Self-References
	public static class MyModel extends DefaultGraphModel {
		private static final long serialVersionUID = 1L;

		// Override Superclass Method
		public boolean acceptsSource(Object edge, Object port) {
			// Source only Valid if not Equal Target
			return (((Edge) edge).getTarget() != port);
		}

		// Override Superclass Method
		public boolean acceptsTarget(Object edge, Object port) {
			// Target only Valid if not Equal Source
			return (((Edge) edge).getSource() != port);
		}
	}

	//
	// Custom MarqueeHandler

	// MarqueeHandler that Connects Vertices and Displays PopupMenus
	public class MyMarqueeHandler extends BasicMarqueeHandler {

		// Holds the Start and the Current Point
		protected Point2D start, current;

		// Holds the First and the Current Port
		protected PortView port, firstPort;

		// Override to Gain Control (for PopupMenu and ConnectMode)
		public boolean isForceMarqueeEvent(MouseEvent e) {
			if (e.isShiftDown())
				return false;
			// If Right Mouse Button we want to Display the PopupMenu
			if (SwingUtilities.isRightMouseButton(e))
				// Return Immediately
				return true;
			// Find and Remember Port
			port = getSourcePortAt(e.getPoint());
			// If Port Found and in ConnectMode (=Ports Visible)
			if (port != null && graph.isPortsVisible())
				return true;
			// Else Call Superclass
			return super.isForceMarqueeEvent(e);
		}

		// Display PopupMenu or Remember Start Location and First Port
		public void mousePressed(final MouseEvent e) {
			// If Right Mouse Button
			if (SwingUtilities.isRightMouseButton(e)) {
				// Find Cell in Model Coordinates
				Object cell = graph.getFirstCellForLocation(e.getX(), e.getY());
				// Create PopupMenu for the Cell
				JPopupMenu menu = createPopupMenu(e.getPoint(), cell);
				// Display PopupMenu
				menu.show(graph, e.getX(), e.getY());
				// Else if in ConnectMode and Remembered Port is Valid
			} else if (port != null && graph.isPortsVisible()) {
				// Remember Start Location
				start = graph.toScreen(port.getLocation(null));
				// Remember First Port
				firstPort = port;
			} else {
				// Call Superclass
				super.mousePressed(e);
			}
		}

		// Find Port under Mouse and Repaint Connector
		public void mouseDragged(MouseEvent e) {
			// If remembered Start Point is Valid
			if (start != null) {
				// Fetch Graphics from Graph
				Graphics g = graph.getGraphics();
				// Reset Remembered Port
				PortView newPort = getTargetPortAt(e.getPoint());
				// Do not flicker (repaint only on real changes)
				if (newPort == null || newPort != port) {
					// Xor-Paint the old Connector (Hide old Connector)
					paintConnector(Color.black, graph.getBackground(), g);
					// If Port was found then Point to Port Location
					port = newPort;
					if (port != null)
						current = graph.toScreen(port.getLocation(null));
					// Else If no Port was found then Point to Mouse Location
					else
						current = graph.snap(e.getPoint());
					// Xor-Paint the new Connector
					paintConnector(graph.getBackground(), Color.black, g);
				}
			}
			// Call Superclass
			super.mouseDragged(e);
		}

		public PortView getSourcePortAt(Point2D point) {
			// Disable jumping
			graph.setJumpToDefaultPort(false);
			PortView result;
			try {
				// Find a Port View in Model Coordinates and Remember
				result = graph.getPortViewAt(point.getX(), point.getY());
			} finally {
				graph.setJumpToDefaultPort(true);
			}
			return result;
		}

		// Find a Cell at point and Return its first Port as a PortView
		protected PortView getTargetPortAt(Point2D point) {
			// Find a Port View in Model Coordinates and Remember
			return graph.getPortViewAt(point.getX(), point.getY());
		}

		// Connect the First Port and the Current Port in the Graph or Repaint
		public void mouseReleased(MouseEvent e) {
			// If Valid Event, Current and First Port
			if (e != null && port != null && firstPort != null && firstPort != port) {
				// Then Establish Connection
				connect((Port) firstPort.getCell(), (Port) port.getCell());
				e.consume();
				// Else Repaint the Graph
			} else
				graph.repaint();
			// Reset Global Vars
			firstPort = port = null;
			start = current = null;
			// Call Superclass
			super.mouseReleased(e);
		}

		// Show Special Cursor if Over Port
		public void mouseMoved(MouseEvent e) {
			// Check Mode and Find Port
			if (e != null && getSourcePortAt(e.getPoint()) != null && graph.isPortsVisible()) {
				// Set Cusor on Graph (Automatically Reset)
				graph.setCursor(new Cursor(Cursor.HAND_CURSOR));
				// Consume Event
				// Note: This is to signal the BasicGraphUI's
				// MouseHandle to stop further event processing.
				e.consume();
			} else
				// Call Superclass
				super.mouseMoved(e);
		}

		// Use Xor-Mode on Graphics to Paint Connector
		protected void paintConnector(Color fg, Color bg, Graphics g) {
			// Set Foreground
			g.setColor(fg);
			// Set Xor-Mode Color
			g.setXORMode(bg);
			// Highlight the Current Port
			paintPort(graph.getGraphics());
			// If Valid First Port, Start and Current Point
			if (firstPort != null && start != null && current != null)
				// Then Draw A Line From Start to Current Point
				g.drawLine((int) start.getX(), (int) start.getY(), (int) current.getX(), (int) current.getY());
		}

		// Use the Preview Flag to Draw a Highlighted Port
		protected void paintPort(Graphics g) {
			// If Current Port is Valid
			if (port != null) {
				// If Not Floating Port...
				boolean o = (GraphConstants.getOffset(port.getAllAttributes()) != null);
				// ...Then use Parent's Bounds
				Rectangle2D r = (o) ? port.getBounds() : port.getParentView().getBounds();
				// Scale from Model to Screen
				r = graph.toScreen((Rectangle2D) r.clone());
				// Add Space For the Highlight Border
				r.setFrame(r.getX() - 3, r.getY() - 3, r.getWidth() + 6, r.getHeight() + 6);
				// Paint Port in Preview (=Highlight) Mode
				graph.getUI().paintCell(g, port, r, true);
			}
		}

	} // End of Editor.MyMarqueeHandler

	//
	//
	//

	//
	// PopupMenu and ToolBar
	//

	//
	//
	//

	//
	// PopupMenu
	//
	public JPopupMenu createPopupMenu(final Point pt, final Object cell) {
		JPopupMenu menu = new JPopupMenu();
		if (cell != null) {
			// Edit
			menu.add(new AbstractAction("Edit") {
				private static final long serialVersionUID = 1L;

				public void actionPerformed(ActionEvent e) {
					graph.startEditingAtCell(cell);
				}
			});
		}
		// Remove
		if (!graph.isSelectionEmpty()) {
			menu.addSeparator();
			menu.add(new AbstractAction("Remove") {
				private static final long serialVersionUID = 1L;

				public void actionPerformed(ActionEvent e) {
					remove.actionPerformed(e);
				}
			});
		}
		menu.addSeparator();
		// Insert
		menu.add(new AbstractAction("Insert") {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent ev) {
				insert(pt);
			}
		});
		return menu;
	}

	//
	// ToolBar
	//
	public JToolBar createToolBar() {
		JToolBar toolbar = new JToolBar();
		toolbar.setFloatable(false);

		// Insert
		//		URL insertUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/insert.gif");
		//		URL insertUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/lightning.gif");
		//		if (insertUrl == null)
		//			System.out.println(MeetingExplorer.ICON_PATH + "/lightning.gif" + " yields null URL");
		//		ImageIcon insertIcon = new ImageIcon(insertUrl);
		ImageIcon insertIcon = new ImageIcon(getClass().getResource(MeetingExplorer.ICON_PATH + "/participant.gif"));
		toolbar.add(new AbstractAction("", insertIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				insert(new Point(10, 10));
			}
		});

		// Toggle Connect Mode
		//		URL connectUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/connecton.gif");
		ImageIcon connectIcon = new ImageIcon(getClass().getResource(MeetingExplorer.ICON_PATH + "/connecton.gif"));
		toolbar.add(new AbstractAction("", connectIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				graph.setPortsVisible(!graph.isPortsVisible());
				URL connectUrl;
				if (graph.isPortsVisible())
					connectUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/connecton.gif");
				else
					connectUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/connectoff.gif");
				ImageIcon connectIcon = new ImageIcon(connectUrl);
				putValue(SMALL_ICON, connectIcon);
			}
		});

		// Undo
		toolbar.addSeparator();
		URL undoUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/undo.gif");
		ImageIcon undoIcon = new ImageIcon(undoUrl);
		undo = new AbstractAction("", undoIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				undo();
			}
		};
		undo.setEnabled(false);
		toolbar.add(undo);

		// Redo
		URL redoUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/redo.gif");
		ImageIcon redoIcon = new ImageIcon(redoUrl);
		redo = new AbstractAction("", redoIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				redo();
			}
		};
		redo.setEnabled(false);
		toolbar.add(redo);

		//
		// Edit Block
		//
		toolbar.addSeparator();
		Action action;
		URL url;

		// Copy
		action = javax.swing.TransferHandler // JAVA13:
				// org.jgraph.plaf.basic.TransferHandler
				.getCopyAction();
		url = getClass().getResource(MeetingExplorer.ICON_PATH + "/copy.gif");
		action.putValue(Action.SMALL_ICON, new ImageIcon(url));
		toolbar.add(copy = new EventRedirector(action));

		// Paste
		action = javax.swing.TransferHandler // JAVA13:
				// org.jgraph.plaf.basic.TransferHandler
				.getPasteAction();
		url = getClass().getResource(MeetingExplorer.ICON_PATH + "/paste.gif");
		action.putValue(Action.SMALL_ICON, new ImageIcon(url));
		toolbar.add(paste = new EventRedirector(action));

		// Cut
		action = javax.swing.TransferHandler // JAVA13:
				// org.jgraph.plaf.basic.TransferHandler
				.getCutAction();
		url = getClass().getResource(MeetingExplorer.ICON_PATH + "/cut.gif");
		action.putValue(Action.SMALL_ICON, new ImageIcon(url));
		toolbar.add(cut = new EventRedirector(action));

		// Remove
		URL removeUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/delete.gif");
		ImageIcon removeIcon = new ImageIcon(removeUrl);
		remove = new AbstractAction("", removeIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				if (!graph.isSelectionEmpty()) {
					Object[] cells = graph.getSelectionCells();
					cells = graph.getDescendants(cells);
					graph.getModel().remove(cells);
				}
			}
		};
		remove.setEnabled(false);
		toolbar.add(remove);

		// To Front
		toolbar.addSeparator();
		URL toFrontUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/tofront.gif");
		ImageIcon toFrontIcon = new ImageIcon(toFrontUrl);
		tofront = new AbstractAction("", toFrontIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				if (!graph.isSelectionEmpty())
					toFront(graph.getSelectionCells());
			}
		};
		tofront.setEnabled(false);
		toolbar.add(tofront);

		// To Back
		URL toBackUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/toback.gif");
		ImageIcon toBackIcon = new ImageIcon(toBackUrl);
		toback = new AbstractAction("", toBackIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				if (!graph.isSelectionEmpty())
					toBack(graph.getSelectionCells());
			}
		};
		toback.setEnabled(false);
		toolbar.add(toback);

		// Zoom Std
		toolbar.addSeparator();
		URL zoomUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/zoom.gif");
		ImageIcon zoomIcon = new ImageIcon(zoomUrl);
		toolbar.add(new AbstractAction("", zoomIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				graph.setScale(1.0);
			}
		});
		// Zoom In
		URL zoomInUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/zoomin.gif");
		ImageIcon zoomInIcon = new ImageIcon(zoomInUrl);
		toolbar.add(new AbstractAction("", zoomInIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				graph.setScale(2 * graph.getScale());
			}
		});
		// Zoom Out
		URL zoomOutUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/zoomout.gif");
		ImageIcon zoomOutIcon = new ImageIcon(zoomOutUrl);
		toolbar.add(new AbstractAction("", zoomOutIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				graph.setScale(graph.getScale() / 2);
			}
		});

		// Group
		toolbar.addSeparator();
		URL groupUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/group.gif");
		ImageIcon groupIcon = new ImageIcon(groupUrl);
		group = new AbstractAction("", groupIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				group(graph.getSelectionCells());
			}
		};
		group.setEnabled(false);
		toolbar.add(group);

		// Ungroup
		URL ungroupUrl = getClass().getResource(MeetingExplorer.ICON_PATH + "/ungroup.gif");
		ImageIcon ungroupIcon = new ImageIcon(ungroupUrl);
		ungroup = new AbstractAction("", ungroupIcon) {
			private static final long serialVersionUID = 1L;

			public void actionPerformed(ActionEvent e) {
				ungroup(graph.getSelectionCells());
			}
		};
		ungroup.setEnabled(false);
		toolbar.add(ungroup);

		return toolbar;
	}

	/**
	 * @return Returns the graph.
	 */
	public JGraph getGraph() {
		return graph;
	}

	/**
	 * @param graph The graph to set.
	 */
	public void setGraph(JGraph graph) {
		this.graph = graph;
	}

	// This will change the source of the actionevent to graph.
	protected class EventRedirector extends AbstractAction {
		private static final long serialVersionUID = 1L;

		protected Action action;

		// Construct the "Wrapper" Action
		public EventRedirector(Action a) {
			super("", (ImageIcon) a.getValue(Action.SMALL_ICON));
			this.action = a;
		}

		// Redirect the Actionevent
		public void actionPerformed(ActionEvent e) {
			e = new ActionEvent(graph, e.getID(), e.getActionCommand(), e.getModifiers());
			action.actionPerformed(e);
		}
	}

	//	----------------------------------------------------------------//
	//	----------------------------------------------------------------//
	//	----------------------------------------------------------------//
	// Main Method
	public static void main(String[] args) {
		System.out.println("Starting:");
		// Construct Frame
		JFrame frame = new JFrame("GraphEd");
		// Set Close Operation to Exit
		// frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		// Fetch URL to Icon Resource
		URL jgraphUrl = GraphEd.class.getResource(MeetingExplorer.ICON_PATH + "/jgraph.gif");
		// If Valid URL
		if (jgraphUrl != null) {
			// Load Icon
			ImageIcon jgraphIcon = new ImageIcon(jgraphUrl);
			// Use in Window
			frame.setIconImage(jgraphIcon.getImage());
			System.out.println("DONE!");
		} else
			System.out.println("Fuck!");

		// Add an Editor Panel
		frame.getContentPane().add(new GraphEd());
		// Set Default Size
		frame.setSize(520, 390);
		// Show Frame
		frame.setVisible(true);
	}
	//	----------------------------------------------------------------//
}
