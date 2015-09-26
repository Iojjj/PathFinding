//
// Created by Alexander Vlasov on 21.09.2015.
//
#include "test_pathfinding_Dijkstra.hpp"
#include "Log.hpp"
#include <list>
#include <cfloat>
#include "Vertex.hpp"
#include "Dijkstra.hpp"

struct ParsedData {
    std::vector<Vertex *> vertices;
    unsigned int rows, columns;
    unsigned long floorId;
};

static ParsedData parseData(JNIEnv *env, jlong floorId, jobjectArray array) {
    unsigned int i, j, rows, columns;
    jbyteArray row;
    rows = (unsigned int) env->GetArrayLength(array);
    row = (jbyteArray) env->GetObjectArrayElement(array, 0);
    columns = (unsigned int) env->GetArrayLength(row);
    env->DeleteLocalRef(row);
    unsigned int matrixSize = rows * columns;
    unsigned long floor = (unsigned long) floorId;
    std::vector<Vertex *> vertices(matrixSize);
    jbyte *waypoints;
    for (i = 0; i < rows; i++) {
        row = (jbyteArray) env->GetObjectArrayElement(array, i);
        waypoints = env->GetByteArrayElements(row, JNI_FALSE);
        for (j = 0; j < columns; j++) {
            if (waypoints[j] != 0) {
                vertices[i * columns + j] = nullptr;
            } else {
                Vertex *vertex = new Vertex();
                vertex->SetPosition(i, j, floor);
                vertex->SetDistance(FLT_MAX);
                vertices[i * columns + j] = vertex;
            }
        }
        env->ReleaseByteArrayElements(row, waypoints, JNI_ABORT);
        env->DeleteLocalRef(row);
    }
    env->DeleteLocalRef(array);
    ParsedData data;
    data.rows = rows;
    data.columns = columns;
    data.vertices = vertices;
    data.floorId = floor;
    return data;
}

JNIEXPORT jlong JNICALL Java_test_pathfinding_Dijkstra_newObject
        (JNIEnv *env, jobject obj) {
    LOGD("Dijkstra", "new object");
    return (long) (new Dijkstra());
}

JNIEXPORT void JNICALL Java_test_pathfinding_Dijkstra_deleteObject
        (JNIEnv *env, jobject obj, jlong ptr) {
    LOGD("Dijkstra", "delete object");
    Dijkstra *dijkstra = (Dijkstra *) ptr;
    delete dijkstra;
}

JNIEXPORT void JNICALL Java_test_pathfinding_Dijkstra_addFloor(JNIEnv *env, jobject obj, jlong ptr,
                                                               jlong floorId,
                                                               jobjectArray waypoints,
                                                               jfloat costStraight,
                                                               jfloat costDiagonal) {
    LOGD("Dijkstra", "add floor");
    Dijkstra *dijkstra = (Dijkstra *) ptr;
    ParsedData data = parseData(env, floorId, waypoints);
    dijkstra->AddFloor(data.floorId, data.rows, data.columns, &data.vertices, costStraight,
                       costDiagonal);
}

JNIEXPORT void JNICALL Java_test_pathfinding_Dijkstra_createTransits(JNIEnv *env,
                                                                             jobject obj,
                                                                             jlong ptr,
                                                                             jobjectArray transits,
                                                                             jfloat costTransit) {
    Dijkstra *dijkstra = (Dijkstra *) ptr;
    unsigned int count = (unsigned int) env->GetArrayLength(transits);
    jobject transit;
    jclass clz;
    jmethodID method;
    std::list<Transit *> list;
    LOGD("Dijkstra", "create transits: %d", count);
    unsigned long floorA, floorB;
    unsigned int rowA, rowB, colA, colB;
    for (int i=0; i<count; i++) {
        transit = env->GetObjectArrayElement(transits, i);
        clz = env->GetObjectClass(transit);
        method = env->GetMethodID(clz, "getFromFloor", "()J");
        floorA = (unsigned long) env->CallLongMethod(transit, method);
        LOGD("Dijkstra", "floor a: %lu", floorA);
        method = env->GetMethodID(clz, "getToFloor", "()J");
        floorB = (unsigned long) env->CallLongMethod(transit, method);
        LOGD("Dijkstra", "floor b: %lu", floorB);
        method = env->GetMethodID(clz, "getFromRow", "()I");
        rowA = (unsigned int) env->CallIntMethod(transit, method);
        LOGD("Dijkstra", "row a: %d", rowA);
        method = env->GetMethodID(clz, "getFromCol", "()I");
        colA = (unsigned int) env->CallIntMethod(transit, method);
        LOGD("Dijkstra", "col a: %d", colA);
        method = env->GetMethodID(clz, "getToRow", "()I");
        rowB = (unsigned int) env->CallIntMethod(transit, method);
        LOGD("Dijkstra", "row b: %d", rowB);
        method = env->GetMethodID(clz, "getToCol", "()I");
        colB = (unsigned int) env->CallIntMethod(transit, method);
        LOGD("Dijkstra", "col b: %d", colB);
        env->DeleteLocalRef(transit);
        list.push_back(new Transit(floorA, rowA, colA, floorB, rowB, colB));
    }
    env->DeleteLocalRef(transits);
    dijkstra->AddTransits(list, costTransit);
}


JNIEXPORT jobjectArray JNICALL Java_test_pathfinding_Dijkstra_findPath(JNIEnv *env, jobject obj,
                                                                       jlong ptr, jlong fromFloor,
                                                                       jint fromRow, jint fromCol,
                                                                       jlong toFloor, jint toRow,
                                                                       jint toCol) {
    LOGD("Dijkstra", "find path");
    Dijkstra *dijkstra = (Dijkstra *) ptr;
    LOGD("Dijkstra", "find path 1");
    Transit *transit = new Transit((unsigned long) fromFloor,
                                   (unsigned int) fromRow,
                                   (unsigned int) fromCol,
                                   (unsigned long) toFloor,
                                   (unsigned int) toRow,
                                   (unsigned int) toCol);
    LOGD("Dijkstra", "find path 2");
    std::list<unsigned int *> list = dijkstra->FindPath(transit);
    LOGD("Dijkstra", "find path 3");
    jobjectArray route;
    jintArray routeRow;
    jint *values;
    unsigned int *val;
    jclass intClass = env->FindClass("[I");
    route = env->NewObjectArray(list.size(), intClass, NULL);
    int i = 0;
    for (std::list<unsigned int *>::iterator it = list.begin(); it != list.end(); it++, i++) {
        val = *it;
        routeRow = env->NewIntArray(3);
        values = new jint[3]{val[0], val[1], val[2]};
        env->SetIntArrayRegion(routeRow, 0, 3, values);
        env->SetObjectArrayElement(route, i, routeRow);
        env->ReleaseIntArrayElements(routeRow, values, JNI_ABORT);
        env->DeleteLocalRef(routeRow);
    }
    return route;
}



