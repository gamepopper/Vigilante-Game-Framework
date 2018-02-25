/**
* @file    VRange.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* Two data structures that simply hold more than one of a variable for representing a range. It's main use is for the VEmitter.
*/

#pragma once

///Class that holds two of the same variable for min/max range.
template <typename T>
class VRange
{
public:
	///Smallest value in range.
	T A;
	///Largest value in range.
	T B;

	VRange() = default;
	///@param value The value to set both ranges to. (Seems almost unnecessary for a range but it saves time setting one value to both manually).
	VRange(T value) : 	A(value),
						B(value)
	{
		
	}

	/**
	* @param min The lowest value in the range.
	* @param max The highest value in the range.
	*/
	VRange(T min, T max) : 	A(min),
							B(max)
	{
		
	}
	~VRange() = default;

	/**
	* @param range The other VRange.
	* @return Returns true if all ranges match.
	*/
	bool operator==(VRange<T> range)
	{
		return A == range.A && B == range.B;
	}

	/**
	* @param range The other VRange.
	* @return Returns true if the range doesn't match.
	*/
	bool operator!=(VRange<T> range)
	{
		return !(*this == range);
	}
};

///Class that holds four of the same object for min/max range. Best use this when storing objects with at least two properties (i.e. 2D vectors)
template <typename T>
class VRangeBounds
{
public:
	///Smallest range bounderies.
	VRange<T> A;
	///Largest range bounderies.
	VRange<T> B;

	VRangeBounds() = default;
	///@param val The value to set both range bounds to. (Seems almost unnecessary for a range but it saves time setting one value to both manually).
	VRangeBounds(T val) :	A(val),
							B(val)
	{

	}

	/**
	* @param AA The lowest value in the range bound A.
	* @param AB The highest value in the range bounds A.
	* @param BA The lowest value in the range bound B.
	* @param BB The highest value in the range bounds B.
	*/
	VRangeBounds(T AA, T AB, T BA, T BB) : 	A(AA,AB),
											B(BA,BB)
	{
		
	}

	/**
	* @param min The lowest value in the range bounds.
	* @param max The highest value in the range bounds.
	*/
	VRangeBounds(T min, T max) :	A(min, min),
									B(max, max)
	{
		
	}

	/**
	* @param min The lowest value in the range bounds as a VRange.
	* @param max The highest value in the range bounds as a VRange.
	*/
	VRangeBounds(const VRange<T>& min, const VRange<T>& max) :	A(min),
																B(max)
	{
		
	}
	~VRangeBounds() = default;

	/**
	* @param range The other VRangeBound.
	* @return Returns true if all range bounds match.
	*/
	bool operator==(VRangeBounds<T> range)
	{
		return A == range.A && B == range.B;
	}

	/**
	* @param range The other VRangeBound.
	* @return Returns true if a range bound doesn't match.
	*/
	bool operator!=(VRangeBounds<T> range)
	{
		return !(*this == range);
	}
};