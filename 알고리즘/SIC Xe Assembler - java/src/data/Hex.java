package data;

public class Hex{
	
	public static String dectohex(int n){
		char[] result = new char[7];
		char tmp;
		int index = 0;
		int q, r, i, rindex;
		boolean minus;
		
		if(n == 0)
			return "0";
		
		else if(n < 0){
			minus = true;
			n = -n;
		}else
			minus = false;
		
		do{
			q = n / 16;
			r = n % 16;
			result[index++] = getHex(r);
			n = q;
		}while(q >= 16);
		
		if(q != 0)
			result[index++] = getHex(q);
		
		// reverse
		for(i = 0; i < result.length / 2; i++){
			tmp = result[i];
			rindex = result.length - 1 - i;
			result[i] = result[rindex];
			result[rindex] = tmp;
		}
		
		// 앞 공백 삭제
		String str = "";
		for(i = 0; i < result.length; i++)
			if(result[i] != 0)
				str += String.valueOf(result[i]);
		
		return minus ? getComplementOfHex(result, 6) : str;
	}
	
	private static String getComplementOfHex(char[] hex, int size){
		int i, len, blank;
		boolean carry = false;
		String result, tmp;
		result = "";
		tmp = "";
		len = 0;
		for(i = 0; i < hex.length; i++)
			if(hex[i] != 0)
				len++;
		
		blank = size - len;
		
		// 음수로 변환
		for(i = hex.length - 1; i >= 0; i--)
			if(hex[i] != 0)
				if(i == hex.length - 1){
					// 마지막 인덱스이면
					if(hex[i] == '0')
						carry = true;
					else{
						carry = false;
						hex[i] = getHex(getDec('F') - getDec(hex[i]) + 1);
					}
				}else{
					// 마지막 인덱스가 아니면
					if(carry)
						hex[i]++;
					
					if('F' - hex[i] >= 0){
						hex[i] = getHex(getDec('F') - getDec(hex[i]));
						carry = false;
					}else{
						// 원래 값이 'F'이고 carry값이 1일 때
						carry = true;
						hex[i] = '1';
					}
					
				}
		
		if(carry)
			return "";
		else{
			// 앞 공백 제거
			for(i = 0; i < hex.length; i++)
				if(hex[i] != 0)
					tmp += String.valueOf(hex[i]);
			
			// 앞에 size를 맞추기 위해 F 추가
			if(blank > 0){
				for(i = 0; i < blank; i++)
					result += "F";
				
				result += tmp;
			}else
				result = tmp.substring(-blank);
			
			return result;
		}
	}
	
	public static int getDec(char h){
		
		switch(h){
			case 'A':
				return 10;
			case 'B':
				return 11;
			case 'C':
				return 12;
			case 'D':
				return 13;
			case 'E':
				return 14;
			case 'F':
				return 15;
			default:
				return h - '0';
				
		}
	}
	
	public static String getFormatedHex(String hex, int size){
		int i, blank;
		String result = "";
		blank = size - hex.length();
		
		if(blank > 0){
			for(i = 0; i < blank; i++)
				result += "0";
			result += hex;
		}else
			result = hex.substring(-blank);
		
		return result;
	}
	
	public static char getHex(int r){
		switch(r){
			case 10:
				return 'A';
			case 11:
				return 'B';
			case 12:
				return 'C';
			case 13:
				return 'D';
			case 14:
				return 'E';
			case 15:
				return 'F';
			default:
				return (char) (r + '0');
		}
	}
	
	public static boolean isDigit(String str){
		char[] string = str.toCharArray();
		
		for(int i = 0; i < string.length; i++)
			if(!(string[i] >= '0' && string[i] <= '9'))
				return false;
		
		return true;
	}
}
