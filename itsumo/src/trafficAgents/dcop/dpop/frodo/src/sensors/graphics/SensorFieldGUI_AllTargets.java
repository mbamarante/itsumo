/*
 * SensorFieldGUI.java
 *
 * Created on October 16, 2002, 2:03 PM
 */

package frodo.sensors.graphics;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionListener;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.JFrame;
import javax.swing.JScrollPane;

import frodo.CSP.CSPAgentInterface;
import frodo.CSP.CSPNodeInterface;
import frodo.communication.DistributedAgentInterface;
import frodo.sensors.Environment;
import frodo.sensors.SensorArray;
import frodo.sensors.SensorLoader;
import frodo.sensors.TargetLoader;

/**
 *
 * @author  apetcu
 */
public class SensorFieldGUI_AllTargets implements ActionListener {

	JFrame myFrame;

	SensorFieldPanel_AllTargets sfp;

	private boolean restricted_set;

	int myMagnification;

	SensorArray sa;

	Vector targets;

	Vector sensors;

	Vector nodes;

	DistributedAgentInterface myAgent;

	//	=========--------------------------------------//
	/** Creates a new instance of SensorFieldGUI */
	public SensorFieldGUI_AllTargets(DistributedAgentInterface myAgent, SensorArray sa, Vector targets, int zoom) {
		this(myAgent, sa, targets, null, zoom);
	}

	//	=========--------------------------------------//
	public SensorFieldGUI_AllTargets(DistributedAgentInterface myA, SensorArray sa, Vector t, Vector sensors, int zoom) {
		if (sensors == null)
			restricted_set = false;
		else
			restricted_set = true;
		this.sensors = sensors;
		this.sa = sa;
		this.targets = t;
		this.myMagnification = zoom;
		myAgent = myA;
		//		myAgent.getLogger().println("Problem data: " + sa.getSize() + " sensors and " + targets.size() + " targets.");
		System.out.println("Problem data: " + sa.getSize() + " sensors and " + targets.size() + " targets.");

		if (targets == null || targets.size() == 0)
			System.out.println("))))))))))))))))))))))))))))))) agent " + myA.getMyName() + " Tracking nothing");
		nodes = new Vector();
		Environment env = (Environment) myAgent.getEnvironment();

		Iterator agents = env.getAgents().values().iterator();
		while (agents.hasNext()) {
			CSPNodeInterface node = ((CSPAgentInterface) agents.next()).getCSPNode();
			nodes.add(node);
		}

		myFrame = new JFrame(myAgent.getMyName());
		myFrame.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent evt){
				//exitForm(evt);
				myFrame.dispose();
				System.exit(0);
			}
		});
		sfp = new SensorFieldPanel_AllTargets(this);
		JScrollPane jsp = new JScrollPane(sfp);
		//		jsp.setPreferredSize(new Dimension(900, 700));
		jsp.setPreferredSize(new Dimension(1150, 650));

		//            myFrame.getContentPane().setLayout(new GridLayout(1,2,10,5));//null);
		myFrame.getContentPane().setLayout(new BorderLayout());
		myFrame.getContentPane().add(jsp);
		//            myFrame.getContentPane().add(butt_panel, BorderLayout.NORTH);

		myFrame.pack();
		//        myFrame.setSize(1200,1000);
		//            Graphics g = jp.getGraphics();
		//            System.out.println("g=" + g);
		//            g.drawLine(10,10,100,100);
	}

	public void actionPerformed(java.awt.event.ActionEvent actionEvent){
		String command = actionEvent.getActionCommand();

		if (command.equalsIgnoreCase("exit")) {
			myFrame.dispose();
			myAgent.getLogger().println("Exiting..");
			System.exit(0);
		} else {
			myAgent.getLogger().println("Problematic command: " + command);
		}
	}

	//	=========--------------------------------------//
	public void displayMessage(String s){
		sfp.displayMessage(s);
	}

	//	=========--------------------------------------//
	public boolean drawRestrictedSet(){
		return restricted_set;
	}

	//	=========--------------------------------------//
	public void repaint(){
		myFrame.repaint();
	}

	//	=========--------------------------------------//
	public void startShowing(){
		myFrame.setVisible(true);
	}

	//=========--------------------------------------//
	public static void main(String[] args){
		if (args.length < 3) {
			System.out.println("Usage: testerCSP targets_file sensors_file magnification");
			System.out.println("\tmagnification = an integer saying how many times the display should be magnified (0,1,2...). Default is 1");
			System.exit(0);
		}
		String target_file = args[0];
		String sensor_file = args[1];
		int mag = Integer.parseInt(args[2]);

		frodo.sensors.Environment env = new frodo.sensors.Environment(target_file, sensor_file, 0, mag);
		TargetLoader tl = new TargetLoader(env.getLogger());
		Vector targets = tl.loadAllTargets(env.getProblemFile());
		System.out.println(targets);
		SensorLoader sl = new SensorLoader(env.getLogger());
		SensorArray sarray = sl.loadSensors(sensor_file);
		//		CSPAgent dummyA = new CSPAgent(env, -1);
		//		env.setMonitorAgent(dummyA);
		env.start();
		new SensorFieldGUI_AllTargets(null, sarray, targets, mag);
		//		sfgat.startShowing();
	}
}