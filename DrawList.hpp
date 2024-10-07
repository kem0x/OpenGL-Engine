#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include <execution>
#include "Math.hpp"

struct AABB // Axis-Aligned Bounding Boxes
{
    glm::vec3 Min;
    glm::vec3 Max;
};

struct DrawableObject : std::enable_shared_from_this<DrawableObject>
{
    VertexArrayObject VAO = VertexArrayObject();

    VertexBufferObject VBO = VertexBufferObject();

    ElementBufferObject EBO = ElementBufferObject();

    std::string TypeName;

    std::vector<Vertex> Vertices;

    Location Loc;

    AABB BoundingBox;

    void CalculateAABB()
    {
        if (Vertices.size() == 0)
            return;

        auto MinVertex = Vertices[0].Position;
        auto MaxVertex = Vertices[0].Position;

        for (auto&& Vertex : Vertices)
        {
            MinVertex = glm::min(MinVertex, Vertex.Position);
            MaxVertex = glm::max(MaxVertex, Vertex.Position);
        }

        BoundingBox.Min = MinVertex + Loc.Vec;
        BoundingBox.Max = MaxVertex + Loc.Vec;
    }

    virtual void Draw(std::shared_ptr<ShaderProgram> Shader) = 0;

    void Push();
};

class Drawlist
{
    inline static std::shared_ptr<Drawlist> InternalInstance;

    inline static std::vector<std::shared_ptr<DrawableObject>> Drawables;

public:
    static auto Singleton()
    {
        if (!InternalInstance)
        {
            InternalInstance = std::make_shared<Drawlist>();
        }

        return InternalInstance;
    }

    static void Push(std::shared_ptr<DrawableObject> InDrawable)
    {
        Singleton()->Drawables.emplace_back(InDrawable);
    }

    static void Draw(std::shared_ptr<ShaderProgram> Shader)
    {
        for (auto&& Drawable : Singleton()->Drawables)
        {
            Drawable->Draw(Shader);
        }
    }
};

void DrawableObject::Push()
{
    Drawlist::Push(shared_from_this());
}

template <typename T, typename... ArgsT>
auto SpawnDrawable(Location InWorldLocation, std::shared_ptr<Texture2D> InTexture, ArgsT... Args)
{
    auto D = std::make_shared<T>(InWorldLocation, InTexture, Args...);
    D->Push();

    return D;
}

bool AABBIntersection(AABB BoundingBox, const glm::vec3& RayOrigin, const glm::vec3& RayDirection)
{
    glm::vec3 InvDir = 1.0f / RayDirection;
    glm::vec3 tMin = (BoundingBox.Min - RayOrigin) * InvDir;
    glm::vec3 tMax = (BoundingBox.Max - RayOrigin) * InvDir;

    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

    return tNear <= tFar && tFar >= 0.0f;
}

inline std::vector<std::shared_ptr<struct DrawableObject>> HitScanTargets;

std::vector<std::shared_ptr<struct DrawableObject>> CheckHitScanTargets(const glm::vec3& RayOrigin, const glm::vec3& RayDirection)
{
    std::vector<std::shared_ptr<struct DrawableObject>> HitTargets;

    for (auto&& Target : HitScanTargets)
    {
        if (AABBIntersection(Target->BoundingBox, RayOrigin, RayDirection))
        {
            HitTargets.emplace_back(Target);
        }
    }

    return HitTargets;
}