/*
 * GeneratorSensorCSP.java
 *
 * Created on October 15, 2002, 11:49 AM
 */
package frodo.sensors;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

import frodo.OCOP.OCOPBasicProblem;


/**
 * 
 * @author apetcu
 */
//------------------------------------------------------------------------------------------------------------//
public class GeneratorSensorCSP {
	int RANGE = 40;

	//------------------------------------------------------------------------------------------------------------//
	/** Creates a new instance of GeneratorSensorCSP */
	public GeneratorSensorCSP() {
	}
	//------------------------------------------------------------------------------------------------------------//
	public void generateRandom(String sfname, String tfname, int nodes, int fieldSize, int conn, int targets) {
		int connect[][] = new int[nodes][nodes];
		try {
			Random rnd_pos = new Random();
			Random rnd_connectivity = new Random();
			Random rnd_connections = new Random();
			System.out.println("Starting generation...");
			for (int i = 0; i < nodes; i++) {
				System.out.println("i=" + i);
				int possible_connections = rnd_connectivity.nextInt(conn) + 1;
				int actual_connections = 0;
				for (int j = 0; j < nodes; j++) {
					if (connect[i][j] != 0) {
						actual_connections++;
					}
				}
				int todo = possible_connections - actual_connections;
				System.out.println("\tpossible_connections=" + possible_connections);
				System.out.println("\tactual_connections=" + actual_connections);
				System.out.println("\ttodo=" + todo);
				if (todo > 0) {
					for (int j = 0; j < todo; j++) {
						boolean made_new_link = false;
						while (!made_new_link) {
							int new_link = rnd_connections.nextInt(nodes - i) + i;
							System.out.println("\t\tmaking link with " + new_link);
							if ((i != new_link) && (connect[i][new_link] == 0)) {
								made_new_link = true;
								connect[i][new_link] = 1;
								connect[new_link][i] = 1;
							}
						}
						if (connect[i][j] != 0) {
							actual_connections++;
						}
					}
				}
			}
			System.out.println("Writing sensor file...");
			FileWriter fw = new FileWriter(sfname);
			fw.write(new String(nodes + "\n"));
			for (int i = 0; i < nodes; i++) {
				String line = "S" + i + " (" + rnd_pos.nextInt(fieldSize) + "," + rnd_pos.nextInt(fieldSize) + "," + RANGE + ") ";
				for (int j = 0; j < nodes; j++) {
					if (connect[i][j] != 0)
						line = line + "S" + j + " ";
				}
				line = line + "\n";
				fw.write(line);
			}
			fw.close();
			System.out.println("Writing targets file...");
			fw = new FileWriter(tfname);
			for (int i = 0; i < targets; i++) {
				String line = "T" + i + " " + rnd_pos.nextInt(fieldSize) + " " + rnd_pos.nextInt(fieldSize) + "\n";
				fw.write(line);
			}
			fw.close();
		} catch (Exception e) {
			System.out.println("Exception while generating problem:");
			e.printStackTrace();
		}
	}

	//------------------------------------------------------------------------------------------------------------//
	public String getNeighbours(int i, int j, int max) {
		StringBuffer rez = new StringBuffer();
		int limit = max - 1;
		if (i > 0 && j > 0)
			rez.append(" S" + ((i - 1) * max + (j - 1)));
		if (i > 0)
			rez.append(" S" + ((i - 1) * max + j));
		if (i > 0 && j < limit)
			rez.append(" S" + ((i - 1) * max + (j + 1)));
		if (j > 0)
			rez.append(" S" + (i * max + (j - 1)));
		if (j < limit)
			rez.append(" S" + (i * max + (j + 1)));
		if (i < limit && j > 0)
			rez.append(" S" + ((i + 1) * max + (j - 1)));
		if (i < limit)
			rez.append(" S" + ((i + 1) * max + j));
		if (i < limit && j < limit)
			rez.append(" S" + ((i + 1) * max + (j + 1)));
		return rez.toString();
	}

	//------------------------------------------------------------------------------------------------------------//
	public void generateRegulate(String sfname, String tfname, int nodes, int fieldSize, int conn, int targets) {
		try {
			Random rnd_pos = new Random();
			System.out.println("Writing sensor file...");
			FileWriter fw = new FileWriter(sfname);
			fw.write(new String(nodes * nodes + "\n"));
			for (int i = 0; i < nodes; i++) {
				for (int j = 0; j < nodes; j++) {
					int sensno = i * nodes + j;
					String line = "S" + sensno + " (" + (j * 20 + 10) + "," + (i * 20 + 10) + "," + RANGE + ")";
					line += getNeighbours(i, j, nodes);
					line = line + "\n";
					fw.write(line);
				}
			}
			fw.close();
			System.out.println("Writing targets file...");
			fw = new FileWriter(tfname);
			for (int i = 0; i < targets; i++) {
				String line = "T" + i + " " + rnd_pos.nextInt(fieldSize) + " " + rnd_pos.nextInt(fieldSize) + "\n";
				fw.write(line);
			}
			fw.close();
		} catch (Exception e) {
			System.out.println("Exception while generating problem:");
			e.printStackTrace();
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public void printMatrix(boolean[][] mat) {
		for (int k = 0; k < mat.length; k++) {
			for (int l = 0; l < mat[k].length; l++) {
				if (mat[k][l] == false)
					System.out.print("0");
				else
					System.out.print("1");
			}
			System.out.print("\n");
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public void generateSensorGrid(String sfname, int nodes, int fieldSize) {
		//------------------Writing sensor file------------------------//
		System.out.println("Writing sensor file...");
		try {
			FileWriter fw;
			fw = new FileWriter(sfname);
			fw.write(new String(nodes * nodes + "\n"));
			for (int i = 0; i < nodes; i++) {
				for (int j = 0; j < nodes; j++) {
					int sensno = i * nodes + j;
					String line = "S" + sensno + " (" + (j * 20 + 10) + "," + (i * 20 + 10) + "," + RANGE + ")";
					line += getNeighbours(i, j, nodes);
					line = line + "\n";
					fw.write(line);
				}
			}
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public void generateSolvable(String tfname, int nodes, int fieldSize, int targets) {
		try {
			//------------------Writing targets file-----------------------//
			Random rnd_pos = new Random();
			System.out.println("Writing targets file...");
			FileWriter fw = new FileWriter(tfname);
			boolean[][] occupied = new boolean[nodes][nodes];
			for (int i = 0; i < targets; i++) {
				boolean found = false;
				int x = 0, y = 0, killer = 0;
				while (!found && killer < 100000) {
					int howmany = 0;
					x = rnd_pos.nextInt(fieldSize);
					y = rnd_pos.nextInt(fieldSize);
					int[][] rollback = new int[3][2];
					for (int k = 0; k < nodes; k++) {
						if (found == true)
							break;
						for (int l = 0; l < nodes; l++) {
							if (found == true)
								break;
							if ((occupied[k][l] == false) && (isInRange(x, y, (k * 20 + 10), (l * 20 + 10)))) {
								occupied[k][l] = true;
								rollback[howmany][0] = k;
								rollback[howmany][1] = l;
								howmany++;
								if (howmany == 3)
									found = true;
							}
						}
					}
					if (!found) {
						//						System.out.println(killer + "Teapa.. rolling back T(" + x + "," + y + ")");
						for (int m = 0; m < howmany; m++) {
							occupied[rollback[m][0]][rollback[m][1]] = false;
						}
					} else {
						System.out.print("\nOk, T" + i + "(" + x + "," + y + ") ->");
						for (int m = 0; m < howmany; m++) {
							System.out.print(" (" + rollback[m][0] + "," + rollback[m][1] + ")");
						}
					}
					killer++;
				}
				if (!found) {
					System.out.println("could not find spot for target " + i);
					printMatrix(occupied);
					//					System.exit(1);
					break;
				}
				String line = "T" + i + " " + x + " " + y + "\n";
				fw.write(line);
			}
			fw.close();
		} catch (Exception e) {
			System.out.println("Exception while generating problem:");
			e.printStackTrace();
		}
	}

	//-----------------------------------------------------------------------------------------------------------------//
	private boolean isInRange(int x1, int y1, int x2, int y2) {
		double dist = Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		if (dist < RANGE)
			return true;
		else
			return false;
	}

	//-----------------------------------------------------------------------------------------------------------------//
	public static void main(String[] args) {
		//		System.out.println("targets:" + args[0] + " sensors:" + args[1]);
		if (args.length < 5) {
			System.out.println("Usage: GeneratorSensorCSP target_file sensor_file number_of_nodes/edge field_size targets [repeater]");
		} else {
			GeneratorSensorCSP g = new GeneratorSensorCSP();
			String target_file = args[0];
			String sensor_file = args[1];
			int nodes = Integer.parseInt(args[2]);
			int fieldsize = Integer.parseInt(args[3]);
			int targets = Integer.parseInt(args[4]);

			g.generateSensorGrid(sensor_file, nodes, fieldsize);

			frodo.OCOP.DPOP.sensors.SensorEnvironment env = new frodo.OCOP.DPOP.sensors.SensorEnvironment(target_file, sensor_file, 0, 1);
			if (args.length == 5) {
				boolean done = false;
				while (!done) {
					g.generateSolvable(target_file, nodes, fieldsize, targets);
					OCOPBasicProblem problem = env.loadProblem();
					if (problem.isConnected())
						done = true;
					else
						System.out.println("The problem is disconnected");
					//					System.exit(0);
				}

			} else if (args.length == 6) {
				int repeater = Integer.parseInt(args[5]);
				for (int i = 0; i < repeater; i++) {
					boolean done = false;
					while (!done) {
						g.generateSolvable(target_file + "-" + String.valueOf(i) + ".txt", nodes, fieldsize, targets);
						OCOPBasicProblem problem = env.loadProblem();
						if (problem.isConnected())
							done = true;
						else
							System.out.println("The problem is disconnected");
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------------//
}