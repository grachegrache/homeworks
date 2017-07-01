package gui.thread;

import excel.IOExcel;
import excel.MoneyRecord;
import gui.ReportPanel;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Map.Entry;

import main.Main;

public class ThreadReport extends Thread{
	private Calendar				cal;
	private ReportPanel				panel;
	private String					per;
	private HashMap<String, String>	period;
	private SimpleDateFormat		sdf;
	private String					startKey;
	private String					type;
	
	public ThreadReport(ReportPanel instance, String per, String input){
		panel = instance;
		this.per = per;
		type = input;
		period = new HashMap<String, String>();
		sdf = new SimpleDateFormat("yyyy�� MM�� dd��");
		cal = Calendar.getInstance();
		startKey = null;
	}
	
	private String getFirstDateOfMonth(MoneyRecord record){
		try{
			cal.setTime(sdf.parse(record.date));
		}catch(ParseException e){
			e.printStackTrace();
			return null;
		}
		
		cal.set(Calendar.DAY_OF_MONTH, cal.getActualMinimum(Calendar.DAY_OF_MONTH));
		return sdf.format(cal.getTime());
	}
	
	private String getFirstDateOfWeek(MoneyRecord record){
		try{
			cal.setTime(sdf.parse(record.date));
		}catch(ParseException e){
			e.printStackTrace();
			return null;
		}
		
		int dayOfWeek = cal.get(Calendar.DAY_OF_WEEK);
		cal.add(Calendar.DATE, (dayOfWeek - 1) * -1);
		
		return sdf.format(cal.getTime());
	}
	
	private String getLastDateOfMonth(MoneyRecord record){
		try{
			cal.setTime(sdf.parse(record.date));
		}catch(ParseException e){
			e.printStackTrace();
			return null;
		}
		
		cal.set(Calendar.DAY_OF_MONTH, cal.getActualMaximum(Calendar.DAY_OF_MONTH));
		return sdf.format(cal.getTime());
	}
	
	private String getLastDateOfWeek(MoneyRecord record){
		try{
			cal.setTime(sdf.parse(record.date));
		}catch(ParseException e){
			e.printStackTrace();
			return null;
		}
		
		int dayOfWeek = cal.get(Calendar.DAY_OF_WEEK);
		cal.add(Calendar.DATE, 7 - dayOfWeek);
		
		return sdf.format(cal.getTime());
	}
	
	private String getNextDateOfWeek(String date){
		try{
			cal.setTime(sdf.parse(date));
		}catch(ParseException e){
			e.printStackTrace();
		}
		
		cal.setTimeInMillis(cal.getTimeInMillis() + 1000 * 60 * 60 * 24 * 7);
		return sdf.format(cal.getTime());
	}
	
	private String getNextFirstDateOfMonth(String firstDate){
		try{
			cal.setTime(sdf.parse(firstDate));
		}catch(ParseException e){
			e.printStackTrace();
		}
		
		cal.set(Calendar.MONTH, cal.get(Calendar.MONTH) + 1);
		cal.set(Calendar.DAY_OF_MONTH, cal.getActualMinimum(Calendar.DAY_OF_MONTH));
		return sdf.format(cal.getTime());
	}
	
	private String getNextFirstDateOfYear(String firstDate){
		try{
			cal.setTime(sdf.parse(firstDate));
		}catch(ParseException e){
			e.printStackTrace();
		}
		
		cal.set(Calendar.YEAR, cal.get(Calendar.YEAR) + 1);
		return sdf.format(cal.getTime());
	}
	
	private String getNextLastDateOfMonth(String lastDate){
		try{
			cal.setTime(sdf.parse(lastDate));
		}catch(ParseException e){
			e.printStackTrace();
		}
		
		cal.set(Calendar.MONTH, cal.get(Calendar.MONTH) + 1);
		cal.set(Calendar.DAY_OF_MONTH, cal.getActualMaximum(Calendar.DAY_OF_MONTH));
		return sdf.format(cal.getTime());
	}
	
	private String getNextLastDateOfYear(String lastDate){
		try{
			cal.setTime(sdf.parse(lastDate));
		}catch(ParseException e){
			e.printStackTrace();
		}
		
		cal.set(Calendar.YEAR, cal.get(Calendar.YEAR) + 1);
		return sdf.format(cal.getTime());
	}
	
	/*
	 * �Էµ� ���ڵ��� date�� �Ⱓ�� ������ ������ �������� �˻��ϴ� �Լ�
	 */
	private boolean isAfter(String periodEnd, MoneyRecord record){
		Calendar end = Calendar.getInstance();
		
		try{
			end.setTime(sdf.parse(periodEnd));
			cal.setTime(sdf.parse(record.date));
		}catch(ParseException e){
			e.printStackTrace();
			return false;
		}
		
		if(cal.after(end))
			return true;
		else
			return false;
	}
	
	/*
	 * ���ڵ忡 �Էµ� date�� �Ⱓ���� �����ϴ��� �˻��ϴ� �Լ�
	 */
	private boolean isInPeriod(String periodStart, String periodEnd, MoneyRecord record){
		Calendar start = Calendar.getInstance();
		Calendar end = Calendar.getInstance();
		
		try{
			start.setTime(sdf.parse(periodStart));
			end.setTime(sdf.parse(periodEnd));
			cal.setTime(sdf.parse(record.date));
		}catch(ParseException e){
			e.printStackTrace();
			return false;
		}
		
		if(cal.compareTo(start) >= 0 && cal.compareTo(end) <= 0)
			return true;
		else
			return false;
	}
	
	@Override
	public void run(){
		IOExcel io = null;
		switch(type){
			case "in":
				io = Main.income;
				break;
			case "out":
				io = Main.outcome;
				break;
			default:
				panel.text.append("�������\n");
				return;
		}
		io.readExcelFile();
		
		if(io.records.isEmpty()){
			panel.text.append("�������\n");
			return;
		}
		
		searchPeriod(io);
		
		// ���ڵ� ����Ʈ�� �Ⱓ �� ��踦 ����
		int index = 0;
		int money = 0;
		Entry<String, String> end = null;
		
		if(period.size() > 1){
			end = (Entry<String, String>) period.entrySet().toArray()[1];
			if(end.getKey().equals(startKey))
				end = (Entry<String, String>) period.entrySet().toArray()[0];
		}else
			end = (Entry<String, String>) period.entrySet().toArray()[0];
		
		String strStart = startKey;
		String strEnd = period.get(startKey);
		
		do{
			MoneyRecord record = io.records.get(index);
			
			if(isInPeriod(strStart, strEnd, record)){
				money += Integer.valueOf(record.money);
				index++;
			}else if(isAfter(end.getValue(), record))
				// ������ �Ⱓ���� �ڿ� ��ϵ� ���ڵ� ����
				break;
			else{
				// ���� ���ڵ尡 �Ⱓ ���� ���� ������
				// ���ݱ����� ������ append
				panel.text.append(strStart + " ~ " + strEnd + "\t" + money + "\n");
				
				// ���� �ʱ�ȭ
				money = 0;
				
				// ���� �˻� ���� �Ⱓ�� ������ �Ⱓ�� �ƴϸ� ���� �Ⱓ���� ����
				if(strStart.equals(end.getKey()) == false)
					switch(per){
						case "week":
							strStart = getNextDateOfWeek(strStart);
							strEnd = getNextDateOfWeek(strEnd);
							break;
						case "month":
							strStart = getNextFirstDateOfMonth(strStart);
							strEnd = getNextLastDateOfMonth(strEnd);
							break;
						case "year":
							strStart = getNextFirstDateOfYear(strStart);
							strEnd = getNextLastDateOfYear(strEnd);
							break;
						default:
							return;
					}
				
			}
			
			// ���ڵ带 ��� �˻����� �ʾ�����
		}while(index < io.records.size());
		
		// append���� ���� ���� append
		panel.text.append(strStart + " ~ " + strEnd + "\t" + money + "\n");
		money = 0;
		
		// ������ �Ⱓ�� ��µ��� �ʾ����� append
		if(strStart.equals(end.getKey()) == false)
			panel.text.append(end.getKey() + " ~ " + end.getValue() + "\t" + money + "\n");
		
		panel.text.append("����Ϸ�\n");
	}
	
	/*
	 * ���� ������ �Էµ� ������ ���� ��¥�� �Ⱓ(������, ��, ���� ���� ù ���� ������ ��)�� ����
	 */
	private void searchPeriod(IOExcel io){
		String start, end;
		start = null;
		end = null;
		
		switch(per){
			case "week":
				if((start = getFirstDateOfWeek(io.records.get(0))) != null)
					if((end = getLastDateOfWeek(io.records.get(0))) != null){
						startKey = start;
						period.put(start, end);
					}
				
				// ���� ��¥�� �������� ù �� ���ϱ�
				cal.setTime(new Date());
				int dayOfWeek = cal.get(Calendar.DAY_OF_WEEK);
				cal.add(Calendar.DATE, (dayOfWeek - 1) * -1);
				start = sdf.format(cal.getTime());
				
				// ���� ��¥�� �������� ������ �� ���ϱ�
				cal.setTime(new Date());
				dayOfWeek = cal.get(Calendar.DAY_OF_WEEK);
				cal.add(Calendar.DATE, 7 - dayOfWeek);
				end = sdf.format(cal.getTime());
				
				period.put(start, end);
				
				break;
			case "month":
				if((start = getFirstDateOfMonth(io.records.get(0))) != null)
					if((end = getLastDateOfMonth(io.records.get(0))) != null){
						startKey = start;
						period.put(start, end);
					}
				
				// ���� ��¥�� �� ���� ù �� ���ϱ�
				cal.setTime(new Date());
				cal.set(Calendar.DAY_OF_MONTH, cal.getActualMinimum(Calendar.DAY_OF_MONTH));
				start = sdf.format(cal.getTime());
				
				// ���� ��¥�� �� ���� ������ �� ���ϱ�
				cal.setTime(new Date());
				cal.set(Calendar.DAY_OF_MONTH, cal.getActualMaximum(Calendar.DAY_OF_MONTH));
				end = sdf.format(cal.getTime());
				
				period.put(start, end);
				break;
			case "year":
				start = io.records.get(0).year + "�� 01�� 01��";
				end = io.records.get(0).year + "�� 12�� 31��";
				startKey = start;
				period.put(start, end);
				
				// ���� ��¥�� �ش� ������ ù ���� ������ �� ���ϱ�
				cal.setTime(new Date());
				start = cal.get(Calendar.YEAR) + "�� 01�� 01��";
				end = cal.get(Calendar.YEAR) + "�� 12�� 31��";
				period.put(start, end);
				break;
			default:
				break;
		}
	}
}
