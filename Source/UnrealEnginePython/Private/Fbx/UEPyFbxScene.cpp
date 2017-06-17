#include "UnrealEnginePythonPrivatePCH.h"

#if WITH_EDITOR

#include "UePyFbx.h"

static PyObject *py_ue_fbx_scene_get_root_node(ue_PyFbxScene *self, PyObject *args) {
	FbxNode *fbx_node = self->fbx_scene->GetRootNode();
	if (!fbx_node) {
		return PyErr_Format(PyExc_Exception, "unable to get RootNode from FbxScene");
	}

	return py_ue_new_fbx_node(fbx_node);
}

static PyObject *py_ue_fbx_scene_get_src_object_count(ue_PyFbxScene *self, PyObject *args) {
	return PyLong_FromLong(self->fbx_scene->GetSrcObjectCount());
}

static PyObject *py_ue_fbx_scene_get_src_object(ue_PyFbxScene *self, PyObject *args) {
	int index;
	if (!PyArg_ParseTuple(args, "i", &index)) {
		return nullptr;
	}
	FbxObject *fbx_object = self->fbx_scene->GetSrcObject(index);
	if (!fbx_object)
		return PyErr_Format(PyExc_Exception, "unable to find FbxObject with index %d", index);

	return py_ue_new_fbx_object(fbx_object);
}

static PyMethodDef ue_PyFbxScene_methods[] = {
	{ "get_root_node", (PyCFunction)py_ue_fbx_scene_get_root_node, METH_VARARGS, "" },
	{ "get_src_object_count", (PyCFunction)py_ue_fbx_scene_get_src_object_count, METH_VARARGS, "" },
	{ "get_src_object", (PyCFunction)py_ue_fbx_scene_get_src_object, METH_VARARGS, "" },
	{ NULL }  /* Sentinel */
};

static PyTypeObject ue_PyFbxSceneType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"unreal_engine.FbxScene", /* tp_name */
	sizeof(ue_PyFbxManager),    /* tp_basicsize */
	0,                         /* tp_itemsize */
	0,   /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_reserved */
	0,                         /* tp_repr */
	0,                         /* tp_as_number */
	0,                         /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash  */
	0,                         /* tp_call */
	0,                         /* tp_str */
	0,                         /* tp_getattro */
	0,                         /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,        /* tp_flags */
	"Unreal Engine FbxScene", /* tp_doc */
	0,                         /* tp_traverse */
	0,                         /* tp_clear */
	0,                         /* tp_richcompare */
	0,                         /* tp_weaklistoffset */
	0,                         /* tp_iter */
	0,                         /* tp_iternext */
	ue_PyFbxScene_methods,    /* tp_methods */
	0,   /* tp_members */
	0,                         /* tp_getset */
};

static int py_ue_fbx_scene_init(ue_PyFbxScene *self, PyObject * args) {
	PyObject *py_object;
	char *name;
	if (!PyArg_ParseTuple(args, "Os", &py_object, &name)) {
		return -1;
	}

	ue_PyFbxManager *py_fbx_manager = py_ue_is_fbx_manager(py_object);
	if (!py_fbx_manager) {
		PyErr_SetString(PyExc_Exception, "argument is not a FbxManager");
		return -1;
	}

	self->fbx_scene = FbxScene::Create(py_fbx_manager->fbx_manager, name);
	return 0;
}

void ue_python_init_fbx_scene(PyObject *ue_module) {
	ue_PyFbxSceneType.tp_new = PyType_GenericNew;;
	ue_PyFbxSceneType.tp_init = (initproc)py_ue_fbx_scene_init;
	if (PyType_Ready(&ue_PyFbxSceneType) < 0)
		return;

	Py_INCREF(&ue_PyFbxSceneType);
	PyModule_AddObject(ue_module, "FbxScene", (PyObject *)&ue_PyFbxSceneType);
}

ue_PyFbxScene *py_ue_is_fbx_scene(PyObject *obj) {
	if (!PyObject_IsInstance(obj, (PyObject *)&ue_PyFbxSceneType))
		return nullptr;
	return (ue_PyFbxScene *)obj;
}

#endif