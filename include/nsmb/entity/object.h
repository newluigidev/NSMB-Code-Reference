#pragma once
#include "common.h"
#include "base.h"


struct ObjectProfile {

	Object* (*constructor)();

	u16 updatePriority;
	u16 renderPriority;

	__inline Object* tryConstruct() {
		return constructor ? constructor() : nullptr;
	}

};


class Object : public Base
{
public:

	__inline Object() {}

	//020131a8
	sym virtual void postCreate(BaseReturnState state) __body

	//D0:0201313c
	//D1:0201311c
	sym virtual ~Object() __body

	//02013168
	sym static Object* spawnScene(u16 objectID, u32 settings, ObjectType type) __rbody

	//02013188
	sym static Object* spawnObject(u16 objectID, Base* scene, u32 settings, ObjectType type) __rbody


	template<class T>
	static Object* constructObject() {
		return new T;
	}

};