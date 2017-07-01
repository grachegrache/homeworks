package gui.thread;

import excel.IOExcel;
import excel.MoneyRecord;
import gui.GUI;
import gui.MoneyPanel;

import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Date;

import main.Constant;
import main.Main;

public class ThreadRegister extends Thread{
	private MoneyPanel	panel;
	
	public ThreadRegister(MoneyPanel instance){
		panel = instance;
	}
	
	@Override
	public void run(){
		try{
			IOExcel io = null;
			switch(panel.type){
				case "in":
					io = Main.income;
					break;
				case "out":
					io = Main.outcome;
					break;
				default:
					panel.text.setText("등록실패");
					return;
			}
			io.readExcelFile();
			
			Date selectedDate = (Date) panel.datePicker.getModel().getValue();
			if(selectedDate == null)
				throw new Exception("날짜가 선택되지 않음");
			
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy년 MM월 dd일");
			String strDate = sdf.format(selectedDate);
			
			String strComment = panel.txtComment.getText();
			if(strComment.length() == 0)
				throw new Exception("내역이 입력되지 않음");
			else if(strComment.length() > 20)
				throw new Exception("내역 글자 수가 20을 초과함");
			
			String strMoney = panel.txtMoney.getText();
			if(strMoney.length() == 0)
				throw new Exception("금액이 입력되지 않음");
			else if(Constant.isDigit(strMoney) == false)
				throw new Exception("금액값에 숫자가 아닌 값이 있음");
			
			MoneyRecord record = new MoneyRecord(strDate, strComment, strMoney);
			for(MoneyRecord rc : io.records)
				if(record.equals(rc))
					throw new Exception("중복된 항목이 존재함");
			
			// ArrayList에 추가 및 오름차순 정렬
			io.records.add(record);
			Collections.sort(io.records, MoneyRecord.comparator);
			
			io.writeExcelFile(panel.type + "come");
		}catch(Exception e1){
			e1.printStackTrace();
			panel.text.setText("등록실패");
			return;
		}finally{
			GUI.isAccessingExcel = false;
		}
		
		panel.text.setText("등록완료");
	}
}
