#include <jni.h>

#ifndef _Included_test_pathfinding_Dijkstra
#define _Included_test_pathfinding_Dijkstra
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_test_pathfinding_Dijkstra_newObject
        (JNIEnv *, jobject);

JNIEXPORT void JNICALL Java_test_pathfinding_Dijkstra_deleteObject
        (JNIEnv *, jobject, jlong);

JNIEXPORT void JNICALL Java_test_pathfinding_Dijkstra_addFloor(JNIEnv *env, jobject obj, jlong ptr,
                                                               jlong floorId,
                                                               jobjectArray waypoints,
                                                               jfloat costStraight,
                                                               jfloat costDiagonal);

JNIEXPORT void JNICALL Java_test_pathfinding_Dijkstra_createTransits(JNIEnv *env, jobject obj,
                                                                             jlong ptr,
                                                                             jobjectArray transits,
                                                                             jfloat costTransit);

JNIEXPORT jobjectArray JNICALL Java_test_pathfinding_Dijkstra_findPath(JNIEnv *env, jobject obj,
                                                                       jlong ptr, jlong fromFloor,
                                                                       jint fromRow, jint fromCol,
                                                                       jlong toFloor, jint toRow,
                                                                       jint toCol);


#ifdef __cplusplus
}
#endif
#endif
