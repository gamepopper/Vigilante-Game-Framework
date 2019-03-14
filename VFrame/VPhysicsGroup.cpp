#include "VPhysicsGroup.h"

#ifndef VFRAME_NO_PHYSICS
#include <chipmunk/chipmunk.h>
#include "VObject.h"
#include "VGlobal.h"

struct CollisionCallbackHelper
{
	VObject* A;
	VObject* B;
	bool Persist;
	VPhysicsGroup::VPhysicsCallbackType Type;
	std::function<bool(VPhysicsObject*, VPhysicsObject*)> CallbackFunc;

	CollisionCallbackHelper(VObject* a, VObject* b, const std::function<bool(VPhysicsObject*, VPhysicsObject*)>& callback, VPhysicsGroup::VPhysicsCallbackType type, bool persist = false)
	{
		A = a;
		B = b;
		Persist = persist;
		Type = type;
		CallbackFunc = callback;
	}
};

VPhysicsGroup::VPhysicsGroup(unsigned int Amount) : VGroup(Amount)
{
	space = cpSpaceNew();
	collisionHandler = cpSpaceAddDefaultCollisionHandler(space);
	collisionHandler->userData = (void*)this;
	collisionHandler->beginFunc = [](cpArbiter *arb, cpSpace *space, void* userData)
	{
		VPhysicsGroup* group = (VPhysicsGroup*)(userData);
		return (unsigned char)(group->ProcessCallback(arb, space, VPhysicsCallbackType::BEGIN) ? 1 : 0);
	};
	collisionHandler->preSolveFunc = [](cpArbiter *arb, cpSpace *space, void* userData)
	{
		VPhysicsGroup* group = (VPhysicsGroup*)(userData);
		return (unsigned char)(group->ProcessCallback(arb, space, VPhysicsCallbackType::PRESOLVE) ? 1 : 0);
	};
	collisionHandler->postSolveFunc = [](cpArbiter *arb, cpSpace *space, void* userData)
	{
		VPhysicsGroup* group = (VPhysicsGroup*)(userData);
		group->ProcessCallback(arb, space, VPhysicsCallbackType::POSTSOLVE);
	};
	collisionHandler->separateFunc = [](cpArbiter *arb, cpSpace *space, void* userData)
	{
		VPhysicsGroup* group = (VPhysicsGroup*)(userData);
		group->ProcessCallback(arb, space, VPhysicsCallbackType::SEPARATE);
	};

	callbackHelperList.reserve(5);
}

VPhysicsObject* VPhysicsGroup::AddObject(VObject* Object, VPhysicsObject::VObjectType BodyType, VPhysicsObject::VObjectShape Shape, std::vector<sf::Vector2f> Verts)
{
	VPhysicsObject* obj = new VPhysicsObject(Object, BodyType, Shape, Verts);
	if (Add(obj))
	{
		obj->Initialise(space);
		return obj;
	}

	delete obj;
	return nullptr;
}

bool VPhysicsGroup::RemoveObject(VObject* Object)
{
	VPhysicsObject* obj = nullptr;
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VPhysicsObject* cur = dynamic_cast<VPhysicsObject*>(members[i]);
		if (cur->GetBaseObject() == Object)
		{
			obj = cur;
			break;
		}
	}

	if (Remove(obj))
	{
		obj->Deinitialise(space);
		delete obj;
		return true;
	}

	return false;
}

void VPhysicsGroup::Destroy()
{
	callbackHelperList.clear();
	cpSpaceFree(space);
	VSUPERCLASS::Destroy();
}

void VPhysicsGroup::Update(float dt)
{
	timestep += dt;
	float fixedTS = 1.0f / VGlobal::p()->FPS;

	while (timestep > fixedTS)
	{
		VSUPERCLASS::Update(dt);
		cpSpaceStep(space, dt);

		/*for (int i = 0; i < (int)callbackHelperList.size(); i++)
		{
			if (!callbackHelperList[i].Persist)
			{
				callbackHelperList.erase(callbackHelperList.begin() + i);
				i--;
			}
		}*/

		timestep -= fixedTS;
	}
}

VPhysicsObject* VPhysicsGroup::getObjectFromBody(cpBody* body)
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		if ((cpBody*)dynamic_cast<VPhysicsObject*>(members[i])->GetBody() == body)
		{
			return dynamic_cast<VPhysicsObject*>(members[i]);
		}
	}

	return nullptr;
}

void VPhysicsGroup::SetCollisionCallback(VObject* a, VObject* b, const std::function<bool(VPhysicsObject*, VPhysicsObject*)>& callback, VPhysicsCallbackType type, bool persist)
{
	callbackHelperList.emplace_back(a, b, callback, type, persist);
}

bool VPhysicsGroup::ProcessCallback(cpArbiter *arb, cpSpace *space, VPhysicsCallbackType type)
{
	if (callbackHelperList.size() == 0)
		return true;

	cpBody* bodyA;
	cpBody* bodyB;

	cpArbiterGetBodies(arb, &bodyA, &bodyB);

	VPhysicsObject* physicsA = getObjectFromBody(bodyA);
	physicsA->Update(0.0f);
	VPhysicsObject* physicsB = getObjectFromBody(bodyB);
	physicsB->Update(0.0f);

	/*if (type == VPhysicsCallbackType::PRESOLVE)
	{
		cpShape* shapeA;
		cpShape* shapeB;
		cpArbiterGetShapes(arb, &shapeA, &shapeB);

		cpFloat elasticityA = cpShapeGetElasticity(shapeA);
		cpFloat elasticityB = cpShapeGetElasticity(shapeB);

		elasticityA = elasticityB = (elasticityA + elasticityB) / 2.0f;

		cpShapeSetElasticity(shapeA, elasticityA);
		cpShapeSetElasticity(shapeB, elasticityB);
	}
	else if (type == VPhysicsCallbackType::POSTSOLVE)
	{
		cpShape* shapeA;
		cpShape* shapeB;
		cpArbiterGetShapes(arb, &shapeA, &shapeB);

		float elasticityA = physicsA->GetBaseObject()->Elasticity;
		float elasticityB = physicsB->GetBaseObject()->Elasticity;

		cpShapeSetElasticity(shapeA, elasticityA);
		cpShapeSetElasticity(shapeB, elasticityB);
	}*/

	bool result = true;
	for (int i = 0; i < (int)callbackHelperList.size(); i++)
	{
		if (callbackHelperList[i].Type != type)
			continue;

		if (callbackHelperList[i].A == physicsA->GetBaseObject() &&
			(callbackHelperList[i].B == physicsB->GetBaseObject() ||
				callbackHelperList[i].B == nullptr))
		{
			result = callbackHelperList[i].CallbackFunc(physicsA, physicsB);

			if (!callbackHelperList[i].Persist)
			{
				callbackHelperList.erase(callbackHelperList.begin() + i);
				i--;
			}

			break;
		}

		if ((callbackHelperList[i].B == physicsA->GetBaseObject() ||
			callbackHelperList[i].B == nullptr) &&
			callbackHelperList[i].A == physicsB->GetBaseObject())
		{
			result = callbackHelperList[i].CallbackFunc(physicsB, physicsA);

			if (!callbackHelperList[i].Persist)
			{
				callbackHelperList.erase(callbackHelperList.begin() + i);
				i--;
			}

			break;
		}
	}

	return result;
}

int VPhysicsGroup::GetIterations()
{
	return cpSpaceGetIterations(space);
}

void VPhysicsGroup::SetIterations(int iterations)
{
	cpSpaceSetIterations(space, iterations);
}

sf::Vector2f VPhysicsGroup::GetGravity()
{
	return ToSFVector(cpSpaceGetGravity(space));
}

void VPhysicsGroup::SetGravity(sf::Vector2f gravity)
{
	cpVect g;
	g.x = gravity.x;
	g.y = gravity.y;
	cpSpaceSetGravity(space, g);
}

float VPhysicsGroup::GetDamping()
{
	return (float)cpSpaceGetDamping(space);
}

void VPhysicsGroup::SetDamping(float damping)
{
	cpSpaceSetDamping(space, damping);
}

float VPhysicsGroup::GetIdleSpeedThreshold()
{
	return (float)cpSpaceGetIdleSpeedThreshold(space);
}

void VPhysicsGroup::SetIdleSpeedThreshold(float idleSpeedThreshold)
{
	cpSpaceSetIdleSpeedThreshold(space, idleSpeedThreshold);
}

float VPhysicsGroup::GetSleepTimeThreshold()
{
	return (float)cpSpaceGetSleepTimeThreshold(space);
}

void VPhysicsGroup::SetSleepTimeThreshold(float sleepTimeThreshold)
{
	cpSpaceSetSleepTimeThreshold(space, sleepTimeThreshold);
}

float VPhysicsGroup::GetCollisionSlop()
{
	return (float)cpSpaceGetCollisionSlop(space);
}

void VPhysicsGroup::SetCollisionSlop(float collisionSlop)
{
	cpSpaceSetCollisionSlop(space, collisionSlop);
}

float VPhysicsGroup::GetCollisionBias()
{
	return (float)cpSpaceGetCollisionBias(space);
}

void VPhysicsGroup::SetCollisionBias(float collisionBias)
{
	cpSpaceSetCollisionBias(space, collisionBias);
}

unsigned int VPhysicsGroup::GetCollisionPersistence()
{
	return cpSpaceGetCollisionPersistence(space);
}

void VPhysicsGroup::SetCollisionPersistence(unsigned int collisionPersistence)
{
	cpSpaceSetCollisionPersistence(space, collisionPersistence);
}

#ifdef _DEBUG
sf::VertexArray m_DebugVertexArray;

sf::Color toSFColor(cpSpaceDebugColor color)
{
	return sf::Color(
		static_cast<sf::Uint8>(color.r * 255),
		static_cast<sf::Uint8>(color.g * 255),
		static_cast<sf::Uint8>(color.b * 255),
		static_cast<sf::Uint8>(color.a * 255)
	);
}

void ChipmunkDebugDrawFatSegment(cpVect a, cpVect b, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor)
{
	cpVect n = cpvnormalize(cpvrperp(cpvsub(b, a)));
	cpVect t = cpvrperp(n);

	cpFloat half = 1.0f;
	cpFloat r = radius + half;
	if (r <= half) {
		r = half;
		fillColor = outlineColor;
	}

	cpVect nw = (cpvmult(n, r));
	cpVect tw = (cpvmult(t, r));
	sf::Vector2f v0 = ToSFVector(cpvsub(b, cpvadd(nw, tw))); // { 1.0, -1.0}
	sf::Vector2f v1 = ToSFVector(cpvadd(b, cpvsub(nw, tw))); // { 1.0,  1.0}
	sf::Vector2f v2 = ToSFVector(cpvsub(b, nw)); // { 0.0, -1.0}
	sf::Vector2f v3 = ToSFVector(cpvadd(b, nw)); // { 0.0,  1.0}
	sf::Vector2f v4 = ToSFVector(cpvsub(a, nw)); // { 0.0, -1.0}
	sf::Vector2f v5 = ToSFVector(cpvadd(a, nw)); // { 0.0,  1.0}
	sf::Vector2f v6 = ToSFVector(cpvsub(a, cpvsub(nw, tw))); // {-1.0, -1.0}
	sf::Vector2f v7 = ToSFVector(cpvadd(a, cpvadd(nw, tw))); // {-1.0,  1.0}

	m_DebugVertexArray.append(sf::Vertex(v0, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v1, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v2, toSFColor(outlineColor)));

	m_DebugVertexArray.append(sf::Vertex(v3, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v1, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v2, toSFColor(outlineColor)));

	m_DebugVertexArray.append(sf::Vertex(v3, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v4, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v2, toSFColor(outlineColor)));

	m_DebugVertexArray.append(sf::Vertex(v3, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v4, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v5, toSFColor(outlineColor)));

	m_DebugVertexArray.append(sf::Vertex(v0, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v1, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v2, toSFColor(outlineColor)));

	m_DebugVertexArray.append(sf::Vertex(v6, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v4, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v5, toSFColor(outlineColor)));

	m_DebugVertexArray.append(sf::Vertex(v6, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v7, toSFColor(outlineColor)));
	m_DebugVertexArray.append(sf::Vertex(v5, toSFColor(outlineColor)));
}

void ChipmunkDebugDrawSegment(cpVect a, cpVect b, cpSpaceDebugColor color)
{
	ChipmunkDebugDrawFatSegment(a, b, 0.0f, color, color);
}

void ChipmunkDebugDrawCircle(cpVect pos, cpFloat angle, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor)
{
	cpFloat r = radius + 1.0f;
	int circleDivisions = 16;
	float anglePerDiv = 3.1415926f * (2.0f / circleDivisions);

	for (int i = 0; i < circleDivisions; i++)
	{
		ChipmunkDebugDrawSegment(
			cpvadd(pos, cpvmult(cpvforangle(anglePerDiv * i), r)), 
			cpvadd(pos, cpvmult(cpvforangle(anglePerDiv * (i + 1)), r)), 
			outlineColor);
	}

	ChipmunkDebugDrawSegment(pos, cpvadd(pos, cpvmult(cpvforangle(angle), radius - 0.5f)), outlineColor);
}

void ChipmunkDebugDrawPolygon(int count, const cpVect *verts, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor)
{
	struct ExtrudeVerts { cpVect offset, n; };
	size_t bytes = sizeof(struct ExtrudeVerts)*count;
	struct ExtrudeVerts *extrude = (struct ExtrudeVerts *)alloca(bytes);
	memset(extrude, 0, bytes);

	for (int i = 0; i < count; i++) {
		cpVect v0 = verts[(i - 1 + count) % count];
		cpVect v1 = verts[i];
		cpVect v2 = verts[(i + 1) % count];

		cpVect n1 = cpvnormalize(cpvrperp(cpvsub(v1, v0)));
		cpVect n2 = cpvnormalize(cpvrperp(cpvsub(v2, v1)));

		cpVect offset = cpvmult(cpvadd(n1, n2), 1.0 / (cpvdot(n1, n2) + 1.0f));
		struct ExtrudeVerts v = { offset, n2 }; extrude[i] = v;
	}

	cpFloat inset = -cpfmax(0.0f, 1.0f - radius);
	for (int i = 0; i < count - 2; i++) {
		sf::Vector2f v0 = ToSFVector(cpvadd(verts[0], cpvmult(extrude[0].offset, inset)));
		sf::Vector2f v1 = ToSFVector(cpvadd(verts[i + 1], cpvmult(extrude[i + 1].offset, inset)));
		sf::Vector2f v2 = ToSFVector(cpvadd(verts[i + 2], cpvmult(extrude[i + 2].offset, inset)));

		m_DebugVertexArray.append(sf::Vertex(v0, toSFColor(fillColor)));
		m_DebugVertexArray.append(sf::Vertex(v1, toSFColor(fillColor)));
		m_DebugVertexArray.append(sf::Vertex(v2, toSFColor(fillColor)));
	}

	cpFloat outset = 1.0f + radius - inset;
	for (int i = 0, j = count - 1; i < count; j = i, i++) {
		cpVect vA = verts[i];
		cpVect vB = verts[j];

		cpVect nA = extrude[i].n;
		cpVect nB = extrude[j].n;

		cpVect offsetA = extrude[i].offset;
		cpVect offsetB = extrude[j].offset;

		cpVect innerA = cpvadd(vA, cpvmult(offsetA, inset));
		cpVect innerB = cpvadd(vB, cpvmult(offsetB, inset));

		// Admittedly my variable naming sucks here...
		sf::Vector2f inner0 = ToSFVector(innerA);
		sf::Vector2f inner1 = ToSFVector(innerB);
		sf::Vector2f outer0 = ToSFVector(cpvadd(innerA, cpvmult(nB, outset)));
		sf::Vector2f outer1 = ToSFVector(cpvadd(innerB, cpvmult(nB, outset)));
		sf::Vector2f outer2 = ToSFVector(cpvadd(innerA, cpvmult(offsetA, outset)));
		sf::Vector2f outer3 = ToSFVector(cpvadd(innerA, cpvmult(nA, outset)));

		sf::Vector2f n0 = ToSFVector(nA);
		sf::Vector2f n1 = ToSFVector(nB);
		sf::Vector2f offset0 = ToSFVector(offsetA);

		m_DebugVertexArray.append(sf::Vertex(inner0, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(inner1, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(outer1, toSFColor(outlineColor)));

		m_DebugVertexArray.append(sf::Vertex(inner0, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(outer0, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(outer1, toSFColor(outlineColor)));

		m_DebugVertexArray.append(sf::Vertex(inner0, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(outer0, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(outer2, toSFColor(outlineColor)));

		m_DebugVertexArray.append(sf::Vertex(inner0, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(outer2, toSFColor(outlineColor)));
		m_DebugVertexArray.append(sf::Vertex(outer3, toSFColor(outlineColor)));
	}
}

void ChipmunkDebugDrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor fillColor)
{
	float r = (float)(size*0.5f);
	sf::Vertex a = sf::Vertex(sf::Vector2f((float)pos.x - r, (float)pos.y - r), toSFColor(fillColor));
	sf::Vertex b = sf::Vertex(sf::Vector2f((float)pos.x - r, (float)pos.y + r), toSFColor(fillColor));
	sf::Vertex c = sf::Vertex(sf::Vector2f((float)pos.x + r, (float)pos.y + r), toSFColor(fillColor));
	sf::Vertex d = sf::Vertex(sf::Vector2f((float)pos.x + r, (float)pos.y - r), toSFColor(fillColor));

	m_DebugVertexArray.append(a); m_DebugVertexArray.append(b); m_DebugVertexArray.append(c);
	m_DebugVertexArray.append(a); m_DebugVertexArray.append(c); m_DebugVertexArray.append(d);
}

static void
DrawCircle(cpVect p, cpFloat a, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
	ChipmunkDebugDrawCircle(p, a, r, outline, fill);
}

static void
DrawSegment(cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data)
{
	ChipmunkDebugDrawSegment(a, b, color);
}

static void
DrawFatSegment(cpVect a, cpVect b, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
	ChipmunkDebugDrawFatSegment(a, b, r, outline, fill);
}

static void
DrawPolygon(int count, const cpVect *verts, cpFloat r, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
	ChipmunkDebugDrawPolygon(count, verts, r, outline, fill);
}

static void
DrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor color, cpDataPointer data)
{
	ChipmunkDebugDrawDot(size, pos, color);
}

static inline cpSpaceDebugColor RGBAColor(float r, float g, float b, float a) {
	cpSpaceDebugColor color = { r, g, b, a };
	return color;
}

static inline cpSpaceDebugColor LAColor(float l, float a) {
	cpSpaceDebugColor color = { l, l, l, a };
	return color;
}

static cpSpaceDebugColor
ColorForShape(cpShape *shape, cpDataPointer data)
{
	return RGBAColor(0.0f, 0.0f, 0.0f, 0.0f);
}

#include "VGlobal.h"
void VPhysicsGroup::Draw(sf::RenderTarget& RenderTarget)
{
	if (VGlobal::p()->DrawDebug)
	{
		m_DebugVertexArray.setPrimitiveType(sf::Triangles);
		m_DebugVertexArray.clear();
		cpSpaceDebugDrawOptions drawOptions = {
			DrawCircle,
			DrawSegment,
			DrawFatSegment,
			DrawPolygon,
			DrawDot,

			(cpSpaceDebugDrawFlags)(CP_SPACE_DEBUG_DRAW_SHAPES/* | CP_SPACE_DEBUG_DRAW_CONSTRAINTS | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS*/),

			{0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1.0f},
			ColorForShape,
			{0.0f, 0.75f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			NULL,
		};

		cpSpaceDebugDraw(space, &drawOptions);

		RenderTarget.draw(m_DebugVertexArray);
	}
}
#endif
#endif