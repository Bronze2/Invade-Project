#pragma once
#include <PxPhysicsAPI.h>
#include <PxPhysics.h>
#include <PxActor.h>
#include <iostream>
#include <PxShape.h>

using namespace std;
using namespace physx;

class CPhysics
{
    // declare variables
    physx::PxDefaultAllocator      mDefaultAllocatorCallback;
    physx::PxDefaultErrorCallback  mDefaultErrorCallback;
    physx::PxDefaultCpuDispatcher* mDispatcher = NULL;
    physx::PxTolerancesScale       mToleranceScale;

    physx::PxFoundation* mFoundation = NULL;
    physx::PxPhysics* mPhysics = NULL;

    physx::PxScene* mScene = NULL;
    physx::PxMaterial* mMaterial = NULL;

    physx::PxPvd* mPvd = NULL;

public :
    void init();
    void create_simulation();
    void run_simulation();

    CPhysics();
    ~CPhysics();

};

