package task;

public class Contact{
	public String name;
	public String tel;
	public String group;
	
	public Contact(){
		group = "πÃ¡ˆ¡§";
	}
	
	public Contact(String name, String tel, String group){
		this.name = name;
		this.tel = tel;
		if(group.length() != 0)
			this.group = group;
		
	}
	
	public boolean retriveName(String str){
		if(name.contains(str))
			return true;
		else
			return false;
	}
	
	public boolean retrivePhoneNumber(String str){
		String tmp = tel.replaceAll("-", "");
		if(tmp.contains(str))
			return true;
		else
			return false;
	}
	
	public boolean retriveGroup(String str){
		if(group.equals(str))
			return true;
		else
			return false;
	}
	
	@Override
	public String toString(){
		return name + "\t" + tel + "\t" + group;
	}
}
