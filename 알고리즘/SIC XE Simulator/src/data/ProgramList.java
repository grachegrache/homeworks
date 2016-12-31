package data;

import java.util.ArrayList;

import main.SicLoader;

public class ProgramList{
	public ArrayList<Program> progList;
	public ArrayList<Symbol> symList;
	
	public ProgramList(){
		progList = new ArrayList<Program>();
		symList = new ArrayList<Symbol>();
	}
	
	public byte[] searchSymbol(String ref){
		for(Symbol symbol : symList)
			if(symbol.name.equals(ref))
				return symbol.address;
		
		return null;
	}
	
	public Program search(int address){
		for(Program prog : progList)
			if(address >= prog.address && address <= prog.address+prog.length)
				return prog;
		
		return null;
	}
	
	public void setSymbolList(){
		Symbol symbol;
		
		for(Program prog : progList){
			//프로그램 추가
			symbol = new Symbol();
			symbol.name = prog.name;
			symbol.address = SicLoader.getHexByte(String.format("%06X", prog.address));
			
			symList.add(symbol);
			
			//Extdef 추가
			for(Extdef def : prog.defList){
				symbol = new Symbol();
				symbol.name = def.name;
				symbol.address = def.address;
				
				symList.add(symbol);
			}
		}
	}
	
	public Program getLast(){
		if(progList.isEmpty())
			return null;
		else
			return progList.get(progList.size()-1);
	}
}
