#include "Matrix3x3.h"

namespace cool{

	// 1 0 0
	// 0 1 0
	// 0 0 1
	const Matrix3x3 Matrix3x3::identity{ { 1, 0, 0 }, { 0, 1 , 0}, { 0, 0 , 1} };
	const Matrix3x3 Matrix3x3::zero{ { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
}