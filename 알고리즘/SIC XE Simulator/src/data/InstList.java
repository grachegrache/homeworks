package data;

import java.util.ArrayList;

import main.Main;
import main.SicSimulator;

public class InstList{
	public ArrayList<Inst> instList;
	public byte[] memory;
	
	public InstList(int size){
		instList = new ArrayList<Inst>();
		memory = new byte[size];
	}
	
	/**
	 * ��ġ�� �Ű������� Inst ��ü�� ��� �޼ҵ�
	 * 
	 * @param locate
	 * @return
	 */
	public Inst getInst(int locate){
		for(Inst inst : instList)
			if(inst.address == locate)
				return inst;
		
		return null;
	}
	
	/**
	 * ���� instList�� �����ϴ� ��� Inst ��ü�� �Ľ��ϴ� �޼ҵ�
	 * 
	 */
	public void parseInstruction(){
		for(Inst inst : instList)
			parseInstruction(inst);
	}
	
	/**
	 * Instruction ��� ���� address, size�� ���� memory�� ������� Instruction �Ľ��ϴ� �޼ҵ�
	 * 
	 * @param inst
	 */
	private void parseInstruction(Inst inst){
		// �ν�Ʈ���� ��������
		byte[] instruction = Main.rm.getMemory(inst.address, inst.size);
		
		if(inst.size == 2){
			// 2���� parsing
			inst.op = instruction[0];
			inst.r1 = (instruction[1] & 0xF0) >> 4;
			inst.r2 = instruction[1] & 0x0F;
			
		}else if(inst.size > 2){
			// 3,4���� parsing
			if(inst.size == 3)
				inst.disp = new byte[2];
			else
				inst.disp = new byte[3];
			
			inst.op = (byte) (instruction[0] & 0xFC);
			inst.n = (instruction[0] & 0x02) == 0x02;
			inst.i = (instruction[0] & 0x01) == 0x01;
			inst.x = (instruction[1] & 0x80) == 0x80;
			inst.b = (instruction[1] & 0x40) == 0x40;
			inst.p = (instruction[1] & 0x20) == 0x20;
			inst.e = (instruction[1] & 0x10) == 0x10;
			inst.disp[0] = (byte) (instruction[1] & 0x0F);
			for(int i = 2; i < inst.size; i++)
				inst.disp[i - 1] = instruction[i];
		}
		return;
	}
	
	/**
	 * object code�� T ���ڵ� �� ���� instList�� �Ľ��ϴ� �޼ҵ�
	 * 
	 * @param line
	 */
	public void parseLine(String line){
		char[] array = line.toCharArray();
		byte opcode;
		boolean isExtended;
		
		for(int i = 0; i < array.length; i++){
			opcode = (byte) (Integer.parseInt(line.substring(i, i + 2), 16) & 0xFC);
			Op op = SicSimulator.opTable.search(opcode);
			Program prog = Main.loader.pl.search(i / 2);
			
			if(op == null)
				continue;
			
			Inst inst = new Inst();
			
			// 4���� Ȯ��
			isExtended = true;
			for(int j = 0; j < 5; j++)
				if(array.length > i + j + 3)
					isExtended &= array[i + j + 3] == '0';
				else
					isExtended = false;
			
			inst.address = Main.loader.startAddress + i / 2;
			
			if(op.format.equals("1")){
				// 1����
				inst.size = 1;
				i += 1;
				
			}else if(op.format.equals("2")){
				// 2����
				inst.size = 2;
				i += 3;
				
			}else if(op.format.contains("/"))
				if(isExtended){
					// 4����
					inst.size = 4;
					i += 7;
					
				}else{
					// 3����
					inst.size = 3;
					i += 5;
					
				}
			
			if(inst.size > 2){
				Main.rm.setRegister(Constant.PC, inst.address + inst.size);
				
				int TA = (int) getTargetAddress(inst);
				if(inst.i == false){
					Program afterProg = Main.loader.pl.search(TA);
					
					if(afterProg == null || prog != afterProg)
						continue;
				}
			}
			
			instList.add(inst);
		}
		
		Main.rm.setRegister(Constant.PC, 0);
	}
	
	/**
	 * Inst�� TA�� ���ϴ� �޼ҵ� immediate�� ��� ���� ���� ���ϵȴ�.
	 * 
	 * @param inst
	 * @return
	 */
	public int getTargetAddress(Inst inst){
		int pc, base, x, disp;
		
		parseInstruction(inst);
		if(inst.size == 3)
			disp = Main.loader.getDecInteger(inst.disp, 3);
		else
			disp = Main.loader.getDecInteger(inst.disp, 5);
		
		if(inst.b){
			// BASE relative
			base = Main.rm.getRegister(Constant.B);
			disp += base;
			
		}else if(inst.p){
			// PC relative
			pc = Main.rm.getRegister(Constant.PC);
			disp += pc;
		}
		
		if(inst.x){
			x = Main.rm.getRegister(Constant.X);
			disp += x;
		}
		
		if(inst.n && inst.i){
			// simple
			return disp;
			
		}else if(inst.n){
			// indirect
			if(disp < 0 || disp >= Constant.memorySize)
				return -1;
			
			byte[] address = new byte[3];
			for(int i = 0; i < 3; i++)
				address[i] = memory[(int) (disp + i)];
			
			if(inst.size == 3)
				return Main.loader.getDecInteger(address, 3);
			else
				return Main.loader.getDecInteger(address, 5);
			
		}else if(inst.i){
			// immediate
			return disp;
			
		}else{
			byte[] TA = new byte[2];
			TA[0] = (byte) (memory[inst.address + 1] & 0x7F);
			TA[1] = memory[inst.address + 2];
			
			if(inst.size == 3)
				return Main.loader.getDecInteger(TA, 3);
			else
				return Main.loader.getDecInteger(TA, 5);
		}
		
	}
}
