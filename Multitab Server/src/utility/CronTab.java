package utility;

import java.io.File;
import java.util.ArrayList;

import it.sauronsoftware.cron4j.ProcessTask;
import it.sauronsoftware.cron4j.Scheduler;
import it.sauronsoftware.cron4j.SchedulerListener;
import it.sauronsoftware.cron4j.TaskExecutor;

public class CronTab {
	public String path;
	public ArrayList<Cron> crontab;
	
	public CronTab(String path) {
		this.path = path;
		crontab = new ArrayList<CronTab.Cron>();
		
	}
	
	public void addReserve(String hour, String minute, String relay, String onoff){
		Cron cron = new Cron(hour,minute,relay,onoff);
		ProcessTask task = new ProcessTask(cron.toStringCommand(),new String[]{"SHELL=/bin/bash"},new File(path));
		cron.id = cron.scheduler.schedule(cron.toString(), task);
		cron.scheduler.addSchedulerListener(new onExitDeschedule(cron));
		crontab.add(cron);
		cron.scheduler.start();
	}
	
	public void deleteReserve(int index){
		Cron cron = crontab.get(index);
		if(cron == null)
			return;
		
		cron.scheduler.deschedule(cron.id);
		cron.scheduler.stop();
		crontab.remove(index);
		
	}
	
	public String getPath() {
		return path;
	}

	public void setPath(String path) {
		this.path = path;
	}

	public ArrayList<Cron> getCrontab() {
		return crontab;
	}

	public void setCrontab(ArrayList<Cron> crontab) {
		this.crontab = crontab;
	}

	public class onExitDeschedule implements SchedulerListener{
		public Cron cron;
		
		public onExitDeschedule(Cron cron) {
			this.cron = cron;
		}
		
		@Override
		public void taskFailed(TaskExecutor arg0, Throwable arg1) {
			arg0.getScheduler().deschedule(cron.id);
			arg0.getScheduler().stop();
			crontab.remove(cron);
		}

		@Override
		public void taskLaunching(TaskExecutor arg0) {
			
		}

		@Override
		public void taskSucceeded(TaskExecutor arg0) {
			arg0.getScheduler().deschedule(cron.id);
			crontab.remove(cron);
			arg0.getScheduler().stop();
		}
	}
	
	public class Cron{
		public Scheduler scheduler;
		public String id;
		public String minute;
		public String hour;
		
		public String getMinute() {
			return minute;
		}

		public void setMinute(String minute) {
			this.minute = minute;
		}

		public String getHour() {
			return hour;
		}

		public void setHour(String hour) {
			this.hour = hour;
		}

		public String getDayOfMonth() {
			return dayOfMonth;
		}

		public void setDayOfMonth(String dayOfMonth) {
			this.dayOfMonth = dayOfMonth;
		}

		public String getMonth() {
			return month;
		}

		public void setMonth(String month) {
			this.month = month;
		}

		public String getDayOfWeek() {
			return dayOfWeek;
		}

		public void setDayOfWeek(String dayOfWeek) {
			this.dayOfWeek = dayOfWeek;
		}

//		public String getUserName() {
//			return userName;
//		}
//
//		public void setUserName(String userName) {
//			this.userName = userName;
//		}

		public String getRelay() {
			return (Integer.parseInt(relay)+1)+"";
		}

		public void setRelay(String relay) {
			this.relay = relay;
		}

		public String getOnoff() {
			if(onoff.equals("0"))
				return "OFF";
			else
				return "ON";
		}

		public void setOnoff(String onoff) {
			this.onoff = onoff;
		}

		public String dayOfMonth;
		public String month;
		public String dayOfWeek;
//		public String userName;
		public String relay;
		public String onoff;
		
		public Cron() {
			scheduler = new Scheduler();
			
		}
		
		public Cron(String hour, String minute, String relay, String onoff) {
			this();
			this.hour = hour;
			this.minute = minute;
			this.relay = relay;
			this.onoff = onoff;
			dayOfMonth = "*";
			month = "*";
			dayOfWeek = "*";
//			userName = "root";
			
		}
		
		public String toString(){
			return minute+" "+hour+" "+dayOfMonth+" "+month+" "+dayOfWeek;
		}
		
		public String[] toStringCommand(){
			return new String[]{path+"/set_relay",relay,onoff};
		}
	}
}
