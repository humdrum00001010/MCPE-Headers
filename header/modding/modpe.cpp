#include "modpe.h"

#define DEX_PATH "/sdcard/games/com.mojang/JSLib.dex"
#define DEX_DUMMY_PATH "/data/data/net.zhuoweizhang.mcpelauncher/files/"
#define READ "r"

#include <stdio.h>

#include <vector>
#include <string>
#include <sstream>

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

struct ClassInfo {
	const char *name;
	jclass singleton;
};

static std::vector<ClassInfo> classArray;

bool ModPE::dexFile = false;

int getRealSize(int hexCode) {
	if (hexCode & 0xff == hexCode) return 1;
	if (hexCode & 0xffff == hexCode) return 2;
	if (hexCode & -1 == hexCode) return 4;
	return -1;
}

#include <modding/modding.h>

JNIEXPORT void JNICALL nativeBinaryPatch(JNIEnv *env, jobject obj, jstring symbol, jint relative_offset, jint hexCode, jint prot) {
	const char *symbol_arr = env->GetStringUTFChars(symbol, NULL);
	std::string real_symbol(symbol_arr);
	env->ReleaseStringUTFChars(symbol, symbol_arr);
	int size = getRealSize(hexCode);
	void *addr = (void *)(dlsym(RTLD_DEFAULT, real_symbol.c_str()) + relative_offset);
	switch (size) {
	case 1:
		Reversing::modify_1(addr, (uint8_t)hexCode, prot);
		break;
	case 2:
		Reversing::modify_2(addr, (uint16_t)hexCode, prot);
		break;
	case 4:
		Reversing::modify_4(addr, (uint32_t)hexCode, prot);
		break;
	default:
		break;
	}
}

static jclass RCE;

#include <thread>

static jclass List;
static jclass MainActivity;
static jclass Object;
static jclass Reference;
static jclass ScriptState;
static jclass ScriptableObject;
static jclass Set;
static jclass Utils;

void ModPE::main(JavaVM *vm) {
	JNI_CALL(vm);
	FILE *pFile;
	if (dexFile = pFile = fopen(DEX_PATH, READ)) {
		fclose(pFile);
		JNI_ATTACH(vm);

		JNINativeMethod _nativeBinaryPatch = { "nativeBinaryPatch", "(Ljava/lang/String;III)V", (void *)nativeBinaryPatch };
		JNINativeMethod methods[] = {
			_nativeBinaryPatch
		};

		loadDex(env, "com.perkan.java.RCE", methods, sizeof(methods) / sizeof(JNINativeMethod), &RCE);

		jclass scriptStateCls = env->FindClass("net/zhuoweizhang/mcpelauncher/ScriptManager$ScriptState");
		ScriptState = (jclass)env->NewGlobalRef(scriptStateCls);
		env->DeleteLocalRef(scriptStateCls);
		jclass scriptableObjectCls = env->FindClass("org/mozilla/javascript/ScriptableObject");
		ScriptableObject = (jclass)env->NewGlobalRef(scriptableObjectCls);
		env->DeleteLocalRef(scriptableObjectCls);
		jclass mainActivityCls = env->FindClass("com/mojang/minecraftpe/MainActivity");
		MainActivity = (jclass)env->NewGlobalRef(mainActivityCls);
		env->DeleteLocalRef(mainActivityCls);
		jclass referenceCls = env->FindClass("java/lang/ref/Reference");
		Reference = (jclass)env->NewGlobalRef(referenceCls);
		env->DeleteLocalRef(referenceCls);
		jclass listCls = env->FindClass("java/util/List");
		List = (jclass)env->NewGlobalRef(listCls);
		env->DeleteLocalRef(listCls);
		jclass utilsCls = env->FindClass("net/zhuoweizhang/mcpelauncher/Utils");
		Utils = (jclass)env->NewGlobalRef(utilsCls);
		env->DeleteLocalRef(utilsCls);
		jclass setCls = env->FindClass("java/util/Set");
		Set = (jclass)env->NewGlobalRef(setCls);
		env->DeleteLocalRef(setCls);
		jclass objectCls = env->FindClass("java/lang/Object");
		Object = (jclass)env->NewGlobalRef(objectCls);
		env->DeleteLocalRef(objectCls);

		JNI_DETACH(vm);
	}
}

void ModPE::callScriptMethod(JNIEnv *env, const char *hookName, jobjectArray objArray) {
	env->CallStaticVoidMethod(
		bl_scriptmanager_class,
		env->GetStaticMethodID(bl_scriptmanager_class, "callScriptMethod", "(Ljava/lang/String;[Ljava/lang/Object;)V"),
		env->NewStringUTF(hookName),
		objArray
	);
}

void ModPE::loadDex(JNIEnv *env, const char *n_className, JNINativeMethod *function_implement, int JNI_s, jclass *globalRef) {
	bl_JavaVM->AttachCurrentThread(&env, NULL);
	jclass classCls = env->FindClass("java/lang/Class");
	jobject classLoader = env->CallObjectMethod(
		env->FindClass("com/mojang/minecraftpe/MainActivity")
		, env->GetMethodID(classCls, "getClassLoader", "()Ljava/lang/ClassLoader;")
	);
	jclass dexClassLoaderCls = env->FindClass("dalvik/system/DexClassLoader");
	jobject dexClassLoader = env->NewObject(
		dexClassLoaderCls
		, env->GetMethodID(dexClassLoaderCls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V")
		, env->NewStringUTF(DEX_PATH)
		, env->NewStringUTF(DEX_DUMMY_PATH)
		, NULL
		, classLoader
	);
	jclass n_jsCls = (jclass)env->CallStaticObjectMethod(
		classCls
		, env->GetStaticMethodID(classCls, "forName", "(Ljava/lang/String;ZLjava/lang/ClassLoader;)Ljava/lang/Class;")
		, env->NewStringUTF(n_className)
		, true
		, dexClassLoader
	);
	env->RegisterNatives(n_jsCls, function_implement, JNI_s);
	*globalRef = (jclass)env->NewGlobalRef(n_jsCls);
	env->DeleteLocalRef(n_jsCls);

	std::vector<std::string> vects = split(n_className, '.');
	classArray.push_back({ vects[vects.size() - 1].c_str(), *globalRef });
}

void ModPE::applyModPE(JNIEnv *env, jclass javaClass, const char *javaClassName) {
	if (!dexFile)
		return;

	jobject mainActivity = env->CallObjectMethod(
		env->GetStaticObjectField(MainActivity, env->GetStaticFieldID(MainActivity, "currentMainActivity", "Ljava/lang/ref/WeakReference;"))
		, env->GetMethodID(Reference, "get", "()Ljava/lang/Object;")
	);
	jfieldID displayMetricsGet = env->GetFieldID(MainActivity, "displayMetrics", "Landroid/util/DisplayMetrics;");
	jobject scripts = env->GetStaticObjectField(bl_scriptmanager_class, env->GetStaticFieldID(bl_scriptmanager_class, "scripts", "Ljava/util/List;"));
	jmethodID sizeGet = env->GetMethodID(List, "size", "()I");
	jint scriptCount = env->CallIntMethod(
		env->CallStaticObjectMethod(Utils, env->GetStaticMethodID(Utils, "getEnabledScripts", "()Ljava/util/Set;"))
		, env->GetMethodID(Set, "size", "()I")
	);
	jint size = env->CallIntMethod(scripts, sizeGet);
	jmethodID listGet = env->GetMethodID(List, "get", "(I)Ljava/lang/Object;");
	jfieldID getScope = env->GetFieldID(ScriptState, "scope", "Lorg/mozilla/javascript/Scriptable;");
	jmethodID hasProperty = env->GetStaticMethodID(ScriptableObject, "hasProperty", "(Lorg/mozilla/javascript/Scriptable;Ljava/lang/String;)Z");
	jmethodID defineClass = env->GetStaticMethodID(ScriptableObject, "defineClass", "(Lorg/mozilla/javascript/Scriptable;Ljava/lang/Class;Z)V");

	for (int i = 0; i < size; i++) {
		jobject scope = env->GetObjectField(env->CallObjectMethod(scripts, listGet, i), getScope);
		if (!env->CallStaticBooleanMethod(ScriptableObject, hasProperty, scope, env->NewStringUTF(javaClassName)))
			env->CallStaticVoidMethod(ScriptableObject, defineClass, scope, javaClass, true);
	}

	callScriptMethod(env, "onNewModPELoaded", env->NewObjectArray(0, Object, NULL));
}

void ModPE::threadFunction() {
	JNI_CALL(bl_JavaVM);
	JNI_ATTACH(bl_JavaVM);

	for (ClassInfo info : classArray) {
		applyModPE(env, info.singleton, info.name);
	}

	JNI_DETACH(bl_JavaVM);
}