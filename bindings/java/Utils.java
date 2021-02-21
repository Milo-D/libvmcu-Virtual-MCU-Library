public class Utils{
	
	public static int unsigned(byte b){
		int unsig = (0xf000 | b) & 0x00ff;
		return unsig;
	}
	
	public static int signed(byte b){
		return (int)b;
	}
}
