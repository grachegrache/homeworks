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
		sdf = new SimpleDateFormat("yyyy년 MM월 dd일");
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
	 * 입력된 레코드의 date가 기간의 마지막 날보다 나중인지 검사하는 함수
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
	 * 레코드에 입력된 date가 기간내에 존재하는지 검사하는 함수
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
				panel.text.append("보고실패\n");
				return;
		}
		io.readExcelFile();
		
		if(io.records.isEmpty()){
			panel.text.append("보고실패\n");
			return;
		}
		
		searchPeriod(io);
		
		// 레코드 리스트로 기간 별 통계를 구함
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
				// 마지막 기간보다 뒤에 기록된 레코드 무시
				break;
			else{
				// 현재 레코드가 기간 내에 있지 않으면
				// 지금까지의 내역을 append
				panel.text.append(strStart + " ~ " + strEnd + "\t" + money + "\n");
				
				// 내역 초기화
				money = 0;
				
				// 현재 검색 중인 기간이 마지막 기간이 아니면 다음 기간으로 설정
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
			
			// 레코드를 모두 검색하지 않았으면
		}while(index < io.records.size());
		
		// append되지 않은 내역 append
		panel.text.append(strStart + " ~ " + strEnd + "\t" + money + "\n");
		money = 0;
		
		// 마지막 기간이 출력되지 않았으면 append
		if(strStart.equals(end.getKey()) == false)
			panel.text.append(end.getKey() + " ~ " + end.getValue() + "\t" + money + "\n");
		
		panel.text.append("보고완료\n");
	}
	
	/*
	 * 가장 옛날에 입력된 내역과 오늘 날짜로 기간(일주일, 월, 연도 별로 첫 날과 마지막 날)을 구함
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
				
				// 현재 날짜로 일주일의 첫 날 구하기
				cal.setTime(new Date());
				int dayOfWeek = cal.get(Calendar.DAY_OF_WEEK);
				cal.add(Calendar.DATE, (dayOfWeek - 1) * -1);
				start = sdf.format(cal.getTime());
				
				// 현재 날짜로 일주일의 마지막 날 구하기
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
				
				// 현재 날짜로 한 달의 첫 날 구하기
				cal.setTime(new Date());
				cal.set(Calendar.DAY_OF_MONTH, cal.getActualMinimum(Calendar.DAY_OF_MONTH));
				start = sdf.format(cal.getTime());
				
				// 현재 날짜로 한 달의 마지막 날 구하기
				cal.setTime(new Date());
				cal.set(Calendar.DAY_OF_MONTH, cal.getActualMaximum(Calendar.DAY_OF_MONTH));
				end = sdf.format(cal.getTime());
				
				period.put(start, end);
				break;
			case "year":
				start = io.records.get(0).year + "년 01월 01일";
				end = io.records.get(0).year + "년 12월 31일";
				startKey = start;
				period.put(start, end);
				
				// 현재 날짜로 해당 연도의 첫 날과 마지막 날 구하기
				cal.setTime(new Date());
				start = cal.get(Calendar.YEAR) + "년 01월 01일";
				end = cal.get(Calendar.YEAR) + "년 12월 31일";
				period.put(start, end);
				break;
			default:
				break;
		}
	}
}
