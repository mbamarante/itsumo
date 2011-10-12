package frodo.dbreakout;

import frodo.sensors.Environment;

public class sensorCSP {
    public sensorCSP(){
    }
    public static void main(String []args){
        if(args.length < 2){
            System.out.println("Usage: sensorCSP number_of_agents 0/1 mag");
            System.out.println("\t0 = random initialization");
            System.out.println("\t1 = NI initialization");
            System.out.println("\tmag = an integer saying how many times the display should be magnified (0,1,2...). Default is 1");
            System.exit(0);
        }
        int mode = Integer.parseInt(args[1]);
        Environment env=null;
        if(args.length==3){
            int mag = Integer.parseInt(args[2]);
            env = new Environment("targets.txt", "sensors.txt", mode, mag);
        }
        else{
            env = new Environment("targets.txt", "sensors.txt", mode,1);
        }
        int howMany=Integer.parseInt(args[0]);
        System.out.println("Sensor CSP starting with " + howMany + " agents. mode=" + mode);
        for(int i=0;i<howMany;i++){
            CSPAgent ag = new CSPAgent(env, i);
            env.addAgent(ag);
        }
        CSPAgent ag = new CSPAgent(env, -1);
        env.setMonitorAgent(ag);
        env.start();
    }
}