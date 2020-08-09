#pragma once
#include<type_traits>
/// <summary>
/// 割り切れるかどうかを返す
/// </summary>
/// <typeparam name="T">整数型に限る</typeparam>
/// <param name="val">チェックしたい値</param>
/// <param name="div">モジュロ値</param>
/// <returns>true:割り切れる false:割り切れない</returns>
template<typename T>
inline bool IsDivisible(const T& val, const T& div) {
	static_assert(std::is_integral<T>::value,"整数型に限る");
	return val % div == 0;
}

/// <summary>
/// divを法とした剰余インクリメント
/// インクリメントの結果が第二引数modulo以上になったときまた0に戻り
/// 永遠に回り続ける
/// </summary>
/// <typeparam name="T">整数型に限る</typeparam>
/// <param name="value">現在の値</param>
/// <param name="modulo">モジュロ値</param>
/// <returns>剰余加算を行った値</returns>
template<typename T>
inline T ModuloIncrement(const T& value, const T& modulo) {
	static_assert(std::is_integral<T>::value);
	return (value + 1) % modulo;
}
/// <summary>
/// divを法とした剰余デクリメント
/// デクリメントの結果が0未満にならないように、-1になりそうな場合、最大値になり
/// 永遠に回り続ける
/// </summary>
/// <typeparam name="T">整数型に限る</typeparam>
/// <param name="value">現在の値</param>
/// <param name="modulo">モジュロ値</param>
/// <returns>剰余減算を行った値</returns>
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
///符号だけを抜き出して返す
///ただし、0の時は1.0が返る
///</summary>
/// <param name="value">値</param>
/// <returns>符号</returns>
extern float Sign(float value);