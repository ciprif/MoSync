package com.mosync.internal.android.extensions;

import java.io.InputStream;
import java.lang.reflect.Method;
import java.nio.charset.Charset;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import com.mosync.api.MoSyncContext;
import com.mosync.internal.android.MoSyncThread;

import android.content.res.AssetManager;
import android.util.Log;

public class MoSyncExtensionLoader {

	public static final String MODULE_TAG = "module";
	public static final String CLASS_ATTR = "class";
	public static final String FUNCTION_TAG = "function";
	public static final String ARGUMENT_TAG = "arg";
	public static final String STRUCT_TAG = "struct";
	public static final String MEMBER_TAG = "member";
	public static final String TYPEDEF_TAG = "typedef";
	public static final String NAME_ATTR = "name";
	public static final String HASH_ATTR = "hash";
	public static final String IN_ATTR = "in";
	public static final String OUT_ATTR = "out";
	public static final String TYPE_ATTR = "type";
	public static final String POINTER_ATTR = "ptr";

	private static MoSyncExtensionLoader instance = new MoSyncExtensionLoader();

	private HashMap<String, ExtensionModule> modules = new HashMap<String, ExtensionModule>();
	private HashSet<String> loadedModules = new HashSet<String>();
	private HashMap<Integer, ExtensionModule> modulesById = new HashMap<Integer, ExtensionModule>();
	private HashMap<Integer, FunctionInvocation> invokersById = new HashMap<Integer, FunctionInvocation>();
	private HashMap<String, TypeDescriptor> typedefs;
	private Map<String, StructType> structs = new HashMap<String, StructType>();
	private int moduleId = 0;

	private MoSyncExtensionLoader() {

	}

	public void load(String extensionName) {
		if (loadedModules.contains(extensionName)) {
			return;
		}
		loadedModules.add(extensionName);

		try {
			ArrayList<ExtensionModule> toBeInitialized = new ArrayList<ExtensionModule>();

			InputStream extensionFile = null;
			try {
				int functionId = 0;

				AssetManager assets = MoSyncThread.getInstance().getActivity()
						.getAssets();
				extensionFile = assets.open(extensionName + ".xml");
				XmlPullParserFactory factory = XmlPullParserFactory
						.newInstance();
				XmlPullParser xpp = factory.newPullParser();
				xpp.setInput(extensionFile, "UTF-8");

				ExtensionModule currentModule = null;
				String currentFunction = null;
				StructType currentStruct = null;
				ArrayList<TypeDescriptor> currentDescriptors = new ArrayList<TypeDescriptor>();
				ArrayList<FunctionInvocation> currentFunctions = new ArrayList<FunctionInvocation>();
				HashMap<String, TypeDescriptor> typedefs = new HashMap<String, TypeDescriptor>();
				HashMap<String, StructType> structs = new HashMap<String, StructType>();

				for (int eventType = xpp.getEventType(); eventType != XmlPullParser.END_DOCUMENT; eventType = xpp
						.getEventType()) {
					String tagName = xpp.getName();
					switch (eventType) {
					case XmlPullParser.START_TAG:
						// Common attrs.
						String name = xpp.getAttributeValue(null,
								NAME_ATTR);
						String type = xpp.getAttributeValue(null, TYPE_ATTR);
						String ptrStr = xpp.getAttributeValue(null, POINTER_ATTR);
						int ptr = ptrStr == null ? 0 : Integer.valueOf(ptrStr);
						if (MODULE_TAG.equals(tagName)) {
							String currentModuleClassName = xpp
									.getAttributeValue(null, CLASS_ATTR);
							String hashStr = xpp.getAttributeValue(null, HASH_ATTR);
							int hash = (int) Long.parseLong(hashStr, 16);
							moduleId++;
							currentModule = new ExtensionModule(moduleId,
									name, currentModuleClassName, hash);
							toBeInitialized.add(currentModule);
							modules.put(name, currentModule);
							modulesById.put(moduleId, currentModule);
							currentModule.setTypedefs(typedefs);
							currentModule.setStructs(structs);
						} else if (TYPEDEF_TAG.equals(tagName)) {
							if (name != null && type != null) {
								typedefs.put(name, new Typedef(currentModule, name, type));
							}
						} else if (STRUCT_TAG.equals(tagName)) {
							String structClass = xpp.getAttributeValue(null, CLASS_ATTR);
							currentStruct = new StructType(currentModule, type, structClass);
							structs.put(name, currentStruct);
						} else if (MEMBER_TAG.equals(tagName)) {
							currentStruct.addMember(name, type, ptr);
						} else if (FUNCTION_TAG.equals(tagName)) {
							currentFunction = xpp.getAttributeValue(null,
									NAME_ATTR);
						} else if (ARGUMENT_TAG.equals(tagName)) {
							boolean out = Boolean.valueOf(xpp
									.getAttributeValue(null, OUT_ATTR));
							currentDescriptors.add(currentModule.getTypeDescriptor(type, ptr,
									out));
						} else {
							throw new IllegalArgumentException("Invalid tag: " + tagName);
						}
						break;
					case XmlPullParser.END_TAG:
						if (FUNCTION_TAG.equals(tagName)) {
							int handle = moduleId << 20 | functionId;
							FunctionInvocation invoker = new FunctionInvocation(
									handle, currentModule, currentFunction,
									currentDescriptors
											.toArray(new TypeDescriptor[0]));
							currentFunctions.add(invoker);
							invokersById.put(handle, invoker);
							currentDescriptors.clear();
							functionId++;
						} else if (MODULE_TAG.equals(tagName)) {
							functionId = 0;
							currentModule.setInvokers(currentFunctions
									.toArray(new FunctionInvocation[0]));
							Log.d("@@MoSync", "Loaded extension "
									+ currentModule.getName());
						}
						break;
					}
					xpp.next();
				}

				for (ExtensionModule module : toBeInitialized) {
					initialize(module);
				}
			} finally {
				if (extensionFile != null) {
					extensionFile.close();
				}
			}
		} catch (Throwable t) {
			Log.e("@@MoSync", "Could not load extensions", t);
			MoSyncThread.getInstance().maPanic(40074,
					"Could not load extensions: " + t);
		}
	}

	private void initialize(ExtensionModule module) throws Exception {
		try {
			Object implementation = module.getModule();
			Method initializer = implementation.getClass().getMethod("initialize", MoSyncContext.class);
			initializer.invoke(implementation, MoSyncThread.getInstance());
			Log.i("@@MoSync", "Called initializer for extension " + module.getName());
		} catch (NoSuchMethodException e) {
			// This is perfectly ok, but a bit awkward
			// to rely on an exception.
			Log.i("@@MoSync", "No initializer exists for extension " + module.getName());
		}
	}

	public ExtensionModule getModule(String module) {
		load(module);
		return modules.get(module);
	}

	public synchronized int maExtensionModuleLoad(String name, int hash) {
		load(name);

		ExtensionModule module = modules.get(name);
		if (module != null) {
			int moduleHash = module.getHash();
			if (moduleHash != hash) {
				MoSyncThread.getInstance().maPanic(40073, "Extension version mismatch.");
			}
			return module.getId();
		} else {
			MoSyncThread.getInstance().maPanic(40075, "Module does not exist. (" + name + ")");
		}
		return 0;
	}

	public int maExtensionFunctionLoad(int moduleId, int ix) {
		ExtensionModule module = modulesById.get(moduleId);
		if (module != null) {
			FunctionInvocation function = module.getFunction(ix);
			if (function != null) {
				return function.getId();
			}
		}
		return 0;
	}

	public int maInvokeExtension(int function, int ptr, int memstart) {
		FunctionInvocation invoker = invokersById.get(function);

		if (invoker != null) {
			try {
				// FIXME: We need no array here
				return invoker.invoke(ptr);
			} catch (Throwable e) {
				// Log && Fall-thru.
				Log.e("@@MoSync", "Could not invoke function", e);
			}
		}

		String fnName = invoker == null ?
				"No function with id " + Integer.toHexString(function) :
				"Function \'" + invoker.getName() + "\'";
		MoSyncThread.getInstance().maPanic(40075,
				"Invalid extension call (" + fnName + ")");
		return 0;
	}

	public static MoSyncExtensionLoader getDefault() {
		return instance;
	}


}