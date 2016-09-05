package task;

import main.Main;

public class ThreadContactDelete extends Thread{
	Contact contact;
	
	public ThreadContactDelete(Contact contact){
		this.contact = contact;
	}
	
	@Override
	public void run(){
		try{
			Main.contactList.remove(contact);
			Main.saveContact();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
}
