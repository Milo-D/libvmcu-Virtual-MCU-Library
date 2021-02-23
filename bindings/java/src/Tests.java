public class Tests {

private static void check(boolean condition, String msg){
	if(!condition){
		throw new RuntimeException("ERROR: "+msg);
	}
}

private static void check(boolean condition){
	check(condition, "Assertion Error");
}

private static void note(String s){
	System.out.println(" TEST: "+s);
}

public static void main(String[] args){

	System.out.println("Running VMCU Binding Tests");
	
	String filename = "../../../examples/m328p/algo/kmp.hex";
	
	testSystem(filename);
	testReport(filename);
}

private static void testSystem(String filename){
	
	VMCUReport report = new VMCUReport(filename);
	VMCUSystem sys = new VMCUSystem(report);
	
	testSystemRWGPR(sys);
	testSystemRWSREG(sys);
	testSystemStepAndBackstep(sys);
	
	testSystemReboot(sys);
	testSystemDumpSREG(sys);
	testSystemDumpGPR(sys);
	testSystemGetSetMovePC(sys);
	testSystemPushPopStack(sys);
	testSystemRWData(sys);
	
	System.out.println("PASSED System Binding Tests");
}

private static void testReport(String filename){

	VMCUReport report = new VMCUReport(filename);
	
	testReportGetProgsize(report);
	testReportGetNlabels(report);
	
	testReportDisassemblyPlainExec(report);
	testReportDisassemblyPlainDWord(report);
	testReportDisassemblyPlainMnem(report);
	testReportDisassemblyPlainKey(report);
	
	//a lot of stuff is not tested here
	
	System.out.println("PASSED Report Binding Tests");
}

// -----------------------------------

private static void testSystemRWGPR(VMCUSystem sys){
	
	note("Read/Write GPR");
	
	final int rx = 0;
	
	for(int i=0; i < 255; i++){
	
		sys.write_gpr(rx, i);
		int value = Utils.unsigned(sys.read_gpr(rx));
		
		check(value == i, "gpr read/write Error");
	}
}
private static void testSystemRWSREG(VMCUSystem sys){

	note("Read/Write SREG");

	//SREGs should be false at the start
	
	//@flag:   flag position { 0, 1, ..., 7 }
	int flag = 4;
	sys.write_sreg(flag, true);
	
	boolean value = sys.read_sreg(flag);
	
	check(value == true, "sreg read/write Error");
}
private static void testSystemStepAndBackstep(VMCUSystem sys){
	
	note("Step/Backstep");
	
	sys.step();
	
	int old_pc = sys.get_pc();
	
	sys.step();
	sys.backstep();
	
	int pc = sys.get_pc();
	
	check(pc == old_pc, "step/backstep Error");
	
	sys.step();
	sys.step();
	
	check(sys.get_steps() == 3, "get_steps() Error");
}

private static void testSystemReboot(VMCUSystem sys){
	
	note("System Reboot");
	
	sys.step();
	sys.step();
	int pc = sys.get_pc();
	
	sys.reboot();
	
	check(sys.get_pc() == 0, "reboot Error");
}
private static void testSystemDumpSREG(VMCUSystem sys){

	note("Dump SREG");

	sys.reboot();
	sys.write_sreg(0, true);
	int sreg = sys.dump_sreg();
	
	check(sreg == 1, "dump SREG Error");

}
private static void testSystemDumpGPR(VMCUSystem sys){

	note("Dump GPR");

	sys.reboot();
	sys.write_gpr(0, 43);
	byte[] gprs = sys.dump_gpr();
	
	check(gprs[0] == 43, "dump_gpr Error");
	gprs[0] = 0;
	
	for(int i=0;i < 32; i++){
		check(gprs[i] == 0, "dump_gpr Error");
	}
}
private static void testSystemGetSetMovePC(VMCUSystem sys){

	note("Get/Set/Move PC");

	sys.reboot();
	check( sys.get_pc() == 0);
	
	sys.move_pc(4);
	check(sys.get_pc() == 4);
	
	sys.move_pc(-3);
	check(sys.get_pc() == 1);
	
	sys.set_pc(48);
	check(sys.get_pc() == 48);
}
private static void testSystemPushPopStack(VMCUSystem sys){
	
	note("Push/Pop Stack");
	
	sys.reboot();
	sys.push_stack(23);
	
	int value = sys.pop_stack();
	
	check(value == 23);
}
private static void testSystemRWData(VMCUSystem sys){

	note("Read/Write Data");

	sys.reboot();

	int addr = 0x43;
	byte value = 0x21;
	
	sys.write_data(addr, value);
	byte value_read = sys.read_data(addr);
	
	check(value == value_read);
}

//-------------------------------------------------------

private static void testReportGetProgsize(VMCUReport report){

	note("GetProgsize");
	
	int progsize = report.get_progsize();
	
	//System.out.println(progsize);
	check(progsize == 304);
}
private static void testReportGetNlabels(VMCUReport report){
	
	note("GetNLabels");
	
	int nlabels = report.get_nlabels();
	
	//System.out.println(nlabels);
	
	check(nlabels == 27);
}

private static void testReportDisassemblyPlainExec(VMCUReport report){

	note("DisassemblyPlainExec");
	int addr = 0x0;
	
	boolean exec = report.get_disassembly_plain_exec(addr);
	check(exec == true);
}
private static void testReportDisassemblyPlainDWord(VMCUReport report){

	note("DisassemblyPlainDWord");
	int addr = 0x0;
	boolean isdword = report.get_disassembly_plain_dword(addr);
	
	check(isdword == true);
}
private static void testReportDisassemblyPlainMnem(VMCUReport report){

	note("DisassemblyPlainMnem");
	
	int index = 0x0;
	String mnem = report.get_disassembly_plain_mnem(index);
	
	//System.out.println(mnem);
	String expected = "jmp +52                   ; PC <- 0x34";
	
	check(mnem.equals(expected));
}
private static void testReportDisassemblyPlainKey(VMCUReport report){
	
	note("DisassemblyPlainKey");
	int index = 0;
	
	int key = report.get_disassembly_plain_key(index);
	//System.out.println(key);
	check(key == 10);
}
	
}
