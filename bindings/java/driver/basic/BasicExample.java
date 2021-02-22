import java.util.Arrays;


class BasicExample {

	public static void main(String args[]){
	
		System.out.println("creating VMCU System");

		String filename = "../../../../examples/m328p/algo/kmp.hex";
		
		//create report
		VMCUReport report = new VMCUReport(filename);
		
		//initialize system with report
		VMCUSystem sys = new VMCUSystem(report);		
		
		int addr = 0;
		report.get_disassembly_plain_opcode(addr);
		report.get_disassembly_plain_key(addr);
		
		int progsize = report.get_progsize();
		System.out.println("progsize="+progsize);
		
		while(sys.get_pc() != 0x141){
			sys.step();
		}
		
		System.out.println("pc="+sys.get_pc());
		
		//read from sram
		int data = sys.read_data(0x20+0x20);
		System.out.println("result: "+data);
		
		byte[] gprs = sys.dump_gpr();
		
		//print gprs
		for(int i=0;i < 32; i++){
			int gpr = Utils.unsigned(gprs[i]);
			System.out.printf("0x%02x,", gpr);
		}
		System.out.println();
		
	}
}
