package main;


public class Main{
	public static SicSimulator simulator;
	public static SicLoader loader;
	public static ResourceManager rm;
	public static VisualSimulator gui;
	
	public static void main(String[] args){
		rm = new ResourceManager();
		rm.initializeRegister();
		rm.initializeMemory();
		loader = new SicLoader();
		simulator = new SicSimulator();
		gui = new VisualSimulator();
	}
}
