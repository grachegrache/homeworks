package data;

import java.util.ArrayList;

import main.Constant;

public class Token{
	public String				comment;
	public String				label;
	public ArrayList<String>	operand;
	public String				operator;
	
	public Token(){
		operand = new ArrayList<String>(Constant.MAX_OPERAND);
	}
	
}
