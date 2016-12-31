package main;

import java.io.File;
import java.io.IOException;

import javax.swing.DefaultListModel;

import data.Constant;
import data.Inst;
import data.Op;
import data.OpTable;
import data.Program;

public class SicSimulator implements ISicSimulator{
	public static OpTable opTable;
	private String log;
	
	public SicSimulator(){
		opTable = new OpTable();
		
	}
	
	@Override
	public void addLog(){
		Main.gui.txtLog.append(log + "\n");
		log = null;
	}
	
	@Override
	public void allStep(){
		while(Main.rm.getRegister(Constant.PC) >= -1)
			oneStep();
	}
	
	@Override
	public void initialize(File instFile){
		Main.loader.load(instFile);
		
		// Visual Simulator 초기화
		// H
		Program prog = Main.loader.pl.progList.get(0);
		Main.gui.txtProgName.setText(prog.name);
		Main.gui.txtStartAddr.setText("000000");
		Main.gui.txtLength.setText(String.format("%06X", prog.length));
		
		// E
		Main.rm.setRegister(Constant.PC, Main.loader.addr1stInst);
		Main.gui.txt1stInst.setText(String.format("%06X", Main.loader.addr1stInst));
		
		// Instructions
		DefaultListModel model = (DefaultListModel) Main.gui.listInst.getModel();
		for(Inst inst : Main.rm.il.instList){
			byte[] tmp = Main.rm.getMemory(inst.address, inst.size);
			int value;
			
			String element = null;
			switch (inst.size){
				case 1:
					value = Main.loader.getDecInteger(tmp, 2);
					element = String.format("%02X", value);
					break;
				case 2:
					value = Main.loader.getDecInteger(tmp, 4);
					element = String.format("%04X", value);
					break;
				case 3:
					value = Main.loader.getDecInteger(tmp, 6);
					element = String.format("%06X", value);
					break;
				case 4:
					value = Main.loader.getDecInteger(tmp, 8);
					element = String.format("%08X", value);
					break;
			
			}
			model.addElement(element);
		}
	}
	
	@Override
	public void oneStep(){
		int pc = Main.rm.getRegister(Constant.PC);
		
		if(pc < 0)
			return;
		if(pc == 0)
			Main.rm.setRegister(Constant.L, -1);
		
		Inst inst = Main.rm.il.getInst(pc);
		if(inst == null)
			return;
		
		int index = Main.rm.il.instList.indexOf(inst);
		Main.gui.listInst.setSelectedIndex(index);
		Main.gui.listInst.ensureIndexIsVisible(index);
		
		Main.rm.setRegister(Constant.PC, pc + inst.size);
		
		if(inst != null){
			int TA = 0;
			if(inst.size > 2){
				TA = Main.rm.il.getTargetAddress(inst);
				Main.rm.TA = TA;
			}
			Op opcode = opTable.search(inst.op);
			
			if(opcode != null){
				String name = opcode.name;
				if(name.equals("CLEAR")){
					Main.rm.setRegister(inst.r1, 0);
					
					log = "해당 레지스터를 0으로 만듦";
					
				}else if(name.equals("COMP")){
					int a = Main.rm.getRegister(Constant.A);
					
					if(inst.n == false && inst.i)
						Main.rm.setRegister(Constant.SW, Integer.compare(a, TA));
					else{
						byte[] m = Main.rm.getMemory(TA, 3);
						
						Main.rm.setRegister(Constant.SW, compare(a, m));
					}
					
					log = "A 레지스터와 해당 메모리를 비교";
					
				}else if(name.equals("COMPR")){
					int r1 = Main.rm.getRegister(inst.r1);
					int r2 = Main.rm.getRegister(inst.r2);
					
					Main.rm.setRegister(Constant.SW, Integer.compare(r1, r2));
					
					log = "레지스터1과 레지스터2 비교";
					
				}else if(name.equals("J")){
					Main.rm.setRegister(Constant.PC, TA);
					
					log = "PC값을 해당 메모리로 설정";
					
				}else if(name.equals("JEQ")){
					if(Main.rm.getRegister(Constant.SW) == 0)
						Main.rm.setRegister(Constant.PC, TA);
					
					log = "SW 값이 0이면 PC값을 해당 메모리로 설정";
					
				}else if(name.equals("JLT")){
					if(Main.rm.getRegister(Constant.SW) < 0)
						Main.rm.setRegister(Constant.PC, TA);
					
					log = "SW 값이 0보다 작으면 PC값을 해당 메모리로 설정";
					
				}else if(name.equals("JSUB")){
					pc = Main.rm.getRegister(Constant.PC);
					Main.rm.setRegister(Constant.L, pc);
					Main.rm.setRegister(Constant.PC, TA);
					
					log = "서브루틴 호출";
					
				}else if(name.equals("LDA")){
					if(inst.n == false && inst.i){
						Main.rm.setRegister(Constant.A, TA);
						
					}else{
						byte[] m = Main.rm.getMemory(TA, 3);
						Main.rm.setRegister(Constant.A, Main.loader.getDecInteger(m, 6));
						
					}
					
					log = "A 레지스터를 해당 메모리로 설정";
					
				}else if(name.equals("LDCH")){
					if(inst.n == false && inst.i){
						Main.rm.setRegister(Constant.A, TA & 0x000000FF);
					}else{
						byte[] m = Main.rm.getMemory(TA, 1);
						Main.rm.setRegister(Constant.A, m[0]);
					}
					
					log = "A 레지스터 1바이트를 해당 메모리로 설정";
					
				}else if(name.equals("LDT")){
					if(inst.n == false && inst.i){
						Main.rm.setRegister(Constant.T, TA);
						
					}else{
						byte[] m = Main.rm.getMemory(TA, 3);
						Main.rm.setRegister(Constant.T, Main.loader.getDecInteger(m, 6));
					}
					
					log = "T 레지스터를 해당 메모리로 설정";
					
				}else if(name.equals("RD")){
					Main.rm.setRegister(Constant.A, Main.rm.readDevice());
					
					log = "디바이스에서 A 레지스터로 1 바이트를 읽음";
					
				}else if(name.equals("RSUB")){
					int l = Main.rm.getRegister(Constant.L);
					Main.rm.setRegister(Constant.PC, l);
					
					log = "PC값을 L 레지스터 값으로 설정";
					
					// 다음 파일 작업을 위해 fos, dev 초기화
					try{
						if(Main.rm.fos != null){
							Main.rm.fos.flush();
							Main.rm.fos.close();
							Main.rm.fos = null;
						}
					}catch(IOException e){
						e.printStackTrace();
					}
					
					Main.rm.dev = null;
					
				}else if(name.equals("STA")){
					int a = Main.rm.getRegister(Constant.A);
					
					byte[] A = new byte[3];
					A[0] = (byte) ((a & 0x00FF0000) >> 16);
					A[1] = (byte) ((a & 0x0000FF00) >> 8);
					A[2] = (byte) (a & 0x000000FF);
					
					Main.rm.setMemory(TA, A, 3);
					
					log = "해당 메모리에 A 레지스터 값을 가져옴";
					
				}else if(name.equals("STCH")){
					int a = Main.rm.getRegister(Constant.A);
					byte[] A = new byte[1];
					A[0] = (byte) (a & 0x000000FF);
					
					Main.rm.setMemory(TA, A, 1);
					
					log = "해당 메모리에 A 레지스터 1 바이트를 가져옴";
					
				}else if(name.equals("STL")){
					int l = Main.rm.getRegister(Constant.L);
					
					byte[] L = new byte[3];
					L[0] = (byte) ((l & 0x00FF0000) >> 16);
					L[1] = (byte) ((l & 0x0000FF00) >> 8);
					L[2] = (byte) (l & 0x000000FF);
					
					Main.rm.setMemory(TA, L, 3);
					
					log = "해당 메모리에 L 레지스터 값을 가져옴";
					
				}else if(name.equals("STX")){
					int x = Main.rm.getRegister(Constant.X);
					
					byte[] X = new byte[3];
					X[0] = (byte) ((x & 0x00FF0000) >> 16);
					X[1] = (byte) ((x & 0x0000FF00) >> 8);
					X[2] = (byte) (x & 0x000000FF);
					
					Main.rm.setMemory(TA, X, 3);
					
					log = "해당 메모리에 X 레지스터 값을 가져옴";
					
				}else if(name.equals("TD")){
					if(Main.rm.fis == null && Main.rm.fos == null){
						String dev = "";
						
						if(inst.n == false && inst.i){
							dev = Integer.toHexString(TA);
						}else{
							byte[] m = Main.rm.getMemory(TA, 1);
							dev = String.format("%02X", Main.loader.getDecInteger(m, 2));
						}
						dev += ".txt";
						
						Main.rm.initialDevice(dev);
						Main.rm.setRegister(Constant.SW, 1);
					}
					
					log = "디바이스 테스트";
					
				}else if(name.equals("TIXR")){
					int x = Main.rm.getRegister(Constant.X);
					Main.rm.setRegister(Constant.X, x + 1);
					
					int r1 = Main.rm.getRegister(inst.r1);
					Main.rm.setRegister(Constant.SW, Integer.compare(x + 1, r1));
					
					log = "X 값을 1 증가시키고, X와 레지스터1를 비교";
					
				}else if(name.equals("WD")){
					int a = Main.rm.getRegister(Constant.A);
					byte[] A = new byte[1];
					A[0] = (byte) (a & 0x000000FF);
					
					Main.rm.writeDevice(A);
					
					log = "디바이스에 A 레지스터 1 바이트를 씀";
				}
			}
			
			addLog();
		}
		
	}
	
	private int compare(int integer, byte[] array){
		int value2 = Main.loader.getDecInteger(array, 6);
		
		return Integer.compare(integer, value2);
	}
}
