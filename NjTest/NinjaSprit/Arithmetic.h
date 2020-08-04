#pragma once
#include<type_traits>
/// <summary>
/// ����؂�邩�ǂ�����Ԃ�
/// </summary>
/// <typeparam name="T">�����^�Ɍ���</typeparam>
/// <param name="v">�`�F�b�N�������l</param>
/// <param name="div">���W�����l</param>
/// <returns>true:����؂��</returns>
template<typename T>
inline bool IsDivisible(const T& v, const T& div) {
	static_assert(std::is_integral<T>::value);
	return v % div == 0;
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

extern float Sign(float value);