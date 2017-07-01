package main;

import excel.IOExcel;
import gui.GUI;

public class Main{
	public static IOExcel	income;
	public static IOExcel	outcome;
	
	public static void main(String[] args){
		income = new IOExcel();
		income.setFile("income book.xlsx");
		income.readExcelFile();
		outcome = new IOExcel();
		outcome.setFile("outcome book.xlsx");
		outcome.readExcelFile();
		
		new GUI();
	}
}
