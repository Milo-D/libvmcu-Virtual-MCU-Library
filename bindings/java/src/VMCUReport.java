class VMCUReport {

	//pointer to the c structure has to be exposed
	//so we can pass VMCUReport to initialize VMCUSystem in java-land
	
	public long c_report;
	//---------------------
	
	//the members of the report struct in c are only 
	//accessed by getters here in java
	//no non-primitive types (other than String) are retured into
	//java-land

	public VMCUReport(String filename){
		c_report = c_report_ctor(filename);
		
		if(c_report == 0){
			System.out.println("c_report == NULL");
			System.exit(1);
		}
	}
	
	public void finalize() {
		c_report_dtor(c_report);
	}
	
	//-------REPORT---------
	
	public int get_progsize(){
		return c_report_get_progsize(c_report);
	}
	
	public int get_nlabels(){
		return c_report_get_nlabels(c_report);
	}
	
	//--------REPORT->DISASSEMBLY----------
	
	public int get_disassembly_plain_opcode(int addr){
		return c_report_get_disassembly_plain_opcode(c_report, addr);
	}
	public int get_disassembly_plain_addr(int addr){
		return addr;
	}
	public boolean get_disassembly_plain_exec(int addr){
		return c_report_get_disassembly_plain_exec(c_report, addr);
	}
	public boolean get_disassembly_plain_dword(int addr){
		return c_report_get_disassembly_plain_dword(c_report, addr);
	}
	public String get_disassembly_plain_mnem(int index){
		return c_report_get_disassembly_plain_mnem(c_report, index);
	}
	public int get_disassembly_plain_key(int addr){
		return c_report_get_disassembly_plain_key(c_report, addr);
	}
	public int get_disassembly_plain_src_value(int addr){
		return c_report_get_disassembly_plain_src_value(c_report, addr);
	}
	public int get_disassembly_plain_src_type(int addr){
		return c_report_get_disassembly_plain_src_type(c_report, addr);
	}
	public int get_disassembly_plain_dest_value(int addr){
		return c_report_get_disassembly_plain_dest_value(c_report, addr);
	}
	public int get_disassembly_plain_dest_type(int addr){
		return c_report_get_disassembly_plain_dest_type(c_report, addr);
	}
	//------------------
	//--------REPORT->LABELS----------------
	public int get_disassembly_label_id(int index){
		return c_report_get_disassembly_label_id(c_report, index);
	}
	public int get_disassembly_label_addr(int index){
		return c_report_get_disassembly_label_addr(c_report, index);
	}
	public int get_disassembly_label_ncallers(int index){
		return c_report_get_disassembly_label_ncallers(c_report, index);
	}
	//-------------------
	//--------REPORT->LABELS->CALLER
	public int get_disassembly_label_caller_addr(int index_label, int index_caller){
		return c_report_get_disassembly_label_caller_addr(c_report, index_label, index_caller);
	}
	//------------------
	static {
		//it expects libvmcu.so
		System.loadLibrary("vmcu_wrapped");
		
		//for it to find the library:
		//java -Djava.library.path="/home/alex/libvmcu-Virtual-MCU-Library/bindings/" Main2

	}
	
	private static native long c_report_ctor(String filename);
	private static native void c_report_dtor(long ptr);
	//-------------------------------------------------------
	
	private static native int c_report_get_progsize(long c_report);
	
	private static native int c_report_get_nlabels(long c_report);
	
	//--------REPORT->DISASSEMBLY----------
	
	private static native int c_report_get_disassembly_plain_opcode(long c_report, int addr);

	private static native boolean c_report_get_disassembly_plain_exec(long c_report, int addr);
	
	private static native boolean c_report_get_disassembly_plain_dword(long c_report, int addr);
	
	private static native String c_report_get_disassembly_plain_mnem(long c_report, int index);
	
	private static native int c_report_get_disassembly_plain_key(long c_report, int addr);
	
	private static native int c_report_get_disassembly_plain_src_value(long c_report, int addr);
	
	private static native int c_report_get_disassembly_plain_src_type(long c_report, int addr);
	
	private static native int c_report_get_disassembly_plain_dest_value(long c_report, int addr);
	
	private static native int c_report_get_disassembly_plain_dest_type(long c_report, int addr);
	
	//------------------
	//--------REPORT->LABELS----------------
	private static native int c_report_get_disassembly_label_id(long c_report, int index);
	
	private static native int c_report_get_disassembly_label_addr(long c_report, int index);
	
	private static native int c_report_get_disassembly_label_ncallers(long c_report, int index);
	
	//-------------------
	//--------REPORT->LABELS->CALLER
	private static native int c_report_get_disassembly_label_caller_addr(long c_report, int index_label, int index_caller);
}
