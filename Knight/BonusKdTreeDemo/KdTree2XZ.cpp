#include "KdTree2XZ.h"


void KdTree2XZ::Build(const std::vector<Vector3>& points)
{
    Clear();
    pts = &points;
    indices.resize((int)pts->size());
    for (int i = 0; i < (int)pts->size(); ++i) indices[i] = i;
    nodes.reserve(pts->size());
    root = BuildRange(0, (int)indices.size(), 0);
}

void KdTree2XZ::RadiusSearchXZ(const Vector3& q, float radius2, std::vector<int>& out) const
{
    out.clear();
    Search(root, q, radius2, out);
}

void KdTree2XZ::Clear()
{
    for (auto* n : nodes)
        delete n;
    nodes.clear();
    indices.clear();
    root = nullptr;
    pts = nullptr;
}

KdNode2* KdTree2XZ::MakeNode()
{
    nodes.push_back(new KdNode2());
    return nodes.back();
}

KdNode2* KdTree2XZ::BuildRange(int l, int r, int depth)
{
    if (l >= r)
        return nullptr;

    int axis = depth & 1;
    int m = (l + r) / 2;
    std::nth_element(indices.begin() + l, indices.begin() + m, indices.begin() + r, 
        [&](int a, int b) { 
            return coordXZ(a, axis) < coordXZ(b, axis); 
        });

    KdNode2* node = MakeNode();
    node->index = indices[m];
    node->axis = axis;
    node->left = BuildRange(l, m, depth + 1);
    node->right = BuildRange(m + 1, r, depth + 1);

    return node;
}

void KdTree2XZ::Search(KdNode2* node, const Vector3& q, float radius2, std::vector<int>& out) const
{
    if (!node)
        return;

    const Vector3& p = (*pts)[node->index];
    if (Dist2XZ(q, p) <= radius2)
        out.push_back(node->index);

    float qv = (node->axis == 0) ? q.x : q.z;
    float pv = (node->axis == 0) ? p.x : p.z;
    KdNode2* first = (qv < pv) ? node->left : node->right;
    KdNode2* second = (qv < pv) ? node->right : node->left;

    Search(first, q, radius2, out);

    float planeDist2 = (qv - pv) * (qv - pv);
    if (planeDist2 <= radius2)
        Search(second, q, radius2, out);
}
