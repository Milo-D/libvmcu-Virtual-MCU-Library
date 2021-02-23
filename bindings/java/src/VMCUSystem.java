public class VMCUSystem {

	//vmcu_system_t*
	private long c_system = 0;
	
	

	public VMCUSystem(VMCUReport report){
		//old:
		//uses analyzer to generate report
		//it takes a filename argument to pass it to
		//the analyzer 
		//c_system = c_system_ctor(filename);
		
		
		c_system = c_system_ctor(report.c_report);
		if(c_system == 0){
			System.out.println("c_system == NULL");
			System.exit(1);
		}
	}
	
	public void finalize() {
	
		c_system_dtor(c_system);
	}
	
	//------------------
	
	public int step(){ return c_system_step(c_system); }
	
	public void backstep(){ c_system_backstep(c_system); }
	
	public int get_steps(){ return c_system_get_steps(c_system); }
	
	public void reboot(){ c_system_reboot(c_system); }
	
	public void write_gpr(int rx, int data){ 
		c_system_write_gpr(c_system, rx, data); 	
	}
	
	public byte read_gpr(int rx){ return c_system_read_gpr(c_system, rx); }
	
	public byte[] dump_gpr(){ 
		return c_system_dump_gpr(c_system); 
	}
	
	public void write_sreg(int flag, boolean bit){
		c_system_write_sreg(c_system, flag, bit);
	}
	
	public boolean read_sreg(int flag){
		return c_system_read_sreg(c_system, flag);
	}
	
	int dump_sreg(){
		return c_system_dump_sreg(c_system);
	}
	
	//--------------------------------
	//used flat-bind to create this binding :)
	//this is wrapped by creating getters to access it's members
	int read_progmem_opcode(int addr){
		return c_system_read_progmem_opcode(addr);
	}
	int read_progmem_addr(int addr){
		return addr;
	}
	int read_progmem_key(int addr){
		return c_system_read_progmem_key(addr);
	}
	boolean read_progmem_exec(int addr){
		return c_system_read_progmem_exec(addr);
	}
	boolean read_progmem_dword(int addr){
		return c_system_read_progmem_dword(addr);
	}
	//--------------------------------
	int read_flash(int address){
		return c_system_read_flash(c_system, address);
	}
	
	void move_pc(int inc){
		c_system_move_pc(c_system, inc);
	}
	
	void set_pc(int addr){
		c_system_set_pc(c_system, addr);
	}
	
	int get_pc(){
		return c_system_get_pc(c_system);
	}
	
	void push_stack(int value){
		c_system_push_stack(c_system, value);
	}
	
	int pop_stack(){
		return c_system_pop_stack(c_system);
	}
	
	void write_data(int addr, int value){
		c_system_write_data(c_system, addr, value);
	}
	
	byte read_data(int addr){
		return c_system_read_data(c_system, addr);
	}
	
	byte[] dump_data(){
		return c_system_dump_data(c_system);
	}
	
	void set_sfr(int addr, int bit){
		c_system_set_sfr(c_system, addr, bit);
	}
	
	void clear_sfr(int addr, int bit){
		c_system_clear_sfr(c_system, addr, bit);
	}
	
	byte[] dump_eeprom(){
		return c_system_dump_eeprom(c_system);
	}
	
	//------------------
	
	static {
		//it expects libvmcu.so
		System.loadLibrary("vmcu_wrapped");
		
		//for it to find the library:
		//java -Djava.library.path="/home/alex/libvmcu-Virtual-MCU-Library/bindings/" Main2

	}
	
	private static native long c_system_ctor(long ptr);
	private static native void c_system_dtor(long ptr);
	//-------------------------------------------------------
	private static native int c_system_step(long ptr);
	private static native void c_system_backstep(long ptr);
	private static native int c_system_get_steps(long ptr);
	
	private static native void c_system_reboot(long ptr);
	private static native void c_system_write_gpr(long ptr, int rx, int data);
	private static native byte c_system_read_gpr(long ptr, int rx);
	private static native byte[] c_system_dump_gpr(long ptr);
	
	private static native void c_system_write_sreg(long ptr, int flag, boolean bit);
	
	private static native boolean c_system_read_sreg(long ptr, int flag);
	
	private static native int c_system_dump_sreg(long ptr);
	
	//-------------------
	//private static native Progmem c_system_read_progmem(long ptr, int addr);
	private static native int c_system_read_progmem_opcode(int addr);
	private static native int c_system_read_progmem_key(int addr);
	private static native boolean c_system_read_progmem_exec(int addr);
	private static native boolean c_system_read_progmem_dword(int addr);
	//-------------------
	
	private static native int c_system_read_flash(long ptr, int addr);
	
	private static native void c_system_move_pc(long ptr, int inc);
	private static native void c_system_set_pc(long ptr, int addr);
	private static native int c_system_get_pc(long ptr);
	
	private static native void c_system_push_stack(long ptr, int value);
	private static native int c_system_pop_stack(long ptr);
	
	private static native void c_system_write_data(long ptr, int addr, int value);
	private static native byte c_system_read_data(long ptr, int addr);
	
	private static native byte[] c_system_dump_data(long ptr);
	
	private static native void c_system_set_sfr(long ptr, int addr, int bit);
	
	private static native void c_system_clear_sfr(long ptr, int addr, int bit);
	
	private static native byte[] c_system_dump_eeprom(long ptr);
}
