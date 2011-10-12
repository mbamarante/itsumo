/*
 * Created on Nov 30, 2004 by apetcu - Adrian.Petcu@epfl.ch
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package frodo.OCOP.DPOP.sensors;

import java.util.Vector;

import frodo.COP.ValuedConstraint;
import frodo.CSP.CSPVariable;
import frodo.CSP.VariableId;
import frodo.OCOP.OCOPAbstractNode;
import frodo.OCOP.OCOPBasicNode;
import frodo.OCOP.OCOPBasicProblem;
import frodo.OCOP.communication.Environment;
import frodo.sensors.SensorArray;
import frodo.sensors.SensorLoader;
import frodo.sensors.Target;
import frodo.sensors.TargetLoader;
import frodo.utils.HelperUtilities;


/**
 * @author apetcu - Adrian.Petcu@epfl.ch
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class SensorEnvironment extends Environment {
	String	sensorFile;
	public SensorEnvironment(String tf, String sf, int mode, int mag) {
		super(tf, mode, mag);
		sensorFile = sf;
	}

	public OCOPBasicProblem loadProblem() {
		System.out.println("Loading problem from: " + sensorFile + " and " + problemFile + ".");
		TargetLoader tl = new TargetLoader(this.getLogger());
		Vector targets = tl.loadAllTargets(this.getProblemFile());
		SensorLoader sl = new SensorLoader(this.getLogger());
		SensorArray sarray = sl.loadSensors(sensorFile);
		OCOPBasicProblem prob = new OCOPBasicProblem();

		for (int i = 0; i < targets.size(); i++) {
			Target trg = (Target) targets.elementAt(i);
			int id = trg.getId();
			OCOPAbstractNode nd = new OCOPBasicNode(null, id, null);
//			OCOPAgent ag = new OCOPAgent(this, id);
			//here I am adding only 1 variable, but in future there should be 3, one for each sensor to be allocated.
			VariableId varId = new VariableId(id, 0);
			//				System.out.print("Constructing variable " + varId);
			CSPVariable var = new CSPVariable(varId);
			var.setDomain(sarray.getCompatibleSensors(trg));
			nd.addVariable(var);
			prob.addNode(nd);
			prob.addVariable(var);
//			ag.setNode(nd);
		}
		for (int i = 0; i < targets.size() - 1; i++) {
			OCOPAbstractNode nd1 = prob.getNodeById(i);
			CSPVariable var1 = (CSPVariable) nd1.getVariable();
			Vector dom1 = var1.getDomain();
			for (int j = i + 1; j < targets.size(); j++) {
				OCOPAbstractNode nd2 = prob.getNodeById(j);
				CSPVariable var2 = (CSPVariable) nd2.getVariable();
				Vector dom2 = var2.getDomain();
				if (HelperUtilities.vectorsIntersect(dom1, dom2)) {
//					System.out.println(nd1.getId() + " is neighbor with " + nd2.getId() +  " because they share ");
					nd1.addNeighbour(nd2);
					nd2.addNeighbour(nd1);
					ValuedConstraint bc = new ValuedConstraint(var1, var2);
					var1.addConstraint(bc);
					prob.addConstraint(bc);
					nd1.addConstraint(bc);
					nd2.addConstraint(bc);
				}
			}
		}
		return prob;
	}
}