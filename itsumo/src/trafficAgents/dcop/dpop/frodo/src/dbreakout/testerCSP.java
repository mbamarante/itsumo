package frodo.dbreakout;

import frodo.sensors.TargetLoader;

public class testerCSP {
	public testerCSP() {
	}
	public static void main(String[] args) {
		if (args.length < 5) {
			System.out.println("Usage: testerCSP targets_file sensors_file 0/1/2/3/4 magnification on_screen");
			System.out.println("\t0 = random initialization");
			System.out.println("\t1 = NI initialization");
			System.out.println("\t2 = WNI initialization");
			System.out.println("\t3 = NPI initialization");
			System.out.println("\t4 = WNI + NPI initialization");
			System.out.println("\tmagnification = an integer saying how many times the display should be magnified (0,1,2...). Default is 1");
			System.out.println("\ton_screen = 0(no visual) / 1 (visual)");
			System.exit(0);
		}
		String target_file = args[0];
		String sensor_file = args[1];
		int mode = Integer.parseInt(args[2]);
		int mag = Integer.parseInt(args[3]);
		int onscreen = Integer.parseInt(args[4]);
		TargetLoader tl = new TargetLoader(null);
		int howMany = tl.targetCount(target_file);
		System.out.println("Found " + howMany + " targets in " + target_file);
		if (howMany > 0) {
			System.out.println("Sensor CSP starting with " + howMany + " agents, mode=" + mode);
			frodo.sensors.Environment env = new frodo.sensors.Environment(target_file, sensor_file, mode, mag);
			for (int i = 0; i < howMany; i++) {
				CSPAgent ag = new CSPAgent(env, i);
				env.addAgent(ag);
			}
			CSPAgent ag = null;
			if (onscreen == 1) {
				ag = new CSPAgent(env,-1);
			}
			env.setMonitorAgent(ag);
			//			env.setMonitorAgent(null);
			env.start();
		}
	}
}
