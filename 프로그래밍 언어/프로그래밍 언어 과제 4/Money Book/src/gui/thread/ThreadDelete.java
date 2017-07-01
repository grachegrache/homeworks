package gui.thread;

import excel.IOExcel;
import excel.MoneyRecord;
import gui.GUI;
import gui.MoneyPanel;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import main.Main;

public class ThreadDelete extends Thread{
	MoneyPanel	panel;
	
	public ThreadDelete(MoneyPanel instance){
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
					panel.text.setText("삭제실패");
					return;
			}
			io.readExcelFile();
			
			Date selectedDate = (Date) panel.datePicker.getModel().getValue();
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy년 MM월 dd일");
			String strDate = sdf.format(selectedDate);
			
			// 지워야 할 레코드 검색
			boolean isRemoved = false;
			ArrayList<MoneyRecord> toRemove = new ArrayList<MoneyRecord>();
			for(MoneyRecord record : io.records)
				if(record.date.equals(strDate)){
					isRemoved = true;
					toRemove.add(record);
				}
			
			if(isRemoved == false)
				throw new Exception("지워진 항목이 없음");
			
			// 지우기
			for(MoneyRecord record : toRemove)
				io.records.remove(record);
			
			io.writeExcelFile(panel.type + "come");
		}catch(Exception e1){
			e1.printStackTrace();
			panel.text.setText("삭제실패");
			return;
		}finally{
			GUI.isAccessingExcel = false;
		}
		
		panel.text.setText("삭제완료");
	}
}
