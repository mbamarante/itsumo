/*
 * SensorFieldPanel.java
 *
 * Created on October 23, 2002, 4:56 PM
 */
package frodo.sensors.graphics;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;

import frodo.CSP.CSPAgentInterface;
import frodo.CSP.CSPNodeInterface;
import frodo.CSP.CSPVariable;
import frodo.CSP.ComplexValue;
import frodo.CSP.ComplexVariable;
import frodo.dbreakout.CSP.CSPNodeDB;
import frodo.sensors.Position;
import frodo.sensors.Sensor;
import frodo.sensors.SensorArray;
import frodo.sensors.Target;
import frodo.utils.Logger;


/**
 *
 * @author  apetcu
 */
public class SensorFieldPanel_AllTargets extends JPanel implements ActionListener {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private final int offset = 150;

	final int numButtons = 3;

	/**
	 * 
	 * @uml.property name="radioButtons"
	 * @uml.associationEnd 
	 * @uml.property name="radioButtons" multiplicity="(0 -1)"
	 */
	JRadioButton[] radioButtons = new JRadioButton[numButtons];

	int myMagnification = 2;

	/**
	 * 
	 * @uml.property name="sa"
	 * @uml.associationEnd 
	 * @uml.property name="sa" multiplicity="(0 1)"
	 */
	SensorArray sa;

	/**
	 * 
	 * @uml.property name="sfg"
	 * @uml.associationEnd 
	 * @uml.property name="sfg" multiplicity="(0 1)" inverse="sfp:testbed.sensors.graphics.SensorFieldGUI_AllTargets"
	 */
	SensorFieldGUI_AllTargets sfg;

	/**
	 * 
	 * @uml.property name="targets"
	 * @uml.associationEnd 
	 * @uml.property name="targets" multiplicity="(0 -1)" elementType="testbed.sensors.Target"
	 */
	Vector targets;

	/**
	 * 
	 * @uml.property name="nodes"
	 * @uml.associationEnd 
	 * @uml.property name="nodes" multiplicity="(0 -1)" elementType="testbed.CSP.CSPNodeInterface"
	 */
	Vector nodes;

	/**
	 * 
	 * @uml.property name="myAgent"
	 * @uml.associationEnd 
	 * @uml.property name="myAgent" multiplicity="(0 1)"
	 */
	CSPAgentInterface myAgent;

	/**
	 * 
	 * @uml.property name="myLogger"
	 * @uml.associationEnd 
	 * @uml.property name="myLogger" multiplicity="(0 1)"
	 */
	Logger myLogger;

	/**
	 * 
	 * @uml.property name="butt_panel"
	 * @uml.associationEnd 
	 * @uml.property name="butt_panel" multiplicity="(0 1)"
	 */
	JPanel butt_panel;

	/**
	 * 
	 * @uml.property name="message"
	 * @uml.associationEnd 
	 * @uml.property name="message" multiplicity="(0 1)"
	 */
	JLabel message;

	/**
	 * 
	 * @uml.property name="csp_graphCHB"
	 * @uml.associationEnd 
	 * @uml.property name="csp_graphCHB" multiplicity="(0 1)"
	 */
	JCheckBox csp_graphCHB;

	boolean paint_csp_graph = false;

	/**
	 * 
	 * @uml.property name="sensor_rangesCHB"
	 * @uml.associationEnd 
	 * @uml.property name="sensor_rangesCHB" multiplicity="(0 1)"
	 */
	JCheckBox sensor_rangesCHB;

	boolean paint_sensor_ranges = false;

	/**
	 * 
	 * @uml.property name="sensor_connectivityCHB"
	 * @uml.associationEnd 
	 * @uml.property name="sensor_connectivityCHB" multiplicity="(0 1)"
	 */
	JCheckBox sensor_connectivityCHB;

	boolean paint_sensor_connectivity = false;

	/**
	 * 
	 * @uml.property name="domainsCHB"
	 * @uml.associationEnd 
	 * @uml.property name="domainsCHB" multiplicity="(0 1)"
	 */
	JCheckBox domainsCHB;

	boolean paint_domains = false;

	/**
	 * 
	 * @uml.property name="text_assignmentsCHB"
	 * @uml.associationEnd 
	 * @uml.property name="text_assignmentsCHB" multiplicity="(0 1)"
	 */
	JCheckBox text_assignmentsCHB;

	boolean paint_text_assignments = false;

	/**
	 * 
	 * @uml.property name="graph_assignmentsCHB"
	 * @uml.associationEnd 
	 * @uml.property name="graph_assignmentsCHB" multiplicity="(0 1)"
	 */
	JCheckBox graph_assignmentsCHB;

	boolean paint_graph_assignments = true;

	/**
	 * 
	 * @uml.property name="violationsCHB"
	 * @uml.associationEnd 
	 * @uml.property name="violationsCHB" multiplicity="(0 1)"
	 */
	JCheckBox violationsCHB;

	boolean paint_violations = false;

	/**
	 * 
	 * @uml.property name="updateGUI_CHB"
	 * @uml.associationEnd 
	 * @uml.property name="updateGUI_CHB" multiplicity="(0 1)"
	 */
	JCheckBox updateGUI_CHB;

	boolean updateGUI = false;

	//-------------------------------------------------------------------------------------//
	public SensorFieldPanel_AllTargets(SensorFieldGUI_AllTargets sfg) {
		this.sfg = sfg;
		targets = sfg.targets;
		sa = sfg.sa;
		myAgent = sfg.myAgent;
		nodes = sfg.nodes;
		myMagnification = sfg.myMagnification;
		myLogger = myAgent.getLogger();

		JPanel chkBoxPanel = new JPanel();
		chkBoxPanel.setBackground(null);
		chkBoxPanel.setForeground(null);
		//        chkBoxPanel.setPreferredSize()
		//        chkBoxPanel.setLayout(new BoxLayout(chkBoxPanel, BoxLayout.Y_AXIS));
		chkBoxPanel.setLayout(new GridLayout(4, 2));

		csp_graphCHB = new JCheckBox("CSP connectivity graph");
		csp_graphCHB.setBackground(null);
		csp_graphCHB.setForeground(null);
		csp_graphCHB.setSelected(paint_csp_graph);
		csp_graphCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_csp_graph = !paint_csp_graph;
				repaint();
			}
		});
		chkBoxPanel.add(csp_graphCHB);
		sensor_rangesCHB = new JCheckBox("Sensor ranges");
		sensor_rangesCHB.setBackground(null);
		sensor_rangesCHB.setForeground(null);
		sensor_rangesCHB.setSelected(paint_sensor_ranges);
		sensor_rangesCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_sensor_ranges = !paint_sensor_ranges;
				repaint();
			}
		});
		chkBoxPanel.add(sensor_rangesCHB);
		sensor_connectivityCHB = new JCheckBox("Sensor connectivity graph");
		sensor_connectivityCHB.setBackground(null);
		sensor_connectivityCHB.setForeground(null);
		sensor_connectivityCHB.setSelected(paint_sensor_connectivity);
		sensor_connectivityCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_sensor_connectivity = !paint_sensor_connectivity;
				repaint();
			}
		});
		chkBoxPanel.add(sensor_connectivityCHB);
		domainsCHB = new JCheckBox("Whole domains");
		domainsCHB.setBackground(null);
		domainsCHB.setForeground(null);
		domainsCHB.setSelected(paint_domains);
		domainsCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_domains = !paint_domains;
				repaint();
			}
		});
		chkBoxPanel.add(domainsCHB);
		text_assignmentsCHB = new JCheckBox("Write assignments as text");
		text_assignmentsCHB.setBackground(null);
		text_assignmentsCHB.setForeground(null);
		text_assignmentsCHB.setSelected(paint_text_assignments);
		text_assignmentsCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_text_assignments = !paint_text_assignments;
				repaint();
			}
		});
		chkBoxPanel.add(text_assignmentsCHB);
		graph_assignmentsCHB = new JCheckBox("Paint assignments as lines");
		graph_assignmentsCHB.setBackground(null);
		graph_assignmentsCHB.setForeground(null);
		graph_assignmentsCHB.setSelected(paint_graph_assignments);
		graph_assignmentsCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_graph_assignments = !paint_graph_assignments;
				repaint();
			}
		});
		chkBoxPanel.add(graph_assignmentsCHB);
		violationsCHB = new JCheckBox("Paint constraint violations in red");
		violationsCHB.setBackground(null);
		violationsCHB.setForeground(null);
		violationsCHB.setSelected(paint_violations);
		violationsCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_violations = !paint_violations;
				repaint();
			}
		});
		chkBoxPanel.add(violationsCHB);
		updateGUI_CHB = new JCheckBox("Update GUI");
		updateGUI_CHB.setBackground(null);
		updateGUI_CHB.setForeground(null);
		updateGUI_CHB.setSelected(updateGUI);
		updateGUI_CHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				updateGUI = !updateGUI;
				repaint();
			}
		});
		chkBoxPanel.add(updateGUI_CHB);

		JPanel radioPanel = new JPanel();
		JLabel scaleLabel = new JLabel("Scale:");
		scaleLabel.setForeground(null);
		scaleLabel.setBackground(null);
		radioPanel.add(scaleLabel);
		radioPanel.setBackground(null);
		radioPanel.setForeground(null);
		ButtonGroup magnGroup = new ButtonGroup();
		for (int i = 0; i < numButtons; i++) {
			myLogger.println("adding radio button " + i);
			radioButtons[i] = new JRadioButton(String.valueOf(i));
			radioButtons[i].setActionCommand(String.valueOf(i));
			radioButtons[i].addActionListener(this);
			radioButtons[i].setBackground(null);
			radioButtons[i].setForeground(null);
			if (i == myMagnification)
				radioButtons[i].setSelected(true);
			magnGroup.add(radioButtons[i]);
			radioPanel.add(radioButtons[i]);
		}

		chkBoxPanel.add(radioPanel);
		butt_panel = new JPanel();
		butt_panel.setLayout(new BoxLayout(butt_panel, BoxLayout.Y_AXIS));
		butt_panel.setForeground(null);
		butt_panel.setBackground(null);
		//        butt_panel.add(new JLabel("Monitoring..."));
		JButton exitb = new JButton("Exit");
		exitb.setActionCommand("Exit");
		exitb.addActionListener(sfg);
		butt_panel.add(exitb);
		/*
		 JButton solb = new JButton("Show assignment");
		 solb.setActionCommand("showsolution");
		 solb.addActionListener(sfg);
		 butt_panel.add(solb);
		 */
		message = new JLabel("Starting...");
		message.setBackground(null);
		message.setForeground(null);
		chkBoxPanel.add(new JLabel());
		chkBoxPanel.add(message);
		add(butt_panel);
		add(chkBoxPanel);
		//		setPreferredSize(new Dimension(1200, 1150));
		//		setPreferredSize(new Dimension(800, 600));
		setPreferredSize(new Dimension(1200, 1000));
		setBackground(new Color(70, 70, 70));
		setForeground(new Color(50, 200, 50));
	}

	//-------------------------------------------------------------------------------------//
	public void drawSensor(Graphics g, Sensor se, boolean active) {
		Color tempColor = g.getColor();
		//        active=false;
		if (active) {
			g.setColor(Color.green);
			Position po = se.getPosition();
			fillCircle(g, po, 1);
			g.setColor(Color.blue);
			g.drawString(se.getName(), (po.getX() << myMagnification) + offset, (po.getY() << myMagnification) + offset + 20);
			//            g.setColor(Color.green);
			if (paint_sensor_ranges)
				drawCircle(g, po, se.getRange());
		} else {
			g.setColor(Color.gray);
			Position po = se.getPosition();
			fillCircle(g, po, 1);
			g.drawString(se.getName(), (po.getX() << myMagnification) + offset, (po.getY() << myMagnification) + offset + 20);
			if (paint_sensor_ranges)
				drawCircle(g, po, se.getRange());
		}
		g.setColor(tempColor);
	}

	//-------------------------------------------------------------------------------------//
	public void drawTarget(Graphics g, Target t) {
		Color tempColor = g.getColor();
		g.setColor(Color.red);
		Position po = t.getPosition();
		fillCircle(g, po, 1);
		g.drawString(t.getName(), (po.getX() << myMagnification) + offset, (po.getY() << myMagnification) + offset + 20);
		g.setColor(tempColor);
	}

	//-------------------------------------------------------------------------------------//
	public void drawCircle(Graphics g, Position p, int radius) {
		this.drawCircle(g, p.getX(), p.getY(), radius);
	}

	//-------------------------------------------------------------------------------------//
	public void fillCircle(Graphics g, Position p, int radius) {
		this.fillCircle(g, p.getX(), p.getY(), radius);
	}

	//-------------------------------------------------------------------------------------//
	public void fillCircle(Graphics g, int x, int y, int radius) {
		g.fillOval(((x - radius) << myMagnification) + offset, ((y - radius) << myMagnification) + offset, radius << (myMagnification + 1), radius << (myMagnification + 1));
	}

	//-------------------------------------------------------------------------------------//
	public void drawCircle(Graphics g, int x, int y, int radius) {
		g.drawOval(((x - radius) << myMagnification) + offset, ((y - radius) << myMagnification) + offset, radius << (myMagnification + 1), radius << (myMagnification + 1));
	}

	//-------------------------------------------------------------------------------------//
	public void drawScaledLine(Graphics g, int x1, int y1, int x2, int y2) {
		g.drawLine((x1 << myMagnification) + offset, (y1 << myMagnification) + offset, (x2 << myMagnification) + offset, (y2 << myMagnification) + offset);
	}

	//-------------------------------------------------------------------------------------//
	public void drawScaledLine(Graphics g, Position p1, Position p2) {
		this.drawScaledLine(g, p1.getX(), p1.getY(), p2.getX(), p2.getY());
	}

	//-------------------------------------------------------------------------------------//
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		if (updateGUI) {
			//target
			try {
				for (int i = 0; i < sa.getAllSensors().size(); i++) {
					Sensor si = sa.getSensor(i);
					Position pi = si.getPosition();
					//sensors
					drawSensor(g, si, false);
					if (paint_sensor_connectivity) {
						//connections
						g.setColor(new Color(130, 130, 50));
						for (int j = 0; j < sa.getAllSensors().size(); j++) {
							Sensor sj = sa.getSensor(j);
							Position pj = sj.getPosition();
							if (sa.areConnected(i, j)) {
								drawScaledLine(g, pi.getX(), pi.getY(), pj.getX(), pj.getY());
							}
						}
					}
				}
				for (int i = 0; i < targets.size(); i++) {
					drawTarget(g, (Target) targets.elementAt(i));
					Position targetPos = ((Target) targets.elementAt(i)).getPosition();

					//CSPNodeInterface node = (CSPNodeInterface) (nodes.elementAt(i));

					//I did this because in principle only DBA plays with sensor networks
					//to return to the old, generic behavior, uncomment the previous line, and comment the following
					CSPNodeDB node = (CSPNodeDB) (nodes.elementAt(i));

					ComplexVariable cvar = node.getComplexVariable();
					ComplexValue cval = cvar.getValue();
					//					Vector vars = node.getVariables();
					g.setColor(Color.red);
					if (paint_text_assignments)
						g.drawString(i + ":" + cval.toStringWithoutIdWithCost(), getSize().width - 220, i * 20 + 150);
					for (int j = 0; j < cval.size(); j++) {
						CSPVariable var = (CSPVariable) node.getVariables().elementAt(j);
						Integer tval = cval.getITHIntegerValue(j);
						if (tval != null) {
							if (paint_domains) {
								g.setColor(new Color(170, 170, 170));
								Vector doma = var.getDomain();
								for (int k = 0; k < doma.size(); k++) {
									Integer cur = (Integer) doma.elementAt(k);
									Sensor si = sa.getSensor(cur.intValue());
									Position pi = si.getPosition();
									drawScaledLine(g, pi.getX(), pi.getY(), targetPos.getX(), targetPos.getY());
								}
							} else {
								Sensor si = sa.getSensor(cval.getITHIntegerValue(j).intValue());
								drawSensor(g, si, true);
								Position pi = si.getPosition();

								if (paint_violations) {
									/*
									 if (node.numberOfViolationsForVariable(var) != 0) {
									 g.setColor(Color.red);
									 drawScaledLine(g, pi.getX(), pi.getY(), targetPos.getX(), targetPos.getY());
									 }
									 */
								} else {
									if (paint_graph_assignments) {
										g.setColor(Color.blue);
										drawScaledLine(g, pi.getX(), pi.getY(), targetPos.getX(), targetPos.getY());
									}
								}
							}
						}
					}
					if (paint_csp_graph) {
						Vector neighbours = node.getNeighbours();
						g.setColor(Color.cyan);
						for (int j = 0; j < neighbours.size(); j++) {
							CSPNodeInterface other = (CSPNodeInterface) neighbours.elementAt(j);
							Position op = ((Target) targets.elementAt(other.getId())).getPosition();
							drawScaledLine(g, op.getX(), op.getY(), targetPos.getX(), targetPos.getY());
						}
					}
				}
			} catch (Exception e) {
				myLogger.println("@@@@@@@@@@@@@ Problem occured while drawing field for " + sfg.myAgent.getMyName());
				e.printStackTrace();
			}
		}
	}

	//-------------------------------------------------------------------------------------//
	public void displayMessage(String s) {
		message.setText(s);
		repaint();
	}

	//-------------------------------------------------------------------------------------//
	public void actionPerformed(ActionEvent e) {
		String cmd = e.getActionCommand();
		if (cmd.equalsIgnoreCase("0"))
			myMagnification = 0;
		else if (cmd.equalsIgnoreCase("1"))
			myMagnification = 1;
		else if (cmd.equalsIgnoreCase("2"))
			myMagnification = 2;
		else {
			myLogger.println("@@@@@@@@@@@@@@ Strange command arrived!!!" + cmd);
			return;
		}
		repaint();
	}
}