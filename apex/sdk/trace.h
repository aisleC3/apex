#pragma once

struct Ray_t
{
	Vector m_Start; //0x0000
	char pad_000C[4]; //0x000C
	Vector m_Delta; //0x0010
	char pad_001C[64]; //0x001C
	bool m_IsRay; //0x005C
	bool m_IsSwept; //0x005D
	char pad_005E[130]; //0x005E

	void Init(Vector start, Vector end)
	{
		m_Start = start;
		m_Delta = end - start;
		m_IsRay = true;
		m_IsSwept = true;
	}
}; //Size: 0x00E0


struct trace_t
{
	Vector start; //0x0000
	char pad_000C[4]; //0x000C
	Vector end; //0x0010
	char pad_001C[4]; //0x001C
	float normal; //0x0020
	char pad_0024[8]; //0x0024
	float distance; //0x002C
	float fraction; //0x0030
	char pad_0034[12]; //0x0034
	char* name; //0x0040
	char pad_0048[24]; //0x0048
	CBaseEntity* ent; //0x0060
	int64_t hitbox; //0x0068
	char pad_0070[224]; //0x0070
}; //Size: 0x0150

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual void			unkn() = 0;
	virtual bool			ShouldHitEntity(void* pEntity, int mask) = 0;
	virtual TraceType_t		GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	void unkn()
	{

	}

	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		CBaseEntity* ent = (CBaseEntity*)pEntityHandle;

		return !(pEntityHandle == pSkip);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

class CEngineTrace
{
public:
	void TraceRay(const Ray_t& ray, unsigned int mask, ITraceFilter* filter, trace_t* trace)
	{
		return getvfunc<void(__thiscall*)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*)>(this, 3)(this, ray, mask, filter, trace);
	}
};