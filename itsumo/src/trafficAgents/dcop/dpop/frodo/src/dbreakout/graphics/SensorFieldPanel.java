/*
 * SensorFieldPanel.java
 *
 * Created on October 23, 2002, 4:56 PM
 */

package frodo.dbreakout.graphics;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

import frodo.communication.DistributedAgentInterface;
import frodo.dbreakout.CSPAgent;
import frodo.dbreakout.CSP.CSPNodeDB;
import frodo.dbreakout.CSP.CSPVariableDBA;
import frodo.sensors.Position;
import frodo.sensors.Sensor;
import frodo.sensors.SensorArray;
import frodo.sensors.Target;
import frodo.utils.Logger;

/**
 *
 * @author  apetcu
 */

public class SensorFieldPanel extends JPanel {
	Logger myLogger;

	private final int			offset						= 100;
	private final byte			mag							= 2;

	/**
	 * 
	 * @uml.property name="sa"
	 * @uml.associationEnd 
	 * @uml.property name="sa" multiplicity="(1 1)"
	 */
	SensorArray sa;

	/**
	 * 
	 * @uml.property name="sfg"
	 * @uml.associationEnd 
	 * @uml.property name="sfg" multiplicity="(1 1)" inverse="sfp:testbed.dbreakout.graphics.SensorFieldGUI"
	 */
	SensorFieldGUI sfg;

	/**
	 * 
	 * @uml.property name="trg"
	 * @uml.associationEnd 
	 * @uml.property name="trg" multiplicity="(1 1)"
	 */
	Target trg;

	/**
	 * 
	 * @uml.property name="myAgent"
	 * @uml.associationEnd 
	 * @uml.property name="myAgent" multiplicity="(1 1)"
	 */
	DistributedAgentInterface myAgent;

	int[][]						coords;
	int[]						ranges;

	/**
	 * 
	 * @uml.property name="sensor_rangesCHB"
	 * @uml.associationEnd 
	 * @uml.property name="sensor_rangesCHB" multiplicity="(1 1)"
	 */
	JCheckBox sensor_rangesCHB;

	boolean						paint_sensor_ranges			= false;

	/**
	 * 
	 * @uml.property name="sensor_connectivityCHB"
	 * @uml.associationEnd 
	 * @uml.property name="sensor_connectivityCHB" multiplicity="(1 1)"
	 */
	JCheckBox sensor_connectivityCHB;

	boolean						paint_sensor_connectivity	= false;

	/**
	 * 
	 * @uml.property name="domainsCHB"
	 * @uml.associationEnd 
	 * @uml.property name="domainsCHB" multiplicity="(1 1)"
	 */
	JCheckBox domainsCHB;

	boolean						paint_domains				= false;

	/**
	 * 
	 * @uml.property name="message"
	 * @uml.associationEnd 
	 * @uml.property name="message" multiplicity="(1 1)"
	 */
	JLabel message;

	//-------------------------------------------------------------------------------------//
	public SensorFieldPanel(SensorFieldGUI sfg) {
		this.sfg = sfg;
		trg = sfg.target;
		sa = sfg.sa;
		myAgent = sfg.myAgent;
		myLogger = myAgent.getLogger();
		coords = sa.getSensorCoordinates();
		ranges = sa.getSensorRanges();

		JPanel chkBoxPanel = new JPanel();
		chkBoxPanel.setLayout(new BoxLayout(chkBoxPanel, BoxLayout.Y_AXIS));

		sensor_rangesCHB = new JCheckBox("Sensor ranges");
		sensor_rangesCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_sensor_ranges = !paint_sensor_ranges;
				repaint();
			}
		});
		chkBoxPanel.add(sensor_rangesCHB);

		sensor_connectivityCHB = new JCheckBox("Sensor connectivity graph");
		sensor_connectivityCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_sensor_connectivity = !paint_sensor_connectivity;
				repaint();
			}
		});
		chkBoxPanel.add(sensor_connectivityCHB);

		domainsCHB = new JCheckBox("Whole domains");
		domainsCHB.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent ie) {
				paint_domains = !paint_domains;
				repaint();
			}
		});
		chkBoxPanel.add(domainsCHB);

		message = new JLabel("Starting...");
		chkBoxPanel.add(message);

		this.add(chkBoxPanel);

		setPreferredSize(new Dimension(950, 950));
		//        f.addActionListener(this);

	}
	//-------------------------------------------------------------------------------------//
	public void drawSensor(Graphics g, Sensor se, int status) {
		Color tempColor = g.getColor();
		Position po = se.getPosition();
		switch (status) {
			case 1 : //that is, sensor is within range
				g.setColor(new Color(100, 100, 180));
				fillCircle(g, po, 1);
				g.drawString(se.getName(), (po.getX() << mag) + offset, (po.getY() << mag) + offset + 20);
				if (paint_sensor_ranges) {
					drawCircle(g, po, se.getRange());
				}
				break;
			case 2 : //that is, sensor is selected
				g.setColor(Color.green);
				fillCircle(g, po, 1);
				//                g.setColor(Color.green);
				g.drawString(se.getName(), (po.getX() << mag) + offset, (po.getY() << mag) + offset + 20);
				//                drawCircle(g,po,se.getRange());
				break;
			default : //that is, sensor is out of range
				g.setColor(Color.gray);
				fillCircle(g, po, 1);
				g.drawString(se.getName(), (po.getX() << mag) + offset, (po.getY() << mag) + offset + 20);
		//                drawCircle(g,po,se.getRange());
		}
		g.setColor(tempColor);
	}
	//-------------------------------------------------------------------------------------//
	public void drawTarget(Graphics g, Target t) {
		Color tempColor = g.getColor();
		g.setColor(Color.red);
		Position po = t.getPosition();
		fillCircle(g, po, 1);
		g.drawString(t.getName(), (po.getX() << mag) + offset, (po.getY() << mag) + offset + 20);
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
		g.fillOval(((x - radius) << mag) + offset, ((y - radius) << mag) + offset, radius << (mag + 1), radius << (mag + 1));
	}
	//-------------------------------------------------------------------------------------//
	public void drawCircle(Graphics g, int x, int y, int radius) {
		g.drawOval(((x - radius) << mag) + offset, ((y - radius) << mag) + offset, radius << (mag + 1), radius << (mag + 1));
	}
	//-------------------------------------------------------------------------------------//
	public void drawScaledLine(Graphics g, int x1, int y1, int x2, int y2) {
		g.drawLine((x1 << mag) + offset, (y1 << mag) + offset, (x2 << mag) + offset, (y2 << mag) + offset);
	}
	//-------------------------------------------------------------------------------------//
	public void drawScaledLine(Graphics g, Position p1, Position p2) {
		this.drawScaledLine(g, p1.getX(), p1.getY(), p2.getX(), p2.getY());
	}
	//-------------------------------------------------------------------------------------//
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		CSPAgent myDBA = (CSPAgent) myAgent;
		CSPNodeDB myDBCSPNode = (CSPNodeDB) myDBA.getCSPNode();
		//target
		try {
			g.setColor(Color.red);
			if (sfg.drawRestrictedSet()) {
				drawTarget(g, trg);
				myLogger.println("RESTRICTED_SET( ) : " + sfg.sensors.size() + " sensors");
				for (int i = 0; i < sa.getSize(); i++) {
					boolean isOk = sfg.sensors.contains(new Integer(i));
					Sensor se = sa.getSensor(i);
					drawSensor(g, se, isOk ? 1 : 0);
					if (isOk) {
						g.setColor(Color.black);
						for (int j = 0; j < sa.getSize(); j++) {
							if (sa.areConnected(i, j) && sfg.sensors.contains(new Integer(j))) {
								Sensor s2 = sa.getSensor(j);
								//                        Sensor s2 = ((Sensor)(sfg.sensors.elementAt(j)));
								drawScaledLine(g, se.getPosition(), s2.getPosition());
								//                        g.drawLine((coords[i][0]+50)<<2,(coords[i][1]+50)<<2,(coords[j][0]+50)<<2, (coords[j][1]+50)<<2);
							}
						}
					}
				}
				int whereToWriteMessages = getSize().height - 50;
				Vector vars = myAgent.getCSPNode().getVariables();
				for (int i = 0; i < vars.size(); i++) {
					CSPVariableDBA var = (CSPVariableDBA) vars.elementAt(i);
					Integer sensorID = var.getValue();
					myLogger.println("---------------))))))))))) var=" + var + ", sensorID=" + sensorID);
					if (sensorID != null) {
						Sensor s = ((CSPAgent) myAgent).getMySensorArray().getSensor(sensorID.intValue());
						Position p = s.getPosition();
						if (myDBCSPNode.numberOfViolationsForVariable(var) == 0) {
							g.setColor(Color.blue);
							drawSensor(g, s, 2);
						} else
							g.setColor(Color.red);
						drawScaledLine(g, p, trg.getPosition());
					}
				}
				if (myDBA.solutionFound()) {
					g.setFont(new Font(null, Font.BOLD, 16));
					g.setColor(Color.red);
					g.drawString("Solution found in " + myDBA.getRounds() + " rounds: " + vars, 50, whereToWriteMessages);
				} else if (myAgent.isUnsolvable()) {
					g.setFont(new Font(null, Font.BOLD, 16));
					g.setColor(Color.red);
					g.drawString("Could not find a solution in " + myDBA.getRounds() + " rounds. :-(", 50, whereToWriteMessages);
					g.drawString("Current assignments are:" + vars, 50, whereToWriteMessages + 20);
				}

			} else {
				myLogger.println("NOT RESTRICTED_SET");
				g.setColor(Color.red);
				g.drawString("Not tracking...", 20, 20);
				for (int i = 0; i < coords.length; i++) {
					//sensors
					drawSensor(g, sa.getSensor(i), 1);
					//connections
					g.setColor(Color.black);
					for (int j = 0; j < coords.length; j++) {
						if (sa.areConnected(i, j)) {
							drawScaledLine(g, coords[i][0], coords[i][1], coords[j][0], coords[j][1]);
						}
					}
				}
			}
		} catch (Exception e) {
			myLogger.println("Problem occured while drawing field.");
			myLogger.println(e.toString());
		}
	}
	//-------------------------------------------------------------------------------------//
	public void displayMessage(String s) {
		message.setText(s);
		message.repaint();
	}
}