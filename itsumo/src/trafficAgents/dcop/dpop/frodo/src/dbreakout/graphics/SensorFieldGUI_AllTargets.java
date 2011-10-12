/*
 * SensorFieldGUI.java
 *
 * Created on October 16, 2002, 2:03 PM
 */

package frodo.dbreakout.graphics;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionListener;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JScrollPane;

import frodo.CSP.CSPNodeInterface;
import frodo.communication.DistributedAgentInterface;
import frodo.dbreakout.CSPAgent;
import frodo.sensors.Environment;
import frodo.sensors.SensorArray;

/**
 *
 * @author  apetcu
 */
public class SensorFieldGUI_AllTargets implements ActionListener {

	/**
	 * 
	 * @uml.property name="f"
	 * @uml.associationEnd 
	 * @uml.property name="f" multiplicity="(1 1)"
	 */
	JFrame f;

	/**
	 * 
	 * @uml.property name="sfp"
	 * @uml.associationEnd 
	 * @uml.property name="sfp" multiplicity="(1 1)" inverse="sfg:testbed.dbreakout.graphics.SensorFieldPanel_AllTargets"
	 */
	SensorFieldPanel_AllTargets sfp;

	private boolean				restricted_set;
	int							myMagnification;

	/**
	 * 
	 * @uml.property name="sa"
	 * @uml.associationEnd 
	 * @uml.property name="sa" multiplicity="(1 1)"
	 */
	SensorArray sa;

	Vector						targets;
	Vector						sensors;

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
	 * @uml.property name="myAgent" multiplicity="(1 1)"
	 */
	DistributedAgentInterface myAgent;

	/** Creates a new instance of SensorFieldGUI */
	public SensorFieldGUI_AllTargets(DistributedAgentInterface myAgent, SensorArray sa, Vector targets, int zoom) {
		this(myAgent, sa, targets, null, zoom);
	}
	public SensorFieldGUI_AllTargets(DistributedAgentInterface myA, SensorArray sa, Vector t, Vector sensors, int zoom) {
		if (sensors == null)
			restricted_set = false;
		else
			restricted_set = true;
		this.sensors = sensors;
		this.sa = sa;
		this.targets = t;
		this.myMagnification = zoom;
		if (targets == null)
			System.out.println("))))))))))))))))))))))))))))))) agent " + myA.getMyName() + " Tracking nothing");
		myAgent = myA;
		nodes = new Vector();
		Environment env = (Environment) myAgent.getEnvironment();
		Iterator agents = env.getAgents().values().iterator();
		while (agents.hasNext()) {
			CSPNodeInterface node = ((CSPAgent) agents.next()).getCSPNode();
			nodes.add(node);
		}
		f = new JFrame(myAgent.getMyName());
		f.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent evt) {
				//exitForm(evt);
				f.dispose();
				System.exit(0);
			}
		});
		sfp = new SensorFieldPanel_AllTargets(this);
		JScrollPane jsp = new JScrollPane(sfp);
		jsp.setPreferredSize(new Dimension(1150, 950));

		//            f.getContentPane().setLayout(new GridLayout(1,2,10,5));//null);
		f.getContentPane().setLayout(new BorderLayout());
		f.getContentPane().add(jsp);
		//            f.getContentPane().add(butt_panel, BorderLayout.NORTH);

		f.pack();
		//        f.setSize(1200,1000);
		//            Graphics g = jp.getGraphics();
		//            System.out.println("g=" + g);
		//            g.drawLine(10,10,100,100);
	}

	public void actionPerformed(java.awt.event.ActionEvent actionEvent) {
		String command = actionEvent.getActionCommand();

		if (command.equals("Exit")) {
			f.dispose();
			System.exit(0);
		}
	}
	public void displayMessage(String s) {
		sfp.displayMessage(s);
	}
	public boolean drawRestrictedSet() {
		return restricted_set;
	}
	public void repaint() {
		f.repaint();
	}
	public void startShowing() {
		f.setVisible(true);
	}
}