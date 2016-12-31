package data;

import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.StringTokenizer;

public class OpTable{
	public ArrayList<Op>	opTable;
	
	public OpTable(){
		opTable = new ArrayList<Op>();
		
		Scanner sc = null;
		StringTokenizer st;
		Op op;
		
		try{
			sc = new Scanner(new File("inst.data"));
			
			// 한 줄씩 입력받아 파싱
			while(sc.hasNextLine()){
				st = new StringTokenizer(sc.nextLine(), "|");
				
				op = new Op();
				op.name = st.nextToken();
				op.format = st.nextToken();
				op.opcode = (byte) Integer.parseInt(st.nextToken(), 16);
				op.operandSize = Integer.valueOf(st.nextToken());
				opTable.add(op);
				
			}
		}catch(Exception e){
			e.printStackTrace();
		}finally{
			if(sc != null)
				sc.close();
		}
		
	}
	
	public Op search(byte opcode){
		for(Op op : opTable)
			if(op.opcode == opcode)
				return op;
		
		return null;
	}
}
