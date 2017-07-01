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
					panel.text.setText("��Ͻ���");
					return;
			}
			io.readExcelFile();
			
			Date selectedDate = (Date) panel.datePicker.getModel().getValue();
			if(selectedDate == null)
				throw new Exception("��¥�� ���õ��� ����");
			
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy�� MM�� dd��");
			String strDate = sdf.format(selectedDate);
			
			String strComment = panel.txtComment.getText();
			if(strComment.length() == 0)
				throw new Exception("������ �Էµ��� ����");
			else if(strComment.length() > 20)
				throw new Exception("���� ���� ���� 20�� �ʰ���");
			
			String strMoney = panel.txtMoney.getText();
			if(strMoney.length() == 0)
				throw new Exception("�ݾ��� �Էµ��� ����");
			else if(Constant.isDigit(strMoney) == false)
				throw new Exception("�ݾװ��� ���ڰ� �ƴ� ���� ����");
			
			MoneyRecord record = new MoneyRecord(strDate, strComment, strMoney);
			for(MoneyRecord rc : io.records)
				if(record.equals(rc))
					throw new Exception("�ߺ��� �׸��� ������");
			
			// ArrayList�� �߰� �� �������� ����
			io.records.add(record);
			Collections.sort(io.records, MoneyRecord.comparator);
			
			io.writeExcelFile(panel.type + "come");
		}catch(Exception e1){
			e1.printStackTrace();
			panel.text.setText("��Ͻ���");
			return;
		}finally{
			GUI.isAccessingExcel = false;
		}
		
		panel.text.setText("��ϿϷ�");
	}
}
