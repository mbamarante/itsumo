/*
 * Created on Jul 9, 2005
 *
 */
package frodo.monitoring.interfaces;

/**
 * @author ushakov
 *
 */
public class OCOPSelectionEvent {

	public static final int NEW_SELECTION_MEETING = 0;

	public static final int NEW_SELECTION_PARTICIPANT = 1;

	public static final int SELECTION_CLEARED = 2;

	int type;

	int[] ids;

	/**
	 * Constructs a selection chaged event.
	 */
	public OCOPSelectionEvent(int type, int[] ids) {
		this.type = type;
		this.ids = ids;
	}

	public int[] getIds() {
		return ids;
	}

	public void setIds(int[] ids) {
		this.ids = ids;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}
}
