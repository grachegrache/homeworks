package excel;

import gui.GUI;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;

import org.apache.poi.openxml4j.opc.OPCPackage;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.xssf.streaming.SXSSFSheet;
import org.apache.poi.xssf.streaming.SXSSFWorkbook;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

public class IOExcel{
	private String					file;
	private XSSFWorkbook			inWorkbook;
	private SXSSFWorkbook			outWorkbook;
	public ArrayList<MoneyRecord>	records;
	
	public IOExcel(){
		file = "money book.xlsx";
		records = new ArrayList<MoneyRecord>();
	}
	
	private SXSSFSheet createSheet(SXSSFSheet sheet){
		Row row;
		Cell cell;
		int rowNum = 0;
		
		for(MoneyRecord record : records){
			row = sheet.createRow(rowNum++);
			cell = row.createCell(0);
			cell.setCellValue(record.date);
			cell = row.createCell(1);
			cell.setCellValue(record.comment);
			cell = row.createCell(2);
			cell.setCellValue(record.money);
		}
		return sheet;
	}
	
	/*
	 * 하나의 row를 하나의 money record 객체로 파싱
	 */
	private void parseMoneyRecord(Row row){
		MoneyRecord record = new MoneyRecord();
		record.date = row.getCell(0).getStringCellValue();
		record.comment = row.getCell(1).getStringCellValue();
		record.money = row.getCell(2).getStringCellValue();
		record.parseDate();
		records.add(record);
	}
	
	/*
	 * xlsx 파일을 열고 xlsx 파일로부터 Money Record 리스트로 파싱
	 */
	public void readExcelFile(){
		try{
			OPCPackage opcPackage = OPCPackage.open(new File(file));
			inWorkbook = new XSSFWorkbook(opcPackage);
			opcPackage.close();
			XSSFSheet sheet = inWorkbook.getSheetAt(0);
			
			records.clear();
			for(Row row : sheet)
				parseMoneyRecord(row);
			
			// inWorkbook.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void setFile(String filename){
		file = filename;
	}
	
	/*
	 * 새로운 workbook을 생성하거나 존재하면 덮어쓴다. 현재 MoneyRecord 리스트를 저장. 저장 후 리스트는 clear
	 * 된다.
	 */
	public void writeExcelFile(String sheetName){
		try{
			outWorkbook = new SXSSFWorkbook();
			SXSSFSheet sheet = (SXSSFSheet) outWorkbook.createSheet(sheetName);
			
			FileOutputStream fos = new FileOutputStream(file);
			sheet = createSheet(sheet);
			outWorkbook.write(fos);
			fos.close();
			outWorkbook.close();
			
		}catch(Exception e){
			e.printStackTrace();
		}
		
		records.clear();
		GUI.isAccessingExcel = false;
	}
}
