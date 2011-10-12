/*
 * Created on 30 mai 2005
 *
 */
package frodo.utils;

import java.util.Vector;

/**
 * @author gushakov
 *
 */
public class MeetingUtils {

	public static void sort(Vector a) {
		int p = 0;
		int r = a.size() - 1;
		quickSort(a, p, r);
	}

	/**
	 * Quicksort algorithm [ref. CLRS 2nd edition].
	 */
	private static void quickSort(Vector a, int p, int r) {
		if (p < r) {
			int q = partition(a, p, r);
			quickSort(a, p, q - 1);
			quickSort(a, q + 1, r);
		}
	}

	private static int partition(Vector a, int p, int r) {
		int x = ((Integer) a.get(r)).intValue();
		int i = p - 1;
		for (int j = p; j < r; j++) {
			int aj = ((Integer) a.get(j)).intValue();
			if (aj <= x) {
				i++;
				Object temp1 = a.set(i, a.get(j));
				a.set(j, temp1);
			}
		}
		Object temp2 = a.set(i + 1, a.get(r));
		a.set(r, temp2);
		return i + 1;
	}

}
