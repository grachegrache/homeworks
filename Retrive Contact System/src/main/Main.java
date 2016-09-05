package main;

import gui.GUI;

import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.StringTokenizer;

import task.Contact;

public class Main{
	private static String file = "data.txt";
	public static ArrayList<String> groupList;
	public static ArrayList<Contact> contactList;
	
	public static void main(String[] args){
		groupList = new ArrayList<String>();
		contactList = new ArrayList<Contact>();
		if(parseContact())
			new GUI();
	}
	
	synchronized public static boolean parseContact(){
		try{
			Scanner scFile = new Scanner(new File(file));
			StringTokenizer st;
			scFile.nextLine();
			
			while(scFile.hasNextLine()){
				Contact c = new Contact();
				st = new StringTokenizer(scFile.nextLine(), "\t");
				c.name = st.nextToken();
				c.tel = st.nextToken();
				c.group = st.nextToken();
				contactList.add(c);
				
				if(groupList.contains(c.group) == false)
					groupList.add(c.group);
			}
			
			scFile.close();
		}catch(Exception e){
			e.printStackTrace();
			return false;
		}
		
		return true;
	}
	
	synchronized public static boolean saveContact(){
		try{
			FileWriter fw = new FileWriter(file);
			fw.write("이름\t전화번호\t그룹\n");
			for(Contact c : contactList)
				fw.append(c.toString()+"\n");
			fw.close();
		}catch(Exception e){
			e.printStackTrace();
			return false;
		}
		return true;
	}
}
