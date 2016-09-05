package gui;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Vector;

import javax.swing.JOptionPane;

/**
 * 메시지 전송 로그를 저장하는 클래스입니다. 모든 로그들을 정리하고, 이후 출력을 원하면
 * 파일 이름을 지정받고 해당 파일에 출력시켜줍니다.
 * 이러한 일련의 작업은 Log에서 이루어집니다.
 * @author Administrator
 *
 */

public class Log {
	//로그로 저장할 메시지 이름 리스트입니다.
	Vector<String> messageNameList;
	Vector<String> logContextList;
	
	//저장할 파일의 경로입니다.
	String FileName;
	FileWriter writer;
	
	public Log(String FileName)
	{
		this.FileName = FileName;
		try {
			writer = new FileWriter(FileName);
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "로그 파일 생성에 실패했습니다.","로그 파일 오류",JOptionPane.ERROR_MESSAGE);
		}
	}
	
	public synchronized void addLogAboutRecieveMessage(String message)
	{
		try {
			
			writer.write("받은 메시지:"+message + "\n");
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "로그파일 로그입력에 실패하였습니다.", "로그 파일 오류", JOptionPane.ERROR_MESSAGE);
		}
	}
	
	
	public synchronized void addLogAboutSendMessage(String message)
	{
		try {
			writer.write("보낸 메시지:"+message + "\n");
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "로그파일 로그입력에 실패하였습니다.", "로그 파일 오류", JOptionPane.ERROR_MESSAGE);	
		}
	}
	
	public synchronized void addGamePlayMessage(String message)
	{
		try{
			writer.write("게임 진행:"+message+ "\n");
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "로그파일 로그입력에 실패하였습니다.","로그 파일 오류",JOptionPane.ERROR_MESSAGE);
		}
	}
	
}
