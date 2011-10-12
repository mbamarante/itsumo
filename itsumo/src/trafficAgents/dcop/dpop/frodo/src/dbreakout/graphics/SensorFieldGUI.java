/*
 * SensorFieldGUI.java
 *
 * Created on October 16, 2002, 2:03 PM
 */

package frodo.dbreakout.graphics;

import java.awt.BorderLayout;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import frodo.communication.DistributedAgentInterface;
import frodo.sensors.SensorArray;
import frodo.sensors.Target;
import frodo.utils.Logger;

/**
 *
 * @author  apetcu
 */
public class SensorFieldGUI implements ActionListener{

	/**
	 * 
	 * @uml.property name="myLogger"
	 * @uml.associationEnd 
	 * @uml.property name="myLogger" multiplicity="(1 1)"
	 */
	Logger myLogger;

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
	 * @uml.property name="sfp" multiplicity="(1 1)" inverse="sfg:testbed.dbreakout.graphics.SensorFieldPanel"
	 */
	SensorFieldPanel sfp;

	/**
	 * 
	 * @uml.property name="butt_panel"
	 * @uml.associationEnd 
	 * @uml.property name="butt_panel" multiplicity="(1 1)"
	 */
	JPanel butt_panel;

    private boolean restricted_set;

	/**
	 * 
	 * @uml.property name="sa"
	 * @uml.associationEnd 
	 * @uml.property name="sa" multiplicity="(1 1)"
	 */
	SensorArray sa;

	/**
	 * 
	 * @uml.property name="target"
	 * @uml.associationEnd 
	 * @uml.property name="target" multiplicity="(1 1)"
	 */
	Target target;

	/**
	 * 
	 * @uml.property name="sensors"
	 * @uml.associationEnd 
	 * @uml.property name="sensors" multiplicity="(0 -1)" elementType="java.lang.Integer"
	 */
	Vector sensors;

	/**
	 * 
	 * @uml.property name="myAgent"
	 * @uml.associationEnd 
	 * @uml.property name="myAgent" multiplicity="(1 1)"
	 */
	DistributedAgentInterface myAgent;

	/**
	 * 
	 * @uml.property name="jsp"
	 * @uml.associationEnd 
	 * @uml.property name="jsp" multiplicity="(1 1)"
	 */
	JScrollPane jsp;

    
    /** Creates a new instance of SensorFieldGUI */
    public SensorFieldGUI(DistributedAgentInterface myAgent, SensorArray sa, Target t) {
        this(myAgent,sa,t,null);
    }
    public SensorFieldGUI(DistributedAgentInterface myA, SensorArray sa, Target t, Vector sensors) {
        myLogger = myA.getLogger();
        if(sensors==null)
            restricted_set=false;
        else
            restricted_set=true;
        this.sensors = sensors;
        this.sa = sa;
        this.target = t;
        if(target==null)
            myLogger.println("))))))))))))))))))))))))))))))) agent " + myA.getMyName() + " Tracking nothing");
        myAgent = myA;
        f = new JFrame(myA.getMyName());
        f.addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                //exitForm(evt);
                f.dispose();
                System.exit(0);
            }
        }
        );
        sfp = new SensorFieldPanel(this);
        butt_panel = new JPanel();
        butt_panel.add(new JLabel("Sensors/ranges"));
        JButton exitb = new JButton("Exit");
        exitb.setActionCommand("Exit");
        exitb.addActionListener(this);
        butt_panel.add(exitb);
        
        f.getContentPane().setLayout(new BorderLayout());
        sfp.add(butt_panel);
        jsp = new JScrollPane((JPanel)sfp);
//        jsp.setPreferredSize(new Dimension(900,700));

        f.getContentPane().add(jsp);
        f.pack();
        //        f.setSize(1000,1000);
        if(target==null)
            startShowing();
    }
    
    public void actionPerformed(java.awt.event.ActionEvent actionEvent) {
        String command = actionEvent.getActionCommand();
        
        if (command.equals("Exit")) {
            System.exit(0);
        }
    }
    public boolean drawRestrictedSet(){
        return restricted_set;
    }
    public void repaint(){
        f.repaint();
    }
    public void startShowing(){
        f.setVisible(true);
    }
    public void displayMessage(String s){
	sfp.displayMessage(s);
    }
}
