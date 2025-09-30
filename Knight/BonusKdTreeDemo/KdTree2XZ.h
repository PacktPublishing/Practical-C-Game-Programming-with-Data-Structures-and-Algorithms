#pragma once

#include <vector>
#include <algorithm>

#include "raylib.h"

using namespace std;

//2D k-d tree (x/z) structure 
struct KdNode2 
{
    int index;
    int axis; // 0=x, 1=z
    KdNode2* left;
    KdNode2* right;
};

static inline float Dist2XZ(const Vector3& a, const Vector3& b) 
{
    float dx = a.x - b.x, dz = a.z - b.z;
    return dx * dx + dz * dz;
}

class KdTree2XZ 
{
    public:

        KdTree2XZ() : pts(nullptr), root(nullptr) {}
        KdTree2XZ(const std::vector<Vector3>& points)
        { 
            Build(points); 
        }

        void Build(const std::vector<Vector3>& points);
        void RadiusSearchXZ(const Vector3& q, float radius2, std::vector<int>& out) const;

        ~KdTree2XZ()
        { 
            Clear(); 
        }

    private:
        const vector<Vector3>* pts = nullptr;
        vector<int> indices;
        vector<KdNode2*> nodes;
        KdNode2* root = nullptr;

        void Clear();

        KdNode2* MakeNode();

        inline float coordXZ(int idx, int axis) const 
        {
            const Vector3& p = (*pts)[idx];
            return (axis == 0) ? p.x : p.z;
        }

        KdNode2* BuildRange(int l, int r, int depth);

        void Search(KdNode2* node, const Vector3& q, float radius2, std::vector<int>& out) const;

};

//end of file KdTree2XZ.h