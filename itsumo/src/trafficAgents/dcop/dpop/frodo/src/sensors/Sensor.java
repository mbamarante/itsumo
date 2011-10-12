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
public class Sensor {

	/**
	 * 
	 * @uml.property name="pos"
	 * @uml.associationEnd 
	 * @uml.property name="pos" multiplicity="(1 1)"
	 */
	private Position pos;

	private String name;

	private int range;

	public Sensor(String n, Position p, int r) {
		pos = p;
		name = n;
		range = r;
	}

	public Sensor(String n, int x, int y, int r) {
		pos = new Position(x, y);
		name = n;
		range = r;
	}

	public Position getPosition() {
		return pos;
	}

	public void setPosition(Position p) {
		pos = p;
	}

	/**
	 * 
	 * @uml.property name="range"
	 */
	public int getRange() {
		return range;
	}

	/**
	 * 
	 * @uml.property name="range"
	 */
	public void setRange(int r) {
		range = r;
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
	public void setName(String a) {
		name = a;
	}

	public boolean sees(Target t) {
		Position p = t.getPosition();
		int xt = p.getX();
		int yt = p.getY();
		int xs = pos.getX();
		int ys = pos.getY();
		double dist = Math.sqrt((xt - xs) * (xt - xs) + (yt - ys) * (yt - ys));
		if (dist < range)
			return true;
		else
			return false;
	}

	public int getID() {
		return Integer.parseInt(name.substring(1));
	}

	public String toString() {
		return new String(name + pos.toString());
	}
}
