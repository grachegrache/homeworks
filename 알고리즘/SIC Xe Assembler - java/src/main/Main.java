package main;

public class Main{
	public static void main(String[] args){
		MyAssembler assem = new MyAssembler();
		
		if(assem.initMyAssembler() < 0){
			System.out.println("initMyAssembler: 프로그램 초기화에 실패 했습니다.");
			return;
		}
		if(MyAssembler.assemPass1() < 0){
			System.out.println("assemPass1: 패스1 과정에서 실패하였습니다.");
			return;
		}
		if(MyAssembler.assemPass2() < 0){
			System.out.println("assemPass2: 패스2 과정에서 실패하였습니다.");
			return;
		}
	}
}
