package frodo.CSP;

import java.io.Serializable;
import java.util.Vector;

/**
 * @author apetcu
 * 
 * To change this generated comment edit the template variable "typecomment": Window>Preferences>Java>Templates. To enable and disable the creation of
 * type comments go to Window>Preferences>Java>Code Generation.
 */

public interface CSPNodeInterface extends Serializable, Cloneable {
	public abstract int getId();

	public abstract Vector getConstraints();

	public abstract Vector getNeighbours();

	public abstract Vector getVariables();

	public abstract void setConstraints(Vector constr);

	public abstract void setNeighbours(Vector neighbors);

	public abstract void setVariables(Vector vars);

	public abstract int getNumberOfConstraints();

	public abstract int getNumberOfNeighbours();

	public abstract int getNumberOfVariables();

	public abstract void addConstraint(Object constr);

	public abstract void addNeighbour(Object node);

	public abstract void addVariable(Object var);

	public abstract boolean deleteConstraint(Object constr);

	public abstract boolean deleteNeighbour(Object node);

	public abstract boolean deleteVariable(Object var);

	public abstract ComplexValue getComplexValue();

	//	public abstract ComplexVariable getComplexVariable();
	//	public abstract BinaryConstraint getConstraint(int i);
}