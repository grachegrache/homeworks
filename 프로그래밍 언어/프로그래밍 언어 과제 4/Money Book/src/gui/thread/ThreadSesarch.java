package gui.thread;

import excel.IOExcel;
import excel.MoneyRecord;
import gui.GUI;
import gui.MoneyPanel;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.StringTokenizer;

import main.Main;

public class ThreadSesarch extends Thread{
	MoneyPanel	panel;
	
	public ThreadSesarch(MoneyPanel instance){
		panel = instance;
	}
	
	@Override
	public void run(){
		IOExcel io = null;
		try{
			switch(panel.type){
				case "in":
					io = Main.income;
					break;
				case "out":
					io = Main.outcome;
					break;
				default:
					panel.text.append("검색실패");
					return;
					
			}
			io.readExcelFile();
			
			Date selectedDate = (Date) panel.datePicker.getModel().getValue();
			if(selectedDate == null)
				throw new Exception("날짜가 선택되지 않음");
			
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy년 MM월 dd일");
			String strDate = sdf.format(selectedDate);
			
			// 레코드 검색
			StringBuffer sb = new StringBuffer();
			for(MoneyRecord record : io.records)
				if(record.date.equals(strDate))
					sb.append(record.toString() + "\n");
			
			// 검색 출력
			panel.text.setText(strDate + "\n");
			StringTokenizer st = new StringTokenizer(sb.toString(), "\n");
			while(st.hasMoreTokens()){
				sleep(1000);
				panel.text.append(st.nextToken() + "\n");
			}
			
		}catch(Exception e1){
			e1.printStackTrace();
			panel.text.append("검색실패");
			return;
		}finally{
			GUI.isAccessingExcel = false;
		}
		panel.text.append("검색완료");
	}
}
