/*
 * Created on Jul 28, 2005
 *
 */
package frodo.testing;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Random;

import frodo.OCOP.OCOPBasicProblem;
import frodo.interaction.meeting.graph.Heuristics;
import frodo.interaction.meeting.gui.RandomMeetingGenerator;
import frodo.interaction.meeting.gui.RootTreeNode;
import frodo.interaction.meeting.roundtrip.DPOPMaker;

/**
 * @author ushakov
 *
 */
public class TestCase1 {

	static RootTreeNode root = null;

	static FileWriter fw = null;

	final static int MINIMUM = 0;

	final static int AVERAGE = 1;

	final static int EACH = 2;
/*
	public static void test1(String file, int numOfMeetings, int numOfParticipants, int numOfAttendances, int maxRange, boolean sameForAll, int numOfRuns, int numOfRandomizations,
			boolean oneTree, boolean oneFile, int stats){
		try {
			if (fw == null || !oneFile) {
				fw = new FileWriter(file);
				fw.write("#Testing " + numOfMeetings + " meetings, " + numOfParticipants + " participants, " + numOfAttendances + " attendances\n");
				fw.write("#DFS MCN vs. DFS CLIQUE vs. DFS randomized\n");
				fw.write("#--------------------------------------------------\n");
				fw.write("#width MCN\twidth CLIQUE\twidth DFS" + "\theight MCN\theight CLIQUE\theight DFS" + "\tHC size MCN\tHC size CLIQUE\tHC size DFS\n");
				fw.flush();
			}
			for (int i = 0; i < numOfRuns; i++) {
				RandomMeetingGenerator generator = new RandomMeetingGenerator();

				if (root == null || !oneTree) {
					root = generator.getTestTree(numOfMeetings, 1, maxRange, sameForAll, numOfParticipants, 1, numOfAttendances, false);
				}
				if (root != null) {
					DPOPMaker maker = new DPOPMaker(root);
					OCOPBasicProblem ocop = maker.getOCOP();

					int totWidthMCN, totWidthClique, totWidthDFS, totHeightMCN, totHeightClique, totHeightDFS, minWidthMCN, minWidthClique, minWidthDFS, minHeightMCN, minHeightClique, minHeightDFS;

					totWidthMCN = totWidthClique = totWidthDFS = 0;
					totHeightMCN = totHeightClique = totHeightDFS = 0;
					minWidthMCN = minWidthClique = minWidthDFS = Integer.MAX_VALUE;
					minHeightMCN = minHeightClique = minHeightDFS = Integer.MAX_VALUE;

					long totHCSizeMCN, totHCSizeClique, totHCSizeDFS, minHCSizeMCN, minHCSizeClique, minHCSizeDFS;

					totHCSizeMCN = totHCSizeClique = totHCSizeDFS = 0;
					minHCSizeMCN = minHCSizeClique = minHCSizeDFS = Long.MAX_VALUE;

					for (int j = 0; j < numOfRandomizations; j++) {
						int widthMCN, widthClique, widthDFS, heightMCN, heightClique, heightDFS;
						long hcSizeMCN, hcSizeClique, hcSizeDFS;

						//--------------------------------------------------------------------
						//DFS MCN testing
						ocop.randomizeNodeOrder();
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN;
						int[] info = Heuristics.buildPseudoTree(ocop);
						widthMCN = info[0];
						heightMCN = ocop.getCurrentDFSHeight();
						hcSizeMCN = ocop.getTotalUTILPayload();
						totWidthMCN += widthMCN;
						totHeightMCN += heightMCN;
						totHCSizeMCN += hcSizeMCN;
						if (widthMCN < minWidthMCN) {
							minWidthMCN = widthMCN;
						}
						if (heightMCN < minHeightMCN) {
							minHeightMCN = heightMCN;
						}
						if (hcSizeMCN < minHCSizeMCN) {
							minHCSizeMCN = hcSizeMCN;
						}

						//--------------------------------------------------------------------
						//DFS CLIQUE MCN testing
						ocop.randomizeNodeOrder();
						ocop.randomizeNodeOrder();
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_CLIQUE_MCN;
						info = Heuristics.buildPseudoTree(ocop);
						widthClique = info[0];
						heightClique = ocop.getCurrentDFSHeight();
						hcSizeClique = ocop.getTotalUTILPayload();
						totWidthClique += widthClique;
						totHeightClique += heightClique;
						totHCSizeClique += hcSizeClique;
						if (widthClique < minWidthClique) {
							minWidthClique = widthClique;
						}
						if (heightClique < minHeightClique) {
							minHeightClique = heightClique;
						}
						if (hcSizeClique < minHCSizeClique) {
							minHCSizeClique = hcSizeClique;
						}

						//--------------------------------------------------------------------
						//DFS Randomized testing
						ocop.randomizeNodeOrder();
						int numOfNodes = ocop.getNumberOfNodes();
						int nextDFSRoot = new Random(System.currentTimeMillis()).nextInt(numOfNodes);
						Heuristics.CURRENT_DFS_ROOT_ID = nextDFSRoot;
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS;
						info = Heuristics.buildPseudoTree(ocop);
						widthDFS = info[0];
						heightDFS = ocop.getCurrentDFSHeight();
						hcSizeDFS = ocop.getTotalUTILPayload();
						totWidthDFS += widthDFS;
						totHeightDFS += heightDFS;
						totHCSizeDFS += hcSizeDFS;
						if (widthDFS < minWidthDFS) {
							minWidthDFS = widthDFS;
						}
						if (heightDFS < minHeightDFS) {
							minHeightDFS = heightDFS;
						}
						if (hcSizeDFS < minHCSizeDFS) {
							minHCSizeDFS = hcSizeDFS;
						}

						if (stats == EACH) {
							fw.write(widthMCN + "\t" + widthClique + "\t" + widthDFS + "\t" + heightMCN + "\t" + heightClique + "\t" + heightDFS + "\t" + hcSizeMCN + "\t"
									+ hcSizeClique + "\t" + hcSizeDFS + "\n");

							fw.flush();

						}

					}

					if (stats == AVERAGE) {
						fw.write((double) totWidthMCN / (double) numOfRandomizations + "\t" + (double) totWidthClique / (double) numOfRandomizations + "\t" + (double) totWidthDFS
								/ (double) numOfRandomizations + "\t" + (double) totHeightMCN / (double) numOfRandomizations + "\t" + (double) totHeightClique
								/ (double) numOfRandomizations + "\t" + (double) totHeightDFS / (double) numOfRandomizations + "\t" + (double) totHCSizeMCN
								/ (double) numOfRandomizations + "\t" + (double) totHCSizeClique / (double) numOfRandomizations + "\t" + (double) totHCSizeDFS
								/ (double) numOfRandomizations + "\n");

						fw.flush();
					}
					if (stats == MINIMUM) {
						fw.write(minWidthMCN + "\t" + minWidthClique + "\t" + minWidthDFS + "\t" + minHeightMCN + "\t" + minHeightClique + "\t" + minHeightDFS + "\t"
								+ minHCSizeMCN + "\t" + minHCSizeClique + "\t" + minHCSizeDFS + "\n");

						fw.flush();
					}
				}

			}
		} catch (Exception e) {
		}
		System.out.println("Done with test 1.");
	}

	public static void test2(String file, int numOfMeetings, int numOfParticipants, int numOfAttendances, int maxRange, boolean sameForAll, int numOfRuns, int numOfRandomizations,
			boolean oneTree, boolean oneFile, int stats){
		try {
			if (fw == null || !oneFile) {
				fw = new FileWriter(file);
				fw.write("#Testing " + numOfMeetings + " meetings, " + numOfParticipants + " participants, " + numOfAttendances + " attendances\n");
				fw.write("#DFS MCN vs. DFS CLIQUE vs. DFS randomized\n");
				fw.write("#--------------------------------------------------\n");
				fw.write("#total HC size MCN\ttotal HC size CLIQUE\ttotal HC size DFS\ttotal HC size DSTB\n");
				fw.flush();
			}
			for (int i = 0; i < numOfRuns; i++) {
				RandomMeetingGenerator generator = new RandomMeetingGenerator();

				if (root == null || !oneTree) {
					root = generator.getTestTree(numOfMeetings, 1, maxRange, sameForAll, numOfParticipants, 1, numOfAttendances, false);
				}
				if (root != null) {
					DPOPMaker maker = new DPOPMaker(root);
					OCOPBasicProblem ocop = maker.getOCOP();

					long totHCSizeMCN, totHCSizeClique, totHCSizeDFS, totHCSizeDSTB, minHCSizeMCN, minHCSizeClique, minHCSizeDFS, minHCSizeDSTB;

					totHCSizeMCN = totHCSizeClique = totHCSizeDFS = totHCSizeDSTB = 0;
					minHCSizeMCN = minHCSizeClique = minHCSizeDFS = minHCSizeDSTB = Long.MAX_VALUE;

					for (int j = 0; j < numOfRandomizations; j++) {
						long hcSizeMCN, hcSizeClique, hcSizeDFS, hcSizeDSTB;

						//--------------------------------------------------------------------
						//DFS MCN testing
						ocop.randomizeNodeOrder();
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN;
						int[] info = Heuristics.buildPseudoTree(ocop);
						hcSizeMCN = ocop.getTotalUTILPayload();
						totHCSizeMCN += hcSizeMCN;
						if (hcSizeMCN < minHCSizeMCN) {
							minHCSizeMCN = hcSizeMCN;
						}

						//--------------------------------------------------------------------
						//DFS CLIQUE MCN testing
						ocop.randomizeNodeOrder();
						ocop.randomizeNodeOrder();
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_CLIQUE_MCN;
						info = Heuristics.buildPseudoTree(ocop);
						hcSizeClique = ocop.getTotalUTILPayload();
						totHCSizeClique += hcSizeClique;
						if (hcSizeClique < minHCSizeClique) {
							minHCSizeClique = hcSizeClique;
						}

						//--------------------------------------------------------------------
						//DFS Randomized testing
						ocop.randomizeNodeOrder();
						int numOfNodes = ocop.getNumberOfNodes();
						int nextDFSRoot = new Random(System.currentTimeMillis()).nextInt(numOfNodes);
						Heuristics.CURRENT_DFS_ROOT_ID = nextDFSRoot;
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS;
						info = Heuristics.buildPseudoTree(ocop);
						hcSizeDFS = ocop.getTotalUTILPayload();
						totHCSizeDFS += hcSizeDFS;
						if (hcSizeDFS < minHCSizeDFS) {
							minHCSizeDFS = hcSizeDFS;
						}

						//--------------------------------------------------------------------
						//DFS MCN  DSTB testing
						ocop.randomizeNodeOrder();
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN_DSTB;
						info = Heuristics.buildPseudoTree(ocop);
						hcSizeDSTB = ocop.getTotalUTILPayload();
						totHCSizeDSTB += hcSizeDSTB;
						if (hcSizeDSTB < minHCSizeDSTB) {
							minHCSizeDSTB = hcSizeDSTB;
						}

						if (stats == EACH) {
							fw.write(hcSizeMCN + "\t" + hcSizeClique + "\t" + hcSizeDFS + "\t" + hcSizeDSTB + "\n");

							fw.flush();

						}

					}

					if (stats == AVERAGE) {
						fw.write((double) totHCSizeMCN / (double) numOfRandomizations + "\t" + (double) totHCSizeClique / (double) numOfRandomizations + "\t"
								+ (double) totHCSizeDFS / (double) numOfRandomizations + "\t" + (double) totHCSizeDSTB / (double) numOfRandomizations + "\n");

						fw.flush();
					}
					if (stats == MINIMUM) {
						fw.write(minHCSizeMCN + "\t" + minHCSizeClique + "\t" + minHCSizeDFS + "\t" + minHCSizeDSTB + "\n");

						fw.flush();
					}
				}

			}
		} catch (Exception e) {
		}
		System.out.println("Done with test 1.");
	}

	public static void test3(String file, int numOfMeetings, int numOfParticipants, int minNumOfAttendances, int numOfAttendances, boolean fixedForAll, int maxRange,
			boolean sameForAll, int numOfRuns, int numOfRandomizations, boolean oneTree, boolean oneFile, int stats){
		try {
			if (fw == null || !oneFile) {
				fw = new FileWriter(file);
				fw.write("#Testing " + numOfMeetings + " meetings, " + numOfParticipants + " participants, " + numOfAttendances + " attendances\n");
				fw.write("#DFS MCN vs. DFS MCN BEC vs. DFS randomized\n");
				fw.write("#--------------------------------------------------\n");
				fw.write("#width MCN\twidth BEC\twidth DFS" + "\theight MCN\theight BEC\theight DFS" + "\tHC size MCN\tHC size BEC\tHC size DFS\n");
				fw.flush();
			}
			for (int i = 0; i < numOfRuns; i++) {
				RandomMeetingGenerator generator = new RandomMeetingGenerator();

				if (root == null || !oneTree) {
					root = generator.getTestTree(numOfMeetings, 1, maxRange, sameForAll, numOfParticipants, minNumOfAttendances, numOfAttendances, fixedForAll);
				}
				if (root != null) {
					DPOPMaker maker = new DPOPMaker(root);
					OCOPProblem ocop = maker.getOCOP();

					int totWidthMCN, totWidthBEC, totWidthDFS, totHeightMCN, totHeightBEC, totHeightDFS, minWidthMCN, minWidthBEC, minWidthDFS, minHeightMCN, minHeightBEC, minHeightDFS;

					totWidthMCN = totWidthBEC = totWidthDFS = 0;
					totHeightMCN = totHeightBEC = totHeightDFS = 0;
					minWidthMCN = minWidthBEC = minWidthDFS = Integer.MAX_VALUE;
					minHeightMCN = minHeightBEC = minHeightDFS = Integer.MAX_VALUE;

					long totHCSizeMCN, totHCSizeBEC, totHCSizeDFS, minHCSizeMCN, minHCSizeBEC, minHCSizeDFS;

					totHCSizeMCN = totHCSizeBEC = totHCSizeDFS = 0;
					minHCSizeMCN = minHCSizeBEC = minHCSizeDFS = Long.MAX_VALUE;

					for (int j = 0; j < numOfRandomizations; j++) {
						int widthMCN, widthBEC, widthDFS, heightMCN, heightBEC, heightDFS;
						long hcSizeMCN, hcSizeBEC, hcSizeDFS;

						//--------------------------------------------------------------------
						//DFS MCN testing
						ocop.randomizeNodeOrder();
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN;
						int[] info = Heuristics.buildPseudoTree(ocop);
						widthMCN = info[0];
						heightMCN = ocop.getCurrentDFSHeight();
						hcSizeMCN = ocop.getTotalUTILPayload();
						totWidthMCN += widthMCN;
						totHeightMCN += heightMCN;
						totHCSizeMCN += hcSizeMCN;
						if (widthMCN < minWidthMCN) {
							minWidthMCN = widthMCN;
						}
						if (heightMCN < minHeightMCN) {
							minHeightMCN = heightMCN;
						}
						if (hcSizeMCN < minHCSizeMCN) {
							minHCSizeMCN = hcSizeMCN;
						}

						//--------------------------------------------------------------------
						//DFS MCN BEC testing
						ocop.randomizeNodeOrder();
						ocop.randomizeNodeOrder();
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS_MCN_BEC;
						info = Heuristics.buildPseudoTree(ocop);
						widthBEC = info[0];
						heightBEC = ocop.getCurrentDFSHeight();
						hcSizeBEC = ocop.getTotalUTILPayload();
						totWidthBEC += widthBEC;
						totHeightBEC += heightBEC;
						totHCSizeBEC += hcSizeBEC;
						if (widthBEC < minWidthBEC) {
							minWidthBEC = widthBEC;
						}
						if (heightBEC < minHeightBEC) {
							minHeightBEC = heightBEC;
						}
						if (hcSizeBEC < minHCSizeBEC) {
							minHCSizeBEC = hcSizeBEC;
						}

						//--------------------------------------------------------------------
						//DFS Randomized testing
						ocop.randomizeNodeOrder();
						int numOfNodes = ocop.getNumberOfNodes();
						int nextDFSRoot = new Random(System.currentTimeMillis()).nextInt(numOfNodes);
						Heuristics.CURRENT_DFS_ROOT_ID = nextDFSRoot;
						Heuristics.CURRENT_TREE_BUILD_METHOD = Heuristics.DFS;
						info = Heuristics.buildPseudoTree(ocop);
						widthDFS = info[0];
						heightDFS = ocop.getCurrentDFSHeight();
						hcSizeDFS = ocop.getTotalUTILPayload();
						totWidthDFS += widthDFS;
						totHeightDFS += heightDFS;
						totHCSizeDFS += hcSizeDFS;
						if (widthDFS < minWidthDFS) {
							minWidthDFS = widthDFS;
						}
						if (heightDFS < minHeightDFS) {
							minHeightDFS = heightDFS;
						}
						if (hcSizeDFS < minHCSizeDFS) {
							minHCSizeDFS = hcSizeDFS;
						}

						if (stats == EACH) {
							fw.write(widthMCN + "\t" + widthBEC + "\t" + widthDFS + "\t" + heightMCN + "\t" + heightBEC + "\t" + heightDFS + "\t" + hcSizeMCN + "\t" + hcSizeBEC
									+ "\t" + hcSizeDFS + "\n");

							fw.flush();

						}

					}

					if (stats == AVERAGE) {
						fw.write((double) totWidthMCN / (double) numOfRandomizations + "\t" + (double) totWidthBEC / (double) numOfRandomizations + "\t" + (double) totWidthDFS
								/ (double) numOfRandomizations + "\t" + (double) totHeightMCN / (double) numOfRandomizations + "\t" + (double) totHeightBEC
								/ (double) numOfRandomizations + "\t" + (double) totHeightDFS / (double) numOfRandomizations + "\t" + (double) totHCSizeMCN
								/ (double) numOfRandomizations + "\t" + (double) totHCSizeBEC / (double) numOfRandomizations + "\t" + (double) totHCSizeDFS
								/ (double) numOfRandomizations + "\n");

						fw.flush();
					}
					if (stats == MINIMUM) {
						fw.write(minWidthMCN + "\t" + minWidthBEC + "\t" + minWidthDFS + "\t" + minHeightMCN + "\t" + minHeightBEC + "\t" + minHeightDFS + "\t" + minHCSizeMCN
								+ "\t" + minHCSizeBEC + "\t" + minHCSizeDFS + "\n");

						fw.flush();
					}
				}

			}
		} catch (Exception e) {
		}
		System.out.println("Done with test 3.");
	}

	public static void average(String fileName, int indexStart, int indexEnd, int numOfValues){
		try {
			FileWriter fw = new FileWriter("output/testcase_average");
			for (int i = indexStart; i <= indexEnd; i++) {
				System.out.println("processing: " + fileName + i);
				BufferedReader reader = new BufferedReader(new FileReader(fileName + i));
				String line = reader.readLine().trim();
				double[] values = new double[numOfValues];
				for (int j = 0; j < numOfValues; j++) {
					values[j] = 0;
				}
				int counter = 0;
				while (line != null) {
					line = line.trim();
					if (!line.startsWith("#")) {
						String[] strings = line.split("\\s");
						for (int j = 0; j < strings.length; j++) {
							values[j] += Double.parseDouble(strings[j]);
						}
						counter++;
					}
					line = reader.readLine();
				}

				for (int j = 0; j < values.length; j++) {
					fw.write(values[j] / (double) counter + "\t");
				}
				fw.write("\n");
				fw.flush();

			}
			System.out.println("Done.");
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(0);
		}

	}

	public static void main(String[] args){

		test3("output/testcase_4_16", //output file
				5, // number of meetings
				5, // number of participants
				5, // min number of attendances
				5, // max number of attendances
				true, // number of attendances is fixed for all participants
				8, // max range for meeting timetable
				true, // same timetable for all meetings
				50, // number attempts to generate a meeting tree
				100, // number of random traversals of each tree
				false, // run tests on the same tree
				true, // record results in one file
				AVERAGE); // type of aggregation of data 
	}
	*/
}