#pragma once
#include<type_traits>
/// <summary>
/// ����؂�邩�ǂ�����Ԃ�
/// </summary>
/// <typeparam name="T">�����^�Ɍ���</typeparam>
/// <param name="val">�`�F�b�N�������l</param>
/// <param name="div">���W�����l</param>
/// <returns>true:����؂�� false:����؂�Ȃ�</returns>
template<typename T>
inline bool IsDivisible(const T& val, const T& div) {
	static_assert(std::is_integral<T>::value,"�����^�Ɍ���");
	return val % div == 0;
}

/// <summary>
/// div��@�Ƃ�����]�C���N�������g
/// �C���N�������g�̌��ʂ�������modulo�ȏ�ɂȂ����Ƃ��܂�0�ɖ߂�
/// �i���ɉ�葱����
/// </summary>
/// <typeparam name="T">�����^�Ɍ���</typeparam>
/// <param name="value">���݂̒l</param>
/// <param name="modulo">���W�����l</param>
/// <returns>��]���Z���s�����l</returns>
template<typename T>
inline T ModuloIncrement(const T& value, const T& modulo) {
	static_assert(std::is_integral<T>::value);
	return (value + 1) % modulo;
}
/// <summary>
/// div��@�Ƃ�����]�f�N�������g
/// �f�N�������g�̌��ʂ�0�����ɂȂ�Ȃ��悤�ɁA-1�ɂȂ肻���ȏꍇ�A�ő�l�ɂȂ�
/// �i���ɉ�葱����
/// </summary>
/// <typeparam name="T">�����^�Ɍ���</typeparam>
/// <param name="value">���݂̒l</param>
/// <param name="modulo">���W�����l</param>
/// <returns>��]���Z���s�����l</returns>
template<typename T>
inline T ModuloDecrement(const T& value, const T& modulo) {
	static_assert(std::is_integral<T>::value);
	return (value + modulo - 1) % modulo;
}

template<typename T>
inline bool InRange(const T& value, const T& minValue, const T& maxValue) {
	return minValue <= value && value <= maxValue;
}

///<summary>
///���������𔲂��o���ĕԂ�
///�������A0�̎���1.0���Ԃ�
///</summary>
/// <param name="value">�l</param>
/// <returns>����</returns>
extern float Sign(float value);