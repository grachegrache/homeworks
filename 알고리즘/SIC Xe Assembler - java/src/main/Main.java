package main;

public class Main{
	public static void main(String[] args){
		MyAssembler assem = new MyAssembler();
		
		if(assem.initMyAssembler() < 0){
			System.out.println("initMyAssembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.");
			return;
		}
		if(MyAssembler.assemPass1() < 0){
			System.out.println("assemPass1: �н�1 �������� �����Ͽ����ϴ�.");
			return;
		}
		if(MyAssembler.assemPass2() < 0){
			System.out.println("assemPass2: �н�2 �������� �����Ͽ����ϴ�.");
			return;
		}
	}
}
