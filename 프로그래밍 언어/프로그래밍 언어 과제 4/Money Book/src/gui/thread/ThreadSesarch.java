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
					panel.text.append("�˻�����");
					return;
					
			}
			io.readExcelFile();
			
			Date selectedDate = (Date) panel.datePicker.getModel().getValue();
			if(selectedDate == null)
				throw new Exception("��¥�� ���õ��� ����");
			
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy�� MM�� dd��");
			String strDate = sdf.format(selectedDate);
			
			// ���ڵ� �˻�
			StringBuffer sb = new StringBuffer();
			for(MoneyRecord record : io.records)
				if(record.date.equals(strDate))
					sb.append(record.toString() + "\n");
			
			// �˻� ���
			panel.text.setText(strDate + "\n");
			StringTokenizer st = new StringTokenizer(sb.toString(), "\n");
			while(st.hasMoreTokens()){
				sleep(1000);
				panel.text.append(st.nextToken() + "\n");
			}
			
		}catch(Exception e1){
			e1.printStackTrace();
			panel.text.append("�˻�����");
			return;
		}finally{
			GUI.isAccessingExcel = false;
		}
		panel.text.append("�˻��Ϸ�");
	}
}
