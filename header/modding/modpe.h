#ifndef MODPE_H
#define MODPE_H

#include <jni.h>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif
	extern JavaVM *bl_JavaVM;
	extern jclass bl_scriptmanager_class;
#ifdef __cplusplus
}
#endif

class ModPE {
public:
	static bool dexFile;
	static void main(JavaVM *vm);
public:
	static void callScriptMethod(JNIEnv *env, const char *hookName, jobjectArray objArray);
	static void loadDex(JNIEnv *env, const char *n_className, JNINativeMethod *function_implement, int JNI_s, jclass *globalRef);
	static void applyModPE(JNIEnv *env, jclass javaClass, const char *javaClassName);
	static void threadFunction();
};

#ifdef JNI_H_
#define JNI_CALL(JVM) \
	JNIEnv *env = NULL; \
	int attachStatus = JVM->GetEnv((void **)&env, JNI_VERSION_1_2); \
	do {} while (false)

#define JNI_ATTACH(JVM) do { \
	if (attachStatus == JNI_EDETACHED) \
		JVM->AttachCurrentThread(&env, NULL); \
} while (false)

#define JNI_DETACH(JVM) do { \
	if (attachStatus == JNI_EDETACHED) \
		JVM->DetachCurrentThread(); \
} while (false)
#endif

#endif