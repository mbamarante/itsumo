/*
 * Created on Feb 1, 2006 by apetcu
 * Project Public
 * Package frodo.OCOP.DPOP
 */
package frodo.OCOP.graph;

public class DFSStatistics {
	private long stats[];

	static String names[] = new String[] { "Root                   ", "Width                  ", "Depth                  ", "Max Size               ", "Node with max msg size ",
			"Total Size             ", "Number of UTIL messages" };

	public DFSStatistics(int root, long width, long depth, long maxSize, int maxSizeNode, long totalSize, long numberMessages) {
		stats = new long[7];
//		names = new String[7];

		stats[0] = root;//this is the root of the current DFS
		stats[1] = width;//this is the width of the problem
		stats[2] = depth;//this is the depth of the problem
		stats[3] = maxSize;//max UTIL message size: in the meeting scheduling with 8 values, this is 8^width
		stats[4] = maxSizeNode;//this is the width of the problem
		stats[5] = totalSize;//total size of UTIL messages
		stats[6] = numberMessages;//

		//		names[0] = "Root                   ";
		//		names[1] = "Width                  ";
		//		names[2] = "Depth                  ";
		//		names[3] = "Max Size               ";
		//		names[4] = "Node with max msg size ";
		//		names[5] = "Total Size             ";
		//		names[6] = "Number of UTIL messages";
	}

	public String toString() {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < stats.length; i++)
			sb.append(names[i] + "\t=" + stats[i] + "\n");
		return sb.toString();
	}

	public static String namestoString() {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < names.length; i++)
			sb.append(names[i] + "\t");
		return sb.toString();
	}

	public String toStringPlain() {
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < stats.length; i++)
			sb.append(stats[i] + "\t");
		return sb.toString();
	}

	public int getRoot() {
		return (int) stats[0];
	}

	public int getWidth() {
		return (int) stats[1];
	}

	public long getDepth() {
		return (int) stats[2];
	}

	public long getMaxSize() {
		return (int) stats[3];
	}

	public int getNodeWithMaxSize() {
		return (int) stats[4];
	}

	public long getTotalSize() {
		return stats[5];
	}

	public long getNumberOfUtilMesssages() {
		return stats[6];
	}
}
