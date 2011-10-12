/*
 * Position.java
 *
 * Created on October 15, 2002, 5:15 PM
 */

package frodo.sensors;

/**
 *
 * @author  apetcu
 */
public class Position {

	private int x = 0;

	private int y = 0;

	public Position(int x, int y) {
		this.x = x;
		this.y = y;
	}

	/**
	 * 
	 * @uml.property name="x"
	 */
	public int getX() {
		return x;
	}

	/**
	 * 
	 * @uml.property name="y"
	 */
	public int getY() {
		return y;
	}

	/**
	 * 
	 * @uml.property name="x"
	 */
	public void setX(int x) {
		this.x = x;
	}

	/**
	 * 
	 * @uml.property name="y"
	 */
	public void setY(int y) {
		this.y = y;
	}

	public String toString() {
		return new String("(" + x + "," + y + ")");
	}
}
