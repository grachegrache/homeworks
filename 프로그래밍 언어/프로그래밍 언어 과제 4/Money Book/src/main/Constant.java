package main;

public class Constant{
	public static final String	in;
	public static final String	month;
	public static final String	out;
	public static final String	week;
	public static final String	year;
	
	static{
		in = "in";
		out = "out";
		week = "week";
		month = "month";
		year = "year";
	}
	
	public static boolean isDigit(String str){
		for(Character ch : str.toCharArray())
			if(Character.isDigit(ch) == false)
				return false;
		return true;
	}
}
