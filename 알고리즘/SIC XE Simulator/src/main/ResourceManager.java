package main;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import data.Constant;
import data.InstList;
import data.Program;
import data.Register;

public class ResourceManager implements IResourceManager{
	private Register A;
	private Register B;
	private Register F;
	public InstList il;
	private Register L;
	private Register PC;
	private Register S;
	private Register SW;
	private Register T;
	private Register X;
	public byte[] data;
	private int index;
	public FileOutputStream fos;
	public FileInputStream fis;
	public String dev;
	public int TA;
	private boolean isFirst;
	
	public ResourceManager(){
		fis = null;
		fos = null;
		index = 0;
		isFirst = true;
	}
	
	@Override
	public void affectVisualSimulator(){
		// H
		Program prog = Main.loader.pl.search(PC.value);
		if(prog != null){
			Main.gui.txtProgName.setText(prog.name);
			Main.gui.txtStartAddr.setText("000000");
			Main.gui.txtLength.setText(String.format("%06X", prog.length));
		}
		
		// E
		Main.gui.txt1stInst.setText(String.format("%06X", Main.loader.addr1stInst));
		
		// Register
		Main.gui.txtDecA.setText(String.valueOf(A.value));
		Main.gui.txtDecX.setText(String.valueOf(X.value));
		Main.gui.txtDecL.setText(String.valueOf(L.value));
		Main.gui.txtDecPC.setText(String.valueOf(PC.value));
		Main.gui.txtDecB.setText(String.valueOf(B.value));
		Main.gui.txtDecS.setText(String.valueOf(S.value));
		Main.gui.txtDecT.setText(String.valueOf(T.value));
		Main.gui.txtHexA.setText(String.format("%06X", A.value));
		Main.gui.txtHexX.setText(String.format("%06X", X.value));
		Main.gui.txtHexL.setText(String.format("%06X", L.value));
		Main.gui.txtHexPC.setText(String.format("%06X", PC.value));
		Main.gui.txtHexB.setText(String.format("%06X", B.value));
		Main.gui.txtHexS.setText(String.format("%06X", S.value));
		Main.gui.txtHexT.setText(String.format("%06X", T.value));
		Main.gui.txtSW.setText(String.valueOf(SW.value));
		Main.gui.txtF.setText(String.valueOf(F.value));
		
		Main.gui.txtTarget.setText(String.format("%06X", TA));
		Main.gui.txtDev.setText(dev);
		
	}
	
	@Override
	public byte[] getMemory(int locate, int size){
		byte[] tmp = new byte[size];
		
		for(int i = 0; i < size; i++)
			tmp[i] = il.memory[locate + i];
		
		return tmp;
	}
	
	@Override
	public int getRegister(int regNum){
		switch (regNum){
			case 0:
				return A.value;
			case 1:
				return X.value;
			case 2:
				return L.value;
			case 3:
				return B.value;
			case 4:
				return S.value;
			case 5:
				return T.value;
			case 6:
				return F.value;
			case 8:
				return PC.value;
			case 9:
				return SW.value;
			default:
				return -1;
		}
	}
	
	@Override
	public void initialDevice(String devName){
		dev = devName;
		
		// try{
		// File file = new File(devName);
		//
		// fos = new FileOutputStream(file);
		//
		// if(file.exists() == false){
		// file.createNewFile();
		// }else{
		//
		// }
		//
		// }catch(IOException e){
		// e.printStackTrace();
		// }finally{
		// if(fis != null)
		// try{
		// fis.close();
		// index = 0;
		// fis = null;
		// }catch(IOException e){
		// e.printStackTrace();
		// }
		// }
	}
	
	@Override
	public void initializeMemory(){
		il = new InstList(Constant.memorySize);
	}
	
	@Override
	public void initializeRegister(){
		A = new Register(0);
		X = new Register(1);
		L = new Register(2);
		PC = new Register(8);
		SW = new Register(9);
		B = new Register(3);
		S = new Register(4);
		T = new Register(5);
		F = new Register(6);
	}
	
	@Override
	public byte readDevice(){
		if(index == 0)
			if(fis == null){
				File file = new File(dev);
				try{
					fis = new FileInputStream(file);
					data = new byte[(int) file.length()];
					fis.read(data);
				}catch(Exception e){
					e.printStackTrace();
					
				}finally{
					try{
						if(fis != null){
							fis.close();
							fis = null;
						}
					}catch(IOException e){
						e.printStackTrace();
					}
				}
				
			}
		
		if(index >= data.length)
			return 0;
		else
			return data[index++];
		
	}
	
	@Override
	public void setMemory(int locate, byte[] data, int size){
		for(int i = 0; i < size; i++)
			il.memory[locate + i] = data[i];
		
	}
	
	@Override
	public void setRegister(int regNum, int value){
		switch (regNum){
			case 0:
				A.value = value;
				break;
			case 1:
				X.value = value;
				break;
			case 2:
				L.value = value;
				break;
			case 3:
				B.value = value;
				break;
			case 4:
				S.value = value;
				break;
			case 5:
				T.value = value;
				break;
			case 6:
				F.value = value;
				break;
			case 8:
				PC.value = value;
				break;
			case 9:
				SW.value = value;
				break;
			default:
				break;
		}
	}
	
	@Override
	public void writeDevice(byte[] data){
		if(fos == null){
			try{
				File file = new File(dev);
				if(file.exists() == false)
					file.createNewFile();
				
				if(isFirst){
					fos = new FileOutputStream(file);
					isFirst = false;
				}else
					fos = new FileOutputStream(file,true);
				
			}catch(IOException e){
				e.printStackTrace();
			}
			
		}
		try{
			fos.write(data);
			
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
}
