package main;

import java.io.File;
import java.util.Scanner;

import data.Constant;
import data.Extdef;
import data.Extref;
import data.Modify;
import data.Program;
import data.ProgramList;

public class SicLoader implements ISicLoader{
	public int length;
	public ProgramList pl;
	public int startAddress;
	public int addr1stInst;
	
	public SicLoader(){
		pl = new ProgramList();
	}
	
	public static int getDecInteger(byte[] array, int count){
		String hex = "";
		
		if(count % 2 == 1)
			hex = String.format("%X", array[0]);
		else
			hex = String.format("%02X", array[0]);
		
		for(int i = 1; i < array.length; i++)
			hex += String.format("%02X", array[i]);
		
		if(count > 2)
			if(hex.charAt(0) == 'F'){
				//음수 변환
				array[0] = (byte)(0x0F - array[0]);
				for(int i=1;i<array.length-1;i++)
					array[i] = (byte)(0xFF - array[i]);
				array[array.length-1] = (byte)(0xFF + 1 - array[array.length-1]);
				return -getDecInteger(array, 3);
			}
		
		return Integer.parseUnsignedInt(hex, 16);
	}
	
	public static byte[] getHexByte(String line){
		byte[] array = new byte[line.length() / 2];
		
		for(int i = 0; i < array.length; i++)
			array[i] = (byte) Integer.parseInt(line.substring(2 * i, 2 * i + 2), 16);
		
		return array;
	}
	
	@Override
	public void load(File objFile){
		byte[] tmp = new byte[Constant.memorySize];
		for(int i = 0; i < Constant.memorySize; i++)
			tmp[i] = 0;
		
		Main.rm.setMemory(0, tmp, Constant.memorySize);
		
		Scanner sc = null;
		
		try{
			sc = new Scanner(objFile);
			
			while(sc.hasNextLine())
				readLine(sc.nextLine());
		}catch(Exception e){
			e.printStackTrace();
			
		}finally{
			if(sc != null)
				sc.close();
		}
		
		pl.setSymbolList();
		
		for(Program prog : pl.progList)
			for(Modify modify : prog.modifyList){
				byte[] data = pl.searchSymbol(modify.ref);
				int size = modify.len / 2 + modify.len % 2;
				
				switch (modify.op){
					case '+':
						byte backup = data[0];
						if(modify.len % 2 == 1){
							byte left = (byte) (Main.rm.il.memory[modify.addr] & 0xF0);
							data[0] &= 0x0F;
							data[0] += left;
						}
						
						Main.rm.setMemory(modify.addr, data, size);
						data[0] = backup;
						break;
					
					case '-':
						byte[] org = Main.rm.getMemory(modify.addr, size);
						for(int i = 0; i < size; i++)
							org[i] -= data[i];
						
						Main.rm.setMemory(modify.addr, org, size);
						break;
					
					default:
						break;
				}
				
			}
		
		Main.rm.il.parseInstruction();
	}
	
	@Override
	public void readLine(String line){
		if(line == null || line.length() == 0)
			return;
		
		Program prog;
		switch (line.charAt(0)){
			case 'H':
				int address = 0;
				for(Program pro : pl.progList)
					address += pro.length;
				prog = new Program();
				prog.name = line.substring(1, 7).trim();
				prog.address = address + Integer.parseInt(line.substring(7, 13));
				prog.length = Integer.parseInt(line.substring(13, 19), 16);
				
				pl.progList.add(prog);
				break;
			case 'D':
				prog = pl.getLast();
				Extdef def;
				
				for(int i = 0; i < line.length() / 6; i++){
					def = new Extdef();
					def.name = line.substring(6 * i + 1, 6 * i + 7).trim();
					i++;
					def.address = getHexByte(line.substring(6 * i + 1, 6 * i + 7));
					prog.defList.add(def);
				}
				break;
			case 'R':
				prog = pl.getLast();
				Extref ref;
				
				for(int i = 0; i < line.length() / 6; i++){
					ref = new Extref();
					ref.name = line.substring(6 * i + 1, 6 * i + 7).trim();
					prog.refList.add(ref);
				}
				break;
			case 'T':
				prog = pl.getLast();
				int start = Integer.parseInt(line.substring(1, 7), 16) + prog.address;
				startAddress = start;
				length = Integer.parseInt(line.substring(7, 9), 16);
				
				Main.rm.setMemory(startAddress, getHexByte(line.substring(9)), length);
				Main.rm.il.parseLine(line.substring(9));
				
				break;
			case 'M':
				prog = pl.getLast();
				Modify modify = new Modify();
				modify.addr = Integer.parseInt(line.substring(1, 7), 16) + prog.address;
				modify.len = Integer.parseInt(line.substring(7, 9), 16);
				modify.op = line.charAt(9);
				modify.ref = line.substring(10);
				
				prog.modifyList.add(modify);
				
				break;
			case 'E':
				if(line.equals("E") == false)
					addr1stInst = Integer.parseInt(line.substring(1), 16);
				break;
			default:
				break;
		}
	}
}
