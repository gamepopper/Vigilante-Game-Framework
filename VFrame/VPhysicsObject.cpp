#include "VPhysicsObject.h"

#ifndef VFRAME_NO_PHYSICS
#include "VObject.h"
#include <chipmunk/chipmunk.h>

cpVect ToCPVect(const sf::Vector2f& v)
{
	cpVect out;
	out.x = v.x;
	out.y = v.y;
	return out;
}

sf::Vector2f ToSFVector(const cpVect& v)
{
	return sf::Vector2f((float)v.x, (float)v.y);
}

VPhysicsObject::VPhysicsObject(VObject* Object, VObjectType BodyType, VObjectShape Shape, std::vector<sf::Vector2f> Verts) : VBase(), shapeType(Shape), bodyType(BodyType)
{
	baseObject = Object;

	float moment = 0.0f;

	switch (BodyType)
	{
	case VObjectType::DYNAMIC:
		body = cpBodyNew(0.0f, 0.0f);
		break;
	case VObjectType::KINEMATIC:
		body = cpBodyNewKinematic();
		break;
	case VObjectType::STATIC:
		body = cpBodyNewStatic();
	}

	switch (Shape)
	{
		default:
			break;
		case VObjectShape::BOX:
		{
			moment = (float)cpMomentForBox((cpFloat)Object->Mass, (cpFloat)Object->Size.x, (cpFloat)Object->Size.y);
			shape = cpBoxShapeNew(body, Object->Size.x, Object->Size.y, 0.0f);
			break;
		}
		case VObjectShape::CIRCLE:
		{
			cpVect offset = cpVect();
			//offset.x = Object->Radius;
			//offset.y = Object->Radius;
			moment = (float)cpMomentForCircle((cpFloat)Object->Mass, (cpFloat)0, (cpFloat)Object->Radius, offset);
			shape = cpCircleShapeNew(body, Object->Radius, offset);
			break;
		}
		case VObjectShape::LINE:
		{
			cpVect a = ToCPVect(Verts[0] - (Object->Size / 2.0f));
			cpVect b = ToCPVect(Verts[1] - (Object->Size / 2.0f));

			moment = (float)cpMomentForSegment((cpFloat)Object->Mass, a, b, (cpFloat)0);
			shape = cpSegmentShapeNew(body, a, b, 0.0f);
			break;
		}
		case VObjectShape::CUSTOM:
		{
			getPolyVertCount = Verts.size();
			std::vector<cpVect> verts(getPolyVertCount);
			for (unsigned int i = 0; i < verts.size(); i++)
			{
				verts[i] = ToCPVect(Verts[i]);
			}

			moment = (float)cpMomentForPoly((cpFloat)Object->Mass, verts.size(), verts.data(), cpVect(), (cpFloat)0);
			shape = cpPolyShapeNew(body, verts.size(), verts.data(), cpTransformIdentity, 0.0f);
			break;
		}
	}

	if (BodyType == VObjectType::DYNAMIC)
	{
		cpBodySetMoment(body, moment);
		cpBodySetMass(body, Object->Mass);
	}

	cpBodySetPosition(body, ToCPVect(Object->Position + (Object->Size / 2.0f)));

	if (Shape != VObjectShape::LINE)
		cpBodySetAngle(body, Object->Angle * (3.1415926f / 180.0f));
}

VPhysicsCPBody* VPhysicsObject::GetBody()
{
	return body;
}

VPhysicsCPShape* VPhysicsObject::GetShape()
{
	return shape;
}

//void VPhysicsObject::SetShapeType(VObjectShape shape)
//{
//	if (shape != shapeType)
//	{
//
//	}
//}

VPhysicsObject::VObjectShape VPhysicsObject::GetShapeType()
{
	return shapeType;
}

void VPhysicsObject::SetBodyType(VObjectType newBodyType)
{
	if (newBodyType != bodyType)
	{
		cpBodySetType(body, (cpBodyType)newBodyType);
		bodyType = newBodyType;
	}
}

VPhysicsObject::VObjectType VPhysicsObject::GetBodyType()
{
	return bodyType;
}

void VPhysicsObject::Initialise(VPhysicsCPSpace* space)
{
	cpSpaceAddBody(space, body);
	cpSpaceAddShape(space, shape);
}

VObject* VPhysicsObject::GetBaseObject()
{
	return baseObject;
}

void VPhysicsObject::Deinitialise(VPhysicsCPSpace* space)
{
	cpSpaceRemoveBody(space, body);
	cpSpaceRemoveShape(space, shape);
}

void VPhysicsObject::Destroy()
{
	VSUPERCLASS::Destroy();

	baseObject = nullptr;
	cpShapeFree(shape);
	cpBodyFree(body);
}

void VPhysicsObject::Update(float dt)
{
	baseObject->Moves = false;

	bool sleep = cpBodyIsSleeping(body);
	cpVect pos = cpBodyGetPosition(body);
	cpVect vel = cpBodyGetVelocity(body);

	if (cpBodyGetType(body) == CP_BODY_TYPE_KINEMATIC)
	{
		pos.x = baseObject->Position.x + (baseObject->Size.x / 2.0f);
		vel.x = baseObject->Velocity.x;
	}

	if (cpBodyGetType(body) == CP_BODY_TYPE_KINEMATIC)
	{
		pos.y = baseObject->Position.y + (baseObject->Size.y / 2.0f);
		vel.y = baseObject->Velocity.y;
	}

	baseObject->SetPositionAtCentre(ToSFVector(pos));
	baseObject->Velocity = ToSFVector(vel);

	cpBodySetPosition(body, pos);
	cpBodySetVelocity(body, vel);

	if (shapeType != VObjectShape::LINE)
	{
		float angle		= (float)cpBodyGetAngle(body);
		float angleVel	= (float)cpBodyGetAngularVelocity(body);

		if (cpBodyGetType(body) == CP_BODY_TYPE_KINEMATIC)
		{
			angle = baseObject->Angle * (3.1415926f / 180.0f);
			angleVel = baseObject->AngleVelocity * (3.1415926f / 180.0f);
		}

		baseObject->Angle = angle * (180.0f / 3.1415926f);
		baseObject->AngleVelocity = angleVel * (180.0f / 3.1415926f);

		cpBodySetAngle(body, angle);
		cpBodySetAngularVelocity(body, angleVel);
	}

	if (cpBodyGetType(body) == CP_BODY_TYPE_DYNAMIC)
	{
		float moment = 0.0f;
		switch (shapeType)
		{
			case VObjectShape::BOX:
			{
				moment = (float)cpMomentForBox((cpFloat)baseObject->Mass, baseObject->Size.x, baseObject->Size.y);
				break;
			}
			case VObjectShape::CIRCLE:
			{
				cpVect offset = cpVect();
				//offset.x = Object->Radius;
				//offset.y = Object->Radius;
				moment = (float)cpMomentForCircle((cpFloat)baseObject->Mass, (cpFloat)0, (cpFloat)baseObject->Radius, offset);
				break;
			}
			case VObjectShape::LINE:
			{
				moment = (float)cpMomentForSegment((cpFloat)baseObject->Mass, cpPolyShapeGetVert(shape, 0), cpPolyShapeGetVert(shape, 1), (cpFloat)0);
				break;
			}
			case VObjectShape::CUSTOM:
			{
				std::vector<cpVect> verts(getPolyVertCount);
				for (unsigned int i = 0; i < getPolyVertCount; i++)
				{
					verts[i] = cpPolyShapeGetVert(shape, i);
				}

				moment = (float)cpMomentForPoly((cpFloat)baseObject->Mass, verts.size(), verts.data(), cpVect(), (cpFloat)0);
				break;
			}
		}

		cpBodySetMoment(body, moment);
		cpBodySetMass(body, baseObject->Mass);
		cpShapeSetMass(shape, baseObject->Mass);
	}

	cpShapeSetElasticity(shape, baseObject->Elasticity);
	
	if (sleep)
		cpBodySleep(body);
}

float VPhysicsObject::GetDensity()
{
	return (float)cpShapeGetDensity(shape);
}

void VPhysicsObject::SetDensity(float density)
{
	cpShapeSetDensity(shape, density);
}

float VPhysicsObject::GetFriction()
{
	return (float)cpShapeGetFriction(shape);
}

void VPhysicsObject::SetFriction(float friction)
{
	cpShapeSetFriction(shape, friction);
}

sf::Vector2f VPhysicsObject::GetSurfaceVelocity()
{
	return ToSFVector(cpShapeGetSurfaceVelocity(shape));
}

void VPhysicsObject::SetSurfaceVelocity(sf::Vector2f surfaceVelocity)
{
	cpVect vel;
	vel.x = surfaceVelocity.x;
	vel.y = surfaceVelocity.y;
	cpShapeSetSurfaceVelocity(shape, vel);
}

sf::Vector2f VPhysicsObject::GetForce()
{
	return ToSFVector(cpBodyGetForce(body));
}

void VPhysicsObject::SetForce(sf::Vector2f force)
{
	cpVect f;
	f.x = force.x;
	f.y = force.y;
	cpBodySetForce(body, f);
}

float VPhysicsObject::GetTorque()
{
	return (float)cpBodyGetTorque(body);
}

void VPhysicsObject::SetTorque(float torque)
{
	cpBodySetTorque(body, torque);
}

void VPhysicsObject::ApplyForceAtWorldPoint(sf::Vector2f force, sf::Vector2f point)
{
	cpBodyApplyForceAtWorldPoint(body, ToCPVect(force), ToCPVect(point));
}

void VPhysicsObject::ApplyForceAtLocalPoint(sf::Vector2f force, sf::Vector2f point)
{
	cpBodyApplyForceAtLocalPoint(body, ToCPVect(force), ToCPVect(point));
}

void VPhysicsObject::ApplyImpulseAtWorldPoint(sf::Vector2f impulse, sf::Vector2f point)
{
	cpBodyApplyImpulseAtWorldPoint(body, ToCPVect(impulse), ToCPVect(point));
}

void VPhysicsObject::ApplyImpulseAtLocalPoint(sf::Vector2f impulse, sf::Vector2f point)
{
	cpBodyApplyImpulseAtLocalPoint(body, ToCPVect(impulse), ToCPVect(point));
}
#endif