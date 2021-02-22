#include <jni.h>

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include "VMCUSystem.h"

//to make libvmcu_system.h work
struct vmcu_report;
typedef struct vmcu_report vmcu_report_t;

#include "libvmcu_system.h"
#include "libvmcu_analyzer.h"


jintArray adjust_uint8_t_array(JNIEnv* env, uint8_t* arr, int n){
	
	//TODO: should we free the allocated arrays later?
	
	jint* outCArray = malloc(sizeof(jint)*n);
	
	for(int i=0;i < n; i++){ outCArray[i] = (uint32_t)arr[i]; }
	
	jintArray out = (*env)->NewIntArray(env, n);
	
	(*env)->SetIntArrayRegion(env, out, 0, n, outCArray);

	return out;
}

jbyteArray adjust_int8_t_array_bytes(JNIEnv* env, int8_t* arr, int n){

	jbyte* outCArray = malloc(sizeof(jbyte)*n);
	
	for(int i=0;i < n; i++){ outCArray[i] = (int8_t)arr[i]; }
	
	jbyteArray out = (*env)->NewByteArray(env, n);
	
	(*env)->SetByteArrayRegion(env, out, 0, n, outCArray);

	return out;
}

//-----------------

/*
 * Class:     VMCUSystem
 * Method:    c_system_ctor
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_VMCUSystem_c_1system_1ctor
  (JNIEnv * env, jclass obj, jlong ptr){
  
	vmcu_system_t* sys;

	(void)env;
	(void)obj;

	//converting java String to char* 
	//const char* my_filename  = (*env)->GetStringUTFChars(env, filename, 0);

	//vmcu_report_t* report = vmcu_analyze_ihex(my_filename);
	vmcu_report_t* report = (vmcu_report_t*)ptr;
	
	if(report == NULL){ return (jlong)0; }

	//free the allocated char*
	//(*env)->ReleaseStringUTFChars(env, filename, my_filename);

	sys = vmcu_system_ctor(report);

	//vmcu_report_dtor(report);

	return (jlong)sys;
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_dtor
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1dtor
  (JNIEnv *env, jclass obj, jlong ptr){
  
	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_dtor(sys);
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_step
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_VMCUSystem_c_1system_1step
  (JNIEnv *env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jint)vmcu_system_step(sys); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_backstep
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1backstep
  (JNIEnv *env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_backstep(sys); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_reboot
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1reboot
  (JNIEnv* env, jclass obj, jlong ptr){
  
	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_reboot(sys); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_write_gpr
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1write_1gpr
  (JNIEnv* env, jclass obj, jlong ptr, jint rx, jint value){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_write_gpr(sys, (int)rx, (int)value); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_read_gpr
 * Signature: (JI)I
 */
JNIEXPORT jbyte JNICALL Java_VMCUSystem_c_1system_1read_1gpr
  (JNIEnv* env, jclass obj, jlong ptr, jint rx){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jbyte)vmcu_system_read_gpr(sys, (int)rx); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_dump_gpr
 * Signature: (J)[I
 */
JNIEXPORT jbyteArray JNICALL Java_VMCUSystem_c_1system_1dump_1gpr
  (JNIEnv* env, jclass obj, jlong ptr){
  
	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	
	const int nGPRs = 32;
	int8_t* gprs = vmcu_system_dump_gpr(sys);
	
	jbyteArray result = adjust_int8_t_array_bytes(env, gprs, nGPRs);
	return result;
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_write_sreg
 * Signature: (JIZ)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1write_1sreg
  (JNIEnv* env, jclass obj, jlong ptr, jint addr, jboolean value){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_write_sreg(sys, (int)addr, (bool)value);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_read_sreg
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_VMCUSystem_c_1system_1read_1sreg
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jboolean)vmcu_system_read_sreg(sys, (int)addr); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_dump_sreg
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_VMCUSystem_c_1system_1dump_1sreg
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jint)vmcu_system_dump_sreg(sys); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_read_flash
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUSystem_c_1system_1read_1flash
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){
	
	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jint)vmcu_system_read_flash(sys, (int)addr); 
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_move_pc
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1move_1pc
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){
	
	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_move_pc(sys, (int)addr);
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_set_pc
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1set_1pc
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_set_pc(sys, (int)addr);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_get_pc
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_VMCUSystem_c_1system_1get_1pc
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jint)vmcu_system_get_pc(sys);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_push_stack
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1push_1stack
  (JNIEnv* env, jclass obj, jlong ptr, jint value){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_push_stack(sys, (int)value);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_pop_stack
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_VMCUSystem_c_1system_1pop_1stack
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jint)vmcu_system_pop_stack(sys);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_write_data
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1write_1data
  (JNIEnv* env, jclass obj, jlong ptr, jint addr, jint value){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_write_data(sys, (int)addr, (int)value);
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_read_data
 * Signature: (JI)I
 */
JNIEXPORT jbyte JNICALL Java_VMCUSystem_c_1system_1read_1data
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	return (jbyte)vmcu_system_read_data(sys, (int)addr);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_dump_data
 * Signature: (J)[I
 */
JNIEXPORT jintArray JNICALL Java_VMCUSystem_c_1system_1dump_1data
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	int8_t* arr = vmcu_system_dump_data(sys);
	
	//TODO: adjust highest address 
	int highest_addr = 0x08ff;
	int upperBoundLength = highest_addr + 1;
	
	jintArray result = adjust_uint8_t_array(env, arr, upperBoundLength);
	return result;
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_set_sfr
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1set_1sfr
  (JNIEnv* env, jclass obj, jlong ptr, jint addr, jint value){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_set_sfr(sys, (int)addr, (int)value);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_clear_sfr
 * Signature: (JII)V
 */
JNIEXPORT void JNICALL Java_VMCUSystem_c_1system_1clear_1sfr
  (JNIEnv* env, jclass obj, jlong ptr, jint addr, jint bit){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	vmcu_system_clear_sfr(sys, (int)addr, (int)bit);   
}

/*
 * Class:     VMCUSystem
 * Method:    c_system_dump_eeprom
 * Signature: (J)[I
 */
JNIEXPORT jintArray JNICALL Java_VMCUSystem_c_1system_1dump_1eeprom
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_system_t* sys = (vmcu_system_t*)ptr;
	int8_t* arr = vmcu_system_dump_eeprom(sys);   
	
	//TODO: adjust the upper bound to be matching
	//for the specific controller
	int upperBoundLength = 0x3ff + 1;
	
	return adjust_uint8_t_array(env, arr, upperBoundLength);
}


