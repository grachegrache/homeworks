package main;

import java.io.File;
import java.io.FileInputStream;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.StringTokenizer;

import data.Inst;
import data.Modify;
import data.Program;
import data.Symbol;
import data.Token;

public class MyAssembler{
	private int					codeIndex;
	private ArrayList<ArrayList<String>>	extdef;
	private ArrayList<ArrayList<String>>	extref;
	private ArrayList<String>			inputData;
	private String				inputFile;
	private ArrayList<Inst>			inst;
	private int					labelNum;
	private static int					lastCtr;
	private static int locctr;
	private int					len;
	private static int					lineNum;
	private static int					mIndex;
	private ArrayList<ArrayList<Modify>>			modifyTable;
	private ArrayList<ArrayList<String>>			objectCode;
	private static String				outputFile;
	private ArrayList<Program> programInfo;
	private static ArrayList<Symbol>			symTable;
	private static ArrayList<Token>				tokenTable;
	
	public MyAssembler(){
		int maxLines = Constant.MAX_LINES;
		
		extdef = new ArrayList<ArrayList<String>>();
		extref = new ArrayList<ArrayList<String>>();
		programInfo = new ArrayList<Program>();
		
		inputData	= new ArrayList<String>(maxLines);
		inst		= new ArrayList<Inst>(Constant.MAX_INST);
		modifyTable		= new ArrayList<ArrayList<Modify>>();
		objectCode	= new ArrayList<ArrayList<String>>();
		symTable	= new ArrayList<Symbol>(maxLines);
		tokenTable	= new ArrayList<Token>(maxLines);
	}
	
	/* -----------------------------------------------------------------------------------
	 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. 
	 * �Ű� : ����
	 * ��ȯ : �������� = 0 , ���� �߻� = -1
	 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� 
	 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
	 *		   �����Ͽ���. 
	 * -----------------------------------------------------------------------------------
	 */

	public int initMyAssembler(){
		int result;

		if((result = initInstFile("inst.data")) < 0)
			return -1;
		if((result = initInputFile()) < 0)
			return -1;
		return result;
	}

	/* -----------------------------------------------------------------------------------
	 * ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�. 
	 *		   �н�1������..
	 *		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
	 *		   ���̺��� �����Ѵ�. 
	 * 
	 * �Ű� : ����
	 * ��ȯ : ���� ���� = 0 , ���� = < 0 
	 * ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
	 *	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�. 
	 *		
	 * -----------------------------------------------------------------------------------
	 */

	public static int assemPass1(){
		//input_data�� ������� token_table�� �Ľ�
		int i = 0;
		for(; i < lineNum; i++)
			if(tokenParsing() < 0)
				return -1;
		printIntercode();
		return 0;
	}

	/* -----------------------------------------------------------------------------------
	 * ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�. 
	 *		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�. 
	 *		   ������ ���� �۾��� ����Ǿ� ����. 
	 *		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�. 
	 * �Ű� : ����
	 * ��ȯ : �������� = 0, �����߻� = < 0 
	 * ���� : 
	 * -----------------------------------------------------------------------------------
	 */

	public static int assemPass2(){
		int i;
		Integer k;
		locctr = 0;
		lastCtr = 0;
		mIndex = 0;
		k = -1;
		
		for(i = 0; i < lineNum; i++)
			makeIntercode(k);

		makeObjectcode();
		return 0;

	}
	/* -----------------------------------------------------------------------------------
	 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)�� 
	 *        �����ϴ� �Լ��̴�. 
	 * �Ű� : ���� ��� ����
	 * ��ȯ : �������� = 0 , ���� < 0 
	 * ���� : ���� ������� ������ ������ ����. 
	 *	
	 *	===============================================================================
	 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
	 *	===============================================================================	   
	 *		
	 * -----------------------------------------------------------------------------------
	 */

	private int initInstFile(String instFile){
		String instLine;
		StringTokenizer st;
		Inst inst;
		File file = new File(instFile);
				
		//���� Ȯ��
		if((file.exists() && file.canRead()) == false)
			return -1;
		Scanner sc = new Scanner(file);
		
		try{
			//�� �پ� �Է¹޾� �Ľ�
			while(sc.hasNextLine()){
				instLine = sc.nextLine();
				st = new StringTokenizer(instLine,"|");
				
				try{
					inst = new Inst();
					inst.name = st.nextToken();
					inst.format = st.nextToken();
					inst.opcode = st.nextToken();
					inst.operandSize = Integer.valueOf(st.nextToken());
				}catch(Exception e){
					e.printStackTrace();
					return -1;
				}
				
			}
		}catch(Exception e){
			e.printStackTrace();
			return -1;
		}finally{
			sc.close();
		}
		
		return 0;
	}

	/* -----------------------------------------------------------------------------------
	 * ���� : ����� �� �ҽ��ڵ带 �о���� �Լ��̴�. 
	 * �Ű� : ������� �ҽ����ϸ�
	 * ��ȯ : �������� = 0 , ���� < 0  
	 * ���� : 
	 *		
	 * -----------------------------------------------------------------------------------
	 */

	private int initInputFile(){
		String inputLine;
		File file = new File(inputFile);
		lineNum = 0;
		
		//���� Ȯ��
		if((file.exists() && file.canRead()) == false)
			return -1;
		Scanner sc = new Scanner(file);
		
		try{
			//�ּ� . �� �����ϰ� �� �� �� �Է¹���
			while(sc.hasNextLine()){
				inputLine = sc.nextLine();
				if(inputLine.trim().charAt(0) != '.'){
					inputData.add(inputLine);
					lineNum++;
					
				}
			}
		
		}catch(Exception e){
			e.printStackTrace();
			return -1;
		}finally{
			sc.close();
		}
		
		return 0;
	}

	/* -----------------------------------------------------------------------------------
	 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. 
	 *        �н� 1�� ���� ȣ��ȴ�. 
	 * �Ű� : �ҽ��ڵ��� ���ι�ȣ  
	 * ��ȯ : �������� = 0 , ���� < 0 
	 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�. 
	 * -----------------------------------------------------------------------------------
	 */
	private int tokenParsing(String input){
		//���� �ʱ�ȭ
		int tabCount;
		tabCount = 0;
		String str;
		Token tok = new Token();
		StringTokenizer st = new StringTokenizer(input, "\t");
		StringTokenizer opSt;
		
		try{
			//�ҽ� �� ������ �ǹ��ڸ� �������� �Ľ�
			tok.label = st.nextToken();
			tok.operator = st.nextToken();
			
			opSt = new StringTokenizer(st.nextToken(), ",");
			while(opSt.hasMoreTokens())
				tok.operand.add(opSt.nextToken());
			
			if(st.hasMoreTokens())
				tok.comment = st.nextToken();
			
		}catch(Exception e){
			e.printStackTrace();
			return -1;
		}
		
		tokenTable.add(tok);
		
//		for(; i < strlen(input_data[index]); i++)
//			if(tab_count == 0 || tab_count == 1){
//				//label, operator�� ���
//				if(input_data[index][i] == '\t'){
//					char* dest = strndup(&input_data[index][last_index], i - last_index);
//
//					//ù �Ľ��� ���
//					if(tab_count == 0)
//						//��ū�� START, END, OPCODE �� �� �ϳ��̸� operator�� ������
//						//�Ǵ�, ù ���ڰ� +, #, @�̰ų� ��ū�� EXTDEF, EXTREF, LTORG�̸� operator�� ������
//						if(search_opcode(dest) != -1 || ((strcmp(dest, "START") && strcmp(dest, "END") && strcmp(
//								dest, "EXTDEF") && strcmp(dest, "EXTREF")) == 0) || dest[0] == '+'){
//							line->label = "";
//							tab_count = 1;
//						}
//
//					switch(tab_count){
//						case 0:
//							line->label = dest;
//							break;
//						case 1:
//							line->operator = dest;
//							break;
//						default:
//							break;
//					}
//					tab_count++;
//					last_index = i + 1;
//				}
//			}else if(tab_count == 2){
//				//operand�� ���
//				if(input_data[index][i] == '\t' || input_data[index][i] == ','){
//					line->operand[operand_index++] = strndup(&input_data[index][last_index],
//							i - last_index);
//					last_index = i + 1;
//
//					if(input_data[index][i] == '\t'){
//						if(input_data[index][last_index] == '\t'){
//							//ù ��° �� ���� ���� ���ڰ� �� �����̸� �ǳʶ�
//							last_index++;
//							i++;
//						}
//						tab_count++;
//					}
//				}
//			}else if(tab_count > 3)
//				return -1;
//
//		if(i != last_index)
//			//���� �Ľ����� ���� ��ū�� ������
//			if(tab_count == 0){
//				//LTORG�� ���� �ϳ��� ��ɾ�
//				line->operator = strndup(&input_data[index][last_index], i - last_index);
//
//			}else if(tab_count == 1){
//				//operator�� ������� �ʾ���
//				line->operator = strndup(&input_data[index][last_index], i - last_index);
//
//			}else if(tab_count == 2){
//				//������ operand�� ������� �ʾ���
//				line->operand[operand_index] = strndup(&input_data[index][last_index], i - last_index);
//
//			}else if(tab_count == 3){
//				//operand ������ �� ���ڰ� ���� => �ּ��� ������
//				line->comment = strndup(&input_data[index][last_index], i - last_index);
//
//			}
//		token_table[index] = line;

		return 0;
	}
	/* -----------------------------------------------------------------------------------
	 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. 
	 * �Ű� : ��ū ������ ���е� ���ڿ� 
	 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0 
	 * ���� : 
	 *		
	 * -----------------------------------------------------------------------------------
	 */

	int searchOpcode(String str){
	//�Է��� NULL�̸� ���� ��ȯ
		if(str == null || str == "")
			return -1;

		try{
	//OPCODE Ž��
		for(Inst tmp :inst)
			if(tmp.name.equals(str))
				return inst.indexOf(tmp);
		}catch(Exception e){
			e.printStackTrace();
			return -1;
		}
		
	}

	/* -----------------------------------------------------------------------------------
	 * ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�. 
	 * �Ű� : ������ ������Ʈ ���ϸ� 
	 * ��ȯ : ����
	 * ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������ 
	 *        ȭ�鿡 ������ش�. 
	 *		
	 * -----------------------------------------------------------------------------------
	 */

	 private void makeObjectcode(String fileName){
	//���� �ʱ�ȭ
		int j, extIndex, lastAddr, TSize, start;
		boolean isEnd;
		Program prog;

		//�Է� ���� null�� �ƴϸ� ���Ϸ� ���
		if(fileName != null){
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
		}
		
		//������ object code�� ���� object code�� ����, ���
		for(int i=0;i<programInfo.size();i++){
			isEnd = false;
			start = 0;
			lastAddr = 0;
			TSize = 0;
			
			//H ���ڵ� ���
			prog = programInfo.get(i);
			System.out.println(String.format("H%-6s000000%06X",prog.name,prog.length));
			
			//D ���ڵ� ���
			if(extdef.size() > 0 && extdef.get(i).size() > 0){
				System.out.print("D");
				for(String def : extdef.get(i)){
					extIndex = searchSymbol(def);
					System.out.print(String.format("%-6s%06X", def,symTable.get(extIndex).addr));
				}
				System.out.println();
			}
			
			//R ���ڵ� ���
			if(extref.size() > 0 && extref.get(i).size() > 0){
				System.out.print("R");
				for(String ref : extref.get(i))
					System.out.print(String.format("%-6s", ref));
				System.out.println();
			}
			
			//T ���ڵ� ���
			for(String code : objectCode.get(i)){
				if(code != null){
					if(TSize + code.length() > 60){
						j = objectCode.get(i).indexOf(code);
						
						System.out.print(String.format("T%06X%02X",lastAddr,TSize/2));
						for(int l = start;l<j;l++)
							System.out.print(objectCode.get(i).get(l));
						System.out.println();
						
						lastAddr += TSize / 2;
						start = j;
						TSize = 0;
					}
					
					TSize += code.length();
				}
			}
			
			//������ �κ� ���
			if(start != j){
				System.out.print(String.format("T%06X%02X",lastAddr,TSize/2));
				for(int l = start;l<objectCode.size();l++)
					System.out.print(objectCode.get(i).get(l));
				System.out.println();
				
			}
			
			//M ���ڵ� ���
			for(Modify modify : modifyTable.get(i))
				System.out.println(String.format("M%06X%02X%c%s",modify.addr,modify.len,modify.op,modify.ref));
			
			//E ���ڵ� ���
			System.out.print("E");
			if(prog.name == "START")
				System.out.print("000000");
			System.out.println();
			System.out.println();
		}
		
		System.setOut(System.out);
	}

	/*
	 * token �ϳ��� object code�� ����� �Լ� 
	 */
	private void makeIntercode(Token token){
		int i, opcodeIndex, symbolIndex, format, TA, immediate, regNum, isExist, num;
		String hex, operator, operand, start;
		immediate = 0;
		isExist = 0;
		Program prog;
		
		format = getInstSize(token);
		if(format == 0){
			len = 0;
			lastCtr = 0;
			locctr = 0;
			object_code[code_index++] = 0;
			object_code[code_index++] = 0;
		}else if(format > 0){
			locctr += format;
		}

	//opcode �˻�
		operator = token->operator;
		if(token->operator[0] == '+')
			operator += 1;

	//opcode�� ������
		if((opcode_index = search_opcode(operator)) >= 0){

			//���Ŀ� ���� �����Ҵ�
			switch(format){
				case 1:
					object_code[code_index] = calloc(3, sizeof(char));
					break;
				case 2:
					object_code[code_index] = calloc(5, sizeof(char));
					break;
				case 3:
					object_code[code_index] = calloc(7, sizeof(char));
					break;
				case 4:
					object_code[code_index] = calloc(9, sizeof(char));
					break;
				default:
					break;
			}

			if(format > 1)
				strcpy(object_code[code_index], inst[opcode_index][2]);

			if(format > 2){
				object_code[code_index][2] = '0';
				object_code[code_index][3] = 0;
			}else if(format == 2)
				object_code[code_index][2] = 0;

			if(format > 2){
				//#, @(immediate, indirect) �˻�
				operand = token->operand[0];
				num = get_dec(object_code[code_index][1]);

				switch(operand[0]){
					case '#':
						num += I;
						operand += 1;
						immediate = 1;
						break;
					case '@':
						num += N;
						operand += 1;
						break;
					default:
						num += N + I;
						break;
				}
				object_code[code_index][1] = get_hex(num);

				//x, e ã��
				for(i = 1; i < 3; i++)
					if(token->operand[i] != NULL)
						if(strcmp(token->operand[i], "X") == 0){
							is_exist = 1;
							break;
						}

				if(is_exist){
					num = get_dec(object_code[code_index][2]);
					num += op_X;
					object_code[code_index][2] = get_hex(num);
				}
			}
			if(format == 4){
				num = get_dec(object_code[code_index][2]);
				num += E;
				object_code[code_index][2] = get_hex(num);
			}

			//TA �˻�
			if(format > 2){
				if(strcmp(operand, "") == 0){
					TA = -1;
					hex = "000";
				}else if(is_digit(operand)){
					if(immediate)
						hex = get_formated_hex(dectohex(atoi(operand)), 3);
					else
						TA = atoi(operand);
				}else if((symbol_index = search_extref(operand, k)) != -1){
					TA = -1;
					hex = "000";
				}else if((symbol_index = search_symbol(operand)) != -1){
					TA = sym_table[symbol_index].addr;
				}else
					TA = -1;
			}

			//�ּ� ���
			if(immediate == 0){
				switch(format){
					case 2:
						for(i = 0; i < 2; i++){
							if((reg_num = get_reg_num(token->operand[i])) != -1)
								hex = dectohex(reg_num);
							else
								hex = "0";
							strcat(object_code[code_index], hex);
						}
						break;
					case 3:
						if(TA != -1){
							if(strcmp(operator, "RSUB") != 0){
								if(TA - locctr >= 16 * 16 * 16){
									num = get_dec(object_code[code_index][2]);
									num += op_B;
									object_code[code_index][2] = get_hex(num);

								}else{
									num = get_dec(object_code[code_index][2]);
									num += P;
									object_code[code_index][2] = get_hex(num);

									hex = get_formated_hex(dectohex(TA - locctr), 3);
									strcat(object_code[code_index], hex);
								}
							}
						}else if(strcmp(hex, "000") == 0)
							strcat(object_code[code_index], hex);
						break;
					case 4:
						hex = "00000";
						m_table[m_index].program_index = k;
						m_table[m_index].len = 5;
						m_table[m_index].addr = len / 2 + 1;
						m_table[m_index].op = '+';
						m_table[m_index].ref = token->operand[0];
						m_index++;
						strcat(object_code[code_index], hex);
						break;
					default:
						break;
				}
			}else if(hex != 0)
				strcat(object_code[code_index], hex);

		}else if((strcmp(token->operator, "WORD") && strcmp(token->operator, "BYTE")) == 0){
			//opcode�� �ƴϰ� WORD Ȥ�� BYTE�� ��
			if(strcmp(token->operator, "WORD") == 0){
				//���ڷ� �̷���� ���� ������
				if(!is_digit(token->operand[0])){
					char op;
					int index_op, index_op1, index_op2;
					char* op1;
					char* op2;

					//������ ã��
					if(strstr(token->operand[0], "+") != NULL){
						op = '+';
					}else if(strstr(token->operand[0], "-") != NULL){
						op = '-';
					}else if(strstr(token->operand[0], "*") != NULL){
						op = '*';
					}else if(strstr(token->operand[0], "/") != NULL){
						op = '/';
					}

					//������ �ε��� ã��
					for(index_op = 0; index_op < strlen(token->operand[0]); index_op++)
						if(token->operand[0][index_op] == op)
							break;

					//�ǿ����� ����
					op1 = strndup(token->operand[0], index_op);
					op2 = strdup(token->operand[0] + index_op + 1);

					m_table[m_index].program_index = k;
					m_table[m_index].len = 6;
					m_table[m_index].addr = len / 2;
					m_table[m_index].op = '+';
					m_table[m_index].ref = op1;
					m_index++;
					m_table[m_index].program_index = k;
					m_table[m_index].len = 6;
					m_table[m_index].addr = len / 2;
					m_table[m_index].op = op;
					m_table[m_index].ref = op2;
					m_index++;

					hex = "000000";
				}else
					hex = get_formated_hex(dectohex(atoi(token->operand[0])), 6);

			}else if(strcmp(token->operator, "BYTE") == 0){
				start = strstr(token->operand[0], "'") + 1;

				for(i = 0; i < strlen(start); i++)
					if(start[i] == '\'')
						break;

				switch(token->operand[0][0]){
					case 'X':
						hex = strndup(start, i);
						break;
					case 'C':
						hex = calloc(i * 2, sizeof(char));
						hex[0] = 0;
						for(i = 0; i < strlen(start) - 1; i++)
							strcat(hex, dectohex(start[i]));
						break;
					default:
						break;
				}

			}

			object_code[code_index] = hex;

		}

		if(object_code[code_index] != NULL){
			len += strlen(object_code[code_index]);
			//�� �ٿ��� object code�� �Ľ̵��� �ʾ��� ��
			if(locctr - last_ctr != format){
				object_code[code_index + 1] = object_code[code_index];
				object_code[code_index] = 0;
				code_index++;
			}
			last_ctr = locctr;
			code_index++;
		}

	//LTORG�� END�� ������ object_code�� ���ͷ� �߰�
		if((strcmp(token->operator, "LTORG") && strcmp(token->operator, "END")) == 0)
			while((len = add_literal()) > 0)
				;

	}

	/*
	 * �߰��ڵ带 �ܼ� â�� ����ϸ�
	 * sym_table�� �ɺ��� �����ϰ�
	 * LTORG, END�� ������ ���ͷ��� �߰��ϴ� �Լ�
	 * 
	 */
	 private void printIntercode(){
	//�ܼ� â�� ���
		int i, j, instSize, index, opcodeIndex;
		String hex;
		index = 0;

		for(Token token : tokenTable){
			if((instSize = getInstSize(token)) != -1){
				if(instSize == 0){
					//program ��ü ����
					if(programInfo.isEmpty())
						programInfo.add(new Program());
					else{
						if(programInfo.size() != 1)
							programInfo.add(new Program());
						programInfo.get(programInfo.size()-1).length = locctr;
					}
					
					//extdef, extref
					extdef.add(new ArrayList<String>());
					extref.add(new ArrayList<String>());
					locctr = 0;
				}
			}
		}
		for(i = 0; i < line_num; i++){
			token = token_table[i];

			if((instSize = get_inst_size(token)) != -1){
				if(instSize == 0){
					k++;
					if(k > 0){
						program_length[k - 1] = locctr;
						def_index[k] = def_index[k - 1];
						ref_index[k] = ref_index[k - 1];
					}
					locctr = 0;
				}

				hex = get_formated_hex(dectohex(locctr), 4);
				printf("%s", hex);

			}

			//sym_table�� �ɺ� �߰�
			if(token->label != 0){
				if(search_symbol(token->label) == -1){
					if(strcmp(token->operator, "EQU") == 0){
						if(print_equal(token, &sym_table[index]))
							index++;

					}else{
						sym_table[index].symbol[0] = 0;
						strcpy(sym_table[index].symbol, token->label);
						sym_table[index++].addr = locctr;
					}
				}
			}

			if(inst_size > 0)
				locctr += inst_size;

			printf("\t");

			if(token->label != NULL)
				printf("%s", token->label);

			printf("\t%s\t", token->operator);

			//EXTDEF, EXTREF ����
			if(def_index[k] < MAX_LINES)
				if(strcmp(token->operator, "EXTDEF") == 0)
					for(j = 0; j < MAX_OPERAND; j++){
						if(token->operand[j] != NULL)
							extdef[def_index[k]++] = token->operand[j];
					}
			if(ref_index[k] < MAX_LINES)
				if(strcmp(token->operator, "EXTREF") == 0){
					for(j = 0; j < MAX_OPERAND; j++){
						if(token->operand[j] != NULL)
							extref[ref_index[k]++] = token->operand[j];
					}
				}

			//���ͷ� �ɺ� ���̺� �߰�
			if(token->operand[0] != NULL)
				if(token->operand[0][0] == '=')
					if(search_symbol(token->operand[0]) == -1){
						sym_table[index].symbol[0] = 0;
						strcpy(sym_table[index].symbol, token->operand[0]);
						sym_table[index++].addr = -1;

					}

			for(j = 0; j < MAX_OPERAND; j++){
				if(token->operand[j] != 0){
					if(j != 0){
						//ù operand�� �ƴϸ�
						printf(",");
					}
					printf("%s", token->operand[j]);
				}else
					break;
			}

			printf("\r\n");

			//LTORG Ȥ�� END�� ������ �� ������ ���� ���ͷ��� �߰���
			if((strcmp(token->operator, "LTORG") && strcmp(token->operator, "END")) == 0)
				print_literal();
		}

		program_length[k] = locctr;
	}

	 print_literal(){
		int i;
		char* hex;

		for(i = 0; i < MAX_LINES; i++)
			if(sym_table[i].symbol[0] == '=' && sym_table[i].addr == -1){
				sym_table[i].addr = locctr;
				hex = get_formated_hex(dectohex(locctr), 4);
				printf("%s", hex);
				printf("\t*\t%s\r\n", sym_table[i].symbol);
				locctr += get_byte_size(sym_table[i].symbol + 1);
			}
	}

	int print_equal(token* token, symbol* symbol){
		char* hex;
		char* op1;
		char* op2;
		char op = 0;
		int i, index_op, index_op1, index_op2, result;
		result = -1;

		if(strcmp(token->operand[0], "*") == 0){
			result = locctr;

			//symbol table�� �߰�
			symbol->symbol[0] = 0;
			strcpy(symbol->symbol, token->label);

			symbol->addr = result;
		}else{
			//������ ã��
			if(strstr(token->operand[0], "+") != NULL){
				op = '+';
			}else if(strstr(token->operand[0], "-") != NULL){
				op = '-';
			}else if(strstr(token->operand[0], "*") != NULL){
				op = '*';
			}else if(strstr(token->operand[0], "/") != NULL){
				op = '/';
			}

			if(op == 0)
				return 0;

			//������ �ε��� ã��
			for(index_op = 0; index_op < strlen(token->operand[0]); index_op++)
				if(token->operand[0][index_op] == op)
					break;

			//�ǿ����� ����
			op1 = strndup(token->operand[0], index_op);
			op2 = strdup(token->operand[0] + index_op + 1);

			//�ǿ����� �ε��� ã��
			index_op1 = search_symbol(op1);
			index_op2 = search_symbol(op2);

			free(op1);
			free(op2);

			if(index_op1 == -1 || index_op2 == -1)
				return 0;

			//���
			switch(op){
				case '+':
					result = sym_table[index_op1].addr + sym_table[index_op2].addr;
					break;
				case '-':
					result = sym_table[index_op1].addr - sym_table[index_op2].addr;
					break;
				case '*':
					result = sym_table[index_op1].addr * sym_table[index_op2].addr;
					break;
				case '/':
					result = sym_table[index_op1].addr / sym_table[index_op2].addr;
					break;
				default:
					return 0;
			}

		}

	//������� ã��������
		if(result != -1){

			//���
			hex = get_formated_hex(dectohex(result), 4);
			printf("%s", hex);

			return 1;
		}

		return 0;
	}

	/*
	 * ���� locctr���� sym_table�� addr�� ���Ͽ�
	 * ���� ���� ���� ���ͷ��� object_code�� �߰��ϴ� �Լ�
	 */
	int add_literal(){
		int i, size;
		char* code = NULL;

		for(i = 0; i < MAX_LINES; i++)
			if(locctr == sym_table[i].addr)
				if(sym_table[i].symbol[0] == '='){
					char* operand = sym_table[i].symbol + 1;
					char* start;
					start = strstr(operand, "'") + 1;

					for(i = 0; i < strlen(start); i++)
						if(start[i] == '\'')
							break;

					switch(operand[0]){
						case 'X':
							code = strndup(start, i);
							size = i / 2 + i % 2;
							break;
						case 'C':
							code = calloc(i * 2, sizeof(char));
							size = i;

							for(i = 0; i < strlen(start); i++)
								if(start[i] == '\'')
									break;
								else
									strcat(code, dectohex(start[i]));
							break;
						default:
							code = dectohex(atoi(operand[0]));
							i = strlen(code);
							size = i / 2 + i % 2;
							break;
					}

					object_code[code_index] = code;
					locctr += size;
					//�� �ٿ��� object code�� �Ľ̵��� �ʾ��� ��
					if(locctr - last_ctr != size){
						object_code[code_index + 1] = object_code[code_index];
						object_code[code_index] = 0;
						code_index++;
					}
					code_index++;
					return strlen(code);
				}

		return 0;
	}

	/*
	 * symbol_table���� ���� �̸��� symbol�� �����ϸ� index ����
	 */
	int search_symbol(char* symbol){
		int i;

		if(symbol == NULL)
			return -1;

		for(i = 0; i < MAX_LINES; i++){
			if(strcmp(sym_table[i].symbol, symbol) == 0)
				return i;
		}

		return -1;
	}

	/*
	 * extref ���� ���� �̸��� �ε��� ��ȯ
	 */
	int search_extref(char* str, int program_index){
		int i, start;

		if(str == NULL)
			return -1;

		if(program_index > 0)
			start = ref_index[program_index - 1];

		for(i = start; i < ref_index[program_index]; i++){
			if(extref[i] != NULL)
				if(strcmp(extref[i], str) == 0)
					return i;
		}

		return -1;
	}

	int get_reg_num(char* operand){
		if(operand == NULL)
			return -1;

		switch(operand[0]){
			case 'A':
				return A;
				break;
			case 'B':
				return reg_B;
				break;
			case 'T':
				return T;
				break;
			case 'S':
				return S;
				break;
			case 'L':
				return L;
				break;
			case 'X':
				return reg_X;
				break;
			default:
				return -1;
				break;
		}
	}

	/*
	 * �Ľ̵� ��ū �ϳ�(instruction)�� size�� ����
	 */
	int get_inst_size(token* token){
		int i, index;
		char* operator = token->operator;
		char* start = 0;

		if(operator == NULL)
			return -1;

		if(operator[0] == '+')
			index = search_opcode(&operator[1]);
		else
			index = search_opcode(operator);

		if(index != -1){
			if(strstr(inst[index][1], "/") != NULL){
				//���Ŀ� / ���ڰ� ���� => 3 �Ǵ� 4
				if(operator[0] == '+')
					return 4;
				else
					return 3;
			}else
				return atoi(inst[index][1]);
		}else{
			if((strcmp(operator, "START") && strcmp(operator, "CSECT")) == 0){
				return 0;

			}else if((strcmp(operator, "END") && strcmp(operator, "LTORG") && strcmp(operator, "EXTDEF") && strcmp(
					operator, "EXTREF")) == 0){
				return -1;

			}else if(strcmp(operator, "RESB") == 0){
				return atoi(token->operand[0]);

			}else if(strcmp(operator, "RESW") == 0){
				return atoi(token->operand[0]) * 3;

			}else if(strcmp(operator, "BYTE") == 0){
				return get_byte_size(token->operand[0]);

			}else if(strcmp(operator, "WORD") == 0){
				return 3;
			}else{
				return -1;
			}
		}

		return -1;
	}

	/*
	 * BYTE operand�� ũ�� ����
	 */
	int get_byte_size(char* operand){
		int i;
		char* start;

		switch(operand[0]){
			case 'X':
				start = strstr(operand, "'") + 1;
				for(i = 0; i < strlen(start); i++)
					if(start[i] == '\'')
						return i / 2 + i % 2;
				break;
			case 'C':
				start = strstr(operand, "'") + 1;
				for(i = 0; i < strlen(start); i++)
					if(start[i] == '\'')
						return i;
				break;
			default:
				i = strlen(dectohex(atoi(operand[0])));
				return i / 2 + i % 2;
				break;
		}
	}

}
