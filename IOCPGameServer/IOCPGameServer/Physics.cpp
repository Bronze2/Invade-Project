//#include "Physics.h"
//
//
//CPhysics::CPhysics() {};
//CPhysics::~CPhysics() {};
//
//void CPhysics::init()
//{
//    // init physx
//    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
//    if (!mFoundation) throw("PxCreateFoundation failed!");
//    mPvd = PxCreatePvd(*mFoundation);
//    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
//    mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
//    //mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(),true, mPvd);
//    mToleranceScale.length = 100;        // typical length of an object
//    mToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
//    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
//    //mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale);
//
//    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
//    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
//    mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
//    sceneDesc.cpuDispatcher = mDispatcher;
//    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
//    mScene = mPhysics->createScene(sceneDesc);
//
//    physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
//    if (pvdClient)
//    {
//        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
//        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
//        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
//    }
//
//}
//
//void CPhysics::create_simulation()
//{
//    // create simulation
//    mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
//    physx::PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, physx::PxPlane(0, 1, 0, 50), *mMaterial);
//    mScene->addActor(*groundPlane);
//
//    PxShape* pShape = mPhysics->createShape(PxBoxGeometry(2.f, 0.2f, 0.1f), *mMaterial, true);
//    pShape->setFlag(PxShapeFlag::eVISUALIZATION, true);
//    pShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
//    pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
//
//    PxRigidDynamic* pActor = mPhysics->createRigidDynamic(PxTransform(PxVec3(0.f, 2.5f, 0.f)));
//    PxRigidActorExt::createExclusiveShape(*pActor, PxBoxGeometry(2.f, 0.2f, 0.1f), *mMaterial);
//    PxRigidActorExt::createExclusiveShape(*pActor, PxBoxGeometry(0.2f, 2.f, 0.1f), *mMaterial);
//    pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
//    pActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
//    pActor->attachShape(*pShape);
//    mScene->addActor(*pActor);
//    pShape->release();
//}
//
//void CPhysics::run_simulation()
//{
//    // run simulation
//    while (1) {
//        pActor->addForce(PxVec3(1.f, 0.f, 0.f), PxForceMode::eFORCE);
//        pActor->setAngularVelocity(PxVec3(0.f, 0.f, 5.f));
//        mScene->simulate(1.0f / 60.0f);
//        cout << pActor->getGlobalPose().p.x << ", "
//            << pActor->getGlobalPose().p.y << ", "
//            << pActor->getGlobalPose().p.z << endl;
//
//        while (!mScene->fetchResults(false))
//        {
//            cout << "!" << endl;
//        }
//    }
//}