package data;

import java.util.ArrayList;

public class Program{
	public ArrayList<Extdef>	defList;
	public int					length;
	public String				name;
	public int address;
	public ArrayList<Extref>	refList;
	public ArrayList<Modify> modifyList;
	
	public Program(){
		defList = new ArrayList<Extdef>();
		refList = new ArrayList<Extref>();
		modifyList = new ArrayList<Modify>();
		
	}
	
}
