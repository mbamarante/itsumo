/*
 * Target.java
 *
 * Created on October 15, 2002, 4:33 PM
 */

package frodo.sensors;

/**
 *
 * @author  apetcu
 */
public class Target {

	/**
	 * 
	 * @uml.property name="pos"
	 * @uml.associationEnd 
	 * @uml.property name="pos" multiplicity="(1 1)"
	 */
	private Position pos;

	private String name;

	private int id;

	/** Creates a new instance of Target */
	public Target(int x, int y, String nm) {
		pos = new Position(x, y);
		name = nm;
		id = Integer.parseInt(nm.substring(1));
	}

	public Target(Position p, String nm) {
		pos = p;
		name = nm;
	}

	public Position getPosition() {
		return pos;
	}

	public void setPosition(Position p) {
		pos = p;
	}

	/**
	 * 
	 * @uml.property name="name"
	 */
	public String getName() {
		return name;
	}

	/**
	 * 
	 * @uml.property name="name"
	 */
	public void setName(String n) {
		name = n;
	}

	public String toString() {
		return new String(name + pos.toString());
	}

	/**
	 * @return Returns the id.
	 */
	public int getId() {
		return id;
	}

	/**
	 * @param id The id to set.
	 */
	public void setId(int id) {
		this.id = id;
	}
}
