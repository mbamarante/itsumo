package frodo.OCOP.DPOP.sensors;

import frodo.OCOP.OCOPBasicProblem;
import frodo.OCOP.graph.DFSMaster;
import frodo.OCOP.graph.DFSStatistics;
import frodo.sensors.TargetLoader;

public class testerCSP {
	public testerCSP() {
	}

	public static void main(String[] args) {
		if (args.length < 4) {
			System.out.println("Usage: testerCSP targets_file sensors_file magnification on_screen");
			System.out.println("\tmagnification = an integer saying how many times the display should be magnified (0,1,2...). Default is 1");
			System.out.println("\ton_screen = 0(no visual) / 1 (visual)");
			System.exit(0);
		}
		String target_file = args[0];
		String sensor_file = args[1];
		int mag = Integer.parseInt(args[2]);
//		int onscreen = Integer.parseInt(args[3]);
		TargetLoader tl = new TargetLoader(null);
		int howMany = tl.targetCount(target_file);
		System.out.println("Found " + howMany + " targets in " + target_file);
		if (howMany > 0) {
			System.out.println("Sensor CSP starting with " + howMany + " agents, mode=" + 0);
			frodo.OCOP.DPOP.sensors.SensorEnvironment env = new frodo.OCOP.DPOP.sensors.SensorEnvironment(target_file, sensor_file, 0, mag);
			OCOPBasicProblem problem = env.loadProblem();
			//			System.out.println(problem.toString());
			//			System.out.println(problem.toStringNodes());
			if (!problem.isConnected()) {
				System.out.println("The problem is disconnected");
				System.exit(0);
			}
			//			int width = problem.getMinimumGraphWidth();
//			int minwidth = Integer.MAX_VALUE;
//			int minwidthRoot = -1;
//			int widths[] = new int[howMany];

			DFSMaster.getMinimalWidthDFS_MCN(problem);
			DFSStatistics statistix  = DFSMaster.getStatisticsOfDFS(problem);
			System.out.println("DFS stats:\n" + statistix);
			System.out.println("The problem has max domain size " + problem.getMaximalDomainSize());
			/*
			 * for (int i = 0; i < howMany; i++) { OCOPAgent ag = new OCOPAgent(null, i); env.addAgent(ag); }
			 *  
			 */
			//			env.setMonitorAgent(null);
			env.start();
		}
	}
}