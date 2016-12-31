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
	 * --------- 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다. 매계 : 없음 반환 : 정상종료 = 0
	 * , 에러 발생 = -1 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기 위해서 파일 단위로 관리하여
	 * 프로그램 초기화를 통해 정보를 읽어 올 수 있도록 구현하였다.
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
	 * 현재 locctr값과 symTable의 addr를 비교하여 같은 값을 갖는 리터럴을 objectCode에 추가하는 함수
	 */
	private static int addLiteral(){
		int size;
		String code = "";
		
		for(Symbol symbol : symTable)
			if(locctr == symbol.addr && symbol.symbol.charAt(0) == '='){
				// '와 ' 사이의 문자열 파싱
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
				
				// 전 줄에서 object code가 파싱되지 않았을 때
				if(locctr - lastCtr != size)
					objectCode.get(objectCode.size() - 1).add("enter");
				
				objectCode.get(objectCode.size() - 1).add(code);
				return code.length();
			}
		
		return 0;
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다. 패스1에서는.. 1. 프로그램 소스를 스캔하여
	 * 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰 테이블을 생성한다.
	 * 
	 * 매계 : 없음 반환 : 정상 종료 = 0 , 에러 = < 0 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간
	 * 상태이다. 따라서 에러에 대한 검사 루틴을 추가해야 한다.
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	public static int assemPass1(){
		// input_data를 기반으로 tokenTable에 파싱
		for(String input : inputData)
			if(tokenParsing(input) < 0)
				return -1;
		
		printIntercode();
		return 0;
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다. 패스 2에서는 프로그램을
	 * 기계어로 바꾸는 작업은 라인 단위로 수행된다. 다음과 같은 작업이 수행되어 진다. 1. 실제로 해당 어셈블리 명령어를 기계어로
	 * 바꾸는 작업을 수행한다. 매계 : 없음 반환 : 정상종료 = 0, 에러발생 = < 0 주의 :
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
	 * --------- 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을 생성하는 함수이다.
	 * 매계 : 기계어 목록 파일 반환 : 정상종료 = 0 , 에러 < 0 주의 : 기계어 목록파일 형식은 다음과 같다.
	 * 
	 * ==========================================================================
	 * ===== | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
	 * ==============================
	 * =================================================
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	/*
	 * BYTE operand의 크기 리턴
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
	 * --------- 설명 : 어셈블리 할 소스코드를 읽어오는 함수이다. 매계 : 어셈블리할 소스파일명 반환 : 정상종료 = 0 ,
	 * 에러 < 0 주의 :
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	/*
	 * 파싱된 토큰 하나(instruction)의 size를 리턴
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
			// operator가 opcode이면
			if(inst.get(index).format.contains("/")){
				// 형식에 / 문자가 존재 => 3 또는 4
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
	 * --------- 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다. 매계 : 토큰 단위로 구분된 문자열 반환 : 정상종료
	 * = 기계어 테이블 인덱스, 에러 < 0 주의 :
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
		
		// inst 형식 찾기
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
		
		// opcode 검색
		operator = token.operator;
		if(operator.charAt(0) == '+')
			operator = operator.substring(1);
		
		// opcode 이면 opcode 복사
		if((opcodeIndex = searchOpcode(operator)) >= 0){
			if(format > 1)
				code = inst.get(opcodeIndex).opcode;
			
			if(format > 2){
				// #, @(immediate, indirect) 검색
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
				
				// x, e 찾기
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
			
			// 4형식이면
			if(format == 4){
				num = Hex.getDec(code.charAt(2));
				num += Constant.E;
				tmp = code.toCharArray();
				tmp[2] = Hex.getHex(num);
				code = String.valueOf(tmp);
			}
			
			// TA 찾기
			if(format > 2)
				if(operand == ""){
					TA = -1;
					hex = "000";
				}else if(Hex.isDigit(operand)){
					// 숫자이면
					if(immediate)
						hex = Hex.getFormatedHex(Hex.dectohex(Integer.valueOf(operand)), 3);
					else
						TA = Integer.valueOf(operand);
				}else if((symbolIndex = searchExtref(operand, programCount)) != -1){
					// extref이면
					TA = -1;
					hex = "000";
				}else if((symbolIndex = searchSymbol(operand)) != -1)
					// symbol이면
					TA = symTable.get(symbolIndex).addr;
				else
					TA = -1;
			
			// 주소 계산
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
			// opcode가 아니고 WORD 혹은 BYTE일 때
			
			if(token.operator.equals("WORD")){
				
				// 숫자로 이루어져 있지 않으면
				if(!Hex.isDigit(token.operand.get(0))){
					char op = 0;
					int indexOp;
					String op1, op2;
					
					// 연산자 찾기
					if(token.operand.get(0).contains("+"))
						op = '+';
					else if(token.operand.get(0).contains("-"))
						op = '-';
					else if(token.operand.get(0).contains("*"))
						op = '*';
					else if(token.operand.get(0).contains("/"))
						op = '/';
					
					// 연산자 인덱스 찾기
					indexOp = token.operand.get(0).indexOf(op);
					
					// 피연산자 복사
					op1 = token.operand.get(0).substring(0, indexOp);
					op2 = token.operand.get(0).substring(indexOp + 1);
					
					// modify 추가
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
			
			// 전 줄에서 object code가 파싱되지 않았을 때
			if(locctr - lastCtr != format)
				objectCode.get(programCount).add("enter");
			
			lastCtr = locctr;
			objectCode.get(programCount).add(code);
		}
		
		// LTORG나 END를 만나면 object_code에 리터럴 추가
		if(token.operator.equals("LTORG") || token.operator.equals("END"))
			while((len = addLiteral()) > 0)
				;
		
		return programCount;
	}
	
	/*
	 * --------------------------------------------------------------------------
	 * --------- 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다. 매계 : 생성할 오브젝트 파일명
	 * 반환 : 없음 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어 화면에 출력해준다.
	 * 
	 * --------------------------------------------------------------------------
	 * ---------
	 */
	
	private static void makeObjectcode(String fileName){
		// 변수 초기화
		int j, extIndex, lastAddr, TSize, start;
		Program prog;
		
		// 입력 값이 null이 아니면 파일로 출력
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
		
		// 생성한 object code로 실제 object code를 생성, 출력
		for(int i = 0; i < programInfo.size(); i++){
			start = 0;
			lastAddr = 0;
			TSize = 0;
			j = 0;
			
			// H 레코드 출력
			prog = programInfo.get(i);
			System.out.println(String.format("H%-6s000000%06X", prog.name, prog.length));
			
			// D 레코드 출력
			if(extdef.size() > 0 && extdef.get(i).size() > 0){
				System.out.print("D");
				for(String def : extdef.get(i)){
					extIndex = searchSymbol(def);
					if(extIndex != -1)
						System.out.print(String.format("%-6s%06X", def, symTable.get(extIndex).addr));
				}
				System.out.println();
			}
			
			// R 레코드 출력
			if(extref.size() > 0 && extref.get(i).size() > 0){
				System.out.print("R");
				for(String ref : extref.get(i))
					System.out.print(String.format("%-6s", ref));
				System.out.println();
			}
			
			// T 레코드 출력
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
			
			// 마지막 부분 출력
			if(start < objectCode.get(i).size()){
				System.out.print(String.format("T%06X%02X", lastAddr, TSize / 2));
				for(int l = start; l < objectCode.get(i).size(); l++)
					if(objectCode.get(i).get(l).equals("enter") == false)
						System.out.print(objectCode.get(i).get(l));
				System.out.println();
				
			}
			
			// M 레코드 출력
			for(Modify modify : modifyTable.get(i))
				System.out.println(String.format("M%06X%02X%c%s", modify.addr, modify.len, modify.op, modify.ref));
			
			// E 레코드 출력
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
		
		// EQU의 피연산자가 현재 locctr이면
		if(token.operand.get(0).equals("*")){
			// symbol table에 추가
			Symbol symbol = new Symbol();
			symbol.symbol = token.label;
			symbol.addr = locctr;
			symTable.add(symbol);
			
			result = locctr;
		}else{
			// 연산자 찾기
			if(token.operand.get(0).contains("+"))
				op = '+';
			else if(token.operand.get(0).contains("-"))
				op = '-';
			else if(token.operand.get(0).contains("*"))
				op = '*';
			else if(token.operand.get(0).contains("/"))
				op = '/';
			
			// 연산자를 찾지 못하면
			if(op == 0)
				return;
			
			// 연산자 인덱스 찾기
			index_op = token.operand.get(0).indexOf(op);
			
			// 피연산자 복사
			op1 = token.operand.get(0).substring(0, index_op);
			op2 = token.operand.get(0).substring(index_op + 1);
			
			// 피연산자 symTable 인덱스 찾기
			index_op1 = searchSymbol(op1);
			index_op2 = searchSymbol(op2);
			
			// 하나라도 찾지 못하면
			if(index_op1 == -1 || index_op2 == -1)
				return;
			
			// 계산
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
		
		// 결과값이 찾아졌으면
		if(result != -1){
			// 출력
			hex = Hex.getFormatedHex(Hex.dectohex(result), 4);
			System.out.print(hex);
		}
		
		return;
	}
	
	/*
	 * 중간코드를 콘솔 창에 출력하며 sym_table에 심볼을 저장하고 LTORG, END를 만나면 리터럴을 추가하는 함수
	 */
	private static void printIntercode(){
		// 콘솔 창에 출력
		int instSize;
		String hex;
		
		for(Token token : tokenTable){
			// inst의 사이즈를 잴 수 있으면
			if((instSize = getInstSize(token)) != -1){
				if(instSize == 0){
					// program 객체 생성
					if(programInfo.isEmpty() == false)
						programInfo.get(programInfo.size() - 1).length = locctr;
					
					Program prog = new Program();
					prog.name = token.label;
					programInfo.add(prog);
					
					// extdef, extref 리스트 객체 추가
					extdef.add(new ArrayList<String>());
					extref.add(new ArrayList<String>());
					locctr = 0;
					System.out.println();
				}
				
				hex = Hex.getFormatedHex(Hex.dectohex(locctr), 4);
				System.out.print(hex);
			}
			
			// symTable에 심볼 추가
			if(token.label != "" && searchSymbol(token.label) == -1)
				if(token.operator.equals("EQU"))
					// EQU 출력
					printEqual(token);
				else{
					Symbol symbol = new Symbol();
					symbol.symbol = token.label;
					symbol.addr = locctr;
					symTable.add(symbol);
				}
			
			// 심볼 추가 후 locctr inst 크기 만큼 증가
			if(instSize > 0)
				locctr += instSize;
			
			// label, operator 출력
			System.out.print("\t");
			if(token.label != "")
				System.out.print(token.label);
			System.out.print("\t" + token.operator + "\t");
			
			// extdef, extref 저장
			if(extdef.size() < Constant.MAX_LINES && token.operator.equals("EXTDEF"))
				for(String op : token.operand)
					extdef.get(extdef.size() - 1).add(op);
			
			if(extref.size() < Constant.MAX_LINES && token.operator.equals("EXTREF"))
				for(String op : token.operand)
					extref.get(extref.size() - 1).add(op);
			
			// 리터럴 symTable에 추가
			if(token.operand.isEmpty() == false)
				if(token.operand.get(0) != "" && token.operand.get(0).charAt(0) == '=')
					if(searchSymbol(token.operand.get(0)) == -1){
						Symbol symbol = new Symbol();
						symbol.symbol = token.operand.get(0);
						symbol.addr = -1;
						symTable.add(symbol);
					}
			
			// operand 출력
			for(String op : token.operand){
				// 첫 op가 아니면
				if(token.operand.indexOf(op) != 0)
					System.out.print(",");
				System.out.print(op);
			}
			
			System.out.println();
			
			// LTORG, END를 만나면 이 때까지 만난 리터럴 추가 및 출력
			if(token.operator.equals("LTORG") || token.operator.equals("END"))
				printLiteral();
		}
		
		// 마지막 프로그램 길이 저장
		programInfo.get(programInfo.size() - 1).length = locctr;
		System.out.println();
	}
	
	private static void printLiteral(){
		String hex;
		
		for(Symbol symbol : symTable)
			// 심볼이 리터럴이면
			if(symbol.symbol.charAt(0) == '=' && symbol.addr == -1){
				symbol.addr = locctr;
				hex = Hex.getFormatedHex(Hex.dectohex(locctr), 4);
				System.out.print(hex);
				System.out.println("\t*\t" + symbol.symbol);
				locctr += getByteSize(symbol.symbol.substring(1));
			}
		
	}
	
	/*
	 * extref 에서 같은 이름의 인덱스 반환
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
		// 입력이 NULL이면 에러 반환
		if(str == null || str == "")
			return -1;
		
		try{
			// OPCODE 탐색
			for(Inst tmp : inst)
				if(tmp.name.equals(str))
					return inst.indexOf(tmp);
			
		}catch(Exception e){
			e.printStackTrace();
		}
		
		return -1;
	}
	
	/*
	 * symbol_table에서 같은 이름의 symbol이 존재하면 index 리턴
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
	 * --------- 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다. 패스 1로 부터 호출된다. 매계
	 * : 소스코드의 라인번호 반환 : 정상종료 = 0 , 에러 < 0 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및
	 * 오브젝트 관리를 하고 있다.
	 * ----------------------------------------------------------
	 * -------------------------
	 */
	private static int tokenParsing(String input){
		// 변수 초기화
		Token tok = new Token();
		StringTokenizer st = new StringTokenizer(input, "\t");
		StringTokenizer opSt;
		
		try{
			// 소스 줄 끝까지 탭문자를 기준으로 파싱
			if(input.charAt(0) != '\t')
				tok.label = st.nextToken();
			
			tok.operator = st.nextToken();
			
			String tmp = input.substring(input.indexOf(tok.operator) + tok.operator.length());
			if(tmp.length() >= 2)
				if((tmp.charAt(0) == '\t' && tmp.charAt(1) == '\t') == false)
					// operator 다음 문자열이 \t\t 가 나오면 operand 파싱 안함
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
		
		// 파일 확인
		if((file.exists() && file.canRead()) == false)
			return -1;
		
		try{
			sc = new Scanner(file);
			
			// 주석 . 은 제외하고 한 줄 씩 입력받음
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
		
		// 파일 확인
		if((file.exists() && file.canRead()) == false)
			return -1;
		
		try{
			sc = new Scanner(file);
			
			// 한 줄씩 입력받아 파싱
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
	 * token 하나로 object code를 만드는 함수
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
