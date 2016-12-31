package gui;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

import javax.swing.JOptionPane;
/**
 * 송신을 담당하는 클래스입니다.
 * @author Administrator
 *
 */
public class Sender{

	Socket clientSocket; //소켓입니다.
	OutputStream socketOS; //소켓의 OutputStream입니다.
	DataOutputStream socketDOS; //소켓의 DataOutputStream입니다.
	byte[] byteArray; //소켓의 InputStream에서 가져온 바이트 배열이 저장될 변수입니다.
	String message; //소켓의 메시지입니다.
	
	public Sender(Socket socket) throws IOException
	{
		this.clientSocket = socket;
		socketOS = clientSocket.getOutputStream();
		socketDOS = new DataOutputStream(socketOS);
	}
	
	public void sendMessage(String sendMessage)
	{
		try {
			sendMessage += "\n";
			socketDOS.write(sendMessage.getBytes());
			
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "메시지 송신에 실패했습니다.", "메시지 송신 오류", JOptionPane.ERROR_MESSAGE);
		}
	}
	
	public void close()
	{
		try {
			socketDOS.close();
			socketOS.close();
			clientSocket.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
