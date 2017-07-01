package excel;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Comparator;

public class MoneyRecord{
	public static Comparator<MoneyRecord>	comparator;
	public String							comment;
	public String							date;
	public String							money;
	public String							year;
	
	public MoneyRecord(){
		date = "";
		year = "";
		comment = "";
		money = "";
		
	}
	
	public MoneyRecord(String strDate, String strComment, String strMoney){
		date = strDate;
		comment = strComment;
		money = strMoney;
		parseDate();
	}
	
	static{
		comparator = new Comparator<MoneyRecord>(){
			@Override
			public int compare(MoneyRecord o1, MoneyRecord o2){
				return o1.date.compareTo(o2.date);
			}
		};
	}
	
	@Override
	public boolean equals(Object obj){
		if(obj instanceof MoneyRecord){
			boolean isEqual;
			MoneyRecord mr = (MoneyRecord) obj;
			isEqual = mr.date.equals(date);
			isEqual &= mr.comment.equals(comment);
			isEqual &= mr.money.equals(money);
			
			return isEqual;
		}
		
		return false;
	}
	
	public void parseDate(){
		try{
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy년 MM월 dd일");
			Calendar cal = Calendar.getInstance();
			cal.setTime(sdf.parse(date));
			year = String.valueOf(cal.get(Calendar.YEAR));
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	@Override
	public String toString(){
		return comment + "\t" + money + "원";
	}
}
