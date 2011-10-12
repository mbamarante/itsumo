/*
 * Created on 27 mai 2005
 *
 */
package frodo.interaction.meeting;

import java.util.Vector;

/**
 * @author gushakov
 *
 */
public class TimeGrid {
	private Integer[] grid = null;

	/**
	 * Default time grid, spans 1 through 8 (like eight hours in the day).
	 * Date is set for an arbitrary day.
	 */
	public TimeGrid() {
		grid = new Integer[8];
		grid[0] = new Integer(1);
		grid[1] = new Integer(2);
		grid[2] = new Integer(3);
		grid[3] = new Integer(4);
		grid[4] = new Integer(5);
		grid[5] = new Integer(6);
		grid[6] = new Integer(7);
		grid[7] = new Integer(8);
	}

	/**
	 * Custom time grid for a specified range of hours in the day.
	 * @param slots
	 */
	public TimeGrid(Integer[] slots) {
		grid = slots;
	}

	public TimeGrid(Object[] slots) {
		grid = new Integer[slots.length];
		for (int i = 0; i < slots.length; i++) {
			grid[i] = (Integer) slots[i];
		}
	}

	public TimeGrid(int begin, int end) {
		int size = end - begin + 1;
		grid = new Integer[size];
		for (int i = 0; i < size; i++) {
			grid[i] = new Integer(begin + i);

		}
	}

	public int getNumberOfTimeSlots() {
		return grid.length;
	}

	/**
	 * Returns the begining of this time grid.
	 */
	public Integer getStartSlot() {
		return grid[0];
	}

	/**
	 * Return the end of this time grid.
	 */
	public Integer getEndSlot() {
		return grid[grid.length - 1];
	}

	/**
	 * Returns true if this time grid overlaps the given time grid.
	 * @param tg A TimeGrid object
	 */
	public boolean overlaps(TimeGrid tg) {

		if (getEndSlot().compareTo(tg.getStartSlot()) < 0 || getStartSlot().compareTo(tg.getEndSlot()) > 0) {
			return false;
		}
		return true;
	}

	/**
	 * Returns the index of slot in this time grid.
	 * @return
	 */
	public int getIndexForSlot(Integer slot) {
		int index = -1;
		boolean found = false;
		int i = 0;
		while (i < grid.length && !found) {
			if (grid[i].compareTo(slot) == 0) {
				found = true;
				index = i;
			}
			i++;
		}
		return index;
	}

	public Integer getSlotAt(int index) {
		return grid[index];
	}

	public boolean hasSlot(int index) {
		if (index >= 0 && index < grid.length) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Returns a time grid consisting of the overlapping time
	 * slots between this time grid and a given time grid.
	 * @return
	 */
	public TimeGrid getOverlappingSlots(TimeGrid tg) {
		Vector result = null;
		if (overlaps(tg)) {
			result = new Vector();
			TimeGrid from = null;
			TimeGrid to = null;
			int toIndex = 0;
			if (getStartSlot().compareTo(tg.getStartSlot()) >= 0) {
				from = this;
				to = tg;
				toIndex = tg.getIndexForSlot(getStartSlot());
			} else {
				from = tg;
				to = this;
				toIndex = getIndexForSlot(tg.getStartSlot());
			}

			int fromIndex = 0;
			while (from.hasSlot(fromIndex) && to.hasSlot(toIndex) && from.getSlotAt(fromIndex).compareTo(to.getSlotAt(toIndex)) == 0) {
				result.add(from.getSlotAt(fromIndex));
				fromIndex++;
				toIndex++;
			}
		}
		return new TimeGrid(result.toArray());
	}

	public Vector getTimeSlots() {
		Vector slots = new Vector();
		for (int i = 0; i < grid.length; i++) {
			slots.add(grid[i]);
		}
		return slots;
	}

	public static TimeGrid getCombinedGrid(TimeGrid tg1, TimeGrid tg2) {
		int start = Math.min(tg1.getStartSlot().intValue(), tg2.getStartSlot().intValue());
		int end = Math.max(tg1.getEndSlot().intValue(), tg2.getEndSlot().intValue());
		return new TimeGrid(start, end);
	}

	public String toString() {
		String out = "[";
		for (int i = 0; i < grid.length; i++) {
			out += grid[i].toString() + (i < grid.length - 1 ? ", " : "");
		}
		out += "]";
		return out;
	}

	public static void main(String[] args) {
		Object[] s1 = { new Integer(1), new Integer(2), new Integer(3), new Integer(4), new Integer(5), new Integer(6), new Integer(7) };
		Object[] s2 = { new Integer(4), new Integer(5), new Integer(6), new Integer(7), new Integer(8), new Integer(9), new Integer(10) };
		TimeGrid tg1 = new TimeGrid(s1);
		TimeGrid tg2 = new TimeGrid(s2);
		System.out.println(tg1.toString());
		System.out.println(tg2.toString());
		System.out.println("overlaps: " + tg1.getOverlappingSlots(tg2).toString());
		System.out.println("combined: " + TimeGrid.getCombinedGrid(tg1, tg2).toString());
	}

	//--------------------------------------------
}
