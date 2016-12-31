package gui;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;

import javax.swing.JOptionPane;

/**
 * 메시지 수신을 담당하는 클래스입니다. Thread를 상속받습니다.
 * @author Administrator
 *
 */
public class RecieverThread extends Thread{
	Socket clientSocket; //소켓입니다.
	InputStream socketIS; //소켓의 InputStream입니다.
	DataInputStream socketDIS; //소켓의 DataInputStream입니다.
	byte[] byteArray; //소켓의 InputStream에서 가져온 바이트 배열이 저장될 변수입니다.
	String message; //소켓의 메시지입니다.
	
	MessagePasser msgPasser; //소켓이 받은 메시지를 파싱하고, 알맞은 메시지 처리를 하는 클래스입니다.
	
	public RecieverThread(Socket socket, Process process) throws IOException {
		this.clientSocket = socket;
		byteArray = new byte[10000];
		socketIS = clientSocket.getInputStream();
		socketDIS = new DataInputStream(socketIS);
		msgPasser = new MessagePasser(process);
	}

	@Override
	public void run() {
		String[] passedString = null;

		try {
		while (!Thread.currentThread().isInterrupted()) {

				socketDIS.read(byteArray);// 메세지를 수신합니다
				System.out.println(byteArray);
				String msg = new String(byteArray);
				//String msg = socketDIS.readUTF();
				if (msg.isEmpty())
					System.out.println("메시지 온거 맞냐?");
				System.out.println(msg);
				msgPasser.passMessage(msg);

			}			
		} 
		catch (IOException e) {
				JOptionPane.showMessageDialog(null, "메시지 수신에 오류가 발생했습니다.", "메시지 수신 오류", JOptionPane.ERROR_MESSAGE);
		} // while문 끝
		finally
		{
			try {
				socketDIS.close();
				socketIS.close();
				clientSocket.close();
				return;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
}
