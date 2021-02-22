#include <jni.h>

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include "VMCUReport.h"

//to make libvmcu_system.h work
struct vmcu_report;
typedef struct vmcu_report vmcu_report_t;

#include "libvmcu_system.h"
#include "libvmcu_analyzer.h"

/*
 * Class:     VMCUReport
 * Method:    c_report_ctor
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_VMCUReport_c_1report_1ctor
  (JNIEnv *env, jclass obj, jstring filename){

	(void)env;
	(void)obj;

	//converting java String to char* 
	const char* my_filename  = (*env)->GetStringUTFChars(env, filename, 0);

	vmcu_report_t* report = vmcu_analyze_ihex(my_filename);
	
	if(report == NULL){ return (jlong)0; }

	//free the allocated char*
	(*env)->ReleaseStringUTFChars(env, filename, my_filename);

	vmcu_system_t* sys = vmcu_system_ctor(report);
	
	return (jlong)report;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_dtor
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_VMCUReport_c_1report_1dtor
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	vmcu_report_dtor(report);
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_progsize
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1progsize
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;
	
	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->progsize;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_nlabels
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1nlabels
  (JNIEnv* env, jclass obj, jlong ptr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return report->nlabels;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_opcode
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1opcode
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->disassembly[(int)addr].opcode;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_exec
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1exec
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jboolean)report->disassembly[(int)addr].exec;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_dword
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1dword
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jboolean)report->disassembly[(int)addr].dword;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_mnem
 * Signature: (JI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1mnem
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	
	char* outCStr = report->disassembly[(int)addr].mnem;
	
	return (*env)->NewStringUTF(env, outCStr);
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_key
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1key
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->disassembly[(int)addr].key;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_src_value
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1src_1value
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->disassembly[(int)addr].src.value;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_src_type
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1src_1type
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->disassembly[(int)addr].src.type;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_dest_value
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1dest_1value
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->disassembly[(int)addr].dest.value;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_plain_dest_type
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1plain_1dest_1type
  (JNIEnv* env, jclass obj, jlong ptr, jint addr){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->disassembly[(int)addr].dest.type;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_label_id
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1label_1id
  (JNIEnv* env, jclass obj, jlong ptr, jint index){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->labels[(int)index].id;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_label_addr
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1label_1addr
  (JNIEnv* env, jclass obj, jlong ptr, jint index){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->labels[(int)index].addr;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_label_ncallers
 * Signature: (JI)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1label_1ncallers
  (JNIEnv* env, jclass obj, jlong ptr, jint index){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->labels[(int)index].ncallers;
}

/*
 * Class:     VMCUReport
 * Method:    c_report_get_disassembly_label_caller_addr
 * Signature: (JII)I
 */
JNIEXPORT jint JNICALL Java_VMCUReport_c_1report_1get_1disassembly_1label_1caller_1addr
  (JNIEnv* env, jclass obj, jlong ptr, jint label_index, jint caller_index){

	(void)env;
	(void)obj;

	vmcu_report_t* report = (vmcu_report_t*)ptr;
	return (jint)report->labels[(int)label_index].caller[(int)caller_index].addr;
}


