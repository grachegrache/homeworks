package main;

import java.io.File;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.StringTokenizer;

import data.Constant;
import data.Hex;
import data.Inst;
import data.Modify;
import data.Program;
import data.Symbol;
import data.Token;

public class MyAssembler{
	private static ArrayList<ArrayList<String>>	extdef;
	private static ArrayList<ArrayList<String>>	extref;
	private static ArrayList<String>			inputData;
	public static String						inputFile;
	private static ArrayList<Inst>				inst;
	private static int							lastCtr;
	private static int							len;
	private static int							locctr;
	private static ArrayList<ArrayList<Modify>>	modifyTable;
	private static ArrayList<ArrayList<String>>	objectCode;
	public static String						outputFile;
	private static ArrayList<Program>			programInfo;
	private static ArrayList<Symbol>			symTable;
	private static ArrayList<Token>				tokenTable;
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. �Ű� : ���� ��ȯ : �������� = 0
	 * , ���� �߻� = -1 ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� ���ؼ� ���� ������ �����Ͽ�
	 * ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ��� �����Ͽ���.
	 * ----------------------------------------
	 * -------------------------------------------
	 */
	
	static{
		int maxLines = Constant.MAX_LINES;
		
		extdef = new ArrayList<ArrayList<String>>();
		extref = new ArrayList<ArrayList<String>>();
		programInfo = new ArrayList<Program>();
		inputData = new ArrayList<String>(maxLines);
		inst = new ArrayList<Inst>(Constant.MAX_INST);
		modifyTable = new ArrayList<ArrayList<Modify>>();
		objectCode = new ArrayList<ArrayList<String>>();
		symTable = new ArrayList<Symbol>(maxLines);
		tokenTable = new ArrayList<Token>(maxLines);
		
		inputFile = "input.txt";
		outputFile = "output.txt";
		lastCtr = 0;
		locctr = 0;
		len = 0;
	}
	
	/*
	 * ���� locctr���� symTable�� addr�� ���Ͽ� ���� ���� ���� ���ͷ��� objectCode�� �߰��ϴ� �Լ�
	 */
	private static int addLiteral(){
		int size;
		String code = "";
		
		for(Symbol symbol : symTable)
			if(locctr == symbol.addr && symbol.symbol.charAt(0) == '='){
				// '�� ' ������ ���ڿ� �Ľ�
				String operand = symbol.symbol.substring(symbol.symbol.indexOf("'") + 1);
				operand = operand.substring(0, operand.indexOf("'"));
				
				switch(symbol.symbol.charAt(1)){
					case 'X':
						code = operand;
						size = operand.length() / 2 + operand.length() % 2;
						break;
					case 'C':
						for(Character ch : operand.toCharArray())
							code += Hex.dectohex(ch.charValue());
						size = operand.length();
						break;
					default:
						code = Hex.dectohex(Integer.parseInt(operand));
						size = code.length() / 2 + code.length() % 2;
						break;
				}
				
				locctr += size;
				
				// �� �ٿ��� object code�� �Ľ̵��� �ʾ��� ��
				if(locctr - lastCtr != size)
					objectCode.get(objectCode.size() - 1).add("enter");
				
				objectCode.get(objectCode.size() - 1).add(code);
				return code.length();
			}
		
		return 0;
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�. �н�1������.. 1. ���α׷� �ҽ��� ��ĵ�Ͽ�
	 * �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū ���̺��� �����Ѵ�.
	 * 
	 * �Ű� : ���� ��ȯ : ���� ���� = 0 , ���� = < 0 ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ
	 * �����̴�. ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	public static int assemPass1(){
		// input_data�� ������� tokenTable�� �Ľ�
		for(String input : inputData)
			if(tokenParsing(input) < 0)
				return -1;
		
		printIntercode();
		return 0;
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�. �н� 2������ ���α׷���
	 * ����� �ٲٴ� �۾��� ���� ������ ����ȴ�. ������ ���� �۾��� ����Ǿ� ����. 1. ������ �ش� ����� ��ɾ �����
	 * �ٲٴ� �۾��� �����Ѵ�. �Ű� : ���� ��ȯ : �������� = 0, �����߻� = < 0 ���� :
	 * ----------------------
	 * -------------------------------------------------------------
	 */
	
	public static int assemPass2(){
		int pc = -1;
		locctr = 0;
		lastCtr = 0;
		
		for(Token token : tokenTable)
			pc = makeIntercode(token, pc);
		
		makeObjectcode(outputFile);
		return 0;
		
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)�� �����ϴ� �Լ��̴�.
	 * �Ű� : ���� ��� ���� ��ȯ : �������� = 0 , ���� < 0 ���� : ���� ������� ������ ������ ����.
	 * 
	 * ==========================================================================
	 * ===== | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
	 * ==============================
	 * =================================================
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	/*
	 * BYTE operand�� ũ�� ����
	 */
	private static int getByteSize(String operand){
		String parsing;
		
		switch(operand.charAt(0)){
			case 'X':
				parsing = operand.substring(operand.indexOf("'"));
				parsing = parsing.substring(0, operand.indexOf("'"));
				return parsing.length() / 2 + parsing.length() % 2;
			case 'C':
				parsing = operand.substring(operand.indexOf("'"));
				parsing = parsing.substring(0, operand.indexOf("'"));
				return parsing.length();
			default:
				parsing = Hex.dectohex(Integer.parseInt(operand));
				return parsing.length() / 2 + parsing.length() % 2;
		}
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : ����� �� �ҽ��ڵ带 �о���� �Լ��̴�. �Ű� : ������� �ҽ����ϸ� ��ȯ : �������� = 0 ,
	 * ���� < 0 ���� :
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	/*
	 * �Ľ̵� ��ū �ϳ�(instruction)�� size�� ����
	 */
	private static int getInstSize(Token token){
		int index;
		String operator = token.operator;
		
		if(operator == "")
			return -1;
		
		if(operator.charAt(0) == '+')
			index = searchOpcode(operator.substring(1));
		else
			index = searchOpcode(operator);
		
		if(index != -1){
			// operator�� opcode�̸�
			if(inst.get(index).format.contains("/")){
				// ���Ŀ� / ���ڰ� ���� => 3 �Ǵ� 4
				if(operator.charAt(0) == '+')
					return 4;
				else
					return 3;
			}else
				return Integer.valueOf(inst.get(index).format);
			
		}else if(operator.equals("START") || operator.equals("CSECT"))
			return 0;
		else if(operator.equals("END") || operator.equals("LTORG") || operator.equals("EXTDEF") || operator.equals("EXTREF"))
			return -1;
		else if(operator.equals("RESB"))
			return Integer.valueOf(token.operand.get(0));
		else if(operator.equals("RESW"))
			return Integer.valueOf(token.operand.get(0)) * 3;
		else if(operator.equals("BYTE"))
			return getByteSize(token.operand.get(0));
		else if(operator.equals("WORD"))
			return 3;
		else
			return -1;
		
	}
	
	private static int getRegNum(String operand){
		if(operand == "")
			return -1;
		
		switch(operand){
			case "A":
				return Constant.A;
			case "B":
				return Constant.regB;
			case "T":
				return Constant.T;
			case "S":
				return Constant.S;
			case "L":
				return Constant.L;
			case "X":
				return Constant.regX;
			default:
				return -1;
		}
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. �Ű� : ��ū ������ ���е� ���ڿ� ��ȯ : ��������
	 * = ���� ���̺� �ε���, ���� < 0 ���� :
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	private static int makeIntercode(Token token, int programCount){
		int opcodeIndex, symbolIndex, format, TA, regNum, num;
		char[] tmp;
		boolean isExist, immediate;
		String code, hex, operator, operand;
		isExist = false;
		immediate = false;
		code = "";
		hex = "";
		operator = "";
		operand = "";
		TA = -1;
		
		// inst ���� ã��
		format = getInstSize(token);
		if(format == 0){
			len = 0;
			lastCtr = 0;
			locctr = 0;
			programCount += 1;
			objectCode.add(new ArrayList<String>());
			modifyTable.add(new ArrayList<Modify>());
			
		}else if(format > 0)
			locctr += format;
		
		// opcode �˻�
		operator = token.operator;
		if(operator.charAt(0) == '+')
			operator = operator.substring(1);
		
		// opcode �̸� opcode ����
		if((opcodeIndex = searchOpcode(operator)) >= 0){
			if(format > 1)
				code = inst.get(opcodeIndex).opcode;
			
			if(format > 2){
				// #, @(immediate, indirect) �˻�
				code += "0";
				num = Hex.getDec(code.charAt(1));
				
				if(token.operand.size() > 0)
					operand = token.operand.get(0);
				else
					num += Constant.N + Constant.I;
				
				if(token.operand.size() > 0)
					switch(operand.charAt(0)){
						case '#':
							num += Constant.I;
							operand = operand.substring(1);
							immediate = true;
							break;
						case '@':
							num += Constant.N;
							operand = operand.substring(1);
							break;
						default:
							num += Constant.N + Constant.I;
							break;
					}
				
				tmp = code.toCharArray();
				tmp[1] = Hex.getHex(num);
				code = String.valueOf(tmp);
				
				// x, e ã��
				for(String op : token.operand)
					if(token.operand.indexOf(op) != 0 && op.equals("X"))
						isExist = true;
				
				if(isExist){
					num = Hex.getDec(code.charAt(2));
					num += Constant.opX;
					tmp = code.toCharArray();
					tmp[2] = Hex.getHex(num);
					code = String.valueOf(tmp);
				}
			}
			
			// 4�����̸�
			if(format == 4){
				num = Hex.getDec(code.charAt(2));
				num += Constant.E;
				tmp = code.toCharArray();
				tmp[2] = Hex.getHex(num);
				code = String.valueOf(tmp);
			}
			
			// TA ã��
			if(format > 2)
				if(operand == ""){
					TA = -1;
					hex = "000";
				}else if(Hex.isDigit(operand)){
					// �����̸�
					if(immediate)
						hex = Hex.getFormatedHex(Hex.dectohex(Integer.valueOf(operand)), 3);
					else
						TA = Integer.valueOf(operand);
				}else if((symbolIndex = searchExtref(operand, programCount)) != -1){
					// extref�̸�
					TA = -1;
					hex = "000";
				}else if((symbolIndex = searchSymbol(operand)) != -1)
					// symbol�̸�
					TA = symTable.get(symbolIndex).addr;
				else
					TA = -1;
			
			// �ּ� ���
			if(immediate == false)
				switch(format){
					case 2:
						int i = 2;
						for(String op : token.operand){
							if((regNum = getRegNum(op)) != -1)
								hex = Hex.dectohex(regNum);
							else
								hex = "0";
							i++;
							code += hex;
						}
						for(; i < 4; i++)
							code += "0";
						
						break;
					case 3:
						if(TA != -1){
							if(operator.equals("RSUB") == false){
								num = Hex.getDec(code.charAt(2));
								
								if(TA - locctr >= Math.pow(16, 3)){
									num += Constant.opB;
									tmp = code.toCharArray();
									tmp[2] = Hex.getHex(num);
									code = String.valueOf(tmp);
								}else{
									num += Constant.P;
									tmp = code.toCharArray();
									tmp[2] = Hex.getHex(num);
									code = String.valueOf(tmp);
									
									hex = Hex.getFormatedHex(Hex.dectohex(TA - locctr), 3);
									code += hex;
								}
							}
						}else if(hex.equals("000"))
							code += hex;
						break;
					case 4:
						hex = "00000";
						Modify modify = new Modify();
						modify.len = 5;
						modify.addr = len / 2 + 1;
						modify.op = '+';
						modify.ref = token.operand.get(0);
						modifyTable.get(programCount).add(modify);
						code += hex;
						break;
					default:
						break;
				}
			else if(hex != "")
				code += hex;
			
		}else if(token.operator.equals("WORD") || token.operator.equals("BYTE")){
			// opcode�� �ƴϰ� WORD Ȥ�� BYTE�� ��
			
			if(token.operator.equals("WORD")){
				
				// ���ڷ� �̷���� ���� ������
				if(!Hex.isDigit(token.operand.get(0))){
					char op = 0;
					int indexOp;
					String op1, op2;
					
					// ������ ã��
					if(token.operand.get(0).contains("+"))
						op = '+';
					else if(token.operand.get(0).contains("-"))
						op = '-';
					else if(token.operand.get(0).contains("*"))
						op = '*';
					else if(token.operand.get(0).contains("/"))
						op = '/';
					
					// ������ �ε��� ã��
					indexOp = token.operand.get(0).indexOf(op);
					
					// �ǿ����� ����
					op1 = token.operand.get(0).substring(0, indexOp);
					op2 = token.operand.get(0).substring(indexOp + 1);
					
					// modify �߰�
					Modify modify = new Modify();
					modify.len = 6;
					modify.addr = len / 2;
					modify.op = '+';
					modify.ref = op1;
					modifyTable.get(programCount).add(modify);
					modify = new Modify();
					modify.len = 6;
					modify.addr = len / 2;
					modify.op = op;
					modify.ref = op2;
					modifyTable.get(programCount).add(modify);
					
					hex = "000000";
				}else
					hex = Hex.getFormatedHex(Hex.dectohex(Integer.valueOf(token.operand.get(0))), 6);
				
			}else if(token.operator.equals("BYTE")){
				String parsing = token.operand.get(0).substring(token.operand.get(0).indexOf("'") + 1);
				parsing = parsing.substring(0, parsing.indexOf("'"));
				
				switch(token.operand.get(0).charAt(0)){
					case 'X':
						hex = parsing;
						break;
					case 'C':
						for(Character ch : parsing.toCharArray())
							hex += Hex.dectohex(ch.charValue());
						break;
					default:
						break;
				}
			}
			
			code = hex;
		}
		
		if(code != ""){
			len += code.length();
			
			// �� �ٿ��� object code�� �Ľ̵��� �ʾ��� ��
			if(locctr - lastCtr != format)
				objectCode.get(programCount).add("enter");
			
			lastCtr = locctr;
			objectCode.get(programCount).add(code);
		}
		
		// LTORG�� END�� ������ object_code�� ���ͷ� �߰�
		if(token.operator.equals("LTORG") || token.operator.equals("END"))
			while((len = addLiteral()) > 0)
				;
		
		return programCount;
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�. �Ű� : ������ ������Ʈ ���ϸ�
	 * ��ȯ : ���� ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������ ȭ�鿡 ������ش�.
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	private static void makeObjectcode(String fileName){
		// ���� �ʱ�ȭ
		int j, extIndex, lastAddr, TSize, start;
		Program prog;
		
		// �Է� ���� null�� �ƴϸ� ���Ϸ� ���
		if(fileName != null)
			try{
				File file = new File(outputFile);
				file.createNewFile();
				PrintStream ps = new PrintStream(file);
				System.setOut(ps);
			}catch(Exception e){
				e.printStackTrace();
				return;
			}finally{
				System.setOut(System.out);
			}
		
		// ������ object code�� ���� object code�� ����, ���
		for(int i = 0; i < programInfo.size(); i++){
			start = 0;
			lastAddr = 0;
			TSize = 0;
			j = 0;
			
			// H ���ڵ� ���
			prog = programInfo.get(i);
			System.out.println(String.format("H%-6s000000%06X", prog.name, prog.length));
			
			// D ���ڵ� ���
			if(extdef.size() > 0 && extdef.get(i).size() > 0){
				System.out.print("D");
				for(String def : extdef.get(i)){
					extIndex = searchSymbol(def);
					if(extIndex != -1)
						System.out.print(String.format("%-6s%06X", def, symTable.get(extIndex).addr));
				}
				System.out.println();
			}
			
			// R ���ڵ� ���
			if(extref.size() > 0 && extref.get(i).size() > 0){
				System.out.print("R");
				for(String ref : extref.get(i))
					System.out.print(String.format("%-6s", ref));
				System.out.println();
			}
			
			// T ���ڵ� ���
			for(String code : objectCode.get(i))
				if(code != null){
					if(TSize + code.length() > 60 || code.equals("enter")){
						j = objectCode.get(i).indexOf(code);
						
						System.out.print(String.format("T%06X%02X", lastAddr, TSize / 2));
						for(int l = start; l < j; l++)
							if(objectCode.get(i).get(l).equals("enter") == false)
								System.out.print(objectCode.get(i).get(l));
						System.out.println();
						
						lastAddr += TSize / 2;
						start = j;
						TSize = 0;
					}
					
					if(code.equals("enter") == false)
						TSize += code.length();
				}
			
			// ������ �κ� ���
			if(start < objectCode.get(i).size()){
				System.out.print(String.format("T%06X%02X", lastAddr, TSize / 2));
				for(int l = start; l < objectCode.get(i).size(); l++)
					if(objectCode.get(i).get(l).equals("enter") == false)
						System.out.print(objectCode.get(i).get(l));
				System.out.println();
				
			}
			
			// M ���ڵ� ���
			for(Modify modify : modifyTable.get(i))
				System.out.println(String.format("M%06X%02X%c%s", modify.addr, modify.len, modify.op, modify.ref));
			
			// E ���ڵ� ���
			System.out.print("E");
			if(prog.name.equals("COPY"))
				System.out.print("000000");
			System.out.println();
			System.out.println();
		}
		
		System.setOut(System.out);
	}
	
	private static void printEqual(Token token){
		String hex, op1, op2;
		char op = 0;
		int index_op, index_op1, index_op2, result;
		
		// EQU�� �ǿ����ڰ� ���� locctr�̸�
		if(token.operand.get(0).equals("*")){
			// symbol table�� �߰�
			Symbol symbol = new Symbol();
			symbol.symbol = token.label;
			symbol.addr = locctr;
			symTable.add(symbol);
			
			result = locctr;
		}else{
			// ������ ã��
			if(token.operand.get(0).contains("+"))
				op = '+';
			else if(token.operand.get(0).contains("-"))
				op = '-';
			else if(token.operand.get(0).contains("*"))
				op = '*';
			else if(token.operand.get(0).contains("/"))
				op = '/';
			
			// �����ڸ� ã�� ���ϸ�
			if(op == 0)
				return;
			
			// ������ �ε��� ã��
			index_op = token.operand.get(0).indexOf(op);
			
			// �ǿ����� ����
			op1 = token.operand.get(0).substring(0, index_op);
			op2 = token.operand.get(0).substring(index_op + 1);
			
			// �ǿ����� symTable �ε��� ã��
			index_op1 = searchSymbol(op1);
			index_op2 = searchSymbol(op2);
			
			// �ϳ��� ã�� ���ϸ�
			if(index_op1 == -1 || index_op2 == -1)
				return;
			
			// ���
			switch(op){
				case '+':
					result = symTable.get(index_op1).addr + symTable.get(index_op2).addr;
					break;
				case '-':
					result = symTable.get(index_op1).addr - symTable.get(index_op2).addr;
					break;
				case '*':
					result = symTable.get(index_op1).addr * symTable.get(index_op2).addr;
					break;
				case '/':
					result = symTable.get(index_op1).addr / symTable.get(index_op2).addr;
					break;
				default:
					return;
			}
			
		}
		
		// ������� ã��������
		if(result != -1){
			// ���
			hex = Hex.getFormatedHex(Hex.dectohex(result), 4);
			System.out.print(hex);
		}
		
		return;
	}
	
	/*
	 * �߰��ڵ带 �ܼ� â�� ����ϸ� sym_table�� �ɺ��� �����ϰ� LTORG, END�� ������ ���ͷ��� �߰��ϴ� �Լ�
	 */
	private static void printIntercode(){
		// �ܼ� â�� ���
		int instSize;
		String hex;
		
		for(Token token : tokenTable){
			// inst�� ����� �� �� ������
			if((instSize = getInstSize(token)) != -1){
				if(instSize == 0){
					// program ��ü ����
					if(programInfo.isEmpty() == false)
						programInfo.get(programInfo.size() - 1).length = locctr;
					
					Program prog = new Program();
					prog.name = token.label;
					programInfo.add(prog);
					
					// extdef, extref ����Ʈ ��ü �߰�
					extdef.add(new ArrayList<String>());
					extref.add(new ArrayList<String>());
					locctr = 0;
					System.out.println();
				}
				
				hex = Hex.getFormatedHex(Hex.dectohex(locctr), 4);
				System.out.print(hex);
			}
			
			// symTable�� �ɺ� �߰�
			if(token.label != "" && searchSymbol(token.label) == -1)
				if(token.operator.equals("EQU"))
					// EQU ���
					printEqual(token);
				else{
					Symbol symbol = new Symbol();
					symbol.symbol = token.label;
					symbol.addr = locctr;
					symTable.add(symbol);
				}
			
			// �ɺ� �߰� �� locctr inst ũ�� ��ŭ ����
			if(instSize > 0)
				locctr += instSize;
			
			// label, operator ���
			System.out.print("\t");
			if(token.label != "")
				System.out.print(token.label);
			System.out.print("\t" + token.operator + "\t");
			
			// extdef, extref ����
			if(extdef.size() < Constant.MAX_LINES && token.operator.equals("EXTDEF"))
				for(String op : token.operand)
					extdef.get(extdef.size() - 1).add(op);
			
			if(extref.size() < Constant.MAX_LINES && token.operator.equals("EXTREF"))
				for(String op : token.operand)
					extref.get(extref.size() - 1).add(op);
			
			// ���ͷ� symTable�� �߰�
			if(token.operand.isEmpty() == false)
				if(token.operand.get(0) != "" && token.operand.get(0).charAt(0) == '=')
					if(searchSymbol(token.operand.get(0)) == -1){
						Symbol symbol = new Symbol();
						symbol.symbol = token.operand.get(0);
						symbol.addr = -1;
						symTable.add(symbol);
					}
			
			// operand ���
			for(String op : token.operand){
				// ù op�� �ƴϸ�
				if(token.operand.indexOf(op) != 0)
					System.out.print(",");
				System.out.print(op);
			}
			
			System.out.println();
			
			// LTORG, END�� ������ �� ������ ���� ���ͷ� �߰� �� ���
			if(token.operator.equals("LTORG") || token.operator.equals("END"))
				printLiteral();
		}
		
		// ������ ���α׷� ���� ����
		programInfo.get(programInfo.size() - 1).length = locctr;
		System.out.println();
	}
	
	private static void printLiteral(){
		String hex;
		
		for(Symbol symbol : symTable)
			// �ɺ��� ���ͷ��̸�
			if(symbol.symbol.charAt(0) == '=' && symbol.addr == -1){
				symbol.addr = locctr;
				hex = Hex.getFormatedHex(Hex.dectohex(locctr), 4);
				System.out.print(hex);
				System.out.println("\t*\t" + symbol.symbol);
				locctr += getByteSize(symbol.symbol.substring(1));
			}
		
	}
	
	/*
	 * extref ���� ���� �̸��� �ε��� ��ȯ
	 */
	private static int searchExtref(String str, int programIndex){
		if(str == "")
			return -1;
		
		for(String ref : extref.get(programIndex))
			if(ref.equals(str))
				return extref.get(programIndex).indexOf(str);
		
		return -1;
	}
	
	private static int searchOpcode(String str){
		// �Է��� NULL�̸� ���� ��ȯ
		if(str == null || str == "")
			return -1;
		
		try{
			// OPCODE Ž��
			for(Inst tmp : inst)
				if(tmp.name.equals(str))
					return inst.indexOf(tmp);
			
		}catch(Exception e){
			e.printStackTrace();
		}
		
		return -1;
	}
	
	/*
	 * symbol_table���� ���� �̸��� symbol�� �����ϸ� index ����
	 */
	private static int searchSymbol(String symbol){
		if(symbol == "")
			return -1;
		
		for(Symbol sym : symTable)
			if(sym.symbol.equals(symbol))
				return symTable.indexOf(sym);
		
		return -1;
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. �н� 1�� ���� ȣ��ȴ�. �Ű�
	 * : �ҽ��ڵ��� ���ι�ȣ ��ȯ : �������� = 0 , ���� < 0 ���� : my_assembler ���α׷������� ���δ����� ��ū ��
	 * ������Ʈ ������ �ϰ� �ִ�.
	 * ----------------------------------------------------------
	 * -------------------------
	 */
	private static int tokenParsing(String input){
		// ���� �ʱ�ȭ
		Token tok = new Token();
		StringTokenizer st = new StringTokenizer(input, "\t");
		StringTokenizer opSt;
		
		try{
			// �ҽ� �� ������ �ǹ��ڸ� �������� �Ľ�
			if(input.charAt(0) != '\t')
				tok.label = st.nextToken();
			
			tok.operator = st.nextToken();
			
			String tmp = input.substring(input.indexOf(tok.operator) + tok.operator.length());
			if(tmp.length() >= 2)
				if((tmp.charAt(0) == '\t' && tmp.charAt(1) == '\t') == false)
					// operator ���� ���ڿ��� \t\t �� ������ operand �Ľ� ����
					if(st.hasMoreTokens()){
						opSt = new StringTokenizer(st.nextToken(), ",");
						while(opSt.hasMoreTokens())
							tok.operand.add(opSt.nextToken());
					}
			
			if(st.hasMoreTokens())
				tok.comment = st.nextToken();
			
		}catch(Exception e){
			e.printStackTrace();
			return -1;
		}
		
		tokenTable.add(tok);
		
		return 0;
	}
	
	private int initInputFile(){
		String inputLine;
		File file = new File(inputFile);
		Scanner sc;
		
		// ���� Ȯ��
		if((file.exists() && file.canRead()) == false)
			return -1;
		
		try{
			sc = new Scanner(file);
			
			// �ּ� . �� �����ϰ� �� �� �� �Է¹���
			while(sc.hasNextLine()){
				inputLine = sc.nextLine();
				if(inputLine.trim().charAt(0) != '.')
					inputData.add(inputLine);
			}
			
		}catch(Exception e){
			e.printStackTrace();
			return -1;
		}
		
		sc.close();
		
		return 0;
	}
	
	private int initInstFile(String instFile){
		String instLine;
		StringTokenizer st;
		Inst inst;
		File file = new File(instFile);
		Scanner sc;
		
		// ���� Ȯ��
		if((file.exists() && file.canRead()) == false)
			return -1;
		
		try{
			sc = new Scanner(file);
			
			// �� �پ� �Է¹޾� �Ľ�
			while(sc.hasNextLine()){
				instLine = sc.nextLine();
				st = new StringTokenizer(instLine, "|");
				
				try{
					inst = new Inst();
					inst.name = st.nextToken();
					inst.format = st.nextToken();
					inst.opcode = st.nextToken();
					inst.operandSize = Integer.valueOf(st.nextToken());
					MyAssembler.inst.add(inst);
				}catch(Exception e){
					e.printStackTrace();
					sc.close();
					return -1;
				}
				
			}
		}catch(Exception e){
			e.printStackTrace();
			return -1;
		}
		
		sc.close();
		
		return 0;
	}
	
	/*
	 * token �ϳ��� object code�� ����� �Լ�
	 */
	public int initMyAssembler(){
		int result;
		
		if((result = initInstFile("inst.data")) < 0)
			return -1;
		if((result = initInputFile()) < 0)
			return -1;
		return result;
	}
}
