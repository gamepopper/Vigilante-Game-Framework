#pragma once

template <typename T>
class VRange
{
public:
	bool Active = true;
	T A;
	T B;

	VRange() = default;
	VRange(T value) : 	A(value),
						B(value)
	{
		
	}
	VRange(T min, T max) : 	A(min),
							B(max)
	{
		
	}
	~VRange() = default;

	bool operator==(VRange<T> range)
	{
		return A == range.A && B == range.B;
	}

	bool operator!=(VRange<T> range)
	{
		return !(*this == range);
	}
};

template <typename T>
class VRangeBounds
{
public:
	bool Active = true;
	VRange<T> A;
	VRange<T> B;

	VRangeBounds() = default;
	VRangeBounds(T AA, T AB, T BA, T BB) : 	A(AA,AB),
											B(BA,BB)
	{
		
	}
	VRangeBounds(T min, T max) :	A(min, min),
									B(max, max)
	{
		
	}
	VRangeBounds(const VRange<T>& min, const VRange<T>& max) :	A(min),
																B(max)
	{
		
	}
	~VRangeBounds() = default;

	bool operator==(VRangeBounds<T> range)
	{
		return A == range.A && B == range.B;
	}

	bool operator!=(VRangeBounds<T> range)
	{
		return !(*this == range);
	}
};